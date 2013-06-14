#include "Offsets.h"


namespace BW
{
  void (__stdcall *pOldDrawGameProc)(BW::Bitmap *pSurface, BW::bounds *pBounds) = NULL;
  void (__stdcall *pOldDrawDialogProc)(BW::Bitmap *pSurface, BW::bounds *pBounds) = NULL;

  const char *GetStatString(int index)
  {
    if ( *BWDATA::StringTableOff )
      return (const char*)&BWDATA::StringTableOff[(u16&)BWDATA::StringTableOff[index * 2 + 2]];
    return "";
  }

};
