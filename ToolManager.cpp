#include "StdAfx.h"
#include "ToolManager.h"
#include "Tool_SelectRegion.h"

CToolManager g_toolManager;

CToolManager::CToolManager(void)
{
	CTool_SelectRegion* pToolSelectRegion = new CTool_SelectRegion();
	m_toolPtrList.push_back(pToolSelectRegion);

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

void CToolManager::SetToolMode( enum_Tool_Mode mode )
{
	m_currentTool = mode;
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

void CToolManager::OnLButtonDown( CView* pView, UINT nFlags, CPoint point )
{
	ITool* pTool = GetCurrentTool();
	if (pTool == NULL)
		return;

	pTool->OnLButtonDown(pView, nFlags, point);
}

void CToolManager::OnLButtonUp( CView* pView, UINT nFlags, CPoint point )
{
	ITool* pTool = GetCurrentTool();
	if (pTool == NULL)
		return;

	pTool->OnLButtonUp(pView, nFlags, point);
}

void CToolManager::OnMouseMove( CView* pView, UINT nFlags, CPoint point )
{
	ITool* pTool = GetCurrentTool();
	if (pTool == NULL)
		return;

	pTool->OnMouseMove(pView, nFlags, point);
}

void CToolManager::OnLButtonDblClk( CView* pView, UINT nFlags, CPoint point )
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


