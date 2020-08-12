// MemberInfoDlg.cpp : implementation file
//

#include "stdafx.h"
#include "MemberInfo.h"
#include "MemberInfoDlg.h"
#include "Resource.h"
#include "Roster.h"
#include "Utilities.h"


// MemberInfoDlg dialog

IMPLEMENT_DYNAMIC(MemberInfoDlg, CDialogEx)

MemberInfoDlg::MemberInfoDlg(CWnd* pParent) : CDialogEx(IDD_MemberInfo, pParent),
            memberInfo(_T("")), maxCallSign(0), maxFirstName(0), maxLastName(0), checkOut(_T("")) { }

MemberInfoDlg::~MemberInfoDlg() { }




BOOL MemberInfoDlg::OnInitDialog() {
MbrIter     iter(members);
MemberInfo* mi;
String      s;

  CDialogEx::OnInitDialog();

  getMax();

  for (mi = iter(); mi; mi = iter++) {

    s = addTab(mi->callSign, maxCallSign);  s += addTab(mi->firstName, maxFirstName);
    s += addTab(mi->lastName, maxLastName); s += mi->badgeNumber;

    memberInfoCtrl.AddString(s);
    }

  loadCheckOut();

  return TRUE;
  }


void MemberInfoDlg::loadCheckOut() {
RosterFilter fltr;
RFIter       iter(fltr);
RstrIter     rstrIter(roster);
Datum*       dtm;
String       s;

  for (dtm = rstrIter(); dtm; dtm = rstrIter++) fltr.add(*dtm);

  for (dtm = iter(); dtm; dtm = iter++) {
    s  = addTab(dtm->fcc, maxCallSign);      s += addTab(dtm->firstName, maxFirstName);
    s += addTab(dtm->lastName, maxLastName); s += dtm->id;
    checkOutCtrl.AddString(s);
    }
  }



void MemberInfoDlg::getMax() {
int    n = members.data.end();
int    i;

  for (i = 0; i < n; i++) {
    MemberInfo& mi = members.data[i];
    getMaxLng(mi.callSign,  maxCallSign);
    getMaxLng(mi.firstName, maxFirstName);
    getMaxLng(mi.lastName,  maxLastName);
    }
  }


#if 0
static String blks = _T("                          ");


String MemberInfoDlg::addTab(String& s, int max)
                                {int n = max - s.length() + 2; return s + _T(',') + blks.substr(0, n);}
#endif

//void MemberInfoDlg::getMax(String& s, int& max) {int lng = s.length();  if (lng > max) max = lng;}



void MemberInfoDlg::DoDataExchange(CDataExchange* pDX) {
  CDialogEx::DoDataExchange(pDX);
  DDX_Control(pDX,  IDC_MemberInfo, memberInfoCtrl);
  DDX_CBString(pDX, IDC_MemberInfo, memberInfo);
  DDX_Control(pDX,  IDC_CheckOut,   checkOutCtrl);
  DDX_CBString(pDX, IDC_CheckOut,   checkOut);
}


BEGIN_MESSAGE_MAP(MemberInfoDlg, CDialogEx)
END_MESSAGE_MAP()

