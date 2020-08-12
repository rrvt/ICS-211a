// Utilities


#pragma once


String addTab(String& s, int max);

String normalizeDate(TCchar* date);
String normalizeTime(TCchar* time);

String nextTok(String& s, int& pos);

String getDateNow();
String getTimeNow();

inline void getMaxLng(String& s, int& max) {int lng = s.length();  if (lng > max) max = lng;}

// Display a string of no more than noChars.  If more, the insert noTabs and no more than noChars until
// string exhausted.  returns number of lines output

//int   dsplyWrap(TCchar* p, int noChars, int noTabs);


bool validateBarCode(String& line);

