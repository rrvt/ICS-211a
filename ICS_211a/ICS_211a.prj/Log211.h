// Manage and Output Spread Sheet Data


#pragma once
#include "Date.h"
#include "Expandable.h"
#include "IterT.h"
#include "Roster.h"
#include "Utilities.h"


class  Archive;
class  Display;
class  ICS_211aView;
class  NotePad;



struct LogDatum {
int    sortKey;

String callSign;              // Raw Data
String firstName;
String lastName;
String id;
Date   checkIn;               // Check In Time (and Date)
Date   checkOut;              // Check Out Time (and Date)
String agency;
bool   visitor;

Datum* rosterIn;              // Derived Data
Datum* rosterOut;

String dateIn;
String timeIn;

bool   chkdOut;
String dateOut;
String timeOut;

double hours;               // Derived Data
String hrs;

  LogDatum() {clear();}
  LogDatum(LogDatum& ssd) {copy(ssd);}
 ~LogDatum() { }

  void clear();

  void create(Datum& dIn, RstrIter& itr);

  //  *s =  *p
  //  *p == *q
  //  *p >  *q
  //  *p <= *q

  LogDatum& operator=  (LogDatum& ssd) {copy(ssd); return *this;}
  bool      operator== (LogDatum& ad)
                                     {return lastName == ad.lastName && firstName == ad.firstName;}
  bool      operator>  (LogDatum& ad) {return lastName >  ad.lastName ||
                                           (lastName == ad.lastName  && firstName > ad.firstName);}
  bool      operator<= (LogDatum& ad) {return lastName <  ad.lastName ||
                                          (lastName == ad.lastName  && firstName <= ad.firstName);}
  int       report(NotePad& np, bool dspDate);

  void      output(String& line);

private:

  Datum*    findNext(RstrIter& itr, LogDatum& lgdtm);

  void      copy(LogDatum& ssd);
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

String      line;
Date        tmpDate;

public:

  Log211() : incidentName(   roster.incidentName),
             date(           roster.date),
             incidentNo(     roster.incidentNo),
             checkInLocation(roster.checkInLocation),
             preparedBy(     roster.preparedBy),
             missionNo(      roster.missionNo),
             totalSecs(0) { }
 ~Log211() { }

  bool    prepare();
  void    organize();

  Date    getMedianCheckOut();
  Date&   suggestDate(LogDatum* datum);

  bool    includeDate();
  void    output(Archive& ar);

private:

  Date   average(Date& d0, Date& d1);

  void   outputLine(Archive& ar) {ar << line << aCrlf;}

  // returns either a pointer to data (or datum) at index i in array or zero
  LogDatum* datum(int i) {return 0 <= i && i < nData() ? &data[i] : 0;}

  // returns number of data items in array
  int   nData()      {return data.end();}

  friend typename LogIter;
  friend class    Log211Rpt;
  };


extern Log211 log211;




