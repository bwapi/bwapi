#pragma once
#include "Opcode.h"

namespace AISCRIPT
{
  class Player_Need : public Opcode
  {
  public:
    // Ctor
    Player_Need() : Opcode(Enum::PLAYER_NEED) {};

    // Execute
    virtual bool execute(aithread &thread) const;
  };
}
