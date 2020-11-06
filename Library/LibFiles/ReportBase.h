// Report Base


#pragma once

class CScrView;
class Display;
class NotePad;


class ReportBase {
protected:

NotePad& np;

int      noLines;
int      maxLines;
int      maxPages;

public:

  ReportBase(NotePad& notePad) : np(notePad), noLines(0), maxLines(0), maxPages(0) { }
 ~ReportBase() { }


          void display(CScrView& vw);
  virtual void print(CScrView& vw);
  virtual void footer(Display& dev, int pageNo);

protected:

  void detNoPages(CScrView& vw);

  virtual void create() = 0;
  virtual void header() = 0;

private:

  ReportBase() : np(*(NotePad*)0) { }
  };

