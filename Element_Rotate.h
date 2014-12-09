#pragma once
#include <Gdiplus.h>
#include "Interface.h"

class CElement_Rotate : public OperElementBase
{
public:
	CElement_Rotate(RotateFlipType type);
	~CElement_Rotate(void);

// IOperElement
public:
	virtual BOOL Do(Image** ppImg);

protected:
	Gdiplus::RotateFlipType m_type;
};
