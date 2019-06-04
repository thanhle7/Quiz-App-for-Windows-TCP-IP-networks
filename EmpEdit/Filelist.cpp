// PropertyList.cpp : implementation file
//

#include "stdafx.h"
#include "FileList.h"
#include "BdQstSel.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CSelectList

CSelectList::CSelectList()
{
}

CSelectList::~CSelectList()
{
}


BEGIN_MESSAGE_MAP(CSelectList, CListBox)
	//{{AFX_MSG_MAP(CSelectList)
	ON_CONTROL_REFLECT(LBN_SELCHANGE, OnSelchange)
	ON_WM_LBUTTONUP()
	ON_WM_KILLFOCUS()
	ON_WM_LBUTTONDOWN()
	ON_WM_VSCROLL()
	ON_WM_SETCURSOR()
	ON_WM_LBUTTONDBLCLK()
	//}}AFX_MSG_MAP
	ON_EN_KILLFOCUS(IDC_PROPEDITBOX, OnKillfocusEditBox)
	ON_EN_CHANGE(IDC_PROPEDITBOX, OnChangeEditBox)
	ON_BN_CLICKED(IDC_PROPBTNCTRL, OnButton)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CSelectList message handlers

void CSelectList::MeasureItem(LPMEASUREITEMSTRUCT lpMeasureItemStruct) 
{
	lpMeasureItemStruct->itemHeight = 21; //pixels
}

void CSelectList::DrawItem(LPDRAWITEMSTRUCT lpDIS) 
{
	CDC dc;
	CString sval;

	dc.Attach(lpDIS->hDC);
	CRect rectFull = lpDIS->rcItem;
	CRect rect = rectFull;
	if (m_nDivider == 0) {
		m_nDivider = rect.Width() - 2*GetSystemMetrics(SM_CXVSCROLL) 
								  - GetSystemMetrics(SM_CXDLGFRAME)
								  - dc.GetTextExtent(" 999", 3).cx;
	}
	rect.left = m_nDivider;
	CRect rect2 = rectFull;
	rect2.right = rect.left - 1;
	int nIndex = lpDIS->itemID;

	if (nIndex != -1)
	{
		//get the CPropertyItem for the current row
		pFileListItem pItem = (pFileListItem) GetItemDataPtr(nIndex);

		//draw two rectangles, one for each row column
		if (pItem->items) {
			// write the value
			if (pItem->socau > 0)
				sval.Format("%d", pItem->socauchon);
			else sval = "***";
			dc.DrawText(sval,CRect(rect.left+3,rect.top+3,
								rect.right+3,rect.bottom+3),
					DT_LEFT | DT_SINGLELINE);
			if (pItem->items > 0) 
				dc.FillSolidRect(rect2,RGB(240,240,192));
			else
				dc.FillSolidRect(rect2,RGB(250,250,220));
		}
		else
			dc.FillSolidRect(rect2,RGB(0xFF,0xFF,0xF0));
		dc.DrawEdge(rect2,EDGE_SUNKEN,BF_BOTTOMRIGHT);
		dc.DrawEdge(rect,EDGE_SUNKEN,BF_BOTTOM);

		//write the title
		if (pItem->items ==0 && pItem->sonhom > 0)
			sval.Format("%s [M:%d,N:%d,C:%d]", pItem->title, pItem->somuc, pItem->sonhom, pItem->socau);
		else sval = pItem->title;
		nIndex = rect2.Width() - 5;
		if (dc.GetTextExtent(sval).cx > nIndex) {
			sval.Insert(3, "...");
			while (dc.GetTextExtent(sval).cx > nIndex)
				sval.Delete(6);
			if (sval.GetAt(6) != '\\' && (nIndex = sval.Find("\\", 6)) > 0)
				sval.Delete(6, nIndex-6);
		}
		dc.SetBkMode(TRANSPARENT);
		dc.DrawText(sval,CRect(rect2.left+3,rect2.top+3,
											rect2.right-3,rect2.bottom+3),
					DT_LEFT | DT_SINGLELINE);
	}
	dc.Detach();
}

void CSelectList::DelPropItem(int idx) 
{
	if (idx >= *itemcount) return;
	delete pitem[idx];
	DeleteString(idx);
	while (idx < *itemcount-1) {
		pitem[idx] = pitem[idx+1];
		idx++;
	}
	*itemcount -= 1;
}

// ItemType = -1: header; =0: itemfile; =-2 : proper Item
int CSelectList::AddPropItem(int ItemType, int Mucdo, int Sonhom, int Socau, int Socauchon)
{
	int nIndex = AddString(_T(""));
	pitem[nIndex] = new FileListItem;
	SetItemDataPtr(nIndex, pitem[nIndex]);
	EmptyItem(pitem[nIndex]);

	pitem[nIndex]->items = ItemType;
	if (ItemType == -2) {
		pitem[nIndex]->somuc = Mucdo;
		pitem[nIndex]->socau = Socau;
		pitem[nIndex]->socauchon = Socauchon;
		pitem[nIndex]->title.Format("M¤c %d [%d nh¢m,%d cƒu]", Mucdo, Sonhom, Socau);
	}

	*itemcount += 1;
	return nIndex;
}

void CSelectList::OnSelchange() 
{
	CRect	rect;
	CString lBoxSelText;

	if (m_btnCtrl)
		m_btnCtrl.ShowWindow(SW_HIDE);

	GetItemRect(m_curSel,rect);
	rect.left = m_nDivider;

	pFileListItem pItem = (pFileListItem) GetItemDataPtr(m_curSel);

	if (pItem->items == -2)
	{
		rect.bottom -= 3;
		if (m_editBox)
			m_editBox.MoveWindow(rect);
		else
		{	
			m_editBox.Create(ES_NUMBER|ES_LEFT|ES_AUTOHSCROLL|WS_VISIBLE|WS_CHILD|WS_BORDER,
							rect,this,IDC_PROPEDITBOX);
			m_editBox.SetFont(m_pvnFont);
		}

		lBoxSelText.Format("%d", pItem->socauchon);

		//set the text in the edit box to the property's current value
		m_editBox.SetWindowText(lBoxSelText);
		m_editBox.ShowWindow(SW_SHOW);
		m_editBox.SetFocus();

		if (!m_spin)
			m_spin.Create(WS_CHILD|WS_VISIBLE|UDS_SETBUDDYINT|UDS_ALIGNRIGHT, 
				CRect(0,0,0,0), this, IDC_PROPSPNCTRL);	
		m_spin.SetBuddy(&m_editBox);
		m_spin.ShowWindow(SW_SHOW);
		m_spin.SetRange(0, pItem->socau);
	}
	else {
		if (pItem->items == 0 || pItem->socau > 0) 
			DisplayButton(rect);
	}
}

void CSelectList::DisplayButton(CRect region)
{
	if (region.Width() > 17)
		region.left = region.right - 17;
	region.bottom -= 3;

	if (m_btnCtrl)
		m_btnCtrl.MoveWindow(region);
	else
	{	
		m_btnCtrl.Create("...",BS_PUSHBUTTON|WS_VISIBLE|WS_CHILD,
						region,this,IDC_PROPBTNCTRL);
		m_btnCtrl.SetFont(m_pvnFont);
	}

	m_btnCtrl.ShowWindow(SW_SHOW);
	m_btnCtrl.SetFocus();
}

void CSelectList::OnKillFocus(CWnd* pNewWnd) 
{
	CListBox::OnKillFocus(pNewWnd);
}

void CSelectList::OnKillfocusEditBox()
{
	m_editBox.ShowWindow(SW_HIDE);

	m_spin.ShowWindow(SW_HIDE);

	Invalidate(FALSE);
}

void CSelectList::OnChangeEditBox()
{
	CString	newStr;
	pFileListItem pItem = (pFileListItem) GetItemDataPtr(m_curSel);

	*sel = *sel - pItem->socauchon;
	m_editBox.GetWindowText(newStr);
	sscanf(newStr, "%d", &(pItem->socauchon));

	if (pItem->socauchon > pItem->socau || pItem->socauchon < 0) {
		pItem->socauchon = pItem->socau;
		newStr.Format("%d", pItem->socauchon);
		m_editBox.SetWindowText(newStr);
	}
	*sel = *sel + pItem->socauchon;
	GetParent()->SetDlgItemInt(id, *sel);
}

void CSelectList::OnButton()
{
	if (pitem[m_curSel]->items == 0) {
		CFileDialog FileDlg(TRUE);

		FileDlg.m_ofn.Flags |= OFN_FILEMUSTEXIST|OFN_HIDEREADONLY;
		FileDlg.m_ofn.lpstrFilter = "Editor Document (*.emp)\0 *.emp\0 All Files (*.*)\0 *.*";
		FileDlg.m_ofn.lpstrTitle = "Select Editor Document Files";

		CString currPath = pitem[m_curSel]->title;
		if (currPath.GetLength() > 0)
			FileDlg.m_ofn.lpstrInitialDir = currPath.Left(
				currPath.GetLength() - currPath.ReverseFind('\\'));

		if(IDOK == FileDlg.DoModal())
		{
			m_btnCtrl.ShowWindow(SW_HIDE);
			UpdateItemFromFile(m_curSel, FileDlg.GetPathName());
			UpdateHeaderFromItems(m_curSel, -1);
		}
	}
	else {
		CBldQstSel dlg(this, pitem[m_curSel]);
		int socauchon = pitem[m_curSel]->socauchon;
		if (dlg.DoModal() == IDOK) {
			*sel += (pitem[m_curSel]->socauchon - socauchon);
			GetParent()->SetDlgItemInt(id, *sel);
			Invalidate(TRUE);
		}
	}
}

void CSelectList::OnLButtonUp(UINT nFlags, CPoint point) 
{
	int i, j, k, l;
	pFileListItem temp;

	if (*itemcount == 0) return;

	if (m_bTracking)
	{
		m_bTracking = FALSE;
		::ClipCursor(NULL);

		// sap xep
		k = GetCurLine(point);
		if (k != m_curSel && pitem[k]->items) {
			if (k > m_curSel) {
				i = m_curSel; j = k;
			}
			else {
				i = k; j = m_curSel;
			}

			l = 0;
			for (k=i; k<j; k++) {
				if (pitem[k]->items)
					l += (pitem[k]->items + 1);
			}
			j += pitem[j]->items;
			while (l--) {
				temp = pitem[i];
				for (k=i; k<j; k++)
					pitem[k] = pitem[k+1];
				pitem[k] = temp;
			}
			for (k=i; k<=j; k++)
				SetItemDataPtr(k, pitem[k]);

			Invalidate(TRUE);
		}
	}
	CListBox::OnLButtonUp(nFlags, point);
}

void CSelectList::OnLButtonDown(UINT nFlags, CPoint point) 
{
	if (*itemcount == 0) return;

	m_curSel = GetCurLine(point);

	if (point.x <= m_nDivider && pitem[m_curSel]->items > 0)
	{
		CRect rt;
		m_bTracking = TRUE;
		if (m_editBox)
			m_editBox.ShowWindow(SW_HIDE);
		if (m_btnCtrl)
			m_btnCtrl.ShowWindow(SW_HIDE);

		// keep mouse pointer in the list box boundary
		GetWindowRect(rt);
		rt.left += 10; 
		rt.right -= (10+GetSystemMetrics(SM_CXVSCROLL));

		::ClipCursor(rt);
		SetCursor(m_hCursorMove);
	}
	else m_bTracking = FALSE;

	CListBox::OnLButtonDown(nFlags, point);
}

void CSelectList::PreSubclassWindow() 
{
	m_nDivider = 0;
	m_bTracking = FALSE;

	m_hCursorMove = AfxGetApp()->LoadCursor(IDC_MOVEITEM);

	m_pvnFont = &((CMainFrame*)AfxGetApp()->m_pMainWnd)->m_vnFont;
	SetFont(m_pvnFont);
}

void CSelectList::OnVScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar) 
{
	// keep the message from spin button
	if (pScrollBar) return;

	if (m_btnCtrl)
		m_btnCtrl.ShowWindow(SW_HIDE);
	if (m_editBox)
		m_editBox.ShowWindow(SW_HIDE);

	CListBox::OnVScroll(nSBCode, nPos, pScrollBar);
}

int CSelectList::GetCurLine(CPoint point)
{
	BOOL loc;
	return ItemFromPoint(point,loc);
}

void CSelectList::Setparameters(CEmpView* pview, int* ItemCount, pFileListItem *PItem, WORD* Sel, int ID)
{
	m_pView = pview;
	pitem = PItem;
	itemcount = ItemCount;
	sel = Sel;
	id = ID;
}

BOOL CSelectList::OnSetCursor(CWnd* pWnd, UINT nHitTest, UINT message) 
{
	if (m_bTracking) return TRUE;

	return CListBox::OnSetCursor(pWnd, nHitTest, message);
}

void CSelectList::OnLButtonDblClk(UINT nFlags, CPoint point)
{
	int idx;
	if (*itemcount == 0 || pitem[m_curSel]->items == -2) return;

	idx = m_curSel;
	while (pitem[idx]->items == 0) idx--;
	if (idx == m_curSel || pitem[idx]->items == 1) {
		*sel -= pitem[idx]->socauchon;
		m_curSel = idx;	// point to delete
		idx = pitem[m_curSel]->items+1;
	}
	else {
		UpdateHeaderFromItems(idx, m_curSel);
		idx = 1;
	}

	while (idx > 0) {
		DelPropItem(m_curSel);
		idx--;
	}

	GetParent()->SetDlgItemInt(id, *sel);
	if (m_btnCtrl) m_btnCtrl.ShowWindow(SW_HIDE);
	SetFocus();
}

void CSelectList::UpdateHeaderFromItems(int idx, int except)
{
	int		jj, ii;

	while (pitem[idx]->items == 0) idx--;	// begin of group

	*sel -= pitem[idx]->socauchon;

	EmptyItem(pitem[idx], false);

	for (jj=0; jj<TOPLEVEL; jj++) {
		pitem[idx]->caumuc[jj]  = -1;	// 0xFFFF
		pitem[idx]->nhommuc[jj] = -1;	// 0xFFFF
	}

	pitem[idx]->items = 0;
	ii = idx+1;		// the first item of group
	while (ii < *itemcount && pitem[ii]->items == 0) {
		if (except == -1 || ii != except) {
			// un accepted item of group
			for (jj=0; jj < TOPLEVEL; jj++) {
				if (pitem[idx]->caumuc[jj] > pitem[ii]->caumuc[jj])
					pitem[idx]->caumuc[jj] = pitem[ii]->caumuc[jj];
				if (pitem[idx]->nhommuc[jj] > pitem[ii]->nhommuc[jj])
					pitem[idx]->nhommuc[jj] = pitem[ii]->nhommuc[jj];
			}
			pitem[idx]->items++;
		}
		ii++;
	}

	for (jj=0; jj<TOPLEVEL; jj++) {
		pitem[idx]->sonhom += pitem[idx]->nhommuc[jj];
		if (pitem[idx]->caumucchon[jj] > pitem[idx]->caumuc[jj])
			pitem[idx]->caumucchon[jj] = pitem[idx]->caumuc[jj];
		if (pitem[idx]->caumuc[jj] > 0) {
			pitem[idx]->somuc++;
			pitem[idx]->socau += pitem[idx]->caumuc[jj];
			pitem[idx]->socauchon += pitem[idx]->caumucchon[jj];
		}
	}

	if (pitem[idx]->socau > 0)
		pitem[idx]->title.Format("* Nh¢m %d b› cƒu h‘i. T™ng c›ng: %d m¤c, %d nh¢m, %d cƒu",
				pitem[idx]->items, pitem[idx]->somuc, pitem[idx]->sonhom, pitem[idx]->socau);
	else
		pitem[idx]->title.Format("* Nh¢m %d b› cƒu h‘i. Ch© s¦ džng phän già thi‰t (n‰u c¢).",
								pitem[idx]->items);

	*sel += pitem[idx]->socauchon;
	GetParent()->SetDlgItemInt(id, *sel);
	Invalidate(TRUE);
}

bool CSelectList::UpdateItemFromFile(int idx, CString& filename)
{
	CFile*	fg;
	char	empID[EmpIDL+1], Reserved, *pMucnhom;
	WORD	socau, sonhom, *pCaunhom, i;
	pFileListItem p = pitem[idx];

	fg = new CFile(filename, CFile::modeRead);
	if (fg == NULL) return false;

	socau = sonhom = i = 0;
	fg->Read(empID, EmpIDL);
	// read some needed data
	if ( fg->Read(&Reserved, char_S) )
		if ( fg->Read(&socau, WORD_S) )
			fg->Read(&i, WORD_S);
	fg->Close();
	delete fg;

	if (strncmp(EmpID, empID, EmpIDL) != 0 || 
		( HIBYTE(i) != EMPDOC_VERSION &&
			!m_pView->ValidEmpDoc( filename.GetBuffer() ) ) ) return false;

	fg = new CFile(filename, CFile::modeRead);
	if (fg == NULL) return false;

	fg->Seek(EmpIDL+1, CFile::begin);
	fg->Read(&socau, WORD_S);
	fg->Read(&i, WORD_S);
	fg->Read(&sonhom, WORD_S);

	// now update the item
	EmptyItem(p);
	p->title = filename;
	p->socau = socau;
	p->sonhom= sonhom;
	if (sonhom > 0) {
		pCaunhom = new WORD[sonhom];
		pMucnhom = new char[sonhom];
		fg->Read(pCaunhom, sonhom * WORD_S);
		fg->Read(pMucnhom, sonhom * char_S);
		// update socau, somuc, socaumuc
		for (i=0; i<sonhom; i++) {
			if (pMucnhom[i] > TOPLEVEL-1) continue;	// 0..TOPLEVEL-1
			if (pCaunhom[i] > 0) {
				if (p->caumuc[pMucnhom[i]] == 0)	// another new level
					p->somuc++;
				p->caumuc[pMucnhom[i]] = p->caumuc[pMucnhom[i]] + pCaunhom[i];
				p->nhommuc[pMucnhom[i]] += 1;
			}
		}
		delete pCaunhom;
		delete pMucnhom;
	}
	fg->Close(); delete fg;
	return true;
}

// item uses only
void CSelectList::EmptyItem(pFileListItem pItem, bool SelClear) 
{
	pItem->items	= 0;
	pItem->title.Empty();
	pItem->socau	= 0;
	pItem->sonhom	= 0;
	pItem->somuc	= 0;
	pItem->socauchon= 0;
	if (SelClear) {
		memset(pItem->caumuc, 0, TOPLEVEL * WORD_S);
		memset(pItem->nhommuc, 0, TOPLEVEL * WORD_S);
		memset(pItem->caumucchon, 0, TOPLEVEL * WORD_S);
	}
}
