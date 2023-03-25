// Members Report


#pragma once
#include "ReportBase.h"
#include "printer.h"



class MembersRpt : public ReportBase {

int        tab;
int        tab1;
int        tab2;

public:

  MembersRpt(NotePad& notePad) : ReportBase(notePad), tab(0), tab1(0), tab2(0) { }
 ~MembersRpt() { }

  void dspHeader(DevBase& dev, int pageNo = 1) { }
  void dspFooter(DevBase& dev, int pageNo = 1) { }

  void display(CScrView& vw);

  void onPreparePrinting(CPrintInfo* info);
  void onBeginPrinting(CScrView& vw);

  void prtHeader(DevBase& dev, int pageNo);
  void prtFooter(DevBase& dev, int pageNo);

private:

  void getTabs();

  MembersRpt() : ReportBase(*(NotePad*)0) { }

  void getData(CScrView& vw);
  };




//  int  header(NotePad& np, bool printing);

