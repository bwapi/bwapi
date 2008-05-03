#pragma once

namespace BW { struct UnitArray; };
namespace BWAPI { class Player; };
namespace BWAPI { class Unit; };

namespace BWAPI
{
  /** 
  * The main class wrapping the whole game data/methods.
  */
  class Game
  {
  public :
    Game();
    ~Game();
    void update();
    Player* players[12];
    void printXY(int x, int y, char* text) const;
  private :
    BW::UnitArray* unitArrayCopy;
    Unit* units[1700];
  };
};
