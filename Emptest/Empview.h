// EmpView.h : interface of the CEmpView class
//
/////////////////////////////////////////////////////////////////////////////

#if !defined(AFX_EMPVIEW_H__676FA39D_3527_11D4_BB67_0008C7331CD4__INCLUDED_)
#define AFX_EMPVIEW_H__676FA39D_3527_11D4_BB67_0008C7331CD4__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000


// from EmpED
const MAXANS			= 10;		// Based on number of bitmap in EmpED
const CHAR_CY			= 200;
const PARALEN			= 8192;
const QNOLEN			= 4;		// MaxLength of question number
const char SAQ[QNOLEN]  = {'*','*','*','/'};
// now suport symbol font format
const NoCF				= 8;
const int CFV[NoCF]		= {CFM_BOLD, CFM_ITALIC, CFM_UNDERLINE,
							CFM_STRIKEOUT, CFM_FACE, CFM_COLOR,
							CFE_SUBSCRIPT, CFE_SUPERSCRIPT};

typedef struct QUESTION {
	int	fMain;			// the firts line of the question
	int	lSub;			// the last line (answer line) of the question
	int	cans;			// the current answer of user
	int res;			// the correct answer
	int	ansNo;			// number of answer of the question
	int	ansLine[MAXANS];// line index of the first line in each answer
} Qst, *pQst;

class CEmpView : public CRichEditView
{
friend class CEmpQstsView;
friend class CMainFrame;
friend class CEmpStdView;
protected: // create from serialization only
	CEmpView();
	DECLARE_DYNCREATE(CEmpView)

// Attributes
public:
	CEmpDoc* GetDocument();

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CEmpView)
	public:
	virtual BOOL OnCmdMsg(UINT nID, int nCode, void* pExtra, AFX_CMDHANDLERINFO* pHandlerInfo);
	protected:
	virtual void OnInitialUpdate(); // called first time after construct
	virtual void OnActivateView(BOOL bActivate, CView* pActivateView, CView* pDeactiveView);
	//}}AFX_VIRTUAL
	afx_msg void OnPaint();
	afx_msg void OnVScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar);

// Implementation
public:
	virtual ~CEmpView();

protected:

// Generated message map functions
protected:
	void	PrepareCharFormat(DWORD dwMask, DWORD dwEffects = 0);
	void	OnQuestionTouch(CPoint* point);
	void	OnQuestionMark(CPoint* point);
	BOOL	GetQstAnsFromLine(int line, int& qst, int& ans);
	BOOL	MalCtrlKey(UINT nChar);
	void	GetResponse(int aLen, char* aBuf);
	BOOL	SetAnswer(int aLen, char* aBuf);
	int		ViewResult(BOOL view, float& score);
	void	ApplyFormat(long startPos, long endPos);
	void	GotoQuestion(int qstNo);
	void	SetStdFormat();
	int		DoLoad(CArchive& ar);	// return number of Qsts
	int		NewItem(int item, const char* filename);
	void	GoBottom(void);
	void	GoNext(void);
	void	GoPrev(void);
	void	GoTop(void);
	void	MarkAnswer(int qst, int ans, int mark, CRichEditCtrl& re);
	UINT	empCode;
	pQst	qsts;
	int		qstNo, TLines, cur_qst, oqst, oans;
	BOOL	m_resetCaretPos;
	HICON	m_hIconMark, m_hIconAnswer;
	CMainFrame*	m_frame;
	CHARFORMAT2	cf;

	//{{AFX_MSG(CEmpView)
	afx_msg void OnDestroy();
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnLButtonDblClk(UINT nFlags, CPoint point);
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
	afx_msg BOOL OnSetCursor(CWnd* pWnd, UINT nHitTest, UINT message);
	afx_msg void OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags);
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

#ifndef _DEBUG  // debug version in EmpView.cpp
inline CEmpDoc* CEmpView::GetDocument()
   { return (CEmpDoc*)m_pDocument; }
#endif

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_EMPVIEW_H__676FA39D_3527_11D4_BB67_0008C7331CD4__INCLUDED_)
