#pragma once
#include "Interface.h"

class CElement_Clip : public COperElementBase
{
public:
	CElement_Clip(CRect& rc);
	~CElement_Clip(void);

public:
	virtual BOOL Do(Image** ppImg);
	virtual void OnReDo();
	virtual void OnUnDo();

protected:
	CRect m_rc;
};
