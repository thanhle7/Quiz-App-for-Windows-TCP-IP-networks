// EmpFrame.cpp : implementation of the CMainFrame class
//

#include "stdafx.h"
#include "Emp.h"
#include "empframe.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CMainFrame

IMPLEMENT_DYNCREATE(CMainFrame, CFrameWnd)

BEGIN_MESSAGE_MAP(CMainFrame, CFrameWnd)
	//{{AFX_MSG_MAP(CMainFrame)
	ON_WM_CREATE()
	ON_WM_CLOSE()
	ON_WM_TIMER()
	ON_COMMAND(ID_SAVE_ITEM, SaveItem)
	ON_COMMAND(ID_TEST, TestWnd)
	ON_COMMAND(ID_QUESTION, QuestionWnd)
	ON_COMMAND(ID_ANSWER, AnswerView)
	ON_COMMAND(ID_AUTORUN, AutoRun)
	ON_COMMAND(ID_STUDENT, NetStudentInfo)
	ON_COMMAND(ID_QUIT, DoQuit)
	ON_COMMAND(ID_EXIT, OnClose)
	ON_COMMAND(ID_HELP, CFrameWnd::OnHelpFinder)
	//}}AFX_MSG_MAP
	ON_COMMAND(ID_OPEN_ITEM, OnOpenItem)
	ON_WM_ACTIVATE()
END_MESSAGE_MAP()

static UINT indicators[] =
{
	ID_SEPARATOR,
	ID_INDICATOR_LOP,
	ID_INDICATOR_SOBD,
	ID_INDICATOR_HOTEN,
	ID_INDICATOR_NGAYSINH,
	ID_INDICATOR_NETTIME
};

/////////////////////////////////////////////////////////////////////////////
// CMainFrame construction/destruction

CMainFrame::CMainFrame()
{
	m_StdIdChk  = FALSE;
	m_wndObjView= NULL;
	zmpNo		= 0;
	curTopic	= -1;
	isAnsView	= FALSE;
	IsShowResult= FALSE;
	cPaneId		= ID_QUESTION;	// Question Pane
	ocDlg		= NULL;
	levelSet	= NULL;
	SockPort	= UDP_PORT;
	curJob		= EMP_CONNECT;
	tAddrSize	= sizeof(SOCKADDR_IN);
	minute = second = AnsBufL = 0;
	ClearStudentInfo();

	// Get windows version
	OSVERSIONINFOEX osvi;
	ZeroMemory(&osvi, sizeof(OSVERSIONINFOEX));
	osvi.dwOSVersionInfoSize = sizeof(OSVERSIONINFOEX);
	m_verCode = 3;
	m_verMajor = m_verMinor = 0;

	if ( !GetVersionEx ((OSVERSIONINFO *) &osvi) ) {
		osvi.dwOSVersionInfoSize = sizeof (OSVERSIONINFO);
		if (! GetVersionEx ( (OSVERSIONINFO *) &osvi) ) 
			return;
	}

/*
0 = "95  ";
1 = "98  ";
2 = "Me  ";
3 = "NT  ";
4 = "2000";
5 = "XP  ";
6 = ".NET";
*/
	switch (osvi.dwPlatformId)
	{
		// Tests for Windows NT product family.
		case VER_PLATFORM_WIN32_NT:
			// Test for the product.
			if ( osvi.dwMajorVersion <= 4 ) {
				m_verCode = 3;
				break;
			}
			if ( osvi.dwMajorVersion > 5 ) {
				// not exist yet
				m_verCode = 5;
				break;
			}
			if (osvi.dwMinorVersion == 0 )
				m_verCode = 4;

			// Test for the workstation type.
			if ( osvi.wProductType == VER_NT_WORKSTATION ) {
				if (osvi.dwMinorVersion == 1)
					m_verCode = 5; 
			}
			// Test for the server type.
			else 
				if ( osvi.dwMinorVersion == 2 )
					m_verCode = 6; 
			break;

			// Test for the Windows 95 product family.
		case VER_PLATFORM_WIN32_WINDOWS:
			if (osvi.dwMajorVersion == 4 && osvi.dwMinorVersion == 0)
				m_verCode = 0;
			else
            if (osvi.dwMajorVersion == 4 && osvi.dwMinorVersion == 10)
				m_verCode = 1;
			else
				m_verCode = 2;
			break;
	}
	m_verMajor = osvi.dwMajorVersion;
	m_verMinor = osvi.dwMinorVersion;
}

CMainFrame::~CMainFrame()
{
	if (AnsBufL > 0) delete AnsBuf;
	if (levelSet) delete levelSet;
	while (zmpNo) {
		zmpNo--;
		delete zmpName[zmpNo].FileName;
		delete zmpName[zmpNo].MenuTitle;
	}
	if (m_wndObjView != NULL)
		delete m_wndObjView;
}

int CMainFrame::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CFrameWnd::OnCreate(lpCreateStruct) == -1)
		return -1;
	
	if (!m_wndToolBar.CreateEx(this, TBSTYLE_FLAT, WS_CHILD | WS_VISIBLE | CBRS_TOP
		| CBRS_GRIPPER | CBRS_TOOLTIPS | CBRS_FLYBY | CBRS_SIZE_DYNAMIC) ||
		!m_wndToolBar.LoadToolBar(IDR_MAINFRAME) ||
		!m_wndStatusBar.CreateEx(this))
	{
		TRACE0("Framework Initiation Failed !\n");
		return -1;
	}

	LOGFONT lf;
	memset(&lf, 0, sizeof(LOGFONT));
	strcpy(lf.lfFaceName, "emp.font");
	lf.lfHeight = 16;
	lf.lfWidth = 12;
	m_vnFont.CreateFontIndirect(&lf);

	CStatusBarCtrl& bc = m_wndStatusBar.GetStatusBarCtrl();
	bc.SetFont(&m_vnFont);
	m_wndStatusBar.SetIndicators(indicators, 6);
	bc.SetMinHeight(25);

	GetNextIniInfo();

	if (TestType > 0) {
		EnvironmentSetting(0);
		EnvironmentSetting(1);
	}

	return 0;
}

BOOL CMainFrame::PreCreateWindow(CREATESTRUCT& cs)
{
	// justify window
	cs.x = cs.y = 0;

	if( !CFrameWnd::PreCreateWindow(cs) )
		return FALSE;
	cs.style = WS_OVERLAPPED | WS_CAPTION | FWS_ADDTOTITLE
				| WS_THICKFRAME | WS_SYSMENU | WS_MINIMIZEBOX | WS_MAXIMIZEBOX | WS_MAXIMIZE;
	cs.lpszClass = ((CEmpApp *)AfxGetApp())->sinst.GetClassName();

	// how to work
	GetExeDir();
	strcat(fName, "TEST.INI");
	TestType = GetPrivateProfileInt("EMP_OPTION", "TEST", 0, fName);

	if (TestType) {
		cs.style ^= WS_MINIMIZEBOX;
		//cs.dwExStyle |= WS_EX_TOPMOST;
	}

	return TRUE;
}

/////////////////////////////////////////////////////////////////////////////
// CMainFrame message handlers

BOOL CMainFrame::OnCreateClient(LPCREATESTRUCT lpcs, CCreateContext* pContext) 
{
	ShowWindow(SW_HIDE);

	// 1 row, 2 columns
	if ( !m_split.CreateStatic(this, 1, 2, WS_CHILD|WS_VISIBLE))
		return FALSE;
	m_split.CreateView(0,0,RUNTIME_CLASS(CEmpQstsView),CSize(100,0), pContext);
	m_split.CreateView(0,1,pContext->m_pNewViewClass,CSize(0,0), pContext);
	pans = (CEmpQstsView*) m_split.GetPane(0,0);
	ptest= (CEmpView*) m_split.GetPane(0,1);

	ShowWindow(SW_SHOWMAXIMIZED);

	return TRUE;
}

BOOL CMainFrame::OnCommand(WPARAM wParam, LPARAM lParam) 
{
	int nId = LOWORD(wParam);
	if (nId < ID_SAVE_ITEM && nId > ID_SAVE_ITEM-200) {
		NewTopic(nId, ID_SAVE_ITEM);
		return TRUE;
	}
	return CFrameWnd::OnCommand(wParam, lParam);
}

void CMainFrame::GetZmpFile()
{
	WIN32_FIND_DATA	mff;
	HANDLE			hfile;
	CMenu*			hmenu;

	fName[fNameLen] = '\0'; strcat(fName, "*.zmp");
	hmenu = GetMenu();
	hfile = FindFirstFile(fName, &mff);
	if (hfile != INVALID_HANDLE_VALUE) {
		do {
			if (!(mff.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY)) {
				fName[fNameLen] = '\0'; strcat(fName, mff.cFileName);
				if (AddZmpFile())
					hmenu->InsertMenu(ID_SAVE_ITEM, MF_BYCOMMAND|MF_ENABLED,
						ID_SAVE_ITEM-zmpNo, zmpName[zmpNo-1].MenuTitle);
			}
		} while (zmpNo <= 25 && FindNextFile(hfile, &mff));
	}
	if (zmpNo)
		hmenu->InsertMenu(ID_SAVE_ITEM, MF_SEPARATOR|MF_BYCOMMAND, 0, "");
}

void CMainFrame::NewTopic(int Id, int BaseMenuId)
{
	CMenu* hmenu = GetMenu();
	if (curTopic > -1)
		hmenu->CheckMenuItem(BaseMenuId-curTopic-1,MF_UNCHECKED);
	curTopic = BaseMenuId-Id-1;
	hmenu->CheckMenuItem(BaseMenuId-curTopic-1,MF_CHECKED);
	m_dethi = 0;

	GetActiveDocument()->SetTitle(zmpName[curTopic].MenuTitle);
}

void CMainFrame::SaveItem()
{
}

BOOL CMainFrame::OnCmdMsg(UINT nID, int nCode, void* pExtra, AFX_CMDHANDLERINFO* pHandlerInfo) 
{
	if (TestType) {
		if ((nID>=ID_SAVE_ITEM && nID<ID_CLOCK) || nID==ID_HELP) 
			return FALSE;
	}
	else
	 if (nID < ID_SAVE_ITEM && nID > ID_SAVE_ITEM-200) return TRUE;
	return CFrameWnd::OnCmdMsg(nID, nCode, pExtra, pHandlerInfo);
}

void CMainFrame::OnClose() 
{
	if (TestType > 0 || (zmpNo>0 && ptest->qstNo > 0 &&
			MessageBox("Would You like to turn off\n\n the Test Program ?", "Exit", MB_YESNO|MB_ICONQUESTION) == IDNO))
		return;

	GetActiveDocument()->SetModifiedFlag(FALSE);
	KillTimer(ID_CLOCK);

	if (TestType < 0) {

		EnvironmentSetting(2);

		if (TestType == -5) {
			UINT exitFlag = EWX_POWEROFF|EWX_FORCE;
			if (!ExitWindowsEx(exitFlag, 0)) {
				HANDLE hToken; 
				TOKEN_PRIVILEGES tkp; 
		 
				// Get a token for this process. 

				while (!OpenProcessToken(GetCurrentProcess(), 
						TOKEN_ADJUST_PRIVILEGES | TOKEN_QUERY, &hToken ))
					Sleep(100);
		 
				// Get the LUID for the shutdown privilege. 
	 			LookupPrivilegeValue(NULL, SE_SHUTDOWN_NAME, 
					&tkp.Privileges[0].Luid); 
				tkp.PrivilegeCount = 1;  // one privilege to set    
				tkp.Privileges[0].Attributes = SE_PRIVILEGE_ENABLED; 
		 
				// Get the shutdown privilege for this process. 
				AdjustTokenPrivileges(hToken, FALSE, &tkp, 0, 
					(PTOKEN_PRIVILEGES)NULL, 0); 

				// Cannot test the return value of AdjustTokenPrivileges. 
				if (GetLastError() != ERROR_SUCCESS)
					exitFlag = EWX_LOGOFF;

				// Shut down the system and force all applications to close. 
				ExitWindowsEx(exitFlag, 0);
			}
		}
	}

	CFrameWnd::OnClose();
}

void CMainFrame::TestWnd()
{
	GotoPane(ID_TEST, TRUE);
}

void CMainFrame::QuestionWnd()
{
	GotoPane(ID_QUESTION, TRUE);
}

void CMainFrame::OnTimer(UINT nIDEvent) 
{
	char s[9];
	switch (nIDEvent) {
	case ID_CLOCK:
		if (m_iIcon) {
			if (m_iIcon & 4) m_iIcon = 0;
			m_wndStatusBar.GetStatusBarCtrl().SetIcon(5,m_hIcon[m_iIcon]);
			if (m_iIcon) m_iIcon = 4;
		}

		if (isAnsView || (minute + second == 0)) 
			return;
		if (second > -1) {
			second--;
			if (second < 0) {
				second = 59;
				minute--;
			}
		}
		sprintf(s, "  %2d:%02d", minute, second);
		m_wndStatusBar.SetPaneText(5, s);
		if (minute + second == 0) {
			// AutoStop as time is over
			curJob = EMP_END;
			PostMessage(WM_COMMAND, ID_AUTORUN, 0);
		}
		break;
	default:
		CFrameWnd::OnTimer(nIDEvent);
	}
}

void CMainFrame::AnswerView()
{
	int		kq;
	float	score;

	if (ptest->qstNo < 1) return;
	isAnsView = !isAnsView;
	if (TestType == 0) {
		IsSelection(ID_ANSWER, isAnsView);
		ptest->Invalidate();
	}
	kq = ptest->ViewResult(isAnsView, score);
	if (isAnsView) {
		LoadMessage(IDS_MESS_KETQUA, rBuf + (UDP_BUF_LEN/2));
		sprintf(rBuf, rBuf + (UDP_BUF_LEN/2), kq, score, ptest->qstNo);
		if (score > 9) strcat(rBuf,"Excellent !");
		else
		if (score > 7) strcat(rBuf, "Very Good !");
		else
		if (score > 5) strcat(rBuf, "Good !");
		else
		if (score > 4) strcat(rBuf, "Try your best !");
		else strcat(rBuf, "Give an attention of yours !");
		ocDlg = new CEmpNItemDlg(this, "Result View", rBuf, 
								(TestType==2)? NIS_NET : NIS_CANCEL);
		ocDlg->Hienthi();

		if (TestType) isAnsView = !isAnsView;
	}
}

void CMainFrame::IsSelection(int Id, BOOL yes)
{
	m_wndToolBar.GetToolBarCtrl().PressButton(Id, yes);
	GetMenu()->CheckMenuItem(Id,(yes)? MF_CHECKED:MF_UNCHECKED);
}

void CMainFrame::GotoPane(int NewPaneMenuId, BOOL force)
{
	if (cPaneId) 
		IsSelection(cPaneId, FALSE);

	cPaneId = NewPaneMenuId;
	IsSelection(cPaneId, TRUE);

	if (force) {
		if (cPaneId == ID_TEST) 
			m_split.SetActivePane(0,1);
		else
			m_split.SetActivePane(0,0);
	}
}

void CMainFrame::ActivateFrame(int nCmdShow) 
{
	RECT		sr;
	CEmpApp*	app;
	LPHOSTENT	phost;
	int			sockopt;

	CFrameWnd::ActivateFrame(SW_SHOWMAXIMIZED);

	GetClientRect(&sr);
	m_split.SetColumnInfo(0,57,0);
	m_split.RecalcLayout();
	m_split.UpdateWindow();
	pans->UpdateWindow();
	ptest->UpdateWindow();
	UpdateWindow();

	app = (CEmpApp*)AfxGetApp(); NetOK = app->NetOK;
	mypointcursor = app->LoadCursor(IDC_TEST_MARK);
	mypullcursor  = app->LoadCursor(IDC_TEST_PULL);

	NetOK &= (gethostname(HostName, HOST_NAME_LEN-1) == 0);
	if ((phost = gethostbyname(HostName)) == NULL) {
		NetOK = FALSE;
		memset(&HostAddress, 0, 4);
	}
	else memcpy(&HostAddress, *(phost->h_addr_list), 4);

	// Get computer name, not for win.2000
	sockopt = HOST_NAME_LEN-1;
	GetComputerName(HostName, (DWORD*)&sockopt);

	myNetData = &(app->myNetData);
	m_hIcon[0] = (HICON) LoadImage(app->m_hInstance, 
							MAKEINTRESOURCE(IDI_NET0),
							IMAGE_ICON,16,16,
							LR_DEFAULTCOLOR|LR_DEFAULTSIZE);
	m_hIcon[1] = (HICON) LoadImage(app->m_hInstance, 
							MAKEINTRESOURCE(IDI_NET1),
							IMAGE_ICON,16,16,
							LR_DEFAULTCOLOR|LR_DEFAULTSIZE);
	m_hIcon[2] = (HICON) LoadImage(app->m_hInstance, 
							MAKEINTRESOURCE(IDI_NET2),
							IMAGE_ICON,16,16,
							LR_DEFAULTCOLOR|LR_DEFAULTSIZE);
	m_hIcon[3] = (HICON) LoadImage(app->m_hInstance, 
							MAKEINTRESOURCE(IDI_NET3),
							IMAGE_ICON,16,16,
							LR_DEFAULTCOLOR|LR_DEFAULTSIZE);

	if (TestType) {
		if (TestType == 2) {
			if (NetOK) {
				// Open sockets
				sSock = socket(AF_INET, SOCK_DGRAM, 0);
				NetOK &= (sSock != INVALID_SOCKET);
				if (NetOK) {
					sockopt = 0;
					setsockopt(sSock, SOL_SOCKET, SO_BROADCAST, 
										(char*)&sockopt, sizeof(sockopt));
					rSock = socket(AF_INET, SOCK_DGRAM, 0);
					NetOK &= (rSock != INVALID_SOCKET);
				}

				if (NetOK) {
					// Receive address from any
					rAddr.sin_family     = AF_INET;
					rAddr.sin_port       = htons(SockPort);
					rAddr.sin_addr.s_addr= htonl(INADDR_ANY);
					NetOK &= (bind(rSock, (LPSOCKADDR)&rAddr, 
											sizeof(rAddr)) != SOCKET_ERROR);
					if (NetOK) WSAAsyncSelect(rSock, m_hWnd, IDM_TO_READ, FD_READ);
				}
			}
			else {
				TestType = 0;
				MessageBox("Windows sockets initialization failed.", "Error", MB_OK|MB_ICONEXCLAMATION);
				PostMessage(WM_COMMAND, ID_EXIT, 0);
				return;
			}
		}
		PostMessage(WM_COMMAND, ID_AUTORUN, 0);
	}

	m_iIcon = 4;
	SetTimer(ID_CLOCK, 1000, 0);
}

void CMainFrame::GetExeDir()
{
	strcpy(fName, ((CEmpApp*)AfxGetApp())->ss);
	if (fName[0] == _T('\0'))
		strcpy(fName, GetCommandLine()+1);
	fNameLen = strlen(fName);
	while (fName[fNameLen] != '\\') fNameLen--;
	fName[++fNameLen] = '\0';
}

BOOL CMainFrame::AddZmpFile()
{
	int		mi, mj;
	char	mbuf[EmpIDL+MENU_TITLE_LEN+6];
	CFile	mfile;

	if (mfile.Open(fName, CFile::modeRead|CFile::shareDenyNone, NULL)) {
		mfile.Read(mbuf, EmpIDL+MENU_TITLE_LEN+6);
		mfile.Close();
		mi = *((WORD*)(mbuf+EmpIDL+MENU_TITLE_LEN));
		if (((TestType == 2) || (mi%256 == mi/256)) &&
				(strncmp(mbuf, EmpID, EmpIDL) == 0)) {
			if (TestType && zmpNo) zmpNo--;
			else {
				zmpName[zmpNo].FileName = new char[_MAX_PATH];
				zmpName[zmpNo].MenuTitle = new char[MENU_TITLE_LEN];
			}
			strcpy(zmpName[zmpNo].FileName, fName);
			strcpy(zmpName[zmpNo].MenuTitle, mbuf+EmpIDL);
			zmpName[zmpNo].ItemNo = *((WORD*)(mbuf+EmpIDL+MENU_TITLE_LEN+sizeof(WORD)));
			zmpName[zmpNo].TimeDone = mi%256;	// time doing exercise
			mi = 0;
			while (mi < zmpNo) {
				if (strcmp(zmpName[mi].MenuTitle, zmpName[zmpNo].MenuTitle)==0)
					break;
				mi++;
			}
			if (mi < zmpNo) {
				mj = 0;
				while (mi < zmpNo) {
					if (strncmp(zmpName[mi].MenuTitle, zmpName[zmpNo].MenuTitle, strlen(zmpName[zmpNo].MenuTitle))==0)
						mj++;
					mi++;
				}
				sprintf(zmpName[zmpNo].MenuTitle, "%s  [%d]", zmpName[zmpNo].MenuTitle, mj);
			}
			zmpNo++;
			return TRUE;
		}
	}

	return FALSE;
}

void CMainFrame::AutoRun()
{
	CEmpDoc* pDoc;
	int kq;

	if (m_wndObjView != NULL && m_wndObjView->IsWindowVisible())
		m_wndObjView->DisplayView(NULL);

	if (TestType > 0) {
		pDoc = ptest->GetDocument();
		if (zmpNo) {
			// set new Test
			pDoc->DeleteContents();
			pDoc->UpdateAllViews(NULL);
			pans->GetListCtrl().DeleteAllItems();
		}
	}

	if (ocDlg != NULL) ocDlg->EndDialog(IDCANCEL);

	if (TestType < 2 || IsShowResult) {
		AnswerView();
		if (TestType == 0) {
			curJob = EMP_TEST;
			GetNextIniInfo();
			return;
		}
	}

	ClearStudentInfo();
	SetStatusBarTexts();
	ptest->qstNo = 0;		// don't let results show if no one is on this computer.

	if (TestType == 2) {
		if (curJob == EMP_CONNECT)
			LoadMessage(IDS_MESS_CONNECT, rBuf);
		else {
			LoadMessage(IDS_MESS_ACCEPT, rBuf);
			curJob = EMP_ACCEPT;
		}
		kq = NIS_NET;
	}
	else {
		LoadMessage(IDS_MESS_DOTEST, rBuf);
		kq = NIS_OK|NIS_STUDENT;
	}

   	ocDlg = new CEmpNItemDlg( this, 
					"Connect to Supervisor", rBuf, kq );
	if (ocDlg->Hienthi() == IDCANCEL) return;

	if (zmpNo) {
		// set new Test
		pDoc->SetTitle(zmpName[0].MenuTitle);

		if (TestType == 1 && curJob != EMP_CONNECT) {
			// recycle of Non-Network Test
			if (!GetNextIniInfo()) return;
		}

		// Exercise Downloading... 
		if (zmpName[0].FileName[0] == '*') {
			CMemFile* pFile = NULL;
			kq = 5;
			while (pFile == NULL) {
				// Receives exercise item from HOT
				// ...
				kq--;
				if (kq == 0 && pFile == NULL) {
					if (MessageBox(	"Supervisor Machine shutdown or Network error"
									"\nExercise Download Fail!", "Network error!", 
									MB_ICONQUESTION | MB_RETRYCANCEL) == IDRETRY)
						kq = 5;
					else {
						DoQuit();
						return;
					}
				}
			}

			kq = LoadNewItem(-1, (char*) pFile);
		}
		else
			kq = LoadNewItem(m_dethi, zmpName[0].FileName);

		// now, kq = number of questions in selected Item.
		curJob = EMP_TEST;

		NewInfo();	// send back to supervisor

		if (TestType == 1) return;

		if (kq > 0) {
			if (2*kq != AnsBufL || !ptest->SetAnswer(AnsBufL, AnsBuf)) {
				if (AnsBufL > 0) delete AnsBuf;
				AnsBufL = 2*kq;
				AnsBuf  = new char[AnsBufL];
				memset(AnsBuf, -1, AnsBufL);
				ptest->GetResponse(AnsBufL, AnsBuf);
			}
		}

		GotoPane(ID_TEST, TRUE);
	}
}

// This block uses the step-by-step network communication.
// Some steps in this function are up to you to rearrange 
// Network programming techniques are varied, and you can 
// try your best to have one of them done.
// Sorry for changes on the orriginal procedure as the
// requirement of security purpose.
// LNTsoft!

LRESULT CMainFrame::WindowProc(UINT message, WPARAM wParam, LPARAM lParam) 
{
	static int reconnecting = 0;	// no reconnection from supervisor

	switch (message) {
	case IDM_TO_READ:
		if (WSAGETSELECTEVENT(lParam) != FD_READ) break;
		if (recvfrom(rSock, rBuf, UDP_BUF_LEN, 0, (LPSOCKADDR)&tAddr, &tAddrSize) != SOCKET_ERROR
			&& strncmp(EmpID, rBuf, EmpIDL) == 0) {
			// must be an EMP. message
			m_iIcon |= 2;
			switch (rBuf[EmpIDL+1]) {
			case EMP_RECONNECT:
				if (curJob == EMP_TEST) {
					// I am working, server requires my info
					if (sAddr.sin_addr.s_addr == tAddr.sin_addr.s_addr) {
						// this is my server
						reconnecting = (ocDlg == NULL)? 2 : 0;
						// try to register to Supervisor
						// ...
					}
					break;
				}
				if (curJob == EMP_ACCEPT)
					curJob = EMP_CONNECT;

			case EMP_CONNECT:
				// I am a new one to HOT
				if (curJob != EMP_CONNECT) break;
				// rBuf = "ID, 'x'"
				sAddr.sin_addr.s_addr = inet_addr(rBuf+EmpIDL+2+HOST_NAME_LEN);
				sAddr.sin_family	= AF_INET;
				sAddr.sin_port		= htons(SockPort);
				sprintf(sBuf, "\n®á tm thãy M y Gi m Th«.\n%s\n%s", rBuf+EmpIDL+2, 
										rBuf+EmpIDL+2+HOST_NAME_LEN);

				// change interface as a supervisor message comming at
				ocDlg->ModifyInterface(0, sBuf);

				// try to register to Supervisor
				//...
				break;

			case EMP_ACCEPT:
				if (curJob != EMP_CONNECT || 
					sAddr.sin_addr.s_addr != tAddr.sin_addr.s_addr) break;
				// Reset old info in standard size (if backup): STUDENT_NAME_LEN, STUDENT_CRBD_LEN
				// rBuf = "ID, 'x',"
				//		  "LOP, SOBD, HOTEN, NGAYSINH"
				curJob = EMP_ACCEPT;
				if (AnsBufL > 0) {
					delete AnsBuf; AnsBufL = 0;
				}
				if (rBuf[EmpIDL+2]) {
					// old info comes in
					m_lop	= rBuf+3;
					m_sobd	= rBuf+3+STUDENT_CRBD_LEN;
					m_hoten	= rBuf+3+2*STUDENT_CRBD_LEN;
					m_ngaysinh = rBuf+3+2*STUDENT_CRBD_LEN+STUDENT_NAME_LEN;
				}
				LoadMessage(IDS_MESS_ACCEPT, rBuf);
				ocDlg->ModifyInterface(NIS_NET, rBuf);
				break;

			case EMP_TEST:
				// --- Start a new test
				if (sAddr.sin_addr.s_addr != tAddr.sin_addr.s_addr ||
					curJob != EMP_ACCEPT ||
					*((int*)(rBuf+EmpIDL+2+6*sizeof(int)+21)) == 0) break;

				// starting a test, numbers is in int-type

				strcpy(fName, rBuf+EmpIDL+2+7*sizeof(int)+21);
				m_StdIdChk = *((int*)(rBuf+2+5*sizeof(int)));

				if (*((int*)(rBuf+EmpIDL+2+4*sizeof(int))) == 1) {
					// Exercise Received from HOT
					// instead of the filename, sending process must set the menu info 
					// of exercise to the @rBuf portion.
					if (zmpNo) zmpNo--;
					else {
						zmpName[zmpNo].FileName = new char[_MAX_PATH];
						zmpName[zmpNo].MenuTitle = new char[MENU_TITLE_LEN];
					}
					strcpy(zmpName[zmpNo].FileName, "*");	// used as a signal in Auto-Run
					strcpy(zmpName[zmpNo].MenuTitle, fName);
					zmpName[zmpNo].ItemNo   = *((int*)(rBuf+EmpIDL+1*sizeof(int)));
					zmpName[zmpNo].TimeDone = *((int*)(rBuf+EmpIDL+2*sizeof(int)));
					zmpNo++;
				}
				else {
					// Exercise read from Windows server
					if (!AddZmpFile()) break;
				}
				
				AnsBufL = *((int*)(rBuf+EmpIDL+2+7*sizeof(int)+21+strlen(fName)+1));
				if (AnsBufL > 0) {
					// use backup info
					AnsBuf = new char[AnsBufL];
					memcpy(AnsBuf, rBuf+2+8*sizeof(int)+21+strlen(fName)+1, AnsBufL);
					memcpy(&m_dethi, rBuf+EmpIDL+2, sizeof(int));	// Backup ExeNo
					m_dethi -= 1;
				}
				else {
					memcpy(&minute, rBuf+EmpIDL, sizeof(int));	// start item
					memcpy(&second, rBuf+EmpIDL+1+sizeof(int), sizeof(int));
					second = second - minute + 1;
					m_dethi = (GetNumFromName() % second);
					if (m_dethi == 0) m_dethi = second;
					m_dethi += minute-2;
				}
				curTopic = 0;
				memcpy(&minute, rBuf+EmpIDL+2*sizeof(int), sizeof(int)); 
				second = 0;
				memcpy(&IsShowResult, rBuf+EmpIDL+3*sizeof(int), sizeof(int));

				// it is now being able go on with this->AutoRun()
				LoadMessage(IDS_MESS_DOTEST, rBuf);
				ocDlg->ModifyInterface(NIS_OK|NIS_STUDENT|((m_StdIdChk)?NIS_IDCHECK:0),rBuf);
				curJob = EMP_TEST;

				break;

			case EMP_END:
				// --- Stop test case
				if (sAddr.sin_addr.s_addr != tAddr.sin_addr.s_addr) break;
				if (curJob != EMP_TEST) break;
				minute = 0; second = 1;	// let OnTimer stop the test process
				break;

			case EMP_RETEST:
				// --- Stop result view, ready for new test
				if (sAddr.sin_addr.s_addr != tAddr.sin_addr.s_addr) break;
				if (curJob != EMP_END) break;
				curJob = EMP_ACCEPT;
				// turn the dialog off
				if (ocDlg != NULL)
					ocDlg->EndDialog(IDCANCEL);
				break;

			case EMP_QUIT:
				// --- Quit the test program
				if (rBuf[EmpIDL+2])
					TestType = -5;	// shutdown windows
				PostMessage(WM_COMMAND, ID_QUIT, 0);
				return 0;

			// some other messages will not be supplied in this source code
			// for security purpose only
			// LNTsoft!

			}
		}
		break;

	case IDM_TO_WRITE:
		if (WSAGETSELECTEVENT(lParam)==FD_WRITE) {
			// network message sending signal
			m_iIcon |= 1;
			// sAddr is already setforce to send to Supervisor
			// ...

			if (reconnecting) {
				// for multi-step message sending
				switch (reconnecting) {
					case 2:
						// Inform New Info
						break;
					case 1:
						// Inform New Answers
						// ...
						break;
				}
				reconnecting--;
			}
		}
		break;
	}

	return CFrameWnd::WindowProc(message, wParam, lParam);
}

void CMainFrame::NetStudentInfo()
{
	char s[1024];
	char ver[50] = "95\0  98\0  Me\0  NT\0  2000\0XP\0  .NET\0";

	wsprintf(s, "\nName: %s"
				"\nIP Address: %s - Winsock %d.%d"
				"\nMicrosoft Windows %s. Ver %d.%d",
				HostName, inet_ntoa(HostAddress),
				LOBYTE(myNetData->wHighVersion),
				HIBYTE(myNetData->wHighVersion),
				ver + m_verCode*5,
				m_verMajor, m_verMinor);

	ocDlg = new CEmpNItemDlg(this, "This Computer", s, 
					NIS_OK|NIS_CANCEL|NIS_STUDENT|((m_StdIdChk) ? NIS_IDCHECK : 0));

	// Chang the student information
	if (ocDlg->Hienthi() == IDOK)
		NewInfo();
}

BOOL CMainFrame::GetNextIniInfo()
{
	if (TestType == 2) return TRUE;
	fName[fNameLen]='\0'; strcat(fName,"TEST.INI");
	minute = GetPrivateProfileInt("EMP_OPTION", "TIME", 30, fName);
	second = 0;
	if (TestType==1) {
		GetPrivateProfileString("EMP_OPTION", "FILENAME", 
								"EMP.ZMP", fName+fNameLen, 
								_MAX_PATH-fNameLen, fName);
		if (AddZmpFile()) {
			fName[fNameLen] = '\0'; strcat(fName, "TEST.INI");
			m_dethi = GetPrivateProfileInt("EMP_OPTION", "ITEM", 1, fName) - 1;
			// One zmp file only
			if (m_dethi < 0 || m_dethi >= zmpName[0].ItemNo)
				m_dethi = zmpName[0].ItemNo-1;
			return TRUE;
		}
		TestType = 0;	// No correct file specified
	}
	if (curJob != EMP_TEST) GetZmpFile();
	return FALSE;
}

int CMainFrame::GetNumFromName()
{
	UINT n1, n2=0;
	for (n1=0; n1 < strlen(HostName); n1++) {
		if (HostName[n1] <= '9' && HostName[n1] >= '0')
			n2 = 10*n2 + (HostName[n1]-48);
	}
	return n2;
}

void CMainFrame::NewInfo()
{
	CStatusBarCtrl& bc = m_wndStatusBar.GetStatusBarCtrl();
	CDC* me = bc.GetDC();
	CFont* oldFont = me->SelectObject(&m_vnFont);
	int lr = me->GetTextExtent("  ").cx;
	m_wndStatusBar.SetPaneInfo(1, ID_INDICATOR_LOP, 0, me->GetTextExtent(m_lop).cx + lr);
	m_wndStatusBar.SetPaneInfo(2, ID_INDICATOR_SOBD, 0, me->GetTextExtent(m_sobd).cx + lr);
	m_wndStatusBar.SetPaneInfo(3, ID_INDICATOR_HOTEN, 0, me->GetTextExtent(m_hoten).cx + lr);
	m_wndStatusBar.SetPaneInfo(4, ID_INDICATOR_NGAYSINH, 0, me->GetTextExtent(m_ngaysinh).cx + lr);
	me->SelectObject(oldFont);
	SetStatusBarTexts();

	// Inform New Info
	// ...
}

void CMainFrame::DoQuit()
{
	if (ocDlg) ocDlg->EndDialog(IDCANCEL);
	ptest->qstNo = 0;
	if (TestType > 0) TestType = -1;
	PostMessage(WM_CLOSE, 0, 0);
}

void CMainFrame::ClearStudentInfo(void)
{
	m_lop.Empty();
	m_sobd.Empty();
	m_hoten.Empty();
	m_ngaysinh.Empty();
}

void CMainFrame::SetStatusBarTexts(void)
{
	m_wndStatusBar.SetPaneText(1, " " + m_lop);
	m_wndStatusBar.SetPaneText(2, " " + m_sobd);
	m_wndStatusBar.SetPaneText(3, " " + m_hoten);
	m_wndStatusBar.SetPaneText(4, " " + m_ngaysinh);
}

void CMainFrame::EnvironmentSetting(int how)
{	/*	how = 0:	get
			= 1:	clear
			= 2:	restore
	*/
	static int powerTimeout;
	static int ssTimeout;
	CWnd* hWndTaskbar = FindWindow(_T("Shell_TrayWnd"), _T(""));

	switch (how) {
		case 0:
			SystemParametersInfo(SPI_GETPOWEROFFTIMEOUT,
									0, &powerTimeout, 0 );
			SystemParametersInfo(SPI_GETSCREENSAVETIMEOUT,
									0, &ssTimeout, 0 );
			break;
		case 1:
			hWndTaskbar->EnableWindow(FALSE);

			SystemParametersInfo(SPI_SETPOWEROFFTIMEOUT,
									0, NULL, SPIF_UPDATEINIFILE );
			SystemParametersInfo(SPI_SETSCREENSAVETIMEOUT,
									0, NULL, SPIF_UPDATEINIFILE );

			SystemParametersInfo(SPI_SETSCREENSAVERRUNNING, 
									1, NULL, 0);
			break;
		case 2:
			hWndTaskbar->EnableWindow(TRUE);

			SystemParametersInfo(SPI_SETPOWEROFFTIMEOUT,
									powerTimeout, NULL, SPIF_UPDATEINIFILE );
			SystemParametersInfo(SPI_SETSCREENSAVETIMEOUT,
									ssTimeout, NULL, SPIF_UPDATEINIFILE );

			SystemParametersInfo(SPI_SETSCREENSAVERRUNNING, 
									0, NULL, 0);
			break;
	}
	return;
}

int CMainFrame::LoadMessage(UINT sID, char* pMessage)
{
	return LoadString(AfxGetInstanceHandle(), sID,
						pMessage, UDP_BUF_LEN-1);
}

void CMainFrame::OnOpenItem()
{
	if (curTopic > -1) {
		LoadMessage(IDS_MESS_CHONDE, rBuf + (UDP_BUF_LEN/2));
		sprintf(rBuf, rBuf + (UDP_BUF_LEN/2), zmpName[curTopic].ItemNo);
		ocDlg = new CEmpNItemDlg(this, zmpName[curTopic].MenuTitle, rBuf, NIS_OK|NIS_CANCEL);
	}
	else {
		LoadMessage(IDS_MESS_CHONMUC, rBuf);
		ocDlg = new CEmpNItemDlg(this, "NoSelection", rBuf, NIS_CANCEL);
	}

	if (ocDlg->Hienthi() == IDOK) {
		curJob = EMP_TEST;
		GetNextIniInfo();
		minute = zmpName[curTopic].TimeDone;

		LoadNewItem(m_dethi, zmpName[curTopic].FileName);

		isAnsView = FALSE;
		IsSelection(ID_ANSWER, isAnsView);
		GotoPane(ID_TEST, TRUE);
	}
}

int CMainFrame::LoadNewItem(int item, const char* filename)
{
	int itemNo;

	m_wndStatusBar.GetWindowText(rBuf, UDP_BUF_LEN-1);

	itemNo = ptest->NewItem(item, filename);

	if (itemNo)
		m_wndStatusBar.SetPaneText(0, rBuf);

	return itemNo;
}

// Active the view frame to show selected object
void CMainFrame::DoObjectView(CRichEditCntrItem* pItem)
{
	if (m_wndObjView == NULL) {
		m_wndObjView = new CObjectView();
		if (!m_wndObjView->CreateEx(WS_EX_TOPMOST, NULL, "ObjectView", 
				WS_CHILD|WS_POPUP|WS_CAPTION|WS_SYSMENU,
				37,0,147,110, this->m_hWnd, NULL)) 
			{
				// error in creating view frame window
				delete m_wndObjView;
				m_wndObjView = NULL;
				return;
			}
	}

	m_wndObjView->DisplayView(pItem);
}

void CMainFrame::OnActivate(UINT nState, CWnd* pWndOther, BOOL bMinimized)
{
	if (nState == WA_INACTIVE) { 
		// turn of the object view
		if (m_wndObjView != NULL && pWndOther != m_wndObjView &&
			m_wndObjView->IsWindowVisible())
			m_wndObjView->SendMessage(WM_CLOSE);
	}

	CFrameWnd::OnActivate(nState, pWndOther, bMinimized);
}
