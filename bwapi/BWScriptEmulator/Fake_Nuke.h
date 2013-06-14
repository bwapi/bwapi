#pragma once
#include "Opcode.h"

namespace AISCRIPT
{
  class Fake_Nuke : public Opcode
  {
  public:
    // Ctor
    Fake_Nuke() : Opcode(Enum::FAKE_NUKE) {};

    // Execute
    virtual bool execute(aithread &thread) const;
  };
}
