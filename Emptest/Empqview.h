#if !defined(AFX_EMPQVIEW_H__76CEB8D8_4263_11D4_BB7E_0008C7331CD4__INCLUDED_)
#define AFX_EMPQVIEW_H__76CEB8D8_4263_11D4_BB7E_0008C7331CD4__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// EmpQView.h : header file
//

#include <afxcview.h>

/////////////////////////////////////////////////////////////////////////////
// CEmpQstsView view

class CEmpView;

class CEmpQstsView : public CListView
{
friend class CEmpView;
friend class CMainFrame;
protected:
	CEmpQstsView();           // protected constructor used by dynamic creation
	DECLARE_DYNCREATE(CEmpQstsView)

// Attributes
public:

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CEmpQstsView)
	public:
	virtual void OnInitialUpdate();
	protected:
	virtual void OnDraw(CDC* pDC);      // overridden to draw this view
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
	virtual void OnActivateView(BOOL bActivate, CView* pActivateView, CView* pDeactiveView);
	//}}AFX_VIRTUAL

// Implementation
protected:
	virtual ~CEmpQstsView();

	// Generated message map functions
protected:
	void		ItemChanged(void);
	void		SeleItem(int iNo, int Mark);
	void		ViewAnswer(int alen, int mark);
	CMainFrame*	m_frame;
	CImageList*	m_smallbmp;

	//{{AFX_MSG(CEmpQstsView)
	afx_msg void OnClick(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnRclick(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags);
	afx_msg BOOL OnSetCursor(CWnd* pWnd, UINT nHitTest, UINT message);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_EMPQVIEW_H__76CEB8D8_4263_11D4_BB7E_0008C7331CD4__INCLUDED_)
