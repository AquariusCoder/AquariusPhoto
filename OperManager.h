#pragma once
#include "Interface.h"

class COperManager
{
public:
	COperManager(void);
	~COperManager(void);

public:
	static COperManager* Instance();
	Image* GetImage();

	void Add(IOperElement* pElem);
	void ReDo();
	void UnDo();

	void Do();
	void Clear();

protected:
	void push_back(IOperElement* pElem);
	void Free(IOperElement* pElem);

protected:
	IOperElement* m_pIndex;
	IOperElement* m_pOperElemList;

	Image* m_pImg;
};
