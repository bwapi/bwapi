#include "CSprite.h"
#include "CImage.h"

namespace BW
{
  void CSprite::draw()
  {
    //u8 color = 0;
    //u8 color = getColourID(this->playerID);
    
    //imgDrawPalettteIndex[2] = tunit_image_palette[2*color];
    //imgDrawPalettteIndex[3] = tunit_image_palette[2*color + 1];
    //gdrawColorID = color;
    
    //if ( this->mainGraphic )
      // unknownColorShiftSomething(this->mainGraphic->paletteType, this->playerID);

    for ( auto i = this->pImageTail; i; i = i->prev )
      i->drawImage();
  }






}
