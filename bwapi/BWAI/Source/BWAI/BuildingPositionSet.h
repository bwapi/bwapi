#include <list>
#include <string>

#include "BuildingPosition.h"

class TiXmlElement;

namespace BWAI
{
  /** Represents set of building positions that are meant to be commutable. */
  class BuildingPositionSet

  {
    public :
      /** Loads the position set from xml element. */
      BuildingPositionSet(TiXmlElement* xmlElement);
      ~BuildingPositionSet();
      std::list<BuildingPosition*> positions;
      int tileWidth;
      int tileHeight;
      std::string shortcut;
  };
}