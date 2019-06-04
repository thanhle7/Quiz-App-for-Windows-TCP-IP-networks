// EmpQView.cpp : implementation file
//

#include "stdafx.h"
#include "emp.h"
#include "EmpQView.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CEmpQstsView

IMPLEMENT_DYNCREATE(CEmpQstsView, CListView)

CEmpQstsView::CEmpQstsView()
{
	m_smallbmp = NULL;
}

CEmpQstsView::~CEmpQstsView()
{
	if (m_smallbmp != NULL) {
		m_smallbmp->DeleteImageList();
		delete m_smallbmp;
	}
}


BEGIN_MESSAGE_MAP(CEmpQstsView, CListView)
	//{{AFX_MSG_MAP(CEmpQstsView)
	ON_NOTIFY_REFLECT(NM_CLICK, OnClick)
	ON_NOTIFY_REFLECT(NM_RCLICK, OnRclick)
	ON_WM_KEYDOWN()
	ON_WM_SETCURSOR()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CEmpQstsView drawing

void CEmpQstsView::OnDraw(CDC* pDC)
{
}

/////////////////////////////////////////////////////////////////////////////
// CEmpQstsView message handlers

BOOL CEmpQstsView::PreCreateWindow(CREATESTRUCT& cs) 
{
	cs.style = WS_CHILD|WS_VISIBLE|LVS_SINGLESEL|LVS_NOCOLUMNHEADER|LVS_SMALLICON|LVS_SHOWSELALWAYS;

	return CListView::PreCreateWindow(cs);
}

void CEmpQstsView::OnClick(NMHDR* pNMHDR, LRESULT* pResult) 
{
	if (m_frame->ptest->qstNo > 0) {
		ItemChanged();
	}
	*pResult = 0;
}

void CEmpQstsView::OnRclick(NMHDR* pNMHDR, LRESULT* pResult) 
{
	*pResult = 0;
}

void CEmpQstsView::OnInitialUpdate() 
{
	CListView::OnInitialUpdate();

	if (m_smallbmp == NULL) {
		CListCtrl&	lctrl = GetListCtrl();
		m_frame = (CMainFrame*) AfxGetApp()->m_pMainWnd;

		m_smallbmp = new CImageList();
		m_smallbmp->Create(IDB_QSTSBMP, 16, 1, RGB(200,200,200));
		lctrl.SetImageList(m_smallbmp, LVSIL_SMALL);
		lctrl.SetBkColor(RGB(0,162,162));
		lctrl.SetTextColor(RGB(0,0,132));
		lctrl.SetTextBkColor(RGB(0,162,162));
	}
}

void CEmpQstsView::ViewAnswer(int alen, int mark)
{
	int			vi;
	char		s[7];
	CListCtrl&	lctrl = GetListCtrl();

	lctrl.DeleteAllItems();
	for (vi=0; vi<alen; vi++) {
		sprintf(s, "%d", vi+1);
		lctrl.InsertItem(vi, s, mark);
	}
	lctrl.SetItemState(0,LVIS_FOCUSED|LVIS_SELECTED,LVIS_FOCUSED|LVIS_SELECTED);
	lctrl.EnsureVisible(0, FALSE);
}

void CEmpQstsView::SeleItem(int iNo, int Mark)
{
	CListCtrl& lc = GetListCtrl();
	lc.SetItemState(iNo,LVIS_FOCUSED|LVIS_SELECTED,LVIS_FOCUSED|LVIS_SELECTED);
	if (Mark > -1) lc.SetItem(iNo, 0, LVIF_IMAGE, "", Mark+1,0,0,0); // MarkImage=0 is '?'
	lc.EnsureVisible(iNo, FALSE);
}

void CEmpQstsView::ItemChanged()
{
	CListCtrl& lctrl = GetListCtrl();
	POSITION pos = lctrl.GetFirstSelectedItemPosition();
	if (pos != NULL) {
		m_frame->ptest->cur_qst = lctrl.GetNextSelectedItem(pos);
		m_frame->ptest->GotoQuestion(m_frame->ptest->cur_qst);
	}
}


void CEmpQstsView::OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags) 
{
	CListView::OnKeyDown(nChar, nRepCnt, nFlags);

	switch (nChar) {
	case VK_UP:
	case VK_DOWN:
	case VK_HOME:
	case VK_END:
	case 33:
	case 34:
		ItemChanged();
	}

}

void CEmpQstsView::OnActivateView(BOOL bActivate, CView* pActivateView, CView* pDeactiveView) 
{
	if (bActivate)
		m_frame->GotoPane(ID_QUESTION, FALSE);
//	CListView::OnActivateView(bActivate, pActivateView, pDeactiveView);
}

BOOL CEmpQstsView::OnSetCursor(CWnd* pWnd, UINT nHitTest, UINT message) 
{
	switch (nHitTest) {
	case HTCLIENT:
		SetCursor(m_frame->mypointcursor);
		return 1;
	case HTVSCROLL:
		SetCursor(m_frame->mypullcursor);
		return 1;
	}
	return CListView::OnSetCursor(pWnd, nHitTest, message);
}
