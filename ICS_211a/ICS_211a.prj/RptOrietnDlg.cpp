// RptOrietnDlg.cpp : implementation file
//

#include "pch.h"
#include "RptOrietnDlg.h"


IMPLEMENT_DYNAMIC(RptOrietnDlg, CDialogEx)

RptOrietnDlg::RptOrietnDlg(CWnd* pParent) :
             CDialogEx(IDD_Orientation, pParent), ntpd(_T("")), log(_T("")), rstr(_T("")), mbr(_T("")) { }

RptOrietnDlg::~RptOrietnDlg() { }


BOOL RptOrietnDlg::OnInitDialog() {

  CDialogEx::OnInitDialog();

  initOrietn(ntpdCtl, ntpd);
  initOrietn(logCtl,  log);
  initOrietn(rstrCtl, rstr);
  initOrietn(mbrCtl,  mbr);

  return TRUE;
  }


void RptOrietnDlg::initOrietn(CComboBox& ctl, Cstring& orietn) {
  ctl.AddString(LandscapeKey);
  ctl.AddString(PortraitKey);
  ctl.SelectString(-1, orietn);

  }


void RptOrietnDlg::DoDataExchange(CDataExchange* pDX)
{
  CDialogEx::DoDataExchange(pDX);
  DDX_Control( pDX, IDC_NtpdOrientn,    ntpdCtl);
  DDX_CBString(pDX, IDC_NtpdOrientn,    ntpd);
  DDX_Control( pDX, IDC_LogOrientn,     logCtl);
  DDX_CBString(pDX, IDC_LogOrientn,     log);
  DDX_Control( pDX, IDC_RosterOrientn,  rstrCtl);
  DDX_CBString(pDX, IDC_RosterOrientn,  rstr);
  DDX_Control( pDX, IDC_MbrInfoOrientn, mbrCtl);
  DDX_CBString(pDX, IDC_MbrInfoOrientn, mbr);
  }


BEGIN_MESSAGE_MAP(RptOrietnDlg, CDialogEx)
END_MESSAGE_MAP()

