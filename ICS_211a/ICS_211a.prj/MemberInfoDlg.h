// Member Information Dialog


#pragma once


// MemberInfoDlg dialog

class MemberInfoDlg : public CDialogEx {

  DECLARE_DYNAMIC(MemberInfoDlg)

int       maxCallSign;
int       maxFirstName;
int       maxLastName;

public:

CComboBox memberInfoCtrl;
CString   memberInfo;
CComboBox checkOutCtrl;
CString   checkOut;

           MemberInfoDlg(CWnd* pParent = nullptr);   // standard constructor

  virtual ~MemberInfoDlg();
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
