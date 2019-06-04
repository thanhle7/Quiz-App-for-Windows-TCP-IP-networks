/*
	This source code is given to people who want to improve their C++ language programming skill.
	It may be used for educational purposes but not for others.
	No guaranties are supplied along this source code.

	Some Libraries and programming techniques in this source code can be found in the book:

		"Lap trinh Windows voi MFC - VC++ 6.0, Le Ngoc Thanh, NXB Thong Ke 2002"

	PS : This source code is uncompatible with later versions of EmpTest from LNT/EMPsoft,
	     and uQuiz from usewide dev.
*/

// stdafx.h : include file for standard system include files,
// or project specific include files that are used frequently,
// but are changed infrequently

#pragma once

#ifndef VC_EXTRALEAN
#define VC_EXTRALEAN		// Exclude rarely-used stuff from Windows headers
#endif

// Modify the following defines if you have to target a platform prior to the ones specified below.
// Refer to MSDN for the latest info on corresponding values for different platforms.
#ifndef WINVER				// Allow use of features specific to Windows 95 and Windows NT 4 or later.
#define WINVER 0x0400		// Change this to the appropriate value to target Windows 98 and Windows 2000 or later.
#endif

#ifndef _WIN32_WINNT		// Allow use of features specific to Windows NT 4 or later.
#define _WIN32_WINNT 0x0400	// Change this to the appropriate value to target Windows 98 and Windows 2000 or later.
#endif						

#ifndef _WIN32_WINDOWS		// Allow use of features specific to Windows 98 or later.
#define _WIN32_WINDOWS 0x0410 // Change this to the appropriate value to target Windows Me or later.
#endif

#ifndef _WIN32_IE			// Allow use of features specific to IE 4.0 or later.
#define _WIN32_IE 0x0400	// Change this to the appropriate value to target IE 5.0 or later.
#endif

#define _ATL_CSTRING_EXPLICIT_CONSTRUCTORS	// some CString constructors will be explicit

// turns off MFC's hiding of some common and often safely ignored warning messages
#define _AFX_ALL_WARNINGS

#include <afxwin.h>         // MFC core and standard components
#include <afxext.h>         // MFC extensions

#ifndef _AFX_NO_AFXCMN_SUPPORT
#include <afxcmn.h>			// MFC support for Windows Common Controls
#endif						// _AFX_NO_AFXCMN_SUPPORT

#include <afxsock.h>		// MFC socket extensions

#define	IDM_TO_READ			WM_USER+400
#define IDM_TO_WRITE		WM_USER+401
#define IDM_HOSTINFO		WM_USER+402
const	UDP_PORT			= 34020;
const	TCP_PORT			= 34021;
const	UDP_BUF_LEN			= 512;
const	TCP_BUF_LEN			= 1024;
const	HOST_NAME_LEN		= 51;
const	EmpIDL				= 8;
const	MENU_TITLE_LEN		= 40;
// Student Information
const	STUDENT_NAME_LEN	= 41;	// length of student name
const	STUDENT_CRBD_LEN	= 11;	// length of Class |Reg# | BirthDate item

// Internal Messages of HOT, Test and Scaner
const char EmpID[EmpIDL]	= {'E','M','P','.','L','N','T','\x1A'};
const char EMP_CONNECT		= '1';	// Supervisor message
const char EMP_RECONNECT	= '2';	// Supervisor message
const char EMP_ACCEPT		= '3';	// Supervisor ..
const char EMP_TEST			= '4';	// Supervisor .. (wait from WSs)
const char EMP_END			= '5';	// Supervisor .. (wait from WSs)
const char EMP_RETEST		= '6';	// Supervisor .. (wait from WSs)
const char EMP_QUIT			= '7';	// Sup & Wss message
const char EMP_SCAN_REGIST	= '8';	// Scaner 
const char EMP_SCAN_REQUEST	= '9';	// ... ask for new student
