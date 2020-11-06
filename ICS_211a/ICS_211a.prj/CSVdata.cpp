// CSV Data


#include "stdafx.h"
#include "CSVdata.h"


CSVRcds csvRcds;


CSVRcds::CSVRcds() : nHdr(-1),        maxLastName(0),  maxFirstName(0),  maxFcc(0),
                     maxStatus(0),    maxActive(0),    maxIS_100(0),     maxIS_200(0),
                     maxIS_700(0),    maxIS_800(0),    maxSems(0),       maxAll(0),
                     maxActive1(0) { }

int CSVRcds::nHeader() {
CSVIter iter(*this);
CSVRcd* rcd;

  if (nHdr >= 0) return nHdr;

  for (nHdr = 0, rcd = iter(); rcd; nHdr++, rcd = iter++)
                                                      if (rcd->lastName == _T("Last Name")) return nHdr;
  nHdr = -1; return nHdr;
  }



void CSVRcds::getMaxs() {
CSVIter iter(*this);
CSVRcd* rcd;
int     i;
int     nHdr = nHeader();

  for (i = 0, rcd = iter(); rcd; i++, rcd = iter++) {

    if (i < nHdr) continue;

    getMax(rcd->lastName,  maxLastName);
    getMax(rcd->firstName, maxFirstName);
    getMax(rcd->fcc,       maxFcc);
    getMax(rcd->status,    maxStatus);
    getMax(rcd->active,    maxActive);
    getMax(rcd->is_100,    maxIS_100);
    getMax(rcd->is_200,    maxIS_200);
    getMax(rcd->is_700,    maxIS_700);
    getMax(rcd->is_800,    maxIS_800);
    getMax(rcd->sems,      maxSems);
    getMax(rcd->all,       maxAll);
    getMax(rcd->active1,   maxActive1);
    }
  }


void CSVRcds::getMax(String& s, int& max) {int lng = s.length();   if (lng > max) max = lng;}


void CSVRcds::setTabs() {
int tab01 =         maxLastName  + 1;
int tab02 = tab01 + maxFirstName + 1;
int tab03 = tab02 + maxFcc       + 1;
int tab04 = tab03 + maxStatus    + 1;
int tab05 = tab04 + maxActive    + 1;
int tab06 = tab05 + maxIS_100    + 1;
int tab07 = tab06 + maxIS_200    + 1;
int tab08 = tab07 + maxIS_700    + 1;
int tab09 = tab08 + maxIS_800    + 1;
int tab10 = tab09 + maxSems      + 1;
int tab11 = tab10 + maxAll       + 1;

  notePad << nClrTabs << nSetTab(tab01) << nSetTab(tab02) << nSetTab(tab03) << nSetTab(tab04);
  notePad << nSetTab(tab05) << nSetTab(tab06) << nSetTab(tab07) << nSetTab(tab08) << nSetTab(tab09);
  notePad << nSetTab(tab10) << nSetTab(tab11);
  }





void CSVRcd::put(TCchar* p) {
String s = p;

  switch (putI) {
    case  0: break;
    case  1: lastName  = s; break;
    case  2: firstName = s; break;
    case  3: fcc       = s; break;
    case  4: status    = s; break;
    case  5: active    = s; break;
    case  6: is_100    = s; break;
    case  7: is_200    = s; break;
    case  8: is_700    = s; break;
    case  9: is_800    = s; break;
    case 10: sems      = s; break;
    case 11: all       = s; break;
    case 12: active1   = s; break;
    }
  }


int CSVRcd::display() {

  csvRcds.setTabs();

  notePad << lastName  << nTab;
  notePad << firstName << nTab;
  notePad << fcc       << nTab;
  notePad << status    << nTab;
  notePad << active    << nTab;
  notePad << is_100    << nTab;
  notePad << is_200    << nTab;
  notePad << is_700    << nTab;
  notePad << is_800    << nTab;
  notePad << sems      << nTab;
  notePad << all       << nCrlf;
//  notePad << active1   ;
  return 1;
  }


void CSVRcd::copy(CSVRcd &r) {
  key       = r.key;
  lastName  = r.lastName;
  firstName = r.firstName;
  fcc       = r.fcc;
  status    = r.status;
  active    = r.active;
  is_100    = r.is_100;
  is_200    = r.is_200;
  is_700    = r.is_700;
  is_800    = r.is_800;
  sems      = r.sems;
  all       = r.all;
  active1   = r.active1;
  }

