// EmpDoc.cpp : implementation of the CEmpDoc class
//

#include "stdafx.h"
#include "Emp.h"

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
		// NOTE - the ClassWizard will add and remove mapping macros here.
		//    DO NOT EDIT what you see in these blocks of generated code!
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
}

CEmpDoc::~CEmpDoc()
{
}

BOOL CEmpDoc::OnNewDocument()
{
	if (!CRichEditDoc::OnNewDocument())
		return FALSE;
	return TRUE;
}

CRichEditCntrItem* CEmpDoc::CreateClientItem(REOBJECT* preo) const
{
	return new CEmpItem(preo, (CEmpDoc*) this);
}
