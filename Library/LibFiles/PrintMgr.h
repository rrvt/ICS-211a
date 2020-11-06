// Print Manager -- needed to isolate variables from Display Management!


#pragma once
#include "ShowMgr.h"

class CScrView;


enum PrtrOrient {Portrait, Landscape};


class PrintMgr : public ShowMgr {

NotePad     npd;

CDC*        cdc;
CPrintInfo* pinfo;

PrtrOrient  orient;

bool        wrapEnabled;

bool        printing;
bool        startDocDone;
bool        outputDone;
bool        endPrinting;

String      leftFooter;               // Data to print at right side of footer
Date        date;                     // Date to print at left edge of footer

public:

  PrintMgr(CScrView& view);
 ~PrintMgr() { }

  void     clear();

  void     setOrientation(PrtrOrient orientation) {orient = orientation;}
  bool     setOrient(HANDLE h, CDC* dc);

  void     disableWrap()    {wrapEnabled = false;}
  void     enableWrap()     {wrapEnabled = true;}

  void     OnPrepareDC(CDC* dc, CPrintInfo* info);

  void     startDev() {if (!endPrinting) dev.startDev();}

  void     suppressOutput() {dev.suppressOutput();}
  void     negateSuppress() {dev.negateSuppress();}

  BOOL     OnPreparePrinting(CPrintInfo* info);

  int      noLinesPrPg();

  void     trialRun(int& maxLines, int& noPages);
  void     OnPrint(CDC* dC, CPrintInfo* info);
  void     printFooter(Display& dev, int pageNo);

  void     endPrint() {printing = false; clear();}

private:

  void     preparePrinter(  CDC* dc, CPrintInfo* info);
  void     preview(         CDC* dc, CPrintInfo* info);
  void     startFooter(Display& dev, CPrintInfo* pInfo);
  bool     isFinishedPrinting(CPrintInfo* info);

           PrintMgr() : ShowMgr(*(CScrView*)0, *(NotePad*)0) { }     //   , *(NotePad*)0
  };

