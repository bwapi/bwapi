#pragma once
namespace BW
{
  template <class Type>
  class BitMask
  {
    public :
      bool getBit(Type bit);
    Type value;
  };
  //----------------------------------- GET BIT ----------------------------------
  template <class Type>
  bool BitMask<Type>::getBit(Type bit)
  {
    return (value & bit) != 0;
  }
}
 