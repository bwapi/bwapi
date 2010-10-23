#pragma once
#include <BWAPI/Game.h>
#include <BWAPI/Player.h>
#include <BWAPI/Unit.h>
#include <Util/Foreach.h>
namespace BWAPI
{
  namespace Templates
  {
    //------------------------------------------ CAN BUILD HERE ----------------------------------------------
    template <class GameImpl, class PlayerImpl, class UnitImpl>
    bool _canBuildHere(Unit* builder, TilePosition position, UnitType type, bool checkExplored)
    {
      Broodwar->setLastError(Errors::Unbuildable_Location);
      int width  = type.tileWidth();
      int height = type.tileHeight();

      int left   = position.x();
      int top    = position.y();
      int right  = left + width;
      int bottom = top + height;

      /* Map limit Check */
      if (left < 0) // left
        return false;
      if (top < 0) // top
        return false;
      if (right > Broodwar->mapWidth()) // right
        return false;
      if (bottom >= Broodwar->mapHeight()) // bottom
        return false;

      //if the unit is a refinery, we just need to check the set of geysers to see if the position
      //matches one of them (and the type is still vespene geyser)
      if (type.isRefinery())
      {
        foreach (Unit* g, Broodwar->getGeysers())
        {
          if (g->getTilePosition() == position)
          {
            if (g->isVisible() && g->getType() != UnitTypes::Resource_Vespene_Geyser)
              return false;
            return Broodwar->setLastError(Errors::None);
          }
        }
        return false;
      }

      /* Tile buildability check */
      for(int ix = left; ix < right; ++ix)
      {
        for(int iy = top; iy < bottom; ++iy)
        {
          // Check if tile is buildable and explored
          if ( !Broodwar->isBuildable(ix, iy) || ( checkExplored && !Broodwar->isExplored(ix, iy)) )
            return false; // @TODO: Error code for !isExplored ??

          // Check if builder is capable of reaching the building site
          if ( builder && !builder->hasPath( (Position)TilePosition(ix, iy) ) )
            return false;
        }
      }

      /* Ground unit dimension check */
      int targetX = left * 32 + type.tileWidth()  * 32 / 2;
      int targetY = top  * 32 + type.tileHeight() * 32 / 2;
      for(int ix = left; ix < right; ++ix)
      {
        for(int iy = top; iy < bottom; ++iy)
        {
          foreach(Unit *u, Broodwar->unitsOnTile(ix,iy))
          {
            if ( u != builder &&
                 u->getPosition().x() + u->getType().dimensionRight() >= targetX - type.dimensionLeft()  &&
                 u->getPosition().y() + u->getType().dimensionDown()  >= targetY - type.dimensionUp()    &&
                 u->getPosition().x() - u->getType().dimensionLeft()  <= targetX + type.dimensionRight() &&
                 u->getPosition().y() - u->getType().dimensionUp()    <= targetY + type.dimensionDown() )
              return false;
          }
        }
      }

      /* Creep Check */
      if (type.getRace() == BWAPI::Races::Zerg )
      {
        //Most Zerg buildings can only be built on creep
        if ( !type.isResourceDepot() )
          for(int ix = left; ix < right; ++ix)
            for(int iy = top; iy < bottom; ++iy)
              if (!Broodwar->hasCreep(ix, iy))
                return false;
      }
      else
      {
        //Non-zerg buildings cannot be built on creep
        for(int ix = left; ix < right; ++ix)
          for(int iy = top; iy < bottom; ++iy)
            if (Broodwar->hasCreep(ix, iy))
              return false;
      }

      /* Power Check */
      if ( type.requiresPsi() && !Broodwar->hasPower(left, top, width, height) )
        return false;

      /* Resource Check (CC, Nex, Hatch) */
      if (type.isResourceDepot())
      {
        foreach (BWAPI::Unit* m, Broodwar->getStaticMinerals())
        {
          if (Broodwar->isVisible(m->getInitialTilePosition()) ||
              Broodwar->isVisible(m->getInitialTilePosition().x() + 1, m->getInitialTilePosition().y()))
            if (!m->isVisible())
              continue; // tile position is visible, but mineral is not => mineral does not exist
          if (m->getInitialTilePosition().x() > left - 5 &&
              m->getInitialTilePosition().y() > top  - 4 &&
              m->getInitialTilePosition().x() < left + 7 &&
              m->getInitialTilePosition().y() < top  + 6)
            return false;
        }
        foreach (BWAPI::Unit* g, Broodwar->getStaticGeysers())
        {
          if (g->getInitialTilePosition().x() > left - 7 &&
              g->getInitialTilePosition().y() > top  - 5 &&
              g->getInitialTilePosition().x() < left + 7 &&
              g->getInitialTilePosition().y() < top  + 6)
            return false;
        }
      }
      //if the build site passes all these tests, return true.
      return Broodwar->setLastError(Errors::None);
    }
  }
}