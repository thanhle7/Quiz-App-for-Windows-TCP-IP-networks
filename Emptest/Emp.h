/*
	This source code is given to people who want to improve their C++ language programming skill.
	It may be used for educational purposes but not for others.
	No guaranties are supplied along this source code.

	Some Libraries and programming techniques in this source code can be found in the book:

		"Lap trinh Windows voi MFC - VC++ 6.0, Le Ngoc Thanh, NXB Thong Ke 2002"

	PS : This source code is uncompatible with later versions of EmpTest from LNT/EMPsoft,
	     and uQuiz from usewide dev.
*/

// emp.h : main header file for the Emp application
//

#if !defined(AFX_Emp_H__676FA395_3527_11D4_BB67_0008C7331CD4__INCLUDED_)
#define AFX_Emp_H__676FA395_3527_11D4_BB67_0008C7331CD4__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#ifndef __AFXWIN_H__
	#error include 'stdafx.h' before including this file for PCH
#endif

#include "resource.h"       // main symbols
#include "EmpNItem.h"
#include "EmpQView.h"
#include "EmpFSplt.h"
#include "EmpFrame.h"
#include "EmpItem.h"
#include "EmpDoc.h"
#include "EmpView.h"
#include "..\..\LNT-LIB.CPP\Shell\Instance.h"

/////////////////////////////////////////////////////////////////////////////
// CEmpApp:
// See emp.cpp for the implementation of this class
//

const char EmpFont[8] = "EMP.FON";

class CEmpApp : public CWinApp
{
public:
	bool	SetScreenMode(bool isSet);
	char	ss[_MAX_PATH+1];	// starting parameters
	BOOL	NetOK, FirstTime;
	WSADATA	myNetData;
	CSingleInstance sinst;
	CEmpApp();
// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CEmpApp)
	public:
	virtual BOOL InitInstance();
	virtual int ExitInstance();
	//}}AFX_VIRTUAL

// Implementation
	//{{AFX_MSG(CEmpApp)
	afx_msg void OnAppAbout();
		// NOTE - the ClassWizard will add and remove member functions here.
		//    DO NOT EDIT what you see in these blocks of generated code !
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};


/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_Emp_H__676FA395_3527_11D4_BB67_0008C7331CD4__INCLUDED_)
