// MainFrm.cpp : implementation of the MainFrame class


#include "stdafx.h"
#include "MainFrame.h"
#include "resource.h"


// MainFrame

IMPLEMENT_DYNCREATE(MainFrame, CFrameWndEx)

BEGIN_MESSAGE_MAP(MainFrame, CFrameWndEx)
  ON_WM_CREATE()
  ON_REGISTERED_MESSAGE(AFX_WM_RESETTOOLBAR, &OnResetToolBar)              // MainFrame::
  ON_WM_SETFOCUS()
END_MESSAGE_MAP()


static UINT indicators[] = {
  ID_SEPARATOR,           // status line indicator
  ID_INDICATOR_CAPS,
  ID_INDICATOR_NUM,
  ID_INDICATOR_SCRL,
  };

// MainFrame construction/destruction

MainFrame::MainFrame() noexcept : isInitialized(false) { }

MainFrame::~MainFrame() { }


BOOL MainFrame::PreCreateWindow(CREATESTRUCT& cs) {

  cs.style &= ~FWS_ADDTOTITLE;  cs.lpszName = _T("ICS_211a");         // Sets the default title left part

  return CFrameWndEx::PreCreateWindow(cs);
  }


int MainFrame::OnCreate(LPCREATESTRUCT lpCreateStruct) {
CRect winRect;

  if (CFrameWndEx::OnCreate(lpCreateStruct) == -1) return -1;

  if (!m_wndMenuBar.Create(this)) {TRACE0("Failed to create menubar\n"); return -1;}
  CMFCPopupMenu::SetForceMenuFocus(FALSE);

  if (!toolBar.CreateEx(this, TBSTYLE_FLAT,
                                        WS_CHILD | WS_VISIBLE | CBRS_TOP | CBRS_TOOLTIPS | CBRS_FLYBY) ||
      !toolBar.LoadToolBar(IDR_MAINFRAME, 0, 0, TRUE)) {TRACE0("Failed to create toolbar\n"); return -1;}

  if (!m_wndStatusBar.Create(this)) {TRACE0("Failed to create status bar\n"); return -1;}

  m_wndStatusBar.SetIndicators(indicators, noElements(indicators));  //sizeof(indicators)/sizeof(UINT)

  GetWindowRect(&winRect);   winPos.initialPos(this, winRect);

  DockPane(&m_wndMenuBar);   DockPane(&toolBar);

  CMFCVisualManager::SetDefaultManager(RUNTIME_CLASS(CMFCVisualManagerWindows7));
                                                                         // Affects look of toolbar, etc.
  isInitialized = true;   return 0;
  }


void MainFrame::OnMove(int x, int y)
           {CRect winRect;   GetWindowRect(&winRect);   winPos.set(winRect);   CFrameWndEx::OnMove(x, y);}


void MainFrame::OnSize(UINT nType, int cx, int cy) {
CRect winRect;

  CFrameWndEx::OnSize(nType, cx, cy);

  if (!isInitialized) return;

  GetWindowRect(&winRect);   winPos.set(winRect);
  }


// MainFrame message handlers

afx_msg LRESULT MainFrame::OnResetToolBar(WPARAM wParam, LPARAM lParam) {setupToolBar();  return 0;}


void MainFrame::setupToolBar() { }


// MainFrame diagnostics

#ifdef _DEBUG
void MainFrame::AssertValid() const {CFrameWndEx::AssertValid();}

void MainFrame::Dump(CDumpContext& dc) const {CFrameWndEx::Dump(dc);}
#endif //_DEBUG

