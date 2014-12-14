#include "helpers.h"

#include <cstring>

using namespace Colors;

void writeColorInfo()
{
  std::ofstream of("colortypes.dox");
  std::vector<Color> colors = { Red, Blue, Teal, Purple, Orange, Brown, White, Yellow, Green, Cyan, Black, Grey };
  for (auto t : colors)
  {
    of << "/*!\n";
    of << "@var BWAPI::Colors::" << t.getName() << "\n";

    char colorStr[32];
    sprintf(colorStr, "%02X%02X%02X", t.red(), t.green(), t.blue());

    of << "@htmlonly\n";
    of << "<div style='width:64px;height:64px;background-color:#" << colorStr << ";'></div>\n";
    of << "@endhtmlonly\n";
    of << "*/\n";
  }
}
