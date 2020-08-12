// ICS_211aView.h : interface of the ICS_211aView class


#pragma once
#include "CScrView.h"
#include "EditBox.h"


class ICS_211aDoc;


class ICS_211aView : public CScrView {

bool displayReport;

protected: // create from serialization only

  ICS_211aView() noexcept : displayReport(false), editBox(), sink(), changeCount(0) { }
  DECLARE_DYNCREATE(ICS_211aView)

public:

EditBox editBox;
EditBox sink;
int     changeCount;
String  line;

  virtual ~ICS_211aView() { }


  void        startBarcode() {editBox.setFocus();}
  void        stopBarcode()  {sink.setFocus();}

          void dsplyReport(bool start = true) {displayReport = start;}

  virtual void OnPrepareDC(CDC* pDC, CPrintInfo* pInfo = NULL);
  virtual void onPrepareOutput();
  virtual void printFooter(Display& dev, int pageNo);

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
  virtual void OnBeginPrinting(CDC* pDC, CPrintInfo* pInfo);
  virtual BOOL OnPreparePrinting(CPrintInfo* pInfo);
  };


#ifndef _DEBUG  // debug version in ICS_211aView.cpp
inline ICS_211aDoc* ICS_211aView::GetDocument() const
   { return reinterpret_cast<ICS_211aDoc*>(m_pDocument); }
#endif

