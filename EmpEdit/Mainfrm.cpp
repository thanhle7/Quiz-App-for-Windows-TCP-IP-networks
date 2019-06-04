// MainFrm.cpp : implementation of the CMainFrame class
//

#include "stdafx.h"
#include "EmpED.h"
#include "MainFrm.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CMainFrame

IMPLEMENT_DYNCREATE(CMainFrame, CFrameWnd)

BEGIN_MESSAGE_MAP(CMainFrame, CFrameWnd)
	//{{AFX_MSG_MAP(CMainFrame)
	ON_WM_CREATE()
	//}}AFX_MSG_MAP
	ON_CBN_SETFOCUS(ID_FORMAT_FONT, LockComboFont)
	ON_UPDATE_COMMAND_UI(ID_FORMAT_UNDERLINE, OnUpdateFormatUnderline)
	ON_UPDATE_COMMAND_UI(ID_FORMAT_BOLD, OnUpdateFormatBold)
	ON_UPDATE_COMMAND_UI(ID_FORMAT_ITALIC, OnUpdateFormatItalic)
	ON_UPDATE_COMMAND_UI(ID_FORMAT_STRIKEOUT, OnUpdateFormatStrikeout)
	ON_UPDATE_COMMAND_UI(ID_FORMAT_SUBSCRIPT, OnUpdateFormatSubscript)
	ON_UPDATE_COMMAND_UI(ID_FORMAT_SUPERSCRIPT, OnUpdateFormatSuperscript)
	ON_UPDATE_COMMAND_UI(ID_FORMAT_FONT, OnUpdateFormatFont)
	ON_COMMAND(ID_HELP, OnHelp)
	ON_COMMAND(ID_EDIT_OLE_SHOW, DoObjectView)
	ON_WM_ACTIVATEAPP()
	ON_WM_ACTIVATE()
	ON_UPDATE_COMMAND_UI(ID_EDIT_OLE_SHOW, OnUpdateEditOleShow)
	ON_WM_DESTROY()
END_MESSAGE_MAP()

static UINT indicators[] =
{
	ID_SEPARATOR,           // status line indicator
	ID_INDICATOR_CAPS,
	ID_INDICATOR_NUM,
	ID_INDICATOR_SCRL,
};

/////////////////////////////////////////////////////////////////////////////
// CMainFrame construction/destruction

CMainFrame::CMainFrame()
{
	m_pWndObjView = NULL;
}

CMainFrame::~CMainFrame()
{
}

void CMainFrame::OnDestroy()
{
	if (m_pWndObjView != NULL) {
		delete (*m_pWndObjView);
		*m_pWndObjView = NULL;
	}

	CFrameWnd::OnDestroy();
}

// Active the view frame to show selected object
void CMainFrame::DoObjectView(void)
{
	if (m_pWndObjView == NULL) {
		m_pWndObjView  = &(m_view->GetDocument()->m_wndObjView);
		*m_pWndObjView = new CObjectView();
		if (!(*m_pWndObjView)->CreateEx(0, NULL, "ObjectView", 
				WS_CHILD|WS_POPUP|WS_CAPTION|WS_SYSMENU,
				37,0,147,110, this->m_hWnd, NULL)) 
			{
				// error in creating view frame window
				delete *m_pWndObjView;
				*m_pWndObjView = NULL;
				m_pWndObjView = NULL;
				return;
			}
	}

	(*m_pWndObjView)->DisplayView(m_view->GetSelectedItem());
}

int CMainFrame::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CFrameWnd::OnCreate(lpCreateStruct) == -1)
		return -1;

	if (!m_wndToolBar.CreateEx(this, TBSTYLE_FLAT, WS_CHILD | WS_VISIBLE | CBRS_TOP
		| CBRS_TOOLTIPS | CBRS_FLYBY | CBRS_SIZE_FIXED) ||
		!m_wndToolBar.LoadToolBar(IDR_MAINFRAME))
	{
		TRACE0("Failed to create toolbar\n");
		return -1;
	}

	m_tip.Create(this);
	m_tip.SetTipBkColor(RGB(240,200,240));

	LOGFONT lf;
	memset(&lf, 0, sizeof(LOGFONT));
	strcpy(lf.lfFaceName, "EMP.font");
	lf.lfHeight = 16;
	lf.lfWidth = 12;
	m_vnFont.CreateFontIndirect(&lf);

	int fIdx = m_wndToolBar.CommandToIndex(ID_FORMAT_FONT);
	CRect rect;
	m_wndToolBar.GetItemRect(fIdx, rect);
	m_wndToolBar.SetButtonInfo(fIdx, ID_FORMAT_FONT, 
				TBBS_SEPARATOR, 150 );

	rect.top += 1;
	rect.right = rect.left + 150;
	rect.bottom = rect.top + 200;
	m_cmbFontName.Create(WS_CHILD|WS_VISIBLE|WS_VSCROLL|CBS_DROPDOWNLIST, 
		rect, &m_wndToolBar, ID_FORMAT_FONT);

	m_cmbFontName.SetFont(&m_vnFont);
	m_cmbFontName.AddString("1. Font ch§ vèn bàn");
	m_cmbFontName.AddString("2. Font ch§ kî hi¬u");
	m_cmbFontName.SetCurSel(0);
	m_cmbFontNameFree = TRUE;

	fIdx = m_wndToolBar.CommandToIndex(ID_FORMAT_COLOR);
	m_wndToolBar.GetItemRect(fIdx, rect);
	m_wndToolBar.SetButtonInfo(fIdx, ID_FORMAT_COLOR, TBBS_SEPARATOR, 50 );
	rect.right = rect.left + 50;
	rect.DeflateRect(1,1,1,1);
	m_btnColor.Create(_T(""), WS_CHILD|WS_VISIBLE|BS_OWNERDRAW|BS_PUSHBUTTON, 
		rect, &m_wndToolBar, ID_FORMAT_COLOR); 
	m_btnColor.m_Color = RGB(0,160,160);

	if (!m_wndStatusBar.Create(this)) return -1;
	m_wndStatusBar.SetFont(&m_vnFont);
	
	if (!m_wndStatusBar.SetIndicators(indicators,
		  sizeof(indicators)/sizeof(UINT)))
		return -1;

	m_wndStatusBar.GetStatusBarCtrl().SetMinHeight(20);

	return 0;
}

BOOL CMainFrame::PreCreateWindow(CREATESTRUCT& cs)
{
	cs.cx = cs.cy = 0;
	if( !CFrameWnd::PreCreateWindow(cs) )
		return FALSE;
	cs.style = WS_OVERLAPPED | WS_CAPTION | FWS_ADDTOTITLE
		| WS_THICKFRAME | WS_SYSMENU | WS_MINIMIZEBOX | WS_MAXIMIZEBOX | WS_MAXIMIZE;

	return TRUE;
}

BOOL CMainFrame::OnCreateClient(LPCREATESTRUCT lpcs, CCreateContext* pContext)
{
	// 1 row, 2 columns 
	if ( !m_wndSplitter.CreateStatic(this, 1, 2) ) return FALSE;
	m_wndSplitter.CreateView(0,0,RUNTIME_CLASS(CEmplView), CSize(60,0), pContext);
	m_wndSplitter.CreateView(0,1,pContext->m_pNewViewClass, CSize(0,0), pContext);
	m_wndSplitter.SetActivePane(0,1);
	m_view = (CEmpView*) m_wndSplitter.GetPane(0,1);

	return TRUE;
}

// Update char format button face
void CMainFrame::UpdateBtnFace(UINT cmdID, DWORD cfEffect)
{
	CToolBarCtrl& tb = m_wndToolBar.GetToolBarCtrl();
	if (cfEffect == CFM_FACE) {
		if (m_view->HasCharFormat(cfEffect))
			m_cmbFontName.SetCurSel(1);
		else
			m_cmbFontName.SetCurSel(0);
	}
	else
		tb.PressButton(cmdID, m_view->HasCharFormat(cfEffect));
}

void CMainFrame::OnUpdateFormatUnderline(CCmdUI *pCmdUI)
{
	UpdateBtnFace(ID_FORMAT_UNDERLINE, CFM_UNDERLINE);
}

void CMainFrame::OnUpdateFormatBold(CCmdUI *pCmdUI)
{
	UpdateBtnFace(ID_FORMAT_BOLD, CFM_BOLD);
}

void CMainFrame::OnUpdateFormatItalic(CCmdUI *pCmdUI)
{
	UpdateBtnFace(ID_FORMAT_ITALIC, CFM_ITALIC);
}

void CMainFrame::OnUpdateFormatStrikeout(CCmdUI *pCmdUI)
{
	UpdateBtnFace(ID_FORMAT_STRIKEOUT, CFM_STRIKEOUT);
}

void CMainFrame::OnUpdateFormatSubscript(CCmdUI *pCmdUI)
{
	UpdateBtnFace(ID_FORMAT_SUBSCRIPT, CFE_SUBSCRIPT);
}

void CMainFrame::OnUpdateFormatSuperscript(CCmdUI *pCmdUI)
{
	UpdateBtnFace(ID_FORMAT_SUPERSCRIPT, CFE_SUPERSCRIPT);
}

void CMainFrame::OnUpdateFormatFont(CCmdUI *pCmdUI)
{
	if (m_cmbFontNameFree)
		UpdateBtnFace(ID_FORMAT_FONT, CFM_FACE);
}

void CMainFrame::LockComboFont(void) 
{
	m_cmbFontNameFree = FALSE;
}

void CMainFrame::HideHelp()
{
	if (m_tip.IsWindowVisible())
		m_tip.Hide();
}

void CMainFrame::OnHelp()
{
	CWnd* pWnd = GetActiveWindow();

	if (pWnd != NULL) {
		POINT point;
		int	nID;
		CString tb;

		GetCursorPos(&point);
		pWnd->ScreenToClient(&point);
		CWnd* pchidWnd = pWnd->ChildWindowFromPoint(point);
		if (pchidWnd != NULL) {
			pWnd->ClientToScreen(&point);
			ScreenToClient(&point);
			nID = pchidWnd->GetDlgCtrlID();
			if (nID > 0 && tb.LoadString(nID)) {
				CRect	rect;
				LOGFONT lf;
				m_vnFont.GetLogFont(&lf);
				pchidWnd->GetWindowRect(rect);
				ScreenToClient(rect);
				point.y += 20;
				m_tip.ShowInfoTip(5, point, tb, 5, rect, &lf, 65);
				return;
			}
		}
	}
	CFrameWnd::OnHelpFinder();
}

void CMainFrame::OnActivateApp(BOOL bActive, DWORD dwThreadID)
{
	HideHelp();

	CFrameWnd::OnActivateApp(bActive, dwThreadID);
}

void CMainFrame::OnActivate(UINT nState, CWnd* pWndOther, BOOL bMinimized)
{
	HideHelp();

	CFrameWnd::OnActivate(nState, pWndOther, bMinimized);
}

void CMainFrame::OnUpdateEditOleShow(CCmdUI *pCmdUI)
{
	pCmdUI->Enable(m_view->GetSelectedItem() != NULL);
}
