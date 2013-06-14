#include <windows.h>
#include <stdio.h>

#include <BWAPI.h>

#include "Replay.h"

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

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow)
{
  BWAPI::BWAPI_init();

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

  switch ( pOpts[1] )
  {
  case 'u':
  case 'U':
  case 'e':
  case 'E':
  case 'd':
  case 'D':
    if ( ParseReplay(szFile, RFLAG_EXTRACT) )
      MessageBox(NULL, "Extracted successfully.", "Success", MB_OK);
    else
      MessageBox(NULL, "Failed reading process somewhere.", "Failure", MB_OK);
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
    ParseReplay(szFile, RFLAG_REPAIR);
    break;
  default:
    return Usage();
  }
  return EXIT_SUCCESS;
}
