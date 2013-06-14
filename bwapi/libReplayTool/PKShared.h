#pragma once
#include "../PKLib/pklib.h"
#include <windows.h>

namespace ReplayTool
{
  class FileReader;
  class FileWriter;
}

typedef struct __Part
{
  DWORD dwCrc32Sum;
  DWORD dwSectionCount;
} _Part;

typedef struct __Param
{
  char  *pCompressedData;
  DWORD dwReadPos;
  char  *pDecompressedData;
  DWORD dwWritePos;
  DWORD dwMaxRead;
  DWORD dwMaxWrite;
} _Param;


unsigned int PKEXPORT read_buf(char *buf, unsigned int *size, void *_param);
void PKEXPORT write_buf(char *buf, unsigned int *size, void *_param);
bool DecompressRead(void *pOutput, size_t outputSize, ReplayTool::FileReader &fr);
void CompressWrite(void *pInput, size_t inputSize, ReplayTool::FileWriter &fw);