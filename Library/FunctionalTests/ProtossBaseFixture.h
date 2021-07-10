#pragma once
#include <gtest/gtest.h>


#include "TestFixture.h"

#include <BWAPI.h>
#include "game.h"

using namespace BWAPI;

class ProtossBaseFixture : public TestFixture {
public:
  ProtossBaseFixture()
  {
    using namespace Funtest;
    UnitType mineralTypes[] = { UnitTypes::Resource_Mineral_Field, UnitTypes::Resource_Mineral_Field_Type_2, UnitTypes::Resource_Mineral_Field_Type_3 };

    TilePosition nexusPos{ 16, 10 };
    TilePosition mineralsPos[] = {
      nexusPos + TilePosition{7, -3},
      nexusPos + TilePosition{6, -2},
      nexusPos + TilePosition{7, -1},
      nexusPos + TilePosition{6, 0},
      nexusPos + TilePosition{6, 1},
      nexusPos + TilePosition{7, 2},
      nexusPos + TilePosition{7, 3},
      nexusPos + TilePosition{6, 4}
    };
    TilePosition gasPos = nexusPos + TilePosition{3, -6};
    
    funGame->createUnit(funGame->self(), UnitTypes::Protoss_Nexus, Position(nexusPos));
    funGame->createUnit(funGame->self(), UnitTypes::Protoss_Probe, Position(nexusPos), 4);
    //funGame->createUnit(funGame->self(), UnitTypes::Resource_Vespene_Geyser, Position(gasPos));
    for (int i = 0; i < std::extent<decltype(mineralsPos)>::value; ++i) {
      funGame->createUnit(funGame->self(), mineralTypes[i % 3], Position(mineralsPos[i]));
    }

    funGame.advance(2); // We can't access the created units for 2 frames for some reason

    Unitset units = funGame->getAllUnits();
    nexus = *units.find_if(Filter::GetType == UnitTypes::Protoss_Nexus);
    //vespeneGeyser = *units.find_if(Filter::GetType == UnitTypes::Resource_Vespene_Geyser);
    mineralField = *units.find_if(Filter::IsMineralField);
    probes = units.erase_if(Filter::GetType != UnitTypes::Protoss_Probe);
  }

  virtual ~ProtossBaseFixture() {}
protected:
  Unitset probes = {};
  Unit nexus = nullptr;
  Unit vespeneGeyser = nullptr;
  Unit mineralField = nullptr;
};
