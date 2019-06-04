//
// Resource Requirements:
//		Bitmap resources: IDB_PREV_* (see resource file)
//		Dialog resource:  IDD_PREVIEW
//		String resources: AFX_ID_PREVIEW_* (see resource file)
//
// Other requirements:
//		MBButton.cpp, .h
//
// Usage: 
//		1. #include "Preview.h" into your view class
//		2. Add Preview.cpp and MBButton.cpp to your project
//		3. Copy the above resources to your .RC file
//		4. Add the following message handler to your view class
//		   When called you get the improved print preview
//
//void CYourView::OnFileImprovedprintpreview() 
//{
//	// need to use CPreview class
//	CPrintPreviewState* pState = new CPrintPreviewState;
//
//	if (!DoPrintPreview(IDD_PREVIEW, this, RUNTIME_CLASS(CPreview), pState))
//	{
//		TRACE0("Error: OnFileImprovedprintpreview failed.\n");
//		AfxMessageBox(AFX_IDP_COMMAND_FAILURE);
//		delete pState;      // preview failed to initialize, delete State now
//	}
//}


#include "stdafx.h"
#include "afxpriv.h"
#include "afxres.h"
#include "Preview.h"
#include "resource.h"

IMPLEMENT_DYNCREATE(CMyPreviewView, CPreviewView)

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CMyPreviewView


CMyPreviewView::CMyPreviewView()
{
}

CMyPreviewView::~CMyPreviewView()
{
}

void CMyPreviewView::OnDisplayPageNumber(UINT nPage, UINT nPagesDisplayed)
{
	CPreviewView::OnDisplayPageNumber( nPage, nPagesDisplayed );
}

void CMyPreviewView::OnPreviewPrint()
{
	CPreviewView::OnPreviewPrint();
}

BEGIN_MESSAGE_MAP(CMyPreviewView, CPreviewView)
	//{{AFX_MSG_MAP(CMyPreviewView)
	ON_WM_CREATE()
	//}}AFX_MSG_MAP
	ON_COMMAND(AFX_ID_PREVIEW_PRINT, OnPreviewPrint)
	ON_UPDATE_COMMAND_UI(AFX_ID_PREVIEW_NUMPAGE, OnUpdateNumPageChange)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CMyPreviewView drawing

/////////////////////////////////////////////////////////////////////////////
// CMyPreviewView diagnostics

#ifdef _DEBUG
void CMyPreviewView::AssertValid() const
{
	CPreviewView::AssertValid();
}

void CMyPreviewView::Dump(CDumpContext& dc) const
{
	CPreviewView::Dump(dc);
}
#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// CMyPreviewView message handlers

void CMyPreviewView::OnUpdateNumPageChange(CCmdUI* pCmdUI)
{
	UINT nPages = m_nZoomState == ZOOM_OUT ? m_nPages : m_nZoomOutPages;

	if (m_bOne) {
		if (nPages == 1) {
			// need to swap to show 2 pages
			m_onetwo.LoadBitmap( IDB_PREV_TWO );
			m_bOne = FALSE;
			m_onetwo.Invalidate();
			}
		}
	else {
		if (nPages != 1) {
			// need to swap to show 1 page
			m_onetwo.LoadBitmap( IDB_PREV_ONE );
			m_bOne = TRUE;
			m_onetwo.Invalidate();
			}
		}

	// enable it only if valid to display another page and not zoomed
	pCmdUI->Enable(m_nZoomState == ZOOM_OUT && m_nMaxPages != 1 &&
		(m_pPreviewInfo->GetMaxPage() > 1 || m_nPages > 1));
}

int CMyPreviewView::OnCreate(LPCREATESTRUCT lpCreateStruct) 
{
	if (CPreviewView::OnCreate(lpCreateStruct) == -1)
		return -1;

	m_pToolBar->EnableToolTips( TRUE );

	m_bOne = FALSE;		 // the default is to show 2 pages, set in the dialog text

	m_print.AutoLoad( AFX_ID_PREVIEW_PRINT, m_pToolBar, IDB_PREV_PRINT );
	m_next.AutoLoad( AFX_ID_PREVIEW_NEXT, m_pToolBar, IDB_PREV_NEXT );
	m_previous.AutoLoad( AFX_ID_PREVIEW_PREV, m_pToolBar, IDB_PREV_PREVIOUS );
	m_onetwo.AutoLoad( AFX_ID_PREVIEW_NUMPAGE, m_pToolBar, IDB_PREV_TWO );
	m_zoomIn.AutoLoad( AFX_ID_PREVIEW_ZOOMIN, m_pToolBar, IDB_PREV_ZOOMIN );
	m_zoomOut.AutoLoad( AFX_ID_PREVIEW_ZOOMOUT, m_pToolBar, IDB_PREV_ZOOMOUT );
	
	return 0;
}
