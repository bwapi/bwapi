#include "MapInfo.h"

#include <tinyxml.h>
#include <Exceptions.h>
#include <StringUtil.h>
#include <Dictionary.h>

#include "MapExpansion.h"
#include "MapStartingPosition.h"

#include "../../../BWAPI/Source/BWAPI/Map.h"
#include "../../../BWAPI/Source/BWAPI/Game.h"
#include "../../../BWAPI/Source/BWAPI/Globals.h"

namespace BWAI
{
  //--------------------------------- CONSTRUCTOR ----------------------------
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
  //--------------------------------- DESTRUCTOR -----------------------------
  MapInfo::~MapInfo()
  {
   for (std::list<MapExpansion*>::iterator i = this->expansions.begin(); i != this->expansions.end(); ++i)
      delete *i;
  }
  //--------------------------------------------------------------------------
  MapExpansion *MapInfo::getExpansion(const std::string& id)
  {
    for (std::list<MapExpansion*>::iterator i = this->expansions.begin(); i != this->expansions.end(); ++i)
      if ((*i)->getID() == id)
        return *i;
     return NULL;
  }
  //--------------------------------------------------------------------------
  void MapInfo::saveDefinedBuildingsMap()
  {
    RectangleArray<char> buildability = BWAPI::Map::getBuildabilityArray();
    RectangleArray<int> counts = RectangleArray<int>(buildability.getWidth(), buildability.getHeight()) ;
    for (unsigned int x = 0; x < buildability.getWidth(); x++)
      for (unsigned int y = 0; y < buildability.getHeight(); y++)    
        if (buildability[x][y] == 'X')
          counts[x][y] = 1;
        else
          counts[x][y] = 0;
        
    
    for (std::list<MapStartingPosition*>::iterator i = this->startingPositions.begin();
         i != this->startingPositions.end();
         ++i)
      for (std::list<BW::TilePosition>::iterator j = (*i)->nonProducing3X2BuildingPositions.begin();
           j != (*i)->nonProducing3X2BuildingPositions.end();
           ++j)
      {
        StringUtil::makeWindow(buildability, (*j).x, (*j).y, 3, 2, 1);
        for (int x = 0; x < 3; x++)
          for (int y = 0; y < 2; y++)
            counts[x + (*j).x][y + (*j).y]++;
      }
    
    for (unsigned int x = 0; x < buildability.getWidth(); x++)
      for (unsigned int y = 0; y < buildability.getHeight(); y++)    
        if (counts[x][y] == 2)
          buildability[x][y] = char(176);
        else if (counts[x][y] == 3)
         buildability[x][y] = char(177);
       else if (counts[x][y] > 3)
         buildability[x][y] = char(178);
    StringUtil::makeBorder(buildability).saveToFile(BWAPI::Broodwar.configuration->getValue("data_path") + "\\pre-defined-buildings.txt");
  }
  //---------------------------------------------------------------------------
}