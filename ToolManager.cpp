#include "StdAfx.h"
#include "ToolManager.h"
#include "Tool_SelectRegion.h"
#include "Tool_DrawLine.h"
#include "Tool_SelectShape.h"

CToolManager g_toolManager;

CToolManager::CToolManager(void)
{
	CTool_SelectRegion* pToolSelectRegion = new CTool_SelectRegion();
	m_toolPtrList.push_back(pToolSelectRegion);

	CTool_SelectRegion* pToolSelectRegionAny = new CTool_SelectRegion();
	m_toolPtrList.push_back(pToolSelectRegionAny);

	CTool_SelectShape* pToolSelectShape = new CTool_SelectShape();
	m_toolPtrList.push_back(pToolSelectShape);

	CTool_DrawLine* pToolDrawLine = new CTool_DrawLine();
	m_toolPtrList.push_back(pToolDrawLine);

	m_currentTool = TM_NONE;
}

CToolManager::~CToolManager(void)
{
	std::vector<ITool*>::iterator it;
	for(it = m_toolPtrList.begin(); it != m_toolPtrList.end(); it++)
	{
		ITool* pTool = *it;
		delete pTool;
	}
}

void CToolManager::SetToolMode( CView* pView, enum_Tool_Mode mode )
{
	std::vector<ITool*>::iterator it;
	for(it = m_toolPtrList.begin(); it != m_toolPtrList.end(); it++)
		(*it)->OnInactivate(pView);

	m_currentTool = mode;
	
	GetCurrentTool()->OnActivate(pView);
}

enum_Tool_Mode CToolManager::GetCurrentToolMode()
{
	return m_currentTool;
}

ITool* CToolManager::GetTool( enum_Tool_Mode mode )
{
	return m_toolPtrList[mode];
}

ITool* CToolManager::GetCurrentTool()
{
	if (TM_NONE == m_currentTool)
		return NULL;

	return m_toolPtrList[m_currentTool];
}

void CToolManager::OnLButtonDown( CView* pView, UINT nFlags, CPoint& point )
{
	ITool* pTool = GetCurrentTool();
	if (pTool == NULL)
		return;

	pTool->OnLButtonDown(pView, nFlags, point);
}

void CToolManager::OnLButtonUp( CView* pView, UINT nFlags, CPoint& point )
{
	ITool* pTool = GetCurrentTool();
	if (pTool == NULL)
		return;

	pTool->OnLButtonUp(pView, nFlags, point);
}

void CToolManager::OnMouseMove( CView* pView, UINT nFlags, CPoint& point )
{
	ITool* pTool = GetCurrentTool();
	if (pTool == NULL)
		return;

	pTool->OnMouseMove(pView, nFlags, point);
}

void CToolManager::OnLButtonDblClk( CView* pView, UINT nFlags, CPoint& point )
{
	ITool* pTool = GetCurrentTool();
	if (pTool == NULL)
		return;

	pTool->OnLButtonDblClk(pView, nFlags, point);
}

CToolManager* CToolManager::Instance()
{
	return &g_toolManager;
}


