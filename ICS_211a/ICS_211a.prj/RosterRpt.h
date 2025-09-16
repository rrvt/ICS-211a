// Roster Report


#pragma once
#include "ReportBase.h"
#include "Printer.h"


class RosterRpt : public ReportBase {

public:

  RosterRpt(NotePad& notePad) : ReportBase(notePad, ReportID) { }
 ~RosterRpt() { }

 void onPreparePrinting(CPrintInfo* info);
 void onBeginPrinting(CScrView& vw);

  void prtHeader(DevStream& dev, int pageNo);
  void prtFooter(DevStream& dev, int pageNo);

private:

  RosterRpt() : ReportBase(*(NotePad*)0, ReportID) { }

  void getData();
  };


//  int  header(NotePad& np, bool printing);


