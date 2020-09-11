// Defaulters -- Person that do not check out, fixing them


#include "stdafx.h"
#include "Defaulters.h"
#include "DefaultersDlg.h"




static String spc = _T("                                ");



void Defaulters::operator() () {
Date          median    = log211.getMedianCheckOut();
String        medChkOut = median.getDate() + _T("  ") + median.getHHMM();
int           pos;
int           pos1;
DefaultersDlg dlg;
LogIter       iter(log211);
LogDatum*     lgdtm;
String        s;
int           lng;

  pos = medChkOut.find(_T(':'));   pos1 = medChkOut.find(_T(':'), pos+1);
  if (pos >= 0) medChkOut = medChkOut.substr(0, pos1);

  dlg.medcs = medChkOut;

  dlg.date = median.getDate();

  for (lgdtm = iter(); lgdtm; lgdtm = iter++) {

    if (lgdtm->hours < 0.25) {
      AttdDsc& dsc = dlg.attendees.nextData();

      s   = lgdtm->callSign;   lng = s.length();    s += spc.substr(0, 9-lng);

      s += lgdtm->firstName + _T(' ') + lgdtm->lastName;   dsc.key = s;   dsc.lgdtm = lgdtm;
      }
    }

  if (dlg.DoModal() == IDOK) {
    DftrIter iter(dlg);
    AttdDsc* dsc;

    for (dsc = iter(); dsc; dsc = iter++) {

      lgdtm = dsc->lgdtm;

      if (dsc->chkOutTm >= lgdtm->dateIn)
                                      {lgdtm->dateOut = dsc->chkOutTm; roster.add(lgdtm, dsc->chkOutTm);}
      }
    }
  }


