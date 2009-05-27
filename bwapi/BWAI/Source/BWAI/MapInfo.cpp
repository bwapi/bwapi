#include "MapInfo.h"

#include <tinyxml.h>
#include <Util/Exceptions.h>
#include <Util/Strings.h>
#include <Util/Dictionary.h>
#include <BWAPI/Game.h>
#include <BWAPI/Globals.h>

#include "MapExpansion.h"
#include "MapStartingPosition.h"
#include "AI.h"
#include "Globals.h"
#include "Unit.h"
#include "BuildingPositionSet.h"

namespace BWAI
{
  //---------------------------------------------- CONSTRUCTOR -----------------------------------------------
  MapInfo::MapInfo(const std::string& xmlFileName)
  {
    FILE* f = fopen(xmlFileName.c_str(),"rb");
    if (!f)
      throw FileException("Unable to load data file " + xmlFileName);
  
    TiXmlDocument doc(xmlFileName.c_str());
    doc.LoadFile(f);
    
    TiXmlNode* node = doc.FirstChild("map-description");
    if (node == NULL)
      throw XmlException("Expected root element map-description in file:" + xmlFileName);
    TiXmlElement* root = node->ToElement();
    
    TiXmlNode* expansions = root->FirstChild("expansions");
    if (expansions == NULL)
      throw XmlException("Expected element <expansions> in <map-description> in file:" + xmlFileName);

    for (TiXmlElement* expansion = expansions->FirstChildElement("expansion"); expansion != NULL; expansion = expansion->NextSiblingElement("expansion"))
      this->expansions.push_back(new MapExpansion(expansion));


    TiXmlNode* startingPositions = root->FirstChild("starting-positions");
    if (startingPositions == NULL)
      throw XmlException("Expected element <starting-positions> in <map-description> in file:" + xmlFileName);

    for (TiXmlElement* startingPosition = startingPositions->FirstChildElement("starting-position"); 
         startingPosition != NULL; 
         startingPosition = startingPosition->NextSiblingElement("starting-position"))
      this->startingPositions.push_back(new MapStartingPosition(startingPosition, this));

    fclose(f);
  }
  //----------------------------------------------- DESTRUCTOR -----------------------------------------------
  MapInfo::~MapInfo()
  {
    for each (MapExpansion* i in this->expansions)
       delete i;
  }
  //----------------------------------------------------------------------------------------------------------
  MapExpansion *MapInfo::getExpansion(const std::string& id)
  {
    for each (MapExpansion* i in this->expansions)
      if (i->getID() == id)
        return i;
     return NULL;
  }
  //----------------------------------------------------------------------------------------------------------
  void MapInfo::saveDefinedBuildingsMap(const std::string& fileName)
  {
    Util::RectangleArray<char> result = Util::RectangleArray<char>(BWAPI::Broodwar.mapWidth(),
                                                                   BWAPI::Broodwar.mapHeight());
    for (int x = 0; x < BWAPI::Broodwar.mapWidth(); x++)
      for (int y = 0; y < BWAPI::Broodwar.mapHeight(); y++)
        result[x][y] = BWAPI::Broodwar.buildable(x,y) ? '.' : 'X';
        
    Util::RectangleArray<int> counts = Util::RectangleArray<int>(BWAPI::Broodwar.mapWidth(),
                                                                 BWAPI::Broodwar.mapHeight()) ;
    for (int x = 0; x < BWAPI::Broodwar.mapWidth(); x++)
      for (int y = 0; y < BWAPI::Broodwar.mapHeight(); y++)    
        counts[x][y] = BWAPI::Broodwar.buildable(x,y) ? 0 : 1;
        
    
    for each (MapStartingPosition* i in this->startingPositions)
    {
      for each (std::pair<std::string, BuildingPositionSet*> l in i->positions)
      {           
        BuildingPositionSet* positions = l.second;
        for each (BuildingPosition* j in positions->positions)
        {
          Util::Strings::makeWindow(result, j->position.x, j->position.y, positions->tileWidth, positions->tileHeight, 1);
          Util::Strings::printTo(result, positions->shortcut, j->position.x + 1, j->position.y + 1);
          for (int x = 0; x < positions->tileWidth; x++)
            for (int y = 0; y < positions->tileHeight; y++)
              counts[x + j->position.x][y + j->position.y]++;
        }
      }
      unsigned int startX = i->expansion->getPosition().x/BW::TILE_SIZE - 2;
      unsigned int startY = (i->expansion->getPosition().y - 45)/BW::TILE_SIZE;
      Util::Strings::makeWindow(result, startX, startY, 4, 3, 0);
        for (int x = 0; x < 4; x++)
          for (int y = 0; y < 3; y++)
            counts[x + startX][y + startY]++;      
    }
    for each (Unit* i in BWAI::ai->units)
    {
      if (i->isMineral())
        {
          unsigned int startX = i->getPosition().x/BW::TILE_SIZE - 1;
          unsigned int startY = i->getPosition().y/BW::TILE_SIZE;
          counts[startX][startY]++;
          counts[startX + 1][startY]++;
          result[startX][startY] = 'M';
          result[startX + 1][startY] = 'M';
        }
      if (i->getType() == BW::UnitID::Resource_VespeneGeyser)
      {
        unsigned int startX = i->getPosition().x/BW::TILE_SIZE - 2;
        unsigned int startY = i->getPosition().y/BW::TILE_SIZE - 1;
        for (int x = 0; x < 4; x++)
          for (int y = 0; y < 2; y++)
          {
            counts[x + startX][y + startY]++;
            result[startX + x][startY + y] = 'V';
          }
      }
    }  
    for (int x = 0; x < BWAPI::Broodwar.mapWidth(); x++)
      for (int y = 0; y < BWAPI::Broodwar.mapHeight(); y++)    
        if (counts[x][y] == 2)
          result[x][y] = char(176);
        else if (counts[x][y] == 3)
         result[x][y] = char(177);
       else if (counts[x][y] > 3)
         result[x][y] = char(178);
    Util::Strings::makeBorder(result).saveToFile(fileName);
  }
  //----------------------------------------------------------------------------------------------------------
}