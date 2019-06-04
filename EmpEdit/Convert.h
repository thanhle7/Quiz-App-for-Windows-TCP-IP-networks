#if !defined(AFX_CONVERT_H__734DD1B1_78DD_43EC_87D0_50E467423A54__INCLUDED_)
#define AFX_CONVERT_H__734DD1B1_78DD_43EC_87D0_50E467423A54__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// Convert.h : header file
//
/////////////////////////////////////////////////////////////////////////////
// CConvert dialog

class CConvert : public CDialog
{
// Construction
public:
	CConvert(CEmpView* pParent);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CConvert)
	enum { IDD = IDD_CONVERTDLG };
		// NOTE: the ClassWizard will add data members here
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CConvert)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	CEmpView* myView;
	BOOL ToZmp;

	void WriteTextAll(CFile* fs, pSubAns& p);
	void WriteAllEnd(CFile* fs, WORD fNo, pCF sF, pCF eF);
	void LineProcess(char* s, long& cSpos, WORD& fNo, pCF& sF, pCF& eF);
	void WriteOutCf(CFile* fs, pCF sf, pCF ef, BOOL color);
	void MakeEmp(CFile* fin, CFile* fout, char* s);
	void MakeZmp(CFile* fin, CFile* fout, char* s);
	void ShowCheck(void);

	// Generated message map functions
	//{{AFX_MSG(CConvert)
	afx_msg void OnCvtSrcfiles();
	afx_msg void OnChangeCvtSrcfile();
	afx_msg void OnCvtEmpfile();
	afx_msg void OnCvtZmpfile();
	virtual BOOL OnInitDialog();
	afx_msg void OnCvtDstfiles();
	virtual void OnOK();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_CONVERT_H__734DD1B1_78DD_43EC_87D0_50E467423A54__INCLUDED_)
