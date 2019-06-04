// EmpDoc.cpp : implementation of the CEmpDoc class
//

#include "stdafx.h"
#include "EmpEd.h"
#include "empdoc.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CEmpDoc

IMPLEMENT_DYNCREATE(CEmpDoc, CRichEditDoc)

BEGIN_MESSAGE_MAP(CEmpDoc, CRichEditDoc)
	//{{AFX_MSG_MAP(CEmpDoc)
	//}}AFX_MSG_MAP
	// Enable default OLE container implementation
	ON_UPDATE_COMMAND_UI(ID_OLE_EDIT_LINKS, CRichEditDoc::OnUpdateEditLinksMenu)
	ON_COMMAND(ID_OLE_EDIT_LINKS, CRichEditDoc::OnEditLinks)
	ON_UPDATE_COMMAND_UI_RANGE(ID_OLE_VERB_FIRST, ID_OLE_VERB_LAST, CRichEditDoc::OnUpdateObjectVerbMenu)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CEmpDoc construction/destruction

CEmpDoc::CEmpDoc()
{
	m_wndObjView = NULL;
}

CEmpDoc::~CEmpDoc()
{
}

BOOL CEmpDoc::OnNewDocument()
{
	if (!CRichEditDoc::OnNewDocument())
		return FALSE;
	CEmpView* view = GetView();
	view->PrintPrepare = view->isPrinting = FALSE;
	return TRUE;
}

CRichEditCntrItem* CEmpDoc::CreateClientItem(REOBJECT* preo) const
{
	CEmpItem*	tuc;
	tuc = new CEmpItem(preo, (CEmpDoc*) this);
	return tuc;
}

/////////////////////////////////////////////////////////////////////////////
// CEmpDoc commands

BOOL CEmpDoc::OnOpenDocument(LPCTSTR lpszPathName) 
{
	char tmpID[EmpIDL];
	CFileException fe;
	CFile* pFile = GetFile(lpszPathName,
		CFile::modeRead|CFile::shareDenyWrite, &fe);
	if (pFile == NULL) return FALSE;

	DeleteContents();
	SetModifiedFlag(TRUE);

	CEmpView* pView = GetView();

	CArchive loadArchive(pFile, CArchive::load | CArchive::bNoFlushOnDelete);
	loadArchive.m_pDocument = this;
	loadArchive.m_bForceFlat = FALSE;

	TRY
	{
		CWaitCursor wait;
		if (pView->isNoBackgrndProcess) 
			pView->ShowWindow(SW_HIDE);
		if (pFile->GetLength() != 0) {
			pFile->Read(tmpID, EmpIDL);
			if (strncmp(EmpID, tmpID, EmpIDL)==0) {
				pView->DoLoad(loadArchive);
			}
			else
				pView->MessageBox("Unsupported Document by LNTsoft Editor", 
									  "Format error", 
									  MB_OK|MB_ICONEXCLAMATION);
		}
		loadArchive.Close();
		ReleaseFile(pFile, FALSE);
	}

	CATCH_ALL(e)
	{
		ReleaseFile(pFile, TRUE);
		e->ReportError();
		e->Delete();
		if (pView->isNoBackgrndProcess)
			pView->ShowWindow(SW_SHOW);
		return FALSE;
	}
	END_CATCH_ALL
	SetModifiedFlag(FALSE);
	if (pView->isNoBackgrndProcess) {
		pView->GetRichEditCtrl().SetSel(0,0);
		pView->SetFocus();
	}
	return TRUE;
}

BOOL CEmpDoc::OnSaveDocument(LPCTSTR lpszPathName) 
{
	CFileException	fe;
	CFile			*pFile = NULL;
	CEmpView		*view = GetView();

	pFile = GetFile(lpszPathName, CFile::modeCreate |
		CFile::modeReadWrite | CFile::shareExclusive, &fe);

	if (pFile == NULL)
	{
		ReportSaveLoadException(lpszPathName, &fe,
			TRUE, AFX_IDP_INVALID_FILENAME);
		return FALSE;
	}

	CArchive saveArchive(pFile, CArchive::store | CArchive::bNoFlushOnDelete);
	saveArchive.m_pDocument = this;
	saveArchive.m_bForceFlat = FALSE;
	TRY
	{
		CWaitCursor wait;
		if (view->isNoBackgrndProcess)
			view->ShowWindow(SW_HIDE);

		pFile->Write(EmpID, EmpIDL);
		view->DoSave(saveArchive);
		saveArchive.Close();
		ReleaseFile(pFile, FALSE);

		if (view->isNoBackgrndProcess)
			view->ShowWindow(SW_SHOW);
	}

	CATCH_ALL(e)
	{
		ReleaseFile(pFile, TRUE);
		e->ReportError(MB_OK);
		e->Delete();
		return FALSE;	// fail
	}
	END_CATCH_ALL

	SetModifiedFlag(FALSE);
	view->SetFocus();
	return TRUE;        // success
}

CEmpView* CEmpDoc::GetView()
{
	CRichEditView* cv = CRichEditDoc::GetView();
	ASSERT(cv->IsKindOf(RUNTIME_CLASS(CEmpView)));
	return (CEmpView*) cv;
}

void CEmpDoc::RemoveItem(CDocItem* pItem)
{
	if (m_wndObjView != NULL)
		m_wndObjView->AdjustObjView((CRichEditCntrItem*) pItem);

	CRichEditDoc::RemoveItem(pItem);
}
