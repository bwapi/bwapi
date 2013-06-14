#include "Opcode.h"

using namespace BWAPI;
using namespace AISCRIPT;

std::array<Opcode*,AISCRIPT::Enum::LAST> Opcode::opcodeList = {};

Opcode::Opcode(AISCRIPT::Enum::Enum id)
  : opcode(id)
{
  Opcode::opcodeList[id] = this;
}

const char * const Opcode::getName() const
{
  return AISCRIPT::getOpcodeName(this->opcode);
}

AISCRIPT::Enum::Enum Opcode::getOpcode() const
{
  return this->opcode;
}

bool Opcode::readOpcode (aithread &thread)
{
  // Read the opcode
  BYTE bOpcode = thread.read<BYTE>();

  // Check if opcode is valid
  if ( bOpcode >= AISCRIPT::Enum::LAST || !Opcode::opcodeList[bOpcode] )
  {
    Broodwar << Text::Red << "Invalid Script Opcode: " << bOpcode << std::endl;
    return false;
  }

  // Call the opcode
  bool success = Opcode::opcodeList[bOpcode]->execute(thread);

  // If the opcode execution succeeded, then we don't want to retry the opcode
  if ( success )
    thread.noretry();
  return success;
}
