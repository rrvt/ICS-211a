// Display Manager -- needed to isolate variables from Print Management


#pragma once
#include "ShowMgr.h"


class CScrView;


class DsplyMgr : public ShowMgr {

NotePad npd;

public:

  DsplyMgr(CScrView& view) : ShowMgr(view, npd) { }
 ~DsplyMgr() { }

  void clear();

  void OnPrepareDC(CDC* dc);
  void startDev() {dev.startDev();}
  void OnDraw(CDC* pDC);
  void setScrollSize();

private:
  DsplyMgr() : ShowMgr(*(CScrView*)0, *(NotePad*)0) { }
  };

