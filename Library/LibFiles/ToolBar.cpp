// Tool Bar to be used with Button and ComboBox


#include "stdafx.h"
#include "Button.h"
#include "ToolBar.h"
#include "TBComboBox.h"
#include "TBEditBox.h"



void ToolBar::setBtnCtrl(int id, TCchar* cptn, int width) {
CtrlInfo*  ci     = find(id);
bool       found  = ci != 0;
WINDOWINFO info;
RECT&      client = info.rcClient;

  GetWindowInfo(&info);

  if (!found) ci = data.allocate();

  ci->init(BtnCtrl, id, width, client.bottom - client.top, TBBS_BUTTON | TBBS_AUTOSIZE);
  ci->setCaption(cptn);

  if (!found) data = ci;
  }


void ToolBar::setCbxCtrl(int id, int width, int depth, ulong style) {
CtrlInfo* ci     = find(id);
bool      found  = ci != 0;

  if (!found) ci = data.allocate();

  style |= CBS_DROPDOWNLIST;  // | CBS_AUTOHSCROLL | WS_VSCROLL | WS_TABSTOP | WS_VISIBLE | WS_CHILD;

  ci->init(ComboBoxCtrl, id, width, depth, style);

  if (!found) data = ci;
  }


void ToolBar::setEbxCtrl(int id, int width) {
CtrlInfo* ci    = find(id);
bool      found = ci != 0;

  if (!found) ci = data.allocate();

  ci->init(EditBoxCtrl, id, width, 0, 0);

  if (!found) data = ci;
  }


void ToolBar::setMnuCtrl(int id, HMENU hMenu, TCchar* cptn) {
CtrlInfo* ci    = find(id);
bool      found = ci != 0;

  if (!found) ci = data.allocate();

  ci->init(MenuCtrl, id, 0, 0, 0);
  ci->setMenu(hMenu);
  ci->setCaption(cptn);

  if (!found) data = ci;
  }



CtrlInfo* ToolBar::find(int id) {
TBIter    iter(*this);
CtrlInfo* ci;

  for (ci = iter(); ci; ci = iter++) if (ci->id == id) return ci;

  return 0;
  }


void CtrlInfo::init(CtrlType typ, int cmdID, int wdth, int dpth, ulong styl) {  // int indx,
  ctrlType  = typ;
  id        = cmdID;
  width     = wdth;
  depth     = dpth;
  style     = styl;
  }


bool ToolBar::install() {
TBIter    iter(*this);
CtrlInfo* ci;
int       noDeleted = 0;

  for (ci = iter(); ci; ci = iter++) {
    if (!ci->install(*this)) return false;
    }

  return true;
  }


bool CtrlInfo::install(ToolBar& tb) {

  switch (ctrlType) {
    case BtnCtrl      : return installBtn(tb);
    case ComboBoxCtrl : return installCbx(tb);
    case EditBoxCtrl  : return installEbx(tb);
    case MenuCtrl     : return installMnu(tb);
    case ListCtrl     :
    default           : return false;
    }

  return true;
  }


bool CtrlInfo::installBtn(ToolBar& tb) {
int      x = GetCmdMgr()->GetCmdImage(id);
TBButton tbBtn(id, x, caption);

  tbBtn.m_bImage = tbBtn.m_bText = true;

  tbBtn.SetStyle(style);

  return tb.ReplaceButton(id, tbBtn) > 0;
  }


bool CtrlInfo::installCbx(ToolBar& tb) {
TBComboBox cbx(id, 0, style, width);

  cbx.SetDropDownHeight(depth);
  cbx.SetFlatMode(false);

  return tb.ReplaceButton(id, cbx) > 0;
  }


bool CtrlInfo::installEbx(ToolBar& tb) {
TBEditBox ebx(id, -1, ES_AUTOHSCROLL, width);

  ebx.SetFlatMode(false);

  return tb.ReplaceButton(id, ebx) > 0;
  }


bool CtrlInfo::installMnu(ToolBar& tb) {
int                   x = GetCmdMgr()->GetCmdImage(id);
CMFCToolBarMenuButton mnu(id, hMenu, x, caption, true);

  return tb.ReplaceButton(id, mnu) > 0;
  }



void CtrlInfo::copy(CtrlInfo& ci) {
  ctrlType  = ci.ctrlType;
  id        = ci.id;
  caption   = ci.caption;
  width     = ci.width;
  depth     = ci.depth;
  style     = ci.style;
  }



void ToolBar::getFont() {
LOGFONT lf;

  font.CreatePointFont(90, _T("Arial"));   font.GetLogFont(&lf);   font.DeleteObject();

  lf.lfWeight = 650;   font.CreateFontIndirect(&lf);
  }


#if 0
bool CtrlInfo::reinstallMFCBtn(ToolBar& tb) {
int     width;
TButton tBtn;

  if (!btn) return false;
  const CRect& r    = btn->Rect();
  CRect        rect = r;

  if (rect.left < tb.endPos) {
    width = rect.right - rect.left;
    rect.left = tb.endPos;  rect.right = rect.left + width;
    tBtn = *btn;
    tBtn.SetRect(rect);
    tb.RemoveButton(index);    deleted = true;
    index = tb.insertBtn(&tBtn, -1);
    }

  tb.endPos = rect.right; tb.index = index;

  return true;
  }


bool CtrlInfo::btnCreate(Button& bt, ToolBar& tb) {return bt.Create(caption, style, rect, &tb, id);}


bool CtrlInfo::cbxCreate(ComboBox& cb, ToolBar& tb) {return cb.Create(style, rect, &tb, id);}

#endif



#if 0
bool ToolBar::createCB(ComboBox& cb, int toolBarItemID, int itemID, int width, int dropHt, ulong style) {
ulong styl = CBS_AUTOHSCROLL | WS_TABSTOP | WS_VISIBLE | WS_CHILD | style;
CRect rect;
int   nIdx;

  if (::IsWindow(cb.m_hWnd)) return false;

  nIdx = CommandToIndex(toolBarItemID);    ASSERT(nIdx >= 0);

  SetButtonInfo(nIdx, itemID, TBBS_SEPARATOR, width);

  GetItemRect(nIdx, &rect);   rect.top = 1;   rect.bottom = rect.top + dropHt;

  if (endPos) {
    int offset = (nIdx - index - 1) * 24;
    rect.left = endPos + offset; rect.right = rect.left + width;
    }

  if (!cb.Create(styl, rect, this, itemID))
                                        {TRACE0("Failed to create combobox in ToolBar\n"); return false;}
  cb.SetFont(&font);

  endPos = rect.right; index = nIdx;   return true;
  }


bool ToolBar::createBtn(Button& bt, int toolBarItemID, int itemID, int width) {
ulong      style = BS_PUSHBUTTON | BS_CENTER | BS_VCENTER | BS_TEXT | WS_VISIBLE;
CRect      rect;
int        nIdx;
WINDOWINFO info;
RECT&      client = info.rcClient;

  if (::IsWindow(bt.m_hWnd)) return false;

  nIdx = CommandToIndex(toolBarItemID);    ASSERT(nIdx >= 0);

  SetButtonInfo(nIdx, itemID, TBBS_SEPARATOR, width);      // TBBS_AUTOSIZE |

  GetWindowInfo(&info);   GetItemRect(nIdx, &rect);

  rect.top = 1;    rect.bottom = 23;

  if (endPos) {
    int offset = (nIdx - index - 1) * 24;
    rect.left = endPos + offset; rect.right = rect.left + width;
    }

  if (!bt.Create(_T("Button"), style, rect, this, itemID))
                                        {TRACE0("Failed to create combobox in ToolBar\n"); return false;}
  SetFont(&font);  endPos = rect.right;  index = nIdx;   return true;
  }
#endif




#if 0
void GetButtonInfo(
int nIndex,
UINT& nID,
UINT& nStyle,
int& iImage) const;

void ToolBar::getCurBtns() {
int       i;
int       n = GetCount();
CtrlInfo* ci;
CMFCToolBarButton* btn;

  for (i = 0; i < n; i++) {
    btn = GetButton(i);   ci = data.allocate();

    ci->ctrlType = RCBtn; ci->index = i;
    ci->btn = (TButton*) btn;

    data = ci;
    }
  }
#endif



#if 0
void ToolBar::setCbxCtrl(CWnd* ct, int tbID, int ctrlIDC, int width, int depth, ulong style) {
int       index = CommandToIndex(tbID);
bool      found = true;
CtrlInfo* ci    = find(index);

  if (!ci) {found = false;    ci = data.allocate();}

  style |= CBS_DROPDOWN | CBS_AUTOHSCROLL | WS_VSCROLL | WS_TABSTOP | WS_VISIBLE | WS_CHILD;

  ci->init(ComboBoxCtrl, ct, index, ctrlIDC, width, depth, style);

  if (!found) data = ci;
  }


void ToolBar::setBtnCtrl(CWnd* ct, int tbID, int ctrlIDC, TCchar* cptn, int width) {
int        index  = CommandToIndex(tbID);
bool       found  = true;
CtrlInfo*  ci     = find(index);
WINDOWINFO info;
RECT&      client = info.rcClient;
ulong      style  = BS_PUSHBUTTON | BS_CENTER | BS_VCENTER | BS_TEXT | WS_VISIBLE;

  GetWindowInfo(&info);

  if (!ci) {found = false;    ci = data.allocate();}

  ci->init(BtnCtrl, ct, index, ctrlIDC, width, client.bottom - client.top, style);
  ci->setCaption(cptn);

  if (!found) data = ci;
  }
#endif


#if 0
//  if (!ctrl || ::IsWindow(ctrl->m_hWnd)) return false;


  tb.GetItemRect(index, &rect);   rect.top = 1;   rect.bottom = rect.top + depth;

  if (tb.endPos) {
    int offset = (index - tb.index - 1) * 24;
    rect.left = tb.endPos + offset; rect.right = rect.left + width;
    }
#endif

//  tbBtn.SetRect(rect);
//  tbBtn.SetImage(GetCmdMgr()->GetCmdImage(graphicID));

  //tbBtn.SetImage(GetCmdMgr()->GetCmdImage(id));

//  tb.SetFont(&tb.font);
//  tbBtn.SetImage(1);

