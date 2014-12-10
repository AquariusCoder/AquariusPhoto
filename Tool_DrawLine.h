#pragma once
#include "Interface.h"

class CTool_DrawLine : public CToolBase
{
public:
	CTool_DrawLine(void);
	~CTool_DrawLine(void);

// ITool
public:
	virtual void OnLButtonDown(CView* pView, UINT nFlags, CPoint& point);
	virtual void OnLButtonUp(CView* pView, UINT nFlages, CPoint& point);
	virtual void OnMouseMove(CView* pView, UINT nFlages, CPoint& point);
	virtual void OnLButtonDblClk(CView* pView, UINT nFlages, CPoint& point);

	virtual void OnActivate(CView* pView);
	virtual void OnInactivate(CView* pView);
};
