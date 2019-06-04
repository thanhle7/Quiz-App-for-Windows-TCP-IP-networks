// EmpED.cpp : Defines the class behaviors for the application.
//

#include "stdafx.h"
#include "EmpED.h"
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
	ON_COMMAND(ID_APP_ABOUT, OnAppAbout)
	//}}AFX_MSG_MAP
	// Standard file based document commands
	ON_COMMAND(ID_FILE_OPEN, CWinApp::OnFileOpen)
	ON_COMMAND(ID_FILE_NEW, CWinApp::OnFileNew)
	// Standard print setup command
	ON_COMMAND(ID_FILE_PRINT_SETUP, CWinApp::OnFilePrintSetup)
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

	// Initialize OLE libraries
	if (!AfxOleInit())
	{
		AfxMessageBox(IDP_OLE_INIT_FAILED);
		return FALSE;
	}

	AddFontResource(EmpFont);
	AfxEnableControlContainer();

	// Check to see wherether my Icon is registered to shell
	if (!CEmpRegKey::IsKeyExist(HKEY_CLASSES_ROOT, "LNTsoft Editor Document")) {
		char ss[_MAX_PATH+1];
		CEmpRegKey::CreateKey(HKEY_CLASSES_ROOT,".emp");
		CEmpRegKey::SetValueData(HKEY_CLASSES_ROOT, ".emp","",
							"LNTsoft Editor Document", 24,
							CEmpRegKey::kieu_chuoi);
		CEmpRegKey::CreateKey(HKEY_CLASSES_ROOT,"LNTsoft Editor Document");
		CEmpRegKey::SetValueData(HKEY_CLASSES_ROOT, "LNTsoft Editor Document", "",
							"Editor Document", 16, 
							CEmpRegKey::kieu_chuoi);
		strcpy((char*)m_pszHelpFilePath+strlen(m_pszHelpFilePath)-3,"EXE");
		sprintf(ss, "%s,1", m_pszHelpFilePath);
		CEmpRegKey::CreateKey(HKEY_CLASSES_ROOT,"LNTsoft Editor Document\\DefaultIcon");
		CEmpRegKey::SetValueData(HKEY_CLASSES_ROOT, "LNTsoft Editor Document\\DefaultIcon",
							"", ss, strlen(ss)+1, CEmpRegKey::kieu_chuoi);

		sprintf(ss, "\"%s\" \"%s\"", m_pszHelpFilePath, "%1");
		CEmpRegKey::CreateKey(HKEY_CLASSES_ROOT,"LNTsoft Editor Document\\Shell");
		CEmpRegKey::CreateKey(HKEY_CLASSES_ROOT,"LNTsoft Editor Document\\Shell\\Open");
		CEmpRegKey::CreateKey(HKEY_CLASSES_ROOT,"LNTsoft Editor Document\\Shell\\Open\\Command");
		CEmpRegKey::SetValueData(HKEY_CLASSES_ROOT, "LNTsoft Editor Document\\Shell\\Open\\Command",
							"", ss, strlen(ss)+1, CEmpRegKey::kieu_chuoi);
		SHChangeNotify(SHCNE_ASSOCCHANGED, SHCNF_IDLIST, NULL, NULL);
	}

	// Help File setting
	strcpy((char*)m_pszHelpFilePath+strlen(m_pszHelpFilePath)-10,"EMP.HLP");

	// Standard initialization

//#ifdef _AFXDLL
//	Enable3dControls();			// Call this when using MFC in a shared DLL
//#else
//	Enable3dControlsStatic();	// Call this when linking to MFC statically
//#endif

	// Change the registry key under which our settings are stored.
	SetRegistryKey(_T("LNTsoft"));

	LoadStdProfileSettings();  // Load standard INI file options (including MRU)

	// Register document templates

	CSingleDocTemplate* pDocTemplate;
	pDocTemplate = new CSingleDocTemplate(
		IDR_MAINFRAME,
		RUNTIME_CLASS(CEmpDoc),
		RUNTIME_CLASS(CMainFrame),       // main SDI frame window
		RUNTIME_CLASS(CEmpView));
	pDocTemplate->SetContainerInfo(IDR_CNTR_INPLACE);
	AddDocTemplate(pDocTemplate);

	// Parse command line for standard shell commands, DDE, file open
	CCommandLineInfo cmdInfo;
	ParseCommandLine(cmdInfo);

	// Dispatch commands specified on the command line
	if (!ProcessShellCommand(cmdInfo))
		return FALSE;
	m_pMainWnd->ShowWindow(SW_HIDE);
	m_pMainWnd->ShowWindow(SW_SHOWMAXIMIZED);
	m_pMainWnd->UpdateWindow();

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
		// No message handlers
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
		// No message handlers
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

// App command to run the dialog
void CEmpApp::OnAppAbout()
{
	CAboutDlg aboutDlg;
	aboutDlg.DoModal();
}

/////////////////////////////////////////////////////////////////////////////
// CEmpApp message handlers


int CEmpApp::ExitInstance() 
{
	SetScreenMode(FALSE);
	RemoveFontResource(EmpFont);
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
