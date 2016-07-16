#pragma once
#include "Opcode.h"

namespace AISCRIPT
{
  class Place_Guard : public Opcode
  {
  public:
    // Ctor
    Place_Guard() : Opcode(Enum::PLACE_GUARD) {};

    // Execute
    virtual bool execute(aithread &thread) const;
  };
}
