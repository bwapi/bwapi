#include "Dialog.h"

namespace BW
{
// -------------------------------------------------- GLOBAL -------------------------------------------------
  // ----------------- CONSTRUCTORS ------------------
  dialog::dialog(WORD ctrlType, short index, WORD width, WORD height)
  {
    if ( ctrlType > 14)
      ctrlType = 9;

    rct.Xmin      = 100;
    rct.Ymin      = 100;
    rct.Xmax      = rct.Xmin + width;
    rct.Ymax      = rct.Ymin + height;
    srcBits.wid   = width;
    srcBits.ht    = height;

    pszText       = "";
    lFlags        = CTRL_VISIBLE;

    wIndex        = index;
    wCtrlType     = ctrlType;

    pfcnInteract  = BW::BWDATA_GenericDlgInteractFxns[wCtrlType];
    pfcnUpdate    = BW::BWDATA_GenericDlgUpdateFxns[wCtrlType];
  }
  dialog::dialog(WORD ctrlType, short index, WORD top, WORD left, WORD width, WORD height)
  {
    if ( ctrlType > 14)
      ctrlType = 9;

    rct.Xmin      = top;
    rct.Ymin      = left;
    rct.Xmax      = rct.Xmin + width;
    rct.Ymax      = rct.Ymin + height;
    srcBits.wid   = width;
    srcBits.ht    = height;

    pszText       = "";
    lFlags        = CTRL_VISIBLE;

    wIndex        = index;
    wCtrlType     = ctrlType;

    pfcnInteract  = BW::BWDATA_GenericDlgInteractFxns[wCtrlType];
    pfcnUpdate    = BW::BWDATA_GenericDlgUpdateFxns[wCtrlType];
  }
  dialog::dialog(WORD ctrlType, short index, const char *text, WORD width, WORD height)
  {
    if ( ctrlType > 14)
      ctrlType = 9;

    rct.Xmin      = 100;
    rct.Ymin      = 100;
    rct.Xmax      = rct.Xmin + width;
    rct.Ymax      = rct.Ymin + height;
    srcBits.wid   = width;
    srcBits.ht    = height;

    pszText       = (char*)text;
    lFlags        = CTRL_VISIBLE;

    wIndex        = index;
    wCtrlType     = ctrlType;

    pfcnInteract  = BW::BWDATA_GenericDlgInteractFxns[wCtrlType];
    pfcnUpdate    = BW::BWDATA_GenericDlgUpdateFxns[wCtrlType];
  }
  dialog::dialog(WORD ctrlType, short index, const char *text, WORD top, WORD left, WORD width, WORD height)
  {
    if ( ctrlType > 14)
      ctrlType = 9;

    rct.Xmin      = top;
    rct.Ymin      = left;
    rct.Xmax      = rct.Xmin + width;
    rct.Ymax      = rct.Ymin + height;
    srcBits.wid   = width;
    srcBits.ht    = height;

    pszText       = (char*)text;
    lFlags        = CTRL_VISIBLE;

    wIndex        = index;
    wCtrlType     = ctrlType;

    pfcnInteract  = BW::BWDATA_GenericDlgInteractFxns[wCtrlType];
    pfcnUpdate    = BW::BWDATA_GenericDlgUpdateFxns[wCtrlType];
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
  // --------------------- NEXT ----------------------
  dialog *dialog::next()
  {
    if ( this && this->pNext )
      return this->pNext;
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
        && this->u.list.pbStrFlags 
        && this->u.list.bSelectedIndex < this->u.list.bStrs 
        && this->u.list.pbStrFlags[this->u.list.bSelectedIndex] == 0)
      return this->u.list.pdwData[this->u.list.bSelectedIndex];
    return 0;
  }
// ------------------- SET SELECTED ------------------
  void dialog::setSelectedIndex(BYTE bIndex)
  {
    if ( this && this->isList() && bIndex < this->u.list.bStrs )
    {
      this->u.list.bCurrStr       = bIndex;
      this->u.list.bSelectedIndex = bIndex;
    }
  }
  void dialog::setSelectedByValue(DWORD dwValue)
  {
    if ( this && this->isList() && this->u.list.pbStrFlags && this->u.list.pdwData)
    {
      for (int i = 0; i < this->u.list.bStrs; i++)
      {
        if ( this->u.list.pbStrFlags[i] == 0 && this->u.list.pdwData[i] == dwValue)
        {
          this->u.list.bCurrStr       = (BYTE)i;
          this->u.list.bSelectedIndex = (BYTE)i;
        }
      }
    } // check
  }
// ------------------ GET SELECTED STR ---------------
  char *dialog::getSelectedString()
  {
    if ( this && this->isList() && this->u.list.ppStrs )
      return this->u.list.ppStrs[this->u.list.bCurrStr];
    return "";
  }

};
