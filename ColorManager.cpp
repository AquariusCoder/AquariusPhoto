#include "StdAfx.h"
#include "ColorManager.h"

CColorManager g_colorManager;

CColorManager::CColorManager(void)
{
	m_foregroundColor = Color::Black;
	m_backgroundColor = Color::White;
}

CColorManager::~CColorManager(void)
{
}

CColorManager* CColorManager::Instance(void)
{
	return &g_colorManager;
}

Color* CColorManager::GetForegroundColor(void)
{
	return &m_foregroundColor;
}

Color* CColorManager::GetBackgroundColor(void)
{
	return &m_backgroundColor;
}

