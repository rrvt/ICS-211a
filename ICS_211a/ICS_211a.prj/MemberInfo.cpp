// Member Information


#include "stdafx.h"
#include "MemberInfo.h"
#include "NotePad.h"


Members members;


void Members::load(Archive& ar) {
Tchar  ch;
String s;
int    epos;
int    bpos;

  data.clr();   loaded = false;

  while (ar.read(ch)) {
    if (ch == _T('\n')) {
      MemberInfo& mi = data[data.end()];

      epos = s.find(_T(','));       mi.callSign    = s.substr(0,    epos);      bpos = epos + 1;
      epos = s.find(_T(','), bpos); mi.firstName   = s.substr(bpos, epos-bpos); bpos = epos + 1;
      epos = s.find(_T(','), bpos); mi.lastName    = s.substr(bpos, epos-bpos); bpos = epos + 1;
                                    mi.badgeNumber = s.substr(bpos);
      s.clear(); continue;
      }

    if (ch == _T('\r')) continue;

    s += ch;
    }

  loaded = data.end() > 0;
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

  notePad << nClrTabs << nSetTab(9) << nSetTab(tab) << nSetTab(tab1);

  for (mi = iter(); mi; mi = iter++) {
    notePad << mi->callSign << nTab << mi->firstName << nTab << mi->lastName << nTab << mi->badgeNumber;
    notePad << nCrlf;
    }
  }


MemberInfo* Members::find(String& fcc) {
MbrIter     iter(*this);
MemberInfo* mi;

  for (mi = iter(); mi; mi = iter++) if (mi->callSign == fcc) return mi;

  return 0;
  }


