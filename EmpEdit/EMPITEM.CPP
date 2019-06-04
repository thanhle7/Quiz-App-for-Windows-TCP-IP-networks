// EmpItem.cpp : implementation of the CEmpItem class
//

#include "stdafx.h"
#include "EmpED.h"
#include "empitem.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CEmpItem implementation

IMPLEMENT_SERIAL(CEmpItem, CRichEditCntrItem, 0)

CEmpItem::CEmpItem(REOBJECT* preo, CEmpDoc* pContainer)
	: CRichEditCntrItem(preo, pContainer)
{
}

CEmpItem::~CEmpItem()
{
}
