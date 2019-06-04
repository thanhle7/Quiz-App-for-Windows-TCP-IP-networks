// EmpNItem.cpp : implementation file
//

#include "stdafx.h"
#include "emp.h"
#include "EmpNItem.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CEmpNItemDlg dialog

CEmpNItemDlg::CEmpNItemDlg( CMainFrame* pParent, char* title, 
									char* mess, DWORD dwStyle )
	: CDialog(CEmpNItemDlg::IDD, pParent)
{
	m_frame = pParent;
	m_title	= title;
	m_mess	= mess;
	m_style = dwStyle;
	m_maxEx = 0;
	m_oldStdID.Empty();

	//{{AFX_DATA_INIT(CEmpNItemDlg)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
}

void CEmpNItemDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CEmpNItemDlg)
		// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CEmpNItemDlg, CDialog)
	//{{AFX_MSG_MAP(CEmpNItemDlg)
	ON_EN_CHANGE(IDC_ITEM_DE, OnChangeItemExno)
	ON_WM_CTLCOLOR()
	//}}AFX_MSG_MAP
	ON_BN_CLICKED(IDC_ITEM_OK, OnBnClickedItemOk)
	ON_WM_DESTROY()
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CEmpNItemDlg message handlers

void CEmpNItemDlg::OnChangeItemExno()
{
	if (m_maxEx > 0) {
		int i = GetDlgItemInt(IDC_ITEM_DE);
		if (i > m_maxEx || i < 1)
			SetDlgItemInt(IDC_ITEM_DE, m_maxEx);
	}
}

BOOL CEmpNItemDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	SetWindowText(m_title);
	ModifyInterface(m_style, m_mess);

	// set focus on my own
	SetCapture();
	SendMessage(WM_SETCURSOR,(LPARAM)(m_hWnd),MAKELPARAM(HTCLIENT,0));
	ReleaseCapture();

	return TRUE;
}

HBRUSH CEmpNItemDlg::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor) 
{
	HBRUSH hbr = CDialog::OnCtlColor(pDC, pWnd, nCtlColor);
	if ((nCtlColor == CTLCOLOR_STATIC &&
		 pWnd->GetDlgCtrlID() == IDC_ITEM_MESS) || 
		nCtlColor == CTLCOLOR_EDIT ) {
		pDC->SetTextColor(RGB(0,0,200));
	}
	return hbr;
}

void CEmpNItemDlg::OnCancel() 
{
	if (m_style & NIS_CANCEL) CDialog::OnCancel();
}

void CEmpNItemDlg::OnBnClickedItemOk()
{
	GetDlgItemText(IDC_ITEM_SOBD, m_frame->m_sobd);
	GetDlgItemText(IDC_ITEM_LOP, m_frame->m_lop);
	GetDlgItemText(IDC_ITEM_HOTEN, m_frame->m_hoten);
	GetDlgItemText(IDC_ITEM_NGAYSINH, m_frame->m_ngaysinh);
	if (m_style & NIS_STUDENT) {
		if (m_frame->m_sobd.IsEmpty()) {
			SetDlgItemText(IDC_ITEM_MESS, "\n\nBân chŸa nhçp s– b o danh !");
			GotoDlgCtrl(GetDlgItem(IDC_ITEM_SOBD));
			return;
		}
		if (m_style & NIS_IDCHECK || !m_oldStdID.IsEmpty()) {
			// The student ID is on checking phase
			if (m_frame->m_lop.IsEmpty() || m_oldStdID != m_frame->m_sobd) {
				// student information is not enough
				m_oldStdID = m_frame->m_sobd;
				// disable OK button
				GetDlgItem(IDC_ITEM_OK)->ShowWindow(SW_HIDE);
				SetDlgItemText(IDC_ITEM_MESS, "\n\n®ang kiŽm tra s– b o danh...");
				// request to HOT to check student ID
				// ...
				return;
			}
		}
		else {
			if (m_frame->m_lop.IsEmpty()) {
				SetDlgItemText(IDC_ITEM_MESS, "\n\nBân chŸa nhçp lôp !");
				GotoDlgCtrl(GetDlgItem(IDC_ITEM_LOP));
				return;
			}
			if (m_frame->m_hoten.IsEmpty()) {
				SetDlgItemText(IDC_ITEM_MESS, "\n\nBân chŸa nhçp H” v… Tˆn !");
				GotoDlgCtrl(GetDlgItem(IDC_ITEM_HOTEN));
				return;
			}
		}
	}
	m_frame->m_dethi = GetDlgItemInt(IDC_ITEM_DE) - 1;
	EndDialog(IDOK);
}

// Modify the new item dialog style
void CEmpNItemDlg::ModifyInterface(DWORD dwStyle, char* lpMessage)
{
	m_style = dwStyle;
	if (lpMessage != NULL)
		SetDlgItemText(IDC_ITEM_MESS, lpMessage);

	if (dwStyle < 1) return;

	SetDlgItemText(IDC_ITEM_LOP, m_frame->m_lop);
	SetDlgItemText(IDC_ITEM_SOBD, m_frame->m_sobd);
	SetDlgItemText(IDC_ITEM_HOTEN, m_frame->m_hoten);
	SetDlgItemText(IDC_ITEM_NGAYSINH, m_frame->m_ngaysinh);

	if (m_style & NIS_NET){
		// Net icon here
		return;
	}
	//GetDlgItem(IDC_ITEM_NET)->ShowWindow(SW_HIDE);

	if (m_style & NIS_CANCEL)
		GetDlgItem(IDCANCEL)->ShowWindow(SW_SHOW);
	if (m_style & NIS_OK) {
		CWnd* wndOK = GetDlgItem(IDC_ITEM_OK);
		wndOK->ShowWindow(SW_SHOW);

		m_maxEx = m_frame->zmpName[m_frame->curTopic].ItemNo;
		((CSpinButtonCtrl*)GetDlgItem(IDC_ITEM_DESPN))->SetRange(1,m_maxEx);
		SetDlgItemInt(IDC_ITEM_DE, m_frame->m_dethi+1);

		if (m_style & NIS_STUDENT) {
			UINT down = IDC_ITEM_SOBDL;			// [down, top] control IDs to enable
			UINT top  = IDC_ITEM_NGAYSINH;
			if (m_style & NIS_IDCHECK) {
				if (m_style & NIS_CANCEL) {
					m_style &= ~NIS_IDCHECK;
					down = IDC_ITEM_LOPL;
				}
				else {
					if (!m_frame->m_sobd.IsEmpty() &&
						!m_frame->m_lop.IsEmpty() && !m_frame->m_hoten.IsEmpty()) {
						EndDialog(IDOK);
						return;
					}
					top = IDC_ITEM_SOBD;
				}
			}
			while (down <= IDC_ITEM_NGAYSINH) {
				GetDlgItem(down)->EnableWindow(down <= top);
				down++;
			}
			GetDlgItem(IDC_ITEM_DE)->EnableWindow(FALSE);
		}
		if ( m_style & NIS_CANCEL ) {
			GetDlgItem(IDC_ITEM_DE)->ShowWindow(SW_SHOW);
			GetDlgItem(IDC_ITEM_DEL)->ShowWindow(SW_SHOW);
			GetDlgItem(IDC_ITEM_DESPN)->ShowWindow(SW_SHOW);

			CRect rect;
			int distance;
			wndOK->GetWindowRect(rect);
			distance = 10 + rect.Width();
			rect.left  -= distance;
			rect.right -= distance;
			//rect.MoveToX(rect.left - 10 - rect.Width());
			ScreenToClient(rect);
			wndOK->MoveWindow(rect, TRUE);
		}
		GetDlgItem(IDC_ITEM_SOBD)->SetFocus();
	}
}

BOOL CEmpNItemDlg::PreTranslateMessage(MSG* pMsg)
{
	if ((pMsg->message == WM_KEYDOWN || pMsg->message == WM_KEYUP )) {
		if (pMsg->wParam == VK_F9)
			return m_frame->PreTranslateMessage(pMsg);
		else
			if (pMsg->wParam == VK_RETURN) {
				UINT ctrlID = GetFocus()->GetDlgCtrlID();
				if ( ctrlID == IDCANCEL || ctrlID == IDC_ITEM_OK )
					pMsg->wParam = VK_SPACE;
				else
					pMsg->wParam = VK_TAB;
			}
	}

	return CDialog::PreTranslateMessage(pMsg);
}

int CEmpNItemDlg::Hienthi()
{
	CMainFrame* frame = m_frame;

	int kq = DoModal();

	delete (frame->ocDlg);
	frame->ocDlg = NULL;

	return  kq;
}
