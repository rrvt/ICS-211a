// Log211 Report


#pragma once
#include "ReportBase.h"
#include "Printer.h"

struct LogDatum;

class Log211Rpt : public ReportBase {

int  maxfirstNameLng;
int  maxlastNameLng;
int  maxagencyLng;
int  maxcallSignLng;
int  maxtimeInLng;
int  maxtimeOutLng;
int  maxhoursLng;
int  maxidLng;


bool dspDate;

int  tab0;
int  tab1;
int  tab2;
int  tab3;
int  tab4;
int  tab5;
int  tab6;
int  tab7;
int  tab8;

public:
         Log211Rpt(NotePad& notePad) : ReportBase(notePad), dspDate(false) { }
        ~Log211Rpt() { }

  void   display(CScrView& vw);

  void   onPreparePrinting(CPrintInfo* info);
  void   onBeginPrinting(CScrView& vw);

  void   dspFooter();

  void   prtHeader(DevBase& dev, int pageNo);
  void   prtFooter(DevBase& dev, int pageN);

private:

  void   getData(CScrView& vw);
  void   setTabs();
  void   clrMaximums();
  void   getMaximums(LogDatum& ssd, bool dspDate);

  double getTotalHours();

  Log211Rpt() : ReportBase(*(NotePad*)0) { }
  };

