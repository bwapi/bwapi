#include "Dialog.h"

namespace BW
{
  bool __fastcall WindowInteract(dialog *dlg, dlgEvent *evt)
  {
    char out[512];
    pt   *mouseOffset = (pt*)&dlg->lUser;
    switch (evt->wNo)
    {
    case 0:
      // Key down
      break;
    case 1:
      // Key repeat
      break;
    case 2:
      // Key Modifier?? Key up?
      break;
    case 3:
      // Mouse update/Move
      if ( dlg->wUnk_0x24 )
      {
        dlg->rct.Xmin = evt->cursor.x - mouseOffset->x;
        dlg->rct.Xmax = dlg->rct.Xmin + dlg->srcBits.wid - 1;
        dlg->rct.Ymin = evt->cursor.y - mouseOffset->y;
        dlg->rct.Ymax = dlg->rct.Ymin + dlg->srcBits.ht - 1;
      }
      break;
    case 4:
      // Left Button down
      if ( evt->cursor.x >= dlg->rct.Xmin &&
         evt->cursor.x <= dlg->rct.Xmax &&
         evt->cursor.y >= dlg->rct.Ymin &&
         evt->cursor.y <= dlg->rct.Ymin + 12 )
      {
        mouseOffset->x = evt->cursor.x - dlg->rct.Xmin;
        mouseOffset->y = evt->cursor.y - dlg->rct.Ymin;
        dlg->wUnk_0x24 = 1;
      }
      break;
    case 5:
      // Left Button up
      dlg->wUnk_0x24 = 0;
      break;
    case 6:
      // Left Button Something (usually paired with down
      break;
    case 7:
      // Right Button Down
      break;
    case 8:
      // Right Button Up
      break;
    case 9:
      // Right Button Something (usually paired with down)
      break;
    case 10:
      // Middle Button Down
      break;
    case 11:
      // Middle Button Up
      break;
    case 12:
      // Middle Button Something (usually paired with down)
      break;
    case 13:
      // unknown; Loop? Always?
      break;
    case 14:
      // Init/Control (used for when a control has been pressed)
      break;
    case 15:
      // User Key press
      break;
    case 17:
      // Mouse wheel Scroll up
      break;
    case 18:
      // Mouse wheel Scroll down
      break;
    default:
      sprintf_s(out, 512, "Event: %d\nUser: 0x%p", evt->wNo, evt->dwUser);
      MessageBoxA(NULL, out, "!", MB_OK);
      break;
    }
    return false;
  }

  // ------------------ CREATE DLG WINDOW ------------
  dialog *CreateDialogWindow(const char *text, WORD top, WORD left, WORD width, WORD height)
  {
    dialog *dlg = new dialog(ctrls::cDLG, 0, text, top, left, width, height, &WindowInteract);
    BYTE *data  = dlg->srcBits.data;
    // Title bar
    // first line
    memset(data, 0x00, 3);
    memset(&data[3], 0x2A, width - 6);
    memset(&data[width - 3], 0x00, 3);

    // second line
    data[width] = 0;
    memset(&data[width + 1], 0x2A, 2);
    memset(&data[width + 3], 0x2C, width - 6);
    memset(&data[width*2 - 3], 0x2A, 2);
    data[width*2 - 1] = 0;

    // third line
    data[width*2] = 0;
    data[width*2 + 1] = 0x2A;
    memset(&data[width*2 + 2], 0x2C, width - 4);
    data[width*3 - 2] = 0x2A;
    data[width*3 - 1] = 0;

    for (int i = 3; i < 12; i++)
    {
      data[width*i] = 0x2A;
      memset(&data[width*i + 1], 0x2C, width - 2);
      data[width*(i+1) - 1] = 0x2A;
    }

    // dialog
    for (int i = 12; i < height - 3; i++)
    {
      data[width*i] = 0x2A;
      memset(&data[width*i + 1], 0x29, width - 2);
      data[width*(i+1) - 1] = 0x2A;
    }

    // third-last line
    data[width*height - width*3] = 0;
    data[width*height - width*3 + 1] = 0x2A;
    memset(&data[width*height - width*3 + 2], 0x29, width - 4);
    data[width*height - width*2 - 2] = 0x2A;
    data[width*height - width*2 - 1] = 0;

    // second-last line
    data[width*height - width*2] = 0;
    memset(&data[width*height - width*2 + 1], 0x2A, 2);
    memset(&data[width*height - width*2 + 3], 0x29, width - 6);
    memset(&data[width*height - width - 3], 0x2A, 2);
    data[width*height - width - 1] = 0;

    // last line
    memset(&data[width*height - width], 0x00, 3);
    memset(&data[width*height - width + 3], 0x2A, width - 6);
    memset(&data[width*height - 3], 0x00, 3);

    dialog *title = new dialog(ctrls::cLSTATIC, -255, text, 8, 1, width - 48, 12);
    title->setFlag(CTRL_FONT_SMALLEST);
    dlg->add(title);

    dialog *close = new dialog(ctrls::cBUTTON, -254, "\x1Bx", 8, 8, 120, 64);
    close->setFlag(CTRL_FONT_SMALLEST | CTRL_ALTERNATE);
    dlg->add(close);
    return dlg;
  }

// -------------------------------------------------- GLOBAL -------------------------------------------------
  // ----------------- CONSTRUCTORS ------------------
  dialog::dialog(WORD ctrlType, short index, WORD top, WORD left, WORD width, WORD height)
  {
    if ( ctrlType > ctrls::max)
      ctrlType = ctrls::cLSTATIC;

    memset(this, 0, sizeof(dialog));
    rct.Xmin      = top;
    rct.Ymin      = left;
    rct.Xmax      = rct.Xmin + width - 1;
    rct.Ymax      = rct.Ymin + height - 1;
    srcBits.wid   = width;
    srcBits.ht    = height;

    pszText       = "";
    lFlags        = CTRL_VISIBLE;
    if ( ctrlType == ctrls::cDLG )
      lFlags      |= CTRL_DLG_NOREDRAW | CTRL_TRANSLUCENT;

    if ( ctrlType == ctrls::cBUTTON || ctrlType == ctrls::cDFLTBTN )
    {
      lFlags      |= 0x20001A18; //CTRL_EVENTS; // | CTRL_ALTERNATE
      wUnk_0x24   = 112;
    }

    wIndex        = index;
    wCtrlType     = ctrlType;

    pfcnInteract  = BW::BWDATA_GenericDlgInteractFxns[wCtrlType];
    pfcnUpdate    = BW::BWDATA_GenericDlgUpdateFxns[wCtrlType];

    if ( ctrlType == ctrls::cDLG )
    {
      srcBits.data        = (BYTE*)malloc(width*height);
      memset(srcBits.data, 0x29, width*height);
      u.dlg.dstBits.wid   = width;
      u.dlg.dstBits.ht    = height;
      u.dlg.dstBits.data  = (BYTE*)malloc(width*height);
    }
  }
  dialog::dialog(WORD ctrlType, short index, const char *text, WORD top, WORD left, WORD width, WORD height, bool (__fastcall *pfInteract)(dialog*,dlgEvent*), void (__fastcall *pfUpdate)(dialog*,int,int,rect*))
  {
    if ( ctrlType > ctrls::max)
      ctrlType = ctrls::cLSTATIC;

    memset(this, 0, sizeof(dialog));
    rct.Xmin        = top;
    rct.Ymin        = left;
    rct.Xmax        = rct.Xmin + width - 1;
    rct.Ymax        = rct.Ymin + height - 1;
    srcBits.wid     = width;
    srcBits.ht      = height;

    pszText         = (char*)text;
    lFlags        = CTRL_VISIBLE;
    if ( ctrlType == ctrls::cDLG )
      lFlags      |= CTRL_DLG_NOREDRAW | CTRL_TRANSLUCENT;

    if ( ctrlType == ctrls::cBUTTON || ctrlType == ctrls::cDFLTBTN )
    {
      lFlags      |= CTRL_EVENTS;
      wUnk_0x24   = 112;
    }

    wIndex          = index;
    wCtrlType       = ctrlType;

    if ( pfInteract )
      pfcnInteract  = pfInteract;
    else
      pfcnInteract  = BW::BWDATA_GenericDlgInteractFxns[wCtrlType];

    if ( pfUpdate )
      pfcnUpdate    = pfUpdate;
    else
      pfcnUpdate    = BW::BWDATA_GenericDlgUpdateFxns[wCtrlType];

    if ( ctrlType == ctrls::cDLG )
    {
      srcBits.data        = (BYTE*)malloc(width*height);
      memset(srcBits.data, 0x29, width*height);
      u.dlg.dstBits.wid   = width;
      u.dlg.dstBits.ht    = height;
      u.dlg.dstBits.data  = (BYTE*)malloc(width*height);
    }
  }
  // ------------------ DESTRUCTOR -------------------
  dialog::~dialog()
  {
    if ( this )
    {
      if ( this->wCtrlType == ctrls::cDLG )
      {
        dialog *prevDialog = (*BW::BWDATA_ScreenDialog);
        while ( prevDialog && prevDialog->pNext != NULL && prevDialog->pNext != this)
          prevDialog = prevDialog->pNext;
        
        if ( prevDialog->pNext == this )
          prevDialog->pNext = this->next();

        dialog *child = this->child();
        while ( child )
        {
          dialog *nextChild = child->next();
          delete child;
          child = nextChild;
        }
        free(this->u.dlg.dstBits.data);
      }
      else
      {
        dialog *previous = this->parent()->child();
        if ( previous == this )
        {
          this->parent()->u.dlg.pFirstChild = this->pNext;
        }
        else
        {
          while ( previous->pNext && previous->pNext != this)
            previous = previous->next();

          if ( previous->pNext == this )
            previous->pNext = this->pNext;
        }
      }
    }
  }
  // --------------------- FIND ----------------------
  dialog *dialog::FindIndex(short wIndex)
  {
    if ( !this )
      return NULL;

    dialog *pCurrDlg;
    if ( this->isDialog() )
      pCurrDlg = this;
    else
      pCurrDlg = this->parent();

    if ( pCurrDlg )
      pCurrDlg = pCurrDlg->child();

    while (pCurrDlg != NULL && pCurrDlg->getIndex() != wIndex)
      pCurrDlg = pCurrDlg->next();
    
    if ( pCurrDlg->getIndex() == wIndex )
      return pCurrDlg;

    return NULL;
  }
  // --------------------- ADD -----------------------
  void dialog::add(dialog *dlg)
  {
    if ( !this || !dlg)
      return;

    dialog *parent = this;
    if ( !parent->isDialog() )
      parent = parent->parent();

    if ( dlg->isDialog() )
    {
      while ( parent->pNext )
        parent = parent->pNext;
      parent->pNext = dlg;
    }
    else
    {
      dlg->u.ctrl.pDlg = parent;
      if ( !parent->child() )
      {
        parent->u.dlg.pFirstChild = dlg;
      }
      else
      {
        dialog *child = parent->child();
        while ( child->pNext )
          child = child->pNext;
        child->pNext = dlg;
      }
    }
  }
  // --------------------- NEXT ----------------------
  dialog *dialog::next()
  {
    if ( this && this->pNext )
      return this->pNext;
    return NULL;
  }
  // ------------------ FIND BY NAME -----------------
  dialog *dialog::findDialogByName(const char *pszName)
  {
    if ( this )
    {
      dialog *parent = this;
      if ( !parent->isDialog() )
        parent = parent->parent();
      
      while ( parent )
      {
        if (parent->pszText && strcmpi(parent->pszText, pszName) == 0 )
          return parent;
        parent = parent->next();
      }
    }
    return NULL;
  }
  // --------------------- FLAGS ---------------------
  void dialog::setFlag(DWORD dwFlag)
  {
    if ( this )
      this->lFlags |= dwFlag;
  }
  void dialog::clearFlag(DWORD dwFlag)
  {
    if ( this )
      this->lFlags &= ~dwFlag;
  }
// -------------------------------------------------- DIALOG -------------------------------------------------
  // --------------------- IS DLG --------------------
  bool dialog::isDialog()
  {
    if ( this && this->wCtrlType == ctrls::cDLG )
      return true;
    return false;
  }
  // --------------------- CHILD ---------------------
  dialog *dialog::child()
  {
    if (this && this->isDialog() && this->u.dlg.pFirstChild )
      return this->u.dlg.pFirstChild;
    return NULL;
  }
// -------------------------------------------------- CONTROL ------------------------------------------------
  // -------------------- PARENT ---------------------
  dialog *dialog::parent()
  {
    if ( this && !this->isDialog() && this->u.ctrl.pDlg )
      return this->u.ctrl.pDlg;
    return NULL;
  }
  // --------------------- INDEX ---------------------
  short dialog::getIndex()
  {
    if ( this )
      return this->wIndex;
    return 0;
  }
  // ------------------- IS CHECKBOX -----------------
  bool dialog::isOption()
  {
    if ( this )
    {
      WORD wCtrl = this->wCtrlType;
      if ( wCtrl == ctrls::cCHKBOX || wCtrl == ctrls::cOPTION)
        return true;
    }
    return false;
  }
  // --------------------- CHECKED -------------------
  bool dialog::isChecked()
  {
    if ( this && this->isOption() )
      return this->u.optn.bEnabled != 0;
    return false;
  }
// -------------------------------------------- LISTBOX & COMBOBOX -------------------------------------------
// ----------------------- IS LIST -------------------
  bool dialog::isList()
  {
    if ( this )
    {
      WORD wCtrl = this->wCtrlType;
      if ( wCtrl == ctrls::cCOMBO || wCtrl == ctrls::cLIST )
        return true;
    }
    return false;
  }
// ------------------- GET SELECTED ------------------
  BYTE dialog::getSelectedIndex()
  {
    if ( this && this->isList() )
      return this->u.list.bSelectedIndex;
    return 0;
  }
  DWORD dialog::getSelectedValue()
  {
    if ( this 
        && this->isList() 
        && this->u.list.pdwData 
        && this->u.list.bSelectedIndex < this->u.list.bStrs)
      return this->u.list.pdwData[this->u.list.bSelectedIndex];
    return 0;
  }
  char *dialog::getSelectedString()
  {
    if ( this && this->isList() && this->u.list.ppStrs && this->u.list.ppStrs[this->u.list.bCurrStr])
      return this->u.list.ppStrs[this->u.list.bCurrStr];
    return "";
  }
// ------------------- SET SELECTED ------------------
  bool dialog::setSelectedIndex(BYTE bIndex)
  {
    if ( this && this->isList() && bIndex < this->u.list.bStrs )
    {
      this->u.list.bCurrStr       = bIndex;
      this->u.list.bSelectedIndex = bIndex;
      return true;
    }
    return false;
  }
  bool dialog::setSelectedByValue(DWORD dwValue)
  {
    if ( this && this->isList() && this->u.list.pdwData)
    {
      for (int i = 0; i < this->u.list.bStrs; i++)
      {
        if ( this->u.list.pdwData[i] == dwValue)
        {
          this->u.list.bCurrStr       = (BYTE)i;
          this->u.list.bSelectedIndex = (BYTE)i;
          return true;
        }
      }
    } // check
    return false;
  }
  bool dialog::setSelectedByString(char *pszString)
  {
    // verify that this is the correct control
    if ( this && this->isList() )
    {
      // Iterate through each entry
      for (int i = 0; i < this->u.list.bStrs; i++)
      {
        // Verify pointer validity
        if ( this->u.list.ppStrs && this->u.list.ppStrs[i] )
        {
          // compare the string to the one we're looking for
          if ( strcmpi(this->u.list.ppStrs[i], pszString) == 0 )
          {
            // set the selected entry
            this->u.list.bCurrStr       = (BYTE)i;
            this->u.list.bSelectedIndex = (BYTE)i;
            return true;
          }
        } // pointer validate
      } // iterator
    }
    return false;
  }

};
