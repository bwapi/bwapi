#include "Map.h"

#include <Util/Exceptions.h>
#include <Util/Strings.h>
#include <Util/Logger.h>
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
  //----------------------------------------- SAVE BUILDABILITY MAP ------------------------------------------
  char *Map::saveBuildabilityMap(const std::string& fileName) const
  {
    FILE* f = fopen(fileName.c_str(),"wt");
    if (!f)
      throw FileException("Couldn't save the buildability map to '" + fileName + "'");
    fprintf(f, "Buildability map for currently opened map\n");
    fprintf(f, "Map file: %s\n", Map::getFileName().c_str());
    fprintf(f, "Map width: %d\n", BWAPI::Map::getWidth());
    fprintf(f, "Map height: %d\n", BWAPI::Map::getHeight());
    fprintf(f, "X = not buildable\n");
    fprintf(f, ". = buildable\n");

    try
    {
      Util::RectangleArray<char> result = Util::RectangleArray<char>(getWidth(),getHeight());

      fprintf(f, "RectangleArray declaration succeeded.\n");
      for (unsigned int x = 0; x < getWidth(); x++)
        for (unsigned int y = 0; y < getHeight(); y++)
          result[x][y] = buildable(x,y) ? '.' : 'X';
      
      Util::Strings::makeBorder(result).printToFile(f); 
      fclose(f);
      return "Successfully saved buildability map.";
    }
    catch (GeneralException&)
    {
      fprintf(f, "Declaration of RectangleArray for fog of war map failed.");
      return "Declaration of RectangleArray for fog of war map failed.";
    }
  }
  //------------------------------------------ SAVE WALKABILITY MAP ------------------------------------------
  char *Map::saveWalkabilityMap(const std::string& fileName) const
  {
    FILE* f = fopen(fileName.c_str(),"wt");
    if (!f)
      throw FileException("Couldn't save the walkability map to '" + fileName + "'");
    fprintf(f, "Walkability map for currently opened map\n");
    fprintf(f, "Map file: %s\n", Map::getFileName().c_str());
    fprintf(f, "Map width: %d\n", BWAPI::Map::getWidth());
    fprintf(f, "Map height: %d\n", BWAPI::Map::getHeight());
    fprintf(f, "X = not walkable\n");
    fprintf(f, ". = walkable\n");
    
    try
    {
      Util::RectangleArray<char> result = Util::RectangleArray<char>(getWidth()*4, 
                                                                     getHeight()*4);

      fprintf(f, "RectangleArray declaration succeeded.\n");
      for (unsigned int x = 0; x < (u16)(getWidth()*4); x++)
        for (unsigned int y = 0; y < (u16)(getHeight()*4); y++)
          result[x][y] = walkable(x,y) ? '.' : 'X';
      
      Util::Strings::makeBorder(result).printToFile(f);
      fclose(f);
      return "Successfully saved walkability map.";
    }
    catch (GeneralException&)
    {
      fprintf(f, "Declaration of RectangleArray for fog of war map failed.");
      return "Declaration of RectangleArray for fog of war map failed.";
    }
  }
  //------------------------------------------ SAVE WALKABILITY MAP ------------------------------------------
  char *Map::saveFogOfWarMap(const std::string& fileName) const
  {
    FILE* f = fopen(fileName.c_str(),"wt");
    if (!f)
      throw FileException("Couldn't save the fog of war map to '" + fileName + "'");
    fprintf(f, "Fog of war map for currently opened map\n");
    fprintf(f, "Map file: %s\n", Map::getFileName().c_str());
    fprintf(f, "Map width: %d\n", BWAPI::Map::getWidth());
    fprintf(f, "Map height: %d\n", BWAPI::Map::getHeight());
    fprintf(f, "X = not visible\n");
    fprintf(f, ". = visible\n");
    
    try
    {
      Util::RectangleArray<char> result = Util::RectangleArray<char>(this->fogOfWar->getWidth(), this->fogOfWar->getHeight());
      for (unsigned int x = 0; x < this->fogOfWar->getWidth(); x++)
        for (unsigned int y = 0; y < this->fogOfWar->getHeight(); y++)
        {
          result[x][y] = visible(x,y) ? '.' : 'X';
        }
    
      Util::Strings::makeBorder(result).printToFile(f); 
      fclose(f);
      return "Successfully saved fog of war map.";
    }
    catch (GeneralException&)
    {
      fprintf(f, "Declaration of RectangleArray for fog of war map failed.");
      return "Declaration of RectangleArray for fog of war map failed.";
    }

  }
  //------------------------------------------------ GET FILE NAME -------------------------------------------
  std::string Map::getFileName()
  {
    std::string mapNameAbsolute(BW::BWDATA_CurrentMapFileName);
    std::string::size_type lastDelimiterPos = mapNameAbsolute.rfind('\\');
    return mapNameAbsolute.substr(lastDelimiterPos + 1, mapNameAbsolute.size() - lastDelimiterPos - 1);
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