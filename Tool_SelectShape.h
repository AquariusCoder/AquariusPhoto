#pragma once
#include "Interface.h"
#include <vector>

class CTool_SelectShape : public CToolBase
{
public:
	CTool_SelectShape(void);
	~CTool_SelectShape(void);

// ITool
public:
	virtual void OnLButtonDown(CView* pView, UINT nFlags, CPoint& point);
	virtual void OnLButtonUp(CView* pView, UINT nFlages, CPoint& point);
	virtual void OnMouseMove(CView* pView, UINT nFlages, CPoint& point);
	virtual void OnLButtonDblClk(CView* pView, UINT nFlages, CPoint& point);

	virtual void OnActivate(CView* pView);
	virtual void OnInactivate(CView* pView);

// shape
public:
	virtual void AddShape(IShape* pShape);
	virtual void SetSelectShape(CPoint& point);
	virtual void SetSelectShape(CRect& rc);

protected:
	void ClearSelect();

protected:
	std::vector<IShape*> m_shapePtrList;
};
