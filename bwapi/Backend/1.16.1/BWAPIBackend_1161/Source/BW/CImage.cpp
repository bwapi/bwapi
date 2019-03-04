#include "CImage.h"

#include <BW/Dialog.h>
#include <BW/CSprite.h>
#include <BW/Offsets.h>

namespace BW
{
  /////////////////////////////////////// IMAGE functions
  grpFrame *CImage::getCurrentFrame() const
  {
    return &this->GRPFile->frames[this->frameIndex];
  }
  BW::Position CImage::getPosition() const
  {
    return Position(this->horizontalOffset, this->verticalOffset) + this->spriteOwner->position;
  }
  void CImage::updateGraphicData()
  {
    grpHead  *pGrpHead  = this->GRPFile;
    grpFrame *pGrpFrame = this->getCurrentFrame();
    
    if ( !(this->flags & 2) ) // Sets the left boundary of the image
      this->mapPosition.x = this->getPosition().x - pGrpHead->width/2 + pGrpFrame->x;
    else // Not sure
      this->mapPosition.x = this->getPosition().x + pGrpHead->width/2 - (pGrpFrame->wid + pGrpFrame->x);

    if ( !(this->flags & 4) ) // Sets the top boundary of the image
      this->mapPosition.y = this->getPosition().y - pGrpHead->height/2 + pGrpFrame->y;

    rect bounds = { 0, 0, pGrpFrame->wid, pGrpFrame->hgt };
    BW::Position screen(this->mapPosition);
    screen -= Position(static_cast<short>(BW::BWDATA::MoveToX), static_cast<short>(BW::BWDATA::MoveToY));
    
    if ( screen.x < 0 )
    {
      bounds.right  += screen.x;  // screenX is negative
      bounds.left   += -screen.x;  // -screenX is positive
      screen.x  = 0;
    }
    if ( screen.y < 0 )
    {
      bounds.bottom += screen.y;   // screenY is negative
      bounds.top    += -screen.y;  // -screenY is positive
      screen.y  = 0;
    }
    bounds.right  = static_cast<s16>(std::min<int>(bounds.right,  BW::BWDATA::GameScreenBuffer.width() - screen.x));
    bounds.bottom = static_cast<s16>(std::min<int>(bounds.bottom, BW::BWDATA::GameScreenBuffer.height() - screen.y));

    this->screenPosition = screen;
    this->grpBounds = bounds;
  }

  void CImage::drawImage()
  {
    if ( !(this->flags & 0x40)      && 
         this->grpBounds.bottom > 0 &&
         this->grpBounds.right  > 0 )
    {
      if ( (this->flags & 1) /*|| this->needsRefresh()*/ )
      {
        rect rctDraw = { this->grpBounds.left, this->grpBounds.top, this->grpBounds.right, this->grpBounds.bottom };
        this->renderFunction(this->screenPosition.x, this->screenPosition.y, this->getCurrentFrame(), &rctDraw, (int)this->coloringData);
      }
    }
    this->flags &= ~1;  // Clear Redraw bit
  }
}
