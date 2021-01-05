// Font Manager
// Copyright Software Design & Engineering, Robert R. Van Tuyl, 2013.  All rights reserved.


#include "stdafx.h"
#include "FontMgr.h"
#include "MessageBox.h"



FontMgr::~FontMgr() {
CFont f;

  clear();

  if (!f.CreatePointFontIndirect(&systemFont, baseAttr.dc)) {createFailed(_T("update")); return;}

  select(f, baseAttr.dc);
  }


void FontMgr::clear()
                    {while (stkX > 0) pop();   sysFontIntlzd = false;   stk.clear();   baseAttr.clear();}


void FontMgr::initialize(TCchar* face, double fontSize, CDC* dc) {
CFont   font;

  stkX = 0;   dc->SetMapMode(MM_TEXT);

  if (!createPointFont(dc, face, fontSize, font)) {createFailed(_T("initialize")); return;}

  if (!sysFontIntlzd) {initializeSysFont(font, dc);   baseAttr.sz   = fontSize;   baseAttr.face = face;}

  FontAttr& attr = getCurAttr();   attr = baseAttr;     update(attr, _T("initialize"));
  }


void FontMgr::initializeSysFont(CFont& font, CDC* dc) {
CFont* original;
CFont  f;

  if (!dc) {deleteObject(font); return;}

  original = dc->SelectObject(&font);   if (!original) {deleteObject(font); return;}

  original->GetLogFont(&systemFont);    deleteObject(*original);

  if (f.CreateFontIndirect(&systemFont)) select(f, dc);

  baseAttr.clear();   baseAttr.dc = dc;   sysFontIntlzd = true;
  }


void FontMgr::reset() {

  if (!sysFontIntlzd) return;

  while (stkX > 0) pop();

  FontAttr& attr = getAttr(0);   attr = baseAttr;    update(attr, _T("reset"));   stkX = 0;
  }


void FontMgr::setFace(TCchar* face)
                    {FontAttr& attr = push();  attr.face      = face;  update(attr, _T("setFace"));}
void FontMgr::setSize(double size)
                    {FontAttr& attr = push();  attr.sz        = size;  update(attr, _T("setSize"));}
void FontMgr::setBold()
                    {FontAttr& attr = push();  attr.bold      = true;  update(attr, _T("setBold"));}
void FontMgr::setItalic()
                    {FontAttr& attr = push();  attr.italic    = true;  update(attr, _T("setItalic"));}
void FontMgr::setUnderLine()
                    {FontAttr& attr = push();  attr.underline = true;  update(attr, _T("setUnderLine"));}
void FontMgr::setStrikeOut()
                    {FontAttr& attr = push();  attr.strikeout = true;  update(attr, _T("setStrikeOut"));}


FontAttr& FontMgr::push() {
FontAttr& next  = getAttr(++stkX);

  next = getAttr(stkX-1);

  return next;
  }


void FontMgr::pop() {

  if (stkX <= 0) return;

  update(getAttr(--stkX), _T("pop"));
  }


void FontMgr::update(FontAttr& attr, TCchar* fn) {
CFont   font;
LOGFONT logFont;
CFont   f;

  if (!createPointFont(attr.dc, attr.face, attr.sz, font)) {createFailed(fn); return;}

  font.GetLogFont(&logFont);   deleteObject(font);

  logFont.lfWeight    = attr.bold ? FW_BOLD : FW_NORMAL;
  logFont.lfItalic    = attr.italic ? -1 : 0;
  logFont.lfUnderline = attr.underline;
  logFont.lfStrikeOut = attr.strikeout;

  if (!f.CreatePointFontIndirect(&logFont, attr.dc)) {createFailed(_T("update")); return;}

  select(f, attr.dc);
  }


bool FontMgr::createPointFont(CDC* dc, TCchar* face, double size, CFont& font) {
double ppiX = dc->GetDeviceCaps(LOGPIXELSX);
double t    = size * scale / ppiX;
int    sz   = int(t + 0.5);

  try {if (!font.CreatePointFont(sz, face, dc)) return false;} catch (...) {return false;}



  return true;
  }


void FontMgr::select(CFont& font, CDC* dc) {
CFont* x;

  if (!dc) {deleteObject(font); return;}



  x = dc->SelectObject(&font);   if (x) deleteObject(*x);
  }


void FontMgr::deleteObject(CFont& font) {


  font.DeleteObject();
  }


void FontMgr::createFailed(TCchar* fn)
                            {String s;   s.format(_T("Create Font failed in %s"), fn);   messageBox(s);}

