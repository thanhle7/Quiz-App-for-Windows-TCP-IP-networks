
// LNT.1999, by Le Ngoc Thanh.

#if !defined(AFX_PROPERTYLIST_H__74205380_1B56_11D4_BC48_00105AA2186F__INCLUDED_)
#define AFX_PROPERTYLIST_H__74205380_1B56_11D4_BC48_00105AA2186F__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// PropertyList.h : header file
//

#include "emped.h"

#define IDC_PROPEDITBOX  713
#define IDC_PROPBTNCTRL  714
#define IDC_PROPSPNCTRL  715	// used with auto buddy edit

/////////////////////////////////////////////////////////////////////////////
//CSelectList Items
typedef struct FILELISTITEM {
	int		items;		// <> 0 is header (number of items belong to this header)
	CString title;		// info (header) or file name (item)
	WORD	socau;		// so cau chon tu cac muc (header) | so cau hoi trong tap tin (item)
	WORD	sonhom;		// so nhom cau trong tap tin (item uses only)
	WORD	somuc;		// so muc do khac nhau trong nhom (header) | tap tin (item)
	WORD	caumuc[TOPLEVEL];		// caumuc[i] la tong so cau hoi muc i; i=1,2,3,4,....
	WORD	nhommuc[TOPLEVEL];		// nhommuc[i] la so nhom cau hoi muc i; i=1,2,3,4,... 
	WORD	socauchon;				// so cau thuc su chon (header uses only)
	WORD	caumucchon[TOPLEVEL];	// caumucchon[i] la so cau chon trong muc i
} FileListItem, *pFileListItem;

/////////////////////////////////////////////////////////////////////////////
// CSelectList window

class CSelectList : public CListBox
{
// Construction
public:
	CSelectList();

// Operations
public:
	void DelPropItem(int idx);
	int  AddPropItem(int ItemType, int Mucdo=0, int Sonhom=0, int Socau=0, int Socauchon=0);
	void UpdateHeaderFromItems(int idx, int except);
	bool UpdateItemFromFile(int idx, CString& filename);

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CSelectList)
	public:
	virtual void MeasureItem(LPMEASUREITEMSTRUCT lpMeasureItemStruct);
	virtual void DrawItem(LPDRAWITEMSTRUCT lpDrawItemStruct);
	protected:
	virtual void PreSubclassWindow();
	//}}AFX_VIRTUAL

// Implementation
public:
	void	Setparameters(CEmpView* pview, int* ItemCount, pFileListItem *PItem, WORD* Sel, int ID);
	virtual	~CSelectList();

	// Generated message map functions
protected:
	void EmptyItem(pFileListItem pItem, bool SelClear = true);
	void DisplayButton(CRect region);
	int  GetCurLine(CPoint point);
	//{{AFX_MSG(CSelectList)
	afx_msg void OnSelchange();
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnKillFocus(CWnd* pNewWnd);
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnVScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar);
	afx_msg BOOL OnSetCursor(CWnd* pWnd, UINT nHitTest, UINT message);
	afx_msg void OnLButtonDblClk(UINT nFlags, CPoint point);
	//}}AFX_MSG
	afx_msg void OnKillfocusEditBox();
	afx_msg void OnChangeEditBox();
	afx_msg void OnButton();

	CEdit		m_editBox;
	CButton		m_btnCtrl;
	CFont*		m_pvnFont;
	CEmpView*	m_pView;
	int			*itemcount, id;
	WORD		*sel;
	int			m_curSel;
	int			m_nDivider;
	BOOL		m_bTracking;
	HCURSOR		m_hCursorMove;
	pFileListItem	*pitem;	// array of pointer
	CSpinButtonCtrl m_spin;

	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_PROPERTYLIST_H__74205380_1B56_11D4_BC48_00105AA2186F__INCLUDED_)
