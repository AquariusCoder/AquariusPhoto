#include "StdAfx.h"
#include "ColorButton.h"

CColorButton::CColorButton(void)
{
	m_pBuddy = NULL;
	m_bSel = FALSE;
	m_bForeground = TRUE;
}

CColorButton::~CColorButton(void)
{
}

void CColorButton::OnPaint()
{
	CButton::OnPaint();

	CRect rc;
	GetWindowRect(&rc);

	SolidBrush sBrush(m_color);
	Bitmap bmp(rc.Width(), rc.Height());
	Graphics* pGpBmp = Graphics::FromImage(&bmp);
	pGpBmp->FillRectangle(&sBrush, Rect(3, 3, rc.Width() - 6, rc.Height() - 6));
	
	// draw select
	GraphicsPath gpPath;
	SolidBrush selBrush(m_bSel ? Color::Gray : Color::White);
	gpPath.AddRectangle(Rect(0, 0, rc.Width(), rc.Height()));
	gpPath.AddRectangle(Rect(2, 2, rc.Width() - 4, rc.Height() - 4));

	pGpBmp->FillPath(&selBrush, &gpPath);
	

	CDC* pDC = GetDC();
	Graphics gp(pDC->GetSafeHdc());
	gp.DrawImage(&bmp, 0, 0);

	ReleaseDC(pDC);
	delete pGpBmp;
}


BEGIN_MESSAGE_MAP(CColorButton, CButton)
	//{{AFX_MSG_MAP(CColorButton)
	ON_WM_PAINT()
	ON_WM_LBUTTONDOWN()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

void CColorButton::SetColor(Color& clr)
{
	m_color = clr;
}

void CColorButton::SetBuddy(CColorButton* pBuddy)
{
	m_pBuddy = pBuddy;
}

void CColorButton::BindBuddy(CColorButton* pBuddy)
{
	SetBuddy(pBuddy);
	pBuddy->SetBuddy(this);

	m_bForeground = TRUE;
	pBuddy->m_bForeground = FALSE;

	m_bSel = TRUE;
	pBuddy->m_bSel = FALSE;
}

void CColorButton::OnLButtonDown(UINT nFlags, CPoint point)
{
	m_bSel = TRUE;
	m_pBuddy->m_bSel = FALSE;

	InvalidateWithBuddy(FALSE);

	CButton::OnLButtonDown(nFlags, point);

}

BOOL CColorButton::IsSelected()
{
	return m_bSel;
}

void CColorButton::InvalidateWithBuddy(BOOL bErase /*= TRUE*/)
{
	if(m_bForeground)
	{
		m_pBuddy->SendMessage(WM_PAINT); // wait
		Invalidate(bErase);
	}
	else
	{
		SendMessage(WM_PAINT);
		m_pBuddy->Invalidate(bErase);
	}
}


