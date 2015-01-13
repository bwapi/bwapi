#include "CSprite.h"
#include "CImage.h"

namespace BW
{
  void CSprite::draw()
  {
    u8 color = this->playerID;
    //u8 color = getColourID(this->playerID);
    
    BW::BWDATA::ImageDrawPalette[2] = BW::BWDATA::PlayerColourPalette[2 * color];
    BW::BWDATA::ImageDrawPalette[3] = BW::BWDATA::PlayerColourPalette[2 * color + 1];
    //gdrawColorID = color;
    
    //if ( this->pImagePrimary )
      // unknownColorShiftSomething(this->pImagePrimary->paletteType, this->playerID);

    for (auto i = this->pImageTail; i; i = i->prev)
      i->drawImage();
  }






}
