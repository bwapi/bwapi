#include "MapInfo.h"
#include "MapExpansion.h"
#include "..//..//TinyXml//tinyxml.h"
#include "..//..//BWAPI//Source//Exceptions.h"
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
    fclose(f);
  }
  //--------------------------------- DESTRUCTOR -----------------------------
  MapInfo::~MapInfo()
  {
   for (std::list<MapExpansion*>::iterator i = this->expansions.begin(); i != this->expansions.end(); ++i)
      delete *i;
  }
  //--------------------------------------------------------------------------
}