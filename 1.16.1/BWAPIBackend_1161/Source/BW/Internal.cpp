#include "Offsets.h"
namespace BW
{
  void (__stdcall *pOldDrawGameProc)(BW::Bitmap *pSurface, BW::bounds *pBounds) = nullptr;
  void (__stdcall *pOldDrawDialogProc)(BW::Bitmap *pSurface, BW::bounds *pBounds) = nullptr;

};
