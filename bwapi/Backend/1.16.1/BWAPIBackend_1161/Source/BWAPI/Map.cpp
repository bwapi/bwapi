#include "Map.h"

#include <fstream>
#include <memory>
#include "../Sha1.h"

#include "../DLLMain.h"
#include "../Config.h"

#include "../BW/Offsets.h"
#include "../BW/TileSet.h"
#include "../BW/TileType.h"
#include "../BW/MiniTileFlags.h"
#include "GameImpl.h"
#include "PlayerImpl.h"

#include <BWAPI/Flag.h>

#pragma warning(push, 0)
#include <generated/cpp/bwapidata.pb.h>
#pragma warning(pop)

using namespace std;
namespace BWAPI4
{
  //----------------------------------------------- GET WIDTH ------------------------------------------------
  u16 Map::getWidth()
  {
    return BW::BWDATA::Game.mapTileSize.x;
  }
  //----------------------------------------------- GET HEIGHT -----------------------------------------------
  u16 Map::getHeight()
  {
    return BW::BWDATA::Game.mapTileSize.y;
  }
  //---------------------------------------------- GET PATH NAME ---------------------------------------------
  std::string Map::getPathName()
  {
    std::string mapPath( BW::BWDATA::Game.mapFileName );

    // If the install path is included in the map path, remove it, creating a relative path
    if (!installPath().empty() && mapPath.compare(0, installPath().length(), installPath()) == 0)
      mapPath.erase(0, installPath().length());

    return mapPath;
  }
  //---------------------------------------------- GET FILE NAME ---------------------------------------------
  std::string Map::getFileName()
  {
    std::filesystem::path mapPath( BW::BWDATA::Game.mapFileName );
    return mapPath.filename().string();
  }
  //------------------------------------------------ GET NAME ------------------------------------------------
  std::string Map::getName()
  {
    return BW::BWDATA::Game.mapTitle;
  }
  void Map::copyToSharedMemory(bwapi::data::MapData* mapData)
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
          bool value = tileData.bVisibilityFlags != 255;
          if (data->isVisible[x][y] != value)
          {
            data->isVisible[x][y] = value;
            auto newData = mapData->add_isvisible();
            newData->set_x(x);
            newData->set_y(y);
            newData->set_value(value);
          }
          value = tileData.bExploredFlags != 255;
          if (data->isExplored[x][y] != value)
          {
            data->isExplored[x][y] = value;
            auto newData = mapData->add_isexplored();
            newData->set_x(x);
            newData->set_y(y);
            newData->set_value(value);
          }
          value = tileData.bTemporaryCreep != 0;
          if (data->hasCreep[x][y] != value)
          {
            data->hasCreep[x][y] = value;
            auto newData = mapData->add_hascreep();
            newData->set_x(x);
            newData->set_y(y);
            newData->set_value(value);
          }
          value = tileData.bCurrentlyOccupied != 0;
          if (data->isOccupied[x][y] != value)
          {
            data->isOccupied[x][y] = value;
            auto newData = mapData->add_isoccupied();
            newData->set_x(x);
            newData->set_y(y);
            newData->set_value(value);
          }
        }
      }
    }
    else
    {
      const bool completeMapInfo = BroodwarImpl.isFlagEnabled(Flag::CompleteMapInformation);
      const u32 playerFlag = 1 << BroodwarImpl.BWAPIPlayer->getNationId();
      for(int x = 0; x < width; ++x)
      {
        for(int y = 0; y < height; ++y)
        {
          const BW::activeTile tileData = getActiveTile(x, y);
          bool value = !(tileData.bVisibilityFlags & playerFlag);
          if (data->isVisible[x][y] != value)
          {
            data->isVisible[x][y] = value;
            auto newData = mapData->add_isvisible();
            newData->set_x(x);
            newData->set_y(y);
            newData->set_value(value);
          }
          value = !(tileData.bExploredFlags & playerFlag);
          if (data->isExplored[x][y] != value)
          {
            data->isExplored[x][y] = value;
            auto newData = mapData->add_isexplored();
            newData->set_x(x);
            newData->set_y(y);
            newData->set_value(value);
          }
          value = (data->isVisible[x][y] || completeMapInfo) && (tileData.bTemporaryCreep != 0 || tileData.bHasCreep != 0);
          if (data->hasCreep[x][y] != value)
          {
            data->hasCreep[x][y] = value;
            auto newData = mapData->add_hascreep();
            newData->set_x(x);
            newData->set_y(y);
            newData->set_value(value);
          }
          value = (data->isVisible[x][y] || completeMapInfo) && tileData.bCurrentlyOccupied != 0;
          if (data->isOccupied[x][y] != value)
          {
            data->isOccupied[x][y] = value;
            auto newData = mapData->add_isoccupied();
            newData->set_x(x);
            newData->set_y(y);
            newData->set_value(value);
          }
        }
      }
    }
  }
  //------------------------------------------------ BUILDABLE -----------------------------------------------
  bool Map::buildable(int x, int y)
  {
    auto const tile = getActiveTile(x, y);
    return !(tile.bAlwaysUnbuildable | tile.bUnwalkable);
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
    return !(value.bVisibilityFlags & (1 << BroodwarImpl.BWAPIPlayer->getNationId()));
  }
  //--------------------------------------------- HAS EXPLORED -----------------------------------------------
  bool Map::isExplored(int x, int y)
  {
    BW::activeTile value = getActiveTile(x, y);
    if ( BroodwarImpl.isReplay() )
      return value.bExploredFlags != 255;
    return !(value.bExploredFlags & (1 << BroodwarImpl.BWAPIPlayer->getNationId()));
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
    BW::TileID tileID = BWAPI4::Map::getTile(tx, ty);
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
