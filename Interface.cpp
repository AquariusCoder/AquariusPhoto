#include "StdAfx.h"
#include "Interface.h"
#include "AquariusPhotoDoc.h"


IOperElement* COperElementBase::Prev()
{
	return m_prev;
}

IOperElement* COperElementBase::Next()
{
	return m_next;
}

int COperElementBase::GetID()
{
	return m_nID;
}

void COperElementBase::SetPrev(IOperElement* pElem)
{
	m_prev = pElem;
	m_nID = pElem->GetID() + 1;
}

void COperElementBase::SetNext(IOperElement* pElem)
{
	m_next = pElem;
}

COperElementBase::COperElementBase(void)
{
	m_nID = 0;
	m_prev = NULL;
	m_next = NULL;
}

void CToolBase::AdjustPosition(CPoint& point, CRect& rc)
{
	if (point.x > rc.Width())
		point.x = rc.Width();

	if (point.y > rc.Height())
		point.y = rc.Height();

	if (point.x < 0)
		point.x = 0;

	if (point.y < 0)
		point.y = 0;
}

void CToolBase::OnLButtonDown(CView* pView, UINT nFlags, CPoint& point)
{
	CAquariusPhotoDoc* pDoc = dynamic_cast<CAquariusPhotoDoc *>(pView->GetDocument());
	ASSERT(pDoc);

	CRect rc;
	if (!pDoc->GetDrawImageRect(rc))
		return;

	AdjustPosition(point, rc);

	m_firstPoint = point;
	m_bFirstDown = TRUE;
}

void CToolBase::OnLButtonUp(CView* pView, UINT nFlages, CPoint& point)
{
	if(!m_bFirstDown)
		return;

	CAquariusPhotoDoc* pDoc = dynamic_cast<CAquariusPhotoDoc *>(pView->GetDocument());
	ASSERT(pDoc);

	CRect rc;
	if (!pDoc->GetDrawImageRect(rc))
		return;

	AdjustPosition(point, rc);

	m_lastPoint = point;

	m_bFirstDown = FALSE;
}

void CToolBase::OnMouseMove(CView* pView, UINT nFlages, CPoint& point)
{
	if(!m_bFirstDown)
		return;

	CAquariusPhotoDoc* pDoc = dynamic_cast<CAquariusPhotoDoc *>(pView->GetDocument());
	ASSERT(pDoc);

	CRect rc;
	if (!pDoc->GetDrawImageRect(rc))
		return;

	AdjustPosition(point, rc);

	m_lastPoint = point;
}

CToolBase::CToolBase(void)
{
	m_bFirstDown = FALSE;
}

CShapeBase::CShapeBase(void)
{
	m_bErased = FALSE;
	m_bSelected = FALSE;
	m_nHandleInflate = 3;
	m_pHandlePen = new Pen(Color::Black, 1);
	m_pHandleBrush = new SolidBrush(Color::White);

	// TODO pen width
	m_penwidth.x = 2;
	m_penwidth.y = 2;
	m_ppen = new Pen(Color::Black, 2);
	
}


CShapeBase::~CShapeBase(void)
{
	if (m_pHandleBrush)
		delete m_pHandleBrush;

	if(m_pHandlePen)
		delete m_pHandlePen;
}

BOOL CShapeBase::IsErased()
{
	return m_bErased;;
}

BOOL CShapeBase::IsSelected()
{
	return m_bSelected;
}

void CShapeBase::SetSelected(BOOL bSel)
{
	m_bSelected = bSel;
}

BOOL CShapeBase::DrawHandle(Graphics* pGraphics)
{
	CRect rc;
	int nCount = GetHandleCount();
	for (int i = 0; i < nCount; i++)
	{
		GetHandleRect(i, rc);
		pGraphics->FillEllipse(m_pHandleBrush, Rect(rc.left, rc.top, rc.Width(), rc.Height()));
		pGraphics->DrawEllipse(m_pHandlePen, Rect(rc.left, rc.top, rc.Width(), rc.Height()));
	}
	return TRUE;
}

