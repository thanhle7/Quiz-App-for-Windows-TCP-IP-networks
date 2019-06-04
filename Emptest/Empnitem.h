/*
	This source code is given to people who want to improve their C++ language programming skill.
	It may be used for educational purposes but not for others.
	No guaranties are supplied along this source code.

	Some Libraries and programming techniques in this source code can be found in the book:

		"Lap trinh Windows voi MFC - VC++ 6.0, Le Ngoc Thanh, NXB Thong Ke 2002"

	PS : This source code is uncompatible with later versions of EmpTest from LNT/EMPsoft,
	     and uQuiz from usewide dev.
*/
#if !defined(AFX_EMPNITEM_H__C35C8274_4328_11D4_BB80_0008C7331CD4__INCLUDED_)
#define AFX_EMPNITEM_H__C35C8274_4328_11D4_BB80_0008C7331CD4__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// EmpNItem.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CEmpNItemDlg dialog

class CMainFrame;

// style of new item dialog
const	NIS_NET		= 1,
		NIS_STUDENT	= 2,
		NIS_OK		= 4,
		NIS_CANCEL	= 8,
		NIS_IDCHECK	= 16;

class CEmpNItemDlg : public CDialog
{
// Construction
public:
	CEmpNItemDlg(CMainFrame* pParent, char* title, char* mess, DWORD dwStyle);

// Dialog Data
	//{{AFX_DATA(CEmpNItemDlg)
	enum { IDD = IDD_NEWITEM };
		// NOTE: the ClassWizard will add data members here
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CEmpNItemDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	CMainFrame	*m_frame;
	int			m_maxEx;
	char		*m_title, *m_mess;
	CString		m_oldStdID;
	DWORD		m_style;

	// Generated message map functions
	//{{AFX_MSG(CEmpNItemDlg)
	afx_msg void OnChangeItemExno();
	virtual BOOL OnInitDialog();
	afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
	virtual void OnCancel();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
public:
	int Hienthi(void);
	afx_msg void OnBnClickedItemOk();
	// Modify the new item dialog style
	void ModifyInterface(DWORD dwStyle, char* lpMessage);
	virtual BOOL PreTranslateMessage(MSG* pMsg);
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_EMPNITEM_H__C35C8274_4328_11D4_BB80_0008C7331CD4__INCLUDED_)
