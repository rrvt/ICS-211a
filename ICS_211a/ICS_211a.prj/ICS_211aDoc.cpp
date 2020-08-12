// ICS_211aDoc.cpp : implementation of the ICS_211aDoc class


#include "stdafx.h"
#include "ICS_211aDoc.h"
#include "CopyFile.h"
#include "Defaulters.h"
#include "EditEntry.h"
#include "filename.h"
#include "GetPathDlg.h"
#include "ICS_211a.h"
#include "ICS_211aView.h"
#include "Log211.h"
#include "MemberInfo.h"
#include "MessageBox.h"
#include "Options.h"
#include "Resource.h"
#include "Scanner.h"


extern TCchar* MemberInfoSect;
extern TCchar* MemberInfoKey;
extern TCchar* SJRaces;

// ICS_211aDoc

IMPLEMENT_DYNCREATE(ICS_211aDoc, CDoc)

BEGIN_MESSAGE_MAP(ICS_211aDoc, CDoc)
  ON_COMMAND(ID_EditTitle,          &ICS_211aDoc::OnEditTitle)
  ON_COMMAND(ID_ReadBarCodes,       &ICS_211aDoc::OnReadBarCodes)
  ON_COMMAND(ID_Member,             &ICS_211aDoc::OnMember)
  ON_COMMAND(ID_Visitor,            &ICS_211aDoc::OnVisitor)

  ON_COMMAND(ID_PrepareLog,         &ICS_211aDoc::onPrepareLog)
  ON_COMMAND(ID_CheckOutDefaulters, &ICS_211aDoc::OnCheckOutDefaulters)
  ON_COMMAND(ID_SaveFile,           &ICS_211aDoc::OnSaveFile)
  ON_COMMAND(ID_OrganizeInfo,       &ICS_211aDoc::OnOrganizeInfo)
  ON_COMMAND(ID_Options,            &ICS_211aDoc::OnOptions)
  ON_COMMAND(ID_EditLogEntry,       &ICS_211aDoc::onEditEntry)

END_MESSAGE_MAP()


// ICS_211aDoc construction/destruction

ICS_211aDoc::ICS_211aDoc() noexcept : storeType(NilStore) {
  saveAsTitle = _T("SpreadSheet Output");   defExt = _T("dsc");   defFilePat = _T("*.csv");
  }


ICS_211aDoc::~ICS_211aDoc() { }


static TCchar* MbrInfoTitle = _T("Member Info");
static TCchar* MbrInfoDef   = _T("dbMemberInfo.txt");
static TCchar* MbrInfoExt   = _T("txt");
static TCchar* MbrInfoPat   = _T("*.txt");


bool ICS_211aDoc::loadMemberInfo() {
String path;

  iniFile.readString(MemberInfoSect, MemberInfoKey, path);

  if (OnOpenDocument(path)) return true;

  if (!getPathDlg(MbrInfoTitle, MbrInfoDef, MbrInfoExt, MbrInfoPat, path)) return false;

  return OnOpenDocument(path);
  }


void ICS_211aDoc::OnReadBarCodes() {startBarCodeRead(); invalidate();}


void ICS_211aDoc::startBarCodeRead() {
ICS_211aView* vw = view();

  if (!members.isLoaded() && !loadMemberInfo()) return;

  if (!initScanner()) {messageBox(_T("BarCode Reader is not ready!")); return;}

  theApp.setTitle(_T("Reading Barcodes"));    vw->dsplyReport(false);

  vw->startBarcode();
  }


bool ICS_211aDoc::initScanner() {
SHORT       ScannerTypesArray[TOTAL_SCANNER_TYPES] = {SCANNER_TYPES_ALL, 0, 0, 0, 0, 0, 0, 0, 0};
long        status;

  notePad << nSetTab(15);

  if (!scanner.open(ScannerTypesArray, 1, status)) return false;

  if (!scanner.getScanners(status)) return false;

  if (!scanner.getVersion(status)) return false;

//  notePad << _T("Version:") << nTab << scanner.version << nCrlf;
  return true;
  }



void ICS_211aDoc::OnEditTitle() {

  view()->stopBarcode();   roster.editTitle();

  backupFile(roster.path(), 5);   storeType = RosterStore;

  if (!OnSaveDocument(roster.path())) {messageBox(_T("Unable to Save Data")); return;}

  display();   invalidate();
  }


void ICS_211aDoc::OnMember() {

  if (!members.isLoaded() && !loadMemberInfo()) return;

  view()->stopBarcode();   roster.addMember();   display();   invalidate();
  }


void ICS_211aDoc::OnVisitor() {view()->stopBarcode();   roster.addVisitor();   display();   invalidate();}


//void ICS_211aDoc::OnDisplay() {display();   invalidate();}


void ICS_211aDoc::display() {notePad.clear();  roster.display();   startBarCodeRead();}


void ICS_211aDoc::OnOrganizeInfo() {
String pth;
String path;

  view()->stopBarcode();

  iniFile.readString(MemberInfoSect, LastPathKey, pth);

  if (getPathDlg(_T("Member Info"), pth, _T("txt"), _T("*.txt"), path)) {

    iniFile.writeString(MemberInfoSect, LastPathKey, path);

    pth = getPath(iniFile.path());   pth += _T("dbMemberInfo.txt");

    copyFile(path, pth);

    iniFile.writeString(MemberInfoSect, MemberInfoKey, pth);

    if (!OnOpenDocument(pth)) {messageBox(_T(" Not Loaded!")); return;}

    notePad.clear();

    notePad << _T("Member Information Updated") << nCrlf << nCrlf;

    members.display();   invalidate();  return;
    }

  notePad << _T("Update failed.") << nCrlf; invalidate();
  }




void ICS_211aDoc::onPrepareLog() {

  view()->stopBarcode();   log211.prepare();   view()->dsplyReport();

  storeType = ReportStore;   invalidate();
  }


void ICS_211aDoc::OnCheckOutDefaulters() {
Defaulters defaulters;

  view()->stopBarcode();   log211.prepare();

  defaulters();

  view()->dsplyReport();   invalidate();
  }


void ICS_211aDoc::onEditEntry() {
EditEntry editEntry;

  view()->stopBarcode();   log211.prepare();

  if (editEntry()) {

    backupFile(roster.path(), 5);   storeType = RosterStore;

    if (!OnSaveDocument(roster.path())) {messageBox(_T("Unable to Save Data")); return;}
    }

  view()->dsplyReport();   invalidate();
  }


void ICS_211aDoc::OnOptions() {Options options; options();}


void ICS_211aDoc::OnSaveFile() {
String path;

  defFileName   = _T("ICS_211a.csv");

  if (!getSaveAsPathDlg(saveAsTitle, defFileName, defExt, defFilePat, path)) return;

  OnSaveDocument(path);
  }


// ICS_211aDoc serialization

void ICS_211aDoc::serialize(Archive& ar) {
  if (ar.isStoring()) {
    switch (storeType) {
      case ReportStore: log211.output(ar); break;;
      case RosterStore: roster.store(ar);     break;
      }

    storeType = NilStore;  return;
    }

  members.load(ar);
  }


// ICS_211aDoc diagnostics

#ifdef _DEBUG
void ICS_211aDoc::AssertValid() const
{
  CDocument::AssertValid();
}

void ICS_211aDoc::Dump(CDumpContext& dc) const
{
  CDocument::Dump(dc);
}
#endif //_DEBUG



#if 0
void ICS_211aDoc::OnFileOpen() {
String path;

  saveAsTitle = _T("ICS_211a");   defExt = _T("txt");   defFilePat = _T("*.txt");

  if (!getPathDlg(saveAsTitle, defFileName, defExt, defFilePat, path)) return;

  defFileName = getMainName(path);   view()->rightFooter= defFileName;  view()->date.getToday();

  notePad.clear();

  if (!OnOpenDocument(path)) messageBox(_T(" Not Loaded!"));

  dataStore.setName(defFileName);  invalidate();
  }
#endif

