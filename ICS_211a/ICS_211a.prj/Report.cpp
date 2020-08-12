// Report -- Essentially the output processing is separated from the file input/output processing


#include "stdafx.h"
#include "Report.h"
#include "ICS_211a.h"
#include "ICS_211aDoc.h"
#include "Log211.h"
#include "Roster.h"
//#include "SprdSheet.h"
#include "Utilities.h"


Report report;


void Report::operator() (OutputFn fn) {
ICS_211aView& vw  = *view();

  printing = false;

  if (fn == DisplayFn) {doc()->display(); return;}
  if (fn == PrintingFn) printing = true;

  log211.prepare();   totalHrs = log211.getTotalHrs();

  if (printing) detNoLines(vw);

//  detWraps(vw);

  vw.disableWrap();

  if (printing) detNoPages(vw);

  create();
  }


void Report::detNoLines(ICS_211aView& vw) {
int i;
int mxPgs;
int noLns = 0;

  vw.suppressOutput();   maxLines = 999999;

  notePad.clear();

  noLns = headerLines();

  for (i = noLns; i < 65; i++) {
    String s; s.format(_T("Line %i"), i);
    notePad  << s << nCrlf;
    }

  vw.trialRun(maxLines, mxPgs);
  }


void Report::detWraps(ICS_211aView& vw) {
Display& dev = vw.getDev();
CDC*     dc  = dev.getDC();
//DSIter   iter(dataStore);
//Data*    datum;

//  for (datum = iter(); datum; datum = iter++) datum->wrap(dev, dc);
  }


void Report::detNoPages(ICS_211aView& vw) {int nLns;   create();   vw.trialRun(nLns, maxPages);}


void Report::create() {
int       ttlLines;
String    t;

//  if (dataStore.isEmpty()) return;

  notePad.clear();   ttlLines = header();

  ttlLines += body();

  if (printing) notePad << nEndPage;
  else          dspFtr();
  }


int Report::header() {

  noLines = headerLines();

  notePad << nClrTabs << nSetTab(6) << nSetRTab(17) << nSetTab(18);
  notePad << nFSize(90) << _T("Data") << nFont;   crlf();

  return noLines;
  }


int Report::headerLines() {
int tab0 =        25;
int tab1 = tab0 + 30;
int tab2 = tab1 +  8;
int tab3 = tab2 +  8;

  notePad << nClrTabs << nSetTab(tab0) << nSetTab(tab1) << nSetTab(tab2) << nSetTab(tab3);

  notePad << nBold  << _T("ICS_211a CHECK IN LIST") << nFont;
  notePad << nTab << nFSize(90) << _T("1. Incident Name:");
  notePad << nTab << _T("2. Date:") << nTab << _T("3. Incident Number:");
  notePad << nRight << _T("4. Check In Location:") << nFont;   crlf();

  notePad << nBold  << _T("San Jose ARES/RACES/ACS") << nFont;
  notePad << nTab   << roster.incidentName;
  notePad << nTab << roster.date;
  notePad << nTab   << roster.incidentNo;
  notePad << nRight << roster.checkInLocation;
  crlf();   crlf();

  setBodyTabs();

  notePad << nBeginLine;
  notePad << _T("Name") << nTab << _T("Agency") << nTab << _T("Call Sign");
  notePad << nTab << _T("Time In") << nTab<< _T("Time Out") << nTab << _T("Hours");
  notePad << nTab << _T("Remarks") << nRight << nEndLine << nCrlf;

  return 4;
  }



// Output report body to NotePad

int Report::body() {
LogIter   iter(log211);
LogDatum* dsc;
int       nLns = 0;
int       n;

  setBodyTabs();

  for (dsc = iter(); dsc; dsc = iter++) {

    n = 1;

    if (iter.isLast()) n += 2;

    if (printing && n + noLines > maxLines) {notePad << nEndPage;    nLns += header();}

    noLines += dsc->report();  nLns += n;
    }

  String s;  s.format(_T("%0.2f"), totalHrs);

  notePad << nCrlf;
  notePad << nTab << nTab << nTab << nTab << _T("Total Hours:") << nTab << s << nCrlf;

  return nLns;
  }


void Report::setBodyTabs() {
int tab0 =        20;
int tab1 = tab0 + 11;
int tab2 = tab1 + 19;
int tab3 = tab2 + 13;
int tab4 = tab3 +  6;
int tab5 = tab4 +  2;
int tab6 = tab5 + 15;

  notePad << nClrTabs << nSetTab(tab0) << nSetTab(tab1) << nSetRTab(tab2) << nSetRTab(tab3);
  notePad << nSetRTab(tab4) << nSetTab(tab5) << nSetTab(tab6);
  }


void Report::footer(Display& dev, int pageN) {

  if (pageN > maxPages) maxPages = pageN;

  dev << dClrTabs << dSetTab(9);
  dev << dBoldFont << _T("ICS 211a") << dPrevFont;
  dev << dFSize(90) << dTab << _T("6.Number of Pages");
  dev << dCenter << _T("7. Prepared By");
  dev << dRight << _T("8. Mission Number") << dPrevFont << dCrlf;

  dev << dBoldFont << _T("SJ RACES") << dPrevFont;
  dev << dTab << pageN << _T(" of ") << maxPages;
  dev << dCenter << roster.preparedBy << dRight << roster.missionNo << dflushFtr;
  }


void Report::dspFtr() {

  notePad << nCrlf;

  notePad << nClrTabs << nSetTab(9);
  notePad << nBold << _T("ICS 211a") << nFont;
  notePad << nFSize(90) << nTab << _T("6.Number of Pages");
  notePad << nCenter << _T("7. Prepared By");
  notePad << nRight << _T("8. Mission Number") << nFont << nCrlf;

  notePad << nBold << _T("SJ RACES") << nFont;
  notePad << nTab << _T("1") << _T(" of ") << _T("1");
  notePad << nCenter << roster.preparedBy << nRight << roster.missionNo << nCrlf;
  }

