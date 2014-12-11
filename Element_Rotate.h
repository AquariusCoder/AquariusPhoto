#pragma once
#include <Gdiplus.h>
#include "Interface.h"

class CElement_Rotate : public COperElementBase
{
public:
	CElement_Rotate(RotateFlipType type);
	~CElement_Rotate(void);

// IOperElement
public:
	virtual BOOL Do(Image** ppImg);
	virtual void OnReDo();
	virtual void OnUnDo();

protected:
	Gdiplus::RotateFlipType m_type;
};
