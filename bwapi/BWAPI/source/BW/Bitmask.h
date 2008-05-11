namespace BW
{
  template <class Type>
  class BitMask
  {
    Type value;
    bool getBit(Type bit);
  };
  //----------------------------------- GET BIT ----------------------------------
  template <class Type>
  bool BitMask<Type>::getBit(Type bit)
  {
    return (value & bit);
  }
}
 