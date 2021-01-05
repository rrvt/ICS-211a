// NotePad Report Base -- Logic to output the NotePad content to the display and/or printer


#include "stdafx.h"
#include "NoteRptB.h"
#include "CScrView.h"
#include "NotePad.h"


void NoteRptB::create() {
NtPdIter iter(notePad);
Note*    note;
int      i;

  np.clear();   noLines = BigNmbr;                      // Skip first header

  for (i = 0, note = iter(); note; i++, note = iter++) {

    if (noLines + 1 > maxLines) {

      if (i) np << nEndPage;

      noLines = header(np, printing);  setTabs(np);
      }

    Note& n = *note->alloc();   n = *note;

    if (np.append(&n)) noLines += 1;
    }
  }


void NoteRptB::setTabs(NotePad& ntpd) {ntpd << nClrTabs;}


int NoteRptB::header(NotePad& np, bool printing) {
Date   dt;
String s;

  if (!printing) return 0;

  dt.getToday();   s = dt.getDate() + _T(" ") + dt.getHHMM();

  np << title << nRight << s << nCrlf << nCrlf;   return 2;
  }


void NoteRptB::footer(Device& dev, int pageN) {

  if (pageN > maxPages) maxPages = pageN;

  dev << dCenter << pageN << _T(" of ") << maxPages << dflushFtr;
  }

