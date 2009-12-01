#include "SharedMemoryExport.h"

namespace Util
{
  //----------------------- CONSTRUCTION -------------------------------
  SharedMemoryExport::SharedMemoryExport()
    : targetProcessMappingObjectHandle(INVALID_HANDLE_VALUE)
  {
  }
  //----------------------- IS VALID -----------------------------------
  bool SharedMemoryExport::isValid() const
  {
    return this->targetProcessMappingObjectHandle != INVALID_HANDLE_VALUE;
  }
  //----------------------- --------------------------------------------
}
