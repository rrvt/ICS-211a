// Validate Bar Code


#include "stdafx.h"
#include "Validate.h"


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

