#pragma once
#include "stdafx.h"
#include "CppUnitTest.h"

#include <BWAPI.h>

namespace Microsoft{
  namespace VisualStudio {
    namespace CppUnitTestFramework
    {
      
#define SPECIALIZE(N) \
      template <> static std::wstring ToString(const BWAPI::N& q) { RETURN_WIDE_STRING(q); } \
      template <> static std::wstring ToString(const BWAPI::N* q) { RETURN_WIDE_STRING(q); } \
      template <> static std::wstring ToString(BWAPI::N* q)       { RETURN_WIDE_STRING(q); }

      SPECIALIZE(BulletType);
      SPECIALIZE(Color);
      SPECIALIZE(DamageType);
      SPECIALIZE(Error);
      SPECIALIZE(ExplosionType);
      SPECIALIZE(GameType);
      SPECIALIZE(Order);
      SPECIALIZE(PlayerType);
      SPECIALIZE(Race);
      SPECIALIZE(TechType);
      SPECIALIZE(UnitSizeType);
      SPECIALIZE(UnitType);
      SPECIALIZE(UpgradeType);
      SPECIALIZE(WeaponType);

      SPECIALIZE(Position);
      SPECIALIZE(TilePosition);
      SPECIALIZE(WalkPosition);

#undef SPECIALIZE

      template <class K, class T> static std::wstring ToString(const std::pair<K, T>& v) { RETURN_WIDE_STRING(v); }
    }
  }
}