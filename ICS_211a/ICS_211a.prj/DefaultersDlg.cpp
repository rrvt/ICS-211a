// DefaultersDlg.cpp : implementation file


#include "stdafx.h"
#include "DefaultersDlg.h"
#include "Resource.h"


IMPLEMENT_DYNAMIC(DefaultersDlg, CDialogEx)


BEGIN_MESSAGE_MAP(DefaultersDlg, CDialogEx)
  ON_CBN_SELCHANGE(IDC_Attendee, &OnChangeAttendee)
  ON_EN_CHANGE(    IDC_ChoTime,  &OnChangeTime)
  ON_EN_CHANGE(    IDC_ChoDate,  &OnChangeDate)
  ON_EN_KILLFOCUS( IDC_ChoDate,  &onLeaveDate)
  ON_EN_KILLFOCUS( IDC_ChoTime,  &OnLeaveTime)
  ON_BN_CLICKED(   IDC_Update,   &OnUpdate)
  ON_BN_CLICKED(   IDOK,         &OnOK)
END_MESSAGE_MAP()


DefaultersDlg::DefaultersDlg(CWnd* pParent) : CDialogEx(IDD_CheckOutRepair, pParent), attendee(_T("")),
                                              medTime(_T("")), updateTime(false), medDate(_T("")),
                                              updateDate(false), choDate(_T("")), choTime(_T("")) { }

DefaultersDlg::~DefaultersDlg() { }


BOOL DefaultersDlg::OnInitDialog() {
int n = attendees.end();
int i;

  CDialogEx::OnInitDialog();

  median.SetWindowText(medcs);

  for (i = 0; i < n; i++) attendeeCtrl.AddString(attendees[i].key);

  return TRUE;
  }


void DefaultersDlg::DoDataExchange(CDataExchange* pDX) {
  CDialogEx::DoDataExchange(pDX);
  DDX_Control( pDX, IDC_Attendee,     attendeeCtrl);
  DDX_CBString(pDX, IDC_Attendee,     attendee);
  DDX_Control( pDX, IDC_ChIDateTime,  checkInCtrl);
  DDX_Control( pDX, IDC_ChoDate,      choDateCtrl);
  DDX_Text(    pDX, IDC_ChoDate,      choDate);
  DDX_Control( pDX, IDC_ChoTime,      choTimeCtrl);
  DDX_Text(    pDX, IDC_ChoTime,      choTime);
  DDX_Control( pDX, IDC_MedianChkOut, median);
  }


void DefaultersDlg::OnChangeAttendee() {
LogDatum* dtm;
String    date;
Date      dt;

  attendeeCtrl.GetLBText(attendeeCtrl.GetCurSel(), attendee);

  dtm = find(attendee);   if (!dtm) return;

  date = dtm->checkIn.getDate() + _T("  ") + dtm->checkIn.getHHMM();

  checkInCtrl.SetWindowText(date);

  if (dtm->timeOut.isEmpty()) dt = log211.suggestDate(dtm);
  else                        dt = dtm->checkOut;

  choDate = dt.getDate();   choDateCtrl.SetWindowText(choDate);
  choTime = dt.getHHMM();   choTimeCtrl.SetWindowText(choTime);
  }


void DefaultersDlg::OnChangeDate() {Date::onChangeDate(choDateCtrl);}
void DefaultersDlg::OnChangeTime() {Date::onChangeTime(choTimeCtrl);}


void DefaultersDlg::OnLeaveTime()
                            {attendeeCtrl.GetWindowText(attendee);   choTimeCtrl.GetWindowText(choTime);}


void DefaultersDlg::onLeaveDate()
                            {attendeeCtrl.GetWindowText(attendee);   choDateCtrl.GetWindowText(choDate);}


void DefaultersDlg::OnUpdate() {saveChkOut();}


void DefaultersDlg::OnOK() {saveChkOut();   CDialogEx::OnOK();}


void DefaultersDlg::saveChkOut() {
String   s;
DftrIter iter(*this);
AttdDsc* dsc;
Date     date;

  if (choDate.IsEmpty() || choTime.IsEmpty()) return;

  s = choDate + _T(" ");  s += choTime;   date = s;

  for (dsc = iter(); dsc; dsc = iter++) if (dsc->key == attendee)
                                        {if (date > dsc->lgdtm->checkIn) dsc->chkOutTm = date;   return;}
  }


LogDatum* DefaultersDlg::find(TCchar* key) {
DftrIter iter(*this);
AttdDsc* dsc;

  for (dsc = iter(); dsc; dsc = iter++) if (dsc->key == key) return dsc->lgdtm;

  return 0;
  }

