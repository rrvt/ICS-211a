// Defaulters -- Person that do not check out, fixing them


#include "stdafx.h"
#include "Defaulters.h"
#include "DefaultersDlg.h"
#include "Utilities.h"


static String spc = _T("                                ");



void Defaulters::operator() () {
DefaultersDlg dlg;
LogIter       iter(log211);
LogDatum*     lgdtm;
Date          median    = log211.getMedianCheckOut();
String        medChkOut = median.getDate() + _T("  ") + median.getHHMM();
int           pos;
int           pos1;
String        s;
String        t;
int           maxName = 0;

  pos = medChkOut.find(_T(':'));   pos1 = medChkOut.find(_T(':'), pos+1);
  if (pos >= 0) medChkOut = medChkOut.substr(0, pos1);

  dlg.medcs = medChkOut;

  dlg.medDate = median.getDate();

  for (lgdtm = iter(); lgdtm; lgdtm = iter++) {
    if (lgdtm->hours >= 0.0825) continue;
    s = lgdtm->firstName + _T(' ') + lgdtm->lastName;  maxLng(s, maxName);
    }

  for (lgdtm = iter(); lgdtm; lgdtm = iter++) {

    if (lgdtm->hours >= 0.0825) continue;

    AttdDsc& dsc = dlg.attendees.nextData();

    s   = addSepTab(lgdtm->callSign, 9);

    t = lgdtm->firstName + _T(' ') + lgdtm->lastName;   s += addSepTab(t, maxName);

    s+= lgdtm->timeIn;   dsc.key = s;   dsc.lgdtm = lgdtm;   //dsc.chkInTm = lgdtm->checkIn;
    }

  if (dlg.DoModal() == IDOK) {
    DftrIter iter(dlg);
    AttdDsc* dsc;

    for (dsc = iter(); dsc; dsc = iter++) {

      if (!dsc->chkOutTm.getSeconds()) continue;

      lgdtm = dsc->lgdtm;  if (!lgdtm) continue;

      if (lgdtm->rosterOut) updateRoster(dsc);

      else if (dsc->chkOutTm >= lgdtm->checkIn) {
        lgdtm->checkOut = dsc->chkOutTm; roster.add(lgdtm, dsc->chkOutTm);
        }
      }
    }
  }


void Defaulters::updateRoster(AttdDsc* dsc) {
LogDatum* lgdtm     = dsc->lgdtm;
Datum*    rosterOut = lgdtm->rosterOut;

  if (rosterOut) {
    String dt = dsc->chkOutTm.getDate();
    String tm = dsc->chkOutTm.getTime();

    rosterOut->setDate(dt, tm);
    }
  }

