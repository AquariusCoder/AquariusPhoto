#include "StdAfx.h"
#include "Element_Open.h"

CElement_Open::CElement_Open(Image* pImg)
{
	m_pImg = pImg;
}

CElement_Open::~CElement_Open(void)
{
}

BOOL CElement_Open::Do(Image** ppImg)
{
	if (*ppImg != NULL)
		delete *ppImg;

	*ppImg = m_pImg->Clone();

	return TRUE;
}
