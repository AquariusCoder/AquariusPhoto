#pragma once
#include "Interface.h"

class CElement_Clip : public OperElementBase
{
public:
	CElement_Clip(CRect& rc);
	~CElement_Clip(void);

public:
	virtual BOOL Do(Image** ppImg);

protected:
	CRect m_rc;
};
