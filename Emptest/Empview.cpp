// EmpView.cpp : implementation of the CEmpView class
//

#include "stdafx.h"
#include "Emp.h"
#include "empview.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CEmpView

IMPLEMENT_DYNCREATE(CEmpView, CRichEditView)

BEGIN_MESSAGE_MAP(CEmpView, CRichEditView)
	//{{AFX_MSG_MAP(CEmpView)
	ON_WM_DESTROY()
	ON_WM_LBUTTONDOWN()
	ON_WM_LBUTTONDBLCLK()
	ON_WM_LBUTTONUP()
	ON_WM_SETCURSOR()
	ON_WM_KEYDOWN()
	ON_COMMAND(ID_TOP, GoTop)
	ON_COMMAND(ID_PREV, GoPrev)
	ON_COMMAND(ID_NEXT, GoNext)
	ON_COMMAND(ID_BOTTOM, GoBottom)
	ON_WM_MOUSEMOVE()
	//}}AFX_MSG_MAP
	ON_WM_PAINT()
	ON_WM_VSCROLL()
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CEmpView construction/destruction

CEmpView::CEmpView()
{
	CWinApp* pApp = AfxGetApp();

	m_hIconMark	  = (HICON)LoadImage(pApp->m_hInstance, MAKEINTRESOURCE(IDI_QST_MARK), IMAGE_ICON, 16, 16, LR_DEFAULTCOLOR);
	m_hIconAnswer = (HICON)LoadImage(pApp->m_hInstance, MAKEINTRESOURCE(IDI_QST_ANSWER), IMAGE_ICON, 16, 16, LR_DEFAULTCOLOR);

	qsts = NULL; qstNo = 0; TLines = 0; oqst = 0, oans = -1;
}

CEmpView::~CEmpView()
{
	if (qsts != NULL) delete qsts;
}

void CEmpView::OnInitialUpdate()
{
	CRichEditView::OnInitialUpdate();

	m_resetCaretPos = FALSE;
	CRichEditCtrl& re = GetRichEditCtrl();
	m_frame = (CMainFrame*)AfxGetApp()->m_pMainWnd;

	if (GetWindowTextLength() == 0) {
		re.SetBackgroundColor(FALSE, RGB(0xFF, 0xFF, 0xF0));
		SetStdFormat();
		if (m_frame->zmpNo < 1 && !m_frame->TestType) {
			re.SetWindowText("\n\t* Kh“ng tm thãy c c tçp tin d§ li¬u cœa Emp.Test (*.zmp),"
						"\n\t   Bân nˆn ch‚p b™ sung c c tçp tin n…y v…o thŸ mžc cœa Test.exe."
						"\n\t* Bàn quy‹n: THQL, ®HKT Tp.HCM."
						"\n\t\t\t\t\t\tLNT.2003");
			PrepareCharFormat(CFV[2]|CFV[5], CFV[2]|CFV[5]);
			cf.crTextColor = RGB(200,200,0);
			ApplyFormat(167, 175);
			cf.crTextColor = RGB(255,0,0);
			ApplyFormat(131, 140);
			PrepareCharFormat(CFV[5], CFV[5]);
			cf.crTextColor = RGB(0,0,255);
			ApplyFormat(53, 58);
			re.SetSel(0,0);
		}
	}
	re.SetModify(FALSE);
}

void CEmpView::OnDestroy()
{
	// Deactivate the item on destruction; this is important
	// when a splitter view is being used.
   COleClientItem* pActiveItem = GetDocument()->GetInPlaceActiveItem(this);
   if (pActiveItem != NULL && pActiveItem->GetActiveView() == this)
   {
      pActiveItem->Deactivate();
      ASSERT(GetDocument()->GetInPlaceActiveItem(this) == NULL);
   }
   CRichEditView::OnDestroy();
}


/////////////////////////////////////////////////////////////////////////////
// CEmpView diagnostics

#ifdef _DEBUG
CEmpDoc* CEmpView::GetDocument() // non-debug version is inline
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CEmpDoc)));
	return (CEmpDoc*)m_pDocument;
}
#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// CEmpView message handlers

void CEmpView::GoTop()
{
	cur_qst = 0;
	GotoQuestion(cur_qst);
}

void CEmpView::GoPrev()
{
	if (cur_qst > 0) {
		cur_qst--;
		GotoQuestion(cur_qst);
	}
}

void CEmpView::GoNext()
{
	if (cur_qst < qstNo-1) {
		cur_qst++;
		GotoQuestion(cur_qst);
	}
}

void CEmpView::GoBottom()
{
	cur_qst = qstNo-1;
	GotoQuestion(cur_qst);
}

void CEmpView::MarkAnswer(int qst, int ans, int mark, CRichEditCtrl& re)
{
	long spos, epos;
	int	 sline;

	PrepareCharFormat(/*CFV[2]|*/CFV[5], /*CFV[2]|*/CFV[5]);
	if (mark==1) {
		m_frame->pans->SeleItem(qst, ans);
		cf.crTextColor = RGB(0,0,255);
	}
	else {
		cf.dwEffects = 0;
		cf.crTextColor = (mark)?(RGB(0,96,0)):(RGB(32,32,128));
	}

	sline = qsts[qst].ansLine[ans];
	while ( (ans == qsts[qst].ansNo-1 && sline <= qsts[qst].lSub ) ||
			(ans < qsts[qst].ansNo-1 && sline < qsts[qst].ansLine[ans+1])) {
		spos = re.LineIndex(sline)+3;
		epos = re.LineIndex(sline+1)-1;
		re.HideSelection(TRUE, FALSE);
		ApplyFormat(spos, epos);
		sline++;
	}
}

int CEmpView::NewItem(int item, const char* filename)
{
	DWORD		ItemPos, ItemEnd;
	WORD		ItemNo;
	char*		cbuf;
	CArchive*	aRead = NULL;

	CFile* pFile;
	if (item == -1) {
		// directly reading from buffer
		pFile = (CFile*) filename;
		ItemEnd = (DWORD) pFile->GetLength();
		ItemPos = 0;
	}
	else {
		// read from zmp file
		pFile = new CFile(filename, CFile::modeRead|CFile::shareDenyNone);
		if (pFile == NULL) return 0;

		cbuf = new char[1024];
		ItemPos = (DWORD) pFile->GetLength();
		if (ItemPos > 1024) ItemPos = 1024;
		pFile->Read(cbuf, ItemPos);

		ItemNo	= *((WORD*)(cbuf+EmpIDL+MENU_TITLE_LEN+sizeof(WORD)))-1;
		
		if (item < ItemNo) {
			ItemNo = item;
			ItemEnd= *(((DWORD*)(cbuf+EmpIDL+MENU_TITLE_LEN+2*sizeof(WORD))) + ItemNo + 1);
		}
		else 
			ItemEnd = (DWORD) pFile->GetLength()+1;
		ItemPos = *(((DWORD*)(cbuf+EmpIDL+MENU_TITLE_LEN+2*sizeof(WORD))) + ItemNo);
		delete cbuf;
		if (ItemPos >= ItemEnd) {
			pFile->Close();
			delete pFile;
			return 0;
		}
		pFile->Seek(ItemPos, CFile::begin);
	}

	TRY
	{
		CWaitCursor wait;
		ShowWindow(SW_HIDE);

		aRead = new CArchive(pFile, CArchive::load);
		aRead->m_bForceFlat = FALSE;

		// Number of questions
		ItemNo = DoLoad(*aRead);

		// ImageMark '?' is used
		m_frame->pans->ViewAnswer(ItemNo, 0);

		aRead->Close(); delete aRead;
		pFile->Close(); delete pFile; pFile = NULL;

		ShowWindow(SW_SHOW);
	}

	CATCH_ALL(e)
	{
		if (aRead != NULL) {
			aRead->Close(); delete aRead;
		}
		if (pFile != NULL) {
			pFile->Close(); delete pFile;
		}

		e->ReportError(MB_OK|MB_ICONEXCLAMATION);
		e->Delete();
		return 0;
	}
	END_CATCH_ALL

	return ItemNo;
}

// return number of questions in the item loaded
int CEmpView::DoLoad(CArchive &ar)
{
	CEmpDoc*		pDoc;
	char			s[PARALEN];
	WORD			socau, sopt;
	int				i;
	long			sPos, ePos, TtLong=0;
	BOOL			isTt=TRUE;
	CRichEditCtrl&	re = GetRichEditCtrl();
	CString			te;
	PARAFORMAT		pr;

	qstNo = 0;
	pDoc = (CEmpDoc*)GetDocument();
	pDoc->DeleteContents();
	te.Empty();
	ar >> socau;							// so cau hoi
	ar >> sopt;								// so dong
	if (m_frame->levelSet) delete m_frame->levelSet;
	m_frame->levelSet = new unsigned char[socau];
	ar.Read(m_frame->levelSet, socau);		// he so cau hoi
	if (qsts != NULL) delete qsts;
	qsts = new Qst[socau];

	// read Text
	m_frame->m_wndStatusBar.SetPaneText(0, "Text Loading...");
	for (i=0; i<sopt; i++) {
		ar.ReadString(s, PARALEN);
		if (strncmp(s, SAQ, QNOLEN)) {
			switch (s[0]) {
			case '\0':
				isTt = FALSE;
				break;
			case '$':
				qsts[qstNo-1].res = qsts[qstNo-1].ansNo;
			case '#':
				qsts[qstNo-1].ansLine[qsts[qstNo-1].ansNo] = i;
				s[1] = (char)(97+qsts[qstNo-1].ansNo);
				qsts[qstNo-1].ansNo += 1;
			case '!':
				if (s[0]=='!') s[1] = ' ';
				qsts[qstNo-1].lSub = i;
				s[0] = s[2] = '\t';
			}
		}
		else {
			isTt = FALSE;
			qsts[qstNo].fMain = i;	// Question start line
			qsts[qstNo].ansNo = 0;	// number of answer now
			qsts[qstNo].cans  = -1;	// NoAnswer now
			qsts[qstNo].res   = -1;	// Will be changed
			qstNo++;				// new question
			sprintf(s, "%3d", qstNo); s[3] = '/';
		}
		strcat(s, "\r");
		te.Insert(te.GetLength(), s);
		if (isTt) 
			TtLong = TtLong + strlen(s);
	}
	SetStdFormat();
	re.SetWindowText(te);
	TLines = re.GetLineCount();

	// Set TabStops
	pr.cbSize = sizeof(PARAFORMAT);
	pr.dwMask = PFM_ALIGNMENT|PFM_TABSTOPS;
	pr.wAlignment = PFA_LEFT;
	pr.cTabCount  = 15;
	pr.rgxTabs[0] = 400;  pr.rgxTabs[1] = 650;  pr.rgxTabs[2] = 1000;
	pr.rgxTabs[3] = 1500; pr.rgxTabs[4] = 2000; pr.rgxTabs[5] = 2500;
	pr.rgxTabs[6] = 3000; pr.rgxTabs[7] = 3500; pr.rgxTabs[8] = 4000;
	pr.rgxTabs[9] = 4500; pr.rgxTabs[10]= 5000; pr.rgxTabs[11]= 5500;
	pr.rgxTabs[12]= 6000; pr.rgxTabs[13]= 6500; pr.rgxTabs[14]= 7000;
	re.SetSel(0,-1); re.SetParaFormat(pr);

	// Title Centered
	pr.dwMask = PFM_ALIGNMENT;
	pr.wAlignment = PFA_CENTER;
	re.SetSel(0,TtLong); re.SetParaFormat(pr);

	// read format
	m_frame->m_wndStatusBar.SetPaneText(0, "Formatted Text Loading...");
	memset(&cf, 0, sizeof(CHARFORMAT2));
	cf.cbSize = sizeof(CHARFORMAT2);
	for (i=0; i<NoCF; i++) {
		PrepareCharFormat(CFV[i], CFV[i]);
		ar >> sopt;
		while (sopt--) {
			if (CFV[i] == CFM_COLOR) 
				ar >> cf.crTextColor;
			ar >> sPos;
			ar >> ePos;
			ApplyFormat(sPos, ePos);
		}
	}
	// Applying own formats
	sPos = TtLong-2;
	while (sPos && te.GetAt(sPos) != '\r') sPos--;
	PrepareCharFormat(CFV[0], CFV[0]);
	ApplyFormat(0, TtLong);
	PrepareCharFormat(CFV[0]|CFV[2]|CFV[5], CFV[0]|CFV[2]|CFV[5]);
	cf.crTextColor = RGB(255,0,0);
	ApplyFormat(sPos, TtLong);
	for (TtLong=0; TtLong<qstNo; TtLong++) {
		// Question number format
		sPos = re.LineIndex(qsts[TtLong].fMain);
		PrepareCharFormat(CFV[0]|CFV[2]|CFV[5], CFV[0]|CFV[2]|CFV[5]);
		cf.crTextColor = RGB(0,0,255);
		ApplyFormat(sPos, sPos+4);

		// Sub answers number format
		PrepareCharFormat(CFV[5], CFV[5]);
		cf.crTextColor = RGB(32,32,128);
		sPos = re.LineIndex(qsts[TtLong].ansLine[0]);
		ePos = re.LineIndex(qsts[TtLong].lSub + 1);
		ApplyFormat(sPos, ePos);

		// a,b,c,... format
		PrepareCharFormat(CFV[5]|CFV[0], CFV[5]|CFV[0]);
		cf.crTextColor = RGB(128,0,128);
		for (i=0; i<qsts[TtLong].ansNo; i++) {
			sPos = re.LineIndex(qsts[TtLong].ansLine[i]);
			ApplyFormat(sPos, sPos+3);
		}
	}

	pDoc->SetModifiedFlag(FALSE);
	re.SetSel(0,0);
	return socau;
}

void CEmpView::SetStdFormat()
{
	CRichEditCtrl& re = GetRichEditCtrl();

	memset(&cf, 0, sizeof(CHARFORMAT2));
	cf.cbSize = sizeof(CHARFORMAT2);
	cf.dwMask = CFM_FACE|CFM_SIZE|CFM_BOLD|CFM_UNDERLINE|CFM_ITALIC|CFM_COLOR;
	cf.dwEffects = 0;
	cf.crTextColor = 0;
	cf.yHeight = CHAR_CY;
	strcpy(cf.szFaceName, "EMP.FONT");

	re.SetSel(0,0);
	re.SetSelectionCharFormat(cf);
	re.SetDefaultCharFormat(cf);

	// Set the printing margins (720 twips = 1/2 inch).
	SetMargins(CRect(720, 720, 720, 720));
	re.SetReadOnly(TRUE);
}

void CEmpView::GotoQuestion(int qstNo)
{
	long spos;
	CRichEditCtrl& re = GetRichEditCtrl();
	spos = re.LineIndex(qsts[qstNo].fMain);
	re.LineScroll(qsts[qstNo].fMain-re.GetFirstVisibleLine());
	re.SetSel(spos,spos);
	m_frame->pans->SeleItem(qstNo, -1);
}

BOOL CEmpView::OnCmdMsg(UINT nID, int nCode, void* pExtra, AFX_CMDHANDLERINFO* pHandlerInfo) 
{
	if (qstNo < 1) return FALSE;

	return CRichEditView::OnCmdMsg(nID, nCode, pExtra, pHandlerInfo);
}

void CEmpView::OnLButtonDblClk(UINT nFlags, CPoint point)
{
	if (qstNo < 1) return;

	CRichEditCtrl& re = GetRichEditCtrl();
	long chrPos = re.CharFromPos(point);
	CRichEditCntrItem* pItem;

	re.SetSel(chrPos, chrPos);

	if ((pItem = GetSelectedItem()) != NULL)
		m_frame->DoObjectView(pItem);
}

void CEmpView::ApplyFormat(long startPos, long endPos)
{
	CRichEditCtrl& re = GetRichEditCtrl();
	re.SetSel(startPos, endPos);
	re.SetSelectionCharFormat(cf);
}

void CEmpView::OnQuestionMark(CPoint* point)
{
	int	 tmp;
	int	 line;
	long sCpos;
	CRichEditCtrl& re = GetRichEditCtrl();

	// set caret to mouse pointer.
	if (point == NULL)
		re.GetSel(sCpos, sCpos);
	else
		sCpos = re.CharFromPos(*point);

	line = re.LineFromChar(sCpos);

	// call question View
	if (GetQstAnsFromLine(line, cur_qst, tmp) || m_frame->isAnsView)
		m_frame->pans->SeleItem(cur_qst, -1);
	else
	if (tmp != qsts[cur_qst].cans) {
		oans = -1;
		if (qsts[cur_qst].cans > -1)
			MarkAnswer(cur_qst, qsts[cur_qst].cans, 0, re);
		qsts[cur_qst].cans = tmp;
		MarkAnswer(cur_qst, qsts[cur_qst].cans, 1, re);
		// Inform New Answers
	}

	// set caret to mouse pointer.
	re.SetSel(sCpos, sCpos);
}

void CEmpView::OnLButtonDown(UINT nFlags, CPoint point) 
{
	return;	// keep the timer process work well
}

void CEmpView::OnLButtonUp(UINT nFlags, CPoint point)
{
	if (qstNo > 0) OnQuestionMark(&point);
}

int CEmpView::ViewResult(BOOL view, float& score)
{
	int		vi, vj=0;
	float	tongdiem=0, tongheso=0;
	char	s[7];
	
	CListCtrl& lc = m_frame->pans->GetListCtrl();
	for (vi=0; vi<qstNo; vi++) {
		if (m_frame->TestType == 0) {
			if (view)
				sprintf(s, "%d%c", vi+1, (char)(97+qsts[vi].res));
			else
				sprintf(s, "%d", vi+1);
			lc.SetItem(vi, 0, LVIF_TEXT, s, 0, 0, 0, 0);
		}
		tongheso = tongheso + m_frame->levelSet[vi];
		if (qsts[vi].cans == qsts[vi].res) {
			tongdiem = tongdiem + m_frame->levelSet[vi];
			vj++;
		}
	}
	score = 10*tongdiem/tongheso;
	return vj;
}

void CEmpView::OnActivateView(BOOL bActivate, CView* pActivateView, CView* pDeactiveView) 
{
	if (bActivate)
		m_frame->GotoPane(ID_TEST, FALSE);
	CRichEditView::OnActivateView(bActivate, pActivateView, pDeactiveView);
}

BOOL CEmpView::SetAnswer(int aLen, char *aBuf)
{
	int		si;
	BOOL	kq=TRUE;
	CRichEditCtrl& re = GetRichEditCtrl();

	ShowWindow(SW_HIDE);
	for (si=0; si < aLen; si += 2) {
		qsts[si/2].cans = *(aBuf+si);
		if (qsts[si/2].cans > -1) {
			re.GetLineCount();	// Make the RichEdit rest
			MarkAnswer(si/2, qsts[si/2].cans, 1, re);
		}
		// if the backup data is not correct
		if (qsts[si/2].res != *(aBuf+si+1))
			kq = FALSE;
	}
	re.SetSel(0,0);
	ShowWindow(SW_SHOW);
	return kq;
}

void CEmpView::GetResponse(int aLen, char *aBuf)
{
	int si;
	for (si=1; si <= aLen; si += 2)
		*(aBuf+si) = qsts[si/2].res;
}

BOOL CEmpView::OnSetCursor(CWnd* pWnd, UINT nHitTest, UINT message) 
{
	switch (nHitTest) {
	case HTCLIENT:
		if (oans == -1) break;
		SetCursor(m_frame->mypointcursor);
		return 1;
	case HTVSCROLL:
		SetCursor(m_frame->mypullcursor);
		return 1;
	}
	return CRichEditView::OnSetCursor(pWnd, nHitTest, message);
}

void CEmpView::OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags) 
{
	if (qstNo < 1 || GetAsyncKeyState(VK_SHIFT) || 
		MalCtrlKey(nChar)) return;
	if (nChar == VK_SPACE || nChar == VK_RETURN)
		OnQuestionMark(NULL);
	else {
		CRichEditView::OnKeyDown(nChar, nRepCnt, nFlags);
		if (nChar == VK_DOWN || nChar == VK_UP ||
			nChar == VK_NEXT || nChar == VK_PRIOR ||
			nChar == VK_LEFT || nChar == VK_RIGHT ||
			nChar == VK_HOME || nChar == VK_END) {
			OnQuestionTouch(NULL);
		}
	}
}

BOOL CEmpView::MalCtrlKey(UINT nChar)
{
	return (GetAsyncKeyState(VK_CONTROL) &&
			nChar != VK_PRIOR &&
			nChar != VK_NEXT &&
			nChar != VK_HOME &&
			nChar != VK_END &&
			nChar != VK_LEFT &&
			nChar != VK_UP &&
			nChar != VK_RIGHT &&
			nChar != VK_DOWN);
}

void CEmpView::OnMouseMove(UINT nFlags, CPoint point) 
{
	if (qstNo > 0) 
		OnQuestionTouch(&point);
}

BOOL CEmpView::GetQstAnsFromLine(int line, int& qst, int& ans)
{
	qst = qstNo-1;
	while (qst>0 && line < qsts[qst].fMain) qst = qst-1;
	if (line < qsts[qst].ansLine[0] || line > qsts[qst].lSub)
		return TRUE;

	ans = 0;
	while (ans < qsts[qst].ansNo-1 &&
			qsts[qst].ansLine[ans+1] <= line) ans = ans+1;
	return FALSE;
}

void CEmpView::OnQuestionTouch(CPoint *point)
{
	int  nqst, nans;
	int  line;
	BOOL ok;
	long sCpos;
	CRichEditCtrl& re = GetRichEditCtrl();
 
	HideCaret();

	if (point == NULL)
		sCpos = -1;
	else
		sCpos = re.CharFromPos(*point);
	line = re.LineFromChar(sCpos);

	if (m_resetCaretPos) {
		if (sCpos == -1)
			sCpos = re.LineIndex(re.GetFirstVisibleLine());
		m_resetCaretPos = FALSE;
	}
	else
		re.GetSel(sCpos, sCpos);

	ok = GetQstAnsFromLine(line, nqst, nans);
	if (oqst != nqst || oans != nans) {
		if (oans != -1) MarkAnswer(oqst, oans, 0, re);

		// call question View
		oqst = nqst;
		if (ok || qsts[oqst].cans == nans || m_frame->isAnsView) {
			oans = -1;
			m_frame->pans->SeleItem(oqst, -1);
		}
		else {
			oans = nans;
			MarkAnswer(oqst, oans, 2, re);
		}
	}
	re.SetSel(sCpos, sCpos);

	ShowCaret();
}

// newCF = Start a new Format
void CEmpView::PrepareCharFormat(DWORD dwMask, DWORD dwEffects)
{
	BOOL isSet;
	cf.dwMask = dwMask;
	cf.dwEffects = (dwEffects & ~CFV[5]);	// CFM_COLOR
	if (dwMask & CFE_SUPERSCRIPT) {
		isSet = (cf.dwEffects & dwEffects);
		cf.yOffset = (isSet)?(-CHAR_CY/7):0;
		cf.dwMask |= CFM_OFFSET;	// | CFM_SIZE);
	}
	if (dwMask & CFE_SUBSCRIPT) {
		isSet = (cf.dwEffects & dwEffects);
		cf.yOffset = (isSet)?(-CHAR_CY/3):0;
		cf.dwMask |= CFM_OFFSET;	// | CFM_SIZE);
	}
	if (dwMask & CFM_FACE) {
		strcpy(cf.szFaceName, SYMBOL);
	}
}

void CEmpView::OnPaint()
{
	CRichEditView::OnPaint();

	CClientDC dc(this);

	// Mark Drawing
	CRichEditCtrl& re = GetRichEditCtrl();
	CRect rectClient;

	int lineCount = re.GetLineCount();
	if (lineCount < 7) return;

	GetClientRect(&rectClient);

	int lineIndex = re.GetFirstVisibleLine();
	int charIndex;
	int in_qst, in_ans;
	CSize size(16,16);

	do {
		charIndex = re.LineIndex(lineIndex);
		CPoint& p = re.PosFromChar(charIndex);
		if (p.y >= rectClient.bottom)
			break;

		if (!GetQstAnsFromLine(lineIndex, in_qst, in_ans) &&
			qsts[in_qst].ansLine[in_ans] == lineIndex) {
			// Mark only on the first line of each answer
			if (qsts[in_qst].cans == in_ans )
				dc.DrawState(p, size, m_hIconMark,DST_ICON,(HBRUSH)NULL);
			if (m_frame->isAnsView && qsts[in_qst].res == in_ans)
				dc.DrawState(p, size, m_hIconAnswer,DST_ICON,(HBRUSH)NULL);
		}
		lineIndex++;
	} while (lineIndex < lineCount);
}

void CEmpView::OnVScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar)
{
	CRichEditView::OnVScroll(nSBCode, nPos, pScrollBar);

	if (nSBCode == SB_ENDSCROLL)
		m_resetCaretPos = TRUE;
}
