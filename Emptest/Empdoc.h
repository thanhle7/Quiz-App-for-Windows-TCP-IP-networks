// EmpDoc.h : interface of the CEmpDoc class
//
/////////////////////////////////////////////////////////////////////////////

#if !defined(AFX_EMPDOC_H__676FA39B_3527_11D4_BB67_0008C7331CD4__INCLUDED_)
#define AFX_EMPDOC_H__676FA39B_3527_11D4_BB67_0008C7331CD4__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

class CEmpItem;

class CEmpDoc : public CRichEditDoc
{
protected: // create from serialization only
	CEmpDoc();
	DECLARE_DYNCREATE(CEmpDoc)

// Attributes
public:

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CEmpDoc)
	public:
	virtual BOOL OnNewDocument();
	//}}AFX_VIRTUAL
	virtual CRichEditCntrItem* CreateClientItem(REOBJECT* preo) const;

// Implementation
public:
	virtual ~CEmpDoc();

protected:

// Generated message map functions
protected:
	//{{AFX_MSG(CEmpDoc)
		// NOTE - the ClassWizard will add and remove member functions here.
		//    DO NOT EDIT what you see in these blocks of generated code !
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_EMPDOC_H__676FA39B_3527_11D4_BB67_0008C7331CD4__INCLUDED_)
