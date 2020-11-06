// Roster Information


#pragma once
#include "CSVRcdB.h"
#include "CSVRcdsT.h"
#include "qsort.h"

class ICS_211aView;
class Display;


enum   DtmType {NilDtmType, CheckInType, CheckOutType};
struct LogDatum;


struct Datum : CSVRcdB {

DtmType dtmType;

String  callSign;
String  firstName;
String  lastName;

String  id;
Date    dt;
String  agency;

time_t  seconds;
bool    visitor;
bool    defChkOut;

  Datum() : dtmType(NilDtmType), seconds(0), visitor(false), defChkOut(false)  { }
  Datum(Datum& dtm) {copy(dtm);}
 ~Datum() { }

  void setDate(String& dt, String& tm);

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

  void copy(Datum& dtm);
  };




typedef RcdPtrT< Datum> RosterP;
typedef CSVRcdsT<Datum, RosterP> RosterB;
typedef CSVIterT<Datum, RosterP> RstrIter;


class Roster : public RosterB {

String ver;

bool   outputCreated;

Datum* arDtm;                           // tempaory pointer to datum for Archive...

public:

int    maxCallSign;
int    maxFirstName;
int    maxLastName;
int    maxID;

String outputFilePath;

String incidentName;
String date;
String incidentNo;
String checkInLocation;

String preparedBy;
String missionNo;

  Roster() : ver(_T("1")), arDtm(0) { }
 ~Roster() { }

  bool    getRosterPath(String& path);

  void    initialize(Archive& ar);
  void    load(Archive& ar);
  bool    isLoaded() {return data.end() > 0;}
  void    editTitle();

  void    addMember();
  void    addVisitor();
  void    addBarcode(String& barCode);
  void    addNew(Datum& datum);

  Datum*  add() {return RosterB::add();}
  void    add(LogDatum* lgdtm, Date& date);
  void    add(Datum& dtm);

  Datum*  find(TCchar* fcc);
  Datum*  find(TCchar* first, TCchar* last);
  Datum*  findVisitor(TCchar* fcc);

  TCchar* path() {return outputCreated ? outputFilePath.str() : 0;}
  void    saveDtm(Datum* d);
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

  friend class RosterRpt;
  };


extern Roster roster;
