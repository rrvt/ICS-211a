// Roster Information


#include "stdafx.h"
#include "Roster.h"
#include "CheckInOutDlg.h"
#include "CopyFile.h"
#include "EventInfoDlg.h"
#include "Ics_211a.h"
#include "Ics_211aDoc.h"
#include "Ics_211aView.h"
#include "Log211.h"
#include "Members.h"
#include "MembersDlg.h"
#include "VisitorInfoDlg.h"


static TCchar  Comma         = _T(',');
static TCchar* VerTag        = _T("Version>");
static TCchar* NameTag       = _T("IncidentName>");
static TCchar* DateTag       = _T("IncidentDate>");
static TCchar* IncidentNoTag = _T("IncidentNo>");
static TCchar* LocTag        = _T("IncidentLocation>");
static TCchar* PreparedByTag = _T("PreparedBy>");
static TCchar* MissionNoTag  = _T("MissionNo>");

static TCchar* SJRaces       = _T("SJ RACES");

Roster roster;                                 // The single roster object



void Roster::initialize(Archive& ar) {

  clear();

  incidentName.clear();      date.clear();         incidentNo.clear();
  checkInLocation.clear();   preparedBy.clear();   missionNo.clear();

  outputCreated = true;   editTitle();
  }


void Roster::load(Archive& ar) {
RstrIter iter(*this);
Datum*   dtm;

  readVersion(ar);   readEventInfo(ar);   RosterB::load(ar);

  maxCallSign = 0; maxFirstName = 0; maxLastName = 0; maxID = 0;

  for (dtm = iter(); dtm; dtm = iter++) getMaximums(*dtm);

  outputCreated = true;   ar.seekEnd();
  }



void Roster::readVersion(Archive& ar) {ver = getTagged(VerTag, ar);}


void Roster::readEventInfo(Archive&         ar) {
  incidentName    = getTagged(NameTag,       ar);
  date            = getTagged(DateTag,       ar);
  incidentNo      = getTagged(IncidentNoTag, ar);
  checkInLocation = getTagged(LocTag,        ar);
  preparedBy      = getTagged(PreparedByTag, ar);
  missionNo       = getTagged(MissionNoTag,  ar);
  }


String Roster::getTagged(TCchar* tag, Archive& ar) {
String bTag = _T('<');  bTag += tag;
String eTag = _T("</"); eTag += tag;
int    lng  = _tcslen(bTag);
String s;
int    bPos;
int    ePos;

  ar.read(s);

  bPos = s.find(bTag) + lng;   ePos = s.find(eTag, bPos);   return s.substr(bPos, ePos-bPos);
  }


// Put one line of csv file into record a field at a time...  -- The function getLine creates the line
// that is read here (be sure they are inverses of each other!)

void Datum::put(TCchar* p) {
String s = p;
uint   x;

static String date;                         // Temporary during loading

  switch (putI) {
    case 0: dtmType = (DtmType) s.stoi(x); break;
    case 1: callSign  = s; break;
    case 2: firstName = s; break;
    case 3: lastName  = s; break;
    case 4: id        = s; break;
    case 5: agency    = s; break;
    case 6: date      = s; break;
    case 7: setDate(date, s); break;
    case 8: visitor   = s == _T('1'); break;
    case 9: defChkOut = s == _T('1'); break;
    }
  }


// Create line to save for one roster element

String Datum::getLine() {
String s;
  s  = dtmType;  s += Comma;
  s += callSign     + Comma;
  s += firstName    + Comma;
  s += lastName     + Comma;
  s += id           + Comma;
  s += agency       + Comma;
  s += dt.getDate() + Comma;
  s += dt.getTime() + Comma;
  s += visitor;  s += Comma;
  s += defChkOut;
  return s;
  }


void Datum::setDate(String& dat, String& tim) {
String s = dat + _T(" ") + tim;

  dt = s;  seconds = dt.getSeconds();
  }



void Datum::copy(Datum& dtm) {dtmType   = dtm.dtmType;
                              callSign  = dtm.callSign;
                              firstName = dtm.firstName;
                              lastName  = dtm.lastName;
                              id        = dtm.id;
                              dt        = dtm.dt;
                              agency    = dtm.agency;
                              seconds   = dtm.seconds;
                              visitor   = dtm.visitor;
                              defChkOut = dtm.defChkOut;
                              }


void Roster::addBarcode(String& barCode) {
int         pos;
Datum       datum;

  barCode.trim();   if (!validateBarCode(barCode)) return;

  pos = barCode.find(_T(" "));

  if (pos > 0) {datum.callSign = barCode.substr(0, pos);   datum.id  = barCode.substr(pos+1);}

  else datum.callSign = barCode;

  addNew(datum);
  }


void Roster::addMember() {
MembersDlg dlg;
String        s;
int           pos = 0;
Datum         datum;

  if (dlg.DoModal() == IDOK) {

    s = dlg.memberInfo;   datum.dtmType = CheckInType;

    if (s.isEmpty()) {s = dlg.checkOut; datum.dtmType = CheckOutType;}  if (s.isEmpty()) return;

    datum.callSign  = nextTok(s, pos);
    datum.firstName = nextTok(s, pos);
    datum.lastName  = nextTok(s, pos);
    datum.id        = nextTok(s, pos);

    addNew(datum);
    }
  }


void Roster::addVisitor() {
VisitorInfoDlg dlg;
Datum          datum;

  if (dlg.DoModal() == IDOK) {

    String s   = dlg.checkOut;
    int    pos = 0;

    if (!s.isEmpty()) {

      datum.dtmType   = CheckOutType;
      datum.callSign  = nextTok(s, pos);
      datum.firstName = nextTok(s, pos);
      datum.lastName  = nextTok(s, pos);
      datum.agency    = nextTok(s, pos);
      datum.visitor   = true;
      addNew(datum); return;
      }

    if (dlg.callSign.IsEmpty() && dlg.firstName.IsEmpty() && dlg.lastName.IsEmpty()) return;

    datum.dtmType   = CheckInType;
    datum.callSign  = dlg.callSign;
    datum.firstName = dlg.firstName;
    datum.lastName  = dlg.lastName;
    datum.agency    = dlg.agency;
    datum.visitor   = true;
    addNew(datum); return;
    }
  }


void Roster::addNew(Datum& datum) {
Date        date;
time_t      seconds;
Datum*      cur;
Datum*      dtm;
MemberInfo* mi;

  date.getToday();   seconds = date.getSeconds();

  cur = datum.callSign.isEmpty() ? find(datum.firstName, datum.lastName) : find(datum.callSign);

  if (cur) {
    if (seconds <= cur->seconds + 60) return;

    if (cur->dtmType == CheckInType) {
      if (datum.dtmType == CheckInType) return;    // Duplicate check-in

      datum.dtmType = CheckOutType;
      }
    else {
      if (datum.dtmType == CheckOutType) return;  // Duplicate check-out
      if (datum.dtmType != CheckInType) {
        CheckInOutDlg dlg;

        if (!dlg.DoModal()) return;           // Never supposed to happen?
        if (dlg.what == ModCheckOut) {cur->dt = date; cur->seconds = seconds; return;}
        else datum.dtmType = CheckInType;
        }
      }
    }

  else datum.dtmType = CheckInType;

  dtm = add();   mi = members.find(datum.callSign);

  dtm->dtmType = datum.dtmType;   dtm->callSign = datum.callSign;   dtm->id = datum.id;

  if (mi) {dtm->firstName = mi->firstName;   dtm->lastName = mi->lastName;   dtm->agency = SJRaces;}
  else    {dtm->firstName = datum.firstName; dtm->lastName = datum.lastName; dtm->agency = datum.agency;}

  dtm->dt = date;  dtm->seconds = seconds;   dtm->visitor = datum.visitor;

  getMaximums(*dtm);   saveDtm(dtm);
  }


void Roster::add(LogDatum* lgdtm, Date& date) {
Datum* dtmt;

  dtmt = RosterB::add();

  dtmt->dtmType   = CheckOutType;
  dtmt->callSign  = lgdtm->callSign;
  dtmt->firstName = lgdtm->firstName;
  dtmt->lastName  = lgdtm->lastName;
  dtmt->id        = lgdtm->id;
  dtmt->agency    = lgdtm->agency;
  dtmt->dt        = date;
  dtmt->seconds   = date.getSeconds();
  dtmt->visitor   = lgdtm->visitor;
  dtmt->defChkOut = true;

  getMaximums(*dtmt);   saveDtm(dtmt);
  }


void Roster::add(Datum& dtm)
                    {Datum* dtmt = roster.add();  *dtmt = dtm;   getMaximums(*dtmt);   saveDtm(dtmt);}


Datum* Roster::find(TCchar* fcc) {
RstrIter iter(*this);
Datum*   dtm;
Datum*   last;

  for (dtm = iter(), last = 0; dtm; dtm = iter++) if (dtm->callSign == fcc) last = dtm;

  return last;
  }


Datum* Roster::findVisitor(TCchar* fcc) {
RstrIter iter(*this);
Datum*   dtm;
Datum*   last;

  for (dtm = iter(), last = 0; dtm; dtm = iter++)
                                      if (dtm->callSign == fcc && !dtm->firstName.isEmpty()) return dtm;
  return 0;
  }



Datum* Roster::find(TCchar* first, TCchar* last) {
RstrIter iter(*this);
Datum*   dtm;
Datum*   lastDtm;

  for (dtm = iter(), lastDtm = 0; dtm; dtm = iter++)
                                    if (dtm->firstName == first && dtm->lastName == last) lastDtm = dtm;
  return lastDtm;
  }



void Roster::store(Datum& dtm) {Datum* dtmt = &dtm;   getMaximums(*dtmt);   saveDtm(dtmt);}


void Roster::saveDtm(Datum* d) {arDtm = d; doc()->saveDtm();}


void Roster::incStore(Archive& ar) {if (!arDtm) return;   ar << arDtm->getLine() << aCrlf;  arDtm = 0;}


void Roster::getMaximums(Datum& datum) {
  getMax(datum.callSign,  maxCallSign);
  getMax(datum.firstName, maxFirstName);
  getMax(datum.lastName,  maxLastName);
  getMax(datum.id,        maxID);
  }


void Roster::getMax(String& s, int& max) {int lng = s.length();   if (lng > max) max = lng;}


void Roster::editTitle() {
EventInfoDlg dlg;

  if (date.isEmpty()) {Date dt;  dt.getToday();  date = dt.getDate();}

  dlg.name = incidentName;  dlg.date = date;  dlg.incidentNo = incidentNo;
  dlg.location = checkInLocation;
  dlg.preparedBy = preparedBy; dlg.missionNo = missionNo;

  if (dlg.DoModal() == IDOK) {
    incidentName = dlg.name; date = dlg.date; incidentNo = dlg.incidentNo;
    checkInLocation = dlg.location;
    preparedBy = dlg.preparedBy; missionNo = dlg.missionNo;
    }
  }


// Save the entrie file so that the name, location may be updated...

void Roster::store(Archive& ar) {
RstrIter iter(roster);
Datum*   dtm;

  ar << getTagLine(ver,             VerTag)        << aCrlf;
  ar << getTagLine(incidentName,    NameTag)       << aCrlf;
  ar << getTagLine(date,            DateTag)       << aCrlf;
  ar << getTagLine(incidentNo,      IncidentNoTag) << aCrlf;
  ar << getTagLine(checkInLocation, LocTag)        << aCrlf;
  ar << getTagLine(preparedBy,      PreparedByTag) << aCrlf;
  ar << getTagLine(missionNo,       MissionNoTag)  << aCrlf;

  for (dtm = iter(); dtm; dtm = iter++) {
    ar << dtm->getLine() << aCrlf;
    }
  }


void Roster::writeVersion(Archive& ar) {putTagged(ver, VerTag, ar);}


void Roster::putTagged(TCchar* data, TCchar* tag, Archive& ar)
                                                      {ar.write(getTagLine(data, tag)); ar.crlf();}


String Roster::getTagLine(TCchar* data, TCchar* tag) {
String bTag = _T('<');  bTag += tag;
String eTag = _T("</"); eTag += tag;
String s = bTag + data + eTag;

  return s;
  }




#if 0
bool Roster::getRosterPath(String& path) {
TCchar* saveAsTitle = _T("eICS211 File");
TCchar* defExt      = _T("211");
TCchar* defFilePat  = _T("*.211");
TCchar* defFileName = _T("*.211");

  if (!getSaveIncPathDlg(saveAsTitle, defFileName, defExt, defFilePat, outputFilePath)) return false;

  backupCopy(outputFilePath, 5);

  path = outputFilePath;  return true;
  }
#endif



#if 0

  DtmType     dtmType = CheckInType;
  String      fcc;
  String      id;
  Date        date;
  time_t      seconds;
  Datum*      cur;
  Datum*      dtm;
  MemberInfo* mi;
  date.getToday();   seconds = date.getSeconds();

  cur = find(fcc);

  if (cur) {
    if (seconds <= cur->seconds + 60) return;
    if (cur->dtmType == CheckInType) {dtmType = CheckOutType;}
    else {
      CheckInOutDlg dlg;

      if (!dlg.DoModal()) return;           // Never supposed to happen?
      if (dlg.what == ModCheckOut) {cur->dt = date; cur->seconds = seconds; return;}
      else dtmType = CheckInType;
      }
    }

  dtm = add();   mi = members.find(fcc);

  dtm->dtmType = dtmType;   dtm->callSign = fcc;   dtm->id = id;

  if (mi) {dtm->firstName = mi->firstName; dtm->lastName = mi->lastName; dtm->agency = SJRaces;}

  dtm->dt = date;  dtm->seconds = seconds;

  getMaximums(*dtm);   saveDtm(dtm);
#endif

