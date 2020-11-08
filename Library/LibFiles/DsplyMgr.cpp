// Display Manager -- needed to isolate variables from Print Management


#include "stdafx.h"
#include "DsplyMgr.h"
#include "CScrView.h"


void DsplyMgr::OnPrepareDC(CDC* dc) {

  dev.clear();

  dev.setHorzMgns(leftMargin, rightMargin);   dev.setVertMgns(topMargin, botMargin);

  dev.prepareDisplay(font, fontSize, dc);

  vw.onPrepareOutput(false, false);
  }


// CScrView drawing,  Override

void DsplyMgr::OnDraw(CDC* pDC) {dev();   dev.clrFont();   setScrollSize();}


void DsplyMgr::setScrollSize() {
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
