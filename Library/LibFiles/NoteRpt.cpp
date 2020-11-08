// NotePad Report -- Logic to output the NotePad content to the display and/or printer


#include "stdafx.h"
#include "NoteRpt.h"
#include "CScrView.h"
#include "NotePad.h"

#if 0
void NoteRpt::print(CScrView& vw) {

  maxLines = vw.noLinesPrPg();

  vw.disableWrap();

  detNoPages(vw);

  create();
  }
#endif


void NoteRptB::create() {
NotePadLoop  iter(notePad);
Note*        note;
int          i;

  np.clear();   noLines = BigNmbr;                      // Skip first header

  for (i = 0, note = iter(); note; i++, note = iter++) {

    if (noLines + 1 > maxLines) {

      if (i) np << nEndPage;

      header();  np << nClrTabs << nSetTab(10);
      }

    Note& n = *note->alloc();   n = *note;   if (np.append(&n)) noLines += 1;
    }
  }


void NoteRptB::header() {
Date   dt;
String s;

  if (!printing) {noLines = 0; return;}

  dt.getToday();   s = dt.getDate() + _T(" ") + dt.getHHMM();

  np << title << nRight << s << nCrlf << nCrlf;   noLines =  2;
  }


void NoteRptB::footer(Display& dev, int pageN) {

  if (pageN > maxPages) maxPages = pageN;

  dev << dCenter << pageN << _T(" of ") << maxPages << dflushFtr;
  }

