#include "StdAfx.h"
#include "Element_Draw_Line.h"
#include "GeometryAlgorithm.h"

CElement_Draw_Line::CElement_Draw_Line(CPoint& p1, CPoint& p2)
{
	m_p1 = p1;
	m_p2 = p2;
}

CElement_Draw_Line::~CElement_Draw_Line(void)
{
}

BOOL CElement_Draw_Line::Do(Image** ppImg)
{
	Graphics* pGraphics = Graphics::FromImage(*ppImg);

	DrawShape(pGraphics);

	delete pGraphics;

	return TRUE;
}

BOOL CElement_Draw_Line::DrawShape(Graphics* pGraphics)
{
	// draw shape
	pGraphics->DrawLine(m_ppen, m_p1.x, m_p1.y, m_p2.x, m_p2.y);

	// draw handle
	if(IsSelected())
		DrawHandle(pGraphics);

	return TRUE;
}

BOOL CElement_Draw_Line::HitTest(CPoint& pos)
{
	CRgn rgn;
	::GetLineBounds(rgn, m_p1, m_p2, m_penwidth);
	return rgn.PtInRegion(pos);
}

BOOL CElement_Draw_Line::GetEnvelope(CRect& rc)
{
	::SetRectByPoint(m_p1.x, m_p1.y, m_p2.x, m_p2.y, rc);

	return TRUE;
}

int CElement_Draw_Line::HandleHitTest(CPoint& pos)
{
	CRect rc;
	GetHandleRect(0, rc);
	if (::PtInRect(rc, pos))
		return 0;

	GetHandleRect(1, rc);
	if (::PtInRect(rc, pos))
		return 1;

	return -1;
}

int CElement_Draw_Line::GetHandleCount()
{
	return 2;
}

BOOL CElement_Draw_Line::GetHandle(int nIndex, CPoint& pos)
{
	if(nIndex == 0)
		pos = m_p1;
	else if (nIndex == 1)
		pos = m_p2;
	else
		return FALSE;

	return TRUE;
}

BOOL CElement_Draw_Line::GetHandleRect(int nIndex, CRect& rc)
{
	CPoint pos;
	if (!GetHandle(nIndex, pos))
		return FALSE;

	rc.SetRect(pos.x - m_nHandleInflate, pos.y - m_nHandleInflate, pos.x + m_nHandleInflate, pos.y + m_nHandleInflate);
	return TRUE;
}

void CElement_Draw_Line::Erase(BOOL bErase /*= TRUE*/)
{
	if (bErase)
		delete this;
	else
		m_bErased = TRUE;
}

