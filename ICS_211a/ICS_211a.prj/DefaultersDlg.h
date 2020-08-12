// Repair Missing Check Out Times


#pragma once
#include "Expandable.h"
#include "Date.h"
#include "Log211.h"


struct AttdDsc {
String    key;
LogDatum* lgdtm;
Date      chkOutTm;

  AttdDsc() : lgdtm(0) { }
 ~AttdDsc() { }
  AttdDsc(AttdDsc& attd) {key = attd.key; lgdtm = attd.lgdtm; chkOutTm = attd.chkOutTm;}

  AttdDsc& operator= (AttdDsc& attd)
                            {key = attd.key; lgdtm = attd.lgdtm; chkOutTm = attd.chkOutTm; return *this;}
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
CEdit     dateCtrl;
CString   date;
CEdit     timeCtrl;
CString   time;
CStatic   median;

           DefaultersDlg(CWnd* pParent = nullptr);   // standard constructor
  virtual ~DefaultersDlg();

// Dialog Data
#ifdef AFX_DESIGN_TIME
  enum { IDD = IDD_CheckOutRepair };
#endif

protected:

  virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

          void updateAttendees();
          bool vrfyHr(  int cnt, TCchar ch, int& v);
          bool vrfyMin( int cnt, TCchar ch, int& v);
          bool vrfyMnth(int cnt, TCchar ch, int& v);
          bool vrfyDay( int cnt, TCchar ch, int& v);
          bool vrfyYr(  int cnt, TCchar ch, int& v);
          void replTmSel(int i, TCchar ch);
          void replDtSel(int i, TCchar ch);

  DECLARE_MESSAGE_MAP()
public:
  afx_msg void OnChangeAttendee();
  afx_msg void OnLeaveTime();
  virtual BOOL OnInitDialog();
  afx_msg void OnOK();
  afx_msg void OnChangeTime();
  afx_msg void OnChangeDate();
  afx_msg void onLeaveDate();

private:

  // returns either a pointer to data (or datum) at index i in array or zero
  AttdDsc* datum(int i) {return 0 <= i && i < nData() ? &attendees[i] : 0;}

  // returns number of data items in array
  int      nData()      {return attendees.end();}

  friend typename DftrIter;
  };
