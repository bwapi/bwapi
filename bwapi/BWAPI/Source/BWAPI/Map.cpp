#include "Map.h"
#include "../DLLMain.h"
#include "../Config.h"

#include <BW/Offsets.h>
#include <BW/TileSet.h>
#include <BW/TileType.h>
#include <BW/MiniTileFlags.h>
#include "GameImpl.h"
#include "PlayerImpl.h"
#include <fstream>
#include <memory>
#include <Util/sha1.h>

#include "../../../Debug.h"

using namespace std;
namespace BWAPI
{
  //----------------------------------------------- GET WIDTH ------------------------------------------------
  u16 Map::getWidth()
  {
    return BW::BWDATA::MapSize.x;
  }
  //----------------------------------------------- GET HEIGHT -----------------------------------------------
  u16 Map::getHeight()
  {
    return BW::BWDATA::MapSize.y;
  }
  //---------------------------------------------- GET PATH NAME ---------------------------------------------
  std::string Map::getPathName()
  {
    std::string mapPath( BW::BWDATA::CurrentMapFileName.data() );
    
    // If the install path is included in the map path, remove it, creating a relative path
    if ( !installPath().empty() && mapPath.compare(0, installPath().length(), installPath()) == 0 )
      mapPath.erase(0, installPath().length() );
    
    return mapPath;
  }
  //---------------------------------------------- GET FILE NAME ---------------------------------------------
  std::string Map::getFileName()
  {
    std::string mapFileName( BW::BWDATA::CurrentMapFileName.data() );
    
    // Remove the path
    size_t tmp = mapFileName.find_last_of("/\\");
    if ( tmp != std::string::npos )
      mapFileName.erase(0, tmp+1);

    return mapFileName;
  }
  //------------------------------------------------ GET NAME ------------------------------------------------
  std::string Map::getName()
  {
    return std::string{ BW::BWDATA::CurrentMapName.data() };
  }
  void Map::copyToSharedMemory()
  {
    const int width = getWidth();
    const int height = getHeight();

    GameData* const data = BroodwarImpl.server.data;
    if ( BroodwarImpl.isReplay() )
    {
      for(int x = 0; x < width; ++x)
      {
        for(int y = 0; y < height; ++y)
        {
          BW::activeTile tileData = getActiveTile(x, y);
          data->isVisible[x][y]  = tileData.bVisibilityFlags   != 255;
          data->isExplored[x][y] = tileData.bExploredFlags     != 255;
          data->hasCreep[x][y]   = tileData.bTemporaryCreep    != 0;
          data->isOccupied[x][y] = tileData.bCurrentlyOccupied != 0;
        }
      }
    }
    else
    {
      const bool completeMapInfo = Broodwar->isFlagEnabled(Flag::CompleteMapInformation);
      const u32 playerFlag = 1 << BroodwarImpl.BWAPIPlayer->getIndex();
      for(int x = 0; x < width; ++x)
      {
        for(int y = 0; y < height; ++y)
        {
          const BW::activeTile tileData = getActiveTile(x, y);
          data->isVisible[x][y]   = !(tileData.bVisibilityFlags & playerFlag);
          data->isExplored[x][y]  = !(tileData.bExploredFlags & playerFlag);
          data->hasCreep[x][y]    = (data->isVisible[x][y] || completeMapInfo) && (tileData.bTemporaryCreep != 0 || tileData.bHasCreep != 0);
          data->isOccupied[x][y]  = (data->isVisible[x][y] || completeMapInfo) && tileData.bCurrentlyOccupied != 0;
        }
      }
    }
  }
  //------------------------------------------------ BUILDABLE -----------------------------------------------
  bool Map::buildable(int x, int y)
  {
    return getActiveTile(x, y).bAlwaysUnbuildable == 0;
  }
  //------------------------------------------------ WALKABLE ------------------------------------------------
  bool Map::walkable(int x, int y)
  {
    if (y >= getHeight()*4 - 4)
      return false;
    if (y >= getHeight() * 4 - 8 && (x < 20 || x >= getWidth() * 4 - 20))
      return false;
    return (Map::getMiniTile(x, y) & BW::MiniTileFlags::Walkable) != 0;
  }
  //------------------------------------------------ VISIBLE -------------------------------------------------
  bool Map::visible(int x, int y)
  {
    BW::activeTile value = getActiveTile(x, y);
    if ( BroodwarImpl.isReplay() )
      return value.bVisibilityFlags != 255;
    return !(value.bVisibilityFlags & (1 << BroodwarImpl.BWAPIPlayer->getIndex()));
  }
  //--------------------------------------------- HAS EXPLORED -----------------------------------------------
  bool Map::isExplored(int x, int y)
  {
    BW::activeTile value = getActiveTile(x, y);
    if ( BroodwarImpl.isReplay() )
      return value.bExploredFlags != 255;
    return !(value.bExploredFlags & (1 << BroodwarImpl.BWAPIPlayer->getIndex()));
  }
  //----------------------------------------------- HAS CREEP ------------------------------------------------
  bool Map::hasCreep(int x, int y)
  {
    return getActiveTile(x, y).bTemporaryCreep != 0 || getActiveTile(x, y).bHasCreep != 0;
  }
  //---------------------------------------------- IS OCCUPIED -----------------------------------------------
  bool Map::isOccupied(int x, int y)
  {
    return getActiveTile(x, y).bCurrentlyOccupied != 0;
  }
  //--------------------------------------------- GROUND HEIGHT ----------------------------------------------
  int Map::groundHeight(int x, int y)
  {
    return getActiveTile(x, y).bGroundHeight;
  }
  //------------------------------------------------ GET TILE ------------------------------------------------
  BW::TileID Map::getTile(int x, int y)
  {
    if ( BW::BWDATA::MapTileArray && static_cast<unsigned>(x) < getWidth() && static_cast<unsigned>(y) < getHeight())
      return BW::BWDATA::MapTileArray[x + y * Map::getWidth()];
    return 0;
  }
  //------------------------------------------- GET TILE VARIATION -------------------------------------------
  u8 Map::getTileVariation(BW::TileID tileType)
  {
    return tileType & 0xF;
  }
  //--------------------------------------------- GET MINITILE -----------------------------------------------
  u16 Map::getMiniTile(int x, int y)
  {
    int tx = x / 4;
    int ty = y / 4;
    int mx = x % 4;
    int my = y % 4;
    BW::TileID tileID = BWAPI::Map::getTile(tx, ty);
    BW::TileType* tile = BW::TileSet::getTileType(tileID);
    if ( tile && BW::BWDATA::MiniTileFlags )
      return BW::BWDATA::MiniTileFlags->tile[tile->megaTileRef[Map::getTileVariation(tileID)]].miniTile[mx + my*4];
    return 0;
  }
  //------------------------------------------ GET MAP HASH --------------------------------------------------
  std::string Map::savedMapHash;

  std::string Map::calculateMapHash()
  {
    unsigned char hash[20];
    char hexstring[sizeof(hash)*2 + 1];
    std::string filename = Map::getPathName();

    // Open File
    Storm::CFile file;
    if ( !file.open(filename, SFILE_FROM_ABSOLUTE) )
    {
      filename += "\\staredit\\scenario.chk";
      if ( !file.open(filename, SFILE_FROM_MPQ) )
        return Map::savedMapHash = "Error_map_cannot_be_opened";
    }

    size_t fileSize = file.size();
    std::vector<char> data(fileSize);

    // Read file
    if ( !file.read(data.data(), fileSize) )
      return Map::savedMapHash = "Error_unable_to_read_file";

    // Calculate hash
    sha1::calc(data.data(), fileSize, hash);
    sha1::toHexString(hash, hexstring);

    return Map::savedMapHash = hexstring;
  }
  std::string Map::getMapHash()
  {
    return Map::savedMapHash;
  }
  //----------------------------------------------------------------------------------------------------------
};
