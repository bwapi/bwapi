#include "WMode.h"
#include "Resolution.h"
#include "BW/Offsets.h"

#include "../../Debug.h"

WNDPROC wOriginalProc;
HWND ghMainWnd;
bool wmode;
HDC  hdcMem;
void* pBits;
bool gbWantUpdate     = false;
bool gbIsCursorHidden = true;
bool gbHoldingAlt     = false;

RGBQUAD palette[256];

void InitializeWModeBitmap(int width, int height)
{
  if ( hdcMem )
    DeleteDC(hdcMem);

  // Create Bitmap HDC
  BITMAPINFO256 bmp = { 0 };
  HBITMAP      hBmp = NULL;

  bmp.bmiHeader.biSize        = sizeof(BITMAPINFOHEADER);
  bmp.bmiHeader.biWidth       = width;
  bmp.bmiHeader.biHeight      = -(height);
  bmp.bmiHeader.biPlanes      = 1;
  bmp.bmiHeader.biBitCount    = 8;
  bmp.bmiHeader.biCompression = BI_RGB;
  for ( int i = 0; i < 256; ++i )
  {
    palette[i].rgbRed   = BW::BWDATA_GamePalette[i].peRed;
    palette[i].rgbGreen = BW::BWDATA_GamePalette[i].peGreen;
    palette[i].rgbBlue  = BW::BWDATA_GamePalette[i].peBlue;
  }
  hBmp   = CreateDIBSection(NULL, (BITMAPINFO*)&bmp, DIB_RGB_COLORS, &pBits, NULL, 0);
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
    lpRect->left    = clientRct.left   - windowRct.left;
    lpRect->top     = clientRct.top    - windowRct.top;
    lpRect->right   = windowRct.right  - clientRct.right;
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

LPARAM FixPoints(LPARAM lParam)
{
  RECT clientRct;
  GetClientRect(ghMainWnd, &clientRct);

  POINTS pt = MAKEPOINTS(lParam);
  if ( clientRct.right != BW::BWDATA_GameScreenBuffer->wid )
    pt.x = (SHORT)((float)pt.x * ((float)BW::BWDATA_GameScreenBuffer->wid / (float)clientRct.right));
  if ( clientRct.bottom != BW::BWDATA_GameScreenBuffer->ht )
    pt.y = (SHORT)((float)pt.y * ((float)BW::BWDATA_GameScreenBuffer->ht  / (float)clientRct.bottom));
  return MAKELPARAM(pt.x, pt.y);
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

        int xLimit = BW::BWDATA_GameScreenBuffer->wid + border.cx;
        int yLimit = BW::BWDATA_GameScreenBuffer->ht  + border.cy;

        RECT *rct = (RECT*)lParam;
        if ( rct->right - rct->left < xLimit )
          rct->right = rct->left + xLimit;
        if ( rct->bottom - rct->top < yLimit )
          rct->bottom = rct->top + yLimit;
        break;
      }
    case WM_PAINT:
      if ( gbWantUpdate && pBits )
      {
        // begin paint
        PAINTSTRUCT paint;
        BeginPaint(hWnd, &paint);

        // Blit to the screen
        SetStretchBltMode(paint.hdc, HALFTONE);
        RECT cRect;
        GetClientRect(hWnd, &cRect);
        StretchBlt(paint.hdc, cRect.left, cRect.top, cRect.right, cRect.bottom, hdcMem, 0, 0, BW::BWDATA_GameScreenBuffer->wid, BW::BWDATA_GameScreenBuffer->ht, SRCCOPY);

        // end paint
        EndPaint(hWnd, &paint);
      } // data
      break;
    case WM_NCMOUSEMOVE:
      SetCursorShowState(true);
      break;
    case WM_MOUSEMOVE:
      SetCursorShowState(false);
      lParam = FixPoints(lParam);
      break;
    case WM_LBUTTONDOWN:
    case WM_RBUTTONDOWN:
    case WM_MBUTTONDOWN:
      {
        RECT clientRct;
        GetClientRect(hWnd, &clientRct);
        ClientToScreen(hWnd, (LPPOINT)&clientRct.left);
        ClientToScreen(hWnd, (LPPOINT)&clientRct.right);
        ClipCursor(&clientRct);
        lParam = FixPoints(lParam);
        break;
      }
    case WM_MBUTTONUP:
    case WM_LBUTTONUP:
    case WM_RBUTTONUP:
      ClipCursor(NULL);
      lParam = FixPoints(lParam);
      break;
    case WM_MOUSEWHEEL:
    case WM_RBUTTONDBLCLK:
    case WM_LBUTTONDBLCLK:
    case WM_MBUTTONDBLCLK:
      lParam = FixPoints(lParam);
      break;
    case WM_ACTIVATEAPP:
      if ( wOriginalProc )
        return wOriginalProc(hWnd, WM_ACTIVATEAPP, (WPARAM)1, NULL);
    case WM_SETCURSOR:
    case WM_ERASEBKGND:
      return DefWindowProc(hWnd, uMsg, wParam, lParam);
    case WM_SYSKEYDOWN:
    case WM_KEYDOWN:
      if ( wParam == VK_MENU && !(lParam & 0x40000000))
      {
        RECT rct;
        GetClientRect(hWnd, &rct);
        ClientToScreen(hWnd, (LPPOINT)&rct.left);
        ClientToScreen(hWnd, (LPPOINT)&rct.right);
        ClipCursor(&rct);
        gbHoldingAlt = true;
      }
      break;
    case WM_SYSKEYUP:
    case WM_KEYUP:
      if ( wParam == VK_MENU )
      {
        ClipCursor(NULL);
        gbHoldingAlt = false;
      }
      break;
    } // switch
  } // if wmode

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
#ifdef _DEBUG
  case WM_KEYDOWN:
    if ( wParam == VK_F6 && !(lParam & 0x40000000) )
    {
      strcpy(BW::BWDATA_CurrentMapFileName, "C:\\Program Files\\Starcraft\\maps\\(2)Challenger.scm");
      *BW::BWDATA_gwGameMode     = 1;
      *BW::BWDATA_gwNextGameMode = 1;
      *BW::BWDATA_GameState      = 0;
      *BW::BWDATA_CampaignIndex  = 0;
      *BW::BWDATA_OpheliaEnabled = 1;
      if ( !BW::FindDialogGlobal("Minimap") && (*BW::BWDATA_DialogList) )
        (*BW::BWDATA_DialogList)->activate();
      return TRUE;
    }
    break;
#endif
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
  if ( !lpPoint )
    return FALSE;

  if ( !wmode )
    return GetCursorPos(lpPoint);

  if ( !gbHoldingAlt )
  {
    lpPoint->x = 320;
    lpPoint->y = 240;
  }
  else
  {
    POINT tempPoint;
    GetCursorPos(&tempPoint);
    ScreenToClient(ghMainWnd, &tempPoint);

    LPARAM lConvert = FixPoints(MAKELPARAM(tempPoint.x, tempPoint.y));
    POINTS final = MAKEPOINTS(lConvert);
    lpPoint->x = final.x;
    lpPoint->y = final.y;
  }
  return TRUE;
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
    *lplpSurface = pBits;
  if ( lpPitch )
    *lpPitch = 640;
  return TRUE;
}

BOOL __stdcall _SDrawUnlockSurface(int surfacenumber, void *lpSurface, int a3, RECT *lpRect)
{
  if ( !wmode )
    return SDrawUnlockSurface(surfacenumber, lpSurface, a3, lpRect);

  gbWantUpdate = true;
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
    SetCursor(NULL);
    SetCursorShowState(false);
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
    SetCursorShowState(false);
    SetFocus(ghMainWnd);

    DDrawInitialize(width, height);
  }
}

void SetCursorShowState(bool bShow)
{
  if ( bShow == gbIsCursorHidden )
  {
    ShowCursor(bShow);
    gbIsCursorHidden = !bShow;
  }
}
