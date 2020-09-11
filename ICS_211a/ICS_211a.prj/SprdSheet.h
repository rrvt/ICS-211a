// Manage and Output Spread Sheet Data


#pragma once
#include "Archive.h"
#include "Date.h"
#include "Expandable.h"
#include "ICS_211a.h"
#include "ICS_211aView.h"
#include "Roster.h"


struct SSdata {

String callSign;
String firstName;
String lastName;
String id;
Date   dateIn;
Date   dateOut;
String agency;
String remark;
bool   visitor;

bool   noChkOut;
double hours;
String timeIn;
String timeOut;
String hrs;

  SSdata() : noChkOut(false), hours(0.0) { }
  SSdata(SSdata& ssd) {copy(ssd);}
 ~SSdata() { }

  void clear();

  //  *s =  *p
  //  *p == *q
  //  *p >  *q
  //  *p <= *q

  SSdata& operator=  (SSdata& ssd) {copy(ssd); return *this;}
  bool    operator== (SSdata& ad) {return lastName == ad.lastName && firstName == ad.firstName;}
  bool    operator>  (SSdata& ad) {return lastName >  ad.lastName ||
                                                (lastName == ad.lastName  && firstName > ad.firstName);}
  bool    operator<= (SSdata& ad) {return lastName <  ad.lastName ||
                                                (lastName == ad.lastName  && firstName <= ad.firstName);}

  double  getHours();
  int     report();

  void    output(String& line);

private:

  void copy(SSdata& ssd);
  };


class SprdSheet;
typedef IterT<SprdSheet, SSdata> RptIter;

typedef Expandable<Date,   2> CheckOuts;
typedef Expandable<SSdata, 2> SprdShtData;


class SprdSheet {

String&     incidentName;
String&     date;
String&     incidentNo;
String&     checkInLocation;

String&     preparedBy;
String&     missionNo;

double      totalHrs;

int         dataX;
SprdShtData data;

CheckOuts   checkOuts;                 // Needed to determine median Check Out
Date        medianChkOut;

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

  SprdSheet() : incidentName(   roster.incidentName),
                date(           roster.date),
                incidentNo(     roster.incidentNo),
                checkInLocation(roster.checkInLocation),
                preparedBy(     roster.preparedBy),
                missionNo(      roster.missionNo),
                dataX(0), totalHrs(0.0), dspDate(false), maxcallSignLng(0), maxfirstNameLng(0),
                maxlastNameLng(0), maxidLng(0), maxtimeInLng(0), maxtimeOutLng(0),
                maxagencyLng(0), maxremarkLng(0), maxhoursLng(0) { }
 ~SprdSheet() { }

  void    prepare();

  double  getTotalHrs() {return totalHrs;}

  void    display() {view()->dsplyReport();}
  void    checkOutDefauters();
  void    output(Archive& ar);

private:

  void   add(SSdata& ssd);
  void   setDspDate();
  void   clrMaximums();
  void   getMaximums(SSdata& ssd);
  Date   getMedianCheckOut();
  void   getTotalHours();

  void   outputLine(Archive& ar) {ar.write(line); ar.crlf();}

  // returns either a pointer to data (or datum) at index i in array or zero
  SSdata* datum(int i) {return 0 <= i && i < nData() ? &data[i] : 0;}

  // returns number of data items in array
  int   nData()      {return data.end();}

  friend typename RptIter;
  friend class    Organize;
  };



extern SprdSheet sprdSheet;


class Organize {

SprdShtData& data;
CheckOuts&   checkOuts;
bool&        dspDate;

public:

  Organize() : checkOuts(sprdSheet.checkOuts), data(sprdSheet.data), dspDate(sprdSheet.dspDate) { }

  void operator() ();

private:


  bool   isPresent(TCchar* callSign, TCchar* firstName, TCchar* lastName);

  Datum* findLast(RstrIter& itr, SSdata& ssd);
  };
