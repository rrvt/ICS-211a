// Roster Report


#pragma once
#include "ReportBase.h"



class RosterRpt : public ReportBase {

public:

  RosterRpt(NotePad& notePad) : ReportBase(notePad) { }
 ~RosterRpt() { }

  void footer(Device& dev, int pageNo);

private:

  RosterRpt() : ReportBase(*(NotePad*)0) { }

  void create(CScrView& vw);
  int  header(NotePad& np, bool printing);
  };

