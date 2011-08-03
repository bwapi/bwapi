#pragma once
#include <set>
#include <vector>
#include <BWAPI/Region.h>
#include <BWAPI/Position.h>

#include <BWAPI/Client/RegionData.h>

namespace BWAPI
{
  class RegionImpl : public Region
  {
  public:
    virtual int getID() const;
    virtual int getRegionGroupID() const;
    virtual BWAPI::Position getCenter() const;
    virtual bool isHigherGround() const;
    virtual int getDefensePriority() const;
    virtual bool isWalkable() const;

    virtual const std::set<Region*> &getNeighbors() const;

    virtual int getBoundsLeft() const;
    virtual int getBoundsTop() const;
    virtual int getBoundsRight() const;
    virtual int getBoundsBottom() const;

    RegionImpl(int id);
    ~RegionImpl();
    void UpdateRegionRelations();
    void AddPoint(int x, int y);
    std::vector<BWAPI::Position> &getSimplePolygon();
    void SimplifyPolygon();
    RegionData *getData();
  private:
    RegionData data;
    RegionData *self;
    
    std::set<Region*> neighbors;
    std::vector<BWAPI::Position> polygon;
  };

};

