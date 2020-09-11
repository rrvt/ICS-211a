// Manage and Output Spread Sheet Data


#pragma once
#include "Archive.h"
#include "Date.h"
#include "Expandable.h"
#include "IterT.h"
#include "Roster.h"


struct LogDatum {

String callSign;
String firstName;
String lastName;
String id;
Date   dateIn;
Date   dateOut;
String agency;
String remark;
bool   visitor;

Datum* rosterIn;
Datum* rosterOut;

bool   noChkOut;
double hours;
String timeIn;
String timeOut;
String hrs;

  LogDatum() {clear();}
  LogDatum(LogDatum& ssd) {copy(ssd);}
 ~LogDatum() { }

  void clear();

  //  *s =  *p
  //  *p == *q
  //  *p >  *q
  //  *p <= *q

  LogDatum& operator=  (LogDatum& ssd) {copy(ssd); return *this;}
  bool      operator== (LogDatum& ad) {return lastName == ad.lastName && firstName == ad.firstName;}
  bool      operator>  (LogDatum& ad) {return lastName >  ad.lastName ||
                                                  (lastName == ad.lastName  && firstName > ad.firstName);}
  bool      operator<= (LogDatum& ad) {return lastName <  ad.lastName ||
                                                (lastName == ad.lastName  && firstName <= ad.firstName);}

  double    getHours();
  int       report();

  void      output(String& line);

private:

  void copy(LogDatum& ssd);
  };


class Log211;
typedef IterT<Log211, LogDatum> LogIter;

typedef Expandable<LogDatum, 2> LogData;


class Log211 {

String&     incidentName;
String&     date;
String&     incidentNo;
String&     checkInLocation;

String&     preparedBy;
String&     missionNo;

double      totalHrs;

LogData     data;

int         maxcallSignLng;
int         maxfirstNameLng;
int         maxlastNameLng;
int         maxidLng;
int         maxtimeInLng;
int         maxtimeOutLng;
int         maxagencyLng;
int         maxremarkLng;
int         maxhoursLng;

String      line;

public:
bool        dspDate;

  Log211() : incidentName(      roster.incidentName),
                date(           roster.date),
                incidentNo(     roster.incidentNo),
                checkInLocation(roster.checkInLocation),
                preparedBy(     roster.preparedBy),
                missionNo(      roster.missionNo), totalHrs(0.0), dspDate(false) {clrMaximums();}
 ~Log211() { }

  void    prepare();

  Date    getMedianCheckOut();

  double  getTotalHrs() {return totalHrs;}

  void    output(Archive& ar);

private:

  Date   average(Date& d0, Date& d1);
  void   clrMaximums();
  void   getMaximums(LogDatum& ssd);
  void   getTotalHours();

  void   outputLine(Archive& ar) {ar.write(line); ar.crlf();}

  // returns either a pointer to data (or datum) at index i in array or zero
  LogDatum* datum(int i) {return 0 <= i && i < nData() ? &data[i] : 0;}

  // returns number of data items in array
  int   nData()      {return data.end();}

  friend typename LogIter;
  friend class    OrganizeLog;
  };



extern Log211 log211;


class OrganizeLog {

LogData& data;
bool&    dspDate;

public:

  OrganizeLog() : data(log211.data), dspDate(log211.dspDate) { }

  void operator() ();

private:


  bool   isPresent(TCchar* callSign, TCchar* firstName, TCchar* lastName);

  Datum* findLast(RstrIter& itr, LogDatum& ssd);
  };

