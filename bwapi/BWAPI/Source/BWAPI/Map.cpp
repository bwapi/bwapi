#include "Map.h"

#include <BW/TileSet.h>
#include <BW/TileType.h>
#include <BW/DoodatType.h>
#include "GameImpl.h"
#include "PlayerImpl.h"
#include <fstream>
#include <sys/stat.h>
#include <Util/sha1.h>

using namespace std;
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
    return BW::BWDATA_MapSize->x;
  }
  //----------------------------------------------- GET HEIGHT -----------------------------------------------
  u16 Map::getHeight()
  {
    return BW::BWDATA_MapSize->y;
  }
  //---------------------------------------------- GET PATH NAME ---------------------------------------------
  std::string Map::getPathName()
  {
    std::string mapPathName(BW::BWDATA_CurrentMapFileName);
    return mapPathName;
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
  void Map::copyToSharedMemory()
  {
    int w = buildability.getWidth();
    int h = buildability.getHeight();
    GameData* data = BroodwarImpl.server.data;
    bool completeMapInfo = Broodwar->isFlagEnabled(Flag::CompleteMapInformation);
    if (BroodwarImpl._isReplay())
    {
      for(int x = 0; x < w; ++x)
      {
        for(int y = 0; y < h; ++y)
        {
          int tileData = (*fogOfWar)[y][x];
          data->isVisible[x][y]  = (tileData & 255) != 255;
          data->isExplored[x][y] = ((tileData >> 8) & 255) != 255;
          data->hasCreep[x][y]   = (*zergCreep)[y][x] != 0;
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
          u32 tileData = (*fogOfWar)[y][x];
          data->isVisible[x][y]  = !(tileData & playerFlag);
          data->isExplored[x][y] = !((tileData >> 8) & playerFlag);
          bool canAccess = data->isVisible[x][y] || completeMapInfo;
          data->hasCreep[x][y] = canAccess && (*zergCreep)[y][x] != 0;
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
    u32 value =  (*this->fogOfWar)[y][x];
    if (BroodwarImpl._isReplay())
      return (value & 255) != 255;
    return !(value & (1 << BroodwarImpl.BWAPIPlayer->getIndex()));
  }
  //--------------------------------------------- HAS EXPLORED -----------------------------------------------
  bool Map::isExplored(int x, int y) const
  {
    if ((unsigned int)x >= buildability.getWidth() || (unsigned int)y >= buildability.getHeight())
      return false;
    u32 value = (*this->fogOfWar)[y][x];
    value = value >> 8;
    if (BroodwarImpl._isReplay())
      return (value & 255) != 255;
    return !(value & (1 << BroodwarImpl.BWAPIPlayer->getIndex()));
  }
  //----------------------------------------------- HAS CREEP ------------------------------------------------
  bool Map::hasCreep(int x, int y) const
  {
    if ((unsigned int)x >= buildability.getWidth() || (unsigned int)y >= buildability.getHeight())
      return false;
    return (*this->zergCreep)[y][x] != 0;
  }
  //--------------------------------------------- GROUND HEIGHT ----------------------------------------------
  int Map::groundHeight(int x, int y) const
  {
    if ((unsigned int)x >= buildability.getWidth() || (unsigned int)y >= buildability.getHeight())
      return 0;
    int value =  (*this->fogOfWar)[y][x];
    value = value >> 16;
    int h = 0;
    if ((value & (0x200))!=0)
      h = 1;
    if ((value & (0x400))!=0)
      h = 2;
    return h;
  }
  //-------------------------------------------------- LOAD --------------------------------------------------
  void Map::load()
  {
    if ( fogOfWar )
    {
      delete fogOfWar;
      fogOfWar = NULL;
    }
    buildability.resize(Map::getWidth(), Map::getHeight());
    walkability.resize(Map::getWidth()*4, Map::getHeight()*4);
    fogOfWar  = new Util::RectangleArray<u32>(Map::getHeight(), Map::getWidth(), BW::BWDATA_MapFogOfWar);
    zergCreep = new Util::RectangleArray<u16>(Map::getHeight(), Map::getWidth(), BW::BWDATA_ZergCreepArray);
    setBuildability();
    setWalkability();
  }
  //------------------------------------------------ GET TILE ------------------------------------------------
  BW::TileID Map::getTile(int x, int y)
  {
    return *(BW::BWDATA_MapTileArray + x + y * Map::getWidth());
  }
  //------------------------------------------- GET TILE VARIATION -------------------------------------------
  u8 Map::getTileVariation(BW::TileID tileType)
  {
    return tileType & 0xF;
  }
  //-------------------------------------------- SET BUILDABILITY --------------------------------------------
  void Map::setBuildability()
  {
    for (unsigned int y = 0; y < BWAPI::Map::getHeight(); ++y)
      for (unsigned int x = 0; x < BWAPI::Map::getWidth(); ++x)
        this->buildability[x][y] = !((BW::TileSet::getTileType(BWAPI::Map::getTile(x, y))->buildability >> 4) & 0x8);
    int y = BWAPI::Map::getHeight()-1;
    for(unsigned int x = 0; x < BWAPI::Map::getWidth();x++)
    {
      this->buildability[x][y] = false;
    }
    --y;
    for(int x = 0; x < 5; ++x)
    {
      this->buildability[x][y] = false;
      this->buildability[BWAPI::Map::getWidth() - x - 1][y] = false;
    }
  }
  //-------------------------------------------- SET WALKABILITY ---------------------------------------------
  void Map::setWalkability()
  {
    for (unsigned int y = 0; y < (u16)(BWAPI::Map::getHeight() * 4); ++y)
      for (unsigned int x = 0; x < (u16)(BWAPI::Map::getWidth() * 4); ++x)
        this->walkability[x][y] = this->getMiniTile(x, y).getBit(BW::MiniTileFlags::Walkable);
    int y = BWAPI::Map::getHeight() * 4 - 1;
    for(unsigned int x = 0; x < (unsigned int)(BWAPI::Map::getWidth() * 4); ++x)
    {
      this->walkability[x][y]   = false;
      this->walkability[x][y-1] = false;
      this->walkability[x][y-2] = false;
      this->walkability[x][y-3] = false;
    }
    y -= 4;
    for(int x = 0; x < 20; x++)
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
  Util::BitMask<u16> Map::getMiniTile(int x, int y) const
  {
    int tx = x / 4;
    int ty = y / 4;
    int mx = x % 4;
    int my = y % 4;
    BW::TileID tileID = BWAPI::Map::getTile(tx, ty);
    BW::TileType* tile = BW::TileSet::getTileType(tileID);
    return BW::BWDATA_MiniTileFlags->tile[tile->miniTile[Map::getTileVariation(tileID)]].miniTile[mx + my*4];
  }
  //------------------------------------------ GET MAP HASH --------------------------------------------------
  std::string Map::getMapHash()
  {
    unsigned char hash[20];
    char hexstring[42];
    std::string filename = Map::getPathName();

    // Open File
    HANDLE hFile = NULL;
    if ( !SFileOpenFileEx(NULL, filename.c_str(), SFILE_FROM_ABSOLUTE, &hFile) || !hFile)
    {
      char szPath[MAX_PATH];
      SStrCopy(szPath, filename.c_str(), MAX_PATH);
      SStrNCat(szPath, "\\staredit\\scenario.chk", MAX_PATH);
      if ( !SFileOpenFileEx(NULL, szPath, SFILE_FROM_MPQ, &hFile) || !hFile)
        return std::string("Error_map_cannot_be_opened");
    }

    // Obtain file size
    DWORD dwFileSize = SFileGetFileSize(hFile, 0);

    // Allocate memory
    void *pBuffer = SMemAlloc(dwFileSize, __FILE__, __LINE__, 0);
    if ( !pBuffer )
    {
      SFileCloseFile(hFile);
      return std::string("Error_could_not_allocate_memory");
    }

    // Read file
    DWORD dwBytesRead = 0;
    SFileReadFile(hFile, pBuffer, dwFileSize, &dwBytesRead, 0);

    // Calculate hash
    sha1::calc(pBuffer, dwBytesRead, hash);
    sha1::toHexString(hash, hexstring);

    // Free memory and return
    SMemFree(pBuffer, __FILE__, __LINE__, 0);
    SFileCloseFile(hFile);
    return string(hexstring);
  }
  //----------------------------------------------------------------------------------------------------------
};
