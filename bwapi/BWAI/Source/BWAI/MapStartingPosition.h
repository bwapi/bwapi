#pragma once
#include <list>
#include "..//..//BWAPI//Source//BW//Position.h"

class TiXmlElement;
namespace BWAI { class MapInfo; }
namespace BWAI { class MapExpansion; }

namespace BWAI
{
  class MapStartingPosition
  {
    public :
     MapStartingPosition(TiXmlElement* xmlElement, MapInfo* mapInfo);
     std::list<BW::TilePosition> nonProducing3X2BuildingPositions;
     MapExpansion* expansion;
  };
}
