// ICS_211a.cpp : Defines the class behaviors for the application.


#include "stdafx.h"
#include "ICS_211a.h"
#include "ICS_211aDoc.h"
#include "ICS_211aView.h"
#include "About.h"
#include "IniFile.h"
#include "MainFrame.h"
#include "NotePad.h"
#include "Options.h"


TCchar* MemberInfoSect = _T("MemberInfo");
TCchar* LastPathKey    = _T("LastPath");
TCchar* MemberInfoKey  = _T("MemberInfoPath");
TCchar* SJRaces        = _T("SJ RACES");


ICS_211a theApp;                       // The one and only ICS_211a object
IniFile iniFile;


// ICS_211a

BEGIN_MESSAGE_MAP(ICS_211a, CApp)
  ON_COMMAND(ID_APP_ABOUT,        &ICS_211a::OnAppAbout)
  ON_COMMAND(ID_FILE_PRINT_SETUP, &ICS_211a::OnPrinterSetup)
  ON_COMMAND(ID_Help,             &ICS_211a::OnHelp)
END_MESSAGE_MAP()


// ICS_211a initialization

BOOL ICS_211a::InitInstance() {

  CWinAppEx::InitInstance();

  iniFile.setAppDataPath(m_pszHelpFilePath, *this);

  notePad.clear();

  SetRegistryKey(appID);

  LoadStdProfileSettings(0);  // Load standard INI file options (including MRU)

  // Register the application's document templates.  Document templates
  // serve as the connection between documents, frame windows and views

  CSingleDocTemplate* pDocTemplate;

  pDocTemplate = new CSingleDocTemplate(
    IDR_MAINFRAME,
    RUNTIME_CLASS(ICS_211aDoc),
    RUNTIME_CLASS(MainFrame),       // main SDI frame window
    RUNTIME_CLASS(ICS_211aView));

  if (!pDocTemplate) return FALSE;

  AddDocTemplate(pDocTemplate);

  // Parse command line for standard shell commands, DDE, file open

  CCommandLineInfo cmdInfo;   ParseCommandLine(cmdInfo);

  // Dispatch commands specified on the command line.  Will return FALSE if
  // app was launched with /RegServer, /Register, /Unregserver or /Unregister.

  if (!ProcessShellCommand(cmdInfo)) return FALSE;

  setAppName(_T("ICS_211a")); //setTitle(_T("ICS-211 with Bar Code Reader Interface"));

  view()->setFont(_T("Arial"), 120);   view()->setOrientation(Landscape);

  options.load();
  doc()->loadMemberInfo();
  doc()->loadRoster();
  String eventTitle = roster.incidentName + _T(" at the ") + roster.checkInLocation;
  eventTitle += _T(" on ") + roster.date;
  setTitle(eventTitle);                     // Title right part

  doc()->displayRoster();

  m_pMainWnd->ShowWindow(SW_SHOW);   m_pMainWnd->UpdateWindow();   return TRUE;
  }


void ICS_211a::OnPrinterSetup() {view()->setPrntrOrient(getDevMode());   CWinApp::OnFilePrintSetup();}


void ICS_211a::OnHelp() {
String topic = m_pszHelpFilePath; topic += _T(">Introduction");

  ::HtmlHelp(m_pMainWnd->m_hWnd, topic,  HH_DISPLAY_TOC, 0);
  }


void ICS_211a::OnAppAbout() {CAboutDlg aboutDlg; aboutDlg.DoModal();}


