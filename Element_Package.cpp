#include "StdAfx.h"
#include "Element_Package.h"

CElement_Package::CElement_Package(void)
{
}

CElement_Package::~CElement_Package(void)
{
}

BOOL CElement_Package::Do(Image** ppImg)
{
	IOperElement* pIndex = m_pOperElemList;
	while(pIndex != NULL)
	{
		pIndex->Do(ppImg);

		pIndex = pIndex->Next();
	}

	return TRUE;
}

void CElement_Package::OnReDo()
{
	IOperElement* pIndex = m_pOperElemList;
	while(pIndex != NULL)
	{
		pIndex->OnReDo();

		pIndex = pIndex->Next();
	}

}

void CElement_Package::OnUnDo()
{
	IOperElement* pIndex = m_pOperElemList;
	while(pIndex != NULL)
	{
		pIndex->OnUnDo();

		pIndex = pIndex->Next();
	}
}

