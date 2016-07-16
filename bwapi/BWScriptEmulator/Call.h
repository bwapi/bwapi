#pragma once
#include "Opcode.h"

namespace AISCRIPT
{
  class Call : public Opcode
  {
  public:
    // Ctor
    Call() : Opcode(Enum::CALL) {};

    // Execute
    virtual bool execute(aithread &thread) const;
  };
}
