#include "..//..//BWAPI//Source//BW//Position.h"
#include <string>
class  TiXmlElement;

namespace BWAI
{
  /** Handles pre-prepared data connected expansion on map. */
  class MapExpansion
  {
    public :
      /**
       * Loads the data from the specified xml element. 
       * @throws XmlException if the xml format is not correct.
       */
      MapExpansion(TiXmlElement* xmlElement);
      const BW::Position& getPosition(void) const;
      const std::string& getID(void) const;
    private :
      BW::Position position;
      std::string id;
      void setID(const std::string& id);
      void setPosition(BW::Position);
  };
}