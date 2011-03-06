#include "Image.h"

namespace BW
{
  void Image::updateGraphicData()
  {
    grpHead  *pGrpHead  = this->GRPFile;
    grpFrame *pGrpFrame = &pGrpHead->frames[this->frameIndex];
    
    int mapX = this->horizontalOffset + this->spriteOwner->position.x;
    if ( this->flags & 2 )
      mapX += (pGrpHead->width >> 1) - pGrpFrame->right - pGrpFrame->left;
    else
      mapX += pGrpFrame->left - (pGrpHead->width >> 1);
    this->mapPosition.x = (u16)mapX;

    if ( !(this->flags & 4) )
      this->mapPosition.y = this->spriteOwner->position.y + this->verticalOffset + pGrpFrame->top - (pGrpHead->height >> 1);
    int grpRight  = pGrpFrame->right;
    int grpBottom = pGrpFrame->bottom;
    int screenX   = this->mapPosition.x - (*BW::BWDATA_MoveToX);
    int screenY   = this->mapPosition.y - (*BW::BWDATA_MoveToY);

    int grpLeft = 0;
    if ( screenX < 0 )
    {
      grpRight += screenX;
      grpLeft  = -screenX;
      screenX  = 0;
    }
    this->screenPosition.x = (s16)screenX;
    this->grpBounds.left   = (s16)grpLeft;
    if ( grpRight >= BW::BWDATA_GameScreenBuffer->wid - screenX )
      grpRight = BW::BWDATA_GameScreenBuffer->wid - screenX;
    int _bot = grpBottom;
    
    this->grpBounds.right = (s16)grpRight;
    int grpTop = 0;
    if ( screenY < 0 )
    {
      _bot    = screenY + grpBottom;
      grpTop  = -screenY;
      screenY = 0;
    }
    this->grpBounds.top    = (s16)grpTop;
    this->screenPosition.y = (s16)screenY;
    if ( _bot < BW::BWDATA_GameScreenBuffer->ht - screenY )
      this->grpBounds.bottom = (s16)_bot;
    else
      this->grpBounds.bottom = (s16)(BW::BWDATA_GameScreenBuffer->ht - screenY);
  }

  void Image::drawImage()
  {
    if ( (this->flags & 0x40)       && 
         this->grpBounds.bottom > 0 &&
         this->grpBounds.right  > 0 )
    {
      if ( (this->flags & 1) /*|| this->needsRefresh()*/ )
      {
        RECT rctDraw = { this->grpBounds.left, this->grpBounds.top, this->grpBounds.right, this->grpBounds.bottom };
        this->renderFunction1(this->screenPosition.x, this->screenPosition.y, &this->GRPFile->frames[this->frameIndex], &rctDraw, (int)this->coloringData);
      }
    }
    this->flags &= 0xFFFE;
  }
}
