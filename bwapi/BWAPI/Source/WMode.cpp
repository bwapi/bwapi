#include "WMode.h"

WNDPROC wOriginalProc;
HWND ghMainWnd;
HDC  hdcMem;
void* pBits;

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
    {
      if ( BW::BWDATA_GameScreenBuffer->data )
      {
        if ( pBits )
        {
          // begin paint
          PAINTSTRUCT paint;
          BeginPaint(hWnd, &paint);

          RGBQUAD newPalette[256];
          for ( int i = 0; i < 256; ++i )
          {
            newPalette[i].rgbRed       = BW::BWDATA_GamePalette[i].peRed;
            newPalette[i].rgbGreen     = BW::BWDATA_GamePalette[i].peGreen;
            newPalette[i].rgbBlue      = BW::BWDATA_GamePalette[i].peBlue;
            newPalette[i].rgbReserved  = 0;
          }
          SetDIBColorTable(hdcMem, 0, 256, newPalette);

          // Copy the broodwar drawing buffer over
          memcpy(pBits, BW::BWDATA_GameScreenBuffer->data, 640*480);

          // Blit to the screen
          BitBlt(paint.hdc, 0, 0, 640, 480, hdcMem, 0, 0, SRCCOPY);

          // end paint
          EndPaint(hWnd, &paint);
        }
      }
    }
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
  if ( wOriginalProc )
    return wOriginalProc(hWnd, uMsg, wParam, lParam);
  return false;
}

BOOL WINAPI _GetCursorPos(LPPOINT lpPoint)
{
  BOOL rval = false;
  if ( lpPoint )
  {
    rval = GetCursorPos(lpPoint);
    if ( ghMainWnd )
    {
      ScreenToClient(ghMainWnd, lpPoint);
      if ( lpPoint->x < 0 )
        lpPoint->x = 0;
      if ( lpPoint->x > 640 )
        lpPoint->x = 640;
      if ( lpPoint->y < 0 )
        lpPoint->y = 0;
      if ( lpPoint->y > 480 )
        lpPoint->y = 480;
    }
  }
  return rval;
}

BOOL WINAPI _SetCursorPos(int X, int Y)
{
  POINT pt = { X, Y };
  if ( ghMainWnd )
  {
    POINT curPos;
    GetCursorPos(&curPos);
    ScreenToClient(ghMainWnd, &curPos);
    if ( curPos.x < 0 || curPos.x > 640 || curPos.y < 0 || curPos.y > 480 )
      return TRUE;
    ClientToScreen(ghMainWnd, &pt);
  }
  return SetCursorPos(pt.x, pt.y);
}

BOOL WINAPI _ClipCursor(const RECT *lpRect)
{
  // BAHAHAH too bad sucker
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