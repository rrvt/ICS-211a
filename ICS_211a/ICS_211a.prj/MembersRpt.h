// Members Report


#pragma once
#include "ReportBase.h"



class MembersRpt : public ReportBase {

public:

  MembersRpt(NotePad& notePad) : ReportBase(notePad) { }
 ~MembersRpt() { }

  void footer(Display& dev, int pageNo);

private:

  MembersRpt() : ReportBase(*(NotePad*)0) { }

  void create();
  void header();
  };

