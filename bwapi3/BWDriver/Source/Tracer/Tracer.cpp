#include "Tracer.h"
#include "Drawing.h"

#include <Util\Strings.h>

namespace BWAPI
{
  namespace Tracer
  {
//private:
    // state
    bool isActive = false;

    //----------------------------------- ----------------------------------------------
//public:
    //----------------------------------- DEASTIVATE -----------------------------------
    void deactivate()
    {
      isActive = false;
    }
    //----------------------------------- COMMAND --------------------------------------
    bool command(const std::vector<std::string>& parsed)
    {
      if(parsed[1] == "on")
      {
        isActive = true;
        return true;
      }
      if(parsed[1] == "off")
      {
        isActive = false;
        return true;
      }
      return false;
    }
    //----------------------------------- ON FRAME -------------------------------------
    void onMatchFrame()
    {
      if(!isActive)
        return;
      removeShapes();

      // tracer code

      pushText(Position(10, 10), "Tracer on");

    }
    //----------------------------------- ----------------------------------------------
    void onDraw()
    {
      if(isActive)
        drawShapes();
    }
    //----------------------------------- ----------------------------------------------
  }
}
