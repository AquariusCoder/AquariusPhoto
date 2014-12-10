#pragma once
#include "Interface.h"

class CElement_Draw_Line : public COperElementBase, public CShapeBase
{
public:
	CElement_Draw_Line(CPoint& p1, CPoint& p2);
	~CElement_Draw_Line(void);

// IOperElement
public:
	virtual BOOL Do(Image** ppImg);

// IShape
public:
	virtual BOOL DrawShape(Graphics* pGraphics);
	virtual void Erase(BOOL bErase = TRUE);
	virtual BOOL HitTest(CPoint& pos);
	virtual int GetHandleCount();
	virtual BOOL GetHandle(int nIndex, CPoint& pos);
	virtual int HandleHitTest(CPoint& pos);
	virtual BOOL GetEnvelope(CRect& rc);
	virtual BOOL GetHandleRect(int nIndex, CRect& rc);

protected:
	CPoint m_p1;
	CPoint m_p2;
};