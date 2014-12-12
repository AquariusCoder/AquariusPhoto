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
	m_bChanged = FALSE;
	m_pParent = NULL;
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

void CShapeBase::Transform(Matrix& mt)
{
	int nCount = GetHandleCount();
	CPoint* pPoint = NULL;
	for (int i = 0; i < nCount; i++)
	{
		GetHandlePtr(i, &pPoint);
		Point pos(pPoint->x, pPoint->y);
		mt.TransformPoints(&pos);

		pPoint->x = pos.X;
		pPoint->y = pos.Y;
	}
}

BOOL CShapeBase::IsChanged()
{
	return m_bChanged;
}

void CShapeBase::SetChanged(BOOL bChanged)
{
	m_bChanged = bChanged;
}

IShape* CShapeBase::GetParent()
{
	return m_pParent;
}

BOOL CShapeBase::IsCanHitTest()
{
	// no see no hit
	return (IsChanged() || IsErased()) ? FALSE : TRUE;
}

COperQueueBase::COperQueueBase(void)
{
	m_pIndex = NULL;
	m_pOperElemList = NULL;
}

COperQueueBase::~COperQueueBase(void)
{
	Free(m_pOperElemList);
}

void COperQueueBase::Add(IOperElement* pElem)
{
	if (m_pOperElemList == NULL)
		m_pOperElemList = pElem;
	else
		push_back(pElem);

	m_pIndex = pElem;
}

void COperQueueBase::push_back(IOperElement* pElem)
{
	// free first
	if (m_pIndex != NULL)
		Free(m_pIndex->Next());

	// push back
	IOperElement* pIndex = m_pOperElemList;
	while(pIndex != NULL)
	{
		if (pIndex->Next() == NULL)
		{
			pIndex->SetNext(pElem);
			pElem->SetPrev(pIndex);

			break;
		}

		pIndex = pIndex->Next();
	}
}

BOOL COperQueueBase::Remove(IOperElement* pElem)
{
	return FALSE;
}

BOOL COperQueueBase::Remove(int nIndex)
{
	return FALSE;
}

void COperQueueBase::Clear()
{
	Free(m_pOperElemList);

	m_pOperElemList = NULL;
}

void COperQueueBase::Free(IOperElement* pElem)
{
	if (pElem == NULL)
		return;

	// set prev's next null
	if (pElem->Prev() != NULL)
		pElem->Prev()->SetNext(NULL);

	// free last
	IOperElement* pIndex = pElem;
	while(pIndex != NULL)
	{
		IOperElement* pDel = pIndex;
		pIndex = pIndex->Next();

		delete pDel;
	}
}

int COperQueueBase::GetCount()
{
	int nCount = 0;
	IOperElement* pIndex = m_pOperElemList;
	while(pIndex != NULL)
	{
		nCount++;

		pIndex = pIndex->Next();
	}

	return nCount;
}

int COperQueueBase::GetValidCount()
{
	int nCount = 0;
	IOperElement* pIndex = m_pOperElemList;
	while(pIndex != NULL)
	{
		nCount++;

		if(pIndex->GetID() == m_pIndex->GetID())
			break;

		pIndex = pIndex->Next();
	}
	
	return nCount;
}

void COperQueueBase::GetShapes(std::vector<IShape*>& vec)
{
	IOperElement* pIndex = m_pOperElemList;
	while(pIndex != NULL)
	{
		IShape* pShape = dynamic_cast<IShape*>(pIndex);
		if (pShape != NULL)
			vec.push_back(pShape);

		IOperQueue* pQueue = dynamic_cast<IOperQueue*>(pIndex);
		if(pQueue != NULL)
			pQueue->GetShapes(vec);

		if(pIndex->GetID() == m_pIndex->GetID())
			break;

		pIndex = pIndex->Next();
	}
}
