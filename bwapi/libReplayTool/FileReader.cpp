#include "FileReader.h"

#include <windows.h>
#include <string>

using namespace ReplayTool;

FileReader::FileReader()
:pMem(NULL)
,dwFileSize(0)
,dwOffset(0)
,eof(false)
,hFile(NULL)
{
}

FileReader::FileReader(const void *pData, DWORD dwDataSize)
:pMem((BYTE*)pData)
,dwFileSize(dwDataSize)
,dwOffset(0)
,eof(false)
,hFile(NULL)
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
  this->hFile = NULL;
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

int FileReader::Read7BitEncodedInt()
{
  int rval = 0, bitshift = 0;
  BYTE bChar = 0;
  do
  {
    if ( bitshift >= 7*5 ) //ErrBox("Exceeded maximum 7BitEncodedInt value.");
      return rval;
    bChar = this->Read<BYTE>();
    rval |= (bChar & 0x7F) << bitshift;
    bitshift += 7;
  } while ( (bChar & 0x80) != 0 );
  return rval;
}

std::string FileReader::ReadString()
{
  int len = this->Read7BitEncodedInt();
  std::string str;
  if ( len > 0 && this->dwOffset + len <= this->dwFileSize )
  {
    str.assign((char*)&this->pMem[this->dwOffset], len);
    this->dwOffset += len;
  }
  else
  {
    str = "";
    if ( this->dwOffset + len > this->dwFileSize )
      this->eof = true;
  }
  return str;
}

std::string FileReader::ReadCString(const char *deliminators)
{
  std::string str;

  int len = 0;
  while ( this->dwOffset + len <= this->dwFileSize && this->pMem[this->dwOffset + len] != 0 )
  {
    bool brk = false;
    for ( int i = 0; deliminators && deliminators[i]; ++i )
    {
      if ( this->pMem[this->dwOffset + len] == deliminators[i] )
        brk = true;
    }
    if ( brk )
      break;
    ++len;
  }

  if ( len > 0 && (this->dwOffset + len <= this->dwFileSize || this->dwFileSize > this->dwOffset) )
  {
    if ( this->dwOffset + len <= this->dwFileSize )
    {
      str.assign((char*)&this->pMem[this->dwOffset], len);
      this->dwOffset += len;
    }
    else
      str.assign((char*)&this->pMem[this->dwOffset], this->dwFileSize - this->dwOffset);
  }
  else
  {
    str = "";
    if ( this->dwOffset + len <= this->dwFileSize )
      this->dwOffset++;
  }
  if ( this->dwOffset + len > this->dwFileSize )
    this->eof = true;
  return str;
}

DWORD FileReader::GetSize()
{
  return this->dwFileSize;
}
/*
  // Requires XNA header
FileReader FileReader::Decompress(DWORD dwCompressedSize, DWORD dwDecompressedSize)
{
  BYTE *pDecompressedData = (BYTE*)malloc(dwDecompressedSize);
  DecompressData(&this->pMem[this->dwOffset], dwCompressedSize, pDecompressedData, dwDecompressedSize);
  return FileReader(pDecompressedData, dwDecompressedSize);
}
*/
bool FileReader::Eof()
{
  return this->eof;
}
