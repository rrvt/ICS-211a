// MemberInfoDlg.cpp : implementation file
//

#include "stdafx.h"
#include "MemberInfo.h"
#include "MemberInfoDlg.h"
#include "Resource.h"
#include "Roster.h"
#include "RosterFilter.h"
#include "Utilities.h"


// MemberInfoDlg dialog

IMPLEMENT_DYNAMIC(MemberInfoDlg, CDialogEx)

MemberInfoDlg::MemberInfoDlg(CWnd* pParent) : CDialogEx(IDD_MemberInfo, pParent),
            memberInfo(_T("")), maxCallSign(0), maxFirstName(0), maxLastName(0), checkOut(_T("")) { }

MemberInfoDlg::~MemberInfoDlg() { }




BOOL MemberInfoDlg::OnInitDialog() {
MbrIter     iter(members);
MemberInfo* mi;
String       s;

  CDialogEx::OnInitDialog();

  getMax();

  for (mi = iter(); mi; mi = iter++) {

    s  = addTab(mi->callSign, maxCallSign); s += addTab(mi->firstName, maxFirstName);
    s += addTab(mi->lastName, maxLastName); s += mi->badgeNumber;

    memberInfoCtrl.AddString(s);
    }

  loadCheckOut();

  return TRUE;
  }


void MemberInfoDlg::loadCheckOut() {
RstrIter     rstrIter(roster);
Datum*       dtm;
RosterFilter fltr;
RFIter       iter(fltr);
IDinfo*      info;
String       s;

  for (dtm = rstrIter(); dtm; dtm = rstrIter++) if (!dtm->visitor) fltr.add(*dtm);

  for (info = iter(); info; info = iter++) {
    s  = addTab(info->callSign,  fltr.maxCallSign);
    s += addTab(info->firstName, fltr.maxFirst);
    s += addTab(info->lastName,  fltr.maxLast);
    s += info->id;

    checkOutCtrl.AddString(s);
    }
  }



void MemberInfoDlg::getMax() {
MbrIter     iter(members);
MemberInfo* mi;

  for (mi = iter(); mi; mi = iter++) {
    getMaxLng(mi->callSign,  maxCallSign);
    getMaxLng(mi->firstName, maxFirstName);
    getMaxLng(mi->lastName,  maxLastName);
    }
  }


void MemberInfoDlg::DoDataExchange(CDataExchange* pDX) {
  CDialogEx::DoDataExchange(pDX);
  DDX_Control(pDX,  IDC_MemberInfo, memberInfoCtrl);
  DDX_CBString(pDX, IDC_MemberInfo, memberInfo);
  DDX_Control(pDX,  IDC_CheckOut,   checkOutCtrl);
  DDX_CBString(pDX, IDC_CheckOut,   checkOut);
  }


BEGIN_MESSAGE_MAP(MemberInfoDlg, CDialogEx)
END_MESSAGE_MAP()


