#pragma once
#include "../Types.h"

namespace BWAPI4 { class UnitImpl; typedef UnitImpl* Unit; }
#pragma pack(1)
namespace BW
{
  class CUnit;
  /**
   * Representation of pointer to unit in bw structure that uses index (not
   * pointer). When pointer to another unit is present in bw structure, it uses
   * two ways to do it
   * 1) Pointer to bw memory where is the unit stored (in UnitNodeTable) This is
   *    used for example in Unit.next, this representation takes 4 bytes.
   * 2) unitIndes in the UnitNodeTable, this is used for example in Orders, the
   *    motivation probably was to save network data capacity, as only 2 bytes
   *    are needed to store the index. But for some unkown reason, value there
   *    is unit index + 1 << 11 (1 in the binary code on 11th place)
   *
   */
  class UnitTarget
  {
    public :
      UnitTarget();

      /// Constructor, takes BWAPI unit pointer, and decodes it to bw index type.
      UnitTarget(const BWAPI4::Unit target);
      UnitTarget(const BW::CUnit* target);
      u16 getTarget() const;
    private :
      u16 targetID = 0; /// Unit index pointer stored in the way bw understands it
  };

  static_assert(sizeof(UnitTarget) == 2, "UnitTarget is expected to be POD 2 bytes.");
};
#pragma pack()
