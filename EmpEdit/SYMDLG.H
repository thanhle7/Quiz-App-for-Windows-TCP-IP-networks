//////////////////////////////////////////////////////////////////////
// Use to insert symbols into a RichEditView
// LNT.2001, by Le Ngoc Thanh
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_SYMDLG_H__EFA0747C_F4F2_40EB_964B_2C52CDBF21A1__INCLUDED_)
#define AFX_SYMDLG_H__EFA0747C_F4F2_40EB_964B_2C52CDBF21A1__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// SymDlg.h : header file
//

const dx = 18;
const dy = 22;

/////////////////////////////////////////////////////////////////////////////
// CSymbolDlg dialog

class CSymbolDlg : public CDialog
{
// Construction
public:
	CSymbolDlg(CRichEditView* pParent, CHARFORMAT2 *cf);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CSymbolDlg)
	enum { IDD = IDD_SYMBOLDLG };
		// NOTE: the ClassWizard will add data members here
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CSymbolDlg)
	public:
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	protected:
	//}}AFX_VIRTUAL

// Implementation
protected:
	BOOL	GetRowCol(CPoint point);
	int		crow, ccol;
	RECT	rt;
	CBitmap	*bmp, *oldbmp;
	CDC		*memDC;
	CBrush	br;
	CFont	*font, *bigfont, *oldfont;
	LOGFONT	lf;
	BOOL	InsertDone;
	CRichEditView *parent;
	CHARFORMAT2 *pcf;

	// Generated message map functions
	//{{AFX_MSG(CSymbolDlg)
	virtual BOOL OnInitDialog();
	virtual void OnOK();
	afx_msg void OnDestroy();
	afx_msg void OnPaint();
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnLButtonDblClk(UINT nFlags, CPoint point);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_SYMDLG_H__EFA0747C_F4F2_40EB_964B_2C52CDBF21A1__INCLUDED_)
