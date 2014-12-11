#include "StdAfx.h"
#include "Tool_SelectShape.h"
#include "GeometryAlgorithm.h"
#include "AquariusPhotoDoc.h"
#include "OperManager.h"

CTool_SelectShape::CTool_SelectShape(void)
{
	m_status = STS_NONE;
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

	switch(m_status)
	{
	case STS_NONE:
		ClearSelect();
		break;
	case STS_SEL_POINT:
		{
			ClearSelect();
			SetSelectShape(point);
		}
		break;
	case STS_SEL_RECT:
		{
			ClearSelect();

			CRect rc;
			::SetRectByPoint(m_firstPoint.x, m_firstPoint.y, m_lastPoint.x, m_lastPoint.y, rc);
			SetSelectShape(rc);
			// enable focus rect
			pDoc->SetFocusRect(NULL);
		}
		break;
	case STS_SHP_MOVE:
		{
			std::vector<IShape*> vec;
			GetSelectedShape(vec);
			std::vector<IShape*>::iterator it = vec.begin();
			Matrix mt(1.0f, 0.0f, 0.0f, 1.0f,(REAL)(m_lastPoint.x - m_firstPoint.x), (REAL)(m_lastPoint.y - m_firstPoint.y));
			for(; it != vec.end(); it++)
			{
				// clone & transform
				IShape* pShape = *it;
				IShape* pClone = pShape->Clone();
				pClone->SetChanged(FALSE);
				pClone->SetSelected(TRUE);
				pClone->Transform(mt);
				pClone->GetParent()->SetChanged(TRUE);
				pClone->GetParent()->SetSelected(FALSE);
				// add operation element
				IOperElement* pElem = dynamic_cast<IOperElement*>(pClone);
				ASSERT(pElem);
				COperManager::Instance()->Add(pElem);
			}
		}
		break;
	case STS_SHP_CHANGE:
		break;
	}

	m_status = STS_NONE;
	// clear dynamic shapes
	pDoc->ClearDynamicShape();
	pView->Invalidate(FALSE);
}

void CTool_SelectShape::OnMouseMove(CView* pView, UINT nFlages, CPoint& point)
{
	if (!m_bFirstDown)
		return;

	CToolBase::OnMouseMove(pView, nFlages, point);
	CAquariusPhotoDoc* pDoc = dynamic_cast<CAquariusPhotoDoc*>(pView->GetDocument());
	ASSERT(pDoc);
	
	if (m_status == STS_NONE || m_status == STS_SEL_POINT)
		m_status = STS_SEL_RECT;

	if (m_status == STS_SHP_MOVE_READY)
		m_status = STS_SHP_MOVE;

	switch(m_status)
	{
	case STS_NONE:
	case STS_SEL_POINT:
		break;
	case STS_SEL_RECT:
		{
			CRect rc;
			::SetRectByPoint(m_firstPoint.x, m_firstPoint.y, m_lastPoint.x, m_lastPoint.y, rc);

			pDoc->SetFocusRect(&rc);
			pView->Invalidate(FALSE);
		}
		break;
	case STS_SHP_MOVE:
		{
			std::vector<IShape*> vec;
			std::vector<IShape*> vecClone;
			GetSelectedShape(vec);
			std::vector<IShape*>::iterator it = vec.begin();
			Matrix mt(1.0f, 0.0f, 0.0f, 1.0f,(REAL)(m_lastPoint.x - m_firstPoint.x), (REAL)(m_lastPoint.y - m_firstPoint.y));
			for(; it != vec.end(); it++)
			{
				IShape* pShape = *it;
				IShape* pClone = pShape->Clone();
				pClone->SetChanged(FALSE);
				pClone->SetSelected(TRUE);
				pClone->GetParent()->SetChanged(TRUE);
				//pClone->GetParent()->SetSelected(FALSE);
				pClone->Transform(mt);
				vecClone.push_back(pClone);
			}

			pDoc->SetDynamicShape(vecClone);
			pView->Invalidate(FALSE);
		}
		break;
	case STS_SHP_CHANGE:
		break;
	}
		
}

void CTool_SelectShape::OnLButtonDblClk(CView* pView, UINT nFlages, CPoint& point)
{

}

void CTool_SelectShape::OnLButtonDown(CView* pView, UINT nFlags, CPoint& point)
{
	CToolBase::OnLButtonDown(pView, nFlags, point);

	std::vector<IShape*> vec;
	int nSelCount = GetSelectedShape(vec);
	if (nSelCount == 0)
	{
		m_status = STS_SEL_POINT;
	}
	else if (nSelCount == 1)
	{
		IShape* pShape = vec[0];
		ASSERT(pShape);

		int nIndex = pShape->HandleHitTest(point);
		if(nIndex > -1)
		{
			m_status = STS_SHP_CHANGE;
			// TODO set cursor
		}
		else if(pShape->HitTest(point))
		{
			m_status = STS_SHP_MOVE_READY;
			// TODO set cursor
		}

	}
	else 
	{
		std::vector<IShape*>::iterator it = vec.begin();
		for (; it != vec.end(); it++)
		{
			if((*it)->HitTest(point))
			{
				m_status = STS_SHP_MOVE_READY;
				break;
			}
		}
	}

}


void CTool_SelectShape::SetSelectShape(CPoint& point)
{
	std::vector<IShape*> vec;
	COperManager::Instance()->GetShapes(vec);
	std::vector<IShape*>::iterator it = vec.begin();
	for (; it != vec.end(); it++)
	{
		IShape* pShape = *it;
		ASSERT(pShape);

		if(pShape->HitTest(point))
			pShape->SetSelected(TRUE);
	}
}

void CTool_SelectShape::SetSelectShape(CRect& rc)
{
	std::vector<IShape*> vec;
	COperManager::Instance()->GetShapes(vec);
	std::vector<IShape*>::iterator it = vec.begin();
	for (; it != vec.end(); it++)
	{
		IShape* pShape = *it;
		ASSERT(pShape);

		if(pShape->HitTest(rc))
			pShape->SetSelected(TRUE);
	}
}

void CTool_SelectShape::ClearSelect()
{
	std::vector<IShape*> vec;
	COperManager::Instance()->GetShapes(vec);
	std::vector<IShape*>::iterator it = vec.begin();
	for (; it != vec.end(); it++)
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

int CTool_SelectShape::GetSelectedShape(std::vector<IShape*>& vec)
{
	std::vector<IShape*> vecShapes;
	COperManager::Instance()->GetShapes(vecShapes);

	std::vector<IShape*>::iterator it = vecShapes.begin();
	for (; it != vecShapes.end(); it++)
	{
		IShape* pShape = *it;
		ASSERT(pShape);

		if (pShape->IsErased()) // no see no select
			continue;

		if(pShape->IsSelected())
			vec.push_back(pShape);
	}

	return (int)vec.size();
}
