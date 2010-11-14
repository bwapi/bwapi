#pragma once
#include "CommandType.h"

namespace BWAPIC
{
  struct Command
  {
    Command()
    {
      type = CommandType::None;
    }
    Command(CommandType::Enum _commandType, int _value1=0, int _value2=0)
    {
      type   = _commandType;
      value1 = _value1;
      value2 = _value2;
    }
    CommandType::Enum type;
    int value1;
    int value2;
  };
}
