#include "StdAfx.h"
#include "Interface.h"


IOperElement* OperElementBase::Prev()
{
	return m_prev;
}

IOperElement* OperElementBase::Next()
{
	return m_next;
}

int OperElementBase::GetID()
{
	return m_nID;
}

void OperElementBase::SetPrev(IOperElement* pElem)
{
	m_prev = pElem;
	m_nID = pElem->GetID() + 1;
}

void OperElementBase::SetNext(IOperElement* pElem)
{
	m_next = pElem;
}

OperElementBase::OperElementBase(void)
{
	m_nID = 0;
	m_prev = NULL;
	m_next = NULL;
}
