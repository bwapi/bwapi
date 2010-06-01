#pragma once
#include <Util/Types.h>
#include <windows.h>

#include "offsets.h"

#define CTRL_VISIBLE        0x00000008
#define CTRL_DLG_NOREDRAW   0x10000000
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
    WORD  wUnk_0x0E;
    WORD  wUnk_0x10;
    pt    cursor;
    DWORD dwUnk_0x16;
  };

  struct bitmap
  {
    WORD  wid;
    WORD  ht;
    void  *data;
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
  private:
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
    void    *pfcnInteract;  // 0x2A
    void    *pfcnUpdate;    // 0x2E
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
        DWORD   dwUnk_0x3E;
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
            0x00  = uses simple values for pdwData
            0x03  = for maps; makes pdwData contain map name, path, other info
            0x10  = Adds an option button to the entry
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

  public:
    // global functions
    dialog(WORD ctrlType, short index, WORD top, WORD left, WORD width, WORD height);
    dialog(WORD ctrlType, short index, const char *text, WORD top, WORD left, WORD width, WORD height, bool (__fastcall *pfInteract)(dialog*,dlgEvent*) = NULL, void (__fastcall *pfUpdate)(dialog*,int,int,rect*) = NULL);
    ~dialog();

    dialog  *FindIndex(short wIndex);
    dialog  *findDialogByName(const char *pszName);
    void    add(dialog *dlg);
    dialog *next();

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
    void  setSelectedIndex(BYTE bIndex);
    void  setSelectedByValue(DWORD dwValue);
    char  *getSelectedString();
  };
#pragma pack()
  bool __fastcall testInteract(dialog *dlg, dlgEvent *evt);
};
