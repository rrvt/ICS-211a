// Roster Report


#include "stdafx.h"
#include "RosterRpt.h"
#include "CScrView.h"
#include "Roster.h"


void RosterRpt::create() {
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

  np.clear();   noLines = BigNmbr;

  for (i = 0, dtm = iter(RstrIter::Rev); dtm; i++, dtm = iter--) {

    if (noLines + 1 > maxLines) {
      if (i) np << nEndPage;

      noLines = header(np, printing);

      np << nClrTabs << nSetTab(tab) << nSetTab(tab1) << nSetTab(tab2);
      np << nSetTab(tab3) << nSetTab(tab4);
      }

    np <<         dtm->callSign;
    np << nTab << dtm->firstName;
    np << nTab << dtm->lastName;
    np << nTab << dtm->id;
    np << nTab << dtm->dt.getDate() << _T("  ") << dtm->dt.getTime();

    typ = dtm->dtmType == CheckInType ? _T("ChkIn") : _T("ChkOut");   np << nTab << typ;

    np << nCrlf;  noLines++;
    }
  }


int RosterRpt::header(NotePad& np, bool printing) {

  np << nClrTabs << nSetTab(20) << nSetTab(30) << nSetTab(40);

  np << nBold << roster.incidentName << nFont;
  np << nTab << _T("Date: ") << roster.date;
  np << nTab << _T("Incident #: ") << roster.incidentNo;
  np << nRight << roster.checkInLocation << nCrlf << nCrlf;    return 2;
  }


void RosterRpt::footer(Display& dev, int pageNo) {

  dev << dBoldFont << _T("Roster ") <<  _T("Prepared By: ") << roster.preparedBy << dPrevFont;

  dev << dCenter << _T("Mission #: ") << roster.missionNo;

  ReportBase:: footer(dev, pageNo);
  }

