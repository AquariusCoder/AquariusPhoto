#pragma once
#include "Interface.h"

class CElement_Open : public COperElementBase
{
public:
	CElement_Open(Image* pImg);
	~CElement_Open(void);

// OperElementBase
public:
	virtual BOOL Do(Image** ppImg);
	virtual void OnReDo();
	virtual void OnUnDo();

	Image* m_pImg;
};
