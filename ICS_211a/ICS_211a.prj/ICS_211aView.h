// ICS_211aView.h : interface of the ICS_211aView class


#pragma once
#include "CScrView.h"
#include "EditBox.h"
#include "Log211Rpt.h"
#include "MembersRpt.h"
#include "RosterRpt.h"


class ICS_211aDoc;


class ICS_211aView : public CScrView {

Log211Rpt  dspLog211;
Log211Rpt  prtLog211;
MembersRpt dspMembers;
MembersRpt prtMembers;
RosterRpt  dspRoster;                     // The print roster object
RosterRpt  prtRoster;

protected: // create from serialization only

  ICS_211aView() noexcept;
  DECLARE_DYNCREATE(ICS_211aView)

public:

EditBox editBox;
EditBox sink;
int     changeCount;
String  line;

  virtual     ~ICS_211aView() { }

          BOOL PreCreateWindow(CREATESTRUCT& cs);

  virtual void startBarcode() {editBox.setFocus();}
  virtual void stopBarcode()  {sink.setFocus();}

  virtual void displayHeader(DevBase& dev) { }
  virtual void displayFooter(DevBase& dev) { }

          void initRptOrietn();
          void saveRptOrietn();

  virtual void onPreparePrinting(CPrintInfo* info);
  virtual void onBeginPrinting();
  virtual void onDisplayOutput();

  virtual void printHeader(DevBase& dev, int pageNo);
  virtual void printFooter(DevBase& dev, int pageNo);
  virtual void OnEndPrinting(CDC* pDC, CPrintInfo* pInfo);

  ICS_211aDoc* GetDocument() const;

  Log211Rpt&   log211Rpt()  {return dspLog211;}
  MembersRpt&  membersRpt() {return dspMembers;}
  RosterRpt&   rosterRpt()  {return dspRoster;}

#ifdef _DEBUG
  virtual void AssertValid() const;
  virtual void Dump(CDumpContext& dc) const;
#endif

protected:

  virtual void OnInitialUpdate();

private:

  void setArchiveAttr(NotePad& np, PrtrOrient orient, double f = 1.0);

public:

  DECLARE_MESSAGE_MAP()

  afx_msg void onOptions();
  afx_msg void onRptOrietn();
  afx_msg void OnSetFocus(CWnd* pOldWnd);

  afx_msg void OnChangeBarCode();
  afx_msg void OnChangeSink();
  };


#ifndef _DEBUG  // debug version in ICS_211aView.cpp
inline ICS_211aDoc* ICS_211aView::GetDocument() const
                                                    {return reinterpret_cast<ICS_211aDoc*>(m_pDocument);}
#endif

