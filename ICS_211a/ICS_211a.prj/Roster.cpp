// Roster Information


#include "stdafx.h"
#include "Roster.h"
#include "CopyFile.h"
#include "EventInfoDlg.h"
#include "GetPathDlg.h"
#include "Ics_211a.h"
#include "Ics_211aDoc.h"
#include "Log211.h"
#include "MemberInfo.h"
#include "MemberInfoDlg.h"
#include "MessageBox.h"
#include "NotePad.h"
#include "Utilities.h"
#include "VisitorInfoDlg.h"


static TCchar  Comma         = _T(',');
static TCchar* VerTag        = _T("Version>");
static TCchar* NameTag       = _T("IncidentName>");
static TCchar* DateTag       = _T("IncidentDate>");
static TCchar* IncidentNoTag = _T("IncidentNo>");
static TCchar* LocTag        = _T("IncidentLocation>");
static TCchar* PreparedByTag = _T("PreparedBy>");
static TCchar* MissionNoTag  = _T("MissionNo>");


extern TCchar* SJRaces;

Roster roster;                      // The single roster object



bool Roster::getRosterPath(String& path) {
TCchar* saveAsTitle = _T("eICS211 File");
TCchar* defExt      = _T("211");
TCchar* defFilePat  = _T("*.211");
TCchar* defFileName = _T("*.211");

  if (!getSaveIncPathDlg(saveAsTitle, defFileName, defExt, defFilePat, outputFilePath)) return false;

  backupCopy(outputFilePath, 5);

  path = outputFilePath;  return true;
  }


bool Roster::initialize(Archive& ar) {

  return true;
  }


void Roster::load(Archive& ar) {
RstrIter iter(*this);
Datum*   dtm;

  readVersion(ar); readEventInfo(ar);
  RosterB::load(ar);
  for (dtm = iter(); dtm; dtm = iter++) getMaximums(*dtm);
  outputCreated = true;

  ar.seekEnd(); //ar.close(); return;
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


void Datum::put(TCchar* p) {
String s = p;

  switch (putI) {
    case 0: callSign  = s; break;
    case 1: firstName = s; break;
    case 2: lastName  = s; break;
    case 3: id        = s; break;
    case 4: agency    = s; break;
    case 5: date      = s; break;
    case 6: time      = s; s = date + _T(" ") + time;
            dt        = s; break;
    case 7: visitor   = s == _T('1'); break;
    case 8: defChkOut = s == _T('1'); break;
    }
  }


void Roster::addBarcode(String& barCode) {
int         pos   = barCode.find(_T(" "));
MemberInfo* mi;

  barCode.trim();

  if (!validateBarCode(barCode)) return;

  dtmt = add();

  if (pos > 0) {dtmt->callSign = barCode.substr(0, pos);   dtmt->id  = barCode.substr(pos+1);}

  else dtmt->callSign = barCode;

  mi = members.find(dtmt->callSign);

  if (mi) {dtmt->firstName = mi->firstName; dtmt->lastName = mi->lastName; dtmt->agency = SJRaces;}

  dtmt->dt.getToday();

  getMaximums(*dtmt);   doc()->saveDtm();

  display();
  }


void Roster::add(LogDatum* lgdtm, Date& date) {

  dtmt = RosterB::add();

  dtmt->callSign  = lgdtm->callSign;
  dtmt->firstName = lgdtm->firstName;
  dtmt->lastName  = lgdtm->lastName;
  dtmt->id        = lgdtm->id;
  dtmt->agency    = lgdtm->agency;
  dtmt->dt        = date;
  dtmt->visitor   = lgdtm->visitor;
  dtmt->defChkOut = true;

  getMaximums(*dtmt);   doc()->saveDtm();
  }


void Roster::addMember() {
MemberInfoDlg dlg;
String        s;
int           pos = 0;

  if (dlg.DoModal() == IDOK) {

    s = dlg.memberInfo;   if (s.isEmpty()) s = dlg.checkOut;  if (s.isEmpty()) return;

    dtmt = roster.add();

    dtmt->callSign  = nextTok(s, pos);
    dtmt->firstName = nextTok(s, pos);
    dtmt->lastName  = nextTok(s, pos);
    dtmt->id        = nextTok(s, pos);
    dtmt->agency    = SJRaces;
    dtmt->dt.getToday();

    getMaximums(*dtmt);   doc()->saveDtm();
    }
  }


void Roster::addVisitor() {
VisitorInfoDlg dlg;

  if (dlg.DoModal() == IDOK) {

    String s   = dlg.checkOut;
    int    pos = 0;

    if (!s.isEmpty()) {
      dtmt = roster.add();

      dtmt->callSign  = nextTok(s, pos); ;
      dtmt->firstName = nextTok(s, pos); ;
      dtmt->lastName  = nextTok(s, pos); ;
      dtmt->agency    = nextTok(s, pos); ;
      dtmt->dt.getToday();
      dtmt->visitor   = true;
      }

    else {

      if (dlg.callSign.IsEmpty() && dlg.firstName.IsEmpty() && dlg.lastName.IsEmpty()) return;

      dtmt = roster.add();

      dtmt->callSign  = dlg.callSign;
      dtmt->firstName = dlg.firstName;
      dtmt->lastName  = dlg.lastName;
      dtmt->agency    = dlg.agency;
      dtmt->dt.getToday();
      dtmt->visitor   = true;
      }

    if (!dtmt) return;

    getMaximums(*dtmt);   doc()->saveDtm();
    }
  }


void Roster::add(Datum& dtm)
                          {dtmt = roster.add();  *dtmt = dtm;   getMaximums(*dtmt);   doc()->saveDtm();}


void Roster::store(Datum& dtm) {dtmt = &dtm;   getMaximums(*dtmt);   doc()->saveDtm();}


void Roster::incStore(Archive& ar) {if (!dtmt) return;   ar << dtmt->getLine() << aCrlf;  dtmt = 0;}


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


void Roster::display() {
int      tab;
int      tab1;
int      tab2;
int      tab3;
RstrIter iter(*this);
Datum*   dtm;

  notePad.clear();  notePad << nClrTabs << nSetTab(20) << nSetTab(30) << nSetTab(40);

  notePad << nBold << incidentName << nFont;
  notePad << nTab << _T("Date: ") << date;
  notePad << nTab << _T("Incident #: ") << incidentNo;
  notePad << nRight << checkInLocation << nCrlf << nCrlf;

  tab =         maxCallSign  + 2;
  tab1 = tab  + maxFirstName + 1;
  tab2 = tab1 + maxLastName  + 1;
  tab3 = tab2 + maxID        + 1;

  notePad << nClrTabs << nSetTab(tab) << nSetTab(tab1) << nSetTab(tab2) << nSetTab(tab3);

  for (dtm = iter(RstrIter::Rev); dtm; dtm = iter--) {
    notePad <<         dtm->callSign;
    notePad << nTab << dtm->firstName;
    notePad << nTab << dtm->lastName;
    notePad << nTab << dtm->id;
    notePad << nTab << dtm->dt.getDate() << _T("  ") << dtm->dt.getTime();
    notePad << nCrlf;
    }

  notePad << nCrlf;
  notePad << _T("Prepared By: ") << preparedBy;
  notePad << nRight << _T("Mission #: ") << missionNo << nCrlf;

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



String Datum::getLine() {
String s;
  s  = callSign     + Comma;
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


