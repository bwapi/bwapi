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
    bool Eof();
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
    int         Read7BitEncodedInt();
    std::string ReadString();
    std::string ReadCString(const char *deliminators = NULL);
    DWORD       GetSize();
    //FileReader  Decompress(DWORD dwCompressedSize, DWORD dwDecompressedSize);  // Requires XNA header
  private:
    HANDLE  hFile;
    BYTE    *pMem;
    DWORD   dwFileSize;
    DWORD   dwOffset;
    bool    eof;
  };
}