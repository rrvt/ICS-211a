// Roster Report


#pragma once
#include "ReportBase.h"



class RosterRpt : public ReportBase {

public:

  RosterRpt(NotePad& notePad) : ReportBase(notePad) { }
 ~RosterRpt() { }

  void footer(Display& dev, int pageNo);

private:

  RosterRpt() : ReportBase(*(NotePad*)0) { }

  void create();
  void header();
  };

