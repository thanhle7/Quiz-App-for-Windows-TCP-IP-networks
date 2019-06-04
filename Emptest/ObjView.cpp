// ObjView.cpp : implementation file
//

#include "stdafx.h"
#include "ObjView.h"

// CObjectView

CObjectView::CObjectView()
{
	m_isNewItem = false;
}

CObjectView::~CObjectView()
{
}

BEGIN_MESSAGE_MAP(CObjectView, CWnd)
	ON_WM_CLOSE()
	ON_WM_PAINT()
END_MESSAGE_MAP()

// CObjectView message handlers

void CObjectView::AdjustObjView(CRichEditCntrItem* pItem)
{
	if (IsWindowVisible() && m_pItem == pItem)
		DisplayView(NULL);
}

void CObjectView::DisplayView(CRichEditCntrItem* pItem)
{
	m_pItem = pItem;
	m_isNewItem = true;
	if (pItem != NULL) {
		ShowWindow(SW_SHOWNA);
		SetWindowText(m_pItem->GetDocument()->GetTitle());
		Invalidate(FALSE);
	}
	else ShowWindow(SW_HIDE);
}

void CObjectView::OnClose()
{
	ShowWindow(SW_HIDE);
}

BOOL CObjectView::PreCreateWindow(CREATESTRUCT& cs)
{
	cs.style |= WS_THICKFRAME;
	cs.dwExStyle |= (WS_EX_CLIENTEDGE|WS_EX_TOOLWINDOW);
	cs.lpszClass  = AfxRegisterWndClass(CS_HREDRAW|CS_VREDRAW, NULL,
							(HBRUSH)::GetStockObject(WHITE_BRUSH), NULL);

	return CWnd::PreCreateWindow(cs);
}

void CObjectView::OnPaint()
{
	CPaintDC dc(this); // device context for painting

	if (m_pItem == NULL) return;

	CRect rect;
	if (m_isNewItem) {
		m_isNewItem = false;

		CSize size(100, 100);
		if (SUCCEEDED(m_pItem->GetExtent(&size, m_pItem->m_nDrawAspect)))
			dc.HIMETRICtoLP(&size);

		size.cx += (2*(GetSystemMetrics(SM_CXEDGE)+GetSystemMetrics(SM_CXSIZEFRAME)));
		size.cy += (2*(GetSystemMetrics(SM_CYEDGE)+GetSystemMetrics(SM_CYSIZEFRAME))+
							GetSystemMetrics(SM_CYSMCAPTION));
		GetWindowRect(rect);
		rect.right  = rect.left + size.cx;
		rect.bottom = rect.top  + size.cy;
		MoveWindow(rect);
	}

	GetClientRect(rect);
	m_pItem->Draw(&dc, rect);
}
