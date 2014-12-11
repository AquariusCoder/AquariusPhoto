#pragma once
#include "Interface.h"
#include <vector>

enum enum_Sel_Tool_Status
{
	STS_NONE = -1,
	STS_SEL_POINT = 0,
	STS_SEL_RECT,
	STS_SHP_MOVE,
	STS_SHP_CHANGE,
};

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
	virtual void SetSelectShape(CPoint& point);
	virtual void SetSelectShape(CRect& rc);

protected:
	void ClearSelect();
	int GetSelectedShape(std::vector<IShape*>& vec);

protected:
	enum_Sel_Tool_Status m_status;
};
