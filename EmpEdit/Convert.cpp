// Convert.cpp : implementation file
//

#include "stdafx.h"
#include "EmpED.h"
#include "Convert.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CConvert dialog

CConvert::CConvert(CEmpView* pParent)
	: CDialog(CConvert::IDD, pParent)
{
	myView = pParent;
	//{{AFX_DATA_INIT(CConvert)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
}


void CConvert::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CConvert)
		// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CConvert, CDialog)
	//{{AFX_MSG_MAP(CConvert)
	ON_BN_CLICKED(IDC_CVT_SRCFILES, OnCvtSrcfiles)
	ON_EN_CHANGE(IDC_CVT_SRCFILE, OnChangeCvtSrcfile)
	ON_BN_CLICKED(IDC_CVT_EMPFILE, OnCvtEmpfile)
	ON_BN_CLICKED(IDC_CVT_ZMPFILE, OnCvtZmpfile)
	ON_BN_CLICKED(IDC_CVT_DSTFILES, OnCvtDstfiles)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CConvert message handlers

void CConvert::OnCvtSrcfiles() 
{
	CEmpView* pr = (CEmpView*)GetParent();
	pr->GetFileDlgCtrl(this, IDC_CVT_SRCFILE, FALSE, TRUE, 
						OFN_FILEMUSTEXIST|OFN_READONLY);
}

void CConvert::OnChangeCvtSrcfile() 
{
	char	s[_MAX_PATH];
	CFile	f;

	if (GetDlgItemText(IDC_CVT_SRCFILE, s, 1024) < 1 ||
		!f.Open(s, CFile::modeRead)) return;
	f.Read(s, 3);
	f.Close();
	if (s[0] != '\xDB' || s[1] != '\r') ToZmp = FALSE;
	else ToZmp = TRUE;
	ShowCheck();
}

void CConvert::OnCvtEmpfile() 
{
	ToZmp = FALSE;
	ShowCheck();
}

void CConvert::OnCvtZmpfile() 
{
	ToZmp = TRUE;
	ShowCheck();
}

void CConvert::ShowCheck()
{
	CheckRadioButton(IDC_CVT_EMPFILE, IDC_CVT_ZMPFILE, 
					(ToZmp)?IDC_CVT_ZMPFILE:IDC_CVT_EMPFILE);
	GetDlgItem(IDC_TIMEDONE)->EnableWindow(ToZmp);
	GetDlgItem(IDC_TIMESPIN)->EnableWindow(ToZmp);
	GetDlgItem(IDC_CVT_MENU)->EnableWindow(ToZmp);
	GetDlgItem(IDC_PASSWORD)->EnableWindow(ToZmp);
	GetDlgItem(IDC_PASSWORD2)->EnableWindow(ToZmp);
}

BOOL CConvert::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	ToZmp = FALSE;
	ShowCheck();
	SetDlgItemInt(IDC_TIMEDONE, 25);
	((CSpinButtonCtrl*)GetDlgItem(IDC_TIMESPIN))->SetRange(1, 180);
	return TRUE;
}

void CConvert::OnCvtDstfiles() 
{
	CEmpView* pr = (CEmpView*)GetParent();
	pr->GetFileDlgCtrl(this, IDC_CVT_DSTFILE, ToZmp, FALSE, 
						OFN_CREATEPROMPT|OFN_OVERWRITEPROMPT);
}

void CConvert::OnOK()
{
	CFile	fin, fout;
	char	s[1025];
	CFileException e;
	CWaitCursor wc;

	if (GetDlgItemText(IDC_CVT_SRCFILE, s, 1024) < 1 ||
		!fin.Open(s, CFile::modeRead, &e)) {
		e.ReportError();
		return;
	}

	if (GetDlgItemText(IDC_CVT_DSTFILE, s, 1024) < 1 ||
		!fout.Open(s, CFile::modeCreate|CFile::modeWrite, &e)) {
		fin.Close();
		e.ReportError();
		return;
	}
	if (ToZmp) MakeZmp(&fin, &fout, s);
	else MakeEmp(&fin, &fout, s);
	fin.Close(); fout.Close();
}

void CConvert::MakeZmp(CFile *fin, CFile *fout, char* s)
{
	CArchive*	ain;
	WORD		ItemNo=0, QstNo, LineNo, F_No=0;
	long		cSpos;
	DWORD*		ItemPos;
	pCF			sF=NULL, eF;
	CMemFile*	mfout;
	char		password[21], password2[21], *heso;
	pSubAns		ph=NULL, pc, ptmp;

	if (GetDlgItemText(IDC_PASSWORD, password, 20)==0)
		strcpy(password, "emp");
	if (GetDlgItemText(IDC_PASSWORD2, password2, 20)==0)
		strcpy(password2, "emp");
	if (strcmp(password, password2)) {
		MessageBox(	"Your password and the retyped-one\nare different !",
					"Warning");
		return;
	}

	ain = new CArchive(fin, CArchive::load);
	if (ain == NULL) return;

	fout->Write(EmpID, EmpIDL);
	if (GetDlgItemText(IDC_CVT_MENU, s, MENU_TITLE_LEN-1) < 1)
		strcpy(s, "Menu titled by Editor");
	s[MENU_TITLE_LEN]='\0';
	fout->Write(s, MENU_TITLE_LEN);

	while (ain->ReadString(s, 1024) != NULL) {
		if (s[0] == '\xDB') 
			ItemNo += 1;
	}
	ain->Close(); delete ain;
	ItemNo--;
	if (ItemNo < 1) return;

	fin->SeekToBegin();
	ain = new CArchive(fin, CArchive::load);
	if (ain == NULL) return;

	mfout = new CMemFile();
	ItemPos = new DWORD[ItemNo];

	QstNo = GetDlgItemInt(IDC_TIMEDONE); QstNo += 256*QstNo;	// normal one
	// Abnormal
	QstNo = QstNo - (strcmp(password, "emp") != 0);
	fout->Write(&QstNo, sizeof(QstNo));		// reserved WORD
	fout->Write(&ItemNo, sizeof(ItemNo));
	fout->Write(ItemPos, ItemNo*sizeof(DWORD));
	ItemPos[0] = (DWORD)fout->GetPosition();

	ItemNo = 0;
	while (ain->ReadString(s+2, 1024) != NULL) {
		if (s[2] == ';') continue;
		if (s[2] == '\xDB') {
			if (ItemNo > 0) {
				heso = new char[QstNo];
				memset(heso, 1, QstNo);
				mfout->Write(&QstNo, sizeof(QstNo));
				mfout->Write(&LineNo, sizeof(LineNo));
				mfout->Write(heso, QstNo);
				delete heso;
				// write text out
				WriteTextAll(mfout, ph);
				// write out all formats
				WriteAllEnd(mfout, F_No, sF, eF);
				myView->FreeCFs(sF);
				sF = NULL; F_No = 0;

				ItemPos[ItemNo-1] = (DWORD)fout->GetPosition();
			}
			cSpos = 0;
			QstNo = LineNo = 0;
			mfout->SetLength(0);	// empty memory file
			ItemNo++;
			continue;
		}
		LineProcess(s, cSpos, F_No, sF, eF);
		// put into queue
		ptmp = new SubAns;
		ptmp->info = new char[strlen(s)+1];
		strcpy(ptmp->info, s);
		ptmp->next = NULL;
		if (ph) pc->next = ptmp;
		else ph = ptmp;
		pc = ptmp;

		LineNo += 1;
		if (s[0] == '*' && s[3] == '/') QstNo += 1;
	};
	fout->Seek(EmpIDL+MENU_TITLE_LEN+2*sizeof(ItemNo), CFile::begin);
	fout->Write(ItemPos, (ItemNo-1)*sizeof(DWORD));
	mfout->Close(); delete mfout;
	ain->Close(); delete ain; 
	delete ItemPos;
	if (sF != NULL) myView->FreeCFs(sF);
	if (ph != NULL) WriteTextAll(NULL, ph);	// delete all contents
}

void CConvert::MakeEmp(CFile *fin, CFile *fout, char* s)
{
	CArchive*	ain;
	WORD		QstNo, LineNo=1, F_No=0;
	DWORD		QstSize=0;
	long		CharNo;
	pCF			sF=NULL, eF;
	BOOL		STOP;
	char*		idx;

	ain = new CArchive(fin, CArchive::load);
	if (ain == NULL) return;

	fout->Write(EmpID, EmpIDL);
	fout->Write(&LineNo, 1);			// he so cau hoi
	fout->Write(&QstNo, sizeof(QstNo));
	QstNo = MAKEWORD(0, 21);			// the lastest version adapted to DOS version
	fout->Write(&QstNo, sizeof(QstNo));	// No main info for this topic
	QstNo = 0;

	while (1) {
		STOP = (ain->ReadString(s+2, 1024) == NULL);
		idx = strchr(s, '\x1A');
		if (idx != NULL) *idx = ' ';
		if (STOP || (s[2]=='*' && s[3]=='*')) {
			// write old one
			if (QstSize > 0) {
				fout->Seek(QstSize-sizeof(QstSize), CFile::begin);
				QstSize = ((DWORD)fout->GetLength()) - QstSize + 1;
				fout->Write(&QstSize, sizeof(QstSize));
				fout->Write(&LineNo, sizeof(LineNo));
				fout->Write(&CharNo, sizeof(CharNo));

				// write out all formats
				WriteAllEnd(fout, F_No, sF, eF);
				myView->FreeCFs(sF);
				sF = NULL; F_No = 0;
				QstNo += 1;
			}
			if (STOP) break;
			// start a one
			fout->Write(&QstSize, sizeof(QstSize));
			QstSize = (DWORD)fout->GetPosition();
			LineNo = 0; CharNo = 0;
			fout->Write(&LineNo, sizeof(LineNo));
			fout->Write(&CharNo, sizeof(CharNo));
		}
		if (QstSize > 0) {
			LineProcess(s, CharNo, F_No, sF, eF);
			fout->Write(s, strlen(s));
			LineNo += 1;
		}
	}
	ain->Close(); delete ain;
	fout->Seek(EmpIDL+1, CFile::begin);
	fout->Write(&QstNo, sizeof(QstNo));
	if (sF != NULL) myView->FreeCFs(sF);
}

void CConvert::WriteOutCf(CFile* fs, pCF sf, pCF ef, BOOL color)
{
	while (sf != NULL) {
		ef = sf; sf = sf->next;
		if (color)
			fs->Write(&(ef->val), sizeof(ef->val));
		if (ToZmp) {
			fs->Write(&(ef->p1), sizeof(ef->p1));
			fs->Write(&(ef->p2), sizeof(ef->p2));
		}
		else {
			fs->Write(&(ef->p1), sizeof(WORD));
			fs->Write(&(ef->p2), sizeof(WORD));
		}
	}
}

void CConvert::LineProcess(char* s, long& cSpos, 
						   WORD& fNo, pCF& sF, pCF& eF)
{
	int		i,j,k;
	long	vcolor;

	switch (s[2]) {
	case '*':
		s[0] = '*';
		if (s[3] == '*') s[4] = '/';
		strcpy(s+1, s+2);
		break;
	case '|':
	case '@':
		s[2] = (s[2]=='|')?'!':'$';
	case '#':
		s[0] = s[2];
		s[1] = s[2] = '\t';
		// LTrim the string s
		if (s[3] == ' ') {
			j = i = 3;
			while (s[j] == ' ') j++;
			while (s[j-1] != '\0') s[i++] = s[j++];
		}
		break;
	default:
		i=2;
		while (s[i] != '\0' && s[i] == ' ') i++;
		j = (i-2+7)/8;
		strcpy(s+j, s+i);
		for (i=0; i<j; i++) s[i] = '\t';
	}

	strcat(s, "\r\n");
	j = i = 0; k = strlen(s)-1;	// not contains '\n'
	for (i=0; i<k-1; i++) {
		if (s[i]>0 && s[i] < 7) {
			vcolor = RGB(0, 230-30*s[i], 230-30*s[i]);
			s[i] = ' ';
			j = i+1;
		}
		else
			if (s[i] == '~') {
				s[i] = ' ';
				if (j==0) continue;
				myView->AddCF(sF, eF, cSpos+j, cSpos+i, vcolor);
				fNo++;
				j = 0;
			}
	}
	cSpos += k;
}

// write out all formats
void CConvert::WriteAllEnd(CFile* fs, WORD fNo, pCF sF, pCF eF)
{
	WORD tmp = 0;
	fs->SeekToEnd();
	fs->Write(&tmp, sizeof(WORD));	// Bold
	fs->Write(&tmp, sizeof(WORD));	// Italic
	fs->Write(&fNo, sizeof(WORD));	// Underline
	WriteOutCf(fs, sF, eF, FALSE);
	fs->Write(&tmp, sizeof(WORD));	// Strike
	fs->Write(&tmp, sizeof(WORD));	// SYMBOL
	fs->Write(&fNo, sizeof(WORD));	// Color
	WriteOutCf(fs, sF, eF, TRUE);
	fs->Write(&tmp, sizeof(WORD));	// Subscript
	fs->Write(&tmp, sizeof(WORD));	// Superscript
	fs->Write(&tmp, sizeof(WORD));	// Objects
}

void CConvert::WriteTextAll(CFile* fs, pSubAns& p)
{
	pSubAns p1;
	while (p) {
		p1 = p; p = p->next;
		if (fs) fs->Write(p1->info, strlen(p1->info));
		delete p1->info;
		delete p1;
	}
}
