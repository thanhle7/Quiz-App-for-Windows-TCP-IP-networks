/*
	This source code is given to people who want to improve their C++ language programming skill.
	It may be used for educational purposes but not for others.
	No guaranties are supplied along this source code.

	Some Libraries and programming techniques in this source code can be found in the book:

		"Lap trinh Windows voi MFC - VC++ 6.0, Le Ngoc Thanh, NXB Thong Ke 2002"

	PS : This source code is uncompatible with later versions of EmpTest from LNT/EMPsoft,
	     and uQuiz from usewide dev.
*/
#pragma once


// CObjectView

class CObjectView : public CWnd
{
public:
	CObjectView();
	virtual ~CObjectView();
	void AdjustObjView(CRichEditCntrItem* pItem);
	void DisplayView(CRichEditCntrItem* pItem);
	afx_msg void OnClose();
	afx_msg void OnPaint();

protected:
	bool m_isNewItem;
	CRichEditCntrItem* m_pItem;
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
	DECLARE_MESSAGE_MAP()
};
