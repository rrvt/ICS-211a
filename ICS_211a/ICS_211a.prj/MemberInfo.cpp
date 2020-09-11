// Member Information


#include "stdafx.h"
#include "MemberInfo.h"
#include "NotePad.h"


Members members;


void    MemberInfo::put(TCchar* p) {

  switch (putI) {
    case 0: callSign    = p; break;
    case 1: firstName   = p; break;
    case 2: lastName    = p; break;
    case 3: badgeNumber = p; break;
    }
  }



String* MemberInfo::get() {

  switch (putI) {
    case 0: return &callSign;
    case 1: return &firstName;
    case 2: return &lastName;
    case 3: return &badgeNumber;
    }
  return 0;
  }



MemberInfo* Members::find(String& callSign) {
MbrIter     iter(*this);
MemberInfo* mi;

  for (mi = iter(); mi; mi = iter++) if (mi->callSign == callSign) return mi;

  return 0;
  }




void Members::display() {
MbrIter     iter(*this);
MemberInfo* mi;
int         firstLng;
int         lastLng;
int         maxFirst = 0;
int         maxLast  = 0;
int         tab;
int         tab1;

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

  notePad.clear();   notePad << nClrTabs << nSetTab(9) << nSetTab(tab) << nSetTab(tab1);

  for (mi = iter(); mi; mi = iter++) {
    notePad << mi->callSign << nTab << mi->firstName << nTab << mi->lastName << nTab << mi->badgeNumber;
    notePad << nCrlf;
    }
  }


