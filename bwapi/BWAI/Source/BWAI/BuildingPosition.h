#include <list>
#include <string>

#include "../../../BWAPI/Source/BW/TilePosition.h"

class TiXmlElement;

namespace BWAI
{
  /** 
   * Represents set of building positions that are meant to be commutable,
   */
  class BuildingPosition
  {
    public :
      BuildingPosition(TiXmlElement* xmlElement);
      std::list<BW::TilePosition> positions;
      int tileWidth;
      int tileHeight;
      std::string shortcut;
  };
}