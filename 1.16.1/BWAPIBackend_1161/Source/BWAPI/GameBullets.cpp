#include "GameImpl.h"

#include "../BW/CBullet.h"
#include <cassert>

#include "BulletImpl.h"

namespace BWAPI4
{
  //----------------------------------------------- GET BULLET FROM INDEX ------------------------------------
  BulletImpl* GameImpl::getBulletFromIndex(int index)
  {
    index &= 0x7F;
    if (static_cast<unsigned>(index) < bulletArray.size())
      return this->bulletArray[index];
    return nullptr;
  }
  //--------------------------------------------- UPDATE BULLETS ---------------------------------------------
  void GameImpl::updateBullets()
  {
    // Reset bullets information
    for (BulletImpl* b : bulletArray)
      b->setExists(false);
    bullets.clear();

    // Repopulate bullets set
    for ( BW::CBullet *curritem = BW::BWDATA::BulletNodeTable_FirstElement; 
          curritem; 
          curritem = curritem->next )
    {
      BulletImpl *b = BulletImpl::BWBulletToBWAPIBullet(curritem);
      b->setExists(true);
      this->bullets.insert(b);
    }

    // Update all bullets info
    for (BulletImpl* b : bulletArray)
    {
      b->saveExists();
      b->updateData();
    }
  }

}

