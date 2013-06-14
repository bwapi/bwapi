#include "Expand.h"
#include <tuple>

using namespace AISCRIPT;

IMPLEMENT(Expand);

bool Expand::execute(aithread &thread) const
{
  // Retrieve parameters
  BYTE bExpansionNumber, wBlockOffset;
  thread.readTuple( std::tie(bExpansionNumber, wBlockOffset) );
          
        // if ( GetExpansionCount() < bExpandMax )
        //   this->Expand(wExpandBlock, bExpandMax);

  // Save debug info and return
  thread.saveDebug(Text::Red, this->getOpcode(), "%u p_%04X", bExpansionNumber, wBlockOffset);
  return true;
}
