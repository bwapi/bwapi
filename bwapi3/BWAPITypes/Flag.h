#pragma once
namespace BWAPI
{
  typedef int Flag;
  namespace Flags
  {
    enum Enum
    {
      CompleteMapInformation = 0,
      UserInput              = 1,
    };
    static const int count   = 2;
  }
}
