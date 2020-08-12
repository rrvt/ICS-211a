// Edit Log Entry Dialog


#pragma once
#include "Log211.h"
#include "Resource.h"


struct EntryDsc {
String    key;
LogDatum* lgdtm;

  EntryDsc() : lgdtm(0) { }
 ~EntryDsc() {lgdtm = 0; key.clear();}

  EntryDsc& operator= (EntryDsc& dsc) {key = dsc.key; lgdtm = dsc.lgdtm; return *this;}
  };


class EditEntryDlg;
typedef IterT<EditEntryDlg, EntryDsc> EEDIter;


// EditEntryDlg dialog

class EditEntryDlg : public CDialogEx {

  DECLARE_DYNAMIC(EditEntryDlg)

Expandable<EntryDsc, 2> entries;

public:
CComboBox logEntryCtrl;
CEdit     fccCtrl;
CString   fcc;
CEdit     firstNameCtrl;
CString   firstName;
CEdit     lastNameCtrl;
CString   lastName;
CEdit     chkInDtCtrl;
CString   chkInDt;
CEdit     chkInTmCtrl;
CString   chkInTm;
CEdit     chkOutDtCtrl;
CString   chkOutDt;
CEdit     chkOutTmCtrl;
CString   chkOutTm;

LogDatum* lgdtm;

           EditEntryDlg(CWnd* pParent = nullptr);   // standard constructor

  virtual ~EditEntryDlg();

// Dialog Data
#ifdef AFX_DESIGN_TIME
  enum { IDD = IDD_EditEntry };
#endif

protected:

  virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

  DECLARE_MESSAGE_MAP()
public:

  virtual BOOL OnInitDialog();
  afx_msg void OnSelchangeLogentry();

private:

  // returns either a pointer to data (or datum) at index i in array or zero
  EntryDsc* datum(int i) {return 0 <= i && i < nData() ? &entries[i] : 0;}

  // returns number of data items in array
  int   nData()      {return entries.end();}

  friend typename EEDIter;
public:
  afx_msg void onChangeDateIn();
  afx_msg void onChangeTimeIn();
  afx_msg void onChangeDateOut();
  afx_msg void onChangeTimeOut();
};
