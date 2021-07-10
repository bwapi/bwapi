#include "helpers.h"

#include <cstring>

using namespace Colors;

void writeColorInfo()
{
  std::ofstream of("colortypes.dox");
  std::vector<Color> colors = { Red, Blue, Teal, Purple, Orange, Brown, White, Yellow, Green, Cyan, Black, Grey };
  for (auto t : colors)
  {
    of << docBegin(t);
    
    char colorStr[32];
    sprintf(colorStr, "%02X%02X%02X", t.red(), t.green(), t.blue());

    of << "@htmlonly\n";
    of << "<div style='width:64px;height:64px;background-color:#" << colorStr << ";'></div>\n";
    of << "@endhtmlonly\n";

    if (!t.getName().empty())
      of << makekeywords(t);

    of << docEnd();
  }
}
