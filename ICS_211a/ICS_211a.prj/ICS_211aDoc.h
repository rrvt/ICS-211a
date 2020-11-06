// ICS_211aDoc.h : interface of the ICS_211aDoc class


#pragma once
#include "CDoc.h"
#include "Date.h"


enum DataSource {NoteSource, MemberSrc, Log211Src, InitRoster, RosterSrc, IncSrc, CSVSrc};


class ICS_211aDoc : public CDoc {

PathDesc    pathDsc;

DataSource  dataSource;

Date        dt;

protected: // create from serialization only

  ICS_211aDoc() noexcept;
  DECLARE_DYNCREATE(ICS_211aDoc)

public:
          bool initScanner();
          void startBarCodeRead();
          bool loadMemberInfo();
          void loadRoster();

    DataSource dataSrc() {return dataSource;}

          void display(DataSource ds);

          bool saveDtm();                // Start Save Roster line item
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
  afx_msg void OnNewICS211a();
  afx_msg void OnEditTitle();
  afx_msg void OnCheckOutDefaulters();
  afx_msg void OnSaveFile();
  afx_msg void OnOptions();
  afx_msg void onEditEntry();
  };


