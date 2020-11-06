// Roster Filter used for determining check in and check out


#pragma once
#include "ExpandableP.h"
#include "Roster.h"


class IDinfo {
public:
String callSign;
String firstName;
String lastName;
String id;
String agency;
bool   chkOut;

  IDinfo() : chkOut(false) { }
  IDinfo(IDinfo& info) {copy(info);}
 ~IDinfo() { }

  bool    operator== (Datum& dtm);
  IDinfo& operator=  (Datum& dtm);
  IDinfo& operator=  (IDinfo& info) {copy(info); return *this;}

private:

  void copy(IDinfo& info);
  };


typedef RcdPtrT<IDinfo> IDinfoP;

class RosterFilter;
typedef IterT<RosterFilter, IDinfo> RFIter;




// Make sure only one roster entry is present
// Caution -- Only works briefly while the roster is not being updated

class RosterFilter {

ExpandableP<IDinfo, IDinfoP, 1> filter;

public:

int maxCallSign;
int maxFirst;
int maxLast;

  RosterFilter() : maxCallSign(0), maxFirst(0), maxLast(0) { }
 ~RosterFilter() { }

  void add(Datum& dtm);
  void getMax(IDinfo& info);

private:

  // returns either a pointer to data (or datum) at index i in array or zero
  IDinfo* datum(int i) {return 0 <= i && i < nData() ? filter[i].p : 0;}

  // returns number of data items in array
  int   nData()      {return filter.end();}

  friend typename RFIter;
  };

