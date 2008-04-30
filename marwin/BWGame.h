class BWHandle;
class Player;
class Unit;
#define BW_MAX_UNIT_COUNT 1700
class BWGame
 {
  public :
   BWGame(BWHandle *handle);
   BWGame(void);
   ~BWGame(void);
   void update(void);
   int getPlayerCount(void);
   Player *getPlayer(int index);
   Unit **globalUnits;
  private :
   BWHandle *bwHandle;
   void init(void);
   bool handleOwned;

   Player **players;
   int playerCount;

 };
