#if !defined(AFX_EMPSVIEW_H__01ED9102_41EF_11D4_BB7D_0008C7331CD4__INCLUDED_)
#define AFX_EMPSVIEW_H__01ED9102_41EF_11D4_BB7D_0008C7331CD4__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// EmpSView.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CEmpStdView form view

#ifndef __AFXEXT_H__
#include <afxext.h>
#endif

class CEmpStdView : public CFormView
{
friend class CMainFrame;
protected:
	CEmpStdView();           // protected constructor used by dynamic creation
	DECLARE_DYNCREATE(CEmpStdView)

// Form Data
public:
	//{{AFX_DATA(CEmpStdView)
	enum { IDD = IDD_STUDENINFO };
		// NOTE: the ClassWizard will add data members here
	//}}AFX_DATA

// Attributes
public:

// Operations
public:
	void ClearOldInfo(void);
	void OnOK(void);

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CEmpStdView)
	public:
	virtual void OnInitialUpdate();
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	virtual void OnActivateView(BOOL bActivate, CView* pActivateView, CView* pDeactiveView);
	//}}AFX_VIRTUAL

// Implementation
protected:
	CMainFrame*		m_frame;
	virtual ~CEmpStdView();

	// Generated message map functions
	//{{AFX_MSG(CEmpStdView)
	afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
	afx_msg void OnCancel();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_EMPSVIEW_H__01ED9102_41EF_11D4_BB7D_0008C7331CD4__INCLUDED_)
