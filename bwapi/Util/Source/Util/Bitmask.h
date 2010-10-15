#pragma once
/** Custom help classes not connected with the project */
namespace Util
{
  /** 
   * Representation of list of bool values using binary.
   * Size of the bitmask is always the same as Type (so it can be mapped in bw structurs)
   * This also means, that the bitmas has sizeof(Type)*8 values.
   */
  template <class Type>
  class BitMask
  {
    public :
      bool getBit(Type bit) const;
      void setBit(Type bit, bool val);
    Type value;
  };
  //------------------------------------------------ GET BIT -------------------------------------------------
  template <class Type>
  bool BitMask<Type>::getBit(Type bit) const
  {
    return (value & bit) != 0;
  }
  //------------------------------------------------ SET BIT -------------------------------------------------
  template <class Type>
  void BitMask<Type>::setBit(Type bit, bool val)
  {
    if (val)
      value |= bit;
    else
      value &= ~bit;
  }
}
