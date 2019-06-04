#if !defined(AFX_EMPLVIEW_H__E5828861_CBA9_4F45_926D_EBA51C3C97C0__INCLUDED_)
#define AFX_EMPLVIEW_H__E5828861_CBA9_4F45_926D_EBA51C3C97C0__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// EmplView.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CEmplView view

class CEmplView : public CView
{
protected:
	CEmplView();           // protected constructor used by dynamic creation
	DECLARE_DYNCREATE(CEmplView)

// Attributes
public:

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CEmplView)
	protected:
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
	//}}AFX_VIRTUAL

// Implementation
protected:
	virtual ~CEmplView();
	virtual void OnDraw(CDC* /*pDC*/);

	// Generated message map functions
protected:
	//{{AFX_MSG(CEmplView)
	afx_msg void OnPaint();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_EMPLVIEW_H__E5828861_CBA9_4F45_926D_EBA51C3C97C0__INCLUDED_)
