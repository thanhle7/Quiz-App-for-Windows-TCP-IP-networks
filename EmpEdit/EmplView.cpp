// EmplView.cpp : implementation file
//

#include "stdafx.h"
#include "emped.h"
#include "EmplView.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CEmplView

IMPLEMENT_DYNCREATE(CEmplView, CView)

CEmplView::CEmplView()
{
}

CEmplView::~CEmplView()
{
}

BEGIN_MESSAGE_MAP(CEmplView, CView)
	//{{AFX_MSG_MAP(CEmplView)
	ON_WM_PAINT()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

void CEmplView::OnPaint() 
{
	PAINTSTRUCT ps;
	RECT		rt;
	CDC* me = BeginPaint(&ps);
	GetClientRect(&rt);
	me->SetBkMode(TRANSPARENT);
	me->FillSolidRect(&rt, RGB(70,185,185));
	rt.top = (rt.top+rt.bottom)/4; rt.bottom = rt.top+200; rt.left++;
	me->SetTextColor(RGB(90,90,90));
	me->DrawText("NEW !\nVersion\n2.3v\n\n\nGroups\nof\nvaried\nlevels\n\nin a\nSegment",58, 
					&rt,DT_CENTER|DT_WORDBREAK);
	rt.left-=2; rt.top--;
	me->SetTextColor(RGB(0,0,190));
	me->DrawText("NEW !\nVersion\n2.3v\n\n\nGroups\nof\nvaried\nlevels\n\nin a\nSegment",58, 
					&rt,DT_CENTER|DT_WORDBREAK);
	EndPaint(&ps);	
}

BOOL CEmplView::PreCreateWindow(CREATESTRUCT& cs) 
{
	cs.style |= WS_DISABLED;	
	return CView::PreCreateWindow(cs);
}

void CEmplView::OnDraw(CDC* /*pDC*/)
{
}
