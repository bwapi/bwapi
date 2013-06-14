#pragma once
#include "Opcode.h"

namespace AISCRIPT
{
  class Wait_Train : public Opcode
  {
  public:
    // Ctor
    Wait_Train() : Opcode(Enum::WAIT_TRAIN) {};

    // Execute
    virtual bool execute(aithread &thread) const;
  };
}
