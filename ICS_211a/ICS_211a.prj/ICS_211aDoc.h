// ICS_211aDoc.h : interface of the ICS_211aDoc class


#pragma once
#include "CDoc.h"
#include "Date.h"
#include "Roster.h"


enum LoadType  {NilLoad,   MemberLoad,  RosterLoad};
enum StoreType {NilStore,  ReportStore, RosterStore, InitiazeRoster, IncStore};
enum DsplyType {RosterDsp, MemberDsp};


class ICS_211aDoc : public CDoc {

LoadType  loadType;
StoreType storeType;
DsplyType dsplyType;

String    saveAsTitle;                                            // Save As Parameters, examples:
String    defFileName;                                            // _T("mumble.txt")
String    defExt;                                                 // _T("txt")
String    defFilePat;                                             // _T("*.txt")

Date      dt;

protected: // create from serialization only

  ICS_211aDoc() noexcept;
  DECLARE_DYNCREATE(ICS_211aDoc)

public:
          bool initScanner();
          bool loadMemberInfo();
          bool loadRoster();

          bool saveDtm();

          void display();
          void displayRoster(); //{dsplyType = RosterDsp; invalidate();}

  virtual void serialize(Archive& ar);

  virtual ~ICS_211aDoc();

#ifdef _DEBUG
  virtual void AssertValid() const;
  virtual void Dump(CDumpContext& dc) const;
#endif

protected:

// Generated message map functions

  DECLARE_MESSAGE_MAP()

private:

  afx_msg void OnReadBarCodes();
  afx_msg void OnMember();
  afx_msg void OnVisitor();
  afx_msg void OnDisplayMembers();
  afx_msg void OnOrganizeInfo();

  afx_msg void onPrepareLog();
  afx_msg void OnEditTitle();
  afx_msg void OnCheckOutDefaulters();
  afx_msg void OnSaveFile();
  afx_msg void OnOptions();
  afx_msg void onEditEntry();

  void startBarCodeRead();
  };
