#include "FileWriter.h"

#include <windows.h>
#include <string>

using namespace ReplayTool;

/*  msdn sample of file appending:
    dwPos = SetFilePointer(hAppend, 0, NULL, FILE_END);
    LockFile(hAppend, dwPos, 0, dwBytesWrite, 0);
    WriteFile(hAppend, buff, dwBytesWrite, &dwBytesWritten, NULL);
    UnlockFile(hAppend, dwPos, 0, dwBytesWrite, 0);
    */

FileWriter::FileWriter()
:hFile(NULL)
{
}

FileWriter::~FileWriter()
{
  this->Close();
}

void FileWriter::Close()
{
  if ( this->hFile )
    CloseHandle(this->hFile);
  this->hFile = NULL;
}

bool FileWriter::Error(const char *pszMsg)
{
  this->Close();
  if ( pszMsg )
    MessageBox(NULL, pszMsg, NULL, MB_OK | MB_ICONERROR);
  return false;
}

bool FileWriter::Open(const char *pszFilename)
{
  // Open the file handle
  this->Close();
  this->hFile = CreateFile(pszFilename, GENERIC_WRITE, 0, NULL, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);
  if ( this->hFile == INVALID_HANDLE_VALUE )
    return this->Error();
  return true;
}

void FileWriter::Write7BitEncodedInt(int value)
{
  int remaining = value;
  do
  {
    this->Write<BYTE>( (remaining & 0x7F) | (remaining > 0x7F ? 0x80 : 0) );
    remaining >>= 7;
  } while ( remaining > 0 );
}

void FileWriter::WriteString(std::string str)
{
  size_t s = str.size();
  this->Write7BitEncodedInt(s);

  const char *p = str.c_str();
  for ( unsigned int i = 0; i < s; ++i )
    this->Write<char>(p[i]);
}

void FileWriter::WriteRaw(void *pData, size_t size)
{
  if ( this->hFile )
  {
    DWORD written;
    WriteFile(this->hFile, pData, size, &written, NULL);
  }
}
