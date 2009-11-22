#include "Map.h"

#include <BW/TileSet.h>
#include <BW/TileType.h>
#include <BW/DoodatType.h>
#include "Globals.h"
#include "GameImpl.h"
#include "PlayerImpl.h"

namespace BWAPI
{
  //---------------------------------------------- CONSTRUCTOR -----------------------------------------------
  Map::Map()
      : fogOfWar(NULL)
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
    std::string mapName(BW::BWDATA_CurrentMapName);
    return mapName;
  }
  //------------------------------------------------ BUILDABLE -----------------------------------------------
  bool Map::buildable(int x, int y) const
  {
    if ((unsigned int)x>=buildability.getWidth() || (unsigned int)y>=buildability.getHeight())
      return false;
    return buildability[x][y];
  }
  //------------------------------------------------ WALKABLE ------------------------------------------------
  bool Map::walkable(int x, int y) const
  {
    if ((unsigned int)x>=walkability.getWidth() || (unsigned int)y>=walkability.getHeight())
      return false;
    return walkability[x][y];
  }
  //------------------------------------------------ VISIBLE -------------------------------------------------
  bool Map::visible(int x, int y) const
  {
    if ((unsigned int)x>=buildability.getWidth() || (unsigned int)y>=buildability.getHeight())
      return false;
    u32 value =  (*this->fogOfWar)[y][x];
    return !(value & (1 << BroodwarImpl.BWAPIPlayer->getID()));
  }
  //----------------------------------------------- HAS CREEP ------------------------------------------------
  bool Map::hasCreep(int x, int y) const
  {
    if ((unsigned int)x>=buildability.getWidth() || (unsigned int)y>=buildability.getHeight())
      return false;
    return (*this->zergCreep)[y][x] != 0;
  }
  //--------------------------------------------- GROUND HEIGHT ----------------------------------------------
  int Map::groundHeight(int x, int y) const
  {
    if ((unsigned int)x>=walkability.getWidth() || (unsigned int)y>=walkability.getHeight())
      return false;
    int mid = this->getMiniTile(x, y).getBit(BW::MiniTileFlags::Middle);
    int high = this->getMiniTile(x, y).getBit(BW::MiniTileFlags::High);
    return mid + high * 2;
  }
  //-------------------------------------------------- LOAD --------------------------------------------------
  void Map::load()
  {
    if (fogOfWar != NULL)
    {
      delete fogOfWar;
      fogOfWar = NULL;
    }
    buildability.resize(Map::getWidth(), Map::getHeight());
    walkability.resize(Map::getWidth()*4, Map::getHeight()*4);
    fogOfWar = new Util::RectangleArray<u32>(Map::getHeight(), Map::getWidth(), *BW::BWDATA_MapFogOfWar);
    zergCreep = new Util::RectangleArray<u16>(Map::getHeight(), Map::getWidth(), *BW::BWDATA_ZergCreepArray);
    setBuildability();
    setWalkability();
  }
  //------------------------------------------------ GET TILE ------------------------------------------------
  BW::TileID Map::getTile(int x, int y)
  {
    return *(*BW::BWDATA_MapTileArray + x + y * Map::getWidth());
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
    for (unsigned int y = 0; y < (u16)(BWAPI::Map::getHeight()*4); y++)
      for (unsigned int x = 0; x < (u16)(BWAPI::Map::getWidth()*4); x++)
        this->walkability[x][y] = this->getMiniTile(x, y).getBit(BW::MiniTileFlags::Walkable);
  }
  //--------------------------------------------- GET MINITILE -----------------------------------------------
  Util::BitMask<u16> Map::getMiniTile(int x, int y) const
  {
    int tx = x / 4;
    int ty = y / 4;
    int mx = x % 4;
    int my = y % 4;
    BW::TileID tileID = BWAPI::Map::getTile(tx, ty);
    BW::TileType* tile = BW::TileSet::getTileType(tileID);
    return (*BW::BWDATA_MiniTileFlags)->tile[tile->miniTile[Map::getTileVariation(tileID)]].miniTile[mx + my*4];
  }
  //------------------------------------------ GET MAP HASH --------------------------------------------------
  int Map::getMapHash()
  {
    int hashval = 0;

    for (unsigned int x = 0; x < BWAPI::Map::getWidth(); x++)
      for (unsigned int y = 0; y < BWAPI::Map::getHeight(); y++)
      {
        hashval += BW::TileSet::getTileType(BWAPI::Map::getTile(x, y))->index;
        hashval *= x + y;
      }

    return hashval;
  }
  //----------------------------------------------------------------------------------------------------------
}