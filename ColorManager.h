#pragma once
#include <vector>
class CColorManager
{
public:
	CColorManager(void);
	~CColorManager(void);
	static CColorManager* Instance(void);

public:
	Color* GetForegroundColor(void);
	Color* GetBackgroundColor(void);

protected:
	Color m_foregroundColor;
	Color m_backgroundColor;

	std::vector<HWND> m_vecHwnd;
};
