// Note Manager -- Used for casual/error messages scattered throughout code


#pragma once
#include "ShowMgr.h"


class CScrView;


class NoteMgr : public ShowMgr {

NotePad npd;

public:

  NoteMgr(CScrView& view) : ShowMgr(_T("Note"), view, npd) { }
 ~NoteMgr() { }

  void clear();

  void OnPrepareDC(CDC* dc);
  void startDev() {dspDev.startDev();}
  void OnDraw(CDC* pDC);
  void setScrollSize();

  void suppressOutput() {dspDev.suppressOutput();}
  void negateSuppress() {dspDev.negateSuppress();}

  void disableWrap()    {dspDev.disableWrap();}
  void enableWrap()     {dspDev.enableWrap();}

private:

  NoteMgr() : ShowMgr(_T(""), *(CScrView*)0, *(NotePad*)0) { }
  };

