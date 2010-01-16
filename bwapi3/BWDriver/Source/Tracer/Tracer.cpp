#include "Tracer.h"
#include "Drawing.h"

#include <Util\Strings.h>

#include <set>
#include <windows.h>

#include <BWAPITypes\StaticGameData.h>

namespace BWAPI
{
  namespace Tracer
  {
//private:
    class ShoutBox
    {
    private:
      Position pos;
    public:
      ShoutBox(Position startpos)
      {
        pos = startpos;
      }
      Position getNext()
      {
        Position retval = pos;
        pos += Position(0, 8);
        return retval;
      }
    };

    class Timer
    {
    private:
      int startTime;
    public:
      int delay;
      Timer()
        : delay(0)
        , startTime(0)
      {
      }
      void wait()
      {
        int waitTime = getTimeLeft();
        if(waitTime > 0)
          Sleep(waitTime);
      }
      void restart()
      {
        startTime = GetTickCount();
      }
      int getElapsedTime()
      {
        return GetTickCount() - startTime;
      }
      int getTimeLeft()
      {
        return (startTime+delay) - GetTickCount();
      }
    };

    int redrawCount;
    int frames;
    bool alternating;
    // state
    bool isActive = false;
    Timer frameTimer; // measures time between frames
    Timer delayTimer; // measures time between the first draw cycle and the next frame

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
      frames++;
      alternating = !alternating;

      // tracer code
      ShoutBox sh(Position(10, 10));

      pushTextScreen(sh.getNext(), "Tracer on");
      pushTextScreen(sh.getNext(), "sizeof(StaticGameData): %d", sizeof(StaticGameData));
      pushTextScreen(sh.getNext(), "redraws: %d", redrawCount);
      pushTextScreen(sh.getNext(), "frames: %d", frames);
      pushTextScreen(sh.getNext(), "frameTime: %d", frameTimer.getElapsedTime());
      pushTextScreen(sh.getNext(), "delayReal: %d", delayTimer.getElapsedTime());
      pushTextScreen(sh.getNext(), "delayTimer: %d", frameTimer.delay);
      redrawCount = 0;

      int excessTime = delayTimer.getElapsedTime();
      if(excessTime > 5) // adaptivity
      {
        frameTimer.delay ++;
      }
      else
        frameTimer.delay -= 2;
      frameTimer.restart();
      frameTimer.wait();
    }
    //----------------------------------- ----------------------------------------------
    void onDraw()
    {
      redrawCount++;
      if(redrawCount == 1)
      {
        delayTimer.restart();
      }
      if(isActive)
      {
        drawShapes();
      }
    }
    //----------------------------------- ----------------------------------------------
  }
}
