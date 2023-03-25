// Set Report Printer Orientation


#pragma once
#include "Resource.h"
#include "PrintMgr.h"


class RptOrietnDlg : public CDialogEx {

  DECLARE_DYNAMIC(RptOrietnDlg)

public:
Cstring    ntpd;
Cstring    log;
Cstring    rstr;
Cstring    mbr;
//PrtrOrient logOriet;
//PrtrOrient rstrOriet;
//PrtrOrient mbrOriet;
CComboBox  ntpdCtl;
CComboBox  logCtl;
CComboBox  rstrCtl;
CComboBox  mbrCtl;

               RptOrietnDlg(CWnd* pParent = nullptr);   // standard constructor
  virtual     ~RptOrietnDlg();

  virtual BOOL OnInitDialog();

// Dialog Data
#ifdef AFX_DESIGN_TIME
  enum { IDD = IDD_Orientation };
#endif

protected:

  virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

  DECLARE_MESSAGE_MAP()

private:

  void initOrietn(CComboBox& ctl, Cstring& orietn);
  };
