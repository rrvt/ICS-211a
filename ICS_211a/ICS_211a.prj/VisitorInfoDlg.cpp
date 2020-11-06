// VisitorInfoDlg.cpp : implementation file
//

#include "stdafx.h"
#include "VisitorInfoDlg.h"
#include "Roster.h"
#include "RosterFilter.h"
#include "Utilities.h"


BEGIN_MESSAGE_MAP(VisitorInfoDlg, CDialogEx)
  ON_EN_KILLFOCUS(IDC_EDIT1, &VisitorInfoDlg::OnLeaveCallSign)
END_MESSAGE_MAP()


// VisitorInfoDlg dialog

IMPLEMENT_DYNAMIC(VisitorInfoDlg, CDialogEx)

VisitorInfoDlg::VisitorInfoDlg(CWnd* pParent) : CDialogEx(IDD_VisitorInfo, pParent), callSign(_T("")),
                             firstName(_T("")), lastName(_T("")), agency(_T("")), checkOut(_T("")) { }


VisitorInfoDlg::~VisitorInfoDlg() { }



BOOL VisitorInfoDlg::OnInitDialog() {
RstrIter     rstrIter(roster);
Datum*       dtm;
RosterFilter fltr;
RFIter       iter(fltr);
IDinfo*      idInfo;
String       s;

  CDialogEx::OnInitDialog();

  for (dtm = rstrIter(); dtm; dtm = rstrIter++) if (dtm->visitor) fltr.add(*dtm);

  for (idInfo = iter(); idInfo; idInfo = iter++) {

    if (idInfo->chkOut) continue;

    s =  addTab(idInfo->callSign,  fltr.maxCallSign);
    s += addTab(idInfo->firstName, fltr.maxFirst);
    s += addTab(idInfo->lastName,  fltr.maxLast);
    s += idInfo->agency;

    checkOutCtrl.AddString(s);
    }

  return TRUE;
  }




void VisitorInfoDlg::DoDataExchange(CDataExchange* pDX) {
  CDialogEx::DoDataExchange(pDX);
  DDX_Text(    pDX, IDC_EDIT1,    callSign);
  DDX_Text(    pDX, IDC_EDIT2,    firstName);
  DDX_Text(    pDX, IDC_EDIT3,    lastName);
  DDX_Text(    pDX, IDC_EDIT4,    agency);
  DDX_CBString(pDX, IDC_CheckOut, checkOut);
  DDX_Control( pDX, IDC_CheckOut, checkOutCtrl);
  DDX_Control( pDX, IDC_EDIT2,    firstCtrl);
  DDX_Control( pDX, IDC_EDIT3,    lastCtrl);
  DDX_Control( pDX, IDC_EDIT4,    agencyCtrl);
  DDX_Control( pDX, IDC_EDIT1,    callSignCtrl);
  }


// VisitorInfoDlg message handlers


void VisitorInfoDlg::OnLeaveCallSign() {
CString fcc;
Datum*  datum;

  callSignCtrl.GetWindowText(fcc);   datum = roster.findVisitor(fcc);

  if (datum) {
    firstCtrl.SetWindowText(datum->firstName);
    lastCtrl.SetWindowText(datum->lastName);
    agencyCtrl.SetWindowText(datum->agency);
    }
  }

