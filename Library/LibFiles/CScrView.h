// Scroll View


#pragma once
#include "DsplyMgr.h"
#include "NoteMgr.h"
#include "PrintMgr.h"


extern const int BigNmbr;


class CScrView : public CScrollView {

static int lastPos;
bool       isNP;

protected:

NoteMgr    nMgr;
DsplyMgr   dMgr;
PrintMgr   pMgr;

public:

  CScrView() : isNP(false), nMgr(*this), dMgr(*this), pMgr(*this) { }
 ~CScrView() { }

          void     setIsNotePad(bool isNotePad) {isNP = isNotePad;}
          void     setFont(TCchar* f, int points = 120);
          void     setMgns(double left, double top, double right, double bot, CDC* dc);

  virtual void     OnUpdate(CView* pSender, LPARAM lHint, CObject* pHint);   // Override

  virtual void     OnPrepareDC(CDC* dc, CPrintInfo* info = NULL);           // Override

  // Override to prepare NotePad output, then call CScrView::onPrepareOutput to start the display/printer
  // output

  virtual void     onPrepareOutput(bool isNotePad, bool printing = false);

  virtual void     OnDraw(CDC* pDC) {if (isNP) nMgr.OnDraw(pDC); else dMgr.OnDraw(pDC);}
                                                                            // Override to draw this view
          void     invalidate() {Invalidate();}

          void     setOrientation(PrtrOrient orientation) {pMgr.setOrientation(orientation);}
          bool     setPrntrOrient(HANDLE h, CDC* dc = 0) {return pMgr.setOrient(h, dc);}

          int      noLinesPrPg() {return pMgr.noLinesPrPg();}     // Determine no lines per printed page

          void     suppressOutput(bool printing);
          void     negateSuppress(bool printing);
          void     disableWrap(bool printing);
          void     enableWrap(bool printing);
          Display& getDev(bool printing);

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





