#include "Map.h"

#include <Exceptions.h>
#include <StringUtil.h>
#include <Logger.h>

#include "../BW/TileSet.h"
#include "../BW/TileType.h"
#include "../BW/DoodatType.h"

namespace BWAPI
{
  //------------------------------- GET TILE --------------------------------
  BW::TileID Map::getTile(int x, int y)
  {
    return *(*BW::BWXFN_MapTileArray + x + y*Map::getWidth());
  }
  //---------------------------- GET TILE VARIATION ------------------------
  u8 Map::getTileVariation(BW::TileID tileType)
  {
    return tileType & 0xF;
  }
  //------------------------------ GET WIDTH --------------------------------
  u16 Map::getWidth()
  {
    return *BW::BWXFN_MapSizeX;
  }
  //------------------------------ GET HEIGHT -------------------------------
  u16 Map::getHeight()
  {
    return *BW::BWXFN_MapSizeY;
  }
  //------------------------------- SAVE BUILDABILITY MAP -------------------
  void Map::saveBuildabilityMap(const std::string& fileName)
  {
    FILE* f = fopen(fileName.c_str(),"wt");
    if (!f)
      throw FileException("Couldn't save the buildability map to '" + fileName + "'");
    fprintf(f, "Buildability map for currently opened map\n");
    fprintf(f, "Map file: %s\n", Map::getFileName());
    fprintf(f, "Map width: %d\n", BWAPI::Map::getWidth());
    fprintf(f, "Map height: %d\n", BWAPI::Map::getHeight());
    fprintf(f, "X = not buildable\n");
    fprintf(f, ". = buildable\n");
    Util::RectangleArray<char> buildability = Map::getBuildabilityArray();
    Util::RectangleArray<char> withBorder = Util::StringUtil::makeBorder(buildability);
    withBorder.printToFile(f); 
    fclose(f);             
  }
   //------------------------------- SAVE BUILDABILITY MAP -------------------
  void Map::saveWalkabilityMap(const std::string& fileName)
  {
    FILE* f = fopen(fileName.c_str(),"wt");
    if (!f)
      throw FileException("Couldn't save the walkability map to '" + fileName + "'");
    fprintf(f, "Walkability map for currently opened map\n");
    fprintf(f, "Map file: %s\n", Map::getFileName());
    fprintf(f, "Map width: %d\n", BWAPI::Map::getWidth());
    fprintf(f, "Map height: %d\n", BWAPI::Map::getHeight());
    fprintf(f, "X = not walkable\n");
    fprintf(f, ". = walkable\n");
    Util::RectangleArray<char> walkability = Map::getWalkabilityArray();
    Util::RectangleArray<char> withBorder = Util::StringUtil::makeBorder(walkability);
    withBorder.printToFile(f); 
    fclose(f);             
  }
  //----------------------------------- GET FILE NAME -------------------------
  char* Map::getFileName(void)
  {
    return BW::BWXFN_CurrentMapFileName;
  }
  //------------------------------- GET BUILDABILITY ARRAY --------------------
  Util::RectangleArray<char> Map::getBuildabilityArray()
  {
    Util::RectangleArray<char> returnValue(Map::getWidth(), Map::getHeight());
    for (unsigned int y = 0; y < BWAPI::Map::getHeight(); y++)
      for (unsigned int x = 0; x < BWAPI::Map::getWidth(); x++)
        if ((((BW::TileSet::getTileType(BWAPI::Map::getTile(x, y))->buildability >> 4) & 0X8)) == 0)
          returnValue[x][y] = '.';
        else
          returnValue[x][y] = 'X';
    return returnValue;
  }
  //---------------------------------------------------------------------------
  Util::RectangleArray<char> Map::getWalkabilityArray()
  {
    Util::RectangleArray<char> returnValue(Map::getWidth()*4, Map::getHeight()*4);
    for (unsigned int y = 0; y < BWAPI::Map::getHeight(); y++)
      for (unsigned int x = 0; x < BWAPI::Map::getWidth(); x++)
      {
        BW::TileID tileID = BWAPI::Map::getTile(x, y);
        BW::TileType* tile = BW::TileSet::getTileType(tileID);
        for (unsigned int my = 0; my < 4; my++)
          for (unsigned int mx = 0; mx < 4; mx++)
            if ((*BW::BWXFN_MiniTileFlags)->tile[tile->miniTile[Map::getTileVariation(tileID)]].miniTile[mx + my*4].getBit(BW::MiniTileFlags::Walkable))
              returnValue[x*4 + mx][y*4 + my] = '.';
            else
              returnValue[x*4 + mx][y*4 + my] = 'X';
      }
    return returnValue;
  }
  //---------------------------------------------------------------------------
}