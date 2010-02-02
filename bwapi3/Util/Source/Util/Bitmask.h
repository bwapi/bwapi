#pragma once
/** Custom help classes not connected with the project */
namespace Util
{
  /** 
   * Representation of list of bool values using binary.
   * Size of the bitmask is always the same as Type (so it can be mapped in bw structurs)
   * This also means, that the bitmask has sizeof(Type)*8 values.
   */
  template <class Type>
  struct BitMask
  {
    Type value;
    //------------------------------------------------ GET BIT -------------------------------------------------
    bool getBit(Type mask) const
    {
      return (value & mask) != 0;
    }
    //------------------------------------------------ GET CONST BIT -------------------------------------------
    template <int mask>
      bool getBit() const
      {
        return (value & mask) != 0;
      }
  };
}
