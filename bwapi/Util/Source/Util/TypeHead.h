#pragma once

namespace Util
{
  //------------------------------------------------
  // Wrapper for typed structures
  //------------------------------------------------
  template<int TYPEID>
    struct TypeHead
    {
      int _type;

      TypeHead() : _type(TYPEID) {};
      static const int _typeId = TYPEID;

    };
}
