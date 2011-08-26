#include <windows.h>
#include <stdio.h>

#include "../PKLib/pklib.h"

#include "RepHeader.h"
#include "FileReader.h"
#include "FileWriter.h"

int Usage()
{
  MessageBox(NULL,  "Usage: reptool -[option] [parameters]\n"
                    "  -u [file]      Unpack replay.\n"
                    "  -p [file]      Pack replay.\n"
                    "  -r [file]      Auto-Repair replay.\n"
                    "\nAliases:\n"
                    "  -e [file]      Extract. Alias for unpack.\n"
                    "  -d [file]      Decompress. Alias for unpack.\n"
                    "  -c [file]      Compress. Alias for pack.\n"
                    "  -f [file]      Fix. Alias for repair.\n", "Usage", MB_OK | MB_ICONINFORMATION);
  return EXIT_SUCCESS;
}

struct _Part
{
  DWORD dwCrc32Sum;
  DWORD dwSectionCount;
};

struct _Param
{
  char  *pCompressedData;
  DWORD dwReadPos;
  char  *pDecompressedData;
  DWORD dwWritePos;
  DWORD dwMaxRead;
  DWORD dwMaxWrite;
} params;

char *pWork;
char *pSegment;

unsigned int __cdecl read_buf(char *buf, unsigned int *size, void *_param)
{
  _Param *param = (_Param*)_param;

  DWORD dwSize = param->dwMaxRead - param->dwReadPos;
  if ( dwSize > *size )
    dwSize = *size;
  memcpy(buf, &param->pCompressedData[param->dwReadPos], dwSize);
  param->dwReadPos += dwSize;
  return dwSize;
}
void __cdecl write_buf(char *buf, unsigned int *size, void *_param)
{
  _Param *param = (_Param*)_param;
  if ( param->dwWritePos + *size <= param->dwMaxWrite )
    memcpy(&param->pDecompressedData[param->dwWritePos], buf, *size);
  param->dwWritePos += *size;
}

int Decompress(void *pComp, int compSize, char **retBuf)
{
  memset(&params, 0, sizeof(params));
  params.pCompressedData    = (char*)pComp;
  params.dwMaxRead          = compSize;
  params.pDecompressedData  = pSegment;
  params.dwMaxWrite         = 0x2000;
  
  if ( explode(&read_buf, &write_buf, pWork, &params) || params.dwWritePos > 0x2000 )
    return 0;

  *retBuf = pSegment;
  return params.dwWritePos;
}

bool DecompressRead(void *pOutput, size_t outputSize, FileReader &fr)
{
  char *_pOutput = (char*)pOutput;
  if ( !outputSize )
    return false;

  _Part hdr = fr.Read<_Part>();
  if ( hdr.dwSectionCount > 0 )
  {
    // Allocate and initialize work buffers
    pWork = (char*)malloc(EXP_BUFFER_SIZE);
    memset(pWork, 0, EXP_BUFFER_SIZE);

    pSegment = (char*)malloc(0x2000);
    memset(pSegment, 0, 0x2000);

    memset(&params, 0, sizeof(params));

    int decompCountTotal = 0;
    DWORD dwOffset = 0;
    for ( DWORD dwSectionCount = 0; dwSectionCount < hdr.dwSectionCount; ++dwSectionCount )
    {
      // Read the chunk size
      size_t chunkSize = fr.Read<size_t>();

      // Obtain the decompressed size difference
      int readCountDiff = outputSize - decompCountTotal;
      if ( chunkSize > (readCountDiff < 0 ? 0 : (DWORD)readCountDiff) )
        break;

      // Read the data
      fr.Read(_pOutput, chunkSize);
      DWORD dwRemaining = outputSize - dwOffset;
      if ( dwRemaining > 0x2000 )
        dwRemaining = 0x2000;

      int decompCount = 0;
      if ( chunkSize != dwRemaining )
      {
        // Decompress data
        char *pOut = NULL;
        decompCount = Decompress(_pOutput, chunkSize, &pOut);
        if ( !decompCount || decompCount > (int)outputSize )
          break;

        // Copy to true output decompression buffer
        memcpy(_pOutput, pOut, decompCount);
      }
      else
      {
        decompCount = chunkSize;
      }

      // Increment Decompression Offsets
      decompCountTotal  += decompCount;
      _pOutput          += decompCount;
      dwOffset          += 0x2000;
    }

    // Free work buffers
    free(pSegment);
    pSegment = NULL;
    free(pWork);
    pWork = NULL;
  }

  unsigned int  dwSize = outputSize;
  unsigned long dwOld  = ~0;
  bool rval = crc32pk((char*)pOutput, &dwSize, &dwOld) == hdr.dwCrc32Sum;
  return rval;
}

replay_resource replayHeader;

bool DecompressReplay(const char *pszFilename)
{
  // Open replay file
  FileReader fr;
  if ( !fr.Open(pszFilename) )
    return false;

  // Read replay resource identifier
  DWORD dwRepResourceID = 0;
  // Best guess: "reRS" is "replay RESOURCE"
  if ( !DecompressRead(&dwRepResourceID, sizeof(dwRepResourceID), fr) || dwRepResourceID != mmioFOURCC('r','e','R','S') )
    return false;

  // Read replay resource header
  if ( !DecompressRead(&replayHeader, sizeof(replayHeader), fr) )
    return false;

  // Read replay actions section size
  DWORD dwActionBufferSize = 0;
  if ( !DecompressRead(&dwActionBufferSize, 4, fr) || !dwActionBufferSize )
    return false;
  
  // Allocate and Read replay actions
  void *pActionBuffer = malloc(dwActionBufferSize);
  if ( !DecompressRead(&pActionBuffer, dwActionBufferSize, fr) )
  {
    free(pActionBuffer);
    return false;
  }

  // allocate and read map chunk data

  // parse data

  // Cleanup and return success
  free(pActionBuffer);
  return true;
}

int APIENTRY WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow)
{
  char *pOpts = strchr(lpCmdLine, '-');
  size_t cmdSize = strlen(lpCmdLine);
  if ( !pOpts || cmdSize < 4 )
    return Usage();

  char szFile[MAX_PATH];
  int p = 0;
  for ( int i = 3; i < (int)cmdSize; ++i )
  {
    if ( lpCmdLine[i] < ' '   ||
         lpCmdLine[i] == '*'  ||
         lpCmdLine[i] == '?'  ||
         lpCmdLine[i] == '\"' ||
         lpCmdLine[i] == '<'  ||
         lpCmdLine[i] == '>'  ||
         lpCmdLine[i] == '|' )
      continue;
    szFile[p] = lpCmdLine[i];
    ++p;
  }
  szFile[p] = 0;

  MessageBox(NULL, "Pause", "dbg", 0);
  switch ( pOpts[1] )
  {
  case 'u':
  case 'U':
  case 'e':
  case 'E':
  case 'd':
  case 'D':
    if ( DecompressReplay(szFile) )
      MessageBox(NULL, "Actions Read", "Success", MB_OK);
    break;
  case 'p':
  case 'P':
  case 'c':
  case 'C':
    break;
  case 'r':
  case 'R':
  case 'f':
  case 'F':
    break;
  default:
    return Usage();
  }
  return EXIT_SUCCESS;
}
