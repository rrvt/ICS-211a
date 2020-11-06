// ICS_211a.h : main header file for the ICS_211a application

#pragma once
#include "CApp.h"


class ICS_211aDoc;
class ICS_211aView;
class MainFrame;


class ICS_211a : public CApp {

public:
               ICS_211a() noexcept { }

  virtual BOOL InitInstance();

  ICS_211aDoc*  doc()  {return (ICS_211aDoc*)  CApp::getDoc();}
  ICS_211aView* view() {return (ICS_211aView*) CApp::getView();}
  MainFrame*    mainFrm() {return (MainFrame*) m_pMainWnd;}

  afx_msg void OnPrinterSetup();
  afx_msg void OnAppAbout();
  afx_msg void OnHelp();

  DECLARE_MESSAGE_MAP()
  };


extern ICS_211a theApp;

inline void          invalidate() {theApp.invalidate();}
inline ICS_211aDoc*  doc()  {return theApp.doc();}
inline ICS_211aView* view() {return theApp.view();}

