#include "GameDrawing.h"

#include <BW/CImage.h>
#include <BW/CSprite.h>
#include <BW/CThingy.h>
#include <BW/Dialog.h>
#include <BW/Offsets.h>
#include <BW/TileType.h>

#include <algorithm>


BW::Bitmap bmpTerrainCache(640+32, 480-32);

void blitTileCache()
{
  BW::BWDATA::GameScreenBuffer->blitBitmap(&bmpTerrainCache);
}

void updateImageDrawingData()
{
  for ( int y = 0; y < BW::BWDATA::MapSize->y; ++y )
  {
    for ( BW::CSprite *s = BW::BWDATA::spriteGroups[y]; s; s = s->next )
    {
      for ( BW::CImage *i = s->underlay; i; i = i->next )
        i->updateGraphicData();
    }
  }
}

void blitGameText(int line, int x, int y)
{
  if ( BW::BWDATA::Chat_GameText[line].txt[0] )
  {
    char szString[256];
    szString[0] = 0;
    if ( BW::BWDATA::Chat_ColorBytes[line] )
    {
      szString[0] = BW::BWDATA::Chat_ColorBytes[line];
      szString[1] = 0;
    }
    strcat(szString, BW::BWDATA::Chat_GameText[line].txt);
    BW::BWDATA::GameScreenBuffer->blitString(szString, x, y);
  }
}

void drawGameText()   // @TODO: Text Alignments
{
  int l = *BW::BWDATA::Chat_NextLine;

  int y = 112;
  for ( int i = 0; i < 11; ++i )
  {
    blitGameText(l, 10, y);
    y += *BW::BWDATA::Chat_IncrementY;
    l = (l + 1) % 11;
  }
  blitGameText(12, 10, 295);
  blitGameText(11, 420, 24);
}

void drawThingys()
{
  if ( *BW::BWDATA::wantThingyUpdate )
  {
    BW::CThingy *pThingyFirst = (*BW::BWDATA::ThingyList_UsedFirst);
    for ( BW::CImage *p = pThingyFirst->sprite->underlay; p; p = p->next )
      p->updateGraphicData();

    pThingyFirst->sprite->draw();
    for ( BW::CImage *p = pThingyFirst->sprite->underlay; p; p = p->next )
      p->flags |= 1;
  }
}

BW::activeTile *getActiveTile(int x, int y)
{
  return &(*BW::BWDATA::ActiveTileArray)[x + y * BW::BWDATA::MapSize->x];
}

WORD *getMtxTile(int x, int y)
{
  return &(*BW::BWDATA::MapTileArray)[x + y * BW::BWDATA::MapSize->x];
}

WORD *getCellTile(int x, int y)
{
  return &(*BW::BWDATA::CellMap)[x + y * BW::BWDATA::MapSize->x];
}

bool hasCreep(int x, int y)
{
  return (*getMtxTile(x,y) & 0x7FF0) == 0x10;
}

WORD getTileRef(WORD tile)
{
  return (*BW::BWDATA::TileSet)[(tile >> 4) & 0x7FF].megaTileRef[tile & 0xF];
}


BW::Position directions[] = { 
  BW::Position( 1, 1),
  BW::Position( 0, 1),
  BW::Position(-1, 1),
  BW::Position( 1, 0),
  BW::Position(-1, 0),
  BW::Position( 1,-1),
  BW::Position( 0,-1),
  BW::Position(-1,-1),
  BW::Position( 0, 0)
};

// bool (__stdcall *cb)(int direction, WORD *pMtxTile, int x, int y, int *lParam);
void IterateDirectionalCallback( bool (__stdcall *cb)(int,WORD*,int,int,int*), int x, int y, int *lParam = nullptr)
{
  BW::Position origin((short)x,(short)y);
  for ( int i = 0; i < countof(directions); ++i )
  {
    BW::Position p = origin + directions[i];
    if ( p.isValid() && !cb(i, getMtxTile(p.x, p.y), p.x, p.y, lParam) )
      return;
  }
}

void drawMinitileImageData(WORD wTileImageIdx, int targx, int targy)
{
  // in pixels
  bool isFlipped = !!(wTileImageIdx & 1);
  wTileImageIdx >>= 1;
  const BW::vr4entry &imageData = (*BW::BWDATA::VR4Data)[wTileImageIdx];

  for ( int y = 0; y < 8; ++y )
    for ( int x = 0; x < 8; ++x )
      bmpTerrainCache.plot(targx+x, targy+y, imageData.cdata[y][isFlipped ? 8 - x - 1 : x]);
}

void drawMegatileImageData(WORD tile, int targTileX, int targTileY)
{
  // in pixels/8
  bool isCreepTile = !!(tile & 0x8000);
  
  BW::Position scrPixelPos( BW::TilePosition((short)targTileX, (short)targTileY) - *BW::BWDATA::MoveToTile );

  tile &= 0x7FFF;
  BW::vx4entry &entry = (*BW::BWDATA::VX4Data)[tile];
  for ( int y = 0; y < 4; ++y )
    for ( int x = 0; x < 4; ++x )
      drawMinitileImageData(entry.wImageRef[y][x], scrPixelPos.x + x*8, scrPixelPos.y + y*8);

  // Render creep tiles
  if ( isCreepTile )
  {
    // Get creep edge data
    u8 creepID = (*BW::BWDATA::CreepEdgeData)[ targTileX + targTileY * BW::BWDATA::MapSize->x ];
    if ( creepID != 0 )   // Render terrain GRP for creep edge (if exists) to terrain cache
      bmpTerrainCache.BlitGraphic(*BW::BWDATA::TerrainGraphics, creepID-1, scrPixelPos.x, scrPixelPos.y);
  }
}

void drawMapTiles()
{
  BW::TilePosition moveTo = *BW::BWDATA::MoveToTile;
  BW::TilePosition mapMax = *BW::BWDATA::MapSize;
  int win_wid = BW::BWDATA::GameScreenBuffer->width() + 32;
  int win_hgt = BW::BWDATA::GameScreenBuffer->height() - 32;

  for ( int y = moveTo.y; y < std::min( (short)(moveTo.y + win_hgt/32), mapMax.y); ++y )
  {
    for ( int x = moveTo.x; x < std::min( (short)(moveTo.x + win_wid/32), mapMax.x); ++x )
    {
      WORD tileRef = getTileRef( *getMtxTile(x,y) );
      WORD *drawTile = getCellTile(x,y);
      //if ( tileRef != (*drawTile & 0x7FFF) )
      {
        *drawTile = tileRef;
        getActiveTile(x,y)->bTemporaryCreep = hasCreep(x,y);

        IterateDirectionalCallback(BW::BWFXN_CreepManagementCB, x, y);

        drawMegatileImageData(*drawTile, x, y );
        *BW::BWDATA::HasMegatileUpdate = true;
      }

    }
  }
}

void GameUpdate(BW::Bitmap* /*pSurface*/, BW::bounds* /*pBounds*/)
{
  // if gameLayer.bits & 1 )
  {
    // maskSomething0();
    // memcpy(sgpMaskMapCopy, sgpCurrMaskMap, 408);
    updateImageDrawingData();   // BW::BWFXN_updateImageData();
    // maskSomething2();
    
    drawMapTiles();     // BW::BWFXN_drawMapTiles();

    blitTileCache();    // BW::BWFXN_blitMapTiles();
  }
  // else
  //{
  //  maskSomething1();
  //  refreshImageRange(_top_y, _bot_y);
  //  maskSomething2();
  //  drawMapTiles();
  //  blitMapTiles2();
  //}

  //
  BW::BWFXN_drawAllSprites();// sprites
  // Space tileset stars/parallax thing goes here
  // updateAllFog(gameLayer.bits & 1);
  drawGameText();
  BW::BWFXN_drawDragSelBox();
  
  drawThingys();      // BW::BWFXN_drawAllThingys();
}
