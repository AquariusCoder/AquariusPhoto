#include "StdAfx.h"
#include "OperManager.h"

COperManager g_operManger;

COperManager::COperManager(void)
{
	m_pIndex = NULL;
	m_pOperElemList = NULL;

	m_pImg = NULL;
}

COperManager::~COperManager(void)
{
	Free(m_pOperElemList);

	if (m_pImg != NULL)
		delete m_pImg;
}

void COperManager::Free(IOperElement* pElem)
{
	if (pElem == NULL)
		return;

	// set prev's next null
	if (pElem->Prev() != NULL)
		pElem->Prev()->SetNext(NULL);

	// free last
	IOperElement* pIndex = pElem;
	while(pIndex != NULL)
	{
		IOperElement* pDel = pIndex;
		pIndex = pIndex->Next();

		delete pDel;
	}
}

COperManager* COperManager::Instance()
{
	return &g_operManger;
}

void COperManager::Add(IOperElement* pElem)
{
	if (m_pOperElemList == NULL)
		m_pOperElemList = pElem;
	else
		push_back(pElem);

	m_pIndex = pElem;
}

void COperManager::push_back(IOperElement* pElem)
{
	// free first
	if (m_pIndex != NULL)
		Free(m_pIndex->Next());

	// push back
	IOperElement* pIndex = m_pOperElemList;
	while(pIndex != NULL)
	{
		if (pIndex->Next() == NULL)
		{
			pIndex->SetNext(pElem);
			pElem->SetPrev(pIndex);

			break;
		}

		pIndex = pIndex->Next();
	}
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

void COperManager::Clear()
{
	Free(m_pOperElemList);

	m_pOperElemList = NULL;
}

Image* COperManager::GetImage()
{
	return m_pImg;
}

void COperManager::GetShapes(std::vector<IShape*>& vec)
{
	vec.clear();

	IOperElement* pIndex = m_pOperElemList;
	while(pIndex != NULL)
	{
		IShape* pShape = dynamic_cast<IShape*>(pIndex);
		if (pShape != NULL)
			vec.push_back(pShape);

		pIndex = pIndex->Next();
	}
}


