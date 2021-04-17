// Manage and Output Spread Sheet Data


#include "stdafx.h"
#include "Log211.h"
#include "ICS_211aView.h"


static TCchar Comma = _T(',');


Log211 log211;


bool Log211::prepare() {
OrganizeLog organize;

  roster.sort();  organize();  getTotalHours();   qsort(&data[0], &data[data.end()-1]);

  return data.end() > 0;
  }


void OrganizeLog::operator() () {
RstrIter iter(roster);
Datum*   dtm;
Datum*   next;
Date     inTime;
Date     outTime;
String   inDate;
String   outDate;

  data.clear();

  for (dtm = iter(); dtm; dtm = iter++) {

    if (dtm->dtmType != CheckInType) continue;

    LogDatum& lgdtm = data.nextData();

    lgdtm.clear();

    lgdtm.firstName = dtm->firstName;
    lgdtm.lastName  = dtm->lastName;
    lgdtm.callSign  = dtm->callSign;
    lgdtm.id        = dtm->id;
    lgdtm.agency    = dtm->agency;
    lgdtm.visitor   = dtm->visitor;

    inTime          = dtm->dt;
    lgdtm.rosterIn  = dtm;

    next = findNext(iter, lgdtm);

    if (next && next->dtmType == CheckOutType)
          {outTime = next->dt; lgdtm.noChkOut = false; lgdtm.rosterOut = next;}
    else  {outTime = inTime;   lgdtm.noChkOut = true;}

    if (lgdtm.noChkOut) {lgdtm.remark = _T("Did not sign out");}

    lgdtm.dateIn = inTime;   lgdtm.dateOut = outTime;

    inDate   = lgdtm.dateIn.getDate();
    outDate  = lgdtm.dateOut.getDate();
    dspDate |= inDate != outDate;
    }
  }


Datum* OrganizeLog::findNext(RstrIter& itr, LogDatum& lgdtm) {
RstrIter iter(itr);
bool     callSignSeen = !lgdtm.callSign.isEmpty();
Datum*   next;

  for (next = iter++; next; next = iter++) {

    if (callSignSeen) {if (next->callSign == lgdtm.callSign) return next;}

    else if (next->lastName == lgdtm.lastName && next->firstName == lgdtm.firstName) return next;
    }

  return 0;
  }




Date Log211::getMedianCheckOut() {
LogIter              iter(log211);
LogDatum*            lgdtm;
Expandable<Date, 32> dates;
int                  n;
Date                 d;

  for (lgdtm = iter(); lgdtm; lgdtm = iter++)
                                            if (lgdtm->dateIn < lgdtm->dateOut) dates += lgdtm->dateOut;
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



Date Log211::suggestDate(LogDatum* datum) {
String    date = datum->dateIn.getDate();
LogIter   iter(*this);
LogDatum* dtm;
time_t    ttl = 0;
int       n   = 0;

  for (dtm = iter(); dtm; dtm = iter++) {
    String dt = dtm->dateOut.getDate();

    if (dt == date) {ttl += dtm->dateOut.getSeconds();   n++;}
    }

  ttl /= n;

  if (ttl) {Date tm(ttl);    return tm;}

  Date now;  now.getToday();  return now;
  }



void Log211::getTotalHours() {
LogIter   iter(*this);
LogDatum* lgdtm;

  totalSecs = 0;   clrMaximums();

  for (lgdtm = iter(); lgdtm; lgdtm = iter++) {

    totalSecs += lgdtm->getSecs();

    getMaximums(*lgdtm);
    }
  }


time_t LogDatum::getSecs() {

  seconds = getDiff(dateOut, dateIn);

  hrs.format(_T("%.3f"), toHours(seconds));

  if (log211.dspDate) {
    timeIn  = dateIn.getDate() + _T("  ") + dateIn.getHHMMSS();
    timeOut = dateOut != dateIn ? dateOut.getDate() + _T("  ") + dateOut.getHHMMSS() : _T("");
    }
  else {
    timeIn  = dateIn.getHHMMSS();
    timeOut = dateOut != dateIn ? dateOut.getHHMMSS() : _T("");
    }

  return seconds;
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
  maxLng(lgdtm.firstName, maxfirstNameLng);
  maxLng(lgdtm.lastName,  maxlastNameLng);
  maxLng(lgdtm.callSign,  maxcallSignLng);
  maxLng(lgdtm.id,        maxidLng);
  maxLng(lgdtm.agency,    maxagencyLng);
  maxLng(lgdtm.remark,    maxremarkLng);
  maxLng(lgdtm.hrs,       maxhoursLng);
  maxLng(lgdtm.timeIn,    maxtimeInLng);
  maxLng(lgdtm.timeOut,   maxtimeOutLng);
  }


int LogDatum::report(NotePad& np) {
  np << firstName << _T(" ");
  np << lastName  << nTab;
  np << agency    << nTab;
  np << callSign  << nTab;
  np << timeIn    << nTab;
  np << timeOut   << nTab;
  np << hrs       << nTab;
  np << id;
  if (!id.isEmpty() && !remark.isEmpty()) np << _T(" - ");
  np << remark;
  np << nCrlf;
  return 1;
  }


static String spc = _T("         ");


void Log211::output(Archive& ar) {
String s;
int    n;
int    i;

  line = _T("Incident Name:,") + incidentName + _T(",,,,,");

  s.format(_T("%.2f,Total Hours"), toHours(totalSecs));   line += s;   outputLine(ar);

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
  line += callSign  + Comma;
  line += id        + Comma;
  line += timeIn    + Comma;
  line += timeOut   + Comma;
  line += hrs       + Comma;
  line += agency    + Comma;
  line += remark;
  }


void LogDatum::clear() {
Date zero;
  sortKey = 0;
  callSign.clear();
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
  seconds   = 0;
  timeIn.clear();
  timeOut.clear();
  hrs.clear();
  }


void LogDatum::copy(LogDatum& lgdtm) {
  sortKey   = lgdtm.sortKey;
  callSign  = lgdtm.callSign;
  firstName = lgdtm.firstName;
  lastName  = lgdtm.lastName;
  id        = lgdtm.id;
  dateIn    = lgdtm.dateIn;
  dateOut   = lgdtm.dateOut;
  agency    = lgdtm.agency;
  remark    = lgdtm.remark;
  visitor   = lgdtm.visitor;
  rosterIn  = lgdtm.rosterIn;
  rosterOut = lgdtm.rosterOut;
  noChkOut  = lgdtm.noChkOut;
  seconds   = lgdtm.seconds;
  timeIn    = lgdtm.timeIn;
  timeOut   = lgdtm.timeOut;
  hrs       = lgdtm.hrs;
  }

