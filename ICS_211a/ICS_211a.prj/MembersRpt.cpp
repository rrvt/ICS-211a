// Members Report


#include "stdafx.h"
#include "MembersRpt.h"
#include "CScrView.h"
#include "Members.h"


void MembersRpt::display(CScrView& vw)
                                    {printing = false;   getTabs();   vw.disableDplWrap();   getData(vw);}


void MembersRpt::onPreparePrinting(CPrintInfo* info) {printer.set(prtrOrietn);}


void MembersRpt::onBeginPrinting(CScrView& vw)
                                {printing = true;   getTabs();   vw.disablePrtWrap();   getPageAttr(vw);}


void MembersRpt::getTabs() {
MbrIter     iter(members);
MemberInfo* mi;
int         firstLng;
int         lastLng;
int         maxFirst = 0;
int         maxLast  = 0;

  for (mi = iter(); mi; mi = iter++) {
    firstLng = mi->firstName.length();
    lastLng  = mi->lastName.length();

    if (firstLng > maxFirst) maxFirst = firstLng;
    if (lastLng  > maxLast)  maxLast  = lastLng;
    }

  maxFirst = (maxFirst * 85) / 100;
  maxLast  = (maxLast  * 85) / 100;

  tab  = 9;
  tab1 = tab  + maxFirst;
  tab2 = tab1 + maxLast + 4;
  }


void MembersRpt::getData(CScrView& vw) {
MbrIter     iter(members);
MemberInfo* mi;
int         i;

  np.clear();    np << nClrTabs << nSetTab(tab) << nSetTab(tab1) << nSetRTab(tab2);

  for (i = 0, mi = iter(); mi; i++, mi = iter++) {

    np << mi->callSign << nTab << mi->firstName << nTab << mi->lastName << nTab << mi->badgeNumber;
    np << nCrlf;
    }
  }


void MembersRpt::prtHeader(DevBase& dev, int pageNo) {

  dev << dClrTabs << dSetTab(tab) << dSetTab(tab1) << dSetRTab(tab2);

  dev << _T("CallSign") << dTab << _T("First Name") << dTab;
  dev << _T("Last Name") << dTab << _T("Badge No.") << dCrlf << dCrlf;
  }


void MembersRpt::prtFooter(DevBase& dev, int pageNo)
                      {dev << dBold << _T("Member Info") << dFont;   ReportBase::prtFooter(dev, pageNo);}


