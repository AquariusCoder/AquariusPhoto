#pragma once
#include "Interface.h"

class CTool_SelectRegion : public ITool
{
public:
	CTool_SelectRegion(void);
	~CTool_SelectRegion(void);

public:
	void AdjustPosition(CPoint& point, CRect& rc);

// ITool
public:
	void OnLButtonDown(CView* pView, UINT nFlags, CPoint point);
	void OnLButtonUp(CView* pView, UINT nFlages, CPoint point);
	void OnMouseMove(CView* pView, UINT nFlages, CPoint point);
	void OnLButtonDblClk(CView* pView, UINT nFlages, CPoint point);

protected:
	BOOL m_bFirstDown;
	CPoint m_firstPoint;
	CPoint m_lastPoint;
};
