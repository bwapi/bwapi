#pragma once
#include "Opcode.h"

namespace AISCRIPT
{
  class Help_IfTrouble : public Opcode
  {
  public:
    // Ctor
    Help_IfTrouble() : Opcode(Enum::HELP_IFTROUBLE) {};

    // Execute
    virtual bool execute(aithread &thread) const;
  };
}
