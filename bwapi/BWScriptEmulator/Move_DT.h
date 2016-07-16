#pragma once
#include "Opcode.h"

namespace AISCRIPT
{
  class Move_DT : public Opcode
  {
  public:
    // Ctor
    Move_DT() : Opcode(Enum::MOVE_DT) {};

    // Execute
    virtual bool execute(aithread &thread) const;
  };
}
