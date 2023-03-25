// Roster Report


#include "stdafx.h"
#include "RosterRpt.h"
#include "CScrView.h"
#include "Roster.h"


void RosterRpt::onPreparePrinting(CPrintInfo* info) {printer.set(prtrOrietn);}


void RosterRpt::onBeginPrinting(CScrView& vw) {printing = true;   vw.disablePrtWrap();   getPageAttr(vw);}


void RosterRpt::getData(CScrView& vw) {
int      tab;
int      tab1;
int      tab2;
int      tab3;
int      tab4;
RstrIter iter(roster);
Datum*   dtm;
String   typ;
int      i;

  tab =         roster.maxCallSign  + 2;
  tab1 = tab  + roster.maxFirstName + 1;
  tab2 = tab1 + roster.maxLastName  + 1;
  tab3 = tab2 + roster.maxID        + 1;
  tab4 = tab3 + 15;

  np.clear();
  np << nClrTabs << nSetTab(tab) << nSetTab(tab1) << nSetTab(tab2) << nSetTab(tab3) << nSetTab(tab4);

  for (i = 0, dtm = iter(RstrIter::Rev); dtm; i++, dtm = iter--) {

    np <<         dtm->callSign;
    np << nTab << dtm->firstName;
    np << nTab << dtm->lastName;
    np << nTab << dtm->id;
    np << nTab << dtm->dt.getDate() << _T("  ") << dtm->dt.getTime();

    typ = dtm->dtmType == CheckInType ? _T("ChkIn") : _T("ChkOut");   np << nTab << typ;

    np << nCrlf;
    }
  }


void RosterRpt::prtHeader(DevBase& dev, int pageNo) {

  dev << dClrTabs << dSetTab(20) << dSetTab(30) << dSetTab(40);

  dev << dBold << roster.incidentName << dFont;
  dev << dTab << _T("Date: ") << roster.date;
  dev << dTab << _T("Incident #: ") << roster.incidentNo;
  dev << dRight << roster.checkInLocation << dCrlf << dCrlf;
  }


void RosterRpt::prtFooter(DevBase& dev, int pageNo) {

  dev << dBold << _T("Roster ") <<  _T("Prepared By: ") << roster.preparedBy << dFont;

  dev << dCenter << _T("Mission #: ") << roster.missionNo;

  ReportBase::prtFooter(dev, pageNo);
  }

