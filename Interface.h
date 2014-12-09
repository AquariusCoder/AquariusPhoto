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

interface IOperElement
{
	virtual BOOL Do(Image** ppImg) = 0;
	virtual IOperElement* Prev() = 0;
	virtual IOperElement* Next() = 0;

	virtual int GetID() = 0;

	virtual void SetPrev(IOperElement* pElem) = 0;
	virtual void SetNext(IOperElement* pElem) = 0;
};

class OperElementBase : public IOperElement
{
public:
	OperElementBase(void);

	virtual BOOL Do(Image** ppImg) = 0;
	virtual IOperElement* Prev();
	virtual IOperElement* Next();

	virtual int GetID();

	virtual void SetPrev(IOperElement* pElem);
	virtual void SetNext(IOperElement* pElem);

protected:
	int m_nID;
	IOperElement* m_prev;
	IOperElement* m_next;
};
