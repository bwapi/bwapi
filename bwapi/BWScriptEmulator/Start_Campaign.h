#pragma once
#include "Opcode.h"

namespace AISCRIPT
{
  class Start_Campaign : public Opcode
  {
  public:
    // Ctor
    Start_Campaign() : Opcode(Enum::START_CAMPAIGN) {};

    // Execute
    virtual bool execute(aithread &thread) const;
  };
}
