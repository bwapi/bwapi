#include "Unusedline.h"
namespace Util
{
  //------------------------------------ CONSTRUCTOR -----------------------------
  UnusedLine::UnusedLine(const std::string &line)
   {
    this->line = line;
   }
  //-------------------------------------- GET LINE ------------------------------
  std::string UnusedLine::getLine(void)
   {
    return line;
   }
  //------------------------------------------------------------------------------
}
