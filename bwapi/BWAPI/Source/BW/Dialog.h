#pragma once
#include <windows.h>

#include "offsets.h"

#define CTRL_DISABLED         0x00000002
#define CTRL_ACTIVE           0x00000004
#define CTRL_VISIBLE          0x00000008
#define CTRL_EVENTS           0x00000010

#define CTRL_BTN_CANCEL       0x00000040
#define CTRL_BTN_NO_SOUND     0x00000080
#define CTRL_BTN_VIRT_HOTKEY  0x00000100
#define CTRL_BTN_HOTKEY       0x00000200
#define CTRL_FONT_SMALLEST    0x00000400
#define CTRL_FONT_LARGE       0x00000800
//                              0x00001000 // button & option button uses this
#define CTRL_TRANSPARENT      0x00002000
#define CTRL_FONT_LARGEST     0x00004000

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

#define CTRL_DLG_NOREDRAW     0x10000000
#define CTRL_REVERSE          0x10000000
#define CTRL_USELOCALGRAPHIC  0x20000000
#define CTRL_DLG_ACTIVE       0x40000000
#define CTRL_LBOX_NORECALC    0x80000000

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
    WORD  wSelection;
    WORD  wUnk_0x06;
    WORD  wVirtKey;
    WORD  wUnk_0x0A;
    WORD  wNo;
    pt    cursor;
    WORD  wUnk_0x12;
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

  struct fntChr
  {
    BYTE w, h, x, y;
    BYTE data[1];
  };

  struct fntHead
  {
    DWORD   magic;
    BYTE    low, high, Xmax, Ymax;
    fntChr  *chrs[1];
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
    WORD    wUnk_0x1C;
    WORD    wUnk_0x1E;
    short   wIndex;         // 0x20   // official name
    WORD    wCtrlType;      // 0x22   // official name
    WORD    wGraphic;       // 0x24
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
    LONG    lUser;          // 0x26   // official name
    bool (__fastcall *pfcnInteract)(dialog*,dlgEvent*);  // 0x2A
    // bool __fastcall pfcnInteract(dialog *dlg, dlgEvent *evt);
    void (__fastcall *pfcnUpdate)(dialog*,int,int,rect*);    // 0x2E
    // void __fastcall pfcnUpdate(dialog *dlg, int x, int y, rect *dest);
    union _u                 // 0x32   // official
    {
      struct _ctrl           // official
      {
        dialog  *pDlg;      // 0x32   // official
      } ctrl;
      
      struct _dlg            // official
      {
        DWORD   dwUnk_0x32;
        bitmap  dstBits;          // 0x36  // official 
        dialog  *pActiveElement;  // 0x3E
        dialog  *pFirstChild;     // 0x42  // official
        dialog  *pMouseElement;
        void    *pModalFcn;       // 0x4A   // official
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
        dialog  *pDlg;            // 0x32   // official
        BYTE    bColor;           // 0x36
        BYTE    bScrollPosition;  // 0x37
        BYTE    bLeftMargin;      // 0x38
        BYTE    bTopMargin;       // 0x39
        WORD    wUnk_0x3A;        // 0x3A
        WORD    wUnk_0x3C;
        BYTE    bCursorPos;       // 0x3E
      } edit;
      
      struct _scroll   // official
      {
        dialog  *pDlg;      // 0x32   // official
        void  *pfcnScrollerUpdate;  // 0x36
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
        DWORD   *pdwData;         // 0x42   // official
        BYTE    bStrs;            // 0x46   // official
        BYTE    unknown_0x47;
        BYTE    bCurrStr;         // 0x48   // official
        BYTE    bSpacing;         // 0x49
        BYTE    bItemsPerPage;    // 0x4A
        BYTE    bUnknown_0x4B;
        BYTE    bDirection;       // 0x4C
        BYTE    bOffset;          // 0x4D
        BYTE    bSelectedIndex;   // 0x4E
        BYTE    bUnknown_0x4F;
        WORD    wVerticalOffset;  // 0x4A
        void    *pDrawItemFcn;    // 0x4C
      } list;
    } u;

    // global functions
    dialog(WORD ctrlType, short index, const char *text, WORD left, WORD top, WORD width, WORD height = 0, bool (__fastcall *pfInteract)(dialog*,dlgEvent*) = NULL);
    ~dialog();

    dialog  *findIndex(short wIndex); // Searches for a control that matches the specified index
    dialog  *findDialog(const char *pszName); // Searches for a dialog that matches the name specified
    dialog  *next();                  // Retrieves the next dialog or control in the list

    bool    setFlags(DWORD dwFlags);    // Sets a flag or set of flags for the control or dialog
    bool    clearFlags(DWORD dwFlags);  // Clears a flag or set of flags for the control or dialog
    bool    hasFlags(DWORD dwFlags);    // Returns true if the dialog or control has all of the specified flags enabled
    bool    setText(char *pszStr);      // Sets the text of a control, or name of a dialog
    char    *getText();                 // Retrieves the text of a control, or name of a dialog
    int     getHotkey();                // Retrieves the hotkey for the button

    BW::bitmap  *getSourceBuffer();   // Retrieves a pointer to a bitmap structure for reading or writing to the source buffer

    bool        enable();     // Enables the dialog or control
    bool        disable();    // Disables the dialog or control
    bool        isDisabled(); // Returns true if the dialog or control is disabled
    bool        show();       // Shows the dialog or control
    bool        hide();       // Hides the dialog or control
    bool        isVisible();  // Returns true if the dialog or control is visible

    // event-specific functions
    bool doEvent(WORD wEvtNum, DWORD dwUser = 0, WORD wSelect = 0, WORD wVirtKey = 0); // Calls a dialog or control's interact function by generating event info using these parameters
    bool defaultInteract(BW::dlgEvent *pEvent); // Calls a dialog or control's default interact function using this event info
    bool activate();        // Activates a control or destroys a dialog
    bool update();          // Updates a control or dialog, refreshing it on the screen

    // dialog-specific functions
    bool        isDialog();               // Returns true if the control type is a dialog
    dialog      *child();                 // Retrieves the child control from the parent dialog
    BW::bitmap  *getDestBuffer();         // Retrieves a pointer to a bitmap structure for reading or writing to the dialog's destination buffer
    bool        addControl(dialog *ctrl); // Adds a control to this dialog
    bool        initialize();             // Performs the dialog's initialization and adds it to the list
    bool        isListed();               // Checks to see if this dialog is initialized/listed
    bool        applyDialogBackground();  // Applies the standard transparent dialog background (like game menu)
    bool        applyWindowBackground();  // Applies the custom window background for the window dialog
    bool        applyBlankBackground();   // Applies a completely invisible background

    // control-specific functions
    dialog  *parent();                      // Retrieves a control's parent dialog
    short   getIndex();                     // Retrieves the index of a control
    bool    clearFontFlags();               // Clears all font formatting flags

    // button-specific
    bool isButton();      // Returns true if the control type is a button

    // checkbox & option button
    bool isOption();      // Returns true if the control type is a checkbox or radio button
    bool isChecked();     // Returns true if the control (checkbox/radio) is selected

    // listbox & combobox
    bool  isList();               // Returns true if the control type is a listbox or combobox
    BYTE  getSelectedIndex();     // Returns the index of the selected element
    DWORD getSelectedValue();     // Returns the value of the selected element
    char  *getSelectedString();   // Returns the name of the selected element

    bool  setSelectedIndex(BYTE bIndex);        // Sets the selected index
    bool  setSelectedByValue(DWORD dwValue);    // Sets the selected index based on the given value
    bool  setSelectedByString(char *pszString); // Sets the selected index based on its name

    bool  addListEntry(char *pszString, DWORD dwValue = 0, BYTE bFlags = 0);  // Adds an entry to a listbox or combobox
    bool  removeListEntry(BYTE bIndex = 0);   // Removes an entry from a listbox or combobox
    bool  clearList();                        // Removes all entries from a listbox or combobox
    BYTE  getListCount();                     // Retrieves the number of elements in a listbox or combobox
  };
#pragma pack()
  dialog  *CreateDialogWindow(const char *pszText, WORD wLeft, WORD wTop, WORD wWidth, WORD wHeight); // Creates a custom window dialog
  dialog  *CreateCanvas(const char *pszName);       // Creates an invisible dialog the size of the screen used for drawing purposes
  dialog  *FindDialogGlobal(const char *pszName);   // Finds a dialog in Starcraft's global list of dialogs
  int     GetTextWidth(const char *pszString, BYTE bSize);  // Retrieves the width of the text string
  int     GetTextHeight(const char *pszString, BYTE bSize); // Retrieves the height of the text string
  bool    BlitText(const char *pszString, bitmap *dst, int x, int y, BYTE bSize); // Draws a string of text to a destination bitmap buffer
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