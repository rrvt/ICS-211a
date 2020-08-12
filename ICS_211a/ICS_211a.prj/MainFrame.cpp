// MainFrm.cpp : implementation of the MainFrame class


#include "stdafx.h"
#include "MainFrame.h"
#include "resource.h"


// MainFrame

IMPLEMENT_DYNCREATE(MainFrame, CFrameWndEx)

BEGIN_MESSAGE_MAP(MainFrame, CFrameWndEx)
  ON_WM_CREATE()
END_MESSAGE_MAP()

static UINT indicators[] = {
  ID_SEPARATOR,           // status line indicator
  ID_INDICATOR_CAPS,
  ID_INDICATOR_NUM,
  ID_INDICATOR_SCRL,
  };

// MainFrame construction/destruction

MainFrame::MainFrame() noexcept { }

MainFrame::~MainFrame() { }


int MainFrame::OnCreate(LPCREATESTRUCT lpCreateStruct) {

  if (CFrameWndEx::OnCreate(lpCreateStruct) == -1) return -1;

  if (!m_wndMenuBar.Create(this)) {TRACE0("Failed to create menubar\n"); return -1;}
  CMFCPopupMenu::SetForceMenuFocus(FALSE);

  if (!m_wndToolBar.CreateEx(this, TBSTYLE_FLAT,
                                        WS_CHILD | WS_VISIBLE | CBRS_TOP | CBRS_TOOLTIPS | CBRS_FLYBY) ||
      !m_wndToolBar.LoadToolBar(IDR_MAINFRAME, 0, 0, TRUE))
       {TRACE0("Failed to create toolbar\n"); return -1;}

  if (!m_wndStatusBar.Create(this)) {TRACE0("Failed to create status bar\n"); return -1;}

  m_wndStatusBar.SetIndicators(indicators, noElements(indicators));  //sizeof(indicators)/sizeof(UINT)

  DockPane(&m_wndMenuBar);
  DockPane(&m_wndToolBar);

  CMFCVisualManager::SetDefaultManager(RUNTIME_CLASS(CMFCVisualManagerWindows7));
                                                                         // Affects look of toolbar, etc.
  return 0;
  }


BOOL MainFrame::PreCreateWindow(CREATESTRUCT& cs) {

  cs.style &= ~FWS_ADDTOTITLE;  cs.lpszName = _T("ICS_211a");         // Sets the default title left part

  return CFrameWndEx::PreCreateWindow(cs);
  }


// MainFrame diagnostics

#ifdef _DEBUG
void MainFrame::AssertValid() const
{
  CFrameWndEx::AssertValid();
}

void MainFrame::Dump(CDumpContext& dc) const
{
  CFrameWndEx::Dump(dc);
}
#endif //_DEBUG


// MainFrame message handlers

