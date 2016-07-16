#pragma once
#include <BWAPI/Region.h>
#include <BWAPI/Position.h>
#include <BWAPI/Client/RegionData.h>

#include <BWAPI/Regionset.h>

namespace BWAPI
{
  class RegionImpl : public RegionInterface
  {
  private:
    RegionData* self;
    Regionset neighbors;
    BWAPI::Region closestAccessibleRgn = nullptr;
    BWAPI::Region closestInaccessibleRgn = nullptr;
  public:
    RegionImpl(int index);
    void setNeighbors();
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
  };

};

