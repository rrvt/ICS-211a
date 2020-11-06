// Scroll View


#pragma once
//#include "DisplayDev.h"
#include "DsplyMgr.h"
#include "NoteMgr.h"
#include "PrintMgr.h"


extern const int BigNmbr;


class CScrView : public CScrollView {

static int lastPos;

protected:

NoteMgr    nMgr;
DsplyMgr   dMgr;
PrintMgr   pMgr;

public:

  CScrView() : nMgr(*this), dMgr(*this), pMgr(*this) { }
 ~CScrView() { }

          void     setFont(TCchar* f, int points = 120);
          void     setMgns(double left, double top, double right, double bot, CDC* dc);

  virtual void     OnUpdate(CView* pSender, LPARAM lHint, CObject* pHint);   // Override

  virtual void     OnPrepareDC(CDC* dc, CPrintInfo* info = NULL);           // Override

  // Override to prepare NotePad output, then call CScrView::onPrepareOutput to start the display/printer
  // output

  virtual void     onPrepareOutput(bool printing = false);

  virtual void     OnDraw(CDC* pDC) {if (nMgr.isEmpty()) dMgr.OnDraw(pDC); else nMgr.OnDraw(pDC);}
                                                                            // Override to draw this view
          void     invalidate() {Invalidate();}

          void     setOrientation(PrtrOrient orientation) {pMgr.setOrientation(orientation);}
          bool     setPrntrOrient(HANDLE h, CDC* dc = 0) {return pMgr.setOrient(h, dc);}

          int      noLinesPrPg() {return pMgr.noLinesPrPg();}     // Determine no lines per printed page

          void     suppressOutput() {pMgr.suppressOutput();}
          void     negateSuppress() {pMgr.negateSuppress();}
          void     disableWrap()    {pMgr.disableWrap();}
          void     enableWrap()     {pMgr.enableWrap();}

  // Printer Overrides

  virtual void     trialRun(int& maxLines, int& noPages) {pMgr.trialRun(maxLines, noPages);}
  virtual void     printFooter(Display& dev, int pageNo) {pMgr.printFooter(dev, pageNo);}

protected:

  virtual void     OnPrint(CDC* dc, CPrintInfo* info) {pMgr.OnPrint(dc, info);}       // Override

  virtual void     OnEndPrinting(CDC* dc, CPrintInfo* info)
                                                  {CScrollView::OnEndPrinting(dc, info); pMgr.endPrint();}

  virtual BOOL     OnPreparePrinting(CPrintInfo* info) {return pMgr.OnPreparePrinting(info);} // Override

private:

  virtual BOOL     OnScroll(UINT nScrollCode, UINT nPos, BOOL bDoScroll = TRUE);    // Override

protected:

  DECLARE_MESSAGE_MAP()
  };





