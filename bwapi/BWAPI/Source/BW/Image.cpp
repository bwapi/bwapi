#include "Image.h"
#include "Offsets.h"

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

    this->graphicLeft = 0;
    if ( screenX < 0 )
    {
      grpRight += screenX;
      this->graphicLeft = (s16)(-screenX);
      screenX = 0;
    }
    this->screenPosition.x = (s16)screenX;
    if ( grpRight >= BW::BWDATA_GameScreenBuffer->wid - screenX )
      grpRight = BW::BWDATA_GameScreenBuffer->wid - screenX;
    int _bot = grpBottom;
    
    this->graphicRight = (s16)grpRight;
    int top = 0;
    if ( screenY < 0 )
    {
      _bot += screenY;
      top = -screenY;
      screenY = 0;
    }
    this->graphicTop       = (s16)top;
    this->screenPosition.y = (s16)screenY;
    if ( _bot < BW::BWDATA_GameScreenBuffer->ht - screenY )
      this->graphicBottom = (s16)_bot;
    else
      this->graphicBottom = (s16)(BW::BWDATA_GameScreenBuffer->ht - screenY);
  }

  void Image::drawImage()
  {
    if ( (this->flags & 0x40)    && 
         this->graphicBottom > 0 &&
         this->graphicRight  > 0 )
    {
      if ( (this->flags & 1) /*|| this->needsRefresh()*/ )
      {
        RECT rctDraw = { this->graphicLeft, this->graphicTop, this->graphicRight, this->graphicBottom };
        this->renderFunction1(this->screenPosition.x, this->screenPosition.y, &this->GRPFile->frames[this->frameIndex], &rctDraw, (int)this->coloringData);
      }
    }
    this->flags &= 0xFFFE;
  }
}
