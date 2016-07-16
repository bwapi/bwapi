#pragma once
#include "Opcode.h"

namespace AISCRIPT
{
  class Nuke_Pos : public Opcode
  {
  public:
    // Ctor
    Nuke_Pos() : Opcode(Enum::NUKE_POS) {};

    // Execute
    virtual bool execute(aithread &thread) const;
  };
}
