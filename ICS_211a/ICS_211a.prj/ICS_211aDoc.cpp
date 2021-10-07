// ICS_211aDoc.cpp : implementation of the ICS_211aDoc class


#include "stdafx.h"
#include "ICS_211aDoc.h"
#include "CalibDspPrt.h"
#include "CopyFile.h"
#include "Defaulters.h"
#include "EditEntry.h"
#include "filename.h"
#include "GetPathDlg.h"
#include "ICS_211a.h"
#include "ICS_211aView.h"
#include "IniFile.h"
#include "Log211.h"
#include "Members.h"
#include "MessageBox.h"
#include "Options.h"
#include "Resource.h"
#include "Roster.h"
#include "Scanner.h"


TCchar* Last211PathKey     = _T("Last211Path");
TCchar* MemberInfoSect     = _T("MemberInfo");
TCchar* LastMbrInfoPathKey = _T("LastMbrInfoPath");
TCchar* MemberInfoKey      = _T("MemberInfoPath");


// ICS_211aDoc

IMPLEMENT_DYNCREATE(ICS_211aDoc, CDoc)

BEGIN_MESSAGE_MAP(ICS_211aDoc, CDoc)

  ON_COMMAND(ID_NewICS211a,         &OnNewICS211a)
  ON_COMMAND(ID_EditTitle,          &OnEditTitle)
  ON_COMMAND(ID_OpenRoster,         &onOpenRoster)
  ON_COMMAND(ID_ReadBarCodes,       &OnReadBarCodes)
  ON_COMMAND(ID_Member,             &OnMember)
  ON_COMMAND(ID_Visitor,            &OnVisitor)

  ON_COMMAND(ID_PrepareLog,         &onPrepareLog)
  ON_COMMAND(ID_CheckOutDefaulters, &OnCheckOutDefaulters)

  ON_COMMAND(ID_Excel,              &OnExcel)
  ON_COMMAND(ID_SaveFile,           &OnSaveFile)

  ON_COMMAND(ID_DisplayMembers,     &OnDisplayMembers)
  ON_COMMAND(ID_OrganizeInfo,       &OnOrganizeInfo)
  ON_COMMAND(ID_Options,            &OnOptions)
  ON_COMMAND(ID_CalibDspPrt,        &OnCalibDspPrt)
  ON_COMMAND(ID_EditLogEntry,       &onEditEntry)

END_MESSAGE_MAP()


ICS_211aDoc::ICS_211aDoc() noexcept : dataSource(NotePadSrc), curNote(&notePad) { }


ICS_211aDoc::~ICS_211aDoc() { }


// Load and Create New 211 File

void ICS_211aDoc::onOpenRoster() {
PathDlgDsc pDsc(_T("ICS211 File"), _T(""), _T("211"), _T("*.211"));
String     pth;
bool       rslt;

  iniFile.readString(MemberInfoSect, Last211PathKey, pth);   pDsc.name = pth;

  if (!setIncSavePath(pDsc)) return;

  roster.outputFilePath = path;

  if (GetFileAttributes(path) != -1) {
    backupCopy(path, 5);
    dataSource = RosterSrc;   rslt = OnOpenIncDocument(path);
    }
  else {dataSource = InitRoster;   rslt = OnSaveDocument(path);}

  if (rslt) {iniFile.writeString(MemberInfoSect, Last211PathKey, path);  display(RosterSrc);}
  }


void ICS_211aDoc::OnNewICS211a() {
PathDlgDsc pDsc(_T("New ICS211a File"), _T("*.211"), _T("211"), _T("*.211"));

  notePad.clear();

  if (!setIncSavePath(pDsc)) return;

  if (GetFileAttributes(path) != -1) return;

  roster.outputFilePath = path;

  dataSource = InitRoster;

  if (OnSaveDocument(path))
                        {iniFile.writeString(MemberInfoSect, Last211PathKey, path); display(RosterSrc);}
  }


void ICS_211aDoc::OnReadBarCodes() {notePad.clear(); startBarCodeRead(); display(RosterSrc);}


void ICS_211aDoc::startBarCodeRead() {
ICS_211aView* vw = view();

  if (!loadMemberInfo()) return;

  if (!initScanner()) {messageBox(_T("BarCode Reader is not ready!")); return;}

  theApp.setTitle(_T("Reading Barcodes"));    vw->startBarcode();
  }


bool ICS_211aDoc::initScanner() {
SHORT       ScannerTypesArray[TOTAL_SCANNER_TYPES] = {SCANNER_TYPES_ALL, 0, 0, 0, 0, 0, 0, 0, 0};
long        status;

  notePad << nSetTab(15);

  if (!scanner.open(ScannerTypesArray, 1, status)) return false;

  if (!scanner.getScanners(status)) return false;

  if (!scanner.getVersion(status)) return false;

  return true;
  }


void ICS_211aDoc::OnEditTitle() {

  view()->stopBarcode();   roster.editTitle();

  backupFile(roster.path(), 5);   dataSource = RosterSrc;

  if (!OnSaveDocument(roster.path())) {messageBox(_T("Unable to Save Data")); return;}

  invalidate();
  }


void ICS_211aDoc::OnMember() {

  notePad.clear();

  if (!loadMemberInfo()) return;

  view()->stopBarcode();   roster.addMember();   display(RosterSrc);
  }


// Load member info from Roaming Directory

bool ICS_211aDoc::loadMemberInfo() {
PathDlgDsc pDsc(_T("Member Info"), _T("dbMemberInfo.txt"), _T("txt"), _T("*.txt"));

  if (members.isLoaded()) return true;

  dataSource = MemberSrc;

  iniFile.readString(MemberInfoSect, MemberInfoKey, path);

  if (OnOpenDocument(path)) return true;

//  pDsc = {_T("Member Info"), _T("dbMemberInfo.txt"), _T("txt"), _T("*.txt")};

  return setPath(pDsc) && OnOpenDocument(path);
  }


void ICS_211aDoc::OnVisitor()
                  {notePad.clear();  view()->stopBarcode();  roster.addVisitor();  display(RosterSrc);}


// Prepare and edit spreadsheet Report

void ICS_211aDoc::onPrepareLog()
   {notePad.clear();  view()->stopBarcode();  theApp.setTitle(_T("Display Log"));   display(Log211Src);}


void ICS_211aDoc::OnCheckOutDefaulters() {
Defaulters defaulters;

  notePad.clear();   view()->stopBarcode();   defaulters();   display(Log211Src);
  }


// Tool Command Implementations

void ICS_211aDoc::OnDisplayMembers() {notePad.clear();   display(MemberSrc);}


void ICS_211aDoc::OnOrganizeInfo() {
String pth;

  notePad.clear();

  dataSource = MemberSrc;   view()->stopBarcode();

  iniFile.readString(MemberInfoSect, LastMbrInfoPathKey, pth);

  pathDsc = PathDlgDsc(_T("Member Info"), pth, _T("txt"), _T("*.txt"));

  if (setPath(pathDsc)) {

    iniFile.writeString(MemberInfoSect, LastMbrInfoPathKey, path);

    pth = ::getPath(iniFile.path());   pth += _T("dbMemberInfo.txt");

    copyFile(path, pth);

    iniFile.writeString(MemberInfoSect, MemberInfoKey, pth);

    if (!OnOpenDocument(pth)) {messageBox(_T(" Not Loaded!")); return;}

    notePad << _T("Member Information Updated") << nCrlf << nCrlf;

    dataSource = MemberSrc;
    }

  else notePad << _T("Update failed.") << nCrlf;

  invalidate();
  }


void ICS_211aDoc::OnOptions() {
PrtrOrient orient;

  notePad.clear();   options();

  switch (dataSource) {
    case RosterSrc: orient = options.rstrOrient;    break;
    case MemberSrc: orient = options.mbrInfoOrient; break;
    case Log211Src: orient = options.logOrient;     break;
    }

  view()->setOrientation(orient);
  }


void ICS_211aDoc::OnCalibDspPrt() {CalibDspPrt calib;  calib();  display(NotePadSrc);}


void ICS_211aDoc::onEditEntry() {
EditEntry editEntry;

  notePad.clear();

  view()->stopBarcode();   log211.prepare();

  if (editEntry()) {

    backupFile(roster.path(), 5);   dataSource = RosterSrc;

    if (!OnSaveDocument(roster.path())) {messageBox(_T("Unable to Save Data")); return;}
    }

  display(Log211Src);
  }


// Save a single line item from the Roster

bool ICS_211aDoc::saveDtm() {dataSource = IncSrc;   return reOpenDocument();}


void ICS_211aDoc::OnExcel() {
String fileName = roster.outputFilePath;
int    pos      = fileName.find_last_of(_T('\\'));
String fileTyp  = _T("csv");
String ext      = _T("*.") + fileTyp;

  if (!log211.prepare()) return;

  fileName = fileName.substr(pos+1);   pos = fileName.find_first_of(_T('.'));
  fileName = fileName.substr(0, pos);

  dataSource = CSVSrc;   pathDsc = PathDlgDsc(_T("SpreadSheet Output"), fileName, fileTyp, ext);

  if (setSaveAsPath(pathDsc)) OnSaveDocument(path);

  display(Log211Src);
  }


void ICS_211aDoc::setOrient(PrtrOrient orient) {

  switch (dataSource) {
    case RosterSrc: options.setRstrOrient(orient); break;
    case MemberSrc: options.setInfoOrient(orient); break;
    case Log211Src: options.setLogOrient( orient); break;
    }

  view()->setOrientation(orient);
  }


void ICS_211aDoc::display(DataSource ds) {
  dataSource = ds;

  switch (dataSource) {
    case Log211Src: setFileSaveAttr(_T("Log"),         _T("Log"),  _T("txt")); break;
    case MemberSrc: setFileSaveAttr(_T("Member List"), _T("Mbrs"), _T("txt")); break;
    case RosterSrc: setFileSaveAttr(_T("Roster"),      _T("Rstr"), _T("txt")); break;
    }

  invalidate();
  }


void ICS_211aDoc::OnSaveFile()
              {dataSource = NotePadSrc;   saveFile(saveTitle, saveSuffix, saveFileType);  invalidate();}


void ICS_211aDoc::saveFile(TCchar* title, TCchar* suffix, TCchar* fileType) {
String fileName = roster.outputFilePath;
int    pos      = fileName.find_last_of(_T('\\'));
String ext      = _T("*."); ext += fileType;
String ttl      = title;    ttl += _T(" Output");

  fileName = fileName.substr(pos+1);   pos = fileName.find_first_of(_T('.'));
  fileName = fileName.substr(0, pos);  fileName += suffix;

  pathDsc = PathDlgDsc(ttl, fileName, fileType, ext);

  if (setSaveAsPath(pathDsc)) OnSaveDocument(path);
  }


// ICS_211aDoc serialization

void ICS_211aDoc::serialize(Archive& ar) {

  switch (ar.isStoring()) {
    case true:
      switch(dataSource) {
        case NotePadSrc : curNote->archive(ar);  break;;
        case CSVSrc     : log211.output(ar);     break;
        case InitRoster : roster.initialize(ar); // And fall throught to store!
        case RosterSrc  : roster.store(ar);      break;
        case IncSrc     : roster.incStore(ar);   break;
        default         : break;
        }
      break;

    case false:
      switch(dataSource) {
        case MemberSrc  : members.load(ar); break;
        case RosterSrc  : roster.load(ar);  break;
        default         : break;
        }
      break;
    }

  dataSource = RosterSrc;
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

