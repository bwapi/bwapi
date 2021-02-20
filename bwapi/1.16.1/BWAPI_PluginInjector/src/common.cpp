#include <Windows.h>
#include <cstdio>
#include <string>
#include <sstream>
#include <fstream>
#include <iomanip>
#include "common.h"

// For MessageBox()
#pragma comment(lib, "User32.lib")

// For Reg* functions
#pragma comment(lib, "Advapi32.lib")


std::string GetPluginName()
{
#ifdef _DEBUG
  return "BWAPI Injector DEBUG";
#else
  return "BWAPI Injector";
#endif
}

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
  if (dwErrCode != ERROR_SUCCESS)
  {
    /// @TODO: Check error
    GetSingleRegString(HKEY_LOCAL_MACHINE, pszSubKey, pszValueName, szTemp, &dwSize);
  }

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

  std::ostringstream errorMessage;
  errorMessage << buffer << '\n'
    << "ID " << reinterpret_cast<void*>(dwLastError) << ": \n"
    << szErrMsg << std::endl;

  // Open a log file and print to it
  std::ofstream log{ "bwapi-error.txt", std::ios::app };
  if (log)
  {
    const time_t now = std::time(nullptr);
    log << '[' << std::put_time(std::localtime(&now), "%F %T") << "] " << errorMessage.str();
  }

  // Create a message box with the message and the error message
  MessageBox(NULL, errorMessage.str().c_str(), NULL, MB_OK | MB_ICONERROR);
  return false;
}
