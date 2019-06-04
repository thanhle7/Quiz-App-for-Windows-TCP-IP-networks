// BldTest.cpp : implementation file
//

#include "stdafx.h"
#include "BldTest.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CBldTest dialog

CBldTest::CBldTest(CEmpView* pParent)
	: CDialog(CBldTest::IDD, pParent)
{
	myView = pParent;
	flNo = totalselect = 0;
	m_filelist.Setparameters(myView, &flNo, fa, &totalselect, IDC_BUILDTOTAL);
	//{{AFX_DATA_INIT(CBldTest)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
}


void CBldTest::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CBldTest)
	DDX_Control(pDX, IDC_BUILDLIST, m_filelist);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CBldTest, CDialog)
	//{{AFX_MSG_MAP(CBldTest)
	ON_BN_CLICKED(IDC_BUILDSAVE, OnBuildsave)
	ON_BN_CLICKED(IDC_BUILDSELE, OnBuildsele)
	ON_BN_CLICKED(IDC_TOFILE, OnSetHowWork)
	ON_EN_KILLFOCUS(IDC_BUILDITEM, OnKillfocusBuilditem)
	ON_BN_CLICKED(IDC_PREVIEW, OnSetHowWork)
	ON_BN_CLICKED(IDC_TOPRINTER, OnSetHowWork)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CBldTest message handlers

void CBldTest::OnBuildsave() 
{
	CEmpView* pr = (CEmpView*)GetParent();
	pr->GetFileDlgCtrl(this, IDC_BUILDFILE, TRUE, FALSE, 
						OFN_CREATEPROMPT|OFN_OVERWRITEPROMPT);
}

BOOL CBldTest::OnInitDialog()
{
	CSpinButtonCtrl* spin;

	if (!CDialog::OnInitDialog())
		return FALSE;
	if (!myView->ClearCurrentDoc(TRUE)) {
		EndDialog(IDCANCEL);
		return FALSE;
	}

	SetDlgItemText(IDC_BUILDFILE, "Emp1");
	SetDlgItemText(IDC_BUILDTITLE, "®‹ Thi Tréc Nghi¬m M“n Tin H”c --+-- Lôp: .....\r\n"
								   "Thi ng…y: 26/10/2002 --+-- Thõi gian: %d ph£t\r\n"
								   "®‹ S–: %d");
	SetDlgItemInt(IDC_BUILDITEM, 10);
	SetDlgItemInt(IDC_TIMEDONE, 25);
	spin = (CSpinButtonCtrl*)GetDlgItem(IDC_BUILDSPIN2);
	spin->SetRange(1,50);
	spin = (CSpinButtonCtrl*)GetDlgItem(IDC_TIMESPIN);
	spin->SetRange(1,180);
	prog = (CProgressCtrl*)GetDlgItem(IDC_BUILDPROG);
	prog->SetRange(1,100);
	CheckRadioButton(IDC_TOFILE,IDC_TOPRINTER,IDC_TOFILE);
	CheckDlgButton(IDC_KEEPFILEORDER, BST_CHECKED);
	howWork = 0;

	// randomize
	srand((unsigned) time(NULL));
	return TRUE;
}

void CBldTest::OnOK()
{
	WORD	sonhom;
	WORD	*socaunhom, *socauchon;		// so cau nhom
	DWORD	*vtdaunhom;					// vi tri dau nhom
	char	*mucdonhom;
	int		*socau_muc;					// so cau chon moi muc; su dung gia tri (-) va (+)
	int		i, j, muc, nhom, SeleQsts;
	int		firstQstGrpIdx;				// chi so nhom cau hoi dau tien trong 1 tap tin de thi
	int		*GroupQstIdx;				// randomize
	int		hienco = 0;					// progress bar
	char	*s, *s2, password[21], password2[21];
	CFile	fin, fout;
	long	Spos;
	pCF		sf[NoCF], ef[NoCF];
	WORD	Item, Items, sopt;
	WORD	oSodong,oSoObj,oSoCF[NoCF],tmp;
	DWORD	qSize, *ItemLoca;
	pSubAns	*pans, ptmpans;
	int		*subSize, *newsubPos, sSub, subNo;
	BOOL	notSub, ToEmp1996;
	CEdit*	iTitle;

	pFileListItem	*pFL;
	CArchive		*ain, *atmp;
	CFileException	e;
	unsigned char	level, *levelSet, *plevelSet;

	Items = GetDlgItemInt(IDC_BUILDITEM);
	sopt  = GetDlgItemInt(IDC_BUILDTOTAL);
	if (flNo < 2 || Items < 1 || sopt < 1) {
		MessageBox(	"Please set enough configuration infos\n"
					"before going on with Test Building.", "Error", 
					MB_OK|MB_ICONINFORMATION);
		return;
	}

	// get password information

	prog->SetPos(0);

	s = new char[PARALEN+1];
	ToEmp1996 = IsDlgButtonChecked(IDC_BUILDDOSVER);
	iTitle = (CEdit*) GetDlgItem(IDC_BUILDTITLE);
	if (!howWork) {
		// prepare output data file
		GetDlgItemText(IDC_BUILDFILE, s, PARALEN);
		i=0;
		while (s[i] && s[i]!='.') i++;
		if (s[i] == '\0') strcat(s, ".zmp");
		if (!fout.Open(s, CFile::modeCreate | CFile::modeWrite, &e)) {
			delete s;
			e.ReportError(MB_OK|MB_ICONSTOP);
			return;
		}

		ItemLoca = new DWORD[Items];
		if (GetDlgItemText(IDC_BUILDMTLE, s, MENU_TITLE_LEN-1) <= 0)
			strcpy(s, "Menu titled by Editor");
		else
			s[MENU_TITLE_LEN-1] = '\0';
		tmp =  GetDlgItemInt(IDC_TIMEDONE);	// time in minutes
		tmp += 256*tmp;						// normal one
		if (!ToEmp1996) {
			fout.Write(EmpID, EmpIDL);
			fout.Write(s, MENU_TITLE_LEN);
			fout.Write(&tmp, sizeof(WORD));		// reserved WORD
			fout.Write(&Items, sizeof(WORD));
			fout.Write(ItemLoca, Items*sizeof(DWORD));
		}
	}

	levelSet = new unsigned char[totalselect];

	for (Item=1; Item <= Items; Item++) {
		// change the exercise structure
		pFL = new pFileListItem[flNo];
		if (IsDlgButtonChecked(IDC_KEEPFILEORDER))
			memcpy(pFL, fa, flNo * sizeof(pFileListItem));
		else {
			// change order
			muc = sonhom = 0;
			GroupQstIdx = new int[flNo];
			while (muc < flNo) {
				i = rand() % flNo;
				while (fa[i]->items == 0) i--;		// a group header
				j = i-1;
				while (j >= 0) {
					// search for another main-info group header (if exist) 
					if (fa[j]->items > 0 && fa[j]->socauchon == 0) {
						if (!NotIn(j, sonhom, GroupQstIdx)) {
							// that one has already come into pFL
							nhom = j+1;
							j    = -1;	// maybe OK	
							if (fa[i]->socauchon == 0) {
								// also another main-info group header
								while (nhom < i) {
									if (fa[nhom]->items > 0 && NotIn(nhom, sonhom, GroupQstIdx))
										break;
									nhom++;
								}
								if (nhom < i) j = 1;	// ofcourse not OK
							}
						}
						break;
					}
					j--;
				}
				if (j < 0 && NotIn(i, sonhom, GroupQstIdx)) {
					for (j=0; j < fa[i]->items+1; j++) {
						pFL[muc] = fa[i+j];
						muc++;
					}
					GroupQstIdx[sonhom] = i;
					sonhom++;
				}
			}
			delete GroupQstIdx;
		}

		// set location map of items
		if (!howWork) 
			ItemLoca[Item-1] = (DWORD)fout.GetPosition();

		atmp = new CArchive(&fout, CArchive::store);// | CArchive::bNoFlushOnDelete);
		oSodong = iTitle->GetLineCount() + 1;
		iTitle->GetWindowText(s, PARALEN);
		sopt = GetDlgItemInt(IDC_TIMEDONE);
		s2 = new char[PARALEN+1];
		strcat(s, "\r\n\r\n");	// (OnOK1) : two pairs add to two exist pairs of '\r\n'
		sprintf(s2, s, sopt, Item);
		strcpy(s, s2); delete s2;

		oSoObj = 0;
		for (j=0; j<NoCF; j++) {
			oSoCF[j] = 0;
			sf[j] = ef[j] = NULL;
		}
		*atmp << totalselect;	// number of question
		*atmp << oSodong;		// text line
		atmp->Write(levelSet, totalselect);

		Spos = strlen(s) - 4;	// (OnOK1): two of '\n'
		atmp->WriteString(s);

		// extract questions from each group file
		muc = 0;	// the first item in file list is a group header
		plevelSet = levelSet;
		while (muc < flNo) {
			// muc is now standing at begin of a group, then select one item.
			tmp = muc + 1 + ((Item-1) % pFL[muc]->items);
			SeleQsts = pFL[muc]->socauchon;

			if (!fin.Open(pFL[tmp]->title, CFile::modeRead)) {
				totalselect -= SeleQsts;
				continue;
			}

			ain = new CArchive(&fin, CArchive::load);
			fin.Read(s, EmpIDL);

			// get header
			*ain >> level;		// not used from ver 2.3
			*ain >> sopt;		// number of question in this file, no need here

			//------------------
			*ain >> sopt;		// is having the main info ?  then Hi is Version number
			//------------------
			// firstQstGrpIdx > 0 means that this document has main-info
			firstQstGrpIdx = (LOBYTE(sopt) > 0) ? 1 : 0;	
			*ain >> sonhom;						// number of qst group in this file
			nhom = sonhom + firstQstGrpIdx;		// include main-info
			socaunhom = new WORD[nhom];
			socauchon = new WORD[nhom];			// number of selected qsts in each group 
			mucdonhom = new char[nhom];
			vtdaunhom = new DWORD[nhom];
			socau_muc = new int[TOPLEVEL];
			for (i=0; i<TOPLEVEL; i++)
				socau_muc[i] = pFL[muc]->caumucchon[i];

			ain->Read(socaunhom + firstQstGrpIdx, sonhom * WORD_S);		// Questions per group
			ain->Read(mucdonhom + firstQstGrpIdx, sonhom * char_S);		// level of each group
			ain->Read(vtdaunhom + firstQstGrpIdx, sonhom * DWORD_S);	// group positions
			ain->Flush();
			if (firstQstGrpIdx) {
				socaunhom[0] = 1;
				socauchon[0] = 1;
				vtdaunhom[0] = (DWORD) fin.GetPosition();
			}

			// arrange a random order on questions groups
			if (sonhom > 0) {
				// the item must have questions
				// main-info, where sonhom=0, alway has 'SeleQsts=0', not come here
				subNo = 30;
				while (sonhom > 1 && subNo-- > 0) {
					i = firstQstGrpIdx + (rand() % sonhom);
					j = firstQstGrpIdx + (rand() % sonhom);
					if (i != j) {
						tmp			= socaunhom[i];
						qSize		= vtdaunhom[i];
						password2[0]= mucdonhom[i];
						socaunhom[i]= socaunhom[j];
						vtdaunhom[i]= vtdaunhom[j];
						mucdonhom[i]= mucdonhom[j];
						socaunhom[j]= tmp;
						vtdaunhom[j]= qSize;
						mucdonhom[j]= password2[0];
					}
				}

				// SeleQsts : Number of selected qsts from this file
				for (i=firstQstGrpIdx; i<nhom; i++) {
					// select 1 question from the i(th) group
					socauchon[i] = 1;
					// the requirement on the correspondant level must be
					// decreased by 1; this may a negative value by exceeding
					socau_muc[mucdonhom[i]] -= 1;
				}

				// justify to valid selected number of qsts in each qsts group
				while (1) {
					// loop until all items in socau_muc[] are equal to 0
					for (i=0; i<TOPLEVEL; i++) {
						if (socau_muc[i] != 0) break;
					}
					if (i >= TOPLEVEL) break;

					i = firstQstGrpIdx + (rand() % sonhom);
					// 1: not enough case
					if (socau_muc[mucdonhom[i]] > 0 && socaunhom[i] > socauchon[i]) {
						socau_muc[mucdonhom[i]]--;
						socauchon[i]++;
					}
					// 2: exceeded case
					if (socau_muc[mucdonhom[i]] < 0 && socauchon[i] > 0) {
						socau_muc[mucdonhom[i]]++;
						socauchon[i]--;
					}
				}
			}

			sonhom += firstQstGrpIdx;	// if firstQstGrpIdx > 0 : includes main-info

			for (nhom=0; nhom<sonhom; nhom++) {
				if (socauchon[nhom] == 0) continue;	// no selected questions
				if (nhom >= firstQstGrpIdx) {
					// current group is a question group, not a main-info; 
					// set level for its quetions that participate into exercises
					memset(plevelSet, mucdonhom[nhom], socauchon[nhom]); 
					plevelSet += socauchon[nhom];
				}

				// seek to begin of group
				ain->GetFile()->Seek(vtdaunhom[nhom], CFile::begin);

				// tao thu tu ngau nhien tu cac cau trong nhom
				GroupQstIdx = new int[socauchon[nhom]];
				// select question at random from this group
				i=0;
				while (i < socauchon[nhom]) {
					GroupQstIdx[i] = rand() % socaunhom[nhom];
					if (NotIn(GroupQstIdx[i], i, GroupQstIdx)) i++;
				}

				// scan all 'socauchon[nhom]' questions in this group
				for (i=0; i < socaunhom[nhom]; i++) {
					*ain >> qSize;
					ain->Flush();
					if (NotIn(i, socauchon[nhom], GroupQstIdx)) {
						// a not selected question, skip...
						// fin.Seek(fin.GetPosition()+qSize, CFile::begin);
						fin.Seek(qSize, CFile::current);
					}
					else {
						// one accept question
						*ain >> sopt;	// so dong
						*ain >> qSize;	// so ky tu

						oSodong += sopt;
						pans = new pSubAns[NOANSWER];
						subSize = new int[NOANSWER];
						newsubPos = new int[NOANSWER];
						for (j=0; j<NOANSWER; j++) {
							pans[j] = NULL;
							subSize[j] = 0;
							newsubPos[j] = j;
						}
						subNo = sSub = 0; notSub = TRUE;
						while (sopt--) {
							ain->ReadString(s, PARALEN);
							strcat(s, "\r\n");
							if (isSub(s)) {
								notSub = FALSE;
								if (s[0] != '!' && subNo < NOANSWER-1) subNo += 1;
							}
							else {
								if (s[0] == '*') {
									// set question mark to "***/ " for match to TEST.EXE
									s[1] = s[2] = '*';
								}
								if (!notSub && (subNo ==0 || isSub(pans[subNo-1]->info)))
									subNo += 1;
							}
							if (notSub) {
								sSub += strlen(s) - 1;			// start pos of sun answers
								atmp->WriteString(s);
							}
							else {
								subSize[subNo-1] += (strlen(s) - 1);	// sub answer size
								AddSubLine(pans[subNo-1], s);
							}
						}
						if (subNo > 0) {
							// subNo == number of sub answer in this question
							if (isSub(pans[0]->info)) {
								// get sub answers and resort them now.
								for (tmp = 0; tmp < 15; tmp++) {
									j = rand() % subNo;
									if (j && isSub(pans[j]->info)) {
										// change two elements: 0 -- j
										sopt = newsubPos[0];
										newsubPos[0] = newsubPos[j];
										newsubPos[j] = sopt;
										// newsubPos[i] cho biet trong thu tu moi,
										// cau o vi tri i la cau nao trong cac cau
										// tu 0 den j-1
										ptmpans = pans[j];
										pans[j] = pans[0];
										pans[0] = ptmpans;
									}
								}
							}
							// write out sub answers of this question
							for (j = 0; j < subNo; j++) {
								while (pans[j] != NULL) {
									ptmpans = pans[j];
									pans[j] = pans[j]->next;
									atmp->WriteString(ptmpans->info);
									delete ptmpans->info;
									delete ptmpans;
								}
							}
						}
						else sSub = qSize+1;	// this must be the main info of topic
						delete pans;

						// format
						for (j=0; j<NoCF; j++) {
							*ain >> sopt;	// so format
							oSoCF[j] += sopt;
							while (sopt--) {
								myView->AddCF(sf[j], ef[j], 0, 0, 0);
								if (CFV[j] == CFM_COLOR)
									*ain >> ef[j]->val;
								*ain >>	tmp;
								ef[j]->p1 = Spos + CorrectPos(subSize, newsubPos, subNo, tmp, sSub);
								*ain >>	tmp;
								ef[j]->p2 = Spos + CorrectPos(subSize, newsubPos, subNo, tmp, sSub);
							}
						}

						delete subSize;
						delete newsubPos;
						ain->Flush();

						Spos = Spos + qSize;

						hienco += 1;
						prog->SetPos(hienco*100/totalselect*Items);
					}
				} // end of : for (i=0; ... ; i++) {
				delete GroupQstIdx;
			}
			// Spos is now pointing to the end char of last question
			// Spos++;	// point to the first char of next question
			ain->Close(); fin.Close();
			delete ain;
			// delete temporel memory
			delete socaunhom;
			delete socauchon;
			delete vtdaunhom;
			delete mucdonhom;
			delete socau_muc;

			muc += (pFL[muc]->items + 1);
		}

		// write format
		for (j=0; j<NoCF; j++) {
			*atmp << oSoCF[j];
			while (sf[j] != NULL) {
				ef[j] = sf[j]; sf[j] = sf[j]->next;
				if (CFV[j] == CFM_COLOR) *atmp << ef[j]->val;
				*atmp << ef[j]->p1;
				*atmp << ef[j]->p2;
				free(ef[j]);
			}
		}

		atmp->Flush(); atmp->Close(); delete atmp;

		// free temporelle document
		myView->ClearCurrentDoc(FALSE);

		fout.Seek(ItemLoca[Item-1], CFile::begin);
		fout.Write(&totalselect, sizeof(WORD));		// number of question
		fout.Write(&oSodong, sizeof(WORD));			// number of text
		fout.Write(levelSet, totalselect);			// races of questions
		fout.Seek(ItemLoca[Item-1], CFile::begin);

		delete pFL;

		// now, fout ready to be a good data test for TEST.exe
		if (howWork) {
			// print to paper
			atmp = new CArchive(&fout, CArchive::load);
			myView->DoPrintTest(*atmp);
			atmp->Close(); delete atmp;

			if (howWork == 2) {
				myView->SendMessage(WM_COMMAND, ID_FILE_PRINT, 0);
				UpdateWindow();
				myView->ClearCurrentDoc(FALSE);
			}
			else {
				PostMessage(WM_COMMAND, IDCANCEL, 0);
				break;
			}
		}
		else {
			if (ToEmp1996) {
				// convert info of new format to old format
				atmp = new CArchive(&fout, CArchive::load);

				*atmp >> sopt;	// number of questions
				*atmp >> sopt;	// number of line
				atmp->Read(levelSet, totalselect);

				notSub = TRUE;
				strcpy(s,	"\xDB\r\n"
							";  This was done by the new version of LNTsoft. Some special formats in\r\n"
							";  Windows(tm), for this reason, will be lost by the LNT.1996 version.\r\n");
				if (strcmp(password, "emp")) s[4] = '-'; // password mark
				fout.Write(s, strlen(s));

				// read Text
				for (i=0; i<sopt; i++) {
					atmp->ReadString(s, PARALEN);

					switch (s[0]) {
					case '\0':
					case '*':
						notSub = FALSE;
						strcpy(s, s+1);
						break;
					case '$':
					case '!':
						s[0] = (s[0]=='$')?'@':'|';
					case '#':
						strcpy(s+1, s+2);
						s[1] = ' ';
					}
					if (notSub && strlen(s) < 76) {
						s2 = new char[80];
						memset(s2, 32, 80);
						strcpy(s2+ (76-strlen(s))/2, s);
						strcpy(s, s2);
						delete s2;
					}
					for (tmp=0; tmp < strlen(s); tmp++) {
						if (s[tmp] == '\t') s[tmp] = ' ';
					}
					strcat(s, "\r\n");
					fout.Write(s, strlen(s));
				}
				atmp->Close(); delete atmp;
			}
		}
		fout.Seek(0, CFile::end);
	}

	if (!howWork) {
		if (ToEmp1996) {
			strcpy(s, "\xDB\r\n");
			fout.Write(s, strlen(s));
		}
		else {
			fout.Seek(EmpIDL+MENU_TITLE_LEN+2*sizeof(WORD), CFile::begin);
			fout.Write(ItemLoca, Items*sizeof(DWORD));
		}
		fout.Close();
		delete ItemLoca;
	}
	delete levelSet;
	delete s;
	prog->SetPos(0);
}

BOOL CBldTest::NotIn(int val, int bfl, int *bf)
{
	int	j=0;
	while (j<bfl && *(bf+j) != val) j++;
	return (j >= bfl);
}

void CBldTest::OnBuildsele() 
{
	int		startPos, curPos;
	CString fn;

	CFileDialog* f = new CFileDialog(TRUE);
	f->m_ofn.Flags |= OFN_ALLOWMULTISELECT|OFN_FILEMUSTEXIST|OFN_HIDEREADONLY;
	f->m_ofn.lpstrFilter = "Editor Document (*.emp)\0 *.emp\0 All Files (*.*)\0 *.*";
	f->m_ofn.lpstrTitle = "Select one or more Editor Document(s)";
	if (f->DoModal() != IDOK) {
		delete f;
		return;
	}

	POSITION pos = f->GetStartPosition();
	if (pos)
		startPos = m_filelist.AddPropItem(-1);
	while (pos != NULL)
	{
		curPos = m_filelist.AddPropItem(0);
		fn = f->GetNextPathName(pos);
		if (!m_filelist.UpdateItemFromFile(curPos, fn)) {
			m_filelist.DelPropItem(curPos);
			curPos--;
		}
	}
	if (curPos == startPos)
		m_filelist.DelPropItem(startPos);
	else
		m_filelist.UpdateHeaderFromItems(startPos, -1);
}

void CBldTest::AddSubLine(pSubAns &ps, char *line)
{
	pSubAns tmp;
	if (ps == NULL)
		ps = tmp = new SubAns;
	else {
		tmp = ps;
		while (tmp->next != NULL)
			tmp = tmp->next;
		tmp->next = new SubAns;
		tmp = tmp->next;
	}
	tmp->info = new char[strlen(line)+1];
	strcpy(tmp->info, line);
	tmp->next = NULL;
}

BOOL CBldTest::isSub(char *s)
{
	return (strncmp(s, "$\t\t", 3)==0 ||
			strncmp(s, "#\t\t", 3)==0 ||
			strncmp(s, "!\t\t", 3)==0);
}

void CBldTest::OnSetHowWork() 
{
	static int oldselItem = 10;
	int oldwork = howWork;
	howWork = GetCheckedRadioButton(IDC_TOFILE,IDC_TOPRINTER)-IDC_TOFILE;
	if (oldwork != howWork) {
		GetDlgItem(IDC_BUILDFILE)->EnableWindow(howWork==0);
		GetDlgItem(IDC_BUILDDOSVER)->EnableWindow(howWork==0);
		GetDlgItem(IDC_BUILDSAVE)->EnableWindow(howWork==0);
		GetDlgItem(IDC_BUILDMTLE)->EnableWindow(howWork==0);
		GetDlgItem(IDC_PASSWORD)->EnableWindow(howWork==0);
		GetDlgItem(IDC_PASSWORD2)->EnableWindow(howWork==0);
		if (howWork == 1) {
			oldselItem = GetDlgItemInt(IDC_BUILDITEM);
			SetDlgItemInt(IDC_BUILDITEM, 1);
		}
		else SetDlgItemInt(IDC_BUILDITEM, oldselItem);
	}
}

void CBldTest::OnKillfocusBuilditem() 
{
	if (howWork == 1)
		SetDlgItemInt(IDC_BUILDITEM, 1);
}

// correct position for some format in sun answers
// two array info and its lens, current pos, start position of sub answers
int CBldTest::CorrectPos(int *ssize, int *snpos, int len, int cpos, int sspos)
{
	int ci, cj, ck;

	cj = cpos - sspos;
	if (cj < 0) return cpos;
	ci = 0;
	while (cj > *(ssize + ci)) { 
		cj -= *(ssize + ci);
		ci++;
	}
	// ci is the sub answer convenient
	// then find where it is now
	ck = 0;
	while (*(snpos + ck) != ci) ck++;
	while (ck) {
		ck -= 1;
		cj += *( ssize + (*(snpos+ck)) );
	}
	return cj + sspos;
}

CBldTest::~CBldTest()
{
	for (int i=0; i<flNo; i++)
		delete fa[i];
}
