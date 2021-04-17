// MembersDlg.cpp : implementation file
//

#include "stdafx.h"
#include "MembersDlg.h"
#include "Members.h"
#include "Roster.h"
#include "RosterFilter.h"
#include "Utilities.h"


// MembersDlg dialog

IMPLEMENT_DYNAMIC(MembersDlg, CDialogEx)

MembersDlg::MembersDlg(CWnd* pParent) : CDialogEx(IDD_MemberInfo, pParent),
            memberInfo(_T("")), maxCallSign(0), maxFirstName(0), maxLastName(0), checkOut(_T("")) { }

MembersDlg::~MembersDlg() { }




BOOL MembersDlg::OnInitDialog() {
MbrIter     iter(members);
MemberInfo* mi;
String      s;
int         x = 0;

  CDialogEx::OnInitDialog();

  getMax();

  for (mi = iter(); mi; mi = iter++) {

    s  = addSepTab(mi->callSign, maxCallSign); s += addSepTab(mi->firstName, maxFirstName);
    s += addSepTab(mi->lastName, maxLastName); s += mi->badgeNumber;

    memberInfoCtrl.AddString(s);
    }

  loadCheckOut();

  return TRUE;
  }


void MembersDlg::loadCheckOut() {
RstrIter     rstrIter(roster);
Datum*       dtm;
RosterFilter fltr;
RFIter       iter(fltr);
IDinfo*      info;
String       s;

  for (dtm = rstrIter(); dtm; dtm = rstrIter++) if (!dtm->visitor) fltr.add(*dtm);

  for (info = iter(); info; info = iter++) {

    if (info->chkOut) continue;

    s  = addSepTab(info->callSign,  fltr.maxCallSign);
    s += addSepTab(info->firstName, fltr.maxFirst);
    s += addSepTab(info->lastName,  fltr.maxLast);
    s += info->id;

    checkOutCtrl.AddString(s);
    }
  }



void MembersDlg::getMax() {
MbrIter     iter(members);
MemberInfo* mi;

  for (mi = iter(); mi; mi = iter++) {
    maxLng(mi->callSign,  maxCallSign);
    maxLng(mi->firstName, maxFirstName);
    maxLng(mi->lastName,  maxLastName);
    }
  }


void MembersDlg::DoDataExchange(CDataExchange* pDX) {
  CDialogEx::DoDataExchange(pDX);
  DDX_Control(pDX,  IDC_MemberInfo, memberInfoCtrl);
  DDX_CBString(pDX, IDC_MemberInfo, memberInfo);
  DDX_Control(pDX,  IDC_CheckOut,   checkOutCtrl);
  DDX_CBString(pDX, IDC_CheckOut,   checkOut);
  }


BEGIN_MESSAGE_MAP(MembersDlg, CDialogEx)
END_MESSAGE_MAP()


