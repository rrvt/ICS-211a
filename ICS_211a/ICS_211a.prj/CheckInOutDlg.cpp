// CheckInOutDlg.cpp : implementation file
//

#include "stdafx.h"
#include "CheckInOutDlg.h"


// CheckInOutDlg dialog

IMPLEMENT_DYNAMIC(CheckInOutDlg, CDialogEx)

CheckInOutDlg::CheckInOutDlg(CWnd* pParent) : CDialogEx(IDD_CheckInOut, pParent), what(NilChkInOut) { }

CheckInOutDlg::~CheckInOutDlg() { }


void CheckInOutDlg::DoDataExchange(CDataExchange* pDX) {
  CDialogEx::DoDataExchange(pDX);
  }


BEGIN_MESSAGE_MAP(CheckInOutDlg, CDialogEx)
  ON_BN_CLICKED(IDC_NewCheckIn,   &OnNewCheckIn)
  ON_BN_CLICKED(IDC_ModifyChkOut, &OnModifyChkOut)
END_MESSAGE_MAP()


// CheckInOutDlg message handlers


void CheckInOutDlg::OnNewCheckIn()   {what = CheckIn;     CDialogEx::OnOK();}
void CheckInOutDlg::OnModifyChkOut() {what = ModCheckOut; CDialogEx::OnOK();}


