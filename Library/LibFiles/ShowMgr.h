// Show Manager -- Parent class for DsplyMgr and PrintMgr


#pragma once
#include "DisplayDev.h"

class CScrView;


class ShowMgr {

protected:

CScrView&   vw;

NotePad&    npd;
DisplayDev  dev;

String      font;
int         fontSize;                 // in 10 * Points

double      leftMargin;
double      topMargin;
double      rightMargin;
double      botMargin;

public:

  ShowMgr(CScrView& view, NotePad& notePad);
 ~ShowMgr() { }

  void     setFont(TCchar* f, int points = 120) {font = f; fontSize = points;}

  void     setMgns(double left, double top, double right, double bot);

  NotePad& getNotePad() {return npd;}
  Display& getDev()     {return dev.getDisplay();}

private:

  ShowMgr() : npd(*(NotePad*)0), dev(*(NotePad*)0), vw(*(CScrView*)0) { }
  };

