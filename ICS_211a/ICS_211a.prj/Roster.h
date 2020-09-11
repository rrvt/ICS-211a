// Roster Information


#pragma once
#include "Archive.h"
#include "CSVRcdB.h"
#include "CSVRcdsT.h"
#include "Date.h"
#include "qsort.h"


struct LogDatum;


struct Datum : CSVRcdB {
String callSign;
String firstName;
String lastName;
String id;
String date;
String time;
Date   dt;
String agency;
bool   visitor;
bool   defChkOut;


  Datum() : visitor(false), defChkOut(false)  { }
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

  virtual void    put(TCchar* p);

//  virtual String* get();

  void copy(Datum& dtm) {
    callSign = dtm.callSign; firstName = dtm.firstName; lastName = dtm.lastName; id = dtm.id; dt = dtm.dt;
    agency = dtm.agency; visitor = dtm.visitor;  defChkOut = dtm.defChkOut;
    }
  };




typedef RcdPtrT< Datum> RosterP;
typedef CSVRcdsT<Datum, RosterP> RosterB;
typedef CSVIterT<Datum, RosterP> RstrIter;


class Roster : public RosterB {
bool    dir;
String  ver;

bool    outputCreated;
String  outputFilePath;
bool    unableSent;

Datum* dtmt;                      // tempaory pointer to datum

public:

String incidentName;
String date;
String incidentNo;
String checkInLocation;

String preparedBy;
String missionNo;

int    maxCallSign;
int    maxFirstName;
int    maxLastName;
int    maxID;

  Roster() : ver(_T("1")), outputCreated(false), unableSent(false), dtmt(0) { }
 ~Roster() { }

  bool    getRosterPath(String& path);

  bool    initialize(Archive& ar);
  void    load(Archive& ar);
  bool    isLoaded() {return data.end() > 0;}
  void    editTitle();

  void    addMember();
  void    addVisitor();
  void    addBarcode(String& barCode);

  Datum*  add() {return RosterB::add();}
  void    add(LogDatum* lgdtm, Date& date);
  void    add(Datum& dtm);
  void    display();

  TCchar* path() {return outputCreated ? outputFilePath.str() : 0;}
  void    incStore(Archive& ar);
  void    store(Archive& ar);
  void    store(Datum& dtm);

  void    sort() {qsort(&data[0], &data[data.end()-1]);}

private:

  void   getMaximums(Datum& datum);
  void   getMax(String& s, int& max);

  void   readEventInfo(Archive& ar);

  void   writeVersion(Archive& ar);
  void   readVersion(Archive& ar);
  String getTagged(TCchar* tag, Archive& ar);
  void   putTagged( TCchar* data, TCchar* tag, Archive& ar);
  String getTagLine(TCchar* data, TCchar* tag);
  };


extern Roster roster;

