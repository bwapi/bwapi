#pragma once
#include "Opcode.h"

namespace AISCRIPT
{
  class Get_OldPeons : public Opcode
  {
  public:
    // Ctor
    Get_OldPeons() : Opcode(Enum::GET_OLDPEONS) {};

    // Execute
    virtual bool execute(aithread &thread) const;
  };
}
