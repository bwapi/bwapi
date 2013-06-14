#pragma once
#include <windows.h>
#include <string>

namespace ReplayTool
{
  class FileWriter
  {
  public:
    FileWriter();
    ~FileWriter();
    bool Open(const char *pszFilename);
    bool Error(const char *pszText = NULL);
    void Close();
    template <class _T>
    void Write(_T val)
    {
      DWORD written;
      WriteFile(this->hFile, &val, sizeof(_T), &written, NULL);
    }
    void  WriteRaw(void *pData, size_t size);
    void  Write7BitEncodedInt(int value);
    void  WriteString(std::string str);
  private:
    HANDLE  hFile;
  };
}