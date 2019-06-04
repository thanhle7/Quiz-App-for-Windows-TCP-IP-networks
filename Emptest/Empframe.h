// EmpFrame.h : interface of the CMainFrame class
//
/////////////////////////////////////////////////////////////////////////////

#if !defined(AFX_EMPFRAME_H__676FA399_3527_11D4_BB67_0008C7331CD4__INCLUDED_)
#define AFX_EMPFRAME_H__676FA399_3527_11D4_BB67_0008C7331CD4__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "ObjView.h"

class CEmpView;

class CMainFrame : public CFrameWnd
{
protected: // create from serialization only
	CMainFrame();
	DECLARE_DYNCREATE(CMainFrame)

// Attributes
public:
	CStatusBar		m_wndStatusBar;
	CFont			m_vnFont;
	// set by new item dialog...
	CString			m_lop, m_sobd, m_hoten, m_ngaysinh;
	int				m_dethi;
	// end of set...
	unsigned char	*levelSet;
	char			*AnsBuf;
	int				AnsBufL;
	int				minute, second;
	CEmpFixSplter	m_split;
	CEmpView*		ptest;
	CEmpQstsView*	pans;
	struct {
		char*	FileName;
		char*	MenuTitle;
		WORD	ItemNo, TimeDone;
	} zmpName[25];
	int				zmpNo;		// number of zmp File
	int				curTopic;	// zmp selection
	BOOL			isAnsView;
	int				cPaneId;
	int				TestType;
	int				fNameLen;
	char			fName[_MAX_PATH];
	BOOL			NetOK, m_StdIdChk;
	char			curJob;
	HCURSOR			mypointcursor, mypullcursor;
	CEmpNItemDlg*	ocDlg;
// Operations
public:
	
// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CMainFrame)
	public:
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
	virtual BOOL OnCmdMsg(UINT nID, int nCode, void* pExtra, AFX_CMDHANDLERINFO* pHandlerInfo);
	virtual void ActivateFrame(int nCmdShow = -1);
	protected:
	virtual BOOL OnCreateClient(LPCREATESTRUCT lpcs, CCreateContext* pContext);
	virtual BOOL OnCommand(WPARAM wParam, LPARAM lParam);
	virtual LRESULT WindowProc(UINT message, WPARAM wParam, LPARAM lParam);
	//}}AFX_VIRTUAL

// Implementation
public:
	// Active the view frame to show selected object
	void	DoObjectView(CRichEditCntrItem* pItem);
	void	NewInfo(void);
	void	IsSelection(int Id, BOOL yes);
	void	GotoPane(int NewPaneMenuId, BOOL force);
	void	SetStatusBarTexts(void);
	afx_msg void OnActivate(UINT nState, CWnd* pWndOther, BOOL bMinimized);
	afx_msg void OnOpenItem();
	virtual ~CMainFrame();
#ifdef _DEBUG
#endif

// Generated message map functions
protected:
	CObjectView	*m_wndObjView;
	CToolBar    m_wndToolBar;
	int			m_verCode, m_verMajor, m_verMinor;
	int			SockPort, tAddrSize;
	SOCKET		rSock, sSock;
	SOCKADDR_IN rAddr, tAddr, sAddr;	// master address
	IN_ADDR		HostAddress;
	char		HostName[HOST_NAME_LEN];
	WSADATA		*myNetData;
	char		sBuf[UDP_BUF_LEN], rBuf[UDP_BUF_LEN];
	int			IsShowResult;
	HICON		m_hIcon[4];
	int			m_iIcon;

	void DoQuit();
	int  GetNumFromName(void);
	BOOL GetNextIniInfo(void);
	void NetStudentInfo(void);
	void AutoRun(void);
	BOOL AddZmpFile(void);
	void GetExeDir(void);
	void AnswerView(void);
	void QuestionWnd();
	void TestWnd(void);
	void SaveItem(void);
	void NewTopic(int Id, int BaseMenuId);
	void ClearStudentInfo(void);
	int	 LoadMessage(UINT sID, char* pMessage);
	void GetZmpFile(void);
	void InformRegistration(char how);
	/* 0: get, 1:clear, 2:restore */
	void EnvironmentSetting(int how);
	int	 LoadNewItem(int item, const char* filename);
	CMemFile*	ExerciseDownload(void);

	//{{AFX_MSG(CMainFrame)
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnClose();
	afx_msg void OnTimer(UINT nIDEvent);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_EMPFRAME_H__676FA399_3527_11D4_BB67_0008C7331CD4__INCLUDED_)
