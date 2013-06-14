#include "Region_Size.h"
#include <tuple>

using namespace AISCRIPT;

IMPLEMENT(Region_Size);

bool Region_Size::execute(aithread &thread) const
{
  // Retrieve parameters
  BYTE bSize;
  WORD wOffset;
  thread.readTuple( std::tie(bSize, wOffset) );

  // @TODO: Region::getTileCount
  // if ( this->pTown->RegionSize() < bSize )
  //   this->dwScriptOffset = wBlock;

  // Save debug info and return
  thread.saveDebug(Text::Red, this->getOpcode(), "%u p_%04X", bSize, wOffset);
  return true;
}
