#include "Offsets.h"


namespace BW
{
  TileID      *BWDATA_MapTileArray    = NULL;
  TileType    *BWDATA_TileSet         = NULL;
  SAI_Paths   *BWDATA_SAIPathing      = NULL;
  activeTile  *BWDATA_ActiveTileArray = NULL;

  MiniTileMaps_type* BWDATA_MiniTileFlags = NULL;
  void (__stdcall *pOldDrawGameProc)(BW::bitmap *pSurface, BW::bounds *pBounds) = NULL;
  void (__stdcall *pOldDrawDialogProc)(BW::bitmap *pSurface, BW::bounds *pBounds) = NULL;

};
