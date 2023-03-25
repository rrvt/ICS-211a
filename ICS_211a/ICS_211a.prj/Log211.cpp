// Manage and Output Spread Sheet Data


#include "stdafx.h"
#include "Log211.h"
#include "ICS_211aView.h"


static TCchar  Comma  = _T(',');
static TCchar* Remark = _T("Did not sign out");



Log211 log211;


bool Log211::prepare() {

  roster.sort();  organize();   qsort(&data[0], &data[data.end()-1]);

  return data.end() > 0;
  }


void Log211::organize() {
RstrIter iter(roster);
Datum*   dtm;

  data.clear();

  for (dtm = iter(); dtm; dtm = iter++) {

    if (dtm->dtmType != CheckInType) continue;

    LogDatum& lgdtm = data.nextData();   lgdtm.create(*dtm,  iter);
    }
  }


void LogDatum::create(Datum& dIn, RstrIter& iter) {
Datum* dOut;

  clear();

  rosterIn  = &dIn;
  firstName = dIn.firstName;
  lastName  = dIn.lastName;
  callSign  = dIn.callSign;
  id        = dIn.id;
  agency    = dIn.agency;
  visitor   = dIn.visitor;
  checkIn   = dIn.dt;

  rosterOut = dOut  = findNext(iter, *this);
  chkdOut   = dOut && dOut->dtmType == CheckOutType;
  checkOut  = chkdOut ? dOut->dt : checkIn;

  dateIn    = checkIn.getDate();
  timeIn    = checkIn.getHHMM();
  if (chkdOut) {
    dateOut   = checkOut.getDate();
    timeOut   = checkOut.getHHMM();
    }

  hours     = toHours(getDiff(checkOut, checkIn));

  hrs.format(_T("%.3f"), hours);
  }


Datum* LogDatum::findNext(RstrIter& itr, LogDatum& lgdtm) {
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
                                            if (lgdtm->checkIn < lgdtm->checkOut) dates += lgdtm->checkOut;
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


Date& Log211::suggestDate(LogDatum* datum) {
String    s   = datum->checkIn.getDate();
LogIter   iter(*this);
LogDatum* dtm;
Time64    ttl = 0;
int       n   = 0;

  for (dtm = iter(); dtm; dtm = iter++) {
    String dt = dtm->checkOut.getDate();

    if (dt == s) {ttl += dtm->checkOut.getSeconds();   n++;}
    }

  ttl /= n;

  if (ttl) {tmpDate = ttl;   return tmpDate;}

  tmpDate.getToday();  return tmpDate;
  }


bool Log211::includeDate() {
LogIter   iter(*this);
LogDatum* dtm;

  for (dtm = iter(); dtm; dtm = iter++) if (dtm->dateIn != dtm->dateOut) return true;

  return false;
  }


int LogDatum::report(NotePad& np, bool dspDate) {

  np << firstName << _T(" ") << lastName;
  np << nTab << agency;                                     // tab0
  np << nTab << callSign;                                   // tab1
  np << nTab;                                               // tab2
  if (dspDate) np << dateIn  << _T(' ');   np  << timeIn;
  np << nTab;                                               // tab3
  if (dspDate) np << dateOut << _T(' ');   np << timeOut;
  np << nTab << hrs;                                        // tab4
  np << nTab << id;                                         // tab5
  if (!chkdOut) np << nTab << Remark;                       // tab6

  np << nTab << nTab;   // Debugging

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
  line += dateIn  + _T(' ') + timeIn  + Comma;
  line += dateOut + _T(' ') + timeOut + Comma;
  line += hrs       + Comma;
  line += agency    + Comma;

  if (!chkdOut) line += _T("Did not sign out");     // tab6
  if (!chkdOut) line += Remark;
  }


void LogDatum::clear() {
Date zero;
  sortKey = 0;
  callSign.clear();
  firstName.clear();
  lastName.clear();
  id.clear();
  checkIn    = zero;
  checkOut   = zero;
  agency.clear();
  visitor   = false;
  chkdOut   = false;
  rosterIn  = 0;
  rosterOut = 0;
  hours     = 0;
  dateIn.clear();
  timeIn.clear();
  dateOut.clear();
  timeOut.clear();
  hrs.clear();
  }


void LogDatum::copy(LogDatum& lgdtm) {
  sortKey   = lgdtm.sortKey;
  callSign  = lgdtm.callSign;
  firstName = lgdtm.firstName;
  lastName  = lgdtm.lastName;
  id        = lgdtm.id;
  checkIn   = lgdtm.checkIn;
  checkOut  = lgdtm.checkOut;
  agency    = lgdtm.agency;
  visitor   = lgdtm.visitor;
  chkdOut   = lgdtm.chkdOut;
  rosterIn  = lgdtm.rosterIn;
  rosterOut = lgdtm.rosterOut;
  dateIn    = lgdtm.dateIn;
  timeIn    = lgdtm.timeIn;
  dateOut   = lgdtm.dateOut;
  timeOut   = lgdtm.timeOut;
  hours     = lgdtm.hours;
  hrs       = lgdtm.hrs;
  }


