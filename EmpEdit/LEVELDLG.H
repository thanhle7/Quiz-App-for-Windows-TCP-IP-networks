#if !defined(AFX_LEVELDLG_H__EBA837F3_7CA9_11D5_AB05_0008C7331CD4__INCLUDED_)
#define AFX_LEVELDLG_H__EBA837F3_7CA9_11D5_AB05_0008C7331CD4__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// LevelDlg.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CLevelDlg dialog

class CLevelDlg : public CDialog
{
// Construction
public:
	CLevelDlg(CEmpView* pParent, char* Pmark);

// Dialog Data
	//{{AFX_DATA(CLevelDlg)
	enum { IDD = IDD_SETLEVEL };
		// NOTE: the ClassWizard will add data members here
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CLevelDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	CEmpView* pview;
	char*	  pMark;

	// Generated message map functions
	//{{AFX_MSG(CLevelDlg)
	virtual BOOL OnInitDialog();
	virtual void OnOK();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_LEVELDLG_H__EBA837F3_7CA9_11D5_AB05_0008C7331CD4__INCLUDED_)
