#pragma once
namespace Util
{
  template <class Type>
  class BitMask
  {
    public :
      bool getBit(Type bit) const;
    Type value;
  };
  //----------------------------------- GET BIT ----------------------------------
  template <class Type>
  bool BitMask<Type>::getBit(Type bit) const
  {
    return (value & bit) != 0;
  }
}
 