#pragma once

#include <list>
#include <map>

#include <BW/TilePosition.h>

class TiXmlElement;
namespace BWAI { class MapInfo; }
namespace BWAI { class MapExpansion; }
namespace BWAI { class BuildingPositionSet; }

namespace BWAI
{
 /** Represents (unoccupied/occupied) map expansions, used to determine where to build buildings. */
  class MapStartingPosition
  {
    public :
     MapStartingPosition(TiXmlElement* xmlElement, MapInfo* mapInfo);
     ~MapStartingPosition();
     std::map<std::string, BuildingPositionSet*> positions;
     MapExpansion* expansion;
  };
}
