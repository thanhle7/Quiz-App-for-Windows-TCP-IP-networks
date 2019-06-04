// LevelDlg.cpp : implementation file
//

#include "stdafx.h"
#include "emped.h"
#include "LevelDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CLevelDlg dialog


CLevelDlg::CLevelDlg(CEmpView* pParent, char* Pmark)
	: CDialog(CLevelDlg::IDD, pParent)
{
	pview = pParent;
	pMark = Pmark;
	//{{AFX_DATA_INIT(CLevelDlg)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
}

void CLevelDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CLevelDlg)
		// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CLevelDlg, CDialog)
	//{{AFX_MSG_MAP(CLevelDlg)
	//}}AFX_MSG_MAP
	ON_BN_CLICKED(IDOK, OnOK)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CLevelDlg message handlers

BOOL CLevelDlg::OnInitDialog() 
{
	CDialog::OnInitDialog();

	((CSpinButtonCtrl*)GetDlgItem(IDC_LEVELSPIN))->SetRange(1,TOPLEVEL-1);

	SetDlgItemInt(IDC_LEVEL, GETLEVEL(pMark[2]));
	pMark[2] = '\0';
	SetDlgItemText(IDC_LEVEL_MARK, pMark+1);
	SetDlgItemText(IDC_DOCTITLE, pview->GetDocument()->GetTitle());

	return TRUE;
}

void CLevelDlg::OnOK()
{
	int k = GetDlgItemInt(IDC_LEVEL);
	GetDlgItemText(IDC_LEVEL_MARK, pMark+1, 2);
	pMark[2] = GETLEVELMARK(k);
	CDialog::OnOK();
}
