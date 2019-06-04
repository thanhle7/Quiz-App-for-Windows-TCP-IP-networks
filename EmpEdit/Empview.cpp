// EmpView.cpp : implementation of the CEmpView class
//

#include "stdafx.h"
#include "EmpED.h"
#include "BldTest.h"
#include "Convert.h"
#include "SymDlg.h"
#include "LevelDlg.h"
#include "Preview.h"
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
	ON_COMMAND(ID_FILE_PRINT_PREVIEW, OnFilePrintPreview)
	ON_COMMAND(ID_BUILDTEST, OnBuildtest)
	ON_COMMAND(ID_CONVERT, OnConvert)
	ON_COMMAND(ID_EDIT_INSERTSYMBOL, OnEditInsertsymbol)
	ON_COMMAND(ID_FILE_PRINT, OnFilePrint)
	ON_COMMAND(ID_FILE_SET_LEVEL, OnFileSetLevel)
	ON_UPDATE_COMMAND_UI(ID_FILE_PRINT, OnUpdateFilePrint)
	ON_UPDATE_COMMAND_UI(ID_FILE_PRINT_PREVIEW, OnUpdateFilePrintPreview)
	ON_COMMAND(ID_FORMAT_BOLD, SetBold)
	ON_COMMAND(ID_FORMAT_ITALIC, SetItalic)
	ON_COMMAND(ID_FORMAT_UNDERLINE, SetUnderline)
	ON_COMMAND(ID_FORMAT_STRIKEOUT, SetStrike)
	ON_COMMAND(ID_FORMAT_COLOR, SetColor)
	ON_COMMAND(ID_FILE_PRINT_LOAD, OnFilePrintLoad)
	//}}AFX_MSG_MAP
	ON_CBN_SELENDOK(ID_FORMAT_FONT, SetFont)
	ON_COMMAND(ID_FORMAT_FONT, SetFont)
	ON_COMMAND(ID_FORMAT_SUBSCRIPT, SetSubscript)
	ON_COMMAND(ID_FORMAT_SUPERSCRIPT, SetSuperscript)
	ON_WM_RBUTTONDOWN()
	ON_WM_KEYUP()
	ON_WM_PAINT()
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CEmpView construction/destruction

CEmpView::CEmpView()
{
	level = 1;
	isNoBackgrndProcess = true;
	highBrush.CreateSolidBrush(RGB(250,0,0));
	highPen.CreatePen(PS_SOLID, 1, RGB(210,0,0));
	lowPen.CreatePen(PS_SOLID, 1, RGB(230,230,230));
}

CEmpView::~CEmpView()
{
	highPen.DeleteObject();
	lowPen.DeleteObject();
	highBrush.DeleteObject();
}

void CEmpView::OnInitialUpdate()
{
	CRichEditView::OnInitialUpdate();
	// Turn on the horizontal scroll bar
	m_nWordWrap = CRichEditView::WrapNone;
	WrapChanged();

	if (GetWindowTextLength() == 0) {
		SetStdFormat(TRUE);
	}
	ShowWindow(SW_SHOW);
}

void CEmpView::OnDestroy()
{
	// Destroy all inplace-edit item.
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

void CEmpView::SetFont()
{
	CMainFrame* frame = (CMainFrame*)AfxGetApp()->m_pMainWnd;

	CRichEditCtrl& re = GetRichEditCtrl();
	CHARRANGE cr;

	re.GetSel(cr);
	memset(&cf, 0, sizeof(CHARFORMAT2));
	cf.cbSize = sizeof(CHARFORMAT2);
	cf.dwMask = CFM_FACE;
	cf.dwEffects = 0;
	int cursel = frame->m_cmbFontName.GetCurSel();
	if (cursel)
		strcpy(cf.szFaceName, SYMBOL);
	else
		strcpy(cf.szFaceName, "EMP.Font");
	
	GetRichEditCtrl().SetSel(cr);
	GetRichEditCtrl().SetSelectionCharFormat(cf);
	SetFocus();

	frame->m_cmbFontNameFree = TRUE;
}

void CEmpView::SetStdFormat(BOOL empFont)
{
	PARAFORMAT pr;
	CRichEditCtrl& re = GetRichEditCtrl();

	memset(&cf, 0, sizeof(CHARFORMAT2));
	cf.cbSize = sizeof(CHARFORMAT2);
	cf.dwMask = CFM_FACE|CFM_SIZE|CFM_BOLD|CFM_UNDERLINE|CFM_ITALIC|CFM_COLOR;
	cf.dwEffects = 0;
	cf.crTextColor = 0;
	cf.yHeight = CHAR_CY;
	if (empFont) 
		strcpy(cf.szFaceName, "EMP.FONT");
	else
		strcpy(cf.szFaceName, "halong");

	re.SetSel(0,-1);
	re.SetSelectionCharFormat(cf);
	re.SetDefaultCharFormat(cf);

	// Set TabStops
	memset(&pr, 0, sizeof(PARAFORMAT));
	pr.cbSize = sizeof(PARAFORMAT);
	pr.dwMask = PFM_ALIGNMENT|PFM_TABSTOPS;
	pr.wAlignment = PFA_LEFT;
	pr.cTabCount  = 15;
	pr.rgxTabs[0] = 450;  pr.rgxTabs[1] = 650;  pr.rgxTabs[2] = 1000;
	pr.rgxTabs[3] = 1500; pr.rgxTabs[4] = 2000; pr.rgxTabs[5] = 2500;
	pr.rgxTabs[6] = 3000; pr.rgxTabs[7] = 3500; pr.rgxTabs[8] = 4000;
	pr.rgxTabs[9] = 4500; pr.rgxTabs[10]= 5000; pr.rgxTabs[11]= 5500;
	pr.rgxTabs[12]= 6000; pr.rgxTabs[13]= 6500; pr.rgxTabs[14]= 7000;
	re.SetSel(0,0); re.SetParaFormat(pr);

	// Set the printing margins (720 twips = 1/2 inch).
	SetMargins(CRect(720, 720, 720, 720));

	// no modify
	GetDocument()->SetModifiedFlag(FALSE);
}

void CEmpView::SetBold()
{
	ApplyCharFormat(CFM_BOLD, CFE_BOLD);
}

void CEmpView::SetItalic()
{
	ApplyCharFormat(CFM_ITALIC, CFE_ITALIC);
}

void CEmpView::SetUnderline()
{
	ApplyCharFormat(CFM_UNDERLINE, CFE_UNDERLINE);
}

void CEmpView::SetStrike()
{
	ApplyCharFormat(CFM_STRIKEOUT, CFE_STRIKEOUT);
}

void CEmpView::SetColor()
{
	CMainFrame* frame = (CMainFrame*)AfxGetApp()->m_pMainWnd;

	GetRichEditCtrl().GetSelectionCharFormat(cf);
	cf.dwMask = CFM_COLOR;
	cf.crTextColor = frame->m_btnColor.m_Color;
	SetCharFormat(cf);
}

BOOL CEmpView::OnPreparePrinting(CPrintInfo* pInfo) 
{
	return DoPreparePrinting(pInfo);
}

void CEmpView::OnPrint(CDC* pDC, CPrintInfo* pInfo) 
{
	char PageNo[10];
	sprintf(PageNo,"Page %d",pInfo->m_nCurPage);
	pDC->TextOut(pInfo->m_rectDraw.left, pInfo->m_rectDraw.bottom-50, PageNo);
	CRichEditView::OnPrint(pDC, pInfo);
}

void CEmpView::OnFilePrintPreview() 
{
	CPrintPreviewState* pState = new CPrintPreviewState;

	if (!DoPrintPreview(IDD_PREVIEW, this, RUNTIME_CLASS(CMyPreviewView), pState))
	{
		TRACE0("Error: OnFileImprovedprintpreview failed.\n");
		AfxMessageBox(AFX_IDP_COMMAND_FAILURE);
		delete pState;      // preview failed to initialize, delete State now
	}
}

void CEmpView::GetFileDlgCtrl(CWnd *pr, int Id, BOOL toZmp, BOOL isRead, int mode)
{
	char s[_MAX_PATH];
	int	 i;
	CFileDialog* f = new CFileDialog(isRead);

	f->m_ofn.Flags |= mode;
	if (isRead)
		f->m_ofn.lpstrFilter = "All Files (*.*)\0 *.*";
	else
		if (toZmp) {
			f->m_ofn.lpstrFilter = "Test Document (*.zmp)\0 *.zmp\0 All Files (*.*)\0 *.*";
			f->m_ofn.lpstrDefExt = ".zmp";
		}
		else {
			f->m_ofn.lpstrFilter = "Editor Document (*.emp)\0 *.emp\0 All Files (*.*)\0 *.*";
			f->m_ofn.lpstrDefExt = ".emp";
		}

	f->m_ofn.lpstrTitle = (isRead)?"Open":
		((toZmp)?"Save Compressed Data To":"Save Document To");
	if (f->DoModal() == IDOK) {
		strcpy(s, f->GetPathName());
		if (!isRead) {
			i = strlen(s);
			while (i>0 && s[i] != '\\' && s[i] != '.') i--;
			if (s[i] != '.')
				strcat(s, (toZmp)?".zmp":".emp");
		}
		pr->SetDlgItemText(Id, s);
	}
	delete f;
}

void CEmpView::AddCF(pCF &sp, pCF &ep, long p1, long p2, long val)
{
	pCF tmp = (pCF)malloc(sizeof(CF));
	tmp->next = NULL;
	tmp->p1 = p1; tmp->p2 = p2; tmp->val = val;
	if (sp == NULL) sp = tmp;
	else ep->next = tmp;
	ep = tmp;
}

void CEmpView::FreeCFs(pCF &p)
{
	pCF tmp;
	while (p != NULL) {
		tmp = p;
		p = p->next;
		free(tmp);
	}
}

void CEmpView::FreeQuestion(pQuestion &p)
{
	int	fi;
	for (fi=0; fi<NoCF; fi++)
	if (p->cf[fi] != NULL)
		FreeCFs(p->cf[fi]);
	free(p);
}

void CEmpView::FreeQuestions(pQuestion &p)
{
	pQuestion tmp;
	while (p != NULL) {
		tmp = p; p = p->next;
		FreeQuestion(tmp);
	}
}

// [lp1, lp2]	: start line - end line
// [cp1, cp2]	: start char - end char	of the question
pQuestion CEmpView::NewQuestion(char grpID, char level, long lp1, long lp2, long cp1, long cp2)
{
	int ci;
	pQuestion tmp = (pQuestion) malloc(sizeof(Question));
	tmp->lp1 = lp1, tmp->lp2 = lp2;
	tmp->cp1 = cp1, tmp->cp2 = cp2;
	tmp->next = NULL;
	for (ci=0; ci<NoCF; ci++)
		tmp->cf[ci] = tmp->lcf[ci] = NULL;
	tmp->groupID = grpID;
	tmp->level = level;
	return tmp;
}

// Do not allow to use charformat in each sub answer.
void CEmpView::SaveQuestion(CArchive &ar, CRichEditCtrl &re, pQuestion pq, pCF *pf)
{
	char	s[1024];
	WORD	si, sj;
	pCF		fm;
	CFile*	f = ar.GetFile();
	int		sSub = -1, endCF;
	DWORD	l;

	l = (DWORD)(f->GetPosition());
	ar << l;
	// write texts
	ar << (WORD) (pq->lp2 - pq->lp1 + 1);	// so dong
	ar << (DWORD)(pq->cp2 - pq->cp1 + 1);	// so ky tu
	for (si=pq->lp1; si<=pq->lp2; si++) {
		sj = re.GetLine(si, s, 1024);		// '\r' at the end of string (rich edit 2.0)
		s[sj] = '\n'; s[sj+1] = '\0';		// To compatible to CArchive
		ar.WriteString(s);
		if (sSub == -1 && (strncmp(s,"#\t\t",3)==0 || strncmp(s,"$\t\t",3)==0))
			sSub = re.LineIndex(si);
	}
	// sSub is start position of sub answers in the question
	// it is also the end position of Character formats
	if (sSub == -1) sSub = pq->cp2;

	// write formats
	for (si=0; si<NoCF; si++) {
		fm = *(pf+si);
		sj = 0;
		// sSub is used to replace pq->cp2
		// only CFM_FACE, CFM_SUPERSCRIPT, CFM_SUBSCRIPT can be 
		// used through the sub answers of question
		endCF = (CFV[si] & (CFM_FACE|CFE_SUPERSCRIPT|CFM_SUBSCRIPT))?pq->cp2:sSub;
		while (fm != NULL && fm->p1 < endCF) {
			if (fm->p2 >= pq->cp1 + QNOLEN) sj++;
			fm = fm->next;
		}
		ar << sj;		// number of format range
		if (sj) {
			fm = *(pf+si);
			while (sj--) {
				fm->p1 = fm->p1 - pq->cp1;				// relational position
				if (fm->p1 < QNOLEN) fm->p1 = QNOLEN;
				if (fm->p2 > pq->cp2) {
					if (CFV[si] == CFM_COLOR) ar << fm->val;	// if color format
					ar << (WORD) fm->p1;				// start format
					fm->p1 = pq->cp2 + QNOLEN;
					ar << (WORD) (endCF - pq->cp1+1);	// end format
				}
				else {
					if (fm->p2 < pq->cp1 + QNOLEN) sj++;
					else {
						if (CFV[si] == CFM_COLOR) ar << fm->val;// if color format
						ar << (WORD) fm->p1;					// start format
						if (fm->p2 < endCF)
							ar << (WORD) (fm->p2 - pq->cp1);	// end format
						else
							ar << (WORD) (endCF - pq->cp1);
					}
					fm = fm->next;
				}
			}
			// skip to next question format for the next call to SaveQuestion
			while (fm != NULL && fm->p1 <= pq->cp2) {
				if (fm->p2 > pq->cp2) {
					fm->p1 = pq->cp2 + QNOLEN;
					break;
				}
				fm = fm->next;
			}
			*(pf+si) = fm;
		}
	}

	ar.Flush();
	f->Seek(l, CFile::begin);
	l = ((DWORD)f->GetLength())-l - sizeof(DWORD);
	f->Write(&l, sizeof(DWORD));
	f->SeekToEnd();
}

void CEmpView::OnBuildtest() 
{
	CBldTest* btdlg;
	btdlg = new CBldTest(this);
	btdlg->DoModal();
	delete btdlg;
}

void CEmpView::OnConvert() 
{
	CConvert cvtdlg(this);
	cvtdlg.DoModal();
}

WORD CEmpView::AdjustPos(WORD pos, WORD* list)
{
	int ai = 0;
	while (list[ai] < pos) ai++;
	return (WORD)(pos - ai);
}

BOOL CEmpView::ValidEmpDoc(char* FileName)
{
	CString	 tb;
	BOOL	 updateOK = TRUE;

	tb.Format(	"The document below is an old-version one\n\n%s\n\n"
				"You must have it updated for use now.",
				FileName);
	if (MessageBox(tb,"Wrong version",MB_YESNO|MB_ICONQUESTION)==IDNO)
		return false;

	isNoBackgrndProcess = false;

		CWaitCursor Wait;
		ShowWindow(SW_HIDE);

		updateOK = ( GetDocument()->OnOpenDocument(FileName) &&
					 GetDocument()->OnSaveDocument(FileName) );

		GetDocument()->OnNewDocument();
		ShowWindow(SW_SHOW);

	isNoBackgrndProcess = true;
	return updateOK;
}

void CEmpView::DoLoad(CArchive &ar)
{
	char			s[1024];
	WORD			i, socau, sopt, tmp;
	DWORD			Spos, sokt, qSize;
	CRichEditCtrl&	re = GetRichEditCtrl();
	CString			te;
	pCF				sf[NoCF], ef[NoCF];
	CHARFORMAT2		cfs;

	// ------------ for old version of richtext 1.0
	BOOL	isRTF_10;
	WORD	*lineBegin;
	int		l10;
	// ------------ end

	// used in printing
	isPrinting = PrintPrepare;
	PrintPrepare = FALSE;

	te.Empty();
	for (i=0; i<NoCF; i++) sf[i] = ef[i] = NULL;
	ar >> level;
	ar >> socau;
	ar >> i;						// has main info or not - document version also

	if (EMPDOC_VERSION < HIBYTE(i)) {
		MessageBox(	"This Document was created by a newer version of Editor.\n"
					"Select the right one for more appropriate uses.",
					"LNTsoft", MB_OK|MB_ICONEXCLAMATION);
		return;
	}

	isRTF_10 = (HIBYTE(i) == 0);	// under ver 2.1
	// for ver 2.2 or higher
	if (HIBYTE(i) > 21) {
		ar >> sopt;						// number of questions group
		ar.Flush();
		tmp = sopt*sizeof(WORD);		// skip group questions number table
		if (HIBYTE(i) > 22)
			tmp = tmp + sopt;			// skip groups level table - ver 2.3 or higher
		tmp = tmp + sopt*sizeof(DWORD);	// skip groups allocation table - ver 2.2 or higher
		ar.GetFile()->Seek(tmp, CFile::current );	// next to info
	}
	
	i = LOBYTE(i);
	if ( i ) socau += 1;
 	
	Spos = 0;			// Start Position of each question
	while (socau) {
		socau--;
		ar >> qSize;
		// read text
		ar >> sopt;		// so dong
		ar >> sokt;		// so ky tu
		
		if (isRTF_10) lineBegin = new WORD[sopt]; 
		l10 = 0;

		while (sopt) {
			sopt--;
			ar.ReadString(s, 1024);
			if (socau || sopt) strcat(s, "\r");	// '\n' is not need from richedit 2.0
			te.Insert(te.GetLength(), s);

			if (isRTF_10) {
				lineBegin[l10] = strlen(s) + 1;
				if (l10)
					lineBegin[l10] += lineBegin[l10-1];
				l10++;
			}
		}
		// read format
		for (i=0; i<NoCF; i++) {
			if (isRTF_10 && i >= NoCF-2)
				sopt = 0;
			else
				ar >> sopt;
			while (sopt--) {
				AddCF(sf[i], ef[i], 0, 0, 0);
				if (CFV[i] == CFM_COLOR) ar >> ef[i]->val;
				ar >> tmp;
				if (isRTF_10) tmp = AdjustPos(tmp, lineBegin);
				ef[i]->p1 = tmp + Spos;
				ar >> tmp;
				if (isRTF_10) tmp = AdjustPos(tmp, lineBegin);
				ef[i]->p2 = tmp + Spos;
			}
		}

		if (isRTF_10) delete lineBegin;

		Spos = Spos + sokt - l10;
	}
	SetStdFormat(!isPrinting);
	re.SetWindowText(te);

	// set format
	memset(&cfs, 0, sizeof(CHARFORMAT2));
	cfs.cbSize = sizeof(CHARFORMAT2);
	for (i=0; i<NoCF; i++) {
		PrepareCharFormat(cfs, CFV[i], CFV[i]);
		while (sf[i] != NULL) {
			ef[i] = sf[i]; sf[i] = sf[i]->next;
			if (CFV[i] == CFM_COLOR)
				cfs.crTextColor = ef[i]->val;
			re.SetSel(ef[i]->p1, ef[i]->p2);
			SetCharFormat(cfs);
			free(ef[i]);
		}
	}
}

void CEmpView::DoPrintTest(CArchive &ar)
{
	CString			te;
	CHARFORMAT2		cf;
	char			s[PARALEN];
	WORD			socau, sopt;
	int				qstNo = 0;
	qstinfo			*aqsts;		// dap an cac cau hoi
	unsigned char	*levelSet;
	int				i;
	long			sPos, ePos, TtLong=0;
	BOOL			isTt=TRUE;
	CRichEditCtrl&	re = GetRichEditCtrl();

	ShowWindow(SW_HIDE);
	isPrinting = TRUE;

	te.Empty();
	ar >> socau; ar >> sopt;
	aqsts = new qstinfo[socau];
	levelSet= new unsigned char[socau];
	ar.Read(levelSet, socau);
	// read Text
	for (i=0; i<sopt; i++) {
		ar.ReadString(s, PARALEN);
		if (s[0] != '*') {
			switch (s[0]) {
			case '\0':
				isTt = FALSE;
				break;
			case '$':
				aqsts[qstNo-1].ans = aqsts[qstNo-1].numOFans;
			case '#':
				s[1] = (char)(97 + aqsts[qstNo-1].numOFans);
				aqsts[qstNo-1].startsubline[aqsts[qstNo-1].numOFans] = i;
				aqsts[qstNo-1].numOFans += 1;
			case '!':
				if (s[0]=='!') s[1] = ' ';
				s[0] = s[2] = '\t';
			}
		}
		else {
			isTt = FALSE;
			aqsts[qstNo].startline= i;
			aqsts[qstNo].numOFans = 0;	// number of answer now
			aqsts[qstNo].ans	  = -1;	// NoAnswer now
			qstNo++;				// new question
			sprintf(s, "%3d", qstNo); s[3] = '/';
		}
		strcat(s, "\r");
		te.Insert(te.GetLength(), s);
		if (isTt) TtLong += strlen(s);
	}

	// print the answer keys
	strcpy(s, "® p  n cœa ­‹ thi:\r");
	for (i=0; i++<socau; ) {
		sprintf(s + strlen(s), "\t%d-%c[%d]\t",
						i, aqsts[i-1].ans+97, levelSet[i-1]);
		if (i%7==0 || i == socau) {
			strcat(s, "\r");
			te.Insert(te.GetLength(), s);
			s[0] = '\0';
		}
	}

	SetStdFormat(FALSE);
	re.SetWindowText(te);

	// Title Centered - No-use with non-wordwraped richedit ctrl
	//pr.dwMask = PFM_ALIGNMENT;
	//pr.wAlignment = PFA_CENTER;
	//re.SetSel(0,TtLong); re.SetParaFormat(pr);

	// read format
	memset(&cf, 0, sizeof(CHARFORMAT2));
	cf.cbSize = sizeof(CHARFORMAT2);
	for (i=0; i<NoCF; i++) {
		PrepareCharFormat(cf, CFV[i], CFV[i]);
		ar >> sopt;
		while (sopt--) {
			if (CFV[i] == CFM_COLOR) 
				ar >> cf.crTextColor;
			ar >> sPos;
			ar >> ePos;
			re.SetSel(sPos, ePos);
			re.SetSelectionCharFormat(cf);
		}
	}

	// Applying own formats
	sPos = TtLong-2;
	while (sPos && te.GetAt(sPos) != '\r') sPos--;
	cf.dwMask = cf.dwEffects = CFV[0]|CFV[2];
	re.SetSel(0, TtLong);
	re.SetSelectionCharFormat(cf);

	for (TtLong=0; TtLong<qstNo; TtLong++) {
		// Question number format
		sPos = re.LineIndex(aqsts[TtLong].startline);
		re.SetSel(sPos, sPos+3);
		re.SetSelectionCharFormat(cf);

		// a,b,c,... format
		for (i=0; i<aqsts[TtLong].numOFans; i++) {
			sPos = re.LineIndex(aqsts[TtLong].startsubline[i]);
			re.SetSel(sPos+1, sPos+2);
			re.SetSelectionCharFormat(cf);
		}
	}

	GetDocument()->SetModifiedFlag(FALSE);
	re.SetSel(0,0);	

	delete aqsts;
	delete levelSet;
	ShowWindow(SW_SHOW);
	GetDocument()->SetTitle("Printed Test Document");
}

// file position points to next to the end of EMPID
void CEmpView::DoSave(CArchive &ar)
{
	int				j;
	WORD			i, sodong, socau, sonhom, *pSocauInNhom;
	DWORD*			pVitriNhom;
	char*			pMucdoNhom;
	long			l, sokt;
	CRichEditCtrl&	re = GetRichEditCtrl();
	char			s1[6], s0[6], grpID;
	pCF				scf[NoCF], lcf[NoCF];
	bool			IsC[NoCF];
	pQuestion		sq=NULL, eq=NULL, tmpq;
	CHARFORMAT2		cf1, cf2;
	CHARRANGE		cr;
	int				CurviewFLine;

	re.GetSel(cr);
	CurviewFLine = re.GetFirstVisibleLine();

	//sokt = re.GetTextLength();			// used by rtf 1.0
	sokt = re.GetTextLengthEx(GTL_DEFAULT);	// used by rtf 2.0

	// get format info
	for (i=0; i<NoCF; i++) {
		scf[i] = lcf[i] = NULL; IsC[i] = FALSE;
	}
	cf1.cbSize = cf2.cbSize = sizeof(CHARFORMAT2);
	re.SetSel(0,0);
	re.GetSelectionCharFormat(cf1);
	cf1.dwEffects=0;
	cf1.crTextColor = 0;
	for (l=0; l<sokt; l++) {
		re.SetSel(l,l);
		re.GetSelectionCharFormat(cf2);
		// AUTHOR1: how formats are different
		if (cf1.dwEffects != cf2.dwEffects ||
			cf1.crTextColor != cf2.crTextColor ||
			cf1.szFaceName[0] != cf2.szFaceName[0]) {
			for (i=0; i<NoCF; i++)
			if ((CFV[i] != CFM_FACE && CFV[i] != CFM_COLOR && 
					((cf1.dwEffects & CFV[i]) != (cf2.dwEffects & CFV[i])))
				||(CFV[i] == CFM_FACE && cf1.szFaceName[0] != cf2.szFaceName[0])
				||(CFV[i] == CFM_COLOR && cf1.crTextColor != cf2.crTextColor)) {
				// must in different format
				IsC[i] = !IsC[i];
				if (IsC[i])
					AddCF(scf[i], lcf[i], l-1, sokt-1, cf2.crTextColor);
				else {
					lcf[i]->p2 = l-1;
					if (CFV[i] == CFM_COLOR && cf2.crTextColor != 0) {
						AddCF(scf[i], lcf[i], l-1, sokt-1, cf2.crTextColor);
						IsC[i] = !IsC[i];
					}
				}
			}
			cf1 = cf2;
		}
	}
	for (i=0; i<NoCF; i++) lcf[i] = scf[i];

	// build list of questions
	sonhom = socau = i = j = 0;
	sodong = re.GetLineCount();

	// search for the first question mark
	while (j < sodong) {
		l = re.LineIndex(j);
		re.GetLine(j, s1, 5);
		if (s1[0] == '*') break;
		j++;
	}

	// if this topic has the main info at begining
	if ((j < sodong && j > 0 && l > 4) ||
		(j == sodong && sokt > 0) ) {
		// -1 (idx) : means the main info of all questions
		eq = sq = NewQuestion(0, 0, 0, sodong-1, 0, sokt /*-1*/);	// '\r' replaced for '\0'
		i = 1; // this topis has one main info
	}

	// search for questions - s1 is now the first line with '*'
	strcpy(s0, "LNT.");	// any string
	while (j < sodong) {
		socau++;
		s1[5] = '\0';
		if (s0[1] != s1[1] || s0[2] != s1[2]) {
			// new group
			strcpy(s0, s1);
			// get question level from question mark
			level = GETLEVEL(s1[2]);
			sonhom++;
		}
		tmpq = NewQuestion(s1[1], level, j, sodong-1, l, sokt); // '\r' replaced for '\0'
		if (sq == NULL)
			sq = tmpq;
		else {
			eq->lp2 = tmpq->lp1-1;
			eq->cp2 = tmpq->cp1-1;
			eq->next = tmpq;
		}
		eq = tmpq;
		// search for the next question mark
		while (++j < sodong) {
			l = re.LineIndex(j);
			re.GetLine(j, s1, 5);
			if (s1[0] == '*') break;
		}
	}

	ar << level;						// un used from ver 2.3
	ar << socau;
	if ( i ) socau += 1;

	// ------------
	i = MAKEWORD(i, EMPDOC_VERSION);	// Document version (20 = 2.0)
	ar << i;							// has main info or not
	// ------------
	ar << sonhom;
	ar.Flush();
	sokt = (long) ar.GetFile()->GetPosition();
	pSocauInNhom = new WORD[sonhom];
	pMucdoNhom	 = new char[sonhom];
	pVitriNhom   = new DWORD[sonhom];
	memset(pSocauInNhom, 0, sonhom * sizeof(WORD));
	memset(pMucdoNhom, 0, sonhom * sizeof(char));
	ar.Write(pSocauInNhom, sonhom * sizeof(WORD));
	ar.Write(pMucdoNhom, sonhom * sizeof(char));
	ar.Write(pVitriNhom, sonhom * sizeof(DWORD));

	ar.Flush();
	level = 0;		// now freely used as a 'group ID' from ver 2.3 or higher
	grpID = 0;
	j     = -1;		// as the Main-Info exist first
	eq = sq; 
	for (i=0; i<socau; i++) {
		if (eq->groupID != 0) {
			// real questions
			if (grpID != eq->groupID || level != eq->level) {
				grpID = eq->groupID;
				level = eq->level;
				j++;
				pVitriNhom[j] = (DWORD)ar.GetFile()->GetPosition();
				pMucdoNhom[j] = eq->level;
			}
			pSocauInNhom[j] = pSocauInNhom[j] + 1;
		}
		SaveQuestion(ar, re, eq, lcf);
		eq = eq->next;
	}
	ar.Flush();
	ar.GetFile()->Seek(sokt, CFile::begin);
	ar.Write(pSocauInNhom, sonhom * sizeof(WORD));
	ar.Write(pMucdoNhom, sonhom * sizeof(char));
	ar.Write(pVitriNhom, sonhom * sizeof(DWORD));

	delete pSocauInNhom;
	delete pMucdoNhom;
	delete pVitriNhom;

	re.SetSel(0,0);
	re.LineScroll(CurviewFLine);
	re.SetSel(cr);

	for (i=0; i<NoCF; i++) FreeCFs(scf[i]);
	FreeQuestions(sq);
}

BOOL CEmpView::ClearCurrentDoc(BOOL prompt)
{
	if (prompt && GetDocument()->IsModified() &&
			MessageBox("Ignore all changes in current document",
					"Save", MB_YESNO|MB_ICONQUESTION) == IDNO)
		return FALSE;
	return GetDocument()->OnNewDocument();
}

void CEmpView::OnEditInsertsymbol() 
{
	CSymbolDlg dlg(this, &cf);
	dlg.DoModal();
}

void CEmpView::OnFilePrint() 
{
	CRichEditView::OnFilePrint();
}

void CEmpView::OnFileSetLevel() 
{
	char mark[6];
	int  line;
	long oPos, pos;

	CRichEditCtrl& re = GetRichEditCtrl();
	ShowWindow(SW_HIDE);

	re.GetSel(oPos, oPos);
	line = re.LineFromChar(oPos);
	while (line >= 0) {
		re.GetLine(line, mark, 5);
		if (mark[0] == '*') break;
		line--;
	}
	if (line >= 0) {
		mark[5] = '\0';
		CLevelDlg dlg(this, mark);
		if (dlg.DoModal() == IDOK) {
			pos = re.LineIndex(line);
			re.SetSel(pos, pos+5);
			re.ReplaceSel(mark);
			re.SetSel(oPos, oPos);
		}
	}
	ShowWindow(SW_SHOW);
}

void CEmpView::OnUpdateFilePrint(CCmdUI* pCmdUI) 
{
	pCmdUI->Enable(isPrinting);
}

void CEmpView::OnUpdateFilePrintPreview(CCmdUI* pCmdUI) 
{
	pCmdUI->Enable(isPrinting);
}

void CEmpView::OnFilePrintLoad() 
{
	PrintPrepare = TRUE;
	GetParent()->PostMessage(WM_COMMAND, ID_FILE_OPEN, 0L);
}

void CEmpView::SetSubscript()
{
	ApplyCharFormat(CFE_SUBSCRIPT, CFE_SUBSCRIPT);
}

void CEmpView::SetSuperscript()
{
	ApplyCharFormat(CFE_SUPERSCRIPT, CFE_SUPERSCRIPT);
}

void CEmpView::PrepareCharFormat(CHARFORMAT2& cf, DWORD dwMask, DWORD dwEffects)
{
	BOOL isSet;
	cf.dwMask = dwMask;
	if (dwMask != CFM_COLOR) 
		cf.dwEffects ^= dwEffects;
	switch (dwMask) {
		case CFE_SUPERSCRIPT:
			isSet = (cf.dwEffects & dwEffects);
//			cf.yHeight = (isSet)?(CHAR_CY/3):CHAR_CY;
			cf.yOffset = (isSet)?(-CHAR_CY/7):0;
			cf.dwMask |= CFM_OFFSET;	// | CFM_SIZE);
			break;
		case CFE_SUBSCRIPT:
			isSet = (cf.dwEffects & dwEffects);
//			cf.yHeight = (isSet)?(CHAR_CY/3):CHAR_CY;
			cf.yOffset = (isSet)?(-CHAR_CY/3):0;
			cf.dwMask |= CFM_OFFSET;	// | CFM_SIZE);
			break;
		case CFM_FACE:
			strcpy(cf.szFaceName, SYMBOL);
			break;
	}
}

void CEmpView::ApplyCharFormat(DWORD dwMask , DWORD dwEffects)
{
	GetRichEditCtrl().GetSelectionCharFormat(cf);
	PrepareCharFormat(cf, dwMask, dwEffects);
	SetCharFormat(cf);
}

// Update the charformat button interface 
BOOL CEmpView::HasCharFormat(DWORD cfEffect)
{
	GetRichEditCtrl().GetSelectionCharFormat(cf);
	if (cfEffect == CFM_FACE)
		return (strcmp(cf.szFaceName, SYMBOL) == 0);

	return ((cf.dwEffects & cfEffect) != 0);
}

void CEmpView::OnRButtonDown(UINT nFlags, CPoint point)
{
	CRichEditView::OnRButtonDown(nFlags, point);

	POINT p = {point.x, point.y};
	ClientToScreen(&p);
	CWnd* main = AfxGetMainWnd();
	CMenu* barMenu = main->GetMenu();
	CMenu* m_popupTracker = barMenu->GetSubMenu(1);
	m_popupTracker->TrackPopupMenu(TPM_LEFTALIGN|TPM_RIGHTBUTTON,
												p.x, p.y, main);
}

void CEmpView::OnKeyUp(UINT nChar, UINT nRepCnt, UINT nFlags)
{
	int  firstChar, curChar, curLine, ki;
	char lInfo[6];
	char tmpLine[6];
	CRichEditCtrl& re = GetRichEditCtrl();

	CRichEditView::OnKeyUp(nChar, nRepCnt, nFlags);

	curChar = re.CharFromPos(re.GetCaretPos());
	curLine = re.LineFromChar(curChar);
	firstChar = re.LineIndex(curLine);
	strcpy(lInfo, "LNT. ");
	lInfo[re.GetLine(curLine, lInfo, 5)] = '\0';
	if (lInfo[0] == '*') {
		ki = GETLEVEL(lInfo[2]);
		if (curChar - firstChar < 6 && 
			(lInfo[3] != '/' || lInfo[4] != ' ' || ki >= TOPLEVEL)) {
			// format of starting question
			HideCaret();
			if (ki >= TOPLEVEL)
				lInfo[2] = GETLEVELMARK(TOPLEVEL-1);
			ki = strlen(lInfo);
			if (lInfo[3] != '/') {
				// find the previous question mark
				if (ki > 3) ki = ((lInfo[4]=='/')? 6 : 3);
				else ki--;
				lInfo[0] = ' ';
				while (curLine > 0 && lInfo[0] != '*') {
					curLine--;
					re.GetLine(curLine, lInfo, 5);
				}
				// no previous question found
				if (lInfo[0] != '*')
					strcpy(lInfo, "***/ ");
			}
			else {
				if (lInfo[4] == ' ') ki = 5;
				else {
					strcpy(lInfo+4, " ");
					if (ki > 4) ki = 4;
				}
			}

			curLine = (ki==4)?(firstChar+6):(firstChar+5);
			re.SetSel(firstChar, firstChar+ki);
			re.ReplaceSel(lInfo);
			re.SetSel(curLine, curLine);
			ShowCaret();
		}
	}
	else if (ISSUB(lInfo[0])) {
		// start of a sub-answer
		if (curChar - firstChar < 4 && strncmp(lInfo+1, "\t\t", 2)) {
			// format of sub answers
			HideCaret();
			ki = curLine - 1;
			while (ki > -1) {
				re.GetLine(ki, tmpLine, 1);
				if (ISSUB(tmpLine[0])) {
					if (ki < curLine - 1) ki = -1;
					break;
				}
				if (lInfo[0] == '!') {
					ki = -1; break;
				}
				if (tmpLine[0] == '*') break;
				ki--;
			}
			if (ki > -1) {
				curLine = (ISSUB(lInfo[curChar-firstChar-1]))?(firstChar+3):(firstChar+4);
				ki = 1;
				while (lInfo[ki] != '\0' && 
						(lInfo[ki] == '\t' || lInfo[ki] == ' ' ||
						ISSUB(lInfo[ki])))
					ki++;
				strcpy(lInfo+1, "\t\t");
			}
			else {
				// sub-answer can't be accepted
				ki = curChar - firstChar;
				curLine = firstChar;
				lInfo[0] = '\0';
			}
			re.SetSel(firstChar, firstChar+ki);
			re.ReplaceSel(lInfo);
			re.SetSel(curLine, curLine);
			ShowCaret();
		}
	}
}

void CEmpView::OnPaint()
{
	CRichEditView::OnPaint();

	CClientDC dc(this);

	// Mark Drawing
	CRichEditCtrl& re = GetRichEditCtrl();
	CRect rectClient;
	char  QstMark[6], oQstMark[6];

	int lineCount = re.GetLineCount();
	if (lineCount < 1) return;

	HideCaret();

	GetClientRect(&rectClient);

	int lineIndex = re.GetFirstVisibleLine();
	int charIndex;
	strcpy(oQstMark, "LNT"); charIndex = lineIndex-1;
	while (charIndex >= 0) {
		oQstMark[re.GetLine(charIndex, oQstMark, 4)] = '\0';
		if (oQstMark[0] == '*') break;
		charIndex--;
	}

	oldPen	 = dc.GetCurrentPen();
	oldBrush = dc.SelectObject(&highBrush);
	dc.FillSolidRect(1,0,9,rectClient.Height(),RGB(255,255,255));
	do {
		charIndex = re.LineIndex(lineIndex);
		CPoint& p = re.PosFromChar(charIndex);
		if (p.y >= rectClient.bottom)
			break;

		QstMark[re.GetLine(lineIndex, QstMark, 4)] = '\0';
		if (QstMark[0] == '*' && QstMark[3] == '/') {
			// Mark only on the first line of each answer
			dc.SelectObject(oldPen);
			dc.Rectangle(2,p.y+4,9,p.y+11);
			dc.SelectObject(&highPen);
			dc.Rectangle(1,p.y+3,8,p.y+10);
			if (strcmp(QstMark, oQstMark))
				strcpy(oQstMark, QstMark);
			else
				dc.SelectObject(&lowPen);

			dc.MoveTo(1, p.y);
			dc.LineTo(rectClient.right-1, p.y);
		}
		lineIndex++;
		
	} while (lineIndex < lineCount);
	dc.SelectObject(oldPen);
	dc.SelectObject(oldBrush);
	ShowCaret();
}
