#pragma once
#include "Opcode.h"

namespace AISCRIPT
{
  class Send_Suicide : public Opcode
  {
  public:
    // Ctor
    Send_Suicide() : Opcode(Enum::SEND_SUICIDE) {};

    // Execute
    virtual bool execute(aithread &thread) const;
  };
}
