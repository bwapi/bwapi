#pragma once
#include <windows.h>
#include <string>

namespace ReplayTool
{
  class FileReader
  {
  public:
    FileReader();
    FileReader(const void *pData, DWORD dwDataSize);
    ~FileReader();
    bool Open(const char *pszFilename);
    bool Error(const char *pszText = NULL);
    void Free();
    template <class _T>
    _T Read()
    {
      _T rval;
      if ( dwOffset + sizeof(_T) <= dwFileSize )
      {
        rval = *(_T*)&this->pMem[dwOffset];
        dwOffset += sizeof(_T);
      }
      else
      {
        memset(&rval, 0, sizeof(_T));
        this->eof = true;
      }
      return rval;
    }
    void Read(void *pBuffer, DWORD dwSize);
  private:
    HANDLE  hFile;
    BYTE    *pMem;
    DWORD   dwFileSize;
    DWORD   dwOffset;
    bool    eof;
  };
}