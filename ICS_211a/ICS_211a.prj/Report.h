// Report -- Essentially the output processing is separated from the file input/output processing


#pragma once
#include "Display.h"
#include "NotePad.h"
#include "ICS_211aView.h"


enum OutputFn {DisplayFn, ReportFn, PrintingFn};

class Report {
bool      printing;
int       noLines;
int       maxPages;
int       maxLines;
CTimeSpan totalTime;

double    totalHrs;

public:

  Report() : printing(false), noLines(999), maxPages(0), totalTime(0) { }
 ~Report() { }

  void operator() (OutputFn fn = DisplayFn);        // Determine the boundaries of the output medium then
                                                  // output the report
  void detNoLines(ICS_211aView& vw);               // Performs a dummy printer output to determine the
                                                  // number of lines on each page
  void detWraps(  ICS_211aView& vw);               // Wrap each entity in the report body

  void detNoPages(ICS_211aView& vw);               // Performs a dummy printer output to determine the
                                                  // number of pages that will be produced
  void footer(Display& dev, int pageNo);          // Output page Footer to NotePad
  void dspFtr();

private:

  void create();                                  // Create the complete report for the display, print
                                                  // preview or printer
  int  header();                                  // Output page heading to NotePad
  int  headerLines();
  int  body();                                    // Output report body to NotePad
  void setBodyTabs();
  void crlf() {notePad << nCrlf; noLines++;}      // Count lines for the report
  };


extern Report report;

