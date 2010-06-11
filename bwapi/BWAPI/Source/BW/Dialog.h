#pragma once
#include <Util/Types.h>
#include <windows.h>

#include "offsets.h"

#define CTRL_DISABLED       0x00000002
#define CTRL_ACTIVE         0x00000004
#define CTRL_VISIBLE        0x00000008
#define CTRL_EVENTS         0x00000010

#define CTRL_BTN_CANCEL     0x00000040
#define CTRL_BTN_NO_SOUND   0x00000080
#define CTRL_BTN_VIRT_HOTKEY 0x00000100
#define CTRL_BTN_HOTKEY     0x00000200
#define CTRL_FONT_SMALLEST  0x00000400
#define CTRL_FONT_LARGE     0x00000800

#define CTRL_TRANSPARENT    0x00002000
#define CTRL_FONT_LARGEST   0x00004000

#define CTRL_FONT_SMALL     0x00010000
#define CTRL_PLAIN          0x00020000
#define CTRL_TRANSLUCENT    0x00040000
#define CTRL_BTN_DEFAULT    0x00080000
#define CTRL_TOP            0x00100000
#define CTRL_HALIGN_LEFT    0x00200000
#define CTRL_HALIGN_RIGHT   0x00400000
#define CTRL_HALIGN_CENTER  0x00800000
#define CTRL_VALIGN_TOP     0x01000000
#define CTRL_VALIGN_MIDDLE  0x02000000
#define CTRL_VALIGN_BOTTOM  0x04000000

#define CTRL_DLG_NOREDRAW   0x10000000
#define CTRL_ALTERNATE      0x20000000
#define CTRL_DLG_ACTIVE     0x40000000
#define CTRL_LBOX_NORECALC  0x80000000

namespace BW
{
  namespace ctrls
  {
    enum Enum
    {
      cDLG                = 0,
      cDFLTBTN            = 1,
      cBUTTON             = 2,
      cOPTION             = 3,
      cCHKBOX             = 4,
      cIMAGE              = 5,
      cHSCROLL            = 6,
      cVSCROLL            = 7,
      cEDIT               = 8,
      cLSTATIC            = 9,
      cRSTATIC            = 10,
      cCSTATIC            = 11,
      cLIST               = 12,
      cCOMBO              = 13,
      cFLCBTN             = 14,
      max                 = cFLCBTN
    };
  };

  struct pt
  {
    WORD  x;
    WORD  y;
  };

  struct dlgEvent
  {
    DWORD dwUser;
    DWORD dwUnk_0x04;
    WORD  wVirtKey;
    WORD  wUnk_0x0A;
    WORD  wNo;
    pt    cursor;
    WORD  wUnk_0x12;
  };

  struct dlgFullEvent
  {
    DWORD dwSize;
    dlgEvent evt;
  };

  struct bitmap
  {
    WORD  wid;
    WORD  ht;
    BYTE  *data;
  };
  
  struct rect
  {
    short Xmin;
    short Ymin;
    short Xmax;
    short Ymax;
  };

#pragma pack(1)
  class dialog   // BIN Dialog
  {
  public:
    dialog  *pNext;         // 0x00
    rect    rct;            // 0x04   // official name
    bitmap  srcBits;        // 0x0C   // official
    char    *pszText;       // 0x14   // official name
    LONG    lFlags;         // 0x18   // official name
    DWORD   dwUnk_0x1C;
    short   wIndex;         // 0x20   // official name
    WORD    wCtrlType;      // 0x22   // official name
    WORD    wUnk_0x24;
    LONG    lUser;          // 0x26   // official name
    bool (__fastcall *pfcnInteract)(dialog*,dlgEvent*);  // 0x2A
    void (__fastcall *pfcnUpdate)(dialog*,int,int,rect*);    // 0x2E
    union _u                 // 0x32   // official
    {
      struct _ctrl           // official
      {
        dialog  *pDlg;      // 0x32   // official
      } ctrl;
      
      struct _dlg            // official
      {
        DWORD   dwUnk_0x32;
        bitmap  dstBits;      // 0x36  // official 
        dialog  *pUnk_0x3E;
        dialog  *pFirstChild;  // 0x42  // official
        DWORD   dwUnk_0x46;
        void    *pModalFcn;   // 0x4A   // official
      } dlg;
      
      struct _btn
      {
        dialog  *pDlg;          // 0x32   // official
        rect    responseRct;    // 0x36
        WORD    wHighlight;     // 0x3E
        WORD    wUnknown_0x40;
        void    *pSmk;          // 0x42
        rect    textRct;        // 0x46
        WORD    wAlignment;    // 0x4E
      } btn;

      struct _optn
      {
        dialog  *pDlg;          // 0x32   // official
        BYTE    bEnabled;       // 0x36
      } optn;
      
      struct _edit
      {
        dialog  *pDlg;      // 0x32   // official
        DWORD dwColor;      // 0x36
        DWORD dwUnk_0x3A;
        DWORD dwCurPos;     // 0x3E
      } edit;
      
      struct _scroll   // official
      {
        dialog  *pDlg;      // 0x32   // official
        void  *pfcnUnknown_0x36;
        WORD  nCurPos;    // 0x3A
        WORD  nMin;       // 0x3C   // official
        WORD  nMax;       // 0x3E   // official
        WORD  wUnk_0x40;
        DWORD dwUnk_0x42;
        BYTE  bSliderSkip;    // 0x46
        BYTE  bUnk_0x47;
        BYTE  bSliderGraphic;   // 0x48
        BYTE  bSliderSpacing;     // 0x49
        WORD  wUnk_0x4A;
        DWORD dwUnk_0x4C;
      } scroll;
      
      struct _list   // official
      {
        dialog  *pDlg;      // 0x32   // official
        dialog  *pScrlBar;    // 0x36
        char    **ppStrs;     // 0x3A   // official
        BYTE    *pbStrFlags;  // 0x3E   // official
          /*
            &0x0F specifies the colour of the entry (Standard colours unless noted)
              0x01  = Use previous (Uses the colour of the entry BEFORE the previous one)
              0x09 - 0x0D  = Use last (Uses the colour of the previous entry)
            &0xF0 specifies additional properties
              0x10  = Gives entry an option button
          */
        DWORD   *pdwData;     // 0x42   // official
        BYTE    bStrs;        // 0x46   // official
        BYTE    unknown_0x47;
        BYTE    bCurrStr;   // 0x48   // official
        BYTE    bSpacing;   // 0x49
        BYTE    bItemsPerPage;
        BYTE    bUnknown_0x4B;
        BYTE    bDirection;       // 0x4C
        BYTE    bOpenFlags;       // 0x4D
        BYTE    bSelectedIndex;   // 0x4E
        BYTE    bUnknown_0x4F;
        WORD    wVerticalOffset;    // 0x4A
        void    *pDrawItemFcn;      // 0x4C
      } list;
    } u;

    // global functions
    dialog(WORD ctrlType, short index, WORD top, WORD left, WORD width, WORD height);
    dialog(WORD ctrlType, short index, const char *text, WORD top, WORD left, WORD width, WORD height, bool (__fastcall *pfInteract)(dialog*,dlgEvent*) = NULL, void (__fastcall *pfUpdate)(dialog*,int,int,rect*) = NULL);
    ~dialog();

    dialog  *FindIndex(short wIndex);
    dialog  *findDialogByName(const char *pszName);
    void    add(dialog *dlg);
    dialog  *next();

    void    setFlag(DWORD dwFlag);
    void    clearFlag(DWORD dwFlag);

    // dialog-specific functions
    bool    isDialog();
    dialog  *child();

    // control-specific functions
    dialog *parent();
    short getIndex();

    // checkbox & option button
    bool isOption();
    bool isChecked();

    // listbox & combobox
    bool  isList();
    BYTE  getSelectedIndex();
    DWORD getSelectedValue();
    char  *getSelectedString();

    bool  setSelectedIndex(BYTE bIndex);
    bool  setSelectedByValue(DWORD dwValue);
    bool  setSelectedByString(char *pszString);

  };
#pragma pack()
  dialog *CreateDialogWindow(const char *text, WORD top, WORD left, WORD width, WORD height);
};

/*
List entry: Format for lists (map list for example)
            pointed to by list.pdwData

  char  szListEntry[65];
  char  szName[32];
  char  szDescription[316];
  char  szHumanSlots[35];
  char  szComputerSlots[35];
  char  szPlayers[35];
  char  szDimensions[35];
  char  szTileset[35];
  DWORD dwUnk1;
  DWORD dwUnk2;
  DWORD dwUnk3;
  DWORD dwUnk4;
  DWORD dwUnk5;
  char  szFullPath[261];
  char  szFileName[261];
  WORD  wWidth;
  WORD  wHeight;
  WORD  wTileset; // or numPlayers? guess
  BYTE  bReserved; // players reserved? guess
  BYTE  bPlayers;
  BYTE  bComputers; // for UMS
  BYTE  bHumans; // guess
*/