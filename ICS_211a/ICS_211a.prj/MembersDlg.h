// Member Information Dialog


#pragma once
#include "Resource.h"


// MembersDlg dialog

class MembersDlg : public CDialogEx {

  DECLARE_DYNAMIC(MembersDlg)

int       maxCallSign;
int       maxFirstName;
int       maxLastName;

public:

CComboBox memberInfoCtrl;
CString   memberInfo;
CComboBox checkOutCtrl;
CString   checkOut;

           MembersDlg(CWnd* pParent = nullptr);   // standard constructor

  virtual ~MembersDlg();
  virtual BOOL OnInitDialog();

#ifdef AFX_DESIGN_TIME
  enum { IDD = IDD_MemberInfo };                          // Dialog Data
#endif

protected:

  void loadCheckOut();

  virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

  void   getMax();

  DECLARE_MESSAGE_MAP()
  };
