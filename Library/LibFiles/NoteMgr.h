// Note Manager -- Used for casual/error messages scattered throughout code


#pragma once
#include "ShowMgr.h"


class CScrView;


class NoteMgr : public ShowMgr {
public:

  NoteMgr(CScrView& view) : ShowMgr(view, notePad) { }
 ~NoteMgr() { }

  void clear();

  bool isEmpty() {return notePad.isEmpty();}

  void OnPrepareDC(CDC* dc);
  void startDev() {dev.startDev();}
  void OnDraw(CDC* pDC);
  void setScrollSize();

private:

  NoteMgr() : ShowMgr(*(CScrView*)0, *(NotePad*)0) { }
  };

