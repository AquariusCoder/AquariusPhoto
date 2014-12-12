#pragma once
#include "Interface.h"
#include <vector>

class COperManager : public COperQueueBase
{
public:
	COperManager(void);
	~COperManager(void);

public:
	static COperManager* Instance();
	Image* GetImage();

// Operation
public:
	void Do();
	void ReDo();
	void UnDo();

protected:
	Image* m_pImg;
};
