// Log211 Report


#pragma once
#include "ReportBase.h"


class Log211Rpt : public ReportBase {

public:

  Log211Rpt(NotePad& notePad) : ReportBase(notePad) { }
 ~Log211Rpt() { }

 void    display(CScrView& vw);
 void    print(CScrView& vw);
 void    footer(Device& dev, int pageN);

private:

  Log211Rpt() : ReportBase(*(NotePad*)0) { }

  void   create(CScrView& vw);
  void   setBodyTabs();
  int    header(NotePad& np, bool printing);
  void   dspFtr();
  };

