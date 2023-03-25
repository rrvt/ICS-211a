// Roster Report


#pragma once
#include "ReportBase.h"
#include "Printer.h"


class RosterRpt : public ReportBase {

public:

  RosterRpt(NotePad& notePad) : ReportBase(notePad) { }
 ~RosterRpt() { }

 void onPreparePrinting(CPrintInfo* info);
 void onBeginPrinting(CScrView& vw);

  void prtHeader(DevBase& dev, int pageNo);
  void prtFooter(DevBase& dev, int pageNo);

private:

  RosterRpt() : ReportBase(*(NotePad*)0) { }

  void getData(CScrView& vw);
  };


//  int  header(NotePad& np, bool printing);


