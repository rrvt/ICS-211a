// dialog box to determine if barcode is to be used as a new check in or to modify last check out

#pragma once
#include "Resource.h"


enum CheckInOut {NilChkInOut, CheckIn, ModCheckOut};


// CheckInOutDlg dialog

class CheckInOutDlg : public CDialogEx {
  DECLARE_DYNAMIC(CheckInOutDlg)

public:

CheckInOut what;

  CheckInOutDlg(CWnd* pParent = nullptr);   // standard constructor
  virtual ~CheckInOutDlg();

// Dialog Data
#ifdef AFX_DESIGN_TIME
  enum { IDD = IDD_CheckInOut };
#endif

protected:
  virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

  DECLARE_MESSAGE_MAP()
public:
  afx_msg void OnNewCheckIn();
  afx_msg void OnModifyChkOut();
};
