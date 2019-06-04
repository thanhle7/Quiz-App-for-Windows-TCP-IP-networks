/*
	This source code is given to people who want to improve their C++ language programming skill.
	It may be used for educational purposes but not for others.
	No guaranties are supplied along this source code.

	Some Libraries and programming techniques in this source code can be found in the book:

		"Lap trinh Windows voi MFC - VC++ 6.0, Le Ngoc Thanh, NXB Thong Ke 2002"

	PS : This source code is uncompatible with later versions of EmpTest from LNT/EMPsoft,
	     and uQuiz from usewide dev.
*/
// MainFrm.h : interface of the CMainFrame class
//
/////////////////////////////////////////////////////////////////////////////

#if !defined(AFX_MAINFRM_H__2E8D62A8_C237_42CA_89EA_F05DC443B295__INCLUDED_)
#define AFX_MAINFRM_H__2E8D62A8_C237_42CA_89EA_F05DC443B295__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "..\emptest\EmpFSplt.h"
#include "..\emptest\ObjView.h"
#include "..\..\lnt-lib.cpp\control\tiptext.h"
#include "..\..\lnt-lib.cpp\control\ColorBtn.h"

class CEmpView;

class CMainFrame : public CFrameWnd
{
protected: // create from serialization only
	CMainFrame();
	DECLARE_DYNCREATE(CMainFrame)

// Attributes
public:
	CFont		m_vnFont;
	CComboBox	m_cmbFontName;
	BOOL		m_cmbFontNameFree;
	CLntColorChooseBtn m_btnColor;
// Operations
public:
	afx_msg void DoObjectView(void);
	afx_msg void OnUpdateFormatUnderline(CCmdUI *pCmdUI);
	afx_msg void OnUpdateFormatBold(CCmdUI *pCmdUI);
	afx_msg void OnUpdateFormatItalic(CCmdUI *pCmdUI);
	afx_msg void OnUpdateFormatStrikeout(CCmdUI *pCmdUI);
	afx_msg void OnUpdateFormatSubscript(CCmdUI *pCmdUI);
	afx_msg void OnUpdateFormatSuperscript(CCmdUI *pCmdUI);
	afx_msg void OnUpdateFormatFont(CCmdUI *pCmdUI);
	afx_msg void LockComboFont(void);
	afx_msg void OnHelp();
	afx_msg void OnActivateApp(BOOL bActive, DWORD dwThreadID);
	afx_msg void OnActivate(UINT nState, CWnd* pWndOther, BOOL bMinimized);

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CMainFrame)
	public:
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~CMainFrame();

protected:  // control bar embedded members
	CStatusBar	m_wndStatusBar;
	CToolBar	m_wndToolBar;
	CEmpView*	m_view;
	CTipTextBox m_tip;
	CObjectView	**m_pWndObjView;

// Generated message map functions
protected:
	CEmpFixSplter	m_wndSplitter;
	virtual BOOL	OnCreateClient(LPCREATESTRUCT lpcs, CCreateContext* pContext);
	// Update char format button face
	void	UpdateBtnFace(UINT cmdID, DWORD cfEffect);
	void	HideHelp();
	//{{AFX_MSG(CMainFrame)
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnUpdateEditOleShow(CCmdUI *pCmdUI);
	afx_msg void OnDestroy();
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_MAINFRM_H__2E8D62A8_C237_42CA_89EA_F05DC443B295__INCLUDED_)
