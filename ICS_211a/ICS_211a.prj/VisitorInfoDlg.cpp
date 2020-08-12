// VisitorInfoDlg.cpp : implementation file
//

#include "stdafx.h"
#include "VisitorInfoDlg.h"
#include "Roster.h"
#include "Utilities.h"



//#include "afxdialogex.h"


// VisitorInfoDlg dialog

IMPLEMENT_DYNAMIC(VisitorInfoDlg, CDialogEx)

VisitorInfoDlg::VisitorInfoDlg(CWnd* pParent) : CDialogEx(IDD_VisitorInfo, pParent), callSign(_T("")),
                             firstName(_T("")), lastName(_T("")), agency(_T("")), checkOut(_T("")) { }


VisitorInfoDlg::~VisitorInfoDlg() { }



BOOL VisitorInfoDlg::OnInitDialog() {
RosterFilter fltr;
RFIter       iter(fltr);
RstrIter     rstrIter(roster);
Datum*       dtm;
String       s;

  CDialogEx::OnInitDialog();

  for (dtm = rstrIter(); dtm; dtm = rstrIter++) if (dtm->visitor) fltr.add(*dtm);

  for (dtm = iter(); dtm; dtm = iter++) {
    s =  addTab(dtm->fcc,  roster.maxFCC);          s += addTab(dtm->firstName, roster.maxFirstName);
    s += addTab(dtm->lastName, roster.maxLastName); s += dtm->agency;
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
  }


BEGIN_MESSAGE_MAP(VisitorInfoDlg, CDialogEx)
END_MESSAGE_MAP()


// VisitorInfoDlg message handlers
