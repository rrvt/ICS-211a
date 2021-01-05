// Capture Data for Debugging


#pragma once
#include <mutex>


struct CapData {
String  key;
int     w;
CFont*  f;
CDC*    dc;
DWORD   threadID;
#if 0
LONG    height;
LOGFONT logFont;     w(0), height(0),
#endif

  CapData() : w(0), f(0), threadID(0), dc(0) { }
  };


class Capture {
int     n;
CapData data[50];
mutex   capLck;

public:

  Capture() : n(0) { }

  void operator() (TCchar* ky, CDC* dc = 0);

private:

  bool inData(CapData& dtm);
  };


extern Capture capture;
