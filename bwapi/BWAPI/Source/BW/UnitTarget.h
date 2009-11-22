#pragma once
#include <Util/Types.h>
namespace BWAPI { class UnitImpl; }

#pragma pack(1)
namespace BW
{

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
      /** Default contructor, sets x and y to 0. */
      UnitTarget();
      /** Constructor, takes BWAPI unit pointer, and decodes it to bw index type.*/
      UnitTarget(BWAPI::UnitImpl* target);
      u16 getTarget() const;
    private :
      u16 targetID; /**< Unit index pointer stored in the way bw understands it */
  };
};
#pragma pack()
