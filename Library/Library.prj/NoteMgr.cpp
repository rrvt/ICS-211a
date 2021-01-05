// Note Manager -- Used for casual/error messages scattered throughout code


#include "stdafx.h"
#include "NoteMgr.h"
#include "CScrView.h"


void NoteMgr::OnPrepareDC(CDC* dc) {

  dspDev.clear();

  dspDev.setHorzMgns(leftMargin, rightMargin);   dspDev.setVertMgns(topMargin, botMargin);

  dspDev.prepareDisplay(font, fontSize, dc);

  vw.onPrepareOutput(true, false);
  }


// CScrView drawing,  Override

void NoteMgr::OnDraw(CDC* pDC) {dspDev();   dspDev.clrFont();   setScrollSize();}


void NoteMgr::setScrollSize() {
RECT  winSize;
int   height = dspDev.chHeight();
int   t      = 1;
CSize scrollViewSize;
CSize pageSize;
CSize scrollSize;

  vw.GetClientRect(&winSize);

  if (height) {t = (winSize.bottom - 1) / height; t *= height;}

  pageSize.cy = t; pageSize.cx = winSize.right;

  scrollSize.cx = dspDev.chWidth();   scrollSize.cy = height;

  dspDev.getMaxPos(scrollViewSize.cx, scrollViewSize.cy);

  vw.SetScrollSizes(MM_TEXT, scrollViewSize, pageSize, scrollSize);
  }

