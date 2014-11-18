#pragma once

interface ITool
{
	virtual void OnLButtonDown(CView* pView, UINT nFlags, CPoint point) = 0;
	virtual void OnLButtonUp(CView* pView, UINT nFlages, CPoint point) = 0;
	virtual void OnMouseMove(CView* pView, UINT nFlages, CPoint point) = 0;
	virtual void OnLButtonDblClk(CView* pView, UINT nFlages, CPoint point) = 0;
};

interface IShape
{
	virtual BOOL DrawShape(Graphics* pView) = 0;
	virtual BOOL DrawHandle(Graphics* pView) = 0;
};
