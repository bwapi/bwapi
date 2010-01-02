#include "Color.h"
#include <list>
namespace BWAPI2
{
  int palette[256] = {0x000000, 0x000000, 0x000000, 0x000000, 0x000000, 0x000000, 0x000000, 0x000000,
                      0x000000, 0x000000, 0x000000, 0x000000, 0x000000, 0x000000, 0x3a003a, 0x190019,
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
                      0x141008, 0x18140c, 0x242c0c, 0x101018, 0x141420, 0x2c2c40, 0x444c68, 0x040404,
                      0x1c1810, 0x201c14, 0x24201c, 0x30281c, 0x40382c, 0x544838, 0x685c4c, 0x907c64,
                      0x282014, 0x302814, 0x342c18, 0x382c1c, 0x3c301c, 0x443824, 0x544430, 0x0c1004,
                      0x141804, 0x181c08, 0x1c2008, 0x20240c, 0x2c3410, 0x343c10, 0x404810, 0x202030,
                      0x28283c, 0x303448, 0x141414, 0x20181c, 0x282018, 0x241c24, 0x282424, 0x302c2c,
                      0x3c2c34, 0x3c383c, 0x483c30, 0x443440, 0x503c48, 0x5c5034, 0x2323ff, 0x2323ff,
                      0x000000, 0x000000, 0x000000, 0x000000, 0x000000, 0x000000, 0x000000, 0xffffff
                     };
  namespace Colors
  {
    BWAPI2_EXPORT const Color Red(111);
    BWAPI2_EXPORT const Color Green(117);
    BWAPI2_EXPORT const Color Blue(246);
    BWAPI2_EXPORT const Color Yellow(61);
    BWAPI2_EXPORT const Color Cyan(128);
    BWAPI2_EXPORT const Color Purple(163);
    BWAPI2_EXPORT const Color Orange(179);
    BWAPI2_EXPORT const Color Black(0);
    BWAPI2_EXPORT const Color White(255);
    BWAPI2_EXPORT const Color Grey(74);

    std::list<int> cell[8][8][8];
    void init()
    {
      for(int i = 0; i < 256; i++)
      {
        int redCell = (palette[i] >> 21) & 0x07;
        int greenCell = (palette[i] >> 13) & 0x07;
        int blueCell = (palette[i] >> 5) & 0x07;
        cell[redCell][greenCell][blueCell].push_back(i);
      }
    }
  }
  int min(int a, int b)
  {
    return a < b ? a : b;
  }
  int max(int a, int b)
  {
    return a > b ? a : b;
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
    this->id = other.id;
  }
  Color::Color(int red, int green, int blue)
  {
    int redCell = red >> 5;
    int greenCell = green >> 5;
    int blueCell = blue >> 5;
    int redCellS = max(redCell - 1, 0);
    int redCellF = min(redCell + 1, 7);
    int greenCellS = max(greenCell - 1, 0);
    int greenCellF = min(greenCell + 1, 7);
    int blueCellS = max(blueCell - 1, 0);
    int blueCellF = min(blueCell + 1, 7);
    int min_dist = 3 * 256 * 256;
    int best_id = -1;
    for(int rc = redCellS; rc <= redCellF; rc++)
    {
      for(int gc = greenCellS; gc <= greenCellF; gc++)
      {
        for(int bc = blueCellS; bc <= blueCellF; bc++)
        {
          for(std::list<int>::iterator i = Colors::cell[rc][gc][bc].begin(); i != Colors::cell[rc][gc][bc].end(); i++)
          {
            int id = *i;
            int ired = (palette[id] >> 16) & 0xFF;
            int igreen = (palette[id] >> 8) & 0xFF;
            int iblue = (palette[id] >> 0) & 0xFF;
            int distance = (red - ired) * (red - ired) + (green - igreen) * (green - igreen) + (blue - iblue) * (blue - iblue);
            if (distance < min_dist)
            {
              min_dist = distance;
              best_id = id;
            }
          }
        }
      }
    }
    if (best_id == -1)
    {
      int min_dist = 3 * 256 * 256;
      int best_id = -1;
      for(int id = 0; id < 255; id++)
      {
        int ired = (palette[id] >> 16) & 0xFF;
        int igreen = (palette[id] >> 8) & 0xFF;
        int iblue = (palette[id] >> 0) & 0xFF;
        int distance = (red - ired) * (red - ired) + (green - igreen) * (green - igreen) + (blue - iblue) * (blue - iblue);
        if (distance < min_dist)
        {
          min_dist = distance;
          best_id = id;
        }
      }
    }
    this->id = best_id;
  }
  Color& Color::operator=(const Color& other)
  {
    this->id = other.id;
    return *this;
  }
  bool Color::operator==(const Color& other) const
  {
    return palette[this->id] == palette[other.id];
  }
  bool Color::operator!=(const Color& other) const
  {
    return palette[this->id] != palette[other.id];
  }
  bool Color::operator<(const Color& other) const
  {
    return this->id < other.id;
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
