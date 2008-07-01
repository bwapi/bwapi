#include "ReservedResources.h"

namespace BWAPI
{
  //-------------------------------------------- CONSTRUCTOR -------------------------------------------------
  ReservedResources::ReservedResources()
  :minerals(0)
  ,gas(0)
  ,supply(0)
  ,reservedIncome(0)
  {
  }
  //-------------------------------------------- CONSTRUCTOR -------------------------------------------------
  ReservedResources::ReservedResources(u16 minerals, u16 gas, u16 supply)
  :minerals(minerals)
  ,gas(gas)
  ,supply(supply)
  ,reservedIncome(0)
  {
  }
  //-------------------------------------------- OPERATOR += -------------------------------------------------
  void ReservedResources::operator += (const ReservedResources& reserved)
  {
    this->minerals += reserved.minerals;
    this->gas += reserved.gas;
    this->supply += reserved.supply;
    this->reservedIncome += reserved.reservedIncome;
  }
  //----------------------------------------------- CLEAR ----------------------------------------------------
  void ReservedResources::clear()
  {
    this->minerals = 0;
    this->gas = 0;
    this->supply = 0;
    this->reservedIncome = 0;
  }  
  //----------------------------------------------------------------------------------------------------------
}