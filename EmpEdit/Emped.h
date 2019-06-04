/*
	This source code is given to people who want to improve their C++ language programming skill.
	It may be used for educational purposes but not for others.
	No guaranties are supplied along this source code.

	Some Libraries and programming techniques in this source code can be found in the book:

		"Lap trinh Windows voi MFC - VC++ 6.0, Le Ngoc Thanh, NXB Thong Ke 2002"

	PS : This source code is uncompatible with later versions of EmpTest from LNT/EMPsoft,
	     and uQuiz from usewide dev.
*/

// EmpED.h : main header file for the EMPED application
//

#if !defined(AFX_EMPED_H__0F4A7E32_394D_4D6F_B63C_6019AF40A28B__INCLUDED_)
#define AFX_EMPED_H__0F4A7E32_394D_4D6F_B63C_6019AF40A28B__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#ifndef __AFXWIN_H__
	#error include 'stdafx.h' before including this file for PCH
#endif

#include "resource.h"
#include "MainFrm.h"
#include "EmpItem.h"
#include "EmpDoc.h"
#include "EmpView.h"
#include "EmplView.h"

const char EmpFont[8] = "EMP.FON";

/////////////////////////////////////////////////////////////////////////////
// CEmpApp:
// See EmpED.cpp for the implementation of this class
//

class CEmpApp : public CWinApp
{
public:
	CEmpApp();
	bool SetScreenMode(bool isSet);

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
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};


/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_EMPED_H__0F4A7E32_394D_4D6F_B63C_6019AF40A28B__INCLUDED_)
