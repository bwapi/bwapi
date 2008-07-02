#pragma once

#include <list>
namespace BuildOrder { class Branch; }
namespace BuildOrder { class Command; }

namespace BuildOrder
{
  class CommandPointer
  {
    public :
      CommandPointer(Branch* branch);
      bool finished();
      Branch* branch;
      std::list<Command*>::iterator position;
  };
}