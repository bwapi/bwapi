#include "Map.h"

#include <BW/Broodwar.h>
#include <BW/TileSet.h>
#include <BW/TileType.h>
#include <BW/DoodatType.h>

namespace BWAPI
{
  namespace Map
  {
//private:
    int selfPlayerId;

    static BW::TileID getTile(int x, int y);
    static u8 getTileVariation(BW::TileID);
    Util::BitMask<u16> getMiniTile(int x, int y);
    Util::RectangleArray<u32>* fogOfWar;
    Util::RectangleArray<u16>* zergCreep;
    Util::RectangleArray<bool> buildability;
    Util::RectangleArray<bool> walkability;
    void setBuildability();
    void setWalkability();
    //---------------------------------------------- -----------------------------------------------------------
//  public:
    //----------------------------------------------- GET WIDTH ------------------------------------------------
    u16 getWidth()
    {
      return *BW::BWDATA_MapSizeX;
    }
    //----------------------------------------------- GET HEIGHT -----------------------------------------------
    u16 getHeight()
    {
      return *BW::BWDATA_MapSizeY;
    }
    //---------------------------------------------- GET FILE NAME ---------------------------------------------
    std::string getFileName()
    {
      std::string mapNameAbsolute(BW::BWDATA_CurrentMapFileName);
      std::string::size_type lastDelimiterPos = mapNameAbsolute.rfind('\\');
      return mapNameAbsolute.substr(lastDelimiterPos + 1, mapNameAbsolute.size() - lastDelimiterPos - 1);
    }
    //------------------------------------------------ GET NAME ------------------------------------------------
    std::string getName()
    {
      std::string mapName(BW::BWDATA_CurrentMapName);
      return mapName;
    }
    //------------------------------------------------ BUILDABLE -----------------------------------------------
    bool buildable(int x, int y)
    {
      if ((unsigned int)x>=buildability.getWidth() || (unsigned int)y>=buildability.getHeight())
        return false;
      return buildability[x][y];
    }
    //------------------------------------------------ WALKABLE ------------------------------------------------
    bool walkable(int x, int y)
    {
      if ((unsigned int)x>=walkability.getWidth() || (unsigned int)y>=walkability.getHeight())
        return false;
      return walkability[x][y];
    }
    //------------------------------------------------ VISIBLE -------------------------------------------------
    bool visible(int x, int y)
    {
      if ((unsigned int)x>=buildability.getWidth() || (unsigned int)y>=buildability.getHeight())
        return false;
      u32 value =  (*Map::fogOfWar)[y][x];
      if (BW::isInReplay())
        return (value & 255) != 255;
      return !(value & (1 << selfPlayerId));
    }
    //--------------------------------------------- HAS EXPLORED -----------------------------------------------
    bool isExplored(int x, int y)
    {
      if ((unsigned int)x>=buildability.getWidth() || (unsigned int)y>=buildability.getHeight())
        return false;
      u32 value =  (*Map::fogOfWar)[y][x];
      value = value >> 8;
      if (BW::isInReplay())
        return (value & 255) != 255;
      return !(value & (1 << selfPlayerId));
    }
    //----------------------------------------------- HAS CREEP ------------------------------------------------
    bool hasCreep(int x, int y)
    {
      if ((unsigned int)x>=buildability.getWidth() || (unsigned int)y>=buildability.getHeight())
        return false;
      return (*Map::zergCreep)[y][x] != 0;
    }
    //--------------------------------------------- GROUND HEIGHT ----------------------------------------------
    int groundHeight(int x, int y)
    {
      if ((unsigned int)x>=walkability.getWidth() || (unsigned int)y>=walkability.getHeight())
        return false;
      int mid = Map::getMiniTile(x, y).getBit<BW::MiniTileFlags::Middle>();
      int high = Map::getMiniTile(x, y).getBit<BW::MiniTileFlags::High>();
      return mid + high * 2;
    }
    //-------------------------------------------------- LOAD --------------------------------------------------
    void load()
    {
      if (fogOfWar != NULL)
      {
        delete fogOfWar;
        fogOfWar = NULL;
      }
      buildability.resize(getWidth(), getHeight());
      walkability.resize(getWidth()*4, getHeight()*4);
      fogOfWar = new Util::RectangleArray<u32>(getHeight(), getWidth(), *BW::BWDATA_MapFogOfWar);
      zergCreep = new Util::RectangleArray<u16>(getHeight(), getWidth(), *BW::BWDATA_ZergCreepArray);
      setBuildability();
      setWalkability();
    }
    //------------------------------------------------ GET TILE ------------------------------------------------
    BW::TileID getTile(int x, int y)
    {
      return *(*BW::BWDATA_MapTileArray + x + y * getWidth());
    }
    //------------------------------------------- GET TILE VARIATION -------------------------------------------
    u8 getTileVariation(BW::TileID tileType)
    {
      return tileType & 0xF;
    }
    //-------------------------------------------- SET BUILDABILITY --------------------------------------------
    void setBuildability()
    {
      for (unsigned int y = 0; y < Map::getHeight(); y++)
        for (unsigned int x = 0; x < Map::getWidth(); x++)
          Map::buildability[x][y] = !((BW::TileSet::getTileType(Map::getTile(x, y))->buildability >> 4) & 0X8);
    }
    //-------------------------------------------- SET WALKABILITY ---------------------------------------------
    void setWalkability()
    {
      for (unsigned int y = 0; y < (u16)(Map::getHeight()*4); y++)
        for (unsigned int x = 0; x < (u16)(Map::getWidth()*4); x++)
          Map::walkability[x][y] = Map::getMiniTile(x, y).getBit<BW::MiniTileFlags::Walkable>();
    }
    //--------------------------------------------- GET MINITILE -----------------------------------------------
    Util::BitMask<u16> getMiniTile(int x, int y)
    {
      int tx = x / 4;
      int ty = y / 4;
      int mx = x % 4;
      int my = y % 4;
      BW::TileID tileID = Map::getTile(tx, ty);
      BW::TileType* tile = BW::TileSet::getTileType(tileID);
      return (*BW::BWDATA_MiniTileFlags)->tile[tile->miniTile[getTileVariation(tileID)]].miniTile[mx + my*4];
    }
    //------------------------------------------ GET MAP HASH --------------------------------------------------
    int getMapHash()
    {
      int hashval = 0;

      for (unsigned int x = 0; x < Map::getWidth(); x++)
        for (unsigned int y = 0; y < Map::getHeight(); y++)
        {
          hashval += BW::TileSet::getTileType(Map::getTile(x, y))->index;
          hashval *= x + y;
        }

      return hashval;
    }
    //----------------------------------------------------------------------------------------------------------
  }
};
