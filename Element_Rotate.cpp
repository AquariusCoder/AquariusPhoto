#include "StdAfx.h"
#include "Element_Rotate.h"

CElement_Rotate::CElement_Rotate(RotateFlipType type)
{
	m_type = type;
}

CElement_Rotate::~CElement_Rotate(void)
{
}

BOOL CElement_Rotate::Do(Image** ppImg)
{
	Image* pImg = *ppImg;
	pImg->RotateFlip(m_type);

	return TRUE;
}

void CElement_Rotate::OnReDo()
{

}

void CElement_Rotate::OnUnDo()
{

}

