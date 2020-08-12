// ICS_211aView.cpp : implementation of the ICS_211aView class


#include "stdafx.h"
#include "ICS_211aView.h"
#include "ICS_211a.h"
#include "ICS_211aDoc.h"
#include "Options.h"
#include "Report.h"
#include "Resource.h"


// ICS_211aView

IMPLEMENT_DYNCREATE(ICS_211aView, CScrView)

BEGIN_MESSAGE_MAP(ICS_211aView, CScrView)
  ON_EN_CHANGE(IDC_Recv,            &ICS_211aView::OnChangeBarCode)
  ON_EN_CHANGE(IDC_Sink,            &ICS_211aView::OnChangeSink)
  END_MESSAGE_MAP()


void ICS_211aView::OnInitialUpdate() {
RECT editRect = {-10, 0,   0, 20};
RECT sinkRect = {-20, 0, -10, 20};
RECT r;

  GetUpdateRect(&r);

  editBox.create(20, editRect, this, IDC_Recv);
  sink.create(   20, sinkRect, this, IDC_Sink);

  CScrView::OnInitialUpdate();
  }



void ICS_211aView::OnChangeBarCode() {
  changeCount++;

  if (changeCount > 8) {
    editBox.getLine(line);

    int pos = line.find(_T('.'));

    if (pos > 0) {
      line = line.substr(0, pos);   roster.addBarcode(line);

      changeCount = 0;   editBox.clear();   editBox.SetWindowText(_T(""));   editBox.setFocus();

      invalidate();
      }
    }
  }


void ICS_211aView::OnChangeSink() {
  changeCount++;

  if (changeCount > 8) {
    sink.getLine(line);
    int pos = line.find(_T('.'));
    if (pos > 0) {changeCount = 0;   sink.clear();   sink.SetWindowText(_T(""));   sink.setFocus();}
    }
  }


void ICS_211aView::OnBeginPrinting(CDC* pDC, CPrintInfo* pInfo) {

  setPrntrOrient(theApp.getDevMode(), pDC);

  CScrView::OnBeginPrinting(pDC, pInfo);
  }




BOOL ICS_211aView::OnPreparePrinting(CPrintInfo* pInfo) {

  return CScrView::OnPreparePrinting(pInfo);
  }


void ICS_211aView::OnPrepareDC(CDC* pDC, CPrintInfo* pInfo) {
uint   x;
double topMgn   = options.topMargin.stod(x);
double leftMgn  = options.leftMargin.stod(x);
double rightMgn = options.rightMargin.stod(x);
double botMgn   = options.botMargin.stod(x);

  if (pDC->IsPrinting()) {setHorzMgns(leftMgn,  rightMgn);  setVertMgns(topMgn,  botMgn);}
  else                   {setHorzMgns(0.33, 0.33); setVertMgns(0.33, 0.33);}

  CScrView::OnPrepareDC(pDC, pInfo);
  }


// Perpare output (i.e. report) then start the output with the call to SCrView

void ICS_211aView::onPrepareOutput() {
  report(isPrinting() ? PrintingFn : displayReport ? ReportFn : DisplayFn);
  CScrView::onPrepareOutput();
  }


// The footer is injected into the printed output, so the output goes directly to the device.
// The output streaming functions are very similar to NotePad's streaming functions so it should not
// be a great hardship to construct a footer.

void ICS_211aView::printFooter(Display& dev, int pageNo) {report.footer(dev, pageNo); invalidate();}




// ICS_211aView diagnostics

#ifdef _DEBUG

void ICS_211aView::AssertValid() const {CScrollView::AssertValid();}

void ICS_211aView::Dump(CDumpContext& dc) const {CScrollView::Dump(dc);}
                                             // non-debug version is inline
ICS_211aDoc* ICS_211aView::GetDocument() const
  {ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(ICS_211aDoc))); return (ICS_211aDoc*)m_pDocument;}

#endif //_DEBUG
