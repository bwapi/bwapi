#include "SettingsDialog.h"

INT_PTR CALLBACK Settings( HWND, UINT, WPARAM, LPARAM );
DWORD WINAPI dlgThread(LPVOID lpParameter);

HWND hDlg;

extern HINSTANCE hInstance;

DWORD WINAPI dlgThread(LPVOID lpParameter)
{
  DialogBox(hInstance, MAKEINTRESOURCE(IDD_SETTINGS), NULL, Settings);
  return 0;
}

SettingsDialog::SettingsDialog() : hdlgThread(NULL) {}
SettingsDialog::~SettingsDialog() {}

void SettingsDialog::init() {
    showSettingsDialog();
}

void SettingsDialog::release() {
    hideSettingsDialog();
}

void SettingsDialog::showSettingsDialog()
{
  hdlgThread = CreateThread(NULL, NULL, dlgThread, NULL, NULL, NULL);
}

void SettingsDialog::hideSettingsDialog()
{
  if(hdlgThread)
  {
    SendMessage(hDlg, WM_CLOSE, 0, 0);
    WaitForSingleObject(hdlgThread, 10000);
    hdlgThread = NULL;
  }
}

const char* SettingsDialog::getHostIPString() const
{
  static char buffer[32];
  if(hDlg)
  {
    GetDlgItemTextA(hDlg, IDC_EDITIP, buffer, 32);
  }
  return buffer;
}

const uint16_t SettingsDialog::getHostPort() const
{
  static char buffer[32];
  if(hDlg)
  {
    GetDlgItemTextA(hDlg, IDC_EDITPORT, buffer, 32);
  }
  return (uint16_t)std::strtoul(buffer, nullptr, 10);
}

const uint16_t SettingsDialog::getLocalPort() const
{
  static char buffer[32];
  if(hDlg)
  {
    GetDlgItemTextA(hDlg, IDC_EDITLPORT, buffer, 32);
  }
  return (uint16_t)std::strtoul(buffer, nullptr, 10);
}

void SettingsDialog::setStatusString(const char *statusText)
{
  if(hDlg)
  {
    SetDlgItemTextA(hDlg, IDC_STATUS, statusText);
  }
}


INT_PTR CALLBACK Settings( HWND hwndDlg, UINT uMsg, WPARAM wParam, LPARAM lParam )
{
  switch( uMsg )
  {
    case WM_INITDIALOG:
      SetDlgItemTextA(hwndDlg, IDC_EDITIP,    "127.0.0.1");
      SetDlgItemTextA(hwndDlg, IDC_EDITPORT,  "6112");
      SetDlgItemTextA(hwndDlg, IDC_EDITLPORT, "6112");
      hDlg = hwndDlg;
      return TRUE;

    case WM_COMMAND:
      if (LOWORD(wParam) == IDOK || LOWORD(wParam) == IDCANCEL) 
      {
        EndDialog(hwndDlg, LOWORD(wParam));
        hDlg = NULL;
        return TRUE;
      }
      break;

    case WM_CLOSE:
      EndDialog(hwndDlg, 0);
      hDlg = NULL;
      return TRUE;
  }
    return FALSE;
}
