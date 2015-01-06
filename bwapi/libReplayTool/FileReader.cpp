#include "FileReader.h"

#include <windows.h>
#include <string>

using namespace ReplayTool;

FileReader::FileReader()
:hFile(NULL)
,pMem(NULL)
,dwFileSize(0)
,dwOffset(0)
,eof(false)
{
}

FileReader::FileReader(const void *pData, DWORD dwDataSize)
:hFile(NULL)
,pMem((BYTE*)pData)
,dwFileSize(dwDataSize)
,dwOffset(0)
,eof(false)
{
}

FileReader::~FileReader()
{
  this->Free();
}

void FileReader::Free()
{
  if ( this->pMem )
    free(this->pMem);
  this->pMem = NULL;

  if ( this->hFile )
    CloseHandle(this->hFile);
  this->hFile = NULL;
  
  this->dwFileSize  = 0;
  this->dwOffset    = 0;
  this->eof         = false;
}

bool FileReader::Error(const char *pszMsg)
{
  this->Free();

  LPSTR pszStr;
  FormatMessage(FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM, NULL, GetLastError(), 0, (LPSTR)&pszStr, 0, NULL);

  char msg[512];
  sprintf_s(msg, 512, "%s\n%s", pszMsg, pszStr);
  MessageBox(NULL, msg, NULL, MB_OK | MB_ICONERROR);
  LocalFree(pszStr);
  return false;
}

bool FileReader::Open(const char *pszFilename)
{
  // Open the file handle
  this->hFile = CreateFile(pszFilename, GENERIC_READ, FILE_SHARE_READ, NULL, OPEN_EXISTING, 0, NULL);
  if ( this->hFile == INVALID_HANDLE_VALUE )
    return this->Error();
  
  // Retrieve the file size
  this->dwFileSize = GetFileSize(hFile, NULL);
  if ( this->dwFileSize == INVALID_FILE_SIZE )
    return this->Error();

  // Allocate buffer
  this->pMem = (BYTE*)malloc(this->dwFileSize);
  if ( !this->pMem )
    return this->Error();

  // Read the file to memory
  DWORD dwRead = 0;
  if ( !ReadFile(this->hFile, this->pMem, this->dwFileSize, &dwRead, NULL) || dwRead != this->dwFileSize )
    return this->Error();

  // Close the file handle
  CloseHandle(hFile);
  hFile = NULL;
  return true;
}

void FileReader::Read(void *pBuffer, DWORD dwSize)
{
  BYTE *pb = (BYTE*)pBuffer;
  DWORD dwNullCount = dwOffset + dwSize - dwFileSize;
  if ( dwNullCount & 0x80000000 )
    dwNullCount = 0;
  DWORD dwNormCount = dwSize - dwNullCount;
  
  if ( dwNormCount )
  {
    memcpy(pb, &this->pMem[dwOffset], dwNormCount);
    dwOffset += dwNormCount;
    pb += dwNormCount;
  }
  if ( dwNullCount )
  {
    this->eof = true;
    memset(pb, 0, dwNullCount);
    return;
  }
}
