// ICS_211a.h : main header file for the ICS_211a application

#pragma once
#include "CApp.h"
#include "IniFile.h"


extern TCchar* MemberInfoSect;
extern TCchar* LastPathKey;
extern TCchar* MemberInfoKey;


class ICS_211aDoc;
class ICS_211aView;



class ICS_211a : public CApp {

public:
               ICS_211a() noexcept { }

  virtual BOOL InitInstance();


  afx_msg void OnAppAbout();
  afx_msg void OnPrinterSetup();
  afx_msg void OnHelp();

  DECLARE_MESSAGE_MAP()

private:

//  MainFrame*   getMainFrame() {return (MainFrame*) m_pMainWnd;}
  ICS_211aDoc*  doc()  {return (ICS_211aDoc*)  CApp::getDoc();}
  ICS_211aView* view() {return (ICS_211aView*) CApp::getView();}

public:
  friend ICS_211aDoc*  doc();
  friend ICS_211aView* view();
  };


extern ICS_211a theApp;

inline void          invalidate() {theApp.invalidate();}
inline ICS_211aDoc*  doc()  {return theApp.doc();}
inline ICS_211aView* view() {return theApp.view();}

