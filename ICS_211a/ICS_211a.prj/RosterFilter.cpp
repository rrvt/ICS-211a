// Roster Filter used for determining check in and check out


#include "stdafx.h"
#include "RosterFilter.h"


void RosterFilter::add(Datum& dtm) {
RFIter  iter(*this);
IDinfo* idInfo;

  for (idInfo = iter(); idInfo; idInfo = iter++) if (*idInfo == dtm)
                                 {idInfo->chkOut = dtm.dtmType == CheckOutType ? true : false;   return;}

  IDinfo& info = filter.nextData();   info = dtm;   getMax(info);
  }


void RosterFilter::getMax(IDinfo& info) {
int lng;

  lng = info.callSign.length();   if (lng > maxCallSign) maxCallSign = lng;
  lng = info.firstName.length();  if (lng > maxFirst)    maxFirst    = lng;
  lng = info.lastName.length();   if (lng > maxLast)     maxLast     = lng;
  }



bool IDinfo::operator== (Datum& dtm) {
  if (!callSign.isEmpty() && !dtm.callSign.isEmpty()) return callSign == dtm.callSign;

  return firstName == dtm.firstName && lastName == dtm.lastName;
  }


IDinfo& IDinfo::operator= (Datum& dtm) {

  callSign  = dtm.callSign;
  firstName = dtm.firstName;
  lastName  = dtm.lastName;
  id        = dtm.id;
  agency    = dtm.agency;
  return *this;
  }


void IDinfo::copy(IDinfo& info) {
  callSign  = info.callSign;
  firstName = info.firstName;
  lastName  = info.lastName;
  id        = info.id;
  agency    = info.agency;
  chkOut    = info.chkOut;
  }

