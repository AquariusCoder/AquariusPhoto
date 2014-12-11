#include "StdAfx.h"
#include "Tool_DrawLine.h"
#include "AquariusPhotoDoc.h"
#include "Element_Draw_Line.h"
#include "OperManager.h"
#include "ToolManager.h"
#include "Tool_SelectShape.h"

CTool_DrawLine::CTool_DrawLine(void)
{
}

CTool_DrawLine::~CTool_DrawLine(void)
{
}

void CTool_DrawLine::OnLButtonDown(CView* pView, UINT nFlags, CPoint& point)
{
	CToolBase::OnLButtonDown(pView, nFlags, point);
}

void CTool_DrawLine::OnLButtonUp(CView* pView, UINT nFlages, CPoint& point)
{
	if(!m_bFirstDown)
		return;

	CToolBase::OnLButtonUp(pView, nFlages, point);
	
	// add line shape
	CElement_Draw_Line* pElem = new CElement_Draw_Line(m_firstPoint, m_lastPoint);
	COperManager::Instance()->Add(pElem);

	ITool* pTool = CToolManager::Instance()->GetTool(TM_SELECT_SHAPE);
	ASSERT(pTool != NULL);
	CTool_SelectShape* pToolSelShape = static_cast<CTool_SelectShape*>(pTool);

	// clear dynamic shape
	CAquariusPhotoDoc* pDoc = dynamic_cast<CAquariusPhotoDoc *>(pView->GetDocument());
	ASSERT(pDoc);

	pDoc->ClearDynamicShape();

	pView->Invalidate(FALSE);
}

void CTool_DrawLine::OnMouseMove(CView* pView, UINT nFlages, CPoint& point)
{
	if(!m_bFirstDown)
		return;

	CToolBase::OnMouseMove(pView, nFlages, point);
	
	// draw dynamic
	CAquariusPhotoDoc* pDoc = dynamic_cast<CAquariusPhotoDoc *>(pView->GetDocument());
	ASSERT(pDoc);

	CElement_Draw_Line* pElement = new CElement_Draw_Line(m_firstPoint, m_lastPoint);
	pDoc->SetDynamicShape(pElement);

	pView->Invalidate(FALSE);
}

void CTool_DrawLine::OnLButtonDblClk(CView* pView, UINT nFlages, CPoint& point)
{

}

void CTool_DrawLine::OnActivate(CView* pView)
{

}

void CTool_DrawLine::OnInactivate(CView* pView)
{

}
