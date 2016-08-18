// CubeFighter.cpp : Defines the entry point for the application.
//

#include "stdafx.h"
#include "CubeFighter.h"

// Definitions
#define MAX_LOADSTRING 100
#define WW 320
#define WH 180

// Types/structures
struct BackBuffer {
  HDC backDC;
  HGDIOBJ oldObj;
  HBITMAP imgObj;
  RECT clientRect;

  BackBuffer() : backDC(nullptr), oldObj(nullptr), imgObj(nullptr) {
    clientRect.left = clientRect.right = 0;
    clientRect.bottom = clientRect.top = 0;
  }
};

// Global Variables:
HINSTANCE hInst;                            // current instance
CHAR szTitle[MAX_LOADSTRING];               // the title bar text
CHAR szWindowClass[MAX_LOADSTRING];         // the main window class name
detail::QuadRenderer *g_renderer = nullptr; // the application renderer
HWND g_window;                              // the application window
HDC g_imgDC;                                // the DC which holds the image data
HANDLE g_thread;                            // the main render threa
BackBuffer g_backBuffer;                    // the back buffer

// Forward declarations of functions included in this code module:
ATOM MyRegisterClass(HINSTANCE hInstance);
HWND InitInstance(HINSTANCE, int);
LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);
DWORD WINAPI RenderLoop(LPVOID);
void PaintWindow(std::vector<structures::textInfo> &info);

int APIENTRY WinMain(_In_ HINSTANCE hInstance, _In_opt_ HINSTANCE hPrevInstance,
                     _In_ LPSTR lpCmdLine, _In_ int nCmdShow) {
  UNREFERENCED_PARAMETER(hPrevInstance);
  UNREFERENCED_PARAMETER(lpCmdLine);

  MSG msg;
  HACCEL hAccelTable;

  // Initialize global strings
  LoadString(hInstance, IDS_APP_TITLE, szTitle, MAX_LOADSTRING);
  LoadString(hInstance, IDC_CUBEFIGHTER, szWindowClass, MAX_LOADSTRING);
  MyRegisterClass(hInstance);

  // Perform application initialization:
  g_window = InitInstance(hInstance, nCmdShow);
  if (!g_window)
    return FALSE;

  hAccelTable = LoadAccelerators(hInstance, MAKEINTRESOURCE(IDC_CUBEFIGHTER));

  // Create the renderer - grab our internal bitmap and window surface.
  g_renderer = new detail::QuadRenderer();

  HDC windowDC = GetWindowDC(g_window);
  g_imgDC = CreateCompatibleDC(windowDC);
  g_backBuffer.backDC = CreateCompatibleDC(windowDC);
  ReleaseDC(g_window, windowDC);

  if (g_imgDC == NULL) {
    OutputDebugString("Dc is NULL");
    return 0;
  }

  SetBkMode(g_imgDC, TRANSPARENT);
  SetTextColor(g_imgDC, RGB(255, 255, 255));
  SetStretchBltMode(g_imgDC, COLORONCOLOR);

  BITMAPINFO bf;
  ZeroMemory(&bf, sizeof(BITMAPINFO));

  bf.bmiHeader.biSize = sizeof(BITMAPINFOHEADER);
  bf.bmiHeader.biWidth = WW;
  bf.bmiHeader.biHeight = -WH;
  bf.bmiHeader.biPlanes = 1;
  bf.bmiHeader.biBitCount = 32;
  bf.bmiHeader.biCompression = BI_RGB;
  bf.bmiHeader.biSizeImage = (WW * WH * (32 / 8));
  bf.bmiHeader.biXPelsPerMeter = -1;
  bf.bmiHeader.biYPelsPerMeter = -1;

  unsigned char *bits;

  HBITMAP hImg =
      CreateDIBSection(g_imgDC, &bf, DIB_RGB_COLORS, (void **)&bits, NULL, 0);
  if (hImg == NULL) {
    OutputDebugString("Image is NULL");
    return 0;
  } else if (hImg == INVALID_HANDLE_VALUE) {
    OutputDebugString("Image is invalid");
    return 0;
  }

  HGDIOBJ oldObj = SelectObject(g_imgDC, hImg);

  // We can only set the paint callback once the DC is populated since setting
  // the callback triggers a paint call.
  g_renderer->SetRenderCallback(PaintWindow);
  g_renderer->SetRenderInfo(reinterpret_cast<unsigned int *>(bits), WW, WH);
  g_renderer->Start();

  // Main message loop:
  while (GetMessage(&msg, NULL, 0, 0)) {
    if (!TranslateAccelerator(msg.hwnd, hAccelTable, &msg)) {
      TranslateMessage(&msg);
      DispatchMessage(&msg);
    }
  }

  SelectObject(g_imgDC, oldObj);
  DeleteDC(g_imgDC);
  SelectObject(g_backBuffer.backDC, g_backBuffer.oldObj);
  DeleteDC(g_backBuffer.backDC);
  DeleteObject(g_backBuffer.imgObj);

  return (int)msg.wParam;
}

//
//  FUNCTION: MyRegisterClass()
//
//  PURPOSE: Registers the window class.
//
ATOM MyRegisterClass(HINSTANCE hInstance) {
  WNDCLASSEX wcex;

  wcex.cbSize = sizeof(WNDCLASSEX);

  wcex.style = 0;
  wcex.lpfnWndProc = WndProc;
  wcex.cbClsExtra = 0;
  wcex.cbWndExtra = 0;
  wcex.hInstance = hInstance;
  wcex.hIcon = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_CUBEFIGHTER));
  wcex.hCursor = LoadCursor(NULL, IDC_ARROW);
  wcex.hbrBackground = 0;
  wcex.lpszMenuName = 0;
  wcex.lpszClassName = szWindowClass;
  wcex.hIconSm = LoadIcon(wcex.hInstance, MAKEINTRESOURCE(IDI_SMALL));

  return RegisterClassEx(&wcex);
}

//
//   FUNCTION: InitInstance(HINSTANCE, int)
//
//   PURPOSE: Saves instance handle and creates main window
//
//   COMMENTS:
//
//        In this function, we save the instance handle in a global variable and
//        create and display the main program window.
//
HWND InitInstance(HINSTANCE hInstance, int nCmdShow) {
  HWND hWnd;

  hInst = hInstance; // Store instance handle in our global variable

  hWnd = CreateWindow(szWindowClass, szTitle,
                      WS_OVERLAPPEDWINDOW | WS_CLIPCHILDREN, CW_USEDEFAULT, 0,
                      CW_USEDEFAULT, 0, NULL, NULL, hInstance, NULL);

  if (!hWnd) {
    return NULL;
  }

  ShowWindow(hWnd, nCmdShow);
  UpdateWindow(hWnd);

  return hWnd;
}

#if LEGACY
//  FUNCTION: HandleKey(WPARAM, bool)
//
//  PURPOSE: Handle keystrokes.
void HandleKey(WPARAM wParam, bool pressed) {
  switch (wParam) {
  case VK_ESCAPE:
    if (g_renderer)
      g_renderer->Stop();
    PostQuitMessage(0);
    break;
  default:
    if (g_renderer)
      g_renderer->HandleKey(static_cast<unsigned int>(wParam), pressed);
    break;
  }
}
#endif // LEGACY

//  FUNCTION: EnsureBackBuffer(HDC, RECT)
//
//  PURPOSE:  Ensure the back dc and back buffer is valid.
void EnsureBackBuffer(HDC dc, RECT &rect) {
  if (g_backBuffer.clientRect.left != rect.left ||
      g_backBuffer.clientRect.top != rect.top ||
      g_backBuffer.clientRect.bottom != rect.bottom ||
      g_backBuffer.clientRect.right != rect.right) {
    // The Rectangles have changed ...
    if (g_backBuffer.imgObj != nullptr) {
      SelectObject(g_backBuffer.backDC, g_backBuffer.oldObj);
      DeleteObject(g_backBuffer.imgObj);
    }

    // Create the new image.
    g_backBuffer.imgObj = CreateCompatibleBitmap(dc, rect.right - rect.left,
                                                 rect.bottom - rect.top);
    g_backBuffer.oldObj =
        SelectObject(g_backBuffer.backDC, g_backBuffer.imgObj);

    // Copy the rectangle information
    g_backBuffer.clientRect = rect;
  }
}

//  FUNCTION: PaintWindow(void)
//
//  PURPOSE:  Paint the window surface.
void PaintWindow(std::vector<structures::textInfo> &text) {
  if (!g_window)
    return;

  HDC paintDC = GetDC(g_window);
  if (!paintDC)
    return;

  RECT rect;
  GetClientRect(g_window, &rect);

  EnsureBackBuffer(paintDC, rect);

  // Stretch the image to fill the entire window.
  FillRect(g_backBuffer.backDC, &rect,
           static_cast<HBRUSH>(GetStockObject(BLACK_BRUSH)));

  int width = rect.right - rect.left;
  int height = rect.bottom - rect.top;
  float proportionToX = static_cast<float>(WH) / static_cast<float>(WW);
  float proportionToY = static_cast<float>(WW) / static_cast<float>(WH);

  int newWidth = static_cast<int>(static_cast<float>(height) * proportionToY);
  int newHeight = height;

  if (newWidth > width) {
    newWidth = width;
    newHeight = static_cast<int>(static_cast<float>(width) * proportionToX);
  }

  int remainderX = width - newWidth;
  int remainderY = height - newHeight;
  int xpos = (remainderX >> 1);
  int ypos = (remainderY >> 1);
  StretchBlt(g_backBuffer.backDC, xpos, ypos, newWidth, newHeight, g_imgDC, 0,
             0, WW, WH, SRCCOPY);

  BitBlt(paintDC, 0, 0, rect.right - rect.left, rect.bottom - rect.top,
         g_backBuffer.backDC, 0, 0, SRCCOPY);

  // Walk through the text info list and push that data onto the screen.

  ReleaseDC(g_window, paintDC);
}

//  FUNCTION: WndProc(HWND, UINT, WPARAM, LPARAM)
//
//  PURPOSE:  Processes messages for the main window.
LRESULT CALLBACK
WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam) {
  switch (message) {
  case WM_DESTROY:
    if (g_renderer)
      g_renderer->Stop();

    PostQuitMessage(0);
    return 0L;
  case WM_ERASEBKGND:
    return 1;
    break;
#if LEGACY
  case WM_KEYDOWN:
    HandleKey(wParam, true);
    return 0L;
  case WM_KEYUP:
    HandleKey(wParam, false);
    return 0L;
  case WM_MOUSEMOVE:
    if (g_renderer)
      g_renderer->SetMouse(LOWORD(lParam), HIWORD(lParam), false);
    return 0L;
  case WM_LBUTTONDOWN:
  case WM_RBUTTONDOWN:
  case WM_MBUTTONDOWN:
    if (g_renderer)
      g_renderer->SetMouse(LOWORD(lParam), HIWORD(lParam), true);
    return 0L;
  case WM_MBUTTONUP:
    if (g_renderer)
      g_renderer->SetMouse(LOWORD(lParam), HIWORD(lParam), false);
    return 0L;
#endif // LEGACY
  default:
    return DefWindowProc(hWnd, message, wParam, lParam);
  }
}

DWORD WINAPI RenderLoop(LPVOID ren) {
  detail::QuadRenderer *renderer{reinterpret_cast<detail::QuadRenderer *>(ren)};
  structures::InputState state;
  std::vector<structures::textInfo> info;

  // Get the initial input state, then every 30 frames after that.
  int counter = 0;
  renderer->GetInputState(state);

  while (renderer->getRunning()) {
    if (++counter > 6) {
      renderer->GetInputState(state);
      counter = 0;
    }

    renderer->Update(state);
    renderer->GetOutput(info);

    // Trigger a paint call.
    renderer->Paint(info);

    // Rest a bit to keep the CPU usage down.
    Sleep(10);
  }
  return 0;
}

// Since this is in the windows namespace, we can call win functions on them.
namespace detail {
void QuadRenderer::GetInputState(structures::InputState &state) {
  // We ignore mouse input for now.
  state.kpressed[0] = !!(GetAsyncKeyState(VK_LEFT) & 0x8000);
  state.kpressed[1] = !!(GetAsyncKeyState(VK_UP) & 0x8000);
  state.kpressed[2] = !!(GetAsyncKeyState(VK_RIGHT) & 0x8000);
  state.kpressed[3] = !!(GetAsyncKeyState(VK_DOWN) & 0x8000);
  state.kpressed[4] = !!(GetAsyncKeyState(VK_SPACE) & 0x8000);
  state.kpressed[5] = !!(GetAsyncKeyState(VK_RETURN) & 0x8000);
  state.kpressed[6] = !!(GetAsyncKeyState(VK_ESCAPE) & 0x8000);
}

void QuadRenderer::Start() {
  running = true;
  g_thread = CreateThread(NULL, 0, RenderLoop, this, 0, NULL);
}

void QuadRenderer::Stop() {
  running = false;
  WaitForSingleObject(g_thread, INFINITE);
  CloseHandle(g_thread);
}
}