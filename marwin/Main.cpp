#include <stdio.h>
#include <windows.h>
#include <conio.h>

#include "BWGame.h"
#include "Exception.h"
#include "Player.h"
#include "Unit.h"
#include "Globals.h"
#include "BWHandle.h"
#define MY_ID 0
#define COMP_ID 1

int main(int argc, char *argv[])
 {
  try
   {
    BWGame *game = new BWGame();
    printf("Library loaded.");
    Unit *lastProbe = NULL;
    while(!kbhit())
     {
//     system("cls");

     // game->update();
      int index = 0;
//      for (int j = 0; j < game->getPlayerCount(); j++)
     int value = 0;
     globalBWHandle->readMemory((LPVOID)0x4BFF80,&value,4);
     if (value != -2081649835)
       printf("value = %d\n",value);
      for (Unit *unit = game->getPlayer(0)->units; unit!= NULL; unit = unit->next)
        {
       if (false/*unit->unitID == Unit::Protoss_Probe
           unit->mainOrderID == Unit::Idle*/)
          {
           if (lastProbe != NULL)
            {
             unit->setOrderTarget(lastProbe);
             unit->setOrderID(Unit::Attacking);
            }
           printf("----------- %s ----------\n", unit->unitName().c_str());
           printf("(%d,%d)->(%d,%d)\n", unit->x, unit->y, unit->getMoveX(), unit->getMoveY());
           printf("State = %s\n", unit->getStateDescription().c_str());
           printf("State2 = %d\n", unit->mainOrderState);
           printf("temp = %d\n", unit->temp);
//           printf("temp = %d\n", unit->temp);
//           printf("resource contained = %d\n", unit->resourceContained);
           //  UNIT_NODE_TABLE_ADDRESS
           if (unit->getOrderTarget() != NULL)
            {
             printf("OrderTarget : %s(%d,%d) - distance %d \n",unit->getOrderTarget()->unitName().c_str(),
                                                               unit->getOrderTarget()->x,unit->getOrderTarget()->y,
                                                   unit->getDistance(unit->getOrderTarget()));
            }

           if (unit->getTarget() != NULL)
            {
             printf("Target : %s - distance %d \n",unit->getTarget()->unitName().c_str(),
                                                   unit->getDistance(unit->getTarget()));
            }
        //   unit->setGroundWeaponCooldown(0);

       //   globalBWHandle->writeMemory((LPVOID)(0x00628500 + index*4), &unit->address,4);
        //  index++;
           lastProbe = unit;
          }
        }
      Sleep(10);
     }
    delete game;
   }
  catch (Exception &e)
   {
    printf("%s\n", e.getMessage().c_str());
   }
 }
