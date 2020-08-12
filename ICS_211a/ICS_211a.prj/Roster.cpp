// Roster Information


#include "stdafx.h"
#include "Roster.h"
#include "CopyFile.h"
#include "EventInfoDlg.h"
#include "GetPathDlg.h"
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
static TCchar* IncidentNoTag = _T("IncidentNo");
static TCchar* LocTag        = _T("IncidentLocation>");
static TCchar* PreparedByTag = _T("PreparedBy");
static TCchar* MissionNoTag  = _T("MissionNo");


extern TCchar* SJRaces;

Roster roster;                      // The single roster object

#if 0
Date dt;
  dt.getToday();  date = dt.getDate();  dateCtrl.SetWindowText(date);
#endif


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


void Roster::addMember() {
MemberInfoDlg dlg;
String        s;
int           pos = 0;

  if (dlg.DoModal() == IDOK) {
    Datum& dtm = data[data.end()];
    s = dlg.memberInfo;   if (s.isEmpty()) s = dlg.checkOut;  if (s.isEmpty()) return;

    dtm.fcc       = nextTok(s, pos);
    dtm.firstName = nextTok(s, pos);
    dtm.lastName  = nextTok(s, pos);
    dtm.id        = nextTok(s, pos);
    dtm.agency    = SJRaces;
    dtm.dt.getToday();

    getMaximums(dtm);  store(dtm);
    }
  }


void Roster::addVisitor() {
VisitorInfoDlg dlg;

  if (dlg.DoModal() == IDOK) {
    Datum& dtm = data[data.end()];
    String s   = dlg.checkOut;
    int    pos = 0;

    if (!s.isEmpty()) {
      dtm.fcc       = nextTok(s, pos); ;
      dtm.firstName = nextTok(s, pos); ;
      dtm.lastName  = nextTok(s, pos); ;
      dtm.agency    = nextTok(s, pos); ;
      dtm.dt.getToday();
      dtm.visitor   = true;
      }

    else {
      dtm.fcc       = dlg.callSign;
      dtm.firstName = dlg.firstName;
      dtm.lastName  = dlg.lastName;
      dtm.agency    = dlg.agency;
      dtm.dt.getToday();
      dtm.visitor   = true;
      }

    getMaximums(dtm);   store(dtm);
    }
  }


void Roster::addBarcode(String& barCode) {
int         pos   = barCode.find(_T(" "));
MemberInfo* mi;

  barCode.trim();

  if (!validateBarCode(barCode)) return;

  Datum& dtm = data[data.end()];

  if (pos > 0) {dtm.fcc = barCode.substr(0, pos);   dtm.id  = barCode.substr(pos+1);}

  else dtm.fcc = barCode;

  mi = members.find(dtm.fcc);

  if (mi) {dtm.firstName = mi->firstName; dtm.lastName = mi->lastName; dtm.agency = SJRaces;}

  dtm.dt.getToday();    getMaximums(dtm);   store(dtm);   display();
  }




void Roster::add(LogDatum* lgdtm, Date& date) {
Datum dtm;

  dtm.fcc       = lgdtm->fcc;
  dtm.firstName = lgdtm->firstName;
  dtm.lastName  = lgdtm->lastName;
  dtm.id        = lgdtm->id;
  dtm.agency    = lgdtm->agency;
  dtm.dt        = date;
  dtm.visitor   = lgdtm->visitor;
  dtm.defChkOut = true;              add(dtm);   store(dtm);
  }


void Roster::add(Datum& dtm) {
Datum& datum = data[data.end()];

  datum = dtm;

  getMax(datum.fcc,       datum.fccLng,       maxFCC);
  getMax(datum.firstName, datum.firstNameLng, maxFirstName);
  getMax(datum.lastName,  datum.lastNameLng,  maxLastName);
  getMax(datum.id,        datum.idLng,        maxID);
  }


void Roster::getMaximums(Datum& datum) {
  getMax(datum.fcc,       datum.fccLng,       maxFCC);
  getMax(datum.firstName, datum.firstNameLng, maxFirstName);
  getMax(datum.lastName,  datum.lastNameLng,  maxLastName);
  getMax(datum.id,        datum.idLng,        maxID);
  }


void Roster::getMax(String& s, int& lng, int& max) {lng = s.length();   if (lng > max) max = lng;}


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

  tab =         maxFCC + 2;
  tab1 = tab  + maxFirstName + 1;
  tab2 = tab1 + maxLastName + 1;
  tab3 = tab2 + maxID + 1;

  notePad << nClrTabs;
  notePad << nSetTab(tab) << nSetTab(tab1) << nSetTab(tab2) << nSetTab(tab3);

  for (dtm = iter(RstrIter::Rev); dtm; dtm = iter--) {
    notePad <<         dtm->fcc;
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


bool Roster::openOutputFile() {
TCchar* saveAsTitle = _T("eICS211 File");
TCchar* defExt      = _T("211");
TCchar* defFilePat  = _T("*.211");
TCchar* defFileName = _T("*.211");

  if (!getSaveIncPathDlg(saveAsTitle, defFileName, defExt, defFilePat, outputFilePath)) return false;

  backupCopy(outputFilePath, 5);

  if (oFile.open(outputFilePath, FileIO::Read | FileIO::Write)) {

    readVersion(); readEventInfo();  loadRoster(); outputCreated = true;

    oFile.seekEnd(); oFile.close(); return true;
    }

  if (oFile.open(outputFilePath, FileIO::Write)) {

    writeVersion();   initEventInfo();  outputCreated = true; oFile.close();

    oFile.setMode(FileIO::Read | FileIO::Write); return true;
    }

  return false;
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


void Roster::initEventInfo() {
EventInfoDlg dlg;
CString      cs;

  if (dlg.DoModal() == IDOK) {
    incidentName    = dlg.name;
    date            = dlg.date;
    incidentNo      = dlg.incidentNo;
    checkInLocation = dlg.location;
    preparedBy      = dlg.preparedBy;
    missionNo       = dlg.missionNo;

    putTagged(incidentName,    NameTag);
    putTagged(date,            DateTag);
    putTagged(incidentNo,      IncidentNoTag);
    putTagged(checkInLocation, LocTag);
    putTagged(preparedBy,      PreparedByTag);
    putTagged(missionNo,       MissionNoTag);
    }
  }


void Roster::readEventInfo() {
  incidentName    = getTagged(NameTag);
  date            = getTagged(DateTag);
  incidentNo      = getTagged(IncidentNoTag);
  checkInLocation = getTagged(LocTag);
  preparedBy      = getTagged(PreparedByTag);
  missionNo       = getTagged(MissionNoTag);
  }


void Roster::writeVersion() {putTagged(ver, VerTag);}


void Roster::readVersion() {ver = getTagged(VerTag);}


void Roster::putTagged(TCchar* data, TCchar* tag) {oFile.write(getTagLine(data, tag)); oFile.crlf();}


String Roster::getTagLine(TCchar* data, TCchar* tag) {
String bTag = _T('<');  bTag += tag;
String eTag = _T("</"); eTag += tag;
String s = bTag + data + eTag;

  return s;
  }


String Roster::getTagged(TCchar* tag) {
String bTag = _T('<');  bTag += tag;
String eTag = _T("</"); eTag += tag;
int    lng  = _tcslen(bTag);
String s;
int    bPos;
int    ePos;

  oFile.read(s);

  bPos = s.find(bTag) + lng;   ePos = s.find(eTag, bPos);   return s.substr(bPos, ePos-bPos);
  }



void Roster::store(Datum& dtm) {
String s;

  if (!oFile.reOpen()) {
    if (!unableSent) {messageBox(_T("Unable to store roster information.")); unableSent = true;}
    return;
    }

  oFile.write(dtm.getLine());  oFile.crlf();  oFile.close();
  }


String Datum::getLine() {
String s;
  s  = fcc          + Comma;
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


void Roster::loadRoster() {
String s;
int    pos;
String dt;

  loop {
    Datum dtm;

    if (!oFile.read(s)) return;
    pos = 0;

    dtm.fcc       = nextTok(s, pos);
    dtm.firstName = nextTok(s, pos);
    dtm.lastName  = nextTok(s, pos);
    dtm.id        = nextTok(s, pos);
    dtm.agency    = nextTok(s, pos);
    dt = nextTok(s, pos) + _T(" ");  dt += nextTok(s, pos);
    dtm.dt = dt;
    dtm.visitor   = nextTok(s, pos) == _T('1');
    dtm.defChkOut = nextTok(s, pos) == _T('1');
    add(dtm);
    }
  }


RosterFilter::~RosterFilter() {
int n = filter.end();
int i;

  for (i = 0; i < n; i++) filter[i].freeDatum();
  }


void RosterFilter::add(Datum& dtm) {
int n = filter.end();
int i;

  for (i = 0; i < n; i++) {
    if (filter[i] == dtm) return;
    }

  filter[n].p = &dtm;
  }







#if 0
//  *p == *q
//  *p >  *q
//  *p <= *q

bool Datum::operator== (Datum& dtm) {
  return lastName == dtm.lastName && firstName == dtm.firstName && fcc == dtm.fcc && dt == dtm.dt;
  }


bool Datum::operator>  (Datum& dtm) {
  return lastName >  dtm.lastName ||
        (lastName == dtm.lastName && firstName >  dtm.firstName) ||
        (lastName == dtm.lastName && firstName == dtm.firstName && fcc >  dtm.fcc) ||
        (lastName == dtm.lastName && firstName == dtm.firstName && fcc == dtm.fcc && dt > dtm.dt);
  }


bool Datum::operator<= (Datum& dtm) {
  return lastName < dtm.lastName ||
        (lastName == dtm.lastName && firstName <  dtm.firstName) ||
        (lastName == dtm.lastName && firstName == dtm.firstName && fcc <  dtm.fcc) ||
        (lastName == dtm.lastName && firstName == dtm.firstName && fcc == dtm.fcc && dt <= dtm.dt);
  }
#endif



//Datum* RosterFilter::startLoop() {fltrX = 0; return current();}


//Datum* RosterFilter::current() {return fltrX < filter.end() ? filter[fltrX].p : 0;}


//Datum* RosterFilter::nextDatum() {fltrX++; return current();}
#if 0
Datum* Roster::startLoop(bool forward)
                                      {dir = forward;  dataX = dir ? 0 : data.end()-1; return current();}


Datum* Roster::nextDatum() {dataX += dir ? 1 : -1;   return current();}


Datum* Roster::current() {return 0 <= dataX && dataX < data.end() ? &data[dataX] : 0;}
#endif


#if 0
void Roster::putTagged(TCchar* begTag, TCchar* data, TCchar* endTag)
  {oFile.write(begTag); oFile.write(data); oFile.write(endTag); oFile.crlf();}


String Roster::getTagged(TCchar* begTag, TCchar* endTag) {
int    lng = _tcslen(begTag);
String s;
int    bPos;
int    ePos;

  oFile.read(s);

  bPos = s.find(begTag) + lng;   ePos = s.find(endTag, bPos);   return s.substr(bPos, ePos-bPos);
  }
#endif

