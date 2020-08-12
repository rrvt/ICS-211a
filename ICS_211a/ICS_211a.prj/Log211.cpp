// Manage and Output Spread Sheet Data


#include "stdafx.h"
#include "Log211.h"
#include "NotePad.h"
#include "Utilities.h"


static TCchar Comma = _T(',');


Log211 log211;


void Log211::prepare() {
OrganizeLog organize;

  roster.sort();  organize();  getTotalHours();   qsort(&data[0], &data[data.end()-1]);
  }


void OrganizeLog::operator() () {
RstrIter iter(roster);
Datum*   dtm;
Datum*   last;
Date     inTime;
Date     outTime;
String   inDate;
String   outDate;

  data.clr();

  for (dtm = iter(); dtm; dtm = iter++) {

    if (isPresent(dtm->fcc, dtm->firstName, dtm->lastName)) continue;

    LogDatum& lgdtm = data[data.end()];

    lgdtm.clear();

    lgdtm.firstName = dtm->firstName;
    lgdtm.lastName  = dtm->lastName;
    lgdtm.fcc       = dtm->fcc;
    lgdtm.id        = dtm->id;
    lgdtm.agency    = dtm->agency;
    lgdtm.visitor   = dtm->visitor;

    inTime          = dtm->dt;
    lgdtm.rosterIn  = dtm;

if (dtm->fcc == _T("AI6EH")) {
int x = 1;
}

    last = findLast(iter, lgdtm);

    if (last != dtm) {outTime = last->dt; lgdtm.noChkOut = last->defChkOut; lgdtm.rosterOut = last;}
    else             {outTime = inTime;   lgdtm.noChkOut = true;}

    if (lgdtm.noChkOut) {lgdtm.remark = _T("Did not sign out");}

    lgdtm.dateIn = inTime;   lgdtm.dateOut = outTime;

    inDate  = lgdtm.dateIn.getDate();
    outDate = lgdtm.dateOut.getDate();
    dspDate |= inDate != outDate;
    }
  }


Datum* OrganizeLog::findLast(RstrIter& itr, LogDatum& lgdtm) {
RstrIter iter(itr);
bool     callSignSeen = !lgdtm.fcc.isEmpty();
Datum*   last         = itr.current();
Datum*   next;

  for (next = iter++; next; next = iter++) {

    if (callSignSeen) {if (next->fcc == lgdtm.fcc) last = next;}

    else if (next->lastName == lgdtm.lastName && next->firstName == lgdtm.firstName) last = next;
    }

  return last;
  }


bool OrganizeLog::isPresent(TCchar* fcc, TCchar* firstName, TCchar* lastName) {
bool      callSignSeen = fcc != 0 && fcc[0] != 0;
LogIter   iter(log211);
LogDatum* lgdtm;

  for (lgdtm = iter(); lgdtm; lgdtm = iter++) {
    if (callSignSeen) {if (lgdtm->fcc == fcc) return true;}
    else if (lgdtm->lastName == lastName && lgdtm->firstName == firstName) return true;
    }

  return false;
  }



Date Log211::getMedianCheckOut() {
LogIter              iter(log211);
LogDatum*            lgdtm;
Expandable<Date, 32> dates;
int                  n;
Date                 d;

  for (lgdtm = iter(); lgdtm; lgdtm = iter++)
                                if (lgdtm->dateIn < lgdtm->dateOut) dates[dates.end()] = lgdtm->dateOut;

  n = dates.end();   if (!n) return d;

  if (n == 1) return dates[0];

  qsort(&dates[0], &dates[n-1]);

  if (n > 2) {
    if (n % 2 == 1) return dates[n/2];
    return average(dates[n/2], dates[n/2+1]);
    }

  return average(dates[0], dates[1]);
  }


Date Log211::average(Date& d0, Date& d1) {
CTimeSpan  delta   = d1 - d0;
__time64_t seconds = delta.GetTotalSeconds();   seconds /= 2;
CTimeSpan  half(seconds);

  return d0 + half;
  }



void Log211::getTotalHours() {
LogIter   iter(*this);
LogDatum* lgdtm;

  totalHrs = 0.0;   clrMaximums();

  for (lgdtm = iter(); lgdtm; lgdtm = iter++) {

    totalHrs += lgdtm->getHours();

    getMaximums(*lgdtm);
    }
  }


double LogDatum::getHours() {

  CTimeSpan delta = dateOut - dateIn;
  hours = (delta.GetTotalSeconds())/3600.0;

  hrs.format(_T("%.2f"), hours);

  if (log211.dspDate) {
    timeIn  = dateIn.getDate() + _T("  ") + dateIn.getHHMM();
    timeOut = dateOut != dateIn ? dateOut.getDate() + _T("  ") + dateOut.getHHMM() : _T("");
    }
  else {
    timeIn  = dateIn.getHHMM();
    timeOut = dateOut != dateIn ? dateOut.getHHMM() : _T("");
    }

  return hours;
  }


void   Log211::clrMaximums() {
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


void Log211::getMaximums(LogDatum& lgdtm) {
  getMaxLng(lgdtm.firstName, maxfirstNameLng);
  getMaxLng(lgdtm.lastName,  maxlastNameLng);
  getMaxLng(lgdtm.fcc,       maxcallSignLng);
  getMaxLng(lgdtm.id,        maxidLng);
  getMaxLng(lgdtm.agency,    maxagencyLng);
  getMaxLng(lgdtm.remark,    maxremarkLng);
  getMaxLng(lgdtm.hrs,       maxhoursLng);
  getMaxLng(lgdtm.timeIn,    maxtimeInLng);
  getMaxLng(lgdtm.timeOut,   maxtimeOutLng);
  }


int LogDatum::report() {
  notePad << firstName << _T(" ");
  notePad << lastName  << nTab;
  notePad << agency    << nTab;
  notePad << fcc       << nTab;
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


void Log211::output(Archive& ar) {
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



void LogDatum::output(String& line) {
  line =  firstName + Comma;
  line += lastName  + Comma;
  line += fcc       + Comma;
  line += id        + Comma;
  line += timeIn    + Comma;
  line += timeOut   + Comma;
  line += hrs       + Comma;
  line += agency    + Comma;
  line += remark;
  }


void LogDatum::clear() {
Date zero;
  fcc.clear();
  firstName.clear();
  lastName.clear();
  id.clear();
  dateIn    = zero;
  dateOut   = zero;
  agency.clear();
  remark.clear();
  visitor   = false;
  rosterIn  = 0;
  rosterOut = 0;
  noChkOut  = false;
  hours     = 0.0;
  timeIn.clear();
  timeOut.clear();
  hrs.clear();
  }


void LogDatum::copy(LogDatum& lgdtm) {
  fcc       = lgdtm.fcc;
  firstName = lgdtm.firstName;
  lastName  = lgdtm.lastName;
  id        = lgdtm.id;
  dateIn    = lgdtm.dateIn;
  dateOut   = lgdtm.dateOut;
  agency    = lgdtm.agency;
  remark    = lgdtm.remark;
  rosterIn  = lgdtm.rosterIn;
  rosterOut = lgdtm.rosterOut;
  noChkOut  = lgdtm.noChkOut;
  hours     = lgdtm.hours;
  timeIn    = lgdtm.timeIn;
  timeOut   = lgdtm.timeOut;
  hrs       = lgdtm.hrs;
  }



#if 0
Date Log211::getMedianCheckOut() {
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
#endif

#if 0
void Log211::checkOutDefauters() {
Date            median    = getMedianCheckOut();
String          medChkOut = median.getDate() + _T("  ") + median.getHHMM();
int             pos;
int             pos1;
DefaultersDlg dlg;
int             n = data.end();
int             i;
String          s;
int             lng;

  pos = medChkOut.find(_T(':'));   pos1 = medChkOut.find(_T(':'), pos+1);
  if (pos >= 0) medChkOut = medChkOut.substr(0, pos1);

  dlg.medcs = medChkOut;

  dlg.date = median.getDate();

  for (i = 0; i < n; i++) {
    LogDatum& lgdtm = data[i];

    if (lgdtm.hours < 0.25) {
      AttdDsc& dsc = dlg.attendees[dlg.attendees.end()];
      s   = lgdtm.fcc;   lng = s.length();    s += spc.substr(0, 9-lng);

      s += lgdtm.firstName + _T(' ') + lgdtm.lastName;   dsc.key = s;   dsc.i = i;
      }
    }

  if (dlg.DoModal() == IDOK) {
    for (i = 0, n = dlg.attendees.end(); i < n; i++) {
      AttdDsc& dsc = dlg.attendees[i];
      LogDatum&  lgdtm = data[dsc.i];

      if (dsc.chkOutTm >= lgdtm.dateIn) {lgdtm.dateOut = dsc.chkOutTm; add(lgdtm);}
      }

    getTotalHours();
    }
  }


void Log211::editLogEntry() {

  }


void Log211::add(LogDatum& lgdtm) {
Datum dtm;

  dtm.fcc       = lgdtm.fcc;
  dtm.firstName = lgdtm.firstName;
  dtm.lastName  = lgdtm.lastName;
  dtm.id        = lgdtm.id;
  dtm.agency    = lgdtm.agency;
  dtm.dt        = lgdtm.dateOut;
  dtm.visitor   = lgdtm.visitor;
  roster.add(dtm);
  }
#endif

