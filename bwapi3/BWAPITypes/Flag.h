#pragma once
namespace BWAPI
{
  namespace Flags
  {
    enum Enum : u32
    {
      CompleteMapInformation = 0,
      UserInput              = 1,
    };
    static const int count   = 2;
  }
  typedef Flags::Enum Flag;
}
