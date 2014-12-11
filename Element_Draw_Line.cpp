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

	// do not draw when changed
	if(!IsChanged())
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

IShape* CElement_Draw_Line::Clone()
{
	CElement_Draw_Line* pClone = new CElement_Draw_Line(m_p1, m_p2);
	pClone->m_bErased = m_bErased;
	pClone->m_bSelected = m_bSelected;
	pClone->m_bChanged = m_bChanged;
	//pClone->m_pParent = m_pParent;
	pClone->m_nHandleInflate = m_nHandleInflate;
	//*pClone->m_pHandleBrush = *m_pHandleBrush;
	//*pClone->m_pHandlePen = *m_pHandlePen;

	pClone->m_pParent = this;

	return pClone;
}

BOOL CElement_Draw_Line::GetHandlePtr(int nIndex, CPoint** ppHandle)
{
	*ppHandle = NULL;

	if (nIndex == 0)
		*ppHandle = &m_p1;
	else if (nIndex == 1)
		*ppHandle = &m_p2;
	else
		return FALSE;

	return TRUE;
}

void CElement_Draw_Line::OnReDo()
{
	IShape* pParent = this->GetParent();
	if (pParent != NULL)
		pParent->SetChanged(TRUE);
}

void CElement_Draw_Line::OnUnDo()
{
	IShape* pParent = this->GetParent();
	if (pParent != NULL)
		pParent->SetChanged(FALSE);
}
