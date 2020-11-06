// Member Information


#include "stdafx.h"
#include "Members.h"
#include "ICS_211aView.h"


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



