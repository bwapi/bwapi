#pragma once
#include "Opcode.h"

namespace AISCRIPT
{
  class Kill_Thread : public Opcode
  {
  public:
    // Ctor
    Kill_Thread() : Opcode(Enum::KILL_THREAD) {};

    // Execute
    virtual bool execute(aithread &thread) const;
  };
}
