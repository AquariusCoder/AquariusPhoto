#pragma once
#include "Interface.h"

class CTool_SelectRegion : public CToolBase
{
public:
	CTool_SelectRegion(void);
	~CTool_SelectRegion(void);

// ITool
public:
	virtual void OnLButtonDown(CView* pView, UINT nFlags, CPoint& point);
	virtual void OnLButtonUp(CView* pView, UINT nFlages, CPoint& point);
	virtual void OnMouseMove(CView* pView, UINT nFlages, CPoint& point);
	virtual void OnLButtonDblClk(CView* pView, UINT nFlages, CPoint& point);

	virtual void OnActivate(CView* pView);
	virtual void OnInactivate(CView* pView);

};
