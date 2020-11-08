// ICS_211aView.h : interface of the ICS_211aView class


#pragma once
#include "CScrView.h"
#include "EditBox.h"
#include "Log211Rpt.h"
#include "MembersRpt.h"
#include "NoteRpt.h"
#include "RosterRpt.h"


class ICS_211aDoc;


class ICS_211aView : public CScrView {

NoteRptB   dspNote;
NoteRptB   prtNote;
RosterRpt  dspRoster;                     // The print roster object
RosterRpt  prtRoster;
MembersRpt dspMembers;
MembersRpt prtMembers;
Log211Rpt  dspLog211;
Log211Rpt  prtLog211;

protected: // create from serialization only

  ICS_211aView() noexcept;
  DECLARE_DYNCREATE(ICS_211aView)

public:

EditBox editBox;
EditBox sink;
int     changeCount;
String  line;

  virtual ~ICS_211aView() { }

  BOOL         PreCreateWindow(CREATESTRUCT& cs);

  void         startBarcode() {editBox.setFocus();}
  void         stopBarcode()  {sink.setFocus();}

  virtual void OnPrepareDC(CDC* pDC, CPrintInfo* pInfo = NULL);
  virtual void onPrepareOutput(bool isNotePad, bool printing = false);

  virtual void OnBeginPrinting(CDC* pDC, CPrintInfo* pInfo);
  virtual void printFooter(Display& dev, int pageNo);
  virtual void OnEndPrinting(CDC* pDC, CPrintInfo* pInfo);

  ICS_211aDoc* GetDocument() const;

public:

#ifdef _DEBUG
  virtual void AssertValid() const;
  virtual void Dump(CDumpContext& dc) const;
#endif

protected:

  virtual void OnInitialUpdate();

//  DECLARE_MESSAGE_MAP()
public:
  DECLARE_MESSAGE_MAP()

  afx_msg void OnChangeBarCode();
  afx_msg void OnChangeSink();
  afx_msg void OnSetFocus(CWnd* pOldWnd);
  };


#ifndef _DEBUG  // debug version in ICS_211aView.cpp
inline ICS_211aDoc* ICS_211aView::GetDocument() const
   { return reinterpret_cast<ICS_211aDoc*>(m_pDocument); }
#endif

