#include "StdAfx.h"
#include "Tool_SelectShape.h"
#include "GeometryAlgorithm.h"
#include "AquariusPhotoDoc.h"

CTool_SelectShape::CTool_SelectShape(void)
{
}

CTool_SelectShape::~CTool_SelectShape(void)
{
}

void CTool_SelectShape::OnLButtonUp(CView* pView, UINT nFlages, CPoint& point)
{
	if (!m_bFirstDown)
		return;

	CToolBase::OnLButtonUp(pView, nFlages, point);
	CAquariusPhotoDoc* pDoc = dynamic_cast<CAquariusPhotoDoc*>(pView->GetDocument());
	ASSERT(pDoc);

	// clear select 
	ClearSelect();

	// hit test select
	if (m_firstPoint == point)
	{
		SetSelectShape(point);
	}
	else
	{
		CRect rc;
		::SetRectByPoint(m_firstPoint.x, m_firstPoint.y, m_lastPoint.x, m_lastPoint.y, rc);
		SetSelectShape(rc);
	}


	// enable focus rect
	pDoc->SetFocusRect(NULL);
	pView->Invalidate(FALSE);
}

void CTool_SelectShape::OnMouseMove(CView* pView, UINT nFlages, CPoint& point)
{
	if (!m_bFirstDown)
		return;

	CToolBase::OnMouseMove(pView, nFlages, point);
	CAquariusPhotoDoc* pDoc = dynamic_cast<CAquariusPhotoDoc*>(pView->GetDocument());
	ASSERT(pDoc);

	CRect rc;
	::SetRectByPoint(m_firstPoint.x, m_firstPoint.y, m_lastPoint.x, m_lastPoint.y, rc);

	pDoc->SetFocusRect(&rc);
	pView->Invalidate(FALSE);
}

void CTool_SelectShape::OnLButtonDblClk(CView* pView, UINT nFlages, CPoint& point)
{

}

void CTool_SelectShape::OnLButtonDown(CView* pView, UINT nFlags, CPoint& point)
{
	CToolBase::OnLButtonDown(pView, nFlags, point);
}

void CTool_SelectShape::AddShape(IShape* pShape)
{
	m_shapePtrList.push_back(pShape);
}

void CTool_SelectShape::SetSelectShape(CPoint& point)
{
	std::vector<IShape*>::iterator it = m_shapePtrList.begin();
	for (; it != m_shapePtrList.end(); it++)
	{
		IShape* pShape = *it;
		if(pShape->HitTest(point))
			pShape->SetSelected(TRUE);
	}
}

void CTool_SelectShape::SetSelectShape(CRect& rc)
{
	CRect rcShape;
	std::vector<IShape*>::iterator it = m_shapePtrList.begin();
	for (; it != m_shapePtrList.end(); it++)
	{
		IShape* pShape = *it;
		ASSERT(pShape);

		pShape->GetEnvelope(rcShape);
		if (rc.PtInRect(rcShape.TopLeft()) && rc.PtInRect(rcShape.BottomRight()))
			pShape->SetSelected(TRUE);
	}
}

void CTool_SelectShape::ClearSelect()
{
	std::vector<IShape*>::iterator it = m_shapePtrList.begin();
	for (; it != m_shapePtrList.end(); it++)
		(*it)->SetSelected(FALSE);
}

void CTool_SelectShape::OnActivate(CView* pView)
{

}

void CTool_SelectShape::OnInactivate(CView* pView)
{
	ClearSelect();

	pView->Invalidate(FALSE);
}
