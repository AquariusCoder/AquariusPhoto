#include "StdAfx.h"
#include "Element_Clip.h"

CElement_Clip::CElement_Clip(CRect& rc)
{
	m_rc = rc;
}

CElement_Clip::~CElement_Clip(void)
{
}

BOOL CElement_Clip::Do(Image** ppImg)
{
	Image* pImg = *ppImg;
	*ppImg = new Bitmap(m_rc.Width(), m_rc.Height());
	Graphics* pGraphics = Graphics::FromImage(*ppImg);
	pGraphics->DrawImage(pImg, 0, 0, m_rc.left, m_rc.top, m_rc.Width(), m_rc.Height(), UnitPixel);
	delete pImg;

	return TRUE;
}

void CElement_Clip::OnReDo()
{

}

void CElement_Clip::OnUnDo()
{

}
