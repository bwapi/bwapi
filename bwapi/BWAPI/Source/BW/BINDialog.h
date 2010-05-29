#pragma once
#include <Util/Types.h>

namespace BW
{
#pragma pack(1)
  struct Bitmap
  {
    u16   width;
    u16   height;
    void  *data;
  };
  
  struct Rect
  {
    short Xmin;
    short Ymin;
    short Xmax;
    short Ymax;
  };

  struct BINDialog
  {
    BINDialog *pNext;         // 0x00
    Rect      rct;            // 0x04   // official name
    Bitmap    srcBits;        // 0x0C   // official
    char      *pszText;       // 0x14   // official name
    LONG      lFlags;         // 0x18   // official name
    u32       unknown1;       // 0x1C
    WORD      wIndex;         // 0x20   // official name
    WORD      wCtrlType;      // 0x22   // official name
    u16       unknown2;       // 0x24
    LONG      lUser;          // 0x26   // official name
    void      *pfcnInteract;  // 0x2A
    void      *pfcnUpdate;    // 0x2E
    u8        crazy_struct[27];
    /*
    union u                   // 0x32   // official
    {
      struct ctrl           // official (size=4)
      {
        BINDialog  *pDlg;      // 0x32   // official
      };
      
      struct dlg            // official (size=16)
      {
        Bitmap  dstBits;      // 0x36  // official 
        
        BINDialog  *pFirstChild;           // official
        void       *pModalFcn;   // 0x4A   // official
      };
      
      struct scroll   // official (size=4)
      {
        WORD  nMin;       // 0x3C   // official
        WORD  nMax;       // 0x3E   // official
      };
      
      struct list   // official (size=27)
      {
        char  **ppStrs;     // 0x3A   // official
        BYTE  *pbStrFlags;  // 0x3E   // official
        DWORD *pdwData;     // 0x42   // official
        BYTE  bStrs;        // 0x46   // official
        BYTE  unknown_0x47;
        BYTE  bCurrStr;   // 0x48   // official
        BYTE  bSpacing;   // 0x49
        BYTE  bUnknown_0x4A;
        BYTE  bUnknown_0x4B;
        BYTE  bDirection;       // 0x4C
        BYTE  bOpenFlags;       // 0x4D
        BYTE  bSelectedIndex;   // 0x4E
        BYTE  bUnknown_0x4F;
        WORD  wVerticalOffset;
        void  *pDrawItemFcn;
      };
    } asdf; //0x32
    */
    u8 _unknown3[2575]; //0x4D
    struct BINPlayer //0xA5C
    {
      u8 raceField1; //0xA5C
      u8 _unknown4[5]; //0xA5D
      u8 raceField2; //0xA62
      u8 _unknown5[79]; //0xA63
    } players[8];
    //0xD0C (=0xA5C+86*8)
  };
#pragma pack()
};