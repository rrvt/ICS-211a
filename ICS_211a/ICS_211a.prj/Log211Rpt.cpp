// Log211 Report


#include "stdafx.h"
#include "Log211Rpt.h"
#include "CScrView.h"
#include "Log211.h"
#include "Roster.h"
#include "Utilities.h"


void Log211Rpt::display(CScrView& vw)
                             {if (log211.prepare()) {setTabs();   ReportBase::display(vw);}  dspFooter();}


void Log211Rpt::onPreparePrinting(CPrintInfo* info) {printer.set(prtrOrietn);}


void Log211Rpt::onBeginPrinting(CScrView& vw)
                              {if (log211.prepare()) {setTabs();   vw.disablePrtWrap(); getPageAttr(vw);}}


void Log211Rpt::getData(CScrView& vw) {
LogIter   iter(log211);
LogDatum* dsc;
int       i;

  np.clear();

  np << nClrTabs << nSetTab(tab0) << nSetTab(tab1) << nSetRTab(tab2) << nSetRTab(tab3);
  np << nSetRTab(tab4) << nSetRTab(tab5) << nSetTab(tab6);

  np << nSetRTab(tab7) << nSetRTab(tab8);

  for (i = 0, dsc = iter(); dsc; i++, dsc = iter++) {dsc->report(np, dspDate);}

  np << nCrlf;

  String s;  s.format(_T("%0.3f"), getTotalHours());

  np << nTab << nTab << nTab << nTab << _T("Total Hours:") << nTab << s << nCrlf;
  }


void Log211Rpt::prtHeader(DevBase& dev, int pageN) {
int tb0 =       30;
int tb1 = tb0 + 30;
int tb2 = tb1 +  8;
int tb3 = tb2 +  8;

  dev << dClrTabs << dSetTab(tb0) << dSetTab(tb1) << dSetTab(tb2) << dSetTab(tb3);

  dev << dBold  << _T("ICS_211a CHECK IN LIST") << dFont;
  dev << dTab;
  dev << dFSize(9.0) << _T("1. Incident Name:");
  dev << dTab        << _T("2. Date:");
  dev << dTab        << _T("3. Incident Number:");
  dev << dRight      << _T("4. Check In Location:") << dFont << dCrlf;

  dev << dBold  << _T("San Jose ARES/RACES/ACS") << dFont;
  dev << dTab   << roster.incidentName;
  dev << dTab   << roster.date;
  dev << dTab   << roster.incidentNo;
  dev << dRight << roster.checkInLocation << dCrlf << dCrlf;

  dev << dClrTabs << dSetTab(tab0) << dSetTab(tab1) << dSetRTab(tab2) << dSetRTab(tab3);
  dev << dSetRTab(tab4) << dSetRTab(tab5) << dSetTab(tab6);

  np << nSetRTab(tab7) << nSetRTab(tab8);

  dev << dBeginLine;
  dev << _T("Name") << dTab << _T("  Agency") << dTab << _T("Call Sign");
  dev << dTab << _T("Time In     ") << dTab<< _T("Time Out    ") << dTab << _T("Hours");
  dev << dTab << _T("ID ") << dTab << _T("Remarks           ") << dRight << dEndLine << dCrlf;
  }


void Log211Rpt::setTabs() {
LogIter   iter(log211);
LogDatum* dsc;

  dspDate = log211.includeDate();

  clrMaximums();

  for (dsc = iter(); dsc; dsc = iter++) {getMaximums(*dsc, dspDate);}

  tab0 = (maxfirstNameLng + maxlastNameLng) * 9 / 10;
  tab1 = tab0 + maxagencyLng;
  tab2 = tab1 + (maxcallSignLng + maxtimeInLng);
  tab3 = tab2 + (maxtimeOutLng) * 9 / 10;
  tab4 = tab3 + (maxhoursLng) * 9 / 10;
  tab5 = tab4 + maxidLng + 3;
  tab6 = tab5 + 2;
  tab7 = tab6 + 10;
  tab8 = tab7 + 10;
  }


void Log211Rpt::clrMaximums() {
  maxfirstNameLng = 0;
  maxlastNameLng  = 0;
  maxagencyLng    = 0;
  maxcallSignLng  = 0;
  maxtimeInLng    = 0;
  maxtimeOutLng   = 0;
  maxhoursLng     = 0;
  maxidLng        = 0;
  }


void Log211Rpt::getMaximums(LogDatum& lgdtm, bool dspDate) {
String s;
  maxLng(lgdtm.firstName, maxfirstNameLng);
  maxLng(lgdtm.lastName,  maxlastNameLng);
  maxLng(lgdtm.agency,    maxagencyLng);
  maxLng(lgdtm.callSign,  maxcallSignLng);

  s = lgdtm.timeIn;   if (dspDate) s += lgdtm.dateIn;   maxLng(s, maxtimeInLng);
  s = lgdtm.timeOut;  if (dspDate) s += lgdtm.dateOut;  maxLng(s, maxtimeOutLng);

  maxLng(lgdtm.hrs,       maxhoursLng);
  maxLng(lgdtm.id,        maxidLng);
  }


double Log211Rpt::getTotalHours() {
LogIter   iter(log211);
LogDatum* dsc;
double    total = 0;

  for (dsc = iter(); dsc; dsc = iter++) total += dsc->hours;

  return total;
  }


void Log211Rpt::prtFooter(DevBase& dev, int pageN) {

  if (pageN > maxPages) maxPages = pageN;

  dev << dClrTabs << dSetTab(9);
  dev << dBold << _T("ICS 211a") << dFont;
  dev << dFSize(9.0) << dTab << _T("6.Number of Pages");
  dev << dCenter << _T("7. Prepared By");
  dev << dRight << _T("8. Mission Number") << dFont << dCrlf;

  dev << dBold << _T("SJ RACES") << dFont;
  dev << dTab << pageN << _T(" of ") << maxPages;
  dev << dCenter << roster.preparedBy << dRight << roster.missionNo << dFlushFtr;
  }


void Log211Rpt::dspFooter() {

  np << nCrlf;

  np << nClrTabs << nSetTab(9);
  np << nBold << _T("ICS 211a") << nFont;
  np << nFSize(9.0) << nTab << _T("6.Number of Pages");
  np << nCenter << _T("7. Prepared By");
  np << nRight << _T("8. Mission Number") << nFont << nCrlf;

  np << nBold << _T("SJ RACES") << nFont;
  np << nTab << _T("1") << _T(" of ") << _T("1");
  np << nCenter << roster.preparedBy << nRight << roster.missionNo << nCrlf;
  }

