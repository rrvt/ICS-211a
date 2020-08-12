// OptionsDlg.cpp : implementation file
//

#include "stdafx.h"
#include "OptionsDlg.h"


// OptionsDlg dialog

IMPLEMENT_DYNAMIC(OptionsDlg, CDialogEx)


BEGIN_MESSAGE_MAP(OptionsDlg, CDialogEx)
  ON_EN_CHANGE(IDC_TopMargin,   &OptionsDlg::OnChangeTop)
  ON_EN_CHANGE(IDC_LeftMargin,  &OptionsDlg::OnChangeLeft)
  ON_EN_CHANGE(IDC_RightMargin, &OptionsDlg::OnChangeRight)
  ON_EN_CHANGE(IDC_BotMargin,   &OptionsDlg::OnChangeBot)
END_MESSAGE_MAP()


OptionsDlg::OptionsDlg(CWnd* pParent) : CDialogEx(IDD_Options, pParent),
                        topMargin(_T("")), leftMargin(_T("")), rightMargin(_T("")), botMargin(_T("")) { }


OptionsDlg::~OptionsDlg() { }


void OptionsDlg::DoDataExchange(CDataExchange* pDX) {
  CDialogEx::DoDataExchange(pDX);
  DDX_Text(   pDX, IDC_TopMargin,   topMargin);
  DDX_Text(   pDX, IDC_LeftMargin,  leftMargin);
  DDX_Text(   pDX, IDC_RightMargin, rightMargin);
  DDX_Text(   pDX, IDC_BotMargin,   botMargin);
  DDX_Control(pDX, IDC_TopMargin,   topCtrl);
  DDX_Control(pDX, IDC_LeftMargin,  leftCtrl);
  DDX_Control(pDX, IDC_RightMargin, rightCtrl);
  DDX_Control(pDX, IDC_BotMargin,   botCtrl);
}


void OptionsDlg::OnChangeTop() {floatingOnly(topCtrl);}


void OptionsDlg::OnChangeLeft() {floatingOnly(leftCtrl);}


void OptionsDlg::OnChangeRight() {floatingOnly(rightCtrl);}


void OptionsDlg::OnChangeBot() {floatingOnly(botCtrl);}


void OptionsDlg::floatingOnly(CEdit& ctrl) {
CString cs;
String  s;
int     n;
int     i;
bool    periodSeen = false;

  ctrl.GetWindowText(cs);   s = cs;

  for (i = 0, n = s.length(); i < n; i++) {
    Tchar ch = s[i];

    if (_T('0') <= ch && ch <= _T('9')) continue;

    if (ch == _T('.') && !periodSeen) {periodSeen = true; continue;}

    Beep(1500, 120);   ctrl.SetSel(i, i+1);   ctrl.ReplaceSel(_T(""));
    }
  }

