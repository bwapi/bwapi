#include "Map.h"
#include "../BW/TileSet.h"
#include "../BW/TileType.h"
#include "../../../Util/Source/Util/Exceptions.h"
namespace BWAPI
{
  //------------------------------- GET TILE --------------------------------
  BW::TileID Map::getTile(int x, int y)
  {
    return *(*BW::BWXFN_MapTileArray + x + y*Map::getWidth());
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
    fprintf(f, "    ");
    for (int x = 0; x < BWAPI::Map::getWidth(); x++)
      if (x % 10 == 0)
      {
        fprintf(f, "%d       ", x);
        if (x < 10)
          fprintf(f, " ");
        if (x < 100)
          fprintf(f, " ");
      }
    fprintf(f, "\n    ");
    for (int x = 0; x < BWAPI::Map::getWidth(); x++)
      fprintf(f, "%d", x % 10);
    fprintf(f, "\n   +");
    for (int x = 0; x < BWAPI::Map::getWidth(); x++)
      fprintf(f, "-");
    fprintf(f, "+\n");
    for (int y = 0; y < BWAPI::Map::getHeight(); y++)
    {
      fprintf(f, "%3d|", y);
      for (int x = 0; x < BWAPI::Map::getWidth(); x++)
      {
        if ((((BW::TileSet::getTileType(BWAPI::Map::getTile(x, y))->buildability >> 4) & 0X8)) == 0)
          fprintf(f, ".");
        else
          fprintf(f, "X");
      }
      fprintf(f, "|%3d\n", y);
    }
    fprintf(f, "   +");
    for (int x = 0; x < BWAPI::Map::getWidth(); x++)
      fprintf(f, "-");
    fprintf(f, "+\n");
    
    fprintf(f, "    ");
    for (int x = 0; x < BWAPI::Map::getWidth(); x++)
      fprintf(f, "%d", x % 10);
    fprintf(f, "\n    ");
    for (int x = 0; x < BWAPI::Map::getWidth(); x++)
      if (x % 10 == 0)
      {
        fprintf(f, "%d       ", x);
        if (x < 10)
          fprintf(f, " ");
        if (x < 100)
          fprintf(f, " ");
      }
    fclose(f);             
  }
  //----------------------------------- GET FILE NAME -------------------------
  char* Map::getFileName(void)
  {
    return BW::BWXFN_CurrentMapFileName;
  }
  //---------------------------------------------------------------------------
}