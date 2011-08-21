#include "WMode.h"
#include "Resolution.h"
#include "BW/Offsets.h"

#include "BWAPI/GameImpl.h"

#include "DLLMain.h"
#include "../../Debug.h"

#include "Recording.h"

WNDPROC wOriginalProc;
HWND ghMainWnd;
bool wmode;
HDC  hdcMem;
void* pBits;
bool gbWantUpdate     = false;
bool gbIsCursorHidden = true;
bool gbHoldingAlt     = false;

bool switchToWMode = false;
RECT windowRect    = { 0, 0, 640, 480 };

BOOL (WINAPI   *_GetCursorPosOld)(LPPOINT lpPoint);
BOOL (WINAPI   *_SetCursorPosOld)(int X, int Y);
BOOL (WINAPI   *_ClipCursorOld)(const RECT *lpRect);
BOOL (STORMAPI *_SDrawLockSurfaceOld)(int surfacenumber, RECT *lpDestRect, void **lplpSurface, int *lpPitch, int arg_unused);
BOOL (STORMAPI *_SDrawUnlockSurfaceOld)(int surfacenumber, void *lpSurface, int a3, RECT *lpRect);
BOOL (STORMAPI *_SDrawUpdatePaletteOld)(unsigned int firstentry, unsigned int numentries, PALETTEENTRY *pPalEntries, int a4);
BOOL (STORMAPI *_SDrawRealizePaletteOld)();

BITMAPINFO256 bmp;
void InitializeWModeBitmap(int width, int height)
{
  if ( hdcMem )
    DeleteDC(hdcMem);
  hdcMem = NULL;

  // Create Bitmap HDC
  memset(&bmp, 0, sizeof(BITMAPINFO256));
  bmp.bmiHeader.biSize          = sizeof(BITMAPINFOHEADER);
  bmp.bmiHeader.biWidth         = width;
  bmp.bmiHeader.biHeight        = -(height);
  bmp.bmiHeader.biPlanes        = 1;
  bmp.bmiHeader.biBitCount      = 8;
  bmp.bmiHeader.biCompression   = BI_RGB;
  bmp.bmiHeader.biSizeImage     = width * height;
  for ( int i = 0; i < 256; ++i )
  {
    bmp.bmiColors[i].rgbRed   = BW::BWDATA_GamePalette[i].peRed;
    bmp.bmiColors[i].rgbGreen = BW::BWDATA_GamePalette[i].peGreen;
    bmp.bmiColors[i].rgbBlue  = BW::BWDATA_GamePalette[i].peBlue;
  }
  HDC     hdc   = GetDC(ghMainWnd);
  HBITMAP hBmp  = CreateDIBSection(hdc, (BITMAPINFO*)&bmp, DIB_RGB_COLORS, &pBits, NULL, 0);
  hdcMem        = CreateCompatibleDC(hdc);
  ReleaseDC(ghMainWnd, hdc);
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

bool SendHotkey(BW::dlgEvent *pEvent)
{
  BW::dialog *dlg = BW::BWDATA_EventDialogs[pEvent->wNo];
  if ( dlg && dlg->pfcnInteract(dlg, pEvent) )
    return true;

  dlg = (*BW::BWDATA_DialogList);
  while ( dlg )
  {
    if ( dlg->pfcnInteract(dlg, pEvent) )
      return true;
    dlg = dlg->next();
  }
  return false;
}

void ButtonEvent(DWORD dwEvent, LPARAM lParam)
{
  BYTE bFlag = 0;
  switch( dwEvent )
  {
  case BW_LBUTTONDOWN:
  case BW_LBUTTONUP:
  case BW_LBUTTONDBLCLK:
    bFlag = 0x02;
    break;
  case BW_RBUTTONDOWN:
  case BW_RBUTTONUP:
  case BW_RBUTTONDBLCLK:
    bFlag = 0x08;
    break;
  case BW_MBUTTONDOWN:
  case BW_MBUTTONUP:
  case BW_MBUTTONDBLCLK:
    bFlag = 0x20;
    break;
  }
  if ( !( (*BW::BWDATA_InputFlags) & ~bFlag & 0x2A) )
  {
    switch( dwEvent )
    {
    case BW_LBUTTONDOWN:
    case BW_RBUTTONDOWN:
    case BW_MBUTTONDOWN:
    case BW_LBUTTONDBLCLK:
    case BW_RBUTTONDBLCLK:
    case BW_MBUTTONDBLCLK:
      *BW::BWDATA_InputFlags |= bFlag;
      SetCapture(ghMainWnd);
      break;
    case BW_LBUTTONUP:
    case BW_RBUTTONUP:
    case BW_MBUTTONUP:
      *BW::BWDATA_InputFlags &= ~bFlag;
      ReleaseCapture();
      break;
    }
    POINTS pt = MAKEPOINTS(lParam);
    BW::dlgEvent evt = { 0 };
    
    evt.wNo = (WORD)dwEvent;
    evt.cursor.x = pt.x;
    evt.cursor.y = pt.y;
    BW::BWDATA_Mouse->x = pt.x;
    BW::BWDATA_Mouse->y = pt.y;
    if ( !SendHotkey(&evt) && BW::BWDATA_InputProcs[dwEvent] )
      BW::BWDATA_InputProcs[dwEvent](&evt);
  }
}

void CorrectWindowWidth(int type, SIZE *ws, RECT *rct, SIZE *border)
{
  switch ( type )
  {
    case WMSZ_LEFT:
    case WMSZ_TOPLEFT:
    case WMSZ_BOTTOMLEFT:
      rct->left = rct->right - ws->cx - border->cx;
      break;
    case WMSZ_RIGHT:
    case WMSZ_TOPRIGHT:
    case WMSZ_BOTTOMRIGHT:
      rct->right = rct->left + ws->cx + border->cx;
      break;
  }
}
void CorrectWindowHeight(int type, SIZE *ws, RECT *rct, SIZE *border)
{
  switch ( type )
  {
    case WMSZ_TOP:
    case WMSZ_TOPLEFT:
    case WMSZ_TOPRIGHT:
      rct->top = rct->bottom - ws->cy - border->cy;
      break;
    case WMSZ_BOTTOM:
    case WMSZ_BOTTOMRIGHT:
    case WMSZ_BOTTOMLEFT:
      rct->bottom = rct->top + ws->cy + border->cy;
      break;
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

        RECT *rct = (RECT*)lParam;
        SIZE ws   = { rct->right - rct->left - border.cx, rct->bottom - rct->top - border.cy };
        if ( ws.cx < WMODE_MIN_WIDTH )
        {
          ws.cx = WMODE_MIN_WIDTH;
          CorrectWindowWidth(wParam, &ws, rct, &border);
        }
        if ( ws.cy < WMODE_MIN_HEIGHT )
        {
          ws.cy = WMODE_MIN_HEIGHT;
          CorrectWindowHeight(wParam, &ws, rct, &border);
        }

        if ( GetKeyState(VK_CONTROL) & 0x8000 && ws.cy != ws.cx * 3 / 4 )
        {
          if ( wParam == WMSZ_TOP || wParam == WMSZ_BOTTOM )
          {
            ws.cx = ws.cy * 4 / 3;
            CorrectWindowWidth(WMSZ_RIGHT, &ws, rct, &border);
          }
          else
          {
            ws.cy = ws.cx * 3 / 4;
            CorrectWindowHeight( (wParam == WMSZ_RIGHT || wParam == WMSZ_LEFT) ? WMSZ_BOTTOM : wParam, &ws, rct, &border);
          }
        }

        if ( ws.cx >= BW::BWDATA_GameScreenBuffer->wid - WMODE_SNAP_RANGE &&
             ws.cx <= BW::BWDATA_GameScreenBuffer->wid + WMODE_SNAP_RANGE )
        {
          ws.cx = BW::BWDATA_GameScreenBuffer->wid;
          CorrectWindowWidth( (wParam == WMSZ_TOP || wParam == WMSZ_BOTTOM) ? WMSZ_RIGHT : wParam, &ws, rct, &border);
        }
        if ( ws.cy >= BW::BWDATA_GameScreenBuffer->ht - WMODE_SNAP_RANGE &&
             ws.cy <= BW::BWDATA_GameScreenBuffer->ht + WMODE_SNAP_RANGE )
        {
          ws.cy = BW::BWDATA_GameScreenBuffer->ht;
          CorrectWindowHeight( (wParam == WMSZ_RIGHT || wParam == WMSZ_LEFT) ? WMSZ_BOTTOM : wParam, &ws, rct, &border);
        }
        break;
      } // case WM_SIZING
    case WM_SIZE:
      {
        switch ( wParam )
        {
        case SIZE_RESTORED:
          {
            char szTemp[32];
            RECT tempRect;
            GetClientRect(hWnd, &tempRect);
            windowRect.right  = tempRect.right;
            windowRect.bottom = tempRect.bottom;
            WritePrivateProfileString("window", "width",  itoa(tempRect.right,  szTemp, 10), szConfigPath);
            WritePrivateProfileString("window", "height", itoa(tempRect.bottom, szTemp, 10), szConfigPath);
            break;
          }
        }// wParam switch
        break;
      } // case WM_SIZE
    case WM_MOVE:
      {
        RECT tempRect;
        GetWindowRect(hWnd, &tempRect);
        if ( tempRect.right > 0 && 
             tempRect.bottom > 0 && 
             tempRect.left < GetSystemMetrics(SM_CXFULLSCREEN) &&
             tempRect.top  < GetSystemMetrics(SM_CYFULLSCREEN) )
        {
          windowRect.left = tempRect.left;
          windowRect.top  = tempRect.top;

          char szTemp[32];
          WritePrivateProfileString("window", "left", itoa(tempRect.left, szTemp, 10), szConfigPath);
          WritePrivateProfileString("window", "top",  itoa(tempRect.top, szTemp, 10), szConfigPath);
        }
        break;
      } // case WM_MOVE
    case WM_PAINT:
      if ( gbWantUpdate && pBits )
      {
        gbWantUpdate = false;

        // begin paint
        PAINTSTRUCT paint;
        HDC hdc = BeginPaint(hWnd, &paint);

        // Blit to the screen
        RECT cRect;
        GetClientRect(hWnd, &cRect);        
        if ( cRect.right == BW::BWDATA_GameScreenBuffer->wid && cRect.bottom == BW::BWDATA_GameScreenBuffer->ht )
        {
          BitBlt(hdc, 0, 0, BW::BWDATA_GameScreenBuffer->wid, BW::BWDATA_GameScreenBuffer->ht, hdcMem, 0, 0, SRCCOPY);
        }
        else
        {
          SetStretchBltMode(hdc, HALFTONE);
          StretchBlt(hdc, 0, 0, cRect.right, cRect.bottom, hdcMem, 0, 0, BW::BWDATA_GameScreenBuffer->wid, BW::BWDATA_GameScreenBuffer->ht, SRCCOPY);
        }

        // end paint
        EndPaint(hWnd, &paint);
        /*
#ifdef _DEBUG
        if ( recordingStarted )
          RecordFrame(pBits, BW::BWDATA_GameScreenBuffer->wid, BW::BWDATA_GameScreenBuffer->ht);
#endif*/
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
      SetWMode(BW::BWDATA_GameScreenBuffer->wid, BW::BWDATA_GameScreenBuffer->ht, !wmode);
      return TRUE;
    }
    break;
  case WM_MOUSEMOVE:
    if ( GetWindowLong(ghMainWnd, GWL_STYLE) & WS_SYSMENU ) // Compatibility for Xen W-Mode
    {
      (*BW::BWDATA_InputFlags) |= 1;
      POINTS pt = MAKEPOINTS(lParam);
      BW::BWDATA_Mouse->x = pt.x;
      BW::BWDATA_Mouse->y = pt.y;
      return TRUE;
    }
    break;
  case WM_LBUTTONDOWN:
  case WM_LBUTTONUP:
  case WM_LBUTTONDBLCLK:
  case WM_RBUTTONDOWN:
  case WM_RBUTTONUP:
  case WM_RBUTTONDBLCLK:
  case WM_MBUTTONDOWN:
  case WM_MBUTTONUP:
  case WM_MBUTTONDBLCLK:
    if ( GetWindowLong(ghMainWnd, GWL_STYLE) & WS_SYSMENU ) // Compatibility for Xen W-Mode
    {
      ButtonEvent(uMsg - WM_MOUSEFIRST + BW_MOUSEFIRST, lParam);
      return TRUE;
    }
    break;
  case WM_SYSCOMMAND:
    if ( wParam == SC_MAXIMIZE )
    {
      SetWMode(BW::BWDATA_GameScreenBuffer->wid, BW::BWDATA_GameScreenBuffer->ht, false);
      return TRUE;
    }
    break;
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
  {
    if ( _GetCursorPosOld )
      return _GetCursorPosOld(lpPoint);
    return GetCursorPos(lpPoint);
  }

  if ( !gbHoldingAlt )
  {
    lpPoint->x = 320;
    lpPoint->y = 240;
  }
  else
  {
    POINT tempPoint;
    if ( _GetCursorPosOld )
      _GetCursorPosOld(&tempPoint);
    else
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
  {
    if ( _SetCursorPosOld )
      return _SetCursorPosOld(X, Y);
    return SetCursorPos(X, Y);
  }
  return TRUE;
}

BOOL WINAPI _ClipCursor(const RECT *lpRect)
{
  if ( !wmode )
  {
    if ( _ClipCursorOld )
      return _ClipCursorOld(lpRect);
    return ClipCursor(lpRect);
  }
  return TRUE;
}

BOOL __stdcall _SDrawLockSurface(int surfacenumber, RECT *lpDestRect, void **lplpSurface, int *lpPitch, int arg_unused)
{
  if ( !wmode )
  {
    if ( _SDrawLockSurfaceOld )
      return _SDrawLockSurfaceOld(surfacenumber, lpDestRect, lplpSurface, lpPitch, arg_unused);
    return SDrawLockSurface(surfacenumber, lpDestRect, lplpSurface, lpPitch, arg_unused);
  }

  if ( lplpSurface )
    *lplpSurface = pBits;
  if ( lpPitch )
    *lpPitch = 640;
  return TRUE;
}

BOOL __stdcall _SDrawUnlockSurface(int surfacenumber, void *lpSurface, int a3, RECT *lpRect)
{
  if ( !wmode )
  {
    if ( _SDrawUnlockSurfaceOld )
      return _SDrawUnlockSurfaceOld(surfacenumber, lpSurface, a3, lpRect);
    return SDrawUnlockSurface(surfacenumber, lpSurface, a3, lpRect);
  }

  if ( ghMainWnd && lpSurface )
  {
    gbWantUpdate = true;
    InvalidateRect(ghMainWnd, lpRect, FALSE);
  }
  return TRUE;
}

BOOL __stdcall _SDrawUpdatePalette(unsigned int firstentry, unsigned int numentries, PALETTEENTRY *pPalEntries, int a4)
{
  for ( unsigned int i = firstentry; i < firstentry + numentries; ++i )
  {
    bmp.bmiColors[i].rgbRed   = pPalEntries[i].peRed;
    bmp.bmiColors[i].rgbGreen = pPalEntries[i].peGreen;
    bmp.bmiColors[i].rgbBlue  = pPalEntries[i].peBlue;
  }
  if ( !wmode || !ghMainWnd )
  {
    if ( _SDrawUpdatePaletteOld )
      return _SDrawUpdatePaletteOld(firstentry, numentries, pPalEntries, a4);
    return SDrawUpdatePalette(firstentry, numentries, pPalEntries, a4);
  }

  if ( !IsIconic(ghMainWnd) )
    SetDIBColorTable(hdcMem, firstentry, numentries, bmp.bmiColors);
  return TRUE;
}

BOOL __stdcall _SDrawRealizePalette()
{
  if ( !wmode || !ghMainWnd )
  {
    if ( _SDrawRealizePaletteOld )
      return _SDrawRealizePaletteOld();
    return SDrawRealizePalette();
  }

  if ( IsIconic(ghMainWnd) )
    return FALSE;

  return TRUE;
}

void SetWMode(int width, int height, bool state)
{
  // Compatibility for Xen W-Mode
  if ( ghMainWnd && !(GetWindowLong(ghMainWnd, GWL_STYLE) & WS_SYSMENU) )
    return;

  if ( state )
  {
    wmode = true;
    if ( !ghMainWnd )
      return;

    // Call the DirectDraw destructor
    DDrawDestroy();
    InitializeWModeBitmap(BW::BWDATA_GameScreenBuffer->wid, BW::BWDATA_GameScreenBuffer->ht);

    // Hack to enable drawing in Broodwar
    *BW::BWDATA_PrimarySurface = (LPDIRECTDRAWSURFACE)1;

    POINT pos = { windowRect.left + (gdwProcNum ? 32 : 0), windowRect.top + (gdwProcNum ? 32 : 0) };
    // Change the window settings
    SetWindowLong(ghMainWnd, GWL_STYLE, WS_OVERLAPPEDWINDOW);
    SetWindowPos(ghMainWnd, HWND_NOTOPMOST, pos.x, pos.y, width, height, SWP_SHOWWINDOW);
    ShowWindow(ghMainWnd, SW_RESTORE);

    SIZE border;
    GetBorderSize(ghMainWnd, &border);
    int w = width + border.cx;
    int h = height + border.cy;

    int cx = GetSystemMetrics(SM_CXFULLSCREEN);
    int cy = GetSystemMetrics(SM_CYFULLSCREEN);
    while ( pos.x < 0 )
      pos.x = 0;
    while ( pos.y < 0 )
      pos.y = 0;
    if ( pos.y + h >= cy )
    {
      if ( gdwProcNum )
        pos.y -= cy - h;
      else
        pos.y = cy - h;
    }
    if ( pos.x + w >= cx )
    {
      if ( gdwProcNum )
        pos.x -= cx - w;
      else
        pos.x = cx - w;
    }
    MoveWindow(ghMainWnd, pos.x, pos.y, w, h, TRUE);
    SetCursor(NULL);
    SetCursorShowState(false);

    SetDIBColorTable(hdcMem, 0, 256, bmp.bmiColors);
    WritePrivateProfileString("window", "windowed", "ON", szConfigPath);
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

    DDrawDestroy();
    DDrawInitialize(width, height);
    WritePrivateProfileString("window", "windowed", "OFF", szConfigPath);
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
