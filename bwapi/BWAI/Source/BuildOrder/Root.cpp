#include "Root.h"

#include <tinyxml.h>
#include <Util/Exceptions.h>
#include <Util/Dictionary.h>
#include <Util/FileLogger.h>

#include "Branch.h"
#include "BuildWeights.h"

#include <BWAPI/Globals.h>
#include <BWAPI/Game.h>
#include <BWAPI/ScreenLogger.h>
#include <BWAPI/Player.h>

#include <BWAI/Globals.h>

/** Things that involve ordering the ai things depending on the build-order xml definition. */
namespace BuildOrder
{
  //------------------------------ CONSTRUCTOR --------------------------------
  Root::Root(const std::string& xmlFileName)
  :log(new Util::FileLogger(BWAPI::Broodwar.configuration->getValue("log_path") + "\\build-order", Util::LogLevel::MicroDetailed))
  ,loadedTypes(false)
  {
    this->log->registerLogger(new BWAPI::ScreenLogger(Util::LogLevel::Normal));
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
  //--------------------------------------------------- DESTRUCTOR -------------------------------------------
  Root::~Root()
  {
    for each (Branch* i in this->buildOrders)
      delete i;
    this->buildOrders.clear();
    for each (std::pair<std::string, BuildWeights* > i in this->weights)
      delete i.second;
    delete log;
  }
  //----------------------------------------------- GET STARTING BRANCH --------------------------------------
  Branch* Root::getStartingBranch()
  {
    if (BWAI::ai->opponent == NULL)
    {
      this->log->log("Unknown opponent player");
      return NULL;
    }
    for each (Branch* i in this->buildOrders)
      if (i->against == BWAI::ai->opponent->getRace())
        return i;
    return NULL;
  }
  //----------------------------------------------------------------------------------------------------------
}
