#include "WMode.h"
#include "BW/Offsets.h"

WNDPROC wOriginalProc;
HWND ghMainWnd;
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
  if ( ghMainWnd )
  {
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
  if ( wOriginalProc )
    return wOriginalProc(hWnd, uMsg, wParam, lParam);
  return DefWindowProc(hWnd, uMsg, wParam, lParam);
}

BOOL WINAPI _GetCursorPos(LPPOINT lpPoint)
{
  if ( lpPoint )
  {
    if ( !ghMainWnd )
      return GetCursorPos(lpPoint);

    lpPoint->x = 320;
    lpPoint->y = 240;
    return TRUE;
  }
  return FALSE;
}

BOOL WINAPI _SetCursorPos(int X, int Y)
{
  POINT pt = { X, Y };
  if ( ghMainWnd )
  {
    POINT curPos;
    GetCursorPos(&curPos);
    ScreenToClient(ghMainWnd, &curPos);
    if ( curPos.x < 0 || curPos.x > BW::BWDATA_GameScreenBuffer->wid || curPos.y < 0 || curPos.y > BW::BWDATA_GameScreenBuffer->ht )
      return TRUE;
    ClientToScreen(ghMainWnd, &pt);
  }
  return SetCursorPos(pt.x, pt.y);
}

BOOL WINAPI _ClipCursor(const RECT *lpRect)
{
  if ( !ghMainWnd )
    return ClipCursor(lpRect);

  return TRUE;
}

BOOL __stdcall _SDrawLockSurface(int surfacenumber, RECT *lpDestRect, void **lplpSurface, int *lpPitch, int arg_unused)
{
  if ( !ghMainWnd )
    return SDrawLockSurface(surfacenumber, lpDestRect, lplpSurface, lpPitch, arg_unused);

  if ( lplpSurface )
    *lplpSurface = BW::BWDATA_GameScreenBuffer->data;
  if ( lpPitch )
    *lpPitch = 640;
  return TRUE;
}

BOOL __stdcall _SDrawUnlockSurface(int surfacenumber, void *lpSurface, int a3, RECT *lpRect)
{
  if ( !ghMainWnd )
    return SDrawUnlockSurface(surfacenumber, lpSurface, a3, lpRect);

  return TRUE;
}

BOOL __stdcall _SDrawUpdatePalette(unsigned int firstentry, unsigned int numentries, PALETTEENTRY *pPalEntries, int a4)
{
  if ( !ghMainWnd )
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
  if ( !ghMainWnd )
    return SDrawRealizePalette();

  if ( IsIconic(ghMainWnd) )
    return FALSE;

  SetDIBColorTable(hdcMem, 0, 256, palette);
  return TRUE;
}

