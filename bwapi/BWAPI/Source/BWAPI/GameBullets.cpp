#include "GameImpl.h"

#include <BW/Bullet.h>

#include <BWAPI/BulletImpl.h>

#include "../../Debug.h"

namespace BWAPI
{
  //----------------------------------------------- GET BULLET FROM INDEX ------------------------------------
  BulletImpl* GameImpl::getBulletFromIndex(int index)
  {
    int i = (index & 0x7F);
    return this->bulletArray[i];
  }
  //--------------------------------------------- UPDATE BULLETS ---------------------------------------------
  void GameImpl::updateBullets()
  {
    // Reset bullets information
    for(int i = 0; i < BULLET_ARRAY_MAX_LENGTH; ++i)
      this->bulletArray[i]->setExists(false);
    bullets.clear();

    // Repopulate bullets set
    for ( BW::Bullet *curritem = *BW::BWDATA_BulletNodeTable_FirstElement; 
          curritem; 
          curritem = curritem->next )
    {
      BulletImpl *b = BulletImpl::BWBulletToBWAPIBullet(curritem);
      b->setExists(true);
      this->bullets.insert(b);
    }

    // Update all bullets info
    for(int i = 0; i < BULLET_ARRAY_MAX_LENGTH; ++i)
    {
      this->bulletArray[i]->saveExists();
      this->bulletArray[i]->updateData();
    }
  }

}

