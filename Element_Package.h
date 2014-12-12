#pragma once
#include "Interface.h"

class CElement_Package : public COperElementBase, public COperQueueBase
{
public:
	CElement_Package(void);
	~CElement_Package(void);

// IOperElement
public:
	virtual BOOL Do(Image** ppImg);
	virtual void OnReDo();
	virtual void OnUnDo();

};
