#pragma once
#include "Opcode.h"

namespace AISCRIPT
{
  class Wait_FinishAttack : public Opcode
  {
  public:
    // Ctor
    Wait_FinishAttack() : Opcode(Enum::WAIT_FINISHATTACK) {};

    // Execute
    virtual bool execute(aithread &thread) const;
  };
}
