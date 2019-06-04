// EmpDoc.h : interface of the CEmpDoc class
//
/////////////////////////////////////////////////////////////////////////////

#if !defined(AFX_EMPDOC_H__F8E44EB4_5BF3_47C2_9ECA_AB9E95983E11__INCLUDED_)
#define AFX_EMPDOC_H__F8E44EB4_5BF3_47C2_9ECA_AB9E95983E11__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

class CEmpDoc : public CRichEditDoc
{
protected: // create from serialization only
	CEmpDoc();
	DECLARE_DYNCREATE(CEmpDoc)

// Attributes
public:
	CObjectView* m_wndObjView;

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CEmpDoc)
	public:
	virtual BOOL OnNewDocument();
	virtual BOOL OnOpenDocument(LPCTSTR lpszPathName);
	virtual BOOL OnSaveDocument(LPCTSTR lpszPathName);
	//}}AFX_VIRTUAL
	virtual CRichEditCntrItem* CreateClientItem(REOBJECT* preo) const;

// Implementation
public:
	CEmpView* GetView();
	virtual ~CEmpDoc();
	virtual void RemoveItem(CDocItem* pItem);
protected:
// Generated message map functions
protected:
	//{{AFX_MSG(CEmpDoc)
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_EMPDOC_H__F8E44EB4_5BF3_47C2_9ECA_AB9E95983E11__INCLUDED_)
