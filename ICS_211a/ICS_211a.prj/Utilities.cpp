// Utilities


#include "stdafx.h"
#include "Utilities.h"
#include "NotePad.h"


static String blks = _T("                          ");

String addTab(String& s, int max) {
int   t  = s.length();
int   n  = max - t + 2;
Tchar ch = _T(',');

  if (!t) ch = _T(' ');    return s + ch + blks.substr(0, n);
  }


//String addTab(String& s, int max) {int n = max - s.length(); return s + blks.substr(0, n);}


String nextTok(String& s, int& pos) {
int bPos = pos;
int n    = s.length();
int ePos;

  ePos = s.find(_T(','), pos);   if (ePos < 0) ePos = s.find(_T('\n'));

  if (ePos < 0) return s.substr(bPos);

  for (pos = ePos+1; pos < n; pos++) if (s[pos] > _T(' ')) break;

  return s.substr(bPos, ePos-bPos);
  }


String normalizeDate(TCchar* date) {
String s  = date ? date : _T("");   if (s.isEmpty()) return s;
Date   dt = s;

  s = dt.format(_T("%D"));  return s;
  }

String normalizeTime(TCchar* time) {
String s  = time ? time : _T("");   if (s.isEmpty()) return s;
Date   dt = s;

  s = dt.format(_T("%R"));  return s;
  }


String getDateNow() {Date dt;   dt.getToday();   return dt.format(_T("%D"));}

String getTimeNow() {Date dt;   dt.getToday();   return dt.format(_T("%R"));}


time_t getDiff(Date& dtUpper, Date& dtLower)
                                   {CTimeSpan delta = dtUpper - dtLower; return delta.GetTotalSeconds();}



static int findLastWh(String& s, int noChars);

static int wrap(int i, int noTabs);




enum State   {BeginBr, PrefixCS, DigitCS, SuffixCS, SpaceBrk, DigitID};
enum ChClass {SpaceCC, LetterCC, DigitCC, OtherCC};


bool validateBarCode(String& line) {
int     n = line.length();
int     i;
State   state = BeginBr;
ChClass chClass;
Tchar   ch;
int     count = 0;

  for (i = 0; i < n; i++) {
    ch = line[i];

    if (ch <= _T(' '))                       chClass = SpaceCC;
    else if (_T('A') <= ch && ch <= _T('Z')) chClass = LetterCC;
    else if (_T('a') <= ch && ch <= _T('z')) chClass = LetterCC;
    else if (_T('0') <= ch && ch <= _T('9')) chClass = DigitCC;
    else                                     chClass = OtherCC;

    switch (state) {
      case BeginBr  : if (chClass == SpaceCC) continue;
                      if (chClass == LetterCC)  {count = 1; state = PrefixCS; continue;}
                      break;
      case PrefixCS : if (count > 2) break;
                      if (chClass == LetterCC)  {count++;   state = DigitCS;  continue;}
                      if (chClass == DigitCC)   {count = 0; state = SuffixCS; continue;}
                      break;
      case DigitCS  : if (chClass == DigitCC)   {count = 0; state = SuffixCS; continue;}
                      break;
      case SuffixCS : if (count > 3) break;
                      if (chClass == LetterCC)  {count++; continue;}
                      if (chClass == SpaceCC)   {count = 0; state = SpaceBrk; continue;}
                      break;
      case SpaceBrk : if (chClass == DigitCC)   {count = 1; state = DigitID; continue;}
                      if (chClass == SpaceCC)                                continue;
                      break;
      case DigitID  : if (chClass == DigitCC)   {count++; continue;}
                      if (chClass == SpaceCC && count > 1) return true;
                      break;
      }

    return false;
    }

  return true;
  }


