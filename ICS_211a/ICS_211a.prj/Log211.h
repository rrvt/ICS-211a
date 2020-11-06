// Manage and Output Spread Sheet Data


#pragma once
#include "IterT.h"
#include "Roster.h"
#include "Utilities.h"



class Archive;
class Display;
class ICS_211aView;
class NotePad;


struct LogDatum {
int    sortKey;

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
time_t seconds;
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

  time_t    getSecs();
  int       report(NotePad& np);

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

time_t      totalSecs;

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

  Log211() : incidentName(   roster.incidentName),
             date(           roster.date),
             incidentNo(     roster.incidentNo),
             checkInLocation(roster.checkInLocation),
             preparedBy(     roster.preparedBy),
             missionNo(      roster.missionNo),
             totalSecs(0), dspDate(false) {clrMaximums();}
 ~Log211() { }

  bool    prepare();

  Date    getMedianCheckOut();
  Date    suggestDate(LogDatum* datum);

  double  getTotalHrs() {return toHours(totalSecs);}

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
  friend class    log211Rpt;
  };


extern Log211 log211;


class OrganizeLog {

LogData& data;
bool&    dspDate;

public:

  OrganizeLog() : data(log211.data), dspDate(log211.dspDate) { }

  void operator() ();

private:

  Datum* findNext(RstrIter& itr, LogDatum& lgdtm);
  };

