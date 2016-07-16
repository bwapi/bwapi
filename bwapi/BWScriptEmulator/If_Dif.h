#pragma once
#include "Opcode.h"

namespace AISCRIPT
{
  class If_Dif : public Opcode
  {
  public:
    // Ctor
    If_Dif() : Opcode(Enum::IF_DIF) {};

    // Execute
    virtual bool execute(aithread &thread) const;
  };
}
