// EditEntryDlg.cpp : implementation file
//

#include "stdafx.h"
#include "EditEntryDlg.h"
#include "Utilities.h"

// EditEntryDlg dialog

IMPLEMENT_DYNAMIC(EditEntryDlg, CDialogEx)


BEGIN_MESSAGE_MAP(EditEntryDlg, CDialogEx)
  ON_CBN_SELCHANGE(IDC_LogEntry, &EditEntryDlg::OnSelchangeLogentry)
  ON_EN_CHANGE(IDC_DateIn, &EditEntryDlg::onChangeDateIn)
  ON_EN_CHANGE(IDC_TimeIn, &EditEntryDlg::onChangeTimeIn)
  ON_EN_CHANGE(IDC_DateOut, &EditEntryDlg::onChangeDateOut)
  ON_EN_CHANGE(IDC_TimeOut, &EditEntryDlg::onChangeTimeOut)
END_MESSAGE_MAP()


EditEntryDlg::EditEntryDlg(CWnd* pParent) : CDialogEx(IDD_EditEntry, pParent), fcc(_T("")),
                                            firstName(_T("")), lastName(_T("")), chkInTm(_T("")),
                                            chkOutDt(_T("")), chkOutTm(_T("")), chkInDt(_T("")),
                                            lgdtm(0) { }


EditEntryDlg::~EditEntryDlg() { }


BOOL EditEntryDlg::OnInitDialog() {
LogIter   iter(log211);
LogDatum* lgdtm;
String    s;
int       maxFcc   = 0;
int       maxFirst = 0;
int       t;

  CDialogEx::OnInitDialog();

  for (lgdtm = iter(); lgdtm; lgdtm = iter++) {
    t = lgdtm->fcc.length();         if (t > maxFcc)   maxFcc   = t;
    t = lgdtm->firstName.length();   if (t > maxFirst) maxFirst = t;
    }

  for (lgdtm = iter(); lgdtm; lgdtm = iter++) {
    s  = addTab(lgdtm->fcc, maxFcc);
    s += addTab(lgdtm->firstName, maxFirst);
    s += lgdtm->lastName;
    logEntryCtrl.AddString(s);
    EntryDsc& dsc = entries[entries.end()];   dsc.key = s; dsc.lgdtm = lgdtm;
    }

  return TRUE;
  }


void EditEntryDlg::OnSelchangeLogentry() {
EEDIter   iter(*this);
EntryDsc* dsc;
CString   cs;
String    s;


  logEntryCtrl.GetLBText(logEntryCtrl.GetCurSel(), cs);   s = cs;

  for (dsc = iter(); dsc; dsc = iter++) if (dsc->key == s) {lgdtm = dsc->lgdtm; break;}

  if (!lgdtm) return;

        fccCtrl.SetWindowText(fcc       = lgdtm->fcc);
  firstNameCtrl.SetWindowText(firstName = lgdtm->firstName);
   lastNameCtrl.SetWindowText(lastName  = lgdtm->lastName);
    chkInDtCtrl.SetWindowText(chkInDt   = lgdtm->dateIn.getDate());
    chkInTmCtrl.SetWindowText(chkInTm   = lgdtm->dateIn.getHHMM());
   chkOutDtCtrl.SetWindowText(chkOutDt  = lgdtm->dateOut.getDate());
   chkOutTmCtrl.SetWindowText(chkOutTm  = lgdtm->dateOut.getHHMM());
  }





void EditEntryDlg::onChangeDateIn()  {Date::onChangeDate(chkInDtCtrl);}
void EditEntryDlg::onChangeTimeIn()  {Date::onChangeTime(chkInTmCtrl);}
void EditEntryDlg::onChangeDateOut() {Date::onChangeDate(chkOutDtCtrl);}
void EditEntryDlg::onChangeTimeOut() {Date::onChangeTime(chkOutTmCtrl);}


void EditEntryDlg::DoDataExchange(CDataExchange* pDX) {
  CDialogEx::DoDataExchange(pDX);
  DDX_Control(pDX, IDC_LogEntry,  logEntryCtrl);
  DDX_Control(pDX, IDC_CallSign,  fccCtrl);
  DDX_Text(   pDX, IDC_CallSign,  fcc);
  DDX_Control(pDX, IDC_FirstName, firstNameCtrl);
  DDX_Text(   pDX, IDC_FirstName, firstName);
  DDX_Control(pDX, IDC_LastName,  lastNameCtrl);
  DDX_Text(   pDX, IDC_LastName,  lastName);
  DDX_Control(pDX, IDC_DateIn,    chkInDtCtrl);
  DDX_Text(   pDX, IDC_DateIn,    chkInDt);
  DDX_Control(pDX, IDC_TimeIn,    chkInTmCtrl);
  DDX_Text(   pDX, IDC_TimeIn,    chkInTm);
  DDX_Control(pDX, IDC_DateOut,   chkOutDtCtrl);
  DDX_Text(   pDX, IDC_DateOut,   chkOutDt);
  DDX_Control(pDX, IDC_TimeOut,   chkOutTmCtrl);
  DDX_Text(   pDX, IDC_TimeOut,   chkOutTm);
  }

