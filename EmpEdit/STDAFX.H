/*
	This source code is given to people who want to improve their C++ language programming skill.
	It may be used for educational purposes but not for others.
	No guaranties are supplied along this source code.

	Some Libraries and programming techniques in this source code can be found in the book:

		"Lap trinh Windows voi MFC - VC++ 6.0, Le Ngoc Thanh, NXB Thong Ke 2002"

	PS : This source code is uncompatible with later versions of EmpTest from LNT/EMPsoft,
	     and uQuiz from usewide dev.
*/
#if !defined(AFX_STDAFX_H__B1BC1EA7_A4C0_48B2_BEB9_783BCC60FCC3__INCLUDED_)
#define AFX_STDAFX_H__B1BC1EA7_A4C0_48B2_BEB9_783BCC60FCC3__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#define VC_EXTRALEAN		// Exclude rarely-used stuff from Windows headers

#include <afxwin.h>         // MFC core and standard components
#include <afxext.h>         // MFC extensions
#include <afxole.h>         // MFC OLE classes
#include <afxodlgs.h>       // MFC OLE dialog classes
#include <afxdisp.h>        // MFC Automation classes
#include <afxdtctl.h>		// MFC support for Internet Explorer 4 Common Controls
#ifndef _AFX_NO_AFXCMN_SUPPORT
#include <afxcmn.h>			// MFC support for Windows Common Controls
#endif // _AFX_NO_AFXCMN_SUPPORT

#include "afxwin.h"
#include <afxrich.h>		// MFC rich edit classes

// SEE: AUTHOR1, the first character of the following fontface name 
// must be defferent from that of EMP.Font name for reason of algorithm 
const	char SYMBOL[7]= "Symbol";

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_STDAFX_H__B1BC1EA7_A4C0_48B2_BEB9_783BCC60FCC3__INCLUDED_)
