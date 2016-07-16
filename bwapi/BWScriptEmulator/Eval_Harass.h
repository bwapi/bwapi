#pragma once
#include "Opcode.h"

namespace AISCRIPT
{
  class Eval_Harass : public Opcode
  {
  public:
    // Ctor
    Eval_Harass() : Opcode(Enum::EVAL_HARASS) {};

    // Execute
    virtual bool execute(aithread &thread) const;
  };
}
