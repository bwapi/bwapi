#include "Offsets.h"


namespace BW
{
  BOOL (__stdcall *SNetReceiveMessage)(int *senderplayerid, u8 **data, int *databytes) = NULL;
  BOOL (__stdcall *SCodeDelete)(HANDLE handle)                                         = NULL;
  int  (__stdcall *SStrCopy)(char *dest, const char *source, size_t size)              = NULL;
  void* (__stdcall *SMemAlloc)(int amount, char *logfilename, int logline, int defaultValue) = NULL;
  BOOL (__stdcall *SFileOpenFileEx)(HANDLE hMpq, const char *szFileName, DWORD dwSearchScope, HANDLE *phFile) = NULL;

  char *BWDATA_StringTableOff = NULL;

  const char *GetStatString(int index)
  {
    if ( BWDATA_StringTableOff )
      return (const char*)&BWDATA_StringTableOff[*(u16*)&BWDATA_StringTableOff[index * 2]];
    else
      return "Unable to initialize BWDATA_StringTableOff.";
  }

};
