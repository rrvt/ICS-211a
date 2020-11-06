// Collect Event Information


#pragma once
#include "Resource.h"


// EventInfoDlg dialog

class EventInfoDlg : public CDialogEx {

  DECLARE_DYNAMIC(EventInfoDlg)

public:
CString name;
CString date;
CString incidentNo;
CString location;
CString preparedBy;
CString missionNo;
CEdit   dateCtrl;

           EventInfoDlg(CWnd* pParent = nullptr);   // standard constructor
  virtual ~EventInfoDlg();

// Dialog Data
#ifdef AFX_DESIGN_TIME
  enum { IDD = IDD_EventTitle };
#endif

protected:

  virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

  DECLARE_MESSAGE_MAP()
public:
  afx_msg void OnEnChangeDate();
  };
