#if !defined(AFX_EMPFSPLT_H__01ED910A_41EF_11D4_BB7D_0008C7331CD4__INCLUDED_)
#define AFX_EMPFSPLT_H__01ED910A_41EF_11D4_BB7D_0008C7331CD4__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// EmpFSplt.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CEmpFixSplter window

class CEmpFixSplter : public CSplitterWnd
{
// Construction
public:
	CEmpFixSplter();

// Attributes
public:

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CEmpFixSplter)
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~CEmpFixSplter();

	// Generated message map functions
protected:
	//{{AFX_MSG(CEmpFixSplter)
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_EMPFSPLT_H__01ED910A_41EF_11D4_BB7D_0008C7331CD4__INCLUDED_)
