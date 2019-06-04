// SymDlg.cpp : implementation file
//

#include "stdafx.h"
#include "resource.h"
#include "SymDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CSymbolDlg dialog

CSymbolDlg::CSymbolDlg(CRichEditView *pParent, CHARFORMAT2 *cf)
	: CDialog(CSymbolDlg::IDD, pParent)
{
	parent = pParent;
	pcf = cf;
	//{{AFX_DATA_INIT(CSymbolDlg)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
}

BEGIN_MESSAGE_MAP(CSymbolDlg, CDialog)
	//{{AFX_MSG_MAP(CSymbolDlg)
	ON_WM_DESTROY()
	ON_WM_PAINT()
	ON_WM_LBUTTONDOWN()
	ON_WM_LBUTTONDBLCLK()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CSymbolDlg message handlers

BOOL CSymbolDlg::OnInitDialog() 
{
	CDialog::OnInitDialog();

	// use ccol as width, crow as height
	ccol = 15 + 28*dx + 2*GetSystemMetrics(SM_CXEDGE);
	crow = 45 + 8*dy + GetSystemMetrics(SM_CYCAPTION) +
					 2*GetSystemMetrics(SM_CYEDGE);

	parent->GetWindowRect(&rt);
	if (rt.right - rt.left > ccol) rt.left += (rt.right - rt.left - ccol)/2;
	if (rt.bottom - rt.top > crow) rt.top  += (rt.bottom - rt.top - crow)/2;

	MoveWindow(rt.left, rt.top, ccol, crow, FALSE);

	GetDlgItem(IDOK)->MoveWindow(7,8*dy+18,63,25, FALSE);
	GetDlgItem(IDCANCEL)->MoveWindow(80,8*dy+18,63,25, FALSE);

	crow = ccol = 0;
	memDC = NULL;
	br.CreateSolidBrush(RGB(200,220,220));
	InsertDone = FALSE;
	return TRUE;
}

void CSymbolDlg::OnOK() 
{
	if (!InsertDone) {
		strcpy(pcf->szFaceName, SYMBOL);
		pcf->dwMask = CFM_FACE;
		parent->SetCharFormat(*pcf);
		InsertDone = TRUE;
	}
	parent->SendMessage(WM_CHAR, 32+crow*28+ccol, 0);
}

void CSymbolDlg::OnDestroy() 
{
	if (InsertDone) {
		strcpy(pcf->szFaceName, "EMP.Font");
		pcf->dwMask = CFM_FACE;
		parent->SetCharFormat(*pcf);
	}

	br.DeleteObject();
	if (memDC != NULL) {
		bigfont->DeleteObject(); delete bigfont;
		memDC->SelectObject(oldfont);
		font->DeleteObject(); delete font;
		memDC->SelectObject(oldbmp);
		bmp->DeleteObject(); delete bmp;
		memDC->DeleteDC(); delete memDC;
	}

	CDialog::OnDestroy();
}

void CSymbolDlg::OnPaint() 
{
	int		i, j;
	char	s[5];
	CDC		*me;
	PAINTSTRUCT ps;

	me = BeginPaint(&ps);
	if (memDC == NULL) {
		font= new CFont();
		memset(&lf, 0, sizeof(LOGFONT));
		lf.lfHeight = dy-2;
		strcpy(lf.lfFaceName, SYMBOL);
		font->CreateFontIndirect(&lf);

		bigfont = new CFont();
		lf.lfHeight = dy+9;
		lf.lfWeight = FW_EXTRABOLD;
		bigfont->CreateFontIndirect(&lf);
		bmp = new CBitmap();
		bmp->CreateCompatibleBitmap(me,28*dx+14,8*dy+14);
		memDC = new CDC();
		memDC->CreateCompatibleDC(me);
		oldbmp = memDC->SelectObject(bmp);
		oldfont= memDC->SelectObject(font);
		memDC->SetBkMode(TRANSPARENT);
	}

	memDC->FillRect(CRect(0,0,28*dx+14, 8*dy+14), &br);

	// ke dong
	for (i=0; i<9; i++) {
		memDC->MoveTo(7, 7+i*dy);
		memDC->LineTo(7+28*dx, 7+i*dy);
	}
	// ke cot
	for (i=0; i<29; i++) {
		memDC->MoveTo(7+i*dx, 7);
		memDC->LineTo(7+i*dx, 7+8*dy);
	}
	
	memDC->SetTextColor(RGB(0,0,0));
	for (i=0; i<8; i++) {
		rt.top = 7+i*dy; rt.bottom = rt.top + dy;
		for (j=0; j<28; j++) {
			rt.left = 7+j*dx; rt.right = rt.left + dx;
			sprintf(s, "%c", 32+i*28+j);
			memDC->DrawText(s, &rt, DT_CENTER|DT_VCENTER);
		}
	}

	memDC->SelectObject(bigfont);
	memDC->SetTextColor(RGB(200,0,200));
	rt.left  = 7+ccol*dx-5;
	rt.top   = 7+crow*dy-5;
	rt.right = rt.left + dx + 10;
	rt.bottom= rt.top + dy + 10;
	memDC->FillRect(&rt, &br);
	sprintf(s, "%c", 32+crow*28+ccol);
	memDC->DrawText(s, &rt, DT_CENTER|DT_VCENTER);
	memDC->Draw3dRect(&rt, RGB(255,255,255), RGB(90,90,90));
	memDC->SelectObject(font);

	me->StretchBlt(	0,0,28*dx+14,8*dy+14,memDC,
					0,0,28*dx+14,8*dy+14,SRCCOPY);

	EndPaint(&ps);
}

BOOL CSymbolDlg::GetRowCol(CPoint point)
{
	if (point.x < 8 || point.x > 7+28*dx ||
		point.y < 7 || point.y > 7+8*dy) return FALSE;
	ccol = (point.x - 7)/dx;
	crow = (point.y - 7)/dy;
	return TRUE;
}

BOOL CSymbolDlg::PreTranslateMessage(MSG* pMsg) 
{
	if (pMsg->message == WM_KEYDOWN) {
		switch (pMsg->wParam) {
		case VK_UP:
			if (crow > 0) crow--;
			break;
		case VK_DOWN:
			if (crow < 7) crow++;
			break;
		case VK_LEFT:
			if (ccol > 0) ccol--;
			break;
		case VK_RIGHT:
			if (ccol < 27) ccol++;
			break;
		default:
			return CDialog::PreTranslateMessage(pMsg);
		}
		Invalidate(FALSE);
		return TRUE;
	}
	return CDialog::PreTranslateMessage(pMsg);
}

void CSymbolDlg::OnLButtonDown(UINT nFlags, CPoint point) 
{
	if (GetRowCol(point)) Invalidate(FALSE);
}

void CSymbolDlg::OnLButtonDblClk(UINT nFlags, CPoint point) 
{
	if (GetRowCol(point))
		PostMessage(WM_COMMAND, IDOK, 0);
}
