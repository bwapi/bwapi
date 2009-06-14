#include "BuildingPosition.h"
namespace BWAI
{
  BuildingPosition::BuildingPosition(const BWAPI::TilePosition& position)
  :position(position)
  ,reserved(false)
  {
  }
}