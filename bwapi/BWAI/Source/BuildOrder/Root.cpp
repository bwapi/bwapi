#include "Root.h"

#include <tinyxml.h>
#include <Util/Exceptions.h>
#include <Util/Dictionary.h>
#include <Util/FileLogger.h>
#include <Util/Xml.h>
#include <BWAPI/Globals.h>
#include <BWAPI/Game.h>
#include <BWAPI/ScreenLogger.h>
#include <BWAPI/Player.h>
#include <BWAI/Globals.h>
#include "Branch.h"
#include "RootBranch.h"
#include "BuildWeights.h"
#include "Executor.h"

/** Things that involve ordering the ai things depending on the build-order xml definition. */
namespace BuildOrder
{
  //------------------------------ CONSTRUCTOR --------------------------------
  Root::Root(const std::string& xmlFileName)
  :log(new Util::FileLogger(config->get("log_path") + "\\build-order", Util::LogLevel::MicroDetailed))
  {
    this->log->registerLogger(new BWAPI::ScreenLogger(Util::LogLevel::Normal));
    include(xmlFileName);
  }
  //---------------------------------------------------- INCLUDE ---------------------------------------------
  void Root::include(const std::string& xmlFileName)
  {
    if (this->includedFiles.count(xmlFileName))
      return; // already included
    
    this->includedFiles.insert(xmlFileName);
    FILE* f = fopen(xmlFileName.c_str(),"rb");
    if (!f)
      throw FileException("Unable to load data file " + xmlFileName);

    TiXmlDocument doc(xmlFileName.c_str());
    doc.LoadFile(f);

    TiXmlNode* node = doc.FirstChild("build-orders");
    if (node == NULL)
      throw XmlException("Expected root element <build-orders> in file:" + xmlFileName);
    TiXmlElement* root = node->ToElement();

    for (TiXmlElement* includeElement = root->FirstChildElement("include"); includeElement != NULL; includeElement = includeElement->NextSiblingElement("include"))
      this->include(config->get("build_order_directory") + "\\" + Util::Xml::getRequiredAttribute(includeElement,"name"));
    
    try
    {
      for (TiXmlElement* buildOrder = root->FirstChildElement("build-order"); buildOrder != NULL; buildOrder = buildOrder->NextSiblingElement("build-order"))
      {
        std::string type = Util::Xml::getRequiredAttribute(buildOrder, "type");
        std::string name = Util::Xml::getRequiredAttribute(buildOrder, "name");
        if (type == "root")
          this->buildOrders.push_back(new RootBranch(buildOrder));
        else if (type == "function")
          this->functions.insert(std::pair<std::string, Branch*>(name, new Branch(buildOrder)));
        else
          throw XmlException("Unknown build-order type '" + type + "'");
      }
    }
    catch (GeneralException& e)
    {
      throw GeneralException((std::string)"Error in build order file '" + xmlFileName + "' :" + e.getMessage());
    }
    fclose(f);
  }
  //--------------------------------------------------- DESTRUCTOR -------------------------------------------
  Root::~Root()
  {
    for each (std::pair<std::string, Branch*> i in this->functions)
      delete i.second;
    for each (RootBranch* i in this->buildOrders)
      delete i;
    for each (std::pair<std::string, BuildWeights* > i in this->weights)
      delete i.second;
    delete log;
  }
  //----------------------------------------------- GET STARTING BRANCH --------------------------------------
  Executor* Root::getStart()
  {
    if (BWAI::ai->opponent == NULL)
    {
      this->log->log("Unknown opponent player");
      return NULL;
    }
    for each (RootBranch* i in this->buildOrders)
      if (i->against == BWAI::ai->opponent->getRace())
        return new Executor(i);
    return NULL;
  }
  //----------------------------------------------------------------------------------------------------------
}
