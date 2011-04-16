#include <BWAPI/Color.h>
#include <list>
#include <Util/Foreach.h>
#include <windows.h>

#include "../../Debug.h"

namespace BWAPI
{
  unsigned int palette[256] = { 0x000000, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF,
                                0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF,
                                0x2c2418, 0x482414, 0x5c2c14, 0x703014, 0x683c24, 0x7c4018, 0x784c2c, 0xa80808,
                                0x8c5430, 0x846044, 0xa0541c, 0xc44c18, 0xbc6824, 0xb4703c, 0xd06420, 0xdc9434,
                                0xe09454, 0xecc454, 0x344428, 0x406c3c, 0x486c50, 0x4c8050, 0x508c5c, 0x5ca078,
                                0x000018, 0x001034, 0x000850, 0x243448, 0x304054, 0x14347c, 0x344c6c, 0x405874,
                                0x48688c, 0x00709c, 0x5880a4, 0x4068d4, 0x18acb8, 0x2424fc, 0x6494bc, 0x70a8cc,
                                0x8cc0d8, 0x94dcf4, 0xacdce8, 0xacfcfc, 0xccf8f8, 0xfcfc00, 0xf4e490, 0xfcfcc0,
                                0x0c0c0c, 0x181410, 0x1c1c20, 0x282830, 0x383024, 0x383c44, 0x4c4030, 0x4c4c4c,
                                0x5c5040, 0x585858, 0x686868, 0x78846c, 0x68946c, 0x74a47c, 0x98948c, 0x90b894,
                                0x98c4a8, 0xb0b0b0, 0xacccb0, 0xc4c0bc, 0xcce0d0, 0xf0f0f0, 0x1c1008, 0x28180c,
                                0x341008, 0x34200c, 0x381020, 0x342820, 0x443408, 0x483018, 0x600000, 0x542820,
                                0x504014, 0x5c5414, 0x840404, 0x684c34, 0x7c3830, 0x706420, 0x7c5050, 0xa4341c,
                                0x946c00, 0x985c40, 0x8c8034, 0x987454, 0xb85444, 0xb09018, 0xb0745c, 0xf40404,
                                0xc87854, 0xfc6854, 0xe0a484, 0xfc9468, 0xfccc2c, 0x10fc18, 0x0c0020, 0x1c1c2c,
                                0x24244c, 0x282c68, 0x2c3084, 0x2018b8, 0x343cac, 0x686894, 0x6490fc, 0x7cacfc,
                                0x00e4fc, 0x9c9040, 0xa89454, 0xbca45c, 0xccb860, 0xe8d880, 0xecc4b0, 0xfcfc38,
                                0xfcfc7c, 0xfcfca4, 0x080808, 0x101010, 0x181818, 0x282828, 0x343434, 0x4c3c38,
                                0x444444, 0x484858, 0x585868, 0x746838, 0x78645c, 0x60607c, 0x847474, 0x84849c,
                                0xac8c7c, 0xac9894, 0x9090b8, 0xb8b8e8, 0xf88c14, 0x10543c, 0x209070, 0x2cb494,
                                0x042064, 0x481c50, 0x083498, 0x683078, 0x88409c, 0x0c48cc, 0xbcb834, 0xdcdc3c,
                                0x100000, 0x240000, 0x340000, 0x480000, 0x601804, 0x8c2808, 0xc81818, 0xe02c2c,
                                0xe82020, 0xe85014, 0xfc2020, 0xe87824, 0xf8ac3c, 0x001400, 0x002800, 0x004400,
                                0x006400, 0x088008, 0x249824, 0x3c9c3c, 0x58b058, 0x68b868, 0x80c480, 0x94d494,
                                0x0c1424, 0x243c64, 0x305084, 0x385c94, 0x4874b4, 0x5484c4, 0x6094d4, 0x78b4ec,
                                0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF,
                                0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF,
                                0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF,
                                0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF,
                                0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF,
                                0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF,
                                0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0xffffff
                               };
  namespace Colors
  {
    const Color Red(111);
    const Color Blue(165);
    const Color Teal(159);
    const Color Purple(164);
    const Color Orange(179);
    const Color Brown(19);
    const Color White(255);
    const Color Yellow(135);
    const Color Green(117);
    const Color Cyan(128);
    
    const Color Black(0);
    const Color Grey(74);

    BYTE closest[64][64][64];
    unsigned int getBestIdFor(unsigned int red, unsigned int green, unsigned int blue)
    {
      unsigned int min_dist   = 3 * 256 * 256;
      unsigned int best_id    = MAXUINT;
      for( unsigned int id = 0; id < 255; ++id )
      {
        RGBQUAD p = *(RGBQUAD*)&palette[id];
        if ( p.rgbReserved )
          continue;
        int r = (int)red   - p.rgbRed;
        int g = (int)green - p.rgbGreen;
        int b = (int)blue  - p.rgbBlue;

        unsigned int distance = r*r + g*g + b*b;
        if ( distance < min_dist )
        {
          min_dist = distance;
          best_id = id;
          if ( !distance )
            break;
        }
      }
      return best_id;
    }

    void init()
    {
      for ( unsigned int r = 0; r < 64; ++r )
        for ( unsigned int g = 0; g < 64; ++g )
          for ( unsigned int b = 0; b < 64; ++b )
            closest[r][g][b] = (BYTE)getBestIdFor(r << 2, g << 2, b << 2);
    } // init
  }
  Color::Color()
  {
    this->id = 0;
  }
  Color::Color(int id)
  {
    this->id = id;
  }
  Color::Color(const Color& other)
  {
    this->id = other;
  }
  Color::Color(int red, int green, int blue)
  {
    this->id = Colors::closest[(BYTE)red >> 2][(BYTE)green >> 2][(BYTE)blue >> 2];
  }
  Color& Color::operator=(const Color& other)
  {
    this->id = other;
    return *this;
  }
  Color::operator int() const
  {
    return id;
  }
  int Color::getID() const
  {
    return this->id;
  }
  int Color::red() const
  {
    return (palette[this->id] >> 16) & 0xFF;
  }
  int Color::green() const
  {
    return (palette[this->id] >> 8) & 0xFF;
  }
  int Color::blue() const
  {
    return palette[this->id] & 0xFF;
  }
}
