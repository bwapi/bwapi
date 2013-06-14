#include "PKShared.h"

#include "../PKLib/pklib.h"
#include "FileReader.h"
#include "FileWriter.h"

using namespace ReplayTool;

_Param params;
char bWorkBuff[EXP_BUFFER_SIZE];
char bWorkBuff2[CMP_BUFFER_SIZE];
char bSegment[0x2000];

unsigned int PKEXPORT read_buf(char *buf, unsigned int *size, void *_param)
{
  _Param *param = (_Param*)_param;

  DWORD dwSize = param->dwMaxRead - param->dwReadPos;
  if ( dwSize > *size )
    dwSize = *size;
  memcpy(buf, &param->pCompressedData[param->dwReadPos], dwSize);
  param->dwReadPos += dwSize;
  return dwSize;
}
void PKEXPORT write_buf(char *buf, unsigned int *size, void *_param)
{
  _Param *param = (_Param*)_param;
  if ( param->dwWritePos + *size <= param->dwMaxWrite )
    memcpy(&param->pDecompressedData[param->dwWritePos], buf, *size);
  param->dwWritePos += *size;
}

bool DecompressRead(void *pOutput, size_t outputSize, FileReader &fr)
{
  if ( !outputSize )
    return false;

  char *_pOutput = (char*)pOutput;
  memset(bWorkBuff, 0, sizeof(bWorkBuff));
  memset(bSegment, 0, sizeof(bSegment));
  memset(&params, 0, sizeof(params));

  _Part hdr = fr.Read<_Part>();
  DWORD dwPos = 0;
  for ( DWORD s = 0; s < hdr.dwSectionCount; ++s )
  {
    size_t chunkSize = fr.Read<size_t>();
    if ( chunkSize > outputSize )
    {
      MessageBox(NULL, "ChunkSize > output", 0, 0);
      return false;
    }

    char *pTmp = (char*)malloc(chunkSize);
    if ( pTmp != nullptr )
    {
      fr.Read(pTmp, chunkSize);
      if ( chunkSize != outputSize )
      {
        memset(&params, 0, sizeof(params));
        params.pCompressedData    = pTmp;
        params.dwMaxRead          = chunkSize;
        params.pDecompressedData  = bSegment;
        params.dwMaxWrite         = sizeof(bSegment);

        if ( explode(&read_buf, &write_buf, bWorkBuff, &params) )
          return false;

        if ( params.dwWritePos <= sizeof(bSegment) )
        {
          memcpy(&_pOutput[dwPos], bSegment, params.dwWritePos);
          dwPos     += params.dwWritePos;
        }
      }
      else
      {
        memcpy(&_pOutput[dwPos], pTmp, chunkSize);
        dwPos += chunkSize;
      }
      free(pTmp);
    }
  }

  unsigned int  dwSize = outputSize;
  unsigned long dwOld  = ~0;
  return crc32pk((char*)pOutput, &dwSize, &dwOld) == hdr.dwCrc32Sum;
}

void CompressWrite(void *pInput, size_t inputSize, FileWriter &fw)
{
  if ( !pInput )
    return;

  char *_pInput = (char*)pInput;
  memset(bWorkBuff2, 0, sizeof(bWorkBuff2));
  memset(bSegment, 0, sizeof(bSegment));
  memset(&params, 0, sizeof(params));

  // Write Header
  _Part hdr = { 0 };

  // checksum
  unsigned int  dwSize = inputSize;
  unsigned long dwOld  = ~0;
  hdr.dwCrc32Sum = crc32pk((char*)pInput, &dwSize, &dwOld);

  // sections
  hdr.dwSectionCount = dwSize / 0x2000;
  if ( dwSize % 0x2000 )
    hdr.dwSectionCount++;
  
  // write
  fw.Write<_Part>(hdr);

  // Iterate sections
  DWORD dwPos = 0;
  for ( DWORD s = 0; s < hdr.dwSectionCount; ++s )
  {
    DWORD dwWriteSize = inputSize - dwPos;
    if ( dwWriteSize > 0x2000 )
      dwWriteSize = 0x2000;

    memset(&params, 0, sizeof(params));
    params.pCompressedData    = &_pInput[dwPos];
    params.dwMaxRead          = dwWriteSize;
    params.pDecompressedData  = bSegment;
    params.dwMaxWrite         = dwWriteSize;

    unsigned int dwType = 0;
    unsigned int dwImplSize = 0x400;
    if ( implode(&read_buf, &write_buf, bWorkBuff2, &params, &dwType, &dwImplSize) || params.dwWritePos >= dwWriteSize )
    {
      if ( !dwWriteSize )
        return;
      fw.Write<DWORD>(dwWriteSize);
      fw.WriteRaw(&_pInput[dwPos], dwWriteSize);
      dwPos += dwWriteSize;
    }
    else
    {
      if ( !params.dwWritePos )
        return;
      fw.Write<DWORD>(params.dwWritePos);
      fw.WriteRaw(bSegment, params.dwWritePos);
      dwPos += dwWriteSize;
    }

    
  } // for
}
