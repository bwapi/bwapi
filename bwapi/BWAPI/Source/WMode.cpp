#include "WMode.h"
#include "Resolution.h"
#include "BW/Offsets.h"

#include "../../Debug.h"

WNDPROC wOriginalProc;
HWND ghMainWnd;
bool wmode;
HDC  hdcMem;
void* pBits;

RGBQUAD palette[256];

void InitializeWModeBitmap(int width, int height)
{
  if ( hdcMem )
    DeleteDC(hdcMem);

  // Create Bitmap HDC
  BITMAPINFO256 bmp = { 0 };
  HBITMAP    hBmp  = NULL;

  bmp.bmiHeader.biSize        = sizeof(BITMAPINFOHEADER);
  bmp.bmiHeader.biWidth       = width;
  bmp.bmiHeader.biHeight      = -(height);
  bmp.bmiHeader.biPlanes      = 1;
  bmp.bmiHeader.biBitCount    = 8;
  bmp.bmiHeader.biCompression = BI_RGB;
  for ( int i = 0; i < 256; ++i )
  {
    bmp.bmiColors[i].rgbRed       = BW::BWDATA_GamePalette[i].peRed;
    bmp.bmiColors[i].rgbGreen     = BW::BWDATA_GamePalette[i].peGreen;
    bmp.bmiColors[i].rgbBlue      = BW::BWDATA_GamePalette[i].peBlue;
    bmp.bmiColors[i].rgbReserved  = 0;
    palette[i].rgbRed             = BW::BWDATA_GamePalette[i].peRed;
    palette[i].rgbGreen           = BW::BWDATA_GamePalette[i].peGreen;
    palette[i].rgbBlue            = BW::BWDATA_GamePalette[i].peBlue;
  }

  hBmp = CreateDIBSection(NULL, (BITMAPINFO*)&bmp, DIB_RGB_COLORS, &pBits, NULL, 0);
  hdcMem = CreateCompatibleDC(NULL);
  SelectObject(hdcMem, hBmp);
}

void GetBorderRect(HWND hWnd, LPRECT lpRect)
{
  RECT clientRct;
  RECT windowRct;
  GetClientRect(hWnd, &clientRct);
  ClientToScreen(hWnd, (LPPOINT)&clientRct.left);
  ClientToScreen(hWnd, (LPPOINT)&clientRct.right);
  GetWindowRect(hWnd, &windowRct);
  if ( lpRect )
  {
    lpRect->left    = clientRct.left - windowRct.left;
    lpRect->top     = clientRct.top - windowRct.top;
    lpRect->right   = windowRct.right - clientRct.right;
    lpRect->bottom  = windowRct.bottom - clientRct.bottom;
  }
}

void GetBorderSize(HWND hWnd, LPSIZE lpSize)
{
  RECT diff;
  GetBorderRect(hWnd, &diff);
  if ( lpSize )
  {
    lpSize->cx = diff.left + diff.right;
    lpSize->cy = diff.top + diff.bottom;
  }
}

LRESULT CALLBACK WindowProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
  if ( wmode )
  {
    // Perform W-Mode only functionality
    switch ( uMsg )
    {
    case WM_SIZING:
      {
        SIZE border;
        GetBorderSize(hWnd, &border);

        int xLimit = 640 + border.cx;
        int yLimit = 480 + border.cy;

        RECT *rct = (RECT*)lParam;
        if ( rct->right - rct->left < xLimit )
          rct->right = rct->left + xLimit;
        if ( rct->bottom - rct->top < yLimit )
          rct->bottom = rct->top + yLimit;
        break;
      }
    case WM_PAINT:
      if ( BW::BWDATA_GameScreenBuffer->data && pBits )
      {
        // begin paint
        PAINTSTRUCT paint;
        BeginPaint(hWnd, &paint);

        // Copy the broodwar drawing buffer over
        memcpy(pBits, BW::BWDATA_GameScreenBuffer->data, BW::BWDATA_GameScreenBuffer->wid * BW::BWDATA_GameScreenBuffer->ht);

        // Blit to the screen
        BitBlt(paint.hdc, 0, 0, BW::BWDATA_GameScreenBuffer->wid, BW::BWDATA_GameScreenBuffer->ht, hdcMem, 0, 0, SRCCOPY);

        // end paint
        EndPaint(hWnd, &paint);
      } // data
      break;
    case WM_MOUSEMOVE:
      if ((wParam & MK_LBUTTON) > 0)
      {
        RECT clientRct;
        GetClientRect(hWnd, &clientRct);

        ClientToScreen(hWnd, (LPPOINT)&clientRct.left);
        ClientToScreen(hWnd, (LPPOINT)&clientRct.right);

        ClipCursor(&clientRct);
      }
      else
      {
        ClipCursor(NULL); 
      }
      break;
    case WM_LBUTTONDOWN:
    case WM_LBUTTONUP:
    case WM_MOUSEWHEEL:
    case WM_RBUTTONDOWN:
    case WM_RBUTTONUP:
    case WM_RBUTTONDBLCLK:
    case WM_LBUTTONDBLCLK:
    case WM_MBUTTONDOWN:
    case WM_MBUTTONUP:
    case WM_MBUTTONDBLCLK:
      {
        RECT rct;
        GetClientRect(hWnd, &rct);

        POINTS pt = MAKEPOINTS(lParam);
        if ( pt.x <= rct.left )
          pt.x = (SHORT)rct.left+1;
        if ( pt.x >= rct.right )
          pt.x = (SHORT)rct.right-1;
        if ( pt.y <= rct.top )
          pt.y = (SHORT)rct.top+1;
        if ( pt.y >= rct.bottom )
          pt.y = (SHORT)rct.bottom-1;
        lParam = MAKELPARAM(pt.x, pt.y);
        break;
      }
    case WM_ACTIVATEAPP:
      if ( wOriginalProc )
        return wOriginalProc(hWnd, WM_ACTIVATEAPP, (WPARAM)1, NULL);
    case WM_SETCURSOR:
    case WM_ERASEBKGND:
      return DefWindowProc(hWnd, uMsg, wParam, lParam);
    }
  }

  // Perform BWAPI-added functionality
  switch ( uMsg )
  {
  case WM_SYSKEYDOWN:
    if ( wParam == VK_RETURN && (lParam & 0x20000000) && !(lParam & 0x40000000) )
    {
      ToggleWMode(640, 480);
      return TRUE;
    }
    break;
  case WM_SYSCOMMAND:
    if ( wParam == SC_MAXIMIZE )
    {
      ToggleWMode(640, 480);
      return TRUE;
    }
  }

  // Call the original WndProc
  if ( wOriginalProc )
    return wOriginalProc(hWnd, uMsg, wParam, lParam);
  return DefWindowProc(hWnd, uMsg, wParam, lParam);
}

BOOL WINAPI _GetCursorPos(LPPOINT lpPoint)
{
  if ( lpPoint )
  {
    if ( !wmode )
      return GetCursorPos(lpPoint);

    lpPoint->x = 320;
    lpPoint->y = 240;
    return TRUE;
  }
  return FALSE;
}

BOOL WINAPI _SetCursorPos(int X, int Y)
{
  if ( !wmode )
    return SetCursorPos(X, Y);
  return TRUE;
}

BOOL WINAPI _ClipCursor(const RECT *lpRect)
{
  if ( !wmode )
    return ClipCursor(lpRect);
  return TRUE;
}

BOOL __stdcall _SDrawLockSurface(int surfacenumber, RECT *lpDestRect, void **lplpSurface, int *lpPitch, int arg_unused)
{
  if ( !wmode )
    return SDrawLockSurface(surfacenumber, lpDestRect, lplpSurface, lpPitch, arg_unused);

  if ( lplpSurface )
    *lplpSurface = BW::BWDATA_GameScreenBuffer->data;
  if ( lpPitch )
    *lpPitch = 640;
  return TRUE;
}

BOOL __stdcall _SDrawUnlockSurface(int surfacenumber, void *lpSurface, int a3, RECT *lpRect)
{
  if ( !wmode )
    return SDrawUnlockSurface(surfacenumber, lpSurface, a3, lpRect);

  return TRUE;
}

BOOL __stdcall _SDrawUpdatePalette(unsigned int firstentry, unsigned int numentries, PALETTEENTRY *pPalEntries, int a4)
{
  if ( !wmode || !ghMainWnd )
    return SDrawUpdatePalette(firstentry, numentries, pPalEntries, a4);

  for ( unsigned int i = firstentry; i < firstentry + numentries; ++i )
  {
    palette[i].rgbRed   = pPalEntries[i].peRed;
    palette[i].rgbGreen = pPalEntries[i].peGreen;
    palette[i].rgbBlue  = pPalEntries[i].peBlue;
  }

  if ( !IsIconic(ghMainWnd) )
    _SDrawRealizePalette();
  return TRUE;
}

BOOL __stdcall _SDrawRealizePalette()
{
  if ( !wmode || !ghMainWnd )
    return SDrawRealizePalette();

  if ( IsIconic(ghMainWnd) )
    return FALSE;

  SetDIBColorTable(hdcMem, 0, 256, palette);
  return TRUE;
}

void ToggleWMode(int width, int height)
{
  if ( !wmode )
  {
    wmode = true;
    if ( !ghMainWnd )
      return;

    // Call the DirectDraw destructor
    DDrawDestroy();
    InitializeWModeBitmap(width, height);

    // Hack to enable drawing in Broodwar
    *BW::BWDATA_PrimarySurface = (LPDIRECTDRAWSURFACE)1;

    // Change the window settings
    SetWindowLong(ghMainWnd, GWL_STYLE, WS_OVERLAPPEDWINDOW);
    SetWindowPos(ghMainWnd, HWND_NOTOPMOST, 0, 0, width, height, SWP_SHOWWINDOW);
    ShowWindow(ghMainWnd, SW_RESTORE);

    SIZE border;
    GetBorderSize(ghMainWnd, &border);
    int w = width + border.cx;
    int h = height + border.cy;
    MoveWindow(ghMainWnd, 0, 0, w, h, TRUE);
    HCURSOR cur = LoadCursor(NULL, IDC_ARROW);
    SetCursor(cur);
    ShowCursor(TRUE);
  }
  else
  {
    wmode = false;
    *BW::BWDATA_PrimarySurface = NULL;
    DeleteDC(hdcMem);
    hdcMem = NULL;

    SetWindowLong(ghMainWnd, GWL_STYLE, WS_POPUP | WS_VISIBLE | WS_SYSMENU);
    SetWindowPos(ghMainWnd, HWND_TOPMOST, 0, 0, GetSystemMetrics(SM_CXSCREEN), GetSystemMetrics(SM_CYSCREEN), SWP_SHOWWINDOW);
    SetCursor(NULL);
    ShowCursor(FALSE);
    SetFocus(ghMainWnd);

    DDrawInitialize(width, height);
  }
}
