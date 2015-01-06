#pragma once
#include <vector>

interface ITool
{
	virtual void OnLButtonDown(CView* pView, UINT nFlags, CPoint& point) = 0;
	virtual void OnLButtonUp(CView* pView, UINT nFlages, CPoint& point) = 0;
	virtual void OnMouseMove(CView* pView, UINT nFlages, CPoint& point) = 0;
	virtual void OnLButtonDblClk(CView* pView, UINT nFlages, CPoint& point) = 0;
	
	virtual void OnActivate(CView* pView) = 0;
	virtual void OnInactivate(CView* pView) = 0;
};

class CToolBase : public ITool
{
public:
	CToolBase(void);
	void AdjustPosition(CPoint& point, CRect& rc);

	virtual void OnLButtonDown(CView* pView, UINT nFlags, CPoint& point);
	virtual void OnLButtonUp(CView* pView, UINT nFlages, CPoint& point);
	virtual void OnMouseMove(CView* pView, UINT nFlages, CPoint& point);
	virtual void OnLButtonDblClk(CView* pView, UINT nFlages, CPoint& point) = 0;

	virtual void OnActivate(CView* pView) = 0;
	virtual void OnInactivate(CView* pView) = 0;

protected:	
	BOOL m_bFirstDown;
	CPoint m_firstPoint;
	CPoint m_lastPoint;
};

interface IShape
{
	virtual BOOL DrawShape(Graphics* pGraphics) = 0;
	virtual BOOL DrawHandle(Graphics* pGraphics) = 0;

	virtual void Erase(BOOL bErase = TRUE) = 0;
	virtual BOOL IsErased() = 0;

	virtual void SetSelected(BOOL bSel) = 0;
	virtual BOOL IsSelected() = 0;

	virtual BOOL HitTest(CPoint& pos) = 0;
	virtual BOOL HitTest(CRect& rc) = 0;
	virtual int HandleHitTest(CPoint& pos) = 0;

	virtual int GetHandleCount() = 0;
	virtual BOOL GetHandle(int nIndex, CPoint& pos) = 0;
	virtual BOOL GetHandlePtr(int nIndex, CPoint** ppHandle) = 0;

	virtual BOOL GetEnvelope(CRect& rc) = 0;
	virtual BOOL GetHandleRect(int nIndex, CRect& rc) = 0;

	virtual IShape* Clone() = 0;
	virtual IShape* GetParent() = 0;
	virtual void Transform(Matrix& mt) = 0;
	virtual BOOL IsChanged() = 0;
	virtual void SetChanged(BOOL bChanged) = 0;

	virtual Color GetColor() = 0;
	virtual void SetColor(Color& clr) = 0;
};

class CShapeBase : public IShape
{
public:
	CShapeBase(void);
	~CShapeBase(void);

	virtual BOOL DrawShape(Graphics* pGraphics) = 0;
	virtual BOOL DrawHandle(Graphics* pGraphics);

	virtual void Erase(BOOL bErase = TRUE) = 0;
	virtual BOOL IsErased();

	virtual void SetSelected(BOOL bSel);
	virtual BOOL IsSelected();

	virtual BOOL HitTest(CPoint& pos) = 0;
	virtual BOOL HitTest(CRect& rc) = 0;
	virtual int HandleHitTest(CPoint& pos) = 0;

	virtual int GetHandleCount() = 0;
	virtual BOOL GetHandle(int nIndex, CPoint& pos) = 0;
	virtual BOOL GetHandlePtr(int nIndex, CPoint** ppHandle) = 0;

	virtual BOOL GetEnvelope(CRect& rc) = 0;
	virtual BOOL GetHandleRect(int nIndex, CRect& rc) = 0;

	virtual IShape* Clone() = 0;
	virtual IShape* GetParent();
	virtual void Transform(Matrix& mt);
	virtual BOOL IsChanged();
	virtual void SetChanged(BOOL bChanged);

	virtual Color GetColor();
	virtual void SetColor(Color& clr);

protected:
	virtual BOOL IsCanHitTest();

protected:
	BOOL m_bErased;
	BOOL m_bSelected;
	BOOL m_bChanged;
	IShape* m_pParent;
	int m_nHandleInflate;
	SolidBrush* m_pHandleBrush;
	Pen* m_pHandlePen;
	Color m_color;

	POINT m_penwidth;
};

interface IOperElement
{
	virtual BOOL Do(Image** ppImg) = 0;
	virtual void OnReDo() = 0;
	virtual void OnUnDo() = 0;
	virtual IOperElement* Prev() = 0;
	virtual IOperElement* Next() = 0;

	virtual int GetID() = 0;

	virtual void SetPrev(IOperElement* pElem) = 0;
	virtual void SetNext(IOperElement* pElem) = 0;
};

class COperElementBase : public IOperElement
{
public:
	COperElementBase(void);

	virtual BOOL Do(Image** ppImg) = 0;
	virtual void OnReDo() = 0;
	virtual void OnUnDo() = 0;
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

interface IOperQueue
{
	virtual void Add(IOperElement* pElem) = 0;
	virtual BOOL Remove(IOperElement* pElem) = 0;
	virtual BOOL Remove(int nIndex) = 0;
	virtual void Clear() = 0;
	virtual int GetCount() = 0;
	virtual int GetValidCount() = 0;
	virtual void GetShapes(std::vector<IShape*>& vec) = 0;
};

class COperQueueBase : public IOperQueue
{
public:
	COperQueueBase(void);
	~COperQueueBase(void);

	virtual void Add(IOperElement* pElem);
	virtual BOOL Remove(IOperElement* pElem);
	virtual BOOL Remove(int nIndex);
	virtual void Clear();
	virtual int GetCount();
	virtual int GetValidCount();
	virtual void GetShapes(std::vector<IShape*>& vec);

protected:
	virtual void push_back(IOperElement* pElem);
	virtual void Free(IOperElement* pElem);

protected:
	IOperElement* m_pIndex;
	IOperElement* m_pOperElemList;
};