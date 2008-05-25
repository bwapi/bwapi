#include <list>
#include <string>

#include "../../../BWAPI/Source/BW/TilePosition.h"

class TiXmlElement;

namespace BWAI
{
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