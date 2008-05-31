#include "Root.h"

#include <tinyxml.h>
#include <Exceptions.h>
#include <Dictionary.h>

#include "Branch.h"
#include "BuildWeights.h"

#include "../../../BWAPI/Source/BWAPI/Globals.h"
#include "../../../BWAPI/Source/BWAPI/Game.h"


/** Things that involve ordering the ai things depending on the build-order xml definition. */
namespace BuildOrder
{
  //------------------------------ CONSTRUCTOR --------------------------------
  Root::Root(const std::string& xmlFileName)
  :log(BWAPI::Broodwar.configuration->getValue("log_path") + "\\build-order", Util::LogLevel::MicroDetailed)
  {
    FILE* f = fopen(xmlFileName.c_str(),"rb");
    if (!f)
      throw FileException("Unable to load data file " + xmlFileName);
  
    TiXmlDocument doc(xmlFileName.c_str());
    doc.LoadFile(f);
    
    TiXmlNode* node = doc.FirstChild("build-orders");
    if (node == NULL)
      throw XmlException("Expected root element <build-orders> in file:" + xmlFileName);
    TiXmlElement* root = node->ToElement();
    
    for (TiXmlElement* buildOrder = root->FirstChildElement("build-order"); buildOrder != NULL; buildOrder = buildOrder->NextSiblingElement("build-order"))
      this->buildOrders.push_back(new Branch(buildOrder));

    fclose(f);
  }
  //------------------------------- DESTRUCTOR --------------------------------
  Root::~Root()
  {
    for (std::list<Branch*>::iterator i = this->buildOrders.begin(); i != this->buildOrders.end(); ++i)
      delete *i;
    this->buildOrders.clear();
    for (std::map<std::string, BuildWeights* >::iterator i = this->weights.begin(); i != this->weights.end(); ++i)
      delete (*i).second;
  }
  //---------------------------------------------------------------------------
}
