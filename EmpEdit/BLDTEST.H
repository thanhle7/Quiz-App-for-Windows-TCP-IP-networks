#if !defined(AFX_BLDTEST_H__F576F964_C911_491D_9BFD_F1558B5139C9__INCLUDED_)
#define AFX_BLDTEST_H__F576F964_C911_491D_9BFD_F1558B5139C9__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// BldTest.h : header file

/////////////////////////////////////////////////////////////////////////////
// CBldTest dialog

#include "filelist.h"

class CBldTest : public CDialog
{
// Construction
public:
	~CBldTest(void);
	CBldTest(CEmpView* pParent);

// Dialog Data
	//{{AFX_DATA(CBldTest)
	enum { IDD = IDD_BUILDTEST };
	CSelectList	m_filelist;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CBldTest)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	CProgressCtrl*	prog;
	pFileListItem	fa[100];
	int				flNo, howWork;
	WORD			totalselect;	// number of selected questions
	CEmpView*		myView;

	BOOL isSub(char* s);
	void AddSubLine(pSubAns& ps, char* line);
	BOOL NotIn(int val, int bfl, int *bf);
	int	 CorrectPos(int *ssize, int *snpos, int len, int cpos, int sspos);
	// Generated message map functions
	//{{AFX_MSG(CBldTest)
	afx_msg void OnBuildsave();
	virtual BOOL OnInitDialog();
	virtual void OnOK();
	afx_msg void OnBuildsele();
	afx_msg void OnSetHowWork();
	afx_msg void OnKillfocusBuilditem();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_BLDTEST_H__F576F964_C911_491D_9BFD_F1558B5139C9__INCLUDED_)
