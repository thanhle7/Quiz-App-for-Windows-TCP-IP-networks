// EmpView.h : interface of the CEmpView class
//
/////////////////////////////////////////////////////////////////////////////

#if !defined(AFX_EMPVIEW_H__2A94EB31_E0B5_41C3_8C2F_0E5CF156FDB9__INCLUDED_)
#define AFX_EMPVIEW_H__2A94EB31_E0B5_41C3_8C2F_0E5CF156FDB9__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#define WORD_S	sizeof(WORD)
#define DWORD_S	sizeof(DWORD)
#define int_S	sizeof(int)
#define char_S	sizeof(char)

//----- LNT.test format file -----

/*
	Richedit 1.0:		Line Feed by '\r\n'
	Richedit 2.0 & 3.0:	Line Feed only by '\r'

	CRichEditCtrl
		GetLine();	// Add '\r' to the end of line
	CArchive
		WriteString	// Write the string info only
		ReadString	// Need the pair "\r\n" at the end of each line
*/

const	EMPDOC_VERSION	= 23;	// current version
const	MENU_TITLE_LEN	= 40;
const	NOANSWER		= 9;	// Maximun Number of answers in each question

	/* Test Topic Data File Structure
	char[EmpIDL]		: ID of LNT.Product
	char[MENU_TITLE_LEN]: Menu Title
	WORD	:	reserved for later use (Lo(this)= Hi(this) is normal)
	WORD	:	Number of Exercises (Items)
	DWORD[]	:	Starting Location of Each Item.
	*/

typedef struct SSUBANS {
	char*  info;
	struct SSUBANS* next;
} SubAns, *pSubAns;

//--- end of LNT.test format file

/*---- Data format File for EmpED
	char[EmpIDL]: Id of LNT.Product
	char: He so cau hoi	--> used for password			; not used from ver 2.3
	WORD: So cau hoi
	WORD: Emp info identifier
		LO(BYTE): Ky hieu ghi nhan phan noi dung chung cua muc
			0	: Khong co noi dung chung;
			#0	: Co noi dung chung ( Main-Info ); cau truc noi dung
				  chung nay giong nhu 1 cau hoi (phan duoi)
		HI(BYTE): Document Version
			Range from 0.0 - 25.5						; major.minor version
			= 0.0 means no version (ancient version)	; under ver 2.1
	WORD :	So nhom cau hoi, khong tinh Main-Info		; from ver 2.2 
	WORD[]:	So cau hoi trong nhom tuong ung				; from ver 2.2
	char[]:	Level cua nhom tuong ung					; from ver 2.3
	DWORD[] Vi tri bat dau cua nhom tuong ung			; from ver 2.2

		....
*/
#define GETLEVEL(x) ((x)=='*')?1:(((x)<='9' && (x)>'0' )?((x)-'0'):(((x)>='A')?((x)-55):1))
#define GETLEVELMARK(x) (x<10)?(x+'0'):('A'+(x-10))
#define ISSUB(chr) (chr == '#' || chr == '$' || chr == '!')
const	TOPLEVEL=	16;		// the maximun level
const	CHAR_CY =	200;
const	PARALEN	=	8192;	// MaxLength of a paragraph
const	NoCF	=	8;		// B, I, U, S, Symbol-Font, Color, subScript, superScript
const	QNOLEN	=	4;		// MaxLength of question number : "***/"
const	EmpIDL	=	8;		// Length of LNT. Identifier
const	char EmpID[EmpIDL]= {'E','M','P','.','L','N','T','\x1A'};
const	int	CFV[NoCF] = {CFM_BOLD, CFM_ITALIC, CFM_UNDERLINE,
							CFM_STRIKEOUT, CFM_FACE, CFM_COLOR,
							CFE_SUBSCRIPT, CFE_SUPERSCRIPT};

// Objects process will not be supplied for security purpose, 
// LNTsoft!

typedef struct FORMAT {
	long	p1, p2;
	long	val;
	struct	FORMAT*	next;
} CF, *pCF;

typedef struct QUESTION {
	char	groupID;	// '\0' used for main-info
	char	level;		// level of question
	int		lp1, lp2;	// line 
	long	cp1, cp2;	// & char position
	pCF		cf[NoCF], lcf[NoCF];
	struct  QUESTION* next;
} Question, *pQuestion;

typedef struct QSTINFO {
	int numOFans, ans;
	int startline, startsubline[15];	// 15 sub-ans
} qstinfo;

class CEmpView : public CRichEditView
{
	friend class CBldTest;
	friend class CConvert;
	friend class CEmpDoc;
	friend class CLevelDlg;
protected:
	// create from serialization only
	CEmpView();
	DECLARE_DYNCREATE(CEmpView)

// Attributes
public:
	BOOL	 ValidEmpDoc(char* FileName);
	CEmpDoc* GetDocument();
	void	 GetFileDlgCtrl(CWnd *pr, int Id, BOOL toZmp, BOOL isRead, int mode);
	// Update the charformat button interface 
	BOOL	HasCharFormat(DWORD cfEffect);
	afx_msg void OnRButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnKeyUp(UINT nChar, UINT nRepCnt, UINT nFlags);
	afx_msg void OnPaint();

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CEmpView)
	protected:
	virtual void OnInitialUpdate(); // called first time after construct
	virtual BOOL OnPreparePrinting(CPrintInfo* pInfo);
	virtual void OnPrint(CDC* pDC, CPrintInfo* pInfo);
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~CEmpView();

protected:

// Generated message map functions
protected:
	void PrepareCharFormat(CHARFORMAT2& cf, DWORD dwMask, DWORD dwEffects);
	void SetStdFormat(BOOL empFont);
	void SetFont(void);
	BOOL ClearCurrentDoc(BOOL prompt);
	void DoSave(CArchive &ar);
	void DoPrintTest(CArchive &ar);
	void DoLoad(CArchive& ar);
	void SaveQuestion(CArchive& ar, CRichEditCtrl& re, pQuestion pq, pCF *pf);
	void FreeQuestions(pQuestion &p);
	void FreeQuestion(pQuestion &p);
	void FreeCFs(pCF &p);
	void AddCF(pCF &sp, pCF &ep, long p1, long p2, long val);
	void SetColor(void);
	void SetStrike(void);
	void SetUnderline(void);
	void SetItalic(void);
	void SetBold(void);
	void ApplyCharFormat(DWORD dwMask , DWORD dwEffects);
	afx_msg void SetSubscript();
	afx_msg void SetSuperscript();
	WORD AdjustPos(WORD pos, WORD* list);

	pQuestion		NewQuestion(char grpID, char level, long lp1, long lp2, long cp1, long cp2);
	CHARFORMAT2		cf;
	BOOL			isPrinting, PrintPrepare;
	bool			isNoBackgrndProcess; // turn on|off all accesses to view
	CPen			highPen, lowPen, *oldPen;
	CBrush			highBrush, *oldBrush;
	unsigned char	level;

	//{{AFX_MSG(CEmpView)
	afx_msg void OnDestroy();
	afx_msg void OnFilePrintPreview();
	afx_msg void OnBuildtest();
	afx_msg void OnConvert();
	afx_msg void OnEditInsertsymbol();
	afx_msg void OnFilePrint();
	afx_msg void OnFileSetLevel();
	afx_msg void OnUpdateFilePrint(CCmdUI* pCmdUI);
	afx_msg void OnUpdateFilePrintPreview(CCmdUI* pCmdUI);
	afx_msg void OnFilePrintLoad();
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

#endif // !defined(AFX_EMPVIEW_H__2A94EB31_E0B5_41C3_8C2F_0E5CF156FDB9__INCLUDED_)
