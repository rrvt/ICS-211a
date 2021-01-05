// Report Base


#pragma once

class CScrView;
class Device;
class NotePad;


class ReportBase {
protected:

NotePad& np;

String   title;
int      noLines;
int      maxLines;
int      maxPages;

bool     printing;

public:

  ReportBase(NotePad& notePad) : np(notePad), noLines(0), maxLines(0), maxPages(0), printing(false) { }
 ~ReportBase() { }

          void setTitle(TCchar* t) {title = t;}

          void display(CScrView& vw);
  virtual void print(CScrView& vw);
  virtual void footer(Device& dev, int pageNo);

protected:

  void detNoPages(CScrView& vw);

  virtual void create() = 0;
  virtual int  header(NotePad& np, bool printing) = 0;

private:

  ReportBase() : np(*(NotePad*)0) { }
  };

