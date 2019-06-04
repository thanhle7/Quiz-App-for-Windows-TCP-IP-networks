// Preview.h : header file
//

#include "afxpriv.h"
#include "MBbutton.h"

/////////////////////////////////////////////////////////////////////////////
// CMyPreviewView view

class CMyPreviewView : public CPreviewView
{
protected:
	CMyPreviewView();           // protected constructor used by dynamic creation
	DECLARE_DYNCREATE(CMyPreviewView)

// Attributes
public:

protected:
	CMappedBitmapButton m_print;
	CMappedBitmapButton m_next;
	CMappedBitmapButton m_previous;
	CMappedBitmapButton m_onetwo;
	CMappedBitmapButton m_zoomIn;
	CMappedBitmapButton m_zoomOut;

	BOOL m_bOne;

// Operations
public:

// Overrides
	virtual void OnDisplayPageNumber(UINT nPage, UINT nPagesDisplayed);

	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CMyPreviewView)
	//}}AFX_VIRTUAL

// Implementation
protected:
	virtual ~CMyPreviewView();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

	// Generated message map functions
protected:
	//{{AFX_MSG(CMyPreviewView)
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	//}}AFX_MSG
	afx_msg void OnPreviewPrint();
	afx_msg void OnUpdateNumPageChange(CCmdUI* pCmdUI);
	afx_msg void OnUpdateZoomIn(CCmdUI* pCmdUI);
	afx_msg void OnUpdateZoomOut(CCmdUI* pCmdUI);
	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////