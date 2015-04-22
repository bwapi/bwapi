#pragma once
#include <BWAPI/Region.h>
#include <BWAPI/Regionset.h>
#include <BWAPI/Position.h>

#include <BWAPI/Client/RegionData.h>

namespace BWAPI
{
  class RegionImpl : public RegionInterface
  {
  public:
    virtual int getID() const override;
    virtual int getRegionGroupID() const override;
    virtual BWAPI::Position getCenter() const override;
    virtual bool  isHigherGround() const override;
    virtual int   getDefensePriority() const override;
    virtual bool  isAccessible() const override;

    virtual const Regionset &getNeighbors() const override;

    virtual int getBoundsLeft() const override;
    virtual int getBoundsTop() const override;
    virtual int getBoundsRight() const override;
    virtual int getBoundsBottom() const override;

    virtual BWAPI::Region getClosestAccessibleRegion() const override;
    virtual BWAPI::Region getClosestInaccessibleRegion() const override;

    RegionImpl(int id);
    void UpdateRegionRelations();
    RegionData* getData();
  private:
    RegionData  data = {};
    RegionData* self = &data;
    
    Regionset neighbors;
    BWAPI::Region closestAccessibleRgn;
    BWAPI::Region closestInaccessibleRgn;
  };

};

