// emp.cpp : Defines the class behaviors for the application.
//

#include "stdafx.h"
#include "emp.h"
#include "..\..\LNT-LIB.CPP\shell\regkey.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CEmpApp

BEGIN_MESSAGE_MAP(CEmpApp, CWinApp)
	//{{AFX_MSG_MAP(CEmpApp)
	ON_COMMAND(ID_ABOUT, OnAppAbout)
		// NOTE - the ClassWizard will add and remove mapping macros here.
		//    DO NOT EDIT what you see in these blocks of generated code!
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CEmpApp construction

CEmpApp::CEmpApp()
{
}

/////////////////////////////////////////////////////////////////////////////
// The one and only CEmpApp object

CEmpApp theApp;

/////////////////////////////////////////////////////////////////////////////
// CEmpApp initialization

BOOL CEmpApp::InitInstance()
{
	// 800x600
	if (!SetScreenMode(TRUE)) {
		AfxMessageBox("800x600 screen resolution is required.\n"
					  "Program is abnormally terminated. Sorry !",
					  MB_OK, 0);
		return FALSE;
	}

	FirstTime = sinst.Create(IDR_MAINFRAME);
	if (!FirstTime) return FALSE;

	//AfxInitRichEdit2();

	if (!AfxOleInit()) return FALSE;

	if (!AfxSocketInit(&myNetData))
		NetOK = FALSE;
	else NetOK = TRUE;

	AfxEnableControlContainer();
	AddFontResource(EmpFont);

	// Check to see wherether my Icon is registered to shell
	if (!CEmpRegKey::IsKeyExist(HKEY_CLASSES_ROOT, "LNTsoft Test Document")) {
		CEmpRegKey::CreateKey(HKEY_CLASSES_ROOT,".zmp");
		CEmpRegKey::SetValueData(HKEY_CLASSES_ROOT, ".zmp","",
							"LNTsoft Test Document", 22,
							CEmpRegKey::kieu_chuoi);
		CEmpRegKey::CreateKey(HKEY_CLASSES_ROOT,"LNTsoft Test Document");
		CEmpRegKey::SetValueData(HKEY_CLASSES_ROOT, "LNTsoft Test Document", "",
							"Test Document", 14, 
							CEmpRegKey::kieu_chuoi);
		strcpy((char*)m_pszHelpFilePath+strlen(m_pszHelpFilePath)-3,"EXE");
		sprintf(ss, "%s,1", m_pszHelpFilePath);
		CEmpRegKey::CreateKey(HKEY_CLASSES_ROOT,"LNTsoft Test Document\\DefaultIcon");
		CEmpRegKey::SetValueData(HKEY_CLASSES_ROOT, "LNTsoft Test Document\\DefaultIcon",
							"", ss, strlen(ss)+1, CEmpRegKey::kieu_chuoi);

		sprintf(ss, "\"%s\" \"%s\"", m_pszHelpFilePath, "%1");
		CEmpRegKey::CreateKey(HKEY_CLASSES_ROOT,"LNTsoft Test Document\\Shell");
		CEmpRegKey::CreateKey(HKEY_CLASSES_ROOT,"LNTsoft Test Document\\Shell\\Open");
		CEmpRegKey::CreateKey(HKEY_CLASSES_ROOT,"LNTsoft Test Document\\Shell\\Open\\Command");
		CEmpRegKey::SetValueData(HKEY_CLASSES_ROOT, "LNTsoft Test Document\\Shell\\Open\\Command",
							"", ss, strlen(ss)+1, CEmpRegKey::kieu_chuoi);		
		SHChangeNotify(SHCNE_ASSOCCHANGED, SHCNF_IDLIST, NULL, NULL);
	}

	strcpy((char*)m_pszHelpFilePath+strlen(m_pszHelpFilePath)-8,"EMP.HLP");
	SetRegistryKey(_T("LNTsoft"));

	CSingleDocTemplate* pDocTemplate;
	pDocTemplate = new CSingleDocTemplate(
		IDR_MAINFRAME,
		RUNTIME_CLASS(CEmpDoc),
		RUNTIME_CLASS(CMainFrame),       // main SDI frame window
		RUNTIME_CLASS(CEmpView));
	AddDocTemplate(pDocTemplate);

	// Parse command line for standard shell commands, DDE, file open
	CCommandLineInfo cmdInfo;
	ParseCommandLine(cmdInfo);

	if (cmdInfo.m_nShellCommand != CCommandLineInfo::FileNew) {
		cmdInfo.m_nShellCommand = CCommandLineInfo::FileNew;
		strcpy(ss, cmdInfo.m_strFileName);
	}
	else ss[0] = _T('\0');

	// Dispatch commands specified on the command line
	if (!ProcessShellCommand(cmdInfo))
		return FALSE;

	return TRUE;
}

/////////////////////////////////////////////////////////////////////////////
// CAboutDlg dialog used for App About

class CAboutDlg : public CDialog
{
public:
	CAboutDlg();

// Dialog Data
	//{{AFX_DATA(CAboutDlg)
	enum { IDD = IDD_ABOUTBOX };
	//}}AFX_DATA

	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CAboutDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	//{{AFX_MSG(CAboutDlg)
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDialog(CAboutDlg::IDD)
{
	//{{AFX_DATA_INIT(CAboutDlg)
	//}}AFX_DATA_INIT
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CAboutDlg)
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialog)
	//{{AFX_MSG_MAP(CAboutDlg)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

// App command to run the dialog
void CEmpApp::OnAppAbout()
{
	CAboutDlg aboutDlg;
	aboutDlg.DoModal();
}

int CEmpApp::ExitInstance() 
{
	SetScreenMode(FALSE);
	if (FirstTime) RemoveFontResource(EmpFont);
	return CWinApp::ExitInstance();
}

bool CEmpApp::SetScreenMode(bool isSet)
{
	static DWORD oldWidth=0, oldHeight=0;
	DEVMODE dm;
	memset(&dm, 0, sizeof(DEVMODE));
	dm.dmSize = sizeof(DEVMODE);

	if (isSet) {
		EnumDisplaySettings(NULL, ENUM_CURRENT_SETTINGS, &dm);
		if (dm.dmPelsWidth == 800 && dm.dmPelsHeight == 600)
			return TRUE;
		oldWidth  = dm.dmPelsWidth;
		oldHeight = dm.dmPelsHeight;
		dm.dmPelsWidth = 800;
		dm.dmPelsHeight= 600;
	}
	else {
		if (oldWidth == 0 && oldHeight == 0) 
			return TRUE;
		dm.dmPelsWidth = oldWidth;
		dm.dmPelsHeight= oldHeight;
	}

	dm.dmFields =  DM_PELSWIDTH | DM_PELSHEIGHT;
	if (ChangeDisplaySettings(&dm, CDS_UPDATEREGISTRY) != DISP_CHANGE_SUCCESSFUL &&
		ChangeDisplaySettings(&dm, 0) != DISP_CHANGE_SUCCESSFUL )
			return FALSE;

	return TRUE;
}
