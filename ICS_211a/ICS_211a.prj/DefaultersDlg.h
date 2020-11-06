// Repair Missing Check Out Times


#pragma once
#include "Date.h"
#include "Log211.h"
#include "Resource.h"


struct AttdDsc {
String    key;
LogDatum* lgdtm;
Date      chkOutTm;

  AttdDsc() : lgdtm(0) { }
 ~AttdDsc() { }
  AttdDsc(AttdDsc& a) {copy(a);}

  AttdDsc& operator= (AttdDsc& attd) {copy(attd); return *this;}

private:

  void copy(AttdDsc& a) {key = a.key; lgdtm = a.lgdtm; chkOutTm = a.chkOutTm;}
  };


class DefaultersDlg;
typedef IterT<DefaultersDlg, AttdDsc> DftrIter;


// DefaultersDlg dialog

class DefaultersDlg : public CDialogEx {

  DECLARE_DYNAMIC(DefaultersDlg)

bool updateTime;
bool updateDate;

public:

Expandable<AttdDsc, 2> attendees;   // Initialization Variables
CString                medcs;

CComboBox attendeeCtrl;             // Dialog Variables
CString   attendee;
CStatic   checkInCtrl;
CEdit     choDateCtrl;
CString   choDate;
CEdit     choTimeCtrl;
CString   choTime;
CStatic   median;
String    medDate;
String    medTime;

           DefaultersDlg(CWnd* pParent = nullptr);   // standard constructor
  virtual ~DefaultersDlg();

  virtual BOOL OnInitDialog();

// Dialog Data
#ifdef AFX_DESIGN_TIME
  enum { IDD = IDD_CheckOutRepair };
#endif

protected:

  virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

          void saveChkOut();
     LogDatum* find(TCchar* key);

  DECLARE_MESSAGE_MAP()

private:

  // returns either a pointer to data (or datum) at index i in array or zero
  AttdDsc* datum(int i) {return 0 <= i && i < nData() ? &attendees[i] : 0;}

  int      nData()      {return attendees.end();}             // returns number of data items in array

public:

  afx_msg void OnChangeAttendee();
  afx_msg void OnChangeTime();
  afx_msg void OnChangeDate();
  afx_msg void onLeaveDate();
  afx_msg void OnLeaveTime();
  afx_msg void OnUpdate();
  afx_msg void OnOK();

  friend typename DftrIter;
  };

