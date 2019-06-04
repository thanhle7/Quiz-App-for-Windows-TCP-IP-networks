#pragma once
#include "filelist.h"

// CBldQstSel dialog

class CBldQstSel : public CDialog
{
	DECLARE_DYNAMIC(CBldQstSel)

public:
	CBldQstSel(CWnd* pParent, pFileListItem pHeaderItem);   // standard constructor
	virtual ~CBldQstSel();

// Dialog Data
	enum { IDD = IDD_QSTSEGMENT };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	CSelectList		m_list;
	int				m_sophantu;
	WORD			m_tongsocau;
	pFileListItem	m_fa[TOPLEVEL], m_pHeaderItem;

	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL OnInitDialog();
	afx_msg void OnBnClickedOk();
};
