// ICS_211aView.cpp : implementation of the ICS_211aView class


#include "stdafx.h"
#include "ICS_211aView.h"
#include "ICS_211a.h"
#include "ICS_211aDoc.h"
#include "Log211.h"
#include "Members.h"
#include "Options.h"
#include "PrintMgr.h"
#include "Resources.h"
#include "Roster.h"


// ICS_211aView

IMPLEMENT_DYNCREATE(ICS_211aView, CScrView)

BEGIN_MESSAGE_MAP(ICS_211aView, CScrView)
  ON_EN_CHANGE(IDC_Recv,            &ICS_211aView::OnChangeBarCode)
  ON_EN_CHANGE(IDC_Sink,            &ICS_211aView::OnChangeSink)
  ON_WM_SETFOCUS()
END_MESSAGE_MAP()


ICS_211aView::ICS_211aView() noexcept : dspNote( nMgr.getNotePad()), prtNote( pMgr.getNotePad()),
                                        dspRoster( dMgr.getNotePad()), prtRoster( pMgr.getNotePad()),
                                        dspMembers(dMgr.getNotePad()), prtMembers(pMgr.getNotePad()),
                                        dspLog211( dMgr.getNotePad()), prtLog211( pMgr.getNotePad()),
                                        editBox(), sink(), changeCount(0) {
ResourceData res;
String       pn;
  if (res.getProductName(pn)) prtNote.setTitle(pn);
  }



BOOL ICS_211aView::PreCreateWindow(CREATESTRUCT& cs) {return CScrView::PreCreateWindow(cs);}


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

      doc()->display(RosterSrc);;
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


void ICS_211aView::OnPrepareDC(CDC* pDC, CPrintInfo* pInfo) {
uint   x;
double topMgn   = options.topMargin.stod(x);
double leftMgn  = options.leftMargin.stod(x);
double rightMgn = options.rightMargin.stod(x);
double botMgn   = options.botMargin.stod(x);

  setIsNotePad(!notePad.isEmpty() || doc()->dataSrc() == NoteSource);

  setMgns(leftMgn,  topMgn,  rightMgn, botMgn, pDC);   CScrView::OnPrepareDC(pDC, pInfo);
  }


// Perpare output (i.e. report) then start the output with the call to SCrView

void ICS_211aView::onPrepareOutput(bool isNotePad, bool printing) {
DataSource ds = isNotePad ? NoteSource : doc()->dataSrc();

  switch (printing) {
    case true : switch(ds) {
                  case NoteSource : prtNote.print(*this);    break;
                  case MemberSrc  : prtMembers.print(*this); break;
                  case Log211Src  : prtLog211.print(*this);  break;
                  case RosterSrc  : prtRoster.print(*this);  break;
                  default         : break;
                  }
                break;

    case false: switch(ds) {
                  case NoteSource : dspNote.display(*this);    break;
                  case MemberSrc  : dspMembers.display(*this); break;
                  case Log211Src  : dspLog211.display(*this);  break;
                  case RosterSrc  : dspRoster.display(*this);  doc()->startBarCodeRead(); break;
                  default         : break;                      //
                  }
                break;
    }

  CScrView::onPrepareOutput(isNotePad, printing);
  }


void ICS_211aView::OnBeginPrinting(CDC* pDC, CPrintInfo* pInfo) {

  theApp.setTitle(_T("Printing"));

  switch(doc()->dataSrc()) {
    case MemberSrc: setOrientation(options.mbrInfoOrient); break;
    case Log211Src: setOrientation(options.logOrient);     break;
    case RosterSrc: setOrientation(options.rstrOrient);    break;
    default       : break;
    }

  setPrntrOrient(theApp.getDevMode(), pDC);

  CScrView::OnBeginPrinting(pDC, pInfo);
  }


// The footer is injected into the printed output, so the output goes directly to the device.
// The output streaming functions are very similar to NotePad's streaming functions so it should not
// be a great hardship to construct a footer.

void ICS_211aView::printFooter(Display& dev, int pageNo) {

  switch(doc()->dataSrc()) {
    case NoteSource : prtNote.footer(dev, pageNo);  break;
    case MemberSrc  : prtMembers.footer(dev, pageNo); break;
    case Log211Src  : prtLog211.footer(dev, pageNo); break;
    case RosterSrc  : prtRoster.footer(dev, pageNo); break;
    }
  }


void ICS_211aView::OnEndPrinting(CDC* pDC, CPrintInfo* pInfo) {

  CScrView::OnEndPrinting(pDC, pInfo);

  switch(doc()->dataSrc()) {
    case Log211Src: stopBarcode();             break;
    default       : doc()->startBarCodeRead(); break;
    }

  invalidate();
  }


void ICS_211aView::OnSetFocus(CWnd* pOldWnd) {

  CScrView::OnSetFocus(pOldWnd);

  switch(doc()->dataSrc()) {
    case Log211Src: stopBarcode();             break;
    default       : doc()->startBarCodeRead(); break;
    }
  }


// ICS_211aView diagnostics

#ifdef _DEBUG

void ICS_211aView::AssertValid() const {CScrollView::AssertValid();}

void ICS_211aView::Dump(CDumpContext& dc) const {CScrollView::Dump(dc);}
                                             // non-debug version is inline
ICS_211aDoc* ICS_211aView::GetDocument() const
  {ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(ICS_211aDoc))); return (ICS_211aDoc*)m_pDocument;}

#endif //_DEBUG

