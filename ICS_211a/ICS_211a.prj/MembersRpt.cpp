// Members Report


#include "stdafx.h"
#include "MembersRpt.h"
#include "CScrView.h"
#include "Members.h"


void MembersRpt::create() {
MbrIter     iter(members);
MemberInfo* mi;
int         firstLng;
int         lastLng;
int         maxFirst = 0;
int         maxLast  = 0;
int         tab;
int         tab1;
int         i;

  for (mi = iter(); mi; mi = iter++) {
    firstLng = mi->firstName.length();
    lastLng  = mi->lastName.length();

    if (firstLng > maxFirst) maxFirst = firstLng;
    if (lastLng  > maxLast)  maxLast  = lastLng;
    }

  maxFirst = (maxFirst * 85) / 100;
  maxLast  = (maxLast  * 85) / 100;

  tab  =   9 + maxFirst;
  tab1 = tab + maxLast;

  np.clear();  noLines = BigNmbr;

  for (i = 0, mi = iter(); mi; i++, mi = iter++) {

    if (noLines + 1 > maxLines) {
      if (i) np << nEndPage;

      np << nClrTabs << nSetTab(9) << nSetTab(tab) << nSetTab(tab1);   noLines = header(np, printing);
      }

    np << mi->callSign << nTab << mi->firstName << nTab << mi->lastName << nTab << mi->badgeNumber;
    np << nCrlf;  noLines++;
    }
  }


int MembersRpt::header(NotePad& np, bool printing) {
  np << _T("CallSign") << nTab << _T("First Name") << nTab;
  np << _T("Last Name") << nTab << _T("Badge No.") << nCrlf << nCrlf;  return 2;
  }


void MembersRpt::footer(Device& dev, int pageNo)
                {dev << dBoldFont << _T("Member Info") << dPrevFont;   ReportBase::footer(dev, pageNo);}

