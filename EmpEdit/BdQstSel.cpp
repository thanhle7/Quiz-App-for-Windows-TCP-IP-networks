// BdQstSel.cpp : implementation file
//

#include "stdafx.h"
#include "EmpED.h"
#include "BdQstSel.h"


// CBldQstSel dialog

IMPLEMENT_DYNAMIC(CBldQstSel, CDialog)

CBldQstSel::CBldQstSel(CWnd* pParent, pFileListItem pHeaderItem)
	: CDialog(CBldQstSel::IDD, pParent)
{
	m_pHeaderItem = pHeaderItem;
	m_sophantu = m_tongsocau = 0;
	m_list.Setparameters(NULL, &m_sophantu, m_fa, &m_tongsocau, IDC_QSTSEG_SOCAUCHON);
}

CBldQstSel::~CBldQstSel()
{
	for (int i=0; i<m_sophantu; i++)
		delete m_fa[i];
}

void CBldQstSel::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_QSTSEG_LIST, m_list);
}


BEGIN_MESSAGE_MAP(CBldQstSel, CDialog)
	ON_BN_CLICKED(IDOK, OnBnClickedOk)
END_MESSAGE_MAP()


// CBldQstSel message handlers

BOOL CBldQstSel::OnInitDialog()
{
	CDialog::OnInitDialog();

	int i;
	m_tongsocau = 0;
	for (i=0; i<TOPLEVEL; i++) {
		if (m_pHeaderItem->caumuc[i] > 0) {
			m_list.AddPropItem(-2,i,m_pHeaderItem->nhommuc[i], m_pHeaderItem->caumuc[i],
															m_pHeaderItem->caumucchon[i]);
			m_tongsocau = m_tongsocau + m_pHeaderItem->caumucchon[i];
		}
	}
	SetDlgItemInt(IDC_QSTSEG_SOCAUCHON, m_tongsocau);
	
	return TRUE;  // return TRUE unless you set the focus to a control
}

void CBldQstSel::OnBnClickedOk()
{
	int i, level;
	m_pHeaderItem->socauchon = m_tongsocau;
	for (i=0; i<m_sophantu; i++) {
		level = m_fa[i]->somuc;
		m_pHeaderItem->caumucchon[level] = m_fa[i]->socauchon;
	}

	OnOK();
}
