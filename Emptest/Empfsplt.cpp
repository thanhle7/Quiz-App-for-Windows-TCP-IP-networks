// EmpFSplt.cpp : implementation file
//

#include "stdafx.h"
#include "EmpFSplt.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CEmpFixSplter

CEmpFixSplter::CEmpFixSplter()
{
}

CEmpFixSplter::~CEmpFixSplter()
{
}

BEGIN_MESSAGE_MAP(CEmpFixSplter, CSplitterWnd)
	//{{AFX_MSG_MAP(CEmpFixSplter)
	ON_WM_LBUTTONDOWN()
	ON_WM_MOUSEMOVE()
	ON_WM_LBUTTONUP()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CEmpFixSplter message handlers

void CEmpFixSplter::OnLButtonDown(UINT nFlags, CPoint point) 
{
	CWnd::OnLButtonDown(nFlags, point);
}

void CEmpFixSplter::OnMouseMove(UINT nFlags, CPoint point) 
{
	CWnd::OnMouseMove(nFlags, point);
}

void CEmpFixSplter::OnLButtonUp(UINT nFlags, CPoint point) 
{
	CWnd::OnLButtonUp(nFlags, point);
}
