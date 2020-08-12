// Visitor Info Dialog


#pragma once
#include "Resource.h"

// VisitorInfoDlg dialog

class VisitorInfoDlg : public CDialogEx {

  DECLARE_DYNAMIC(VisitorInfoDlg)

public:

CString   callSign;
CString   firstName;
CString   lastName;
CString   agency;
CString   checkOut;
CComboBox checkOutCtrl;

           VisitorInfoDlg(CWnd* pParent = nullptr);   // standard constructor
  virtual ~VisitorInfoDlg();

  virtual BOOL OnInitDialog();


// Dialog Data
#ifdef AFX_DESIGN_TIME
  enum { IDD = IDD_VisitorInfo };
#endif

protected:

  virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

  DECLARE_MESSAGE_MAP()
  };
