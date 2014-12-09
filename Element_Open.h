#pragma once
#include "Interface.h"

class CElement_Open : public OperElementBase
{
public:
	CElement_Open(Image* pImg);
	~CElement_Open(void);

// OperElementBase
public:
	virtual BOOL Do(Image** ppImg);

	Image* m_pImg;
};
