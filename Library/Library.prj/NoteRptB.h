// NotePad Report -- Logic to output the NotePad content to the display and/or printer


#pragma once
#include "ReportBase.h"


class NoteRptB : public ReportBase {
public:

  NoteRptB(NotePad& notePad) : ReportBase(notePad) {title = _T("NotePad");}
 ~NoteRptB() { }

  virtual void footer(Device& dev, int pageNo);          // Output page Footer to NotePad

private:

  NoteRptB() : ReportBase(*(NotePad*)0) { }

  virtual void create();

protected:

  virtual int  header( NotePad& np, bool printing);
  virtual void setTabs(NotePad& ntpd);
  };
