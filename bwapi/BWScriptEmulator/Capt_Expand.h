#pragma once
#include "Opcode.h"

namespace AISCRIPT
{
  class Capt_Expand : public Opcode
  {
  public:
    // Ctor
    Capt_Expand() : Opcode(Enum::CAPT_EXPAND) {};

    // Execute
    virtual bool execute(aithread &thread) const;
  };
}
