// DefaultersDlg.cpp : implementation file
//

#include "stdafx.h"
#include "DefaultersDlg.h"
#include "Resource.h"

                                                             //ON_EN_CHANGE
// DefaultersDlg dialog

IMPLEMENT_DYNAMIC(DefaultersDlg, CDialogEx)


BEGIN_MESSAGE_MAP(DefaultersDlg, CDialogEx)
  ON_CBN_SELCHANGE(IDC_Attendee, &DefaultersDlg::OnChangeAttendee)
  ON_EN_KILLFOCUS( IDC_Time,     &DefaultersDlg::OnLeaveTime)
  ON_BN_CLICKED(   IDOK,         &DefaultersDlg::OnOK)
  ON_EN_CHANGE(    IDC_Time,     &DefaultersDlg::OnChangeTime)
  ON_EN_CHANGE(    IDC_Date,     &DefaultersDlg::OnChangeDate)
  ON_EN_KILLFOCUS(IDC_Date, &DefaultersDlg::onLeaveDate)
END_MESSAGE_MAP()


DefaultersDlg::DefaultersDlg(CWnd* pParent) : CDialogEx(IDD_CheckOutRepair, pParent),
          attendee(_T("")), time(_T("")), updateTime(false), date(_T("")), updateDate(false) { }

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
  DDX_Control(pDX, IDC_Attendee,     attendeeCtrl);
  DDX_CBString(pDX,IDC_Attendee,     attendee);
  DDX_Control(pDX, IDC_Time,         timeCtrl);
  DDX_Text(pDX,    IDC_Time,         time);
  DDX_Control(pDX, IDC_MedianChkOut, median);
  DDX_Control(pDX, IDC_Date,         dateCtrl);
  DDX_Text(pDX,    IDC_Date,         date);
  }


void DefaultersDlg::OnChangeDate() {Date::onChangeDate(dateCtrl);}
void DefaultersDlg::OnChangeTime() {Date::onChangeTime(timeCtrl);}


bool DefaultersDlg::vrfyMnth(int cnt, TCchar ch, int& v)
                                           {v = v * cnt * 10 + ch - _T('0');   return 1 <= v && v <= 12;}
bool DefaultersDlg::vrfyDay(int cnt, TCchar ch, int& v)
                                           {v = v * cnt * 10 + ch - _T('0');   return 1 <= v && v <= 31;}
bool DefaultersDlg::vrfyYr(int cnt, TCchar ch, int& v)
                                           {v = v * cnt * 10 + ch - _T('0');   return 0 <= v && v <= 40;}







bool DefaultersDlg::vrfyHr(int cnt, TCchar ch, int& v)
                                                    {v = v * cnt * 10 + ch - _T('0');   return v < 24;}
bool DefaultersDlg::vrfyMin(int cnt, TCchar ch, int& v)
                                                    {v = v * cnt * 10 + ch - _T('0');   return v < 60;}


void DefaultersDlg::replTmSel(int i, TCchar ch) {
bool   aCh = ch != 0;
String s;

  if (aCh) {s = _T(':'); s += ch;}
  else     {Beep(1500, 120);}

  timeCtrl.SetSel(i, i+1);   timeCtrl.ReplaceSel(s);
  }


void DefaultersDlg::replDtSel(int i, TCchar ch) {
bool   aCh = ch != 0;
String s;

  if (aCh) {s = _T('/'); s += ch;}
  else     {Beep(1500, 120);}

  dateCtrl.SetSel(i, i+1);   dateCtrl.ReplaceSel(s);
  }


void DefaultersDlg::OnLeaveTime() {
  attendeeCtrl.GetWindowText(attendee);

  timeCtrl.GetWindowText(time);    dateCtrl.GetWindowText(date);   updateAttendees();
  }


void DefaultersDlg::onLeaveDate() {
  attendeeCtrl.GetWindowText(attendee);

  timeCtrl.GetWindowText(time);    dateCtrl.GetWindowText(date);   updateAttendees();
  }


void DefaultersDlg::OnChangeAttendee() {

  attendeeCtrl.GetLBText(attendeeCtrl.GetCurSel(), attendee);

  timeCtrl.GetWindowText(time);   dateCtrl.GetWindowText(date);   updateAttendees();
  }


void DefaultersDlg::updateAttendees() {
int    n = attendees.end();
int    i;
String s;

  for (i = 0; i < n; i++) {
    AttdDsc& dsc = attendees[i];

    s = date + _T(" ");  s += time;

    if (dsc.key == attendee) {dsc.chkOutTm = s;  return;}
    }
  }


void DefaultersDlg::OnOK() {
  attendeeCtrl.GetWindowText(attendee);

  timeCtrl.GetWindowText(time);    dateCtrl.GetWindowText(date);   updateAttendees();

  CDialogEx::OnOK();
  }


