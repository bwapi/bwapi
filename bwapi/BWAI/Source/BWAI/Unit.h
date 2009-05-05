#pragma once

#include <list>

#include <BWAPI/Unit.h>

namespace BWAI  { class Task; }
namespace BWAI  { class Expansion; }

namespace BWAI
{
  /** 
   * Wrapper of BWAPI unit type with included information that uses BWAI
   * Note that there is some small waste 
   */
  class Unit : public BWAPI::Unit
  {
    public :
      Unit(BWAPI::Unit *unit);
      Expansion* expansion;
      static Unit* BWUnitToBWAIUnit(BW::Unit* unit);
      static Unit* BWAPIUnitToBWAIUnit(BWAPI::Unit* unit);
      /** 
       * Gets #bwUnit->BW#Unit#targetUnit. 
       * This is similar to BWAPI#Unit#getTarget, it just get's the unit from ai scope
       */
      Unit* getTarget();
      Unit* getTargetLocal();
      Unit* getOrderTarget();
      Unit* getOrderTargetLocal();
      Unit* getNext();
      int lastFrameSpam;
      
      Task* getTask();
      void freeFromTask();
      void clearTask();
      void setTask(Task* task);
      std::list<Unit*>::iterator taskListIterator;
    private :
      Task* task;
  };
}