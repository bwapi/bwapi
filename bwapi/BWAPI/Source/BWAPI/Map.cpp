#include "Map.h"
#include "../DLLMain.h"
#include "../Config.h"

#include <BW/TileSet.h>
#include <BW/TileType.h>
#include <BW/MiniTileFlags.h>
#include "GameImpl.h"
#include "PlayerImpl.h"
#include <fstream>
#include <Util/sha1.h>

#include "../../../Debug.h"

using namespace std;
namespace BWAPI
{
  //---------------------------------------------- CONSTRUCTOR -----------------------------------------------
  Map::Map()
      : activeTiles(nullptr)
  {
  }
  //----------------------------------------------- DESTRUCTOR -----------------------------------------------
  Map::~Map()
  {
    if ( activeTiles )
    {
      delete activeTiles;
      activeTiles = nullptr;
    }
  }
  //----------------------------------------------- GET WIDTH ------------------------------------------------
  u16 Map::getWidth()
  {
    return BW::BWDATA::MapSize->x;
  }
  //----------------------------------------------- GET HEIGHT -----------------------------------------------
  u16 Map::getHeight()
  {
    return BW::BWDATA::MapSize->y;
  }
  //---------------------------------------------- GET PATH NAME ---------------------------------------------
  std::string Map::getPathName()
  {
    std::string mapPath( BW::BWDATA::CurrentMapFileName );
    
    // If the install path is included in the map path, remove it, creating a relative path
    if ( !installPath.empty() && mapPath.compare(0, installPath.length(), installPath) == 0 )
      mapPath.erase(0, installPath.length() );
    
    return mapPath;
  }
  //---------------------------------------------- GET FILE NAME ---------------------------------------------
  std::string Map::getFileName()
  {
    std::string mapFileName( BW::BWDATA::CurrentMapFileName );
    
    // Remove the path
    size_t tmp = mapFileName.find_last_of("/\\");
    if ( tmp != std::string::npos )
      mapFileName.erase(0, tmp+1);

    return mapFileName;
  }
  //------------------------------------------------ GET NAME ------------------------------------------------
  std::string Map::getName()
  {
    std::string mapName( BW::BWDATA::CurrentMapName );
    return mapName;
  }
  void Map::copyToSharedMemory()
  {
    int w = buildability.getWidth();
    int h = buildability.getHeight();
    GameData* data = BroodwarImpl.server.data;
    bool completeMapInfo = Broodwar->isFlagEnabled(Flag::CompleteMapInformation);
    if ( BroodwarImpl.isReplay() )
    {
      for(int x = 0; x < w; ++x)
      {
        for(int y = 0; y < h; ++y)
        {
          BW::activeTile tileData = (*activeTiles)[y][x];
          data->isVisible[x][y]  = tileData.bVisibilityFlags   != 255;
          data->isExplored[x][y] = tileData.bExploredFlags     != 255;
          data->hasCreep[x][y]   = tileData.bTemporaryCreep    != 0;
          data->isOccupied[x][y] = tileData.bCurrentlyOccupied != 0;
        }
      }
    }
    else
    {
      int playerIndex = BroodwarImpl.BWAPIPlayer->getIndex();
      u32 playerFlag = 1 << playerIndex;
      for(int x = 0; x < w; ++x)
      {
        for(int y = 0; y < h; ++y)
        {
          BW::activeTile tileData = (*activeTiles)[y][x];
          data->isVisible[x][y]   = !(tileData.bVisibilityFlags & playerFlag);
          data->isExplored[x][y]  = !(tileData.bExploredFlags & playerFlag);
          data->hasCreep[x][y]    = (data->isVisible[x][y] || completeMapInfo) && tileData.bTemporaryCreep != 0;
          data->isOccupied[x][y]  = (data->isVisible[x][y] || completeMapInfo) && tileData.bCurrentlyOccupied != 0;
        }
      }
    }
  }
  //------------------------------------------------ BUILDABLE -----------------------------------------------
  bool Map::buildable(int x, int y) const
  {
    if ((unsigned int)x >= buildability.getWidth() || (unsigned int)y >= buildability.getHeight())
      return false;
    return buildability[x][y];
  }
  //------------------------------------------------ WALKABLE ------------------------------------------------
  bool Map::walkable(int x, int y) const
  {
    if ((unsigned int)x >= walkability.getWidth() || (unsigned int)y >= walkability.getHeight())
      return false;
    return walkability[x][y];
  }
  //------------------------------------------------ VISIBLE -------------------------------------------------
  bool Map::visible(int x, int y) const
  {
    if ((unsigned int)x >= buildability.getWidth() || (unsigned int)y >= buildability.getHeight())
      return false;
    BW::activeTile value = (*this->activeTiles)[y][x];
    if ( BroodwarImpl.isReplay() )
      return value.bVisibilityFlags != 255;
    return !(value.bVisibilityFlags & (1 << BroodwarImpl.BWAPIPlayer->getIndex()));
  }
  //--------------------------------------------- HAS EXPLORED -----------------------------------------------
  bool Map::isExplored(int x, int y) const
  {
    if ((unsigned int)x >= buildability.getWidth() || (unsigned int)y >= buildability.getHeight())
      return false;
    BW::activeTile value = (*this->activeTiles)[y][x];
    if ( BroodwarImpl.isReplay() )
      return value.bExploredFlags != 255;
    return !(value.bExploredFlags & (1 << BroodwarImpl.BWAPIPlayer->getIndex()));
  }
  //----------------------------------------------- HAS CREEP ------------------------------------------------
  bool Map::hasCreep(int x, int y) const
  {
    if ((unsigned int)x >= buildability.getWidth() || (unsigned int)y >= buildability.getHeight())
      return false;
    return (*this->activeTiles)[y][x].bTemporaryCreep != 0 || (*this->activeTiles)[y][x].bHasCreep != 0;
  }
  //---------------------------------------------- IS OCCUPIED -----------------------------------------------
  bool Map::isOccupied(int x, int y) const
  {
    if ((unsigned int)x >= buildability.getWidth() || (unsigned int)y >= buildability.getHeight())
      return false;
    return (*this->activeTiles)[y][x].bCurrentlyOccupied != 0;
  }
  //--------------------------------------------- GROUND HEIGHT ----------------------------------------------
  int Map::groundHeight(int x, int y) const
  {
    if ((unsigned int)x >= buildability.getWidth() || (unsigned int)y >= buildability.getHeight())
      return 0;
    return (*this->activeTiles)[y][x].bGroundHeight;
  }
  //-------------------------------------------------- LOAD --------------------------------------------------
  void Map::load()
  {
    if ( activeTiles )
    {
      delete activeTiles;
      activeTiles = nullptr;
    }
    buildability.resize(Map::getWidth(), Map::getHeight());
    walkability.resize(Map::getWidth()*4, Map::getHeight()*4);
    activeTiles = new Util::RectangleArray<BW::activeTile>(Map::getHeight(), Map::getWidth(), *BW::BWDATA::ActiveTileArray);
    setBuildability();
    setWalkability();
  }
  //------------------------------------------------ GET TILE ------------------------------------------------
  BW::TileID Map::getTile(int x, int y)
  {
    if ( *BW::BWDATA::MapTileArray )
      return *((*BW::BWDATA::MapTileArray) + x + y * Map::getWidth());
    return 0;
  }
  //------------------------------------------- GET TILE VARIATION -------------------------------------------
  u8 Map::getTileVariation(BW::TileID tileType)
  {
    return tileType & 0xF;
  }
  //-------------------------------------------- SET BUILDABILITY --------------------------------------------
  void Map::setBuildability()
  {
    u16 h = BWAPI::Map::getHeight();
    u16 w = BWAPI::Map::getWidth();
    for (unsigned int y = 0; y < h; ++y)
      for (unsigned int x = 0; x < w; ++x)
        this->buildability[x][y] = (*this->activeTiles)[y][x].bAlwaysUnbuildable == 0;
  }
  //-------------------------------------------- SET WALKABILITY ---------------------------------------------
  void Map::setWalkability()
  {
    u16 h = BWAPI::Map::getHeight() * 4;
    u16 w = BWAPI::Map::getWidth() * 4;
    for (unsigned int y = 0; y < h; ++y)
      for (unsigned int x = 0; x < w; ++x)
        this->walkability[x][y] = (this->getMiniTile(x, y) & BW::MiniTileFlags::Walkable) != 0;
    int y = h - 1;
    for(unsigned int x = 0; x < w; ++x)
    {
      this->walkability[x][y]   = false;
      this->walkability[x][y-1] = false;
      this->walkability[x][y-2] = false;
      this->walkability[x][y-3] = false;
    }
    y -= 4;
    for(int x = 0; x < 20; ++x)
    {
      this->walkability[x][y]   = false;
      this->walkability[x][y-1] = false;
      this->walkability[x][y-2] = false;
      this->walkability[x][y-3] = false;
      this->walkability[getWidth()*4 - x - 1][y]   = false;
      this->walkability[getWidth()*4 - x - 1][y-1] = false;
      this->walkability[getWidth()*4 - x - 1][y-2] = false;
      this->walkability[getWidth()*4 - x - 1][y-3] = false;
    }
  }
  //--------------------------------------------- GET MINITILE -----------------------------------------------
  u16 Map::getMiniTile(int x, int y) const
  {
    int tx = x / 4;
    int ty = y / 4;
    int mx = x % 4;
    int my = y % 4;
    BW::TileID tileID = BWAPI::Map::getTile(tx, ty);
    BW::TileType* tile = BW::TileSet::getTileType(tileID);
    if ( tile && BW::BWDATA::MiniTileFlags )
      return (*BW::BWDATA::MiniTileFlags)->tile[tile->megaTileRef[Map::getTileVariation(tileID)]].miniTile[mx + my*4];
    return 0;
  }
  //------------------------------------------ GET MAP HASH --------------------------------------------------
  std::string Map::getMapHash()
  {
    unsigned char hash[20];
    char hexstring[42];
    std::string filename = Map::getPathName();

    // Open File
    Storm::CFile file;
    if ( !file.open(filename, SFILE_FROM_ABSOLUTE) )
    {
      filename += "\\staredit\\scenario.chk";
      if ( !file.open(filename, SFILE_FROM_MPQ) )
        return std::string("Error_map_cannot_be_opened");
    }

    // Obtain file size
    size_t fileSize = file.size();

    // Allocate memory
    void *pBuffer = SMAlloc(fileSize);
    if ( !pBuffer )
      return std::string("Error_could_not_allocate_memory");

    // Read file
    if ( !file.read(pBuffer, fileSize) )
      return std::string("Error_unable_to_read_file");

    // Calculate hash
    sha1::calc(pBuffer, fileSize, hash);
    sha1::toHexString(hash, hexstring);

    // Free memory and return
    SMFree(pBuffer);
    return string(hexstring);
  }
  //----------------------------------------------------------------------------------------------------------
};
