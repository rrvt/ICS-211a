// Roster Information


#pragma once
#include "Archive.h"
#include "Date.h"
#include "Expandable.h"
#include "FileIO.h"
#include "IterT.h"
#include "qsort.h"


struct LogDatum;


struct Datum {
String fcc;
String firstName;
String lastName;
String id;
Date   dt;
String agency;
bool   visitor;
bool   defChkOut;

int    fccLng;
int    firstNameLng;
int    lastNameLng;
int    idLng;


  Datum() : fccLng(0), firstNameLng(0), lastNameLng(0), idLng(0), visitor(false), defChkOut(false)  { }
  Datum(Datum& dtm) {copy(dtm);}
 ~Datum() { }

  String getLine();

 //  *s = *p

  Datum& operator= (Datum& dtm) {copy(dtm); return *this;}

  //  *p == *q
  //  *p >  *q
  //  *p <= *q

  bool operator== (Datum& dtm) {return dt == dtm.dt;}
  bool operator>  (Datum& dtm) {return dt >  dtm.dt;}
  bool operator<= (Datum& dtm) {return dt <= dtm.dt;}

private:

  void copy(Datum& dtm) {
    fcc = dtm.fcc; firstName = dtm.firstName; lastName = dtm.lastName; id = dtm.id; dt = dtm.dt;
    agency = dtm.agency; fccLng = dtm.fccLng; firstNameLng = dtm.firstNameLng;
    lastNameLng = dtm.lastNameLng; idLng = dtm.idLng;   visitor = dtm.visitor;  defChkOut = dtm.defChkOut;
    }
  };


class Roster;
typedef IterT<Roster, Datum> RstrIter;


class Roster {

bool                 dir;
int                  dataX;
Expandable<Datum, 2> data;

String ver;

bool   outputCreated;
String outputFilePath;
FileIO oFile;
bool   unableSent;

public:

String incidentName;
String date;
String incidentNo;
String checkInLocation;

String preparedBy;
String missionNo;

int    maxFCC;
int    maxFirstName;
int    maxLastName;
int    maxID;

  Roster() : ver(_T("1")), outputCreated(false), unableSent(false) { }
 ~Roster() { }

  void    editTitle();

  void    addMember();
  void    addVisitor();
  void    addBarcode(String& barCode);

  void    add(LogDatum* lgdtm, Date& date);
  void    add(Datum& dtm);
  void    display();

  bool    openOutputFile();
  TCchar* path() {return outputCreated ? outputFilePath.str() : 0;}
  void    store(Archive& ar);

  void    sort() {qsort(&data[0], &data[data.end()-1]);}

private:

  void   getMaximums(Datum& datum);
  void   getMax(String& s, int& lng, int& max);

  void   initEventInfo();
  void   readEventInfo();

  void   writeVersion();
  void   readVersion();
  String getTagged(TCchar* tag);
  void   putTagged( TCchar* data, TCchar* tag);
  String getTagLine(TCchar* data, TCchar* tag);

  void   store(Datum& dtm);
  void   loadRoster();

private:

  // returns either a pointer to data (or datum) at index i in array or zero
  Datum* datum(int i) {return 0 <= i && i < nData() ? &data[i] : 0;}

  // returns number of data items in array
  int   nData()      {return data.end();}

  friend typename RstrIter;
  };


extern Roster roster;




class DatumP {
Datum* p;

public:

  DatumP() : p(0) { }
  DatumP(DatumP& dp) {p = dp.p;}
 ~DatumP() {p = 0;}

  void    freeDatum() {p = 0;}
  DatumP& operator= (DatumP& dp) {p = dp.p; return *this;}

  bool    operator== (Datum& dp) {
    if (!p)                           return false;
    if (p            == &dp)          return true;
    if (p->fcc       != dp.fcc)       return false;
    if (p->lastName  != dp.lastName)  return false;
    if (p->firstName != dp.firstName) return false;
    return true;
    }

  friend class RosterFilter;
  };


class RosterFilter;
typedef IterT<RosterFilter, Datum> RFIter;


// Make sure only one roster entry is present
// Caution -- Only works briefly while the roster is not being updated

class RosterFilter {
int                   fltrX;
Expandable<DatumP, 2> filter;

public:

  RosterFilter() { }
 ~RosterFilter();

  void add(Datum& dtm);

//  Datum* startLoop();
//  Datum* current();
//  Datum* nextDatum();

private:

  // returns either a pointer to data (or datum) at index i in array or zero
  Datum* datum(int i) {return 0 <= i && i < nData() ? filter[i].p : 0;}

  // returns number of data items in array
  int   nData()      {return filter.end();}

  friend typename RFIter;
  };



