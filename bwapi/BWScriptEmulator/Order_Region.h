#pragma once
#include "Opcode.h"

namespace AISCRIPT
{
  class Order_Region : public Opcode
  {
  public:
    // Ctor
    Order_Region(AISCRIPT::Enum::Enum n) : Opcode(n) {};

    // Execute
    virtual bool execute(aithread &thread) const;
  };
}
