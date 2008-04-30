class Unit;
class BWHandle;
class Player
 {
  public :
    Player(BWHandle *bwHandle, int playerIndex);
    ~Player(void);
    void update(BWHandle *handle);
  public :
    int playerIndex;
    unsigned long minerals, gas;
    Unit *units;
    void addUnit(Unit* unit);
 };