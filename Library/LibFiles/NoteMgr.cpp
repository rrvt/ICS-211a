// Note Manager -- Used for casual/error messages scattered throughout code


#include "stdafx.h"
#include "NoteMgr.h"
#include "CScrView.h"


void NoteMgr::OnPrepareDC(CDC* dc) {

  dev.clear();

  dev.setHorzMgns(leftMargin, rightMargin);   dev.setVertMgns(topMargin, botMargin);

  dev.prepareDisplay(font, fontSize, dc);

  vw.onPrepareOutput(true, false);
  }


// CScrView drawing,  Override

void NoteMgr::OnDraw(CDC* pDC) {dev();   dev.clrFont();   setScrollSize();}


void NoteMgr::setScrollSize() {
RECT  winSize;
int   height = dev.chHeight();
int   t      = 1;
CSize scrollViewSize;
CSize pageSize;
CSize scrollSize;

  vw.GetClientRect(&winSize);

  if (height) {t = (winSize.bottom - 1) / height; t *= height;}

  pageSize.cy = t; pageSize.cx = winSize.right;

  scrollSize.cx = dev.chWidth();   scrollSize.cy = height;

  dev.getMaxPos(scrollViewSize.cx, scrollViewSize.cy);

  vw.SetScrollSizes(MM_TEXT, scrollViewSize, pageSize, scrollSize);
  }

