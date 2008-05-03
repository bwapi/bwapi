#pragma once
class Player;
class Unit;
struct BW_UnitArray;
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
  private :
    BW_UnitArray *unitArrayCopy;
    Unit* units[1700];
};