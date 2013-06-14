#include <Windows.h>
#include <cstdio>
#include <string>

#include "common.h"

const char *pszPluginName = "BWAPI Injector (" STARCRAFT_VER ") r" SVN_REV_STR " (" BUILD_STR ")";

std::string GetBWAPITarget()
{
  return GetBWAPIDataDirectory() + ("\\" MODULE);
}

std::string GetBWAPIDataDirectory()
{
  std::string sInstallPath = GetRegString("SOFTWARE\\Blizzard Entertainment\\Starcraft", "InstallPath");
  return sInstallPath + "\\bwapi-data";
}

DWORD GetSingleRegString(HKEY hBaseKey, const char *pszSubKey, const char *pszValueName, char *pszOutput, DWORD *dwOutSize)
{
  HKEY hKey = NULL;
  pszOutput[0] = '\0';

  // Open the key
  DWORD dwErrCode = RegOpenKeyEx(hBaseKey, pszSubKey, 0, KEY_QUERY_VALUE, &hKey);
  if ( dwErrCode != ERROR_SUCCESS )
    return dwErrCode;

  // Query the value
  dwErrCode = RegQueryValueEx(hKey, pszValueName, NULL, NULL, (LPBYTE)pszOutput, dwOutSize);

  // Close key and return error code
  RegCloseKey(hKey);
  return dwErrCode;
}

std::string GetRegString(const char *pszSubKey, const char *pszValueName)
{
  // Declare temporary string and size
  char szTemp[MAX_PATH];
  DWORD dwSize = MAX_PATH;

  // Retrieve the key and value from HKCU
  DWORD dwErrCode = GetSingleRegString(HKEY_CURRENT_USER, pszSubKey, pszValueName, szTemp, &dwSize);
  // If it's not found, then search HKLM
  if ( dwErrCode != ERROR_SUCCESS )
    dwErrCode = GetSingleRegString(HKEY_LOCAL_MACHINE, pszSubKey, pszValueName, szTemp, &dwSize);

  // Return the string
  return std::string(szTemp);
}

bool BWAPIError(const char *format, ...)
{
  // Format the variable argument list
  char buffer[MAX_PATH];
  va_list ap;
  va_start(ap, format);
  vsnprintf_s(buffer, MAX_PATH, MAX_PATH, format, ap);
  va_end(ap);

  // Get the last error and error message
  char szErrMsg[MAX_PATH];
  DWORD dwLastError = GetLastError();
  FormatMessage(FORMAT_MESSAGE_FROM_SYSTEM, NULL, dwLastError, 0, szErrMsg, MAX_PATH, NULL);

  // Retrieve the system time
  SYSTEMTIME time;
  GetSystemTime(&time);

  // Open a log file and print to it
  FILE* f = fopen("bwapi-error.txt", "a+");
  if ( f )
  {
    fprintf(f, "[%u/%02u/%02u - %02u:%02u:%02u] %s - %08X: %s\n", time.wYear, 
                                                            time.wMonth, 
                                                            time.wDay, 
                                                            time.wHour, 
                                                            time.wMinute, 
                                                            time.wSecond, 
                                                            buffer,
                                                            dwLastError,
                                                            szErrMsg);
    fclose(f);
  }

  // Create a message box with the message and the error message
  char szBuffer[MAX_PATH*2];
  sprintf(szBuffer, "%s\n%08X: %s", buffer, dwLastError, szErrMsg);
  MessageBox(NULL, szBuffer, NULL, MB_OK | MB_ICONERROR );
  return false;
}
