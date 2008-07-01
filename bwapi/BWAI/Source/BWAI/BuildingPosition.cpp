#include "BuildingPosition.h"
namespace BWAI
{
  BuildingPosition::BuildingPosition(const BW::TilePosition& position)
  :position(position)
  ,reserved(false)
  {
  }
}