#pragma once
#include "Opcode.h"

namespace AISCRIPT
{
  class Define_Max : public Opcode
  {
  public:
    // Ctor
    Define_Max() : Opcode(Enum::DEFINE_MAX) {};

    // Execute
    virtual bool execute(aithread &thread) const;
  };
}
