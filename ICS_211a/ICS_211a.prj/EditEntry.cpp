// Edit Entry -- Backup when things don't work out.


#include "stdafx.h"
#include "EditEntry.h"
#include "EditEntryDlg.h"
#include "Log211.h"
#include "Roster.h"


bool EditEntry::operator() () {
EditEntryDlg dlg;
LogDatum*    lgdtm;
Datum*       rosterIn;
Datum*       rosterOut;
String       s;
Date         date;

  if (dlg.DoModal()) {
    lgdtm = dlg.lgdtm;             if (!lgdtm) return false;
    rosterIn  = lgdtm->rosterIn;   if (!rosterIn) return false;
    rosterOut = lgdtm->rosterOut;

    s = dlg.chkInDt  + _T(' ') + dlg.chkInTm;   rosterIn->dt  = s;

    s = dlg.chkOutDt + _T(' ') + dlg.chkOutTm;  date = s;
    if (rosterOut) rosterOut->dt = date;
    else if (date > rosterIn->dt) roster.add(lgdtm, date);

    return true;
    }

  return false;
  }

