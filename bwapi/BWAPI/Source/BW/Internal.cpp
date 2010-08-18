#include "Offsets.h"


namespace BW
{
  TileID      *BWDATA_ZergCreepArray  = NULL;
  TileID      *BWDATA_MapTileArray    = NULL;
  TileType    *BWDATA_TileSet         = NULL;
  DoodatType  *BWDATA_DoodatSet       = NULL;

  char *BWDATA_StringTableOff = NULL;
  u32  *BWDATA_MapFogOfWar    = NULL;

  MiniTileMaps_type* BWDATA_MiniTileFlags = NULL;
  void (__stdcall *pOldDrawHook)(BW::bitmap *pSurface, BW::bounds *pBounds) = NULL;

  const char *GetStatString(int index)
  {
    if ( BWDATA_StringTableOff )
      return (const char*)&BWDATA_StringTableOff[*(u16*)&BWDATA_StringTableOff[index * 2]];
    else
      return "Unable to initialize BWDATA_StringTableOff.";
  }

  BOOL __stdcall FakeROP3(int maxiterations, int lpSurface, int width, int height, int width2, int pitch, int a7, DWORD rop)
  {
    return TRUE;
  }

  BOOL __stdcall FakeBlt(int handle, int a2, int a3, int a4, int a5)
  {
    return TRUE;
  }

  BOOL __stdcall FakeBltUsingMask(int lpSurface, int a2, int pitch, int width, int handle)
  {
    return TRUE;
  }
};
