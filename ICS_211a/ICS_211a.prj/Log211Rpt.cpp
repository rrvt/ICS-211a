// Log211 Report


#include "stdafx.h"
#include "Log211Rpt.h"
#include "CScrView.h"
#include "Log211.h"
#include "Roster.h"


void Log211Rpt::display(CScrView& vw)
                    {if (log211.prepare()) {vw.disableWrap();   ReportBase::display(vw);   dspFtr();}}


void Log211Rpt::print(CScrView& vw) {if (log211.prepare()) {vw.disableWrap();  ReportBase::print(vw);}}


void Log211Rpt::create() {
LogIter   iter(log211);
LogDatum* dsc;
int       i;
int       n;

  noLines = BigNmbr;   np.clear();

  for (i = 0, dsc = iter(); dsc; i++, dsc = iter++) {

    n = iter.isLast() ? 3 : 1;

    if (noLines + n > maxLines) {if (i) np << nEndPage;   header();}

    setBodyTabs();   noLines += dsc->report(np);
    }

  String s;  s.format(_T("%0.3f"), log211.getTotalHrs());

  setBodyTabs();  np << nCrlf;

  np << nTab << nTab << nTab << nTab << _T("Total Hours:") << nTab << s << nCrlf;
  }


void Log211Rpt::setBodyTabs() {
int tab0 =        20;
int tab1 = tab0 + 11;
int tab2 = tab1 + 20;
int tab3 = tab2 + 14;
int tab4 = tab3 +  7;
int tab5 = tab4 +  2;
int tab6 = tab5 + 15;

  np << nClrTabs << nSetTab(tab0) << nSetTab(tab1) << nSetRTab(tab2) << nSetRTab(tab3);
  np << nSetRTab(tab4) << nSetTab(tab5) << nSetTab(tab6);
  }


void Log211Rpt::header() {
int tab0 =        25;
int tab1 = tab0 + 30;
int tab2 = tab1 +  8;
int tab3 = tab2 +  8;

  np << nClrTabs << nSetTab(tab0) << nSetTab(tab1) << nSetTab(tab2) << nSetTab(tab3);

  np << nBold  << _T("ICS_211a CHECK IN LIST") << nFont;
  np << nTab << nFSize(90) << _T("1. Incident Name:");
  np << nTab << _T("2. Date:") << nTab << _T("3. Incident Number:");
  np << nRight << _T("4. Check In Location:") << nFont << nCrlf;

  np << nBold  << _T("San Jose ARES/RACES/ACS") << nFont;
  np << nTab   << roster.incidentName;
  np << nTab << roster.date;
  np << nTab   << roster.incidentNo;
  np << nRight << roster.checkInLocation << nCrlf << nCrlf;

  setBodyTabs();

  np << nBeginLine;
  np << _T("Name") << nTab << _T("Agency") << nTab << _T("Call Sign");
  np << nTab << _T("Time In") << nTab<< _T("Time Out") << nTab << _T("Hours");
  np << nTab << _T("Remarks") << nRight << nEndLine << nCrlf;

  np << nClrTabs << nSetTab(6) << nSetRTab(17) << nSetTab(18);
  np << nFSize(90) << _T("Data") << nFont << nCrlf;

  noLines = 5;
  }




void Log211Rpt::footer(Display& dev, int pageN) {

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


void Log211Rpt::dspFtr() {

  np << nCrlf;

  np << nClrTabs << nSetTab(9);
  np << nBold << _T("ICS 211a") << nFont;
  np << nFSize(90) << nTab << _T("6.Number of Pages");
  np << nCenter << _T("7. Prepared By");
  np << nRight << _T("8. Mission Number") << nFont << nCrlf;

  np << nBold << _T("SJ RACES") << nFont;
  np << nTab << _T("1") << _T(" of ") << _T("1");
  np << nCenter << roster.preparedBy << nRight << roster.missionNo << nCrlf;
  }

