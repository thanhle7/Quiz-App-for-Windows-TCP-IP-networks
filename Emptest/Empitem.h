// EmpItem.h : interface of the CEmpItem class
//

#if !defined(AFX_EMPITEM_H__676FA39F_3527_11D4_BB67_0008C7331CD4__INCLUDED_)
#define AFX_EMPITEM_H__676FA39F_3527_11D4_BB67_0008C7331CD4__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

class CEmpDoc;
class CEmpView;

class CEmpItem : public CRichEditCntrItem
{
// Constructors
public:
	CEmpItem(REOBJECT* preo = NULL, CEmpDoc* pContainer = NULL);

// Attributes
public:
	CEmpDoc* GetDocument()
		{ return (CEmpDoc*)CRichEditCntrItem::GetDocument(); }
	CEmpView* GetActiveView()
		{ return (CEmpView*)CRichEditCntrItem::GetActiveView(); }

	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CEmpItem)
	public:
	protected:
	//}}AFX_VIRTUAL

// Implementation
public:
	~CEmpItem();
protected:
	DECLARE_SERIAL(CEmpItem)
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_EMPITEM_H__676FA39F_3527_11D4_BB67_0008C7331CD4__INCLUDED_)
