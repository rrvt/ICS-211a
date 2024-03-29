// EditEntryDlg.cpp : implementation file
//

#include "stdafx.h"
#include "EditEntryDlg.h"
#include "Log211.h"


// EditEntryDlg dialog

IMPLEMENT_DYNAMIC(EditEntryDlg, CDialogEx)


BEGIN_MESSAGE_MAP(EditEntryDlg, CDialogEx)
  ON_CBN_SELCHANGE(IDC_LogEntry, &EditEntryDlg::OnSelchangeLogentry)
  ON_EN_CHANGE(IDC_DateIn, &EditEntryDlg::onChangeDateIn)
  ON_EN_CHANGE(IDC_TimeIn, &EditEntryDlg::onChangeTimeIn)
  ON_EN_CHANGE(IDC_DateOut, &EditEntryDlg::onChangeDateOut)
  ON_EN_CHANGE(IDC_TimeOut, &EditEntryDlg::onChangeTimeOut)
END_MESSAGE_MAP()


EditEntryDlg::EditEntryDlg(CWnd* pParent) : CDialogEx(IDD_EditEntry, pParent), callSign(_T("")),
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
int       maxLast  = 0;
int       t;

  CDialogEx::OnInitDialog();

  for (lgdtm = iter(); lgdtm; lgdtm = iter++) {
    t = lgdtm->callSign.length();    if (t > maxFcc)   maxFcc   = t;
    t = lgdtm->firstName.length();   if (t > maxFirst) maxFirst = t;
    t = lgdtm->lastName.length();    if (t > maxLast)  maxLast  = t;
    }

  for (lgdtm = iter(); lgdtm; lgdtm = iter++) {
    s  = addSepTab(lgdtm->callSign,  maxFcc);
    s += addSepTab(lgdtm->firstName, maxFirst);
    s += addSepTab(lgdtm->lastName,  maxLast);
    s += addSepTab(lgdtm->timeIn,    16);
    s += lgdtm->timeOut;

    logEntryCtrl.AddString(s);

    EntryDsc& dsc = entries.nextData();   dsc.key = s; dsc.lgdtm = lgdtm;
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

        fccCtrl.SetWindowText(callSign  = lgdtm->callSign);
  firstNameCtrl.SetWindowText(firstName = lgdtm->firstName);
   lastNameCtrl.SetWindowText(lastName  = lgdtm->lastName);
    chkInDtCtrl.SetWindowText(chkInDt   = lgdtm->checkIn.getDate());
    chkInTmCtrl.SetWindowText(chkInTm   = lgdtm->checkIn.getHHMM());
   chkOutDtCtrl.SetWindowText(chkOutDt  = lgdtm->checkOut.getDate());
   chkOutTmCtrl.SetWindowText(chkOutTm  = lgdtm->checkOut.getHHMM());
  }





void EditEntryDlg::onChangeDateIn()  {Date::onChangeDate(chkInDtCtrl);}
void EditEntryDlg::onChangeTimeIn()  {Date::onChangeTime(chkInTmCtrl);}
void EditEntryDlg::onChangeDateOut() {Date::onChangeDate(chkOutDtCtrl);}
void EditEntryDlg::onChangeTimeOut() {Date::onChangeTime(chkOutTmCtrl);}


void EditEntryDlg::DoDataExchange(CDataExchange* pDX) {
  CDialogEx::DoDataExchange(pDX);
  DDX_Control(pDX, IDC_LogEntry,  logEntryCtrl);
  DDX_Control(pDX, IDC_CallSign,  fccCtrl);
  DDX_Text(   pDX, IDC_CallSign,  callSign);
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

