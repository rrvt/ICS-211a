// Manage and Output Spread Sheet Data


#include "stdafx.h"
#include "SprdSheet.h"
#include "DefaultersDlg.h"
#include "EventInfoDlg.h"
#include "NotePad.h"
#include "Roster.h"
#include "Utilities.h"


static TCchar Comma = _T(',');


SprdSheet sprdSheet;


void SprdSheet::prepare() {
Organize organize;

  roster.sort();  organize();  getTotalHours();   qsort(&data[0], &data[data.end()-1]);
  }


void Organize::operator() () {
RstrIter iter(roster);
Datum*   dtm;
Datum*   last;
Date     inTime;
Date     outTime;
String   inDate;
String   outDate;

  data.clear(); checkOuts.clear();

  for (dtm = iter(); dtm; dtm = iter++) {

    if (isPresent(dtm->callSign, dtm->firstName, dtm->lastName)) continue;

    SSdata& ssd = data.nextData();

    ssd.clear();

    ssd.firstName = dtm->firstName;
    ssd.lastName  = dtm->lastName;
    ssd.callSign  = dtm->callSign;
    ssd.id        = dtm->id;
    ssd.agency    = dtm->agency;
    ssd.visitor   = dtm->visitor;
                                                              //[checkOuts.end()]
    inTime        = dtm->dt;

    last = findLast(iter, ssd);

    if (dtm != last) checkOuts += outTime = last->dt;
    else             {outTime = inTime;   ssd.noChkOut = true;   ssd.remark = _T("Did not sign out");}

    ssd.dateIn = inTime;   ssd.dateOut = outTime;

    inDate  = ssd.dateIn.getDate();
    outDate = ssd.dateOut.getDate();
    dspDate |= inDate != outDate;
    }
  }


Datum* Organize::findLast(RstrIter& itr, SSdata& ssd) {
RstrIter iter(itr);
bool      callSignSeen = !ssd.callSign.isEmpty();
Datum*   last         = itr.current();
Datum*   next;

  for (next = iter++; next; next = iter++) {

    if (callSignSeen) {if (next->callSign == ssd.callSign) last = next;}

    else if (next->lastName == ssd.lastName && next->firstName == ssd.firstName) last = next;
    }

  return last;
  }


bool Organize::isPresent(TCchar* callSign, TCchar* firstName, TCchar* lastName) {
bool callSignSeen = callSign != 0 && callSign[0] != 0;
RptIter iter(sprdSheet);
SSdata* ssd;

  for (ssd = iter(); ssd; ssd = iter++) {
    if (callSignSeen) {if (ssd->callSign == callSign) return true;}
    else if (ssd->lastName == lastName && ssd->firstName == firstName) return true;
    }

  return false;
  }


void SprdSheet::getTotalHours() {
int n = data.end();
int i;

  totalHrs = 0.0;   clrMaximums();

  for (i = 0; i < n; i++) {
    SSdata& ssd = data[i];

    totalHrs += ssd.getHours();

    getMaximums(ssd);
    }
  }


double SSdata::getHours() {

  CTimeSpan delta = dateOut - dateIn;
  hours = (delta.GetTotalSeconds())/3600.0;

  hrs.format(_T("%.2f"), hours);

  if (sprdSheet.dspDate) {
    timeIn  = dateIn.getDate() + _T("  ") + dateIn.getHHMM();
    timeOut = dateOut != dateIn ? dateOut.getDate() + _T("  ") + dateOut.getHHMM() : _T("");
    }
  else {
    timeIn  = dateIn.getHHMM();
    timeOut = dateOut != dateIn ? dateOut.getHHMM() : _T("");
    }

  return hours;
  }


void   SprdSheet::clrMaximums() {
  maxfirstNameLng = 0;
  maxlastNameLng  = 0;
  maxcallSignLng  = 0;
  maxidLng        = 0;
  maxagencyLng    = 0;
  maxremarkLng    = 0;
  maxhoursLng     = 0;
  maxtimeInLng    = 0;
  maxtimeOutLng   = 0;
  }


void SprdSheet::getMaximums(SSdata& ssd) {
  getMaxLng(ssd.firstName, maxfirstNameLng);
  getMaxLng(ssd.lastName,  maxlastNameLng);
  getMaxLng(ssd.callSign,  maxcallSignLng);
  getMaxLng(ssd.id,        maxidLng);
  getMaxLng(ssd.agency,    maxagencyLng);
  getMaxLng(ssd.remark,    maxremarkLng);
  getMaxLng(ssd.hrs,       maxhoursLng);
  getMaxLng(ssd.timeIn,    maxtimeInLng);
  getMaxLng(ssd.timeOut,   maxtimeOutLng);
  }


int SSdata::report() {
  notePad << firstName << _T(" ");
  notePad << lastName  << nTab;
  notePad << agency    << nTab;
  notePad << callSign  << nTab;
  notePad << timeIn    << nTab;
  notePad << timeOut   << nTab;
  notePad << hrs       << nTab;
  notePad << id;
  if (!id.isEmpty() && !remark.isEmpty()) notePad << _T(" - ");
  notePad << remark;
  notePad << nCrlf;
  return 1;
  }


static String spc = _T("         ");



void SprdSheet::add(SSdata& ssd) {
Datum dtm;

  dtm.callSign  = ssd.callSign;
  dtm.firstName = ssd.firstName;
  dtm.lastName  = ssd.lastName;
  dtm.id        = ssd.id;
  dtm.agency    = ssd.agency;
  dtm.dt        = ssd.dateOut;
  dtm.visitor   = ssd.visitor;
  roster.add(dtm);
  }


Date SprdSheet::getMedianCheckOut() {
int  n = checkOuts.end();
Date d;

  if (!n) {return d;}

  if (n == 1) return checkOuts[0];

  qsort(&checkOuts[0], &checkOuts[n-1]);

  if (n > 2) return checkOuts[n/2];

  CTimeSpan delta = checkOuts[n-1] - checkOuts[0];
  LONGLONG seconds = delta.GetSeconds();   seconds /= 2;
  CTimeSpan half(seconds);

  return checkOuts[0] + half;
  }


void SprdSheet::output(Archive& ar) {
String s;
int    n;
int    i;

  line = _T("Incident Name:,") + incidentName + _T(",,,,,");
  s.format(_T("%.2f,Total Hours"), totalHrs+0.005);   line += s;   outputLine(ar);

  line = _T("Date:,")        + date;            outputLine(ar);
  line = _T("Location:,")    + checkInLocation; outputLine(ar);
  line = _T("Incident #:,")  + incidentNo;      outputLine(ar);
  line = _T("Mission #:,")   + missionNo;       outputLine(ar);
  line = _T("Prepared By:,") + preparedBy;      outputLine(ar);

  line.clear();  outputLine(ar);

  line = _T("First Name,Last Name,Call Sign,ID#,Time In,Time Out,Hours,Agency,Remarks"); outputLine(ar);

  for (i = 0, n = data.end(); i < n; i++) {data[i].output(line);  outputLine(ar);}
  }



void SSdata::output(String& line) {
  line =  firstName + Comma;
  line += lastName  + Comma;
  line += callSign  + Comma;
  line += id        + Comma;
  line += timeIn    + Comma;
  line += timeOut   + Comma;
  line += hrs       + Comma;
  line += agency    + Comma;
  line += remark;
  }


void SSdata::clear() {
Date zero;
  callSign.clear();
  firstName.clear();
  lastName.clear();
  id.clear();
  dateIn  = zero;
  dateOut = zero;
  agency.clear();
  remark.clear();
  noChkOut = false;
  hours    = 0.0;
  timeIn.clear();
  timeOut.clear();
  hrs.clear();
  }


void SSdata::copy(SSdata& ssd) {
  callSign  = ssd.callSign;
  firstName = ssd.firstName;
  lastName  = ssd.lastName;
  id        = ssd.id;
  dateIn    = ssd.dateIn;
  dateOut   = ssd.dateOut;
  agency    = ssd.agency;
  remark    = ssd.remark;
  noChkOut  = ssd.noChkOut;
  hours     = ssd.hours;
  timeIn    = ssd.timeIn;
  timeOut   = ssd.timeOut;
  hrs       = ssd.hrs;
  }

