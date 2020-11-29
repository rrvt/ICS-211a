// Display Manager -- needed to isolate variables from Print Management


#pragma once
#include "ShowMgr.h"


class CScrView;


class DsplyMgr : public ShowMgr {

NotePad npd;

bool    wrapEnabled;

public:

  DsplyMgr(CScrView& view) : ShowMgr(view, npd) { }
 ~DsplyMgr() { }

  void OnPrepareDC(CDC* dc);
  void startDev() {dev.startDev();}
  void OnDraw(CDC* pDC);
  void setScrollSize();

  void suppressOutput() {dev.suppressOutput();}
  void negateSuppress() {dev.negateSuppress();}

  void disableWrap()    {wrapEnabled = false;}
  void enableWrap()     {wrapEnabled = true;}

private:
  DsplyMgr() : ShowMgr(*(CScrView*)0, *(NotePad*)0) { }
  };

