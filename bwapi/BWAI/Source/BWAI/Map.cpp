#include "Map.h"
#include <Util/Exceptions.h>
#include <Util/Strings.h>
#include <BWAPI/Map.h>
#include <BWAPI/Globals.h>

namespace BWAI
{
  namespace Map
  {
    //----------------------------------------- SAVE BUILDABILITY MAP ------------------------------------------
    std::string saveBuildabilityMap(const std::string& fileName)
    {
      FILE* f = fopen(fileName.c_str(),"wt");
      if (!f)
        throw FileException("Couldn't save the buildability map to '" + fileName + "'");
      fprintf_s(f, "Buildability map for currently opened map\n");
      fprintf_s(f, "Map file: %s\n", BWAPI::Map::getFileName().c_str());
      fprintf_s(f, "Map width: %d\n", BWAPI::Map::getWidth());
      fprintf_s(f, "Map height: %d\n", BWAPI::Map::getHeight());
      fprintf_s(f, "X = not buildable\n");
      fprintf_s(f, ". = buildable\n");

      try
      {
        Util::RectangleArray<char> result = Util::RectangleArray<char>(BWAPI::Map::getWidth(),BWAPI::Map::getHeight());

        fprintf_s(f, "RectangleArray declaration succeeded.\n");
        for (unsigned int x = 0; x < BWAPI::Map::getWidth(); x++)
          for (unsigned int y = 0; y < BWAPI::Map::getHeight(); y++)
            result[x][y] = BWAPI::Broodwar.map.buildable(x,y) ? '.' : 'X';
        
        Util::Strings::makeBorder(result).printToFile(f); 
        fclose(f);
        return std::string("Successfully saved buildability map.");
      }
      catch (GeneralException&)
      {
        fprintf_s(f, "Declaration of RectangleArray for fog of war map failed.");
        return std::string("Declaration of RectangleArray for fog of war map failed.");
      }
    }
    //------------------------------------------ SAVE WALKABILITY MAP ------------------------------------------
    std::string saveWalkabilityMap(const std::string& fileName)
    {
      FILE* f = fopen(fileName.c_str(),"wt");
      if (!f)
        throw FileException("Couldn't save the walkability map to '" + fileName + "'");
      fprintf_s(f, "Walkability map for currently opened map\n");
      fprintf_s(f, "Map file: %s\n", BWAPI::Map::getFileName().c_str());
      fprintf_s(f, "Map width: %d\n", BWAPI::Map::getWidth());
      fprintf_s(f, "Map height: %d\n", BWAPI::Map::getHeight());
      fprintf_s(f, "X = not walkable\n");
      fprintf_s(f, ". = walkable\n");
      
      try
      {
        Util::RectangleArray<char> result = Util::RectangleArray<char>(BWAPI::Map::getWidth()*4, 
                                                                       BWAPI::Map::getHeight()*4);

        fprintf_s(f, "RectangleArray declaration succeeded.\n");
        for (unsigned int x = 0; x < (u16)(BWAPI::Map::getWidth()*4); x++)
          for (unsigned int y = 0; y < (u16)(BWAPI::Map::getHeight()*4); y++)
            result[x][y] = BWAPI::Broodwar.map.walkable(x,y) ? '.' : 'X';
        
        Util::Strings::makeBorder(result).printToFile(f);
        fclose(f);
        return std::string("Successfully saved walkability map.");
      }
      catch (GeneralException&)
      {
        fprintf_s(f, "Declaration of RectangleArray for fog of war map failed.");
        return std::string("Declaration of RectangleArray for fog of war map failed.");
      }
    }
    //------------------------------------------ SAVE WALKABILITY MAP ------------------------------------------
    std::string saveFogOfWarMap(const std::string& fileName)
    {
      FILE* f = fopen(fileName.c_str(),"wt");
      if (!f)
        throw FileException("Couldn't save the fog of war map to '" + fileName + "'");
      fprintf_s(f, "Fog of war map for currently opened map\n");
      fprintf_s(f, "Map file: %s\n", BWAPI::Map::getFileName().c_str());
      fprintf_s(f, "Map width: %d\n", BWAPI::Map::getWidth());
      fprintf_s(f, "Map height: %d\n", BWAPI::Map::getHeight());
      fprintf_s(f, "X = not visible\n");
      fprintf_s(f, ". = visible\n");
      
      try
      {
        Util::RectangleArray<char> result = Util::RectangleArray<char>(BWAPI::Broodwar.map.getWidth(), BWAPI::Broodwar.map.getHeight());
        for (unsigned int x = 0; x < BWAPI::Broodwar.map.getWidth(); x++)
          for (unsigned int y = 0; y < BWAPI::Broodwar.map.getHeight(); y++)
          {
            result[x][y] = BWAPI::Broodwar.map.visible(x,y) ? '.' : 'X';
          }
      
        Util::Strings::makeBorder(result).printToFile(f); 
        fclose(f);
        return std::string("Successfully saved fog of war map.");
      }
      catch (GeneralException&)
      {
        fprintf_s(f, "Declaration of RectangleArray for fog of war map failed.");
        return std::string("Declaration of RectangleArray for fog of war map failed.");
      }

    }
  }
}