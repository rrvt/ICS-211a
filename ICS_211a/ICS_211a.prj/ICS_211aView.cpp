// ICS_211aView.cpp : implementation of the ICS_211aView class


#include "stdafx.h"
#include "ICS_211aView.h"
#include "ICS_211a.h"
#include "ICS_211aDoc.h"
#include "IniFileEx.h"
#include "Log211.h"
#include "Members.h"
#include "OptionsDlg.h"
#include "ManagePrinter.h"
#include "Resource.h"
#include "ResourceData.h"
#include "Roster.h"
#include "RptOrientDlgFour.h"


//static TCchar* RptOrietnSect = _T("ReportOrientn");
//static TCchar* NoteOrietnKey = _T("NotePad");
static TCchar* LogOrietnKey  = _T("Log");
static TCchar* RstrOrietnKey = _T("Roster");
static TCchar* MbrOrietnKey  = _T("Member");


IMPLEMENT_DYNCREATE(ICS_211aView, CScrView)

BEGIN_MESSAGE_MAP(ICS_211aView, CScrView)
  ON_COMMAND(  ID_Options,     &onOptions)
  ON_COMMAND(  ID_Orientation, &onRptOrietn)

  ON_EN_CHANGE(IDC_Recv,       &OnChangeBarCode)
  ON_EN_CHANGE(IDC_Sink,       &OnChangeSink)
  ON_WM_SETFOCUS()
END_MESSAGE_MAP()


ICS_211aView::ICS_211aView() noexcept : dspRoster( dMgr.getNotePad()),
                                        prtRoster( pMgr.getNotePad()),
                                        dspMembers(dMgr.getNotePad()),
                                        prtMembers(pMgr.getNotePad()),
                                        dspLog211( dMgr.getNotePad()),
                                        prtLog211( pMgr.getNotePad()),
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


void ICS_211aView::onOptions() {
OptionsDlg dlg;

  if (printer.name.isEmpty()) printer.load(0);

  if (dlg.DoModal() == IDOK) pMgr.setFontScale(printer.scale);
  }


void ICS_211aView::initRptOrietn() {
  dspLog211.prtrOrietn  = prtLog211.prtrOrietn  =
                                   (PrtrOrient) iniFile.readInt(RptOrietnSect, LogOrietnKey,  PortOrient);
  dspMembers.prtrOrietn = prtMembers.prtrOrietn =
                                   (PrtrOrient) iniFile.readInt(RptOrietnSect, MbrOrietnKey,  PortOrient);
  dspRoster.prtrOrietn  = prtRoster.prtrOrietn  =
                                   (PrtrOrient) iniFile.readInt(RptOrietnSect, RstrOrietnKey, PortOrient);
  }


void ICS_211aView::onRptOrietn() {
RptOrietnDlg dlg;

  dlg.lbl01 = _T("ICS-211a Log:");
  dlg.lbl02 = _T("Roster:");
  dlg.lbl03 = _T("Member Info:");


  dlg.ntpd = printer.toStg(prtNote.prtrOrietn);
  dlg.rpt1  = printer.toStg(prtLog211.prtrOrietn);
  dlg.rpt2 = printer.toStg(prtRoster.prtrOrietn);
  dlg.rpt3  = printer.toStg(prtMembers.prtrOrietn);

  if (dlg.DoModal() == IDOK) {
    dspNote.prtrOrietn    = prtNote.prtrOrietn    = printer.toOrient(dlg.ntpd);
    dspLog211.prtrOrietn  = prtLog211.prtrOrietn  = printer.toOrient(dlg.rpt1);
    dspMembers.prtrOrietn = prtMembers.prtrOrietn = printer.toOrient(dlg.rpt2);
    dspRoster.prtrOrietn  = prtRoster.prtrOrietn  = printer.toOrient(dlg.rpt3);
    saveNoteOrietn();   saveRptOrietn();
    }
  }


void ICS_211aView::saveRptOrietn() {
  iniFile.write(RptOrietnSect, LogOrietnKey,  (int) prtLog211.prtrOrietn);
  iniFile.write(RptOrietnSect, RstrOrietnKey, (int) prtRoster.prtrOrietn);
  iniFile.write(RptOrietnSect, MbrOrietnKey,  (int) prtMembers.prtrOrietn);
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


void ICS_211aView::onPreparePrinting(CPrintInfo* info) {
  switch(doc()->dataSrc()) {
    case NotePadSrc : prtNote.onPreparePrinting(info);    break;
    case MemberSrc  : prtMembers.onPreparePrinting(info); break;
    case Log211Src  : prtLog211.onPreparePrinting(info);  break;
    case RosterSrc  : prtRoster.onPreparePrinting(info);  break;
    }
  }



// Perpare output (i.e. report) then start the output with the call to SCrView

void ICS_211aView::onBeginPrinting() {

  switch(doc()->dataSrc()) {
    case NotePadSrc : prtNote.onBeginPrinting(*this);    break;
    case MemberSrc  : prtMembers.onBeginPrinting(*this); break;
    case Log211Src  : prtLog211.onBeginPrinting(*this);  break;
    case RosterSrc  : prtRoster.onBeginPrinting(*this);  break;
    }
  }



void ICS_211aView::onDisplayOutput() {
NotePad& np = dMgr.getNotePad();

  switch(doc()->dataSrc()) {
    case NotePadSrc : dspNote.display(*this);    break;

    case MemberSrc  : dspMembers.display(*this); break;

    case Log211Src  : setArchiveAttr(np, prtLog211.prtrOrietn, 1.30);
                      dspLog211.display(*this);  break;

    case RosterSrc  : setArchiveAttr(np, printer.orient, 1.30);
                      dspRoster.display(*this);
                      doc()->startBarCodeRead(); break;
    }
  }


#if 0
void ICS_211aView::displayFooter(DevStream& dev) {

  switch(doc()->dataSrc()) {
    case Log211Src  : dspLog211.dspFooter();
    }
  }
#endif



void ICS_211aView::setArchiveAttr(NotePad& np, PrtrOrient orient, double f)
                                  {int w = orient == PortOrient ? 106 : 128;   np.setArchiveAttr(w, f);}


void ICS_211aView::printHeader(DevStream& dev, int pageNo) {

  switch(doc()->dataSrc()) {
    case NotePadSrc : prtNote.prtHeader(dev, pageNo);   break;
    case MemberSrc  : prtMembers.prtHeader(dev, pageNo); break;
    case Log211Src  : prtLog211.prtHeader(dev, pageNo); break;
    case RosterSrc  : prtRoster.prtHeader(dev, pageNo); break;
    }
  }


// The footer is injected into the printed output, so the output goes directly to the device.
// The output streaming functions are very similar to NotePad's streaming functions so it should not
// be a great hardship to construct a footer.

void ICS_211aView::printFooter(DevStream& dev, int pageNo) {

  switch(doc()->dataSrc()) {
    case NotePadSrc : prtNote.prtFooter(dev, pageNo);  break;
    case MemberSrc  : prtMembers.prtFooter(dev, pageNo); break;
    case Log211Src  : prtLog211.prtFooter(dev, pageNo); break;
    case RosterSrc  : prtRoster.prtFooter(dev, pageNo); break;
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







    //  initNoteOrietn();   dlg.orient = printer.toStg(prtNote.prtrOrietn);
//    prtNote.prtrOrietn = printer.toOrient(dlg.orient);   saveNoteOrietn();

