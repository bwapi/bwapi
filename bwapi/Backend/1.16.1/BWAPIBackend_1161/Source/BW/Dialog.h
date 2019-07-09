#pragma once
#include "Bitmap.h"
#include <string>

#pragma pack(1)

#define CTRL_UPDATE           0x00000001
#define CTRL_DISABLED         0x00000002
#define CTRL_ACTIVE           0x00000004
#define CTRL_VISIBLE          0x00000008
#define CTRL_EVENTS           0x00000010
// 0x00000020
#define CTRL_BTN_CANCEL       0x00000040
#define CTRL_BTN_NO_SOUND     0x00000080
#define CTRL_BTN_VIRT_HOTKEY  0x00000100
#define CTRL_BTN_HOTKEY       0x00000200
#define CTRL_FONT_SMALLEST    0x00000400
#define CTRL_FONT_LARGE       0x00000800
//                              0x00001000 // button & option button uses this
#define CTRL_TRANSPARENT      0x00002000
#define CTRL_FONT_LARGEST     0x00004000
// 0x00008000
#define CTRL_FONT_SMALL       0x00010000
#define CTRL_PLAIN            0x00020000
#define CTRL_TRANSLUCENT      0x00040000
#define CTRL_BTN_DEFAULT      0x00080000
#define CTRL_TOP              0x00100000
#define CTRL_HALIGN_LEFT      0x00200000
#define CTRL_HALIGN_RIGHT     0x00400000
#define CTRL_HALIGN_CENTER    0x00800000
#define CTRL_VALIGN_TOP       0x01000000
#define CTRL_VALIGN_MIDDLE    0x02000000
#define CTRL_VALIGN_BOTTOM    0x04000000
// 0x08000000
#define CTRL_DLG_NOREDRAW     0x10000000
#define CTRL_REVERSE          0x10000000
#define CTRL_USELOCALGRAPHIC  0x20000000
#define CTRL_DLG_ACTIVE       0x40000000
#define CTRL_LBOX_NORECALC    0x80000000

#define BW_EVN_KEYFIRST       0
#define BW_EVN_KEYDOWN        0
#define BW_EVN_KEYRPT         1
#define BW_EVN_KEYREPEAT      1
#define BW_EVN_KEYUP          2
#define BW_EVN_MOUSEMOVE      3
#define BW_EVN_MOUSEFIRST     3
#define BW_EVN_LBUTTONDOWN    4
#define BW_EVN_LBUTTONUP      5
#define BW_EVN_LBUTTONDBLCLK  6
#define BW_EVN_RBUTTONDOWN    7
#define BW_EVN_RBUTTONUP      8
#define BW_EVN_RBUTTONDBLCLK  9
#define BW_EVN_MBUTTONDOWN    10
#define BW_EVN_MBUTTONUP      11
#define BW_EVN_MBUTTONDBLCLK  12
#define BW_EVN_IDLE           13
#define BW_EVN_USER           14
#define BW_EVN_CHAR           15
#define BW_EVN_SYSCHAR        16
#define BW_EVN_WHEELUP        17
#define BW_EVN_WHEELDWN       18
#define BW_EVN_MAX            19

#define BW_USER_CREATE        0
#define BW_USER_DESTROY       1
#define BW_USER_ACTIVATE      2
#define BW_USER_HOTKEY        3
#define BW_USER_MOUSEMOVE     4
#define BW_USER_PREV          5
#define BW_USER_NEXT          6
#define BW_USER_UNK_7         7
#define BW_USER_UNK_8         8
#define BW_USER_MOUSEENTER    9
#define BW_USER_INIT          10
#define BW_USER_SELECT        11
#define BW_USER_UNK_12        12
#define BW_USER_SHOW          13
#define BW_USER_HIDE          14
#define BW_USER_SCROLLUP      15
#define BW_USER_SCROLLDOWN    16

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
    u16  x;
    u16  y;
  };

  struct dlgEvent
  {
    u32 dwUser;
    u16  wSelection;
    u16  wUnk_0x06;
    u16  wVirtKey;
    u16  wUnk_0x0A;
    u16  wNo;
    pt    cursor;
    u16  wUnk_0x12;
  };

#pragma pack(1)
  class dialog   // BIN Dialog
  {
  public:
    typedef bool __fastcall FnInteract(dialog* dlg, dlgEvent* evt);
    typedef void __fastcall FnUpdate(dialog* dlg, int x, int y, rect* dst);
    typedef void __fastcall FnDrawItem(dialog* dlg, u8 selectedIndex, rect* dstRect, int x, int y);

    // global functions
    dialog  *findIndex(short wIndex);         // Searches for a control that matches the specified index
    dialog  *findDialog(const char *pszName); // Searches for a dialog that matches the name specified
    dialog  *next() const;                    // Retrieves the next dialog or control in the list

    bool    setFlags(u32 dwFlags);        // Sets a flag or set of flags for the control or dialog
    bool    clearFlags(u32 dwFlags);      // Clears a flag or set of flags for the control or dialog
    bool    hasFlags(u32 dwFlags) const;  // Returns true if the dialog or control has all of the specified flags enabled
    bool    setText(char *pszStr);        // Sets the text of a control, or name of a dialog
    const char *getText() const;          // Retrieves the text of a control, or name of a dialog
    int     getHotkey() const;            // Retrieves the hotkey for the button

    BW::Bitmap  *getSourceBuffer();   // Retrieves a pointer to a bitmap structure for reading or writing to the source buffer

    bool enable();            // Enables the dialog or control
    bool disable();           // Disables the dialog or control
    bool isDisabled() const;  // Returns true if the dialog or control is disabled
    bool show();              // Shows the dialog or control
    bool hide();              // Hides the dialog or control
    bool isVisible() const;   // Returns true if the dialog or control is visible

    u16 width() const;
    u16 height() const;

    // event-specific functions
    bool doEvent(u16 wEvtNum, u32 dwUser = 0, u16 wSelect = 0, u16 wVirtKey = 0); // Calls a dialog or control's interact function by generating event info using these parameters
    // cppcheck-suppress functionConst
    //bool defaultInteract(BW::dlgEvent *pEvent); // Calls a dialog or control's default interact function using this event info
    bool activate();        // Activates a control or destroys a dialog
    bool update();          // Updates a control or dialog, refreshing it on the screen

    // dialog-specific functions
    bool        isDialog() const;         // Returns true if the control type is a dialog
    dialog      *child() const;           // Retrieves the child control from the parent dialog
    bool        addControl(dialog *ctrl); // Adds a control to this dialog
    bool        initialize();             // Performs the dialog's initialization and adds it to the list
    bool        isListed() const;         // Checks to see if this dialog is initialized/listed

    // control-specific functions
    dialog  *parent() const;              // Retrieves a control's parent dialog
    short   getIndex() const;             // Retrieves the index of a control
    bool    clearFontFlags();             // Clears all font formatting flags

    // button-specific
    bool isButton() const;      // Returns true if the control type is a button

    // checkbox & option button
    bool isOption() const;      // Returns true if the control type is a checkbox or radio button
    bool isChecked() const;     // Returns true if the control (checkbox/radio) is selected

    // listbox & combobox
    bool  isList() const;               // Returns true if the control type is a listbox or combobox
    u8    getSelectedIndex() const;     // Returns the index of the selected element
    u32   getSelectedValue() const;     // Returns the value of the selected element
    const char  *getSelectedString() const;   // Returns the name of the selected element

    bool  setSelectedIndex(u8 bIndex);              // Sets the selected index
    bool  setSelectedByValue(u32 dwValue);          // Sets the selected index based on the given value
    bool  setSelectedByString(const char *pszString, bool noctrl = true); // Sets the selected index based on its name, noctrl = ignore control characters
    bool  setSelectedByString(const std::string &s, bool noctrl = true); // Sets the selected index based on its name, noctrl = ignore control characters

    bool  addListEntry(char *pszString, u32 dwValue = 0, u8 bFlags = 0);  // Adds an entry to a listbox or combobox
    bool  removeListEntry(u8 bIndex = 0);   // Removes an entry from a listbox or combobox
    bool  clearList();                      // Removes all entries from a listbox or combobox
    u8  getListCount() const;               // Retrieves the number of elements in a listbox or combobox


    // Data //
    dialog  *pNext;         // 0x00
    rect    rct;            // 0x04   // official
    Bitmap  srcBits;        // 0x0C   // official
    char    *pszText;       // 0x14   // official
    u32     lFlags;         // 0x18   // official
    u16     wUnk_0x1C;
    u16     wUnk_0x1E;
    s16     wIndex;         // 0x20   // official
    u16     wCtrlType;      // 0x22   // official
    u16     wUser;          // 0x24   // official
    /*
      CHECKBOX
        0-2 Show/Hide minimap button
        5   Disabled bullet
        6   Standard bullet
        7   Active bullet
        10  Disabled checkbox
        11  Standard checkbox
        12  Active checkbox
        82  Disabled alliance
        83  Standard Alliance
        85  Disabled Skull
        86  Standard Skull (changes to bird when checked)
        87  Active Skull
        98  Grey bullet thingy
        129 Disabled Messaging
        130 Standard Messaging
        131 Active Messaging
        136 White box + white Xmark
        139 Disabled Vision
        140 Standard Vision
        141 Active Vision
        142 Weird glowy bullet 1
        143 Weird glowy bullet 2

      BUTTON
        100 Grey Left menu button
        103 Left menu button
        106 Highlighted Left menu button
        109 Grey Center menu button
        112 Center menu button
        115 Highlighted Center menu button
        118 Grey Right menu button
        121 Right menu button
        124 Highlighted Right menu button

    */
    s32    lUser;          // 0x26   // official name
    FnInteract* pfcnInteract;  // 0x2A    // bool __fastcall pfcnInteract(dialog *dlg, dlgEvent *evt);
    FnUpdate*   pfcnUpdate;    // 0x2E    // void __fastcall pfcnUpdate(dialog *dlg, int x, int y, rect *dest);
    
    union _u                 // 0x32   // official
    {
      struct _ctrl           // official
      {
        dialog  *pDlg;      // 0x32   // official
      } ctrl;
      
      struct _dlg            // official
      {
        u32   dwUnk_0x32;
        struct{
          u16 wid, ht;
          u8 *data;
        } dstBits;                // 0x36  // official 
        dialog  *pActiveElement;  // 0x3E
        dialog  *pFirstChild;     // 0x42  // official
        dialog  *pMouseElement;
        void    *pModalFcn;       // 0x4A   // official
      } dlg;
      
      struct _btn
      {
        dialog* pDlg;         // 0x32   // official
        rect    responseRct;   // 0x36
        u16     wHighlight;     // 0x3E
        u16     wUnknown_0x40;  // padding?
        void*   pSmk;         // 0x42
        rect    textRct;       // 0x46
        u16     wAlignment;     // 0x4E
      } btn;

      struct _optn
      {
        dialog* pDlg;        // 0x32   // official
        u8      bEnabled;       // 0x36
      } optn;
      
      struct _edit
      {
        dialog* pDlg;          // 0x32   // official
        u8      bColor;           // 0x36
        u8      bScrollPosition;  // 0x37
        u8      bLeftMargin;      // 0x38
        u8      bTopMargin;       // 0x39
        u16     wUnk_0x3A;       // 0x3A
        u16     wUnk_0x3C;
        u8      bCursorPos;       // 0x3E
      } edit;
      
      struct _scroll   // official
      {
        dialog* pDlg;      // 0x32   // official
        void*   pfcnScrollerUpdate;  // 0x36
        u16     nCurPos;    // 0x3A
        u16     nMin;       // 0x3C   // official
        u16     nMax;       // 0x3E   // official
        u16     wUnk_0x40;    
        u32     dwScrollFlags;    // 0x42
        u8      bSliderSkip;      // 0x46
        u8      bUpdateCounter;   // 0x47
        u8      bSliderGraphic;   // 0x48
        u8      bSliderSpacing;   // 0x49
        u16     wUnk_0x4A;
        u32     dwUnk_0x4C;
      } scroll;
      
      struct _list   // official
      {
        dialog* pDlg;        // 0x32   // official
        dialog* pScrlBar;    // 0x36
        char**  ppStrs;      // 0x3A   // official
        u8*     pbStrFlags;  // 0x3E   // official
          /*
            &0x0F specifies the colour of the entry (Standard colours unless noted)
              0x01  = Use previous (Uses the colour of the entry BEFORE the previous one)
              0x09 - 0x0D  = Use last (Uses the colour of the previous entry)
            &0xF0 specifies additional properties
              0x10  = Gives entry an option button
          */
        u32*  pdwData;         // 0x42   // official
        u8    bStrs;            // 0x46   // official
        u8    unknown_0x47;
        u8    bCurrStr;         // 0x48   // official
        u8    bSpacing;         // 0x49
        u8    bItemsPerPage;    // 0x4A
        u8    bUnknown_0x4B;
        u8    bDirection;       // 0x4C
        u8    bOffset;          // 0x4D
        u8    bSelectedIndex;   // 0x4E
        u8    bUnknown_0x4F;    // padding?
        u16   wVerticalOffset;  // 0x4A
        FnDrawItem* pDrawItemFcn;    // 0x4C  // void __fastcall pDrawItemFcn(dialog *dlg, u8 selectedIndex, rect *dstRect, int x, int y)
      } list;
    } u;
  };
#pragma pack()
  //dialog  *CreateDialogWindow(const char *pszText, u16 wLeft, u16 wTop, u16 wWidth, u16 wHeight); // Creates a custom window dialog
  dialog  *FindDialogGlobal(const char *pszName);   // Finds a dialog in Starcraft's global list of dialogs
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
  u32 dwUnk1;
  u32 dwUnk2;
  u32 dwUnk3;
  u32 dwUnk4;
  u32 dwUnk5;
  char  szFullPath[261];
  char  szFileName[261];
  u16  wWidth;
  u16  wHeight;
  u16  wTileset; // or numPlayers? guess
  u8  bReserved; // players reserved? guess
  u8  bPlayers;
  u8  bComputers; // for UMS
  u8  bHumans; // guess
*/
#pragma pack()
