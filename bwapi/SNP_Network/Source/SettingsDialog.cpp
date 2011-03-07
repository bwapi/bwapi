#include "SettingsDialog.h"
#include <windows.h>

#include "resource.h"

INT_PTR CALLBACK Sessings( HWND, UINT, WPARAM, LPARAM );
DWORD WINAPI dlgThread(LPVOID lpParameter);

volatile bool dlgActive = false;
volatile bool wantExit = false;

HANDLE dlgThreadHandle = NULL;
HWND hDlg;

extern HINSTANCE hInstance;

DWORD WINAPI dlgThread(LPVOID lpParameter)
{
  DialogBoxA(hInstance, MAKEINTRESOURCE(IDD_SETTINGS), NULL, Sessings);
  return 0;
}

void showSettingsDialog()
{
  wantExit = false;
  dlgActive = true;
  dlgThreadHandle = CreateThread(NULL, NULL, dlgThread, NULL, NULL, NULL);
}

void hideSettingsDialog()
{
  if(dlgActive)
  {
    wantExit = true;
    SendMessage(hDlg, WM_CLOSE, 0, 0);
    WaitForSingleObject(dlgThreadHandle, 10000);
    dlgActive = false;
    dlgThreadHandle = NULL;
  }
}

const char* getHostIPString()
{
  static char buffer[32];
  if(hDlg)
  {
    GetDlgItemText(hDlg, IDC_EDITIP, buffer, 32);
  }
  return buffer;
}

const char* getHostPortString()
{
  static char buffer[32];
  if(hDlg)
  {
    GetDlgItemText(hDlg, IDC_EDITPORT, buffer, 32);
  }
  return buffer;
}

const char* getLocalPortString()
{
  static char buffer[32];
  if(hDlg)
    GetDlgItemText(hDlg, IDC_EDITLPORT, buffer, 32);
  return buffer;
}

void setStatusString(const char *statusText)
{
  if(hDlg)
    SetDlgItemText(hDlg, IDC_STATUS, statusText);
}


INT_PTR CALLBACK Sessings( HWND _hDlg, UINT message, WPARAM wParam, LPARAM lParam )
{
  hDlg = _hDlg;
  switch( message )
  {
  case WM_INITDIALOG:
    SetDlgItemTextA(_hDlg, IDC_EDITIP,    "127.0.0.1");
    SetDlgItemTextA(_hDlg, IDC_EDITPORT,  "6112");
    SetDlgItemTextA(_hDlg, IDC_EDITLPORT, "6112");
    return TRUE;
  case WM_COMMAND:
    if (LOWORD(wParam) == IDOK || LOWORD(wParam) == IDCANCEL) 
    {
      EndDialog(hDlg, LOWORD(wParam));
      return TRUE;
    }
    break;
  case WM_CLOSE:
    EndDialog(hDlg, 0);
    break;
  }
  return FALSE;
}
