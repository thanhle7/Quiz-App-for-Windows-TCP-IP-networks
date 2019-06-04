// EmpSView.cpp : implementation file
//

#include "stdafx.h"
#include "emp.h"
#include "EmpSView.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CEmpStdView

IMPLEMENT_DYNCREATE(CEmpStdView, CFormView)

CEmpStdView::CEmpStdView()
	: CFormView(CEmpStdView::IDD)
{
	//{{AFX_DATA_INIT(CEmpStdView)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
}

CEmpStdView::~CEmpStdView()
{
}

void CEmpStdView::DoDataExchange(CDataExchange* pDX)
{
	CFormView::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CEmpStdView)
		// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CEmpStdView, CFormView)
	//{{AFX_MSG_MAP(CEmpStdView)
	ON_WM_CTLCOLOR()
	ON_COMMAND(IDOK, OnOK)
	ON_COMMAND(IDCANCEL, OnCancel)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CEmpStdView message handlers

HBRUSH CEmpStdView::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor) 
{
	HBRUSH hbr = CFormView::OnCtlColor(pDC, pWnd, nCtlColor);

	switch (nCtlColor) {
	case CTLCOLOR_EDIT:
		pDC->SetTextColor(RGB(0,0,255));
		break;
	case CTLCOLOR_STATIC:
		if (pWnd->GetDlgCtrlID() == IDC_CLOCK)
			pDC->SetTextColor(RGB(255,0,0));
	}
	return hbr;
}

void CEmpStdView::OnInitialUpdate() 
{
	CFormView::OnInitialUpdate();

	m_frame = (CMainFrame*)AfxGetApp()->m_pMainWnd;
}

void CEmpStdView::OnCancel()
{
	int		kq=0;
	CWnd*	tuc;
	tuc = GetDlgItem(IDC_LOP);
	if (tuc->GetWindowTextLength() == 0) {
		tuc->SetFocus();
		return;
	}
	tuc = GetDlgItem(IDC_SOBD);
	if (tuc->GetWindowTextLength() == 0) {
		tuc->SetFocus();
		return;
	}
	tuc = GetDlgItem(IDC_HOTEN);
	if (tuc->GetWindowTextLength() == 0) {
		tuc->SetFocus();
		return;
	}

	m_frame->ptest->EnableWindow(TRUE);
	EnableWindow(FALSE);
	m_frame->PostMessage(WM_COMMAND,ID_TEST, 0);

	if (m_frame->TestType == 0) return;

	// kq = number of questions in selected Item.
	if (m_frame->curJob == EMP_TEST1) {
		kq = m_frame->ptest->NewItem(m_frame->zmpName[0].ItemNo, 
									m_frame->zmpName[0].FileName);
		m_frame->curJob = EMP_TEST;
	}
	if (m_frame->TestType == 1) return;

	m_frame->NewInfo();	// send back to supervisor

	if (kq > 0) {
		if (2*kq != m_frame->AnsBufL || 
			!m_frame->ptest->SetAnswer(m_frame->AnsBufL, m_frame->AnsBuf)) {
			if (m_frame->AnsBufL > 0)
				delete m_frame->AnsBuf;
			m_frame->AnsBufL = 2*kq;
			m_frame->AnsBuf  = new char[m_frame->AnsBufL];
			memset(m_frame->AnsBuf, -1, m_frame->AnsBufL);
			m_frame->ptest->GetResponse(m_frame->AnsBufL, m_frame->AnsBuf);
		}
	}
}

void CEmpStdView::OnOK()
{
	return;
}

void CEmpStdView::OnActivateView(BOOL bActivate, CView* pActivateView, CView* pDeactiveView) 
{
	CFormView::OnActivateView(bActivate, pActivateView, pDeactiveView);
//	m_frame->IsSelection(ID_STUDENT, bActivate);

	if (bActivate && m_frame->AnsBufL==0) ClearOldInfo();
}

void CEmpStdView::ClearOldInfo()
{
	SetDlgItemText(IDC_SOBD, "");
	SetDlgItemText(IDC_HOTEN, "");
	SetDlgItemText(IDC_NSINH, "");
	SetDlgItemText(IDC_LOP, "");
}

BOOL CEmpStdView::PreTranslateMessage(MSG* pMsg) 
{
	if ((pMsg->message == WM_KEYDOWN || pMsg->message == WM_KEYUP ) && 
		pMsg->wParam == VK_RETURN) {
		if (GetFocus()->GetDlgCtrlID() == IDCANCEL)
			pMsg->wParam = VK_SPACE;
		else
			pMsg->wParam = VK_TAB;
	}
	
	return CFormView::PreTranslateMessage(pMsg);
}
