// EventInfoDlg.cpp : implementation file
//

#include "stdafx.h"
#include "EventInfoDlg.h"
#include "Date.h"
#include "Resource.h"


// EventInfoDlg dialog

IMPLEMENT_DYNAMIC(EventInfoDlg, CDialogEx)


BEGIN_MESSAGE_MAP(EventInfoDlg, CDialogEx)
  ON_EN_CHANGE(IDC_Date, &EventInfoDlg::OnEnChangeDate)
END_MESSAGE_MAP()


EventInfoDlg::EventInfoDlg(CWnd* pParent) : CDialogEx(IDD_EventInfo, pParent), name(_T("")),
            location(_T("")), incidentNo(_T("")), preparedBy(_T("")), missionNo(_T("")), date(_T("")) { }

EventInfoDlg::~EventInfoDlg() { }


void EventInfoDlg::DoDataExchange(CDataExchange* pDX) {
  CDialogEx::DoDataExchange(pDX);
  DDX_Text(   pDX, IDC_EventName,     name);
  DDX_Text(   pDX, IDC_Date,          date);
  DDX_Text(   pDX, IDC_EventLocation, location);
  DDX_Text(   pDX, IDC_EDIT2,         incidentNo);
  DDX_Text(   pDX, IDC_EDIT3,         preparedBy);
  DDX_Text(   pDX, IDC_EDIT4,         missionNo);
  DDX_Control(pDX, IDC_Date,          dateCtrl);
}


// EventInfoDlg message handlers

void EventInfoDlg::OnEnChangeDate() {Date::onChangeDate(dateCtrl);}
