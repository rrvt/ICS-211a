// Capture Data for Debugging


#include "stdafx.h"
#include "Capture.h"


Capture capture;
static TCchar* sample = _T("Now is the time for all good people to come to the aid of the party");


void Capture::operator() (TCchar* ky, CDC* dc) {
CSize   sz;
Cstring cs = sample;
DWORD   threadID;
LOGFONT logFont;
bool    isLogFont = true;

  capLck.lock();

  if (n >= noElements(data)) {capLck.unlock(); return;}

  threadID = GetCurrentThreadId();

  if (!dc) {                                                      //dtm.w = 0; dtm.height = 0;
    CapData& dtm = data[n++];  dtm.key = ky;  dtm.threadID = threadID;  capLck.unlock();  return;
    }

  if (!dc->m_bPrinting) {capLck.unlock(); return;}

  CapData& dtm = data[n];

  dtm.key = ky;   dtm.threadID = threadID;    dtm.dc = dc;

  try {dtm.f = dc->GetCurrentFont();} catch (...) {dtm.f = (CFont*)-1;}

  if (dtm.f > 0) {
    try {dtm.f->GetLogFont(&logFont);} catch (...) {isLogFont = false;}

    if (isLogFont) {
      try {sz = dc->GetOutputTextExtent(cs); dtm.w = sz.cx;} catch (...) {dtm.w = 0;}
      }
    }

  if (!inData(dtm)) n++;

  capLck.unlock();
  }


bool Capture::inData(CapData& dtm) {
int i;

  for (i = 0; i < n; i++) {

    if (data[i].key != dtm.key) continue;

    if (data[i].threadID != dtm.threadID) continue;

    if (data[i].dc  != dtm.dc)  continue;

#if 0
    LOGFONT& lf = data[i].logFont;
    LOGFONT& tf = dtm.logFont;
    String   fc = tf.lfFaceName;

    if (lf.lfHeight         != tf.lfHeight)         continue;
    if (lf.lfWidth          != tf.lfWidth)          continue;
    if (lf.lfEscapement     != tf.lfEscapement)     continue;
    if (lf.lfOrientation    != tf.lfOrientation)    continue;
    if (lf.lfWeight         != tf.lfWeight)         continue;
    if (lf.lfItalic         != tf.lfItalic)         continue;
    if (lf.lfUnderline      != tf.lfUnderline)      continue;
    if (lf.lfStrikeOut      != tf.lfStrikeOut)      continue;
    if (lf.lfCharSet        != tf.lfCharSet)        continue;
    if (lf.lfOutPrecision   != tf.lfOutPrecision)   continue;
    if (lf.lfClipPrecision  != tf.lfClipPrecision)  continue;
    if (lf.lfQuality        != tf.lfQuality)        continue;
    if (lf.lfPitchAndFamily != tf.lfPitchAndFamily) continue;
    if (fc                  != lf.lfFaceName)       continue;
#endif
    return true;
    }

  return false;
  }

