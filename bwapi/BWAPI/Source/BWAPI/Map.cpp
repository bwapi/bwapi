#include "Map.h"

#include <BW/TileSet.h>
#include <BW/TileType.h>
#include <BW/DoodatType.h>
#include <BWAPI/Globals.h>
#include <BWAPI/Player.h>

namespace BWAPI
{
  //---------------------------------------------- CONSTRUCTOR -----------------------------------------------
  Map::Map()
  :fogOfWar(NULL)
  {
  }
  //----------------------------------------------- DESTRUCTOR -----------------------------------------------
  Map::~Map()
  {
  }
  //----------------------------------------------- GET WIDTH ------------------------------------------------
  u16 Map::getWidth()
  {
    return *BW::BWDATA_MapSizeX;
  }
  //----------------------------------------------- GET HEIGHT -----------------------------------------------
  u16 Map::getHeight()
  {
    return *BW::BWDATA_MapSizeY;
  }
  //---------------------------------------------- GET FILE NAME ---------------------------------------------
  std::string Map::getFileName()
  {
    std::string mapNameAbsolute(BW::BWDATA_CurrentMapFileName);
    std::string::size_type lastDelimiterPos = mapNameAbsolute.rfind('\\');
    return mapNameAbsolute.substr(lastDelimiterPos + 1, mapNameAbsolute.size() - lastDelimiterPos - 1);
  }
  //------------------------------------------------ GET NAME ------------------------------------------------
  std::string Map::getName()
  {
    std::string fileName=getFileName();
    int i=fileName.find_last_of('.');
    if (i!=std::string::npos)
    {
      return fileName.substr(0,i);
    }
    return fileName;
  }
  //------------------------------------------------ BUILDABLE -----------------------------------------------
  bool Map::buildable(int x, int y) const
  {
    return buildability[x][y];
  }
  //------------------------------------------------ WALKABLE ------------------------------------------------
  bool Map::walkable(int x, int y) const
  {
    return walkability[x][y];
  }
  //------------------------------------------------ VISIBLE -------------------------------------------------
  bool Map::visible(int x, int y) const
  {
    u32 value =  (*this->fogOfWar)[y][x];
    return !(value & (1<<Broodwar.BWAPIPlayer->getID()));
  }
  //-------------------------------------------------- LOAD --------------------------------------------------
  void Map::load()
  {
    if (fogOfWar!=NULL)
    {
      delete fogOfWar;
      fogOfWar=NULL;
    }
    buildability.resize(Map::getWidth(), Map::getHeight());
    walkability.resize(Map::getWidth()*4, Map::getHeight()*4);
    fogOfWar=new Util::RectangleArray<u32>(Map::getHeight(), Map::getWidth(), *BW::BWDATA_MapFogOfWar);
    setBuildability();
    setWalkability();
  }
  //------------------------------------------------ GET TILE ------------------------------------------------
  BW::TileID Map::getTile(int x, int y)
  {
    return *(*BW::BWDATA_MapTileArray + x + y*Map::getWidth());
  }
  //------------------------------------------- GET TILE VARIATION -------------------------------------------
  u8 Map::getTileVariation(BW::TileID tileType)
  {
    return tileType & 0xF;
  }
  //-------------------------------------------- SET BUILDABILITY --------------------------------------------
  void Map::setBuildability()
  {
    for (unsigned int y = 0; y < BWAPI::Map::getHeight(); y++)
      for (unsigned int x = 0; x < BWAPI::Map::getWidth(); x++)
        this->buildability[x][y] = !((BW::TileSet::getTileType(BWAPI::Map::getTile(x, y))->buildability >> 4) & 0X8);
  }
  //-------------------------------------------- SET WALKABILITY ---------------------------------------------
  void Map::setWalkability()
  {
    for (unsigned int y = 0; y < BWAPI::Map::getHeight(); y++)
      for (unsigned int x = 0; x < BWAPI::Map::getWidth(); x++)
      {
        BW::TileID tileID = BWAPI::Map::getTile(x, y);
        BW::TileType* tile = BW::TileSet::getTileType(tileID);
        for (unsigned int my = 0; my < 4; my++)
          for (unsigned int mx = 0; mx < 4; mx++)
            this->walkability[x*4 + mx][y*4 + my] = 
              (*BW::BWDATA_MiniTileFlags)->tile[tile->miniTile[Map::getTileVariation(tileID)]].miniTile[mx + my*4].getBit(BW::MiniTileFlags::Walkable);
     }
  }
  //----------------------------------------------------------------------------------------------------------
}