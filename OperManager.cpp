#include "StdAfx.h"
#include "OperManager.h"

COperManager g_operManger;

COperManager::COperManager(void)
{
	m_pImg = NULL;
}

COperManager::~COperManager(void)
{
	if (m_pImg != NULL)
		delete m_pImg;
}

COperManager* COperManager::Instance()
{
	return &g_operManger;
}

void COperManager::ReDo()
{
	if (m_pIndex != NULL && m_pIndex->Next() != NULL)
	{
		m_pIndex = m_pIndex->Next();
		m_pIndex->OnReDo();
	}
}

void COperManager::UnDo()
{
	if (m_pIndex != NULL && m_pIndex->Prev() != NULL)
	{
		m_pIndex->OnUnDo();
		m_pIndex = m_pIndex->Prev();
	}
}

void COperManager::Do()
{
	IOperElement* pIndex = m_pOperElemList;
	while(pIndex != NULL)
	{
		pIndex->Do(&m_pImg);

		if(pIndex->GetID() == m_pIndex->GetID())
			break;
		
		pIndex = pIndex->Next();
	}
}

Image* COperManager::GetImage()
{
	return m_pImg;
}



