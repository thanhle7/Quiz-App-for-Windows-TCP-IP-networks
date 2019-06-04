// MappedBitmapButton.h : header file
//

#ifndef __MappedBitmapButton_h__
#define __MappedBitmapButton_h__

/////////////////////////////////////////////////////////////////////////////
// CMappedBitmapButton window

// Usage -- a replacement for the MFC CBitmapButton
// 1. include an owner-draw button in your dialog
// 2. declare a CMappedBitmapButton member in the CDialog code
// 3. hook in the CMappedBitmapButton using a call to AutoLoad
//
// the bitmap resource specified in AutoLoad must be divisable into
// 4 equally sized images that represent (left to right) the
// up, down, focused and disabled states of the button

class CMappedBitmapButton : public CButton
{
// Construction
public:
DECLARE_DYNAMIC( CMappedBitmapButton )
	CMappedBitmapButton();

// Attributes
public:

protected:
	CImageList	m_image;
	UINT		m_idResource;

// Operations
public:
	BOOL LoadBitmap( UINT idBitmapResource );
	void SizeToContent();
	BOOL AutoLoad(UINT nID, CWnd* pParent, UINT idBitmapResource);

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CMappedBitmapButton)
	public:
	virtual void DrawItem(LPDRAWITEMSTRUCT lpDrawItemStruct);
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~CMappedBitmapButton();

	// Generated message map functions
protected:
	//{{AFX_MSG(CMappedBitmapButton)
	afx_msg void OnSysColorChange();
	//}}AFX_MSG

	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

#endif //#ifndef __MappedBitmapButton_h__
