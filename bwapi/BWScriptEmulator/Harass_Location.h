#pragma once
#include "Opcode.h"

namespace AISCRIPT
{
  class Harass_Location : public Opcode
  {
  public:
    // Ctor
    Harass_Location() : Opcode(Enum::HARASS_LOCATION) {};

    // Execute
    virtual bool execute(aithread &thread) const;
  };
}
