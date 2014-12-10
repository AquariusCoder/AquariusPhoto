#pragma once
#include "Interface.h"
#include <vector>

enum enum_Tool_Mode
{
	TM_NONE = -1,
	TM_SELECT_REGION_RECT = 0,
	TM_SELECT_REGION_ANY,
	TM_SELECT_SHAPE,
	TM_DRAW_LINE,
	TM_DRAW_RECT, 
	TM_DRAW_ELLIPSE, 
	TM_DRAW_POLYGON,
};

class CToolManager
{
public:
	CToolManager(void);
	~CToolManager(void);

	static CToolManager* Instance();

public:
	void SetToolMode(CView* pView, enum_Tool_Mode mode);
	enum_Tool_Mode GetCurrentToolMode();

	ITool* GetTool(enum_Tool_Mode mode);
	ITool* GetCurrentTool();

	void OnLButtonDown(CView* pView, UINT nFlags, CPoint& point);
	void OnLButtonUp(CView* pView, UINT nFlags, CPoint& point);
	void OnMouseMove(CView* pView, UINT nFlags, CPoint& point);
	void OnLButtonDblClk(CView* pView, UINT nFlags, CPoint& point);

protected:
	
	
	enum_Tool_Mode m_currentTool;
	std::vector<ITool*> m_toolPtrList;
};

