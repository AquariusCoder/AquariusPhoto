// AquariusPhoto.h : AquariusPhoto Ӧ�ó������ͷ�ļ�
//
#pragma once

#ifndef __AFXWIN_H__
	#error "�ڰ������ļ�֮ǰ������stdafx.h�������� PCH �ļ�"
#endif

#include "resource.h"       // ������


// CAquariusPhotoApp:
// �йش����ʵ�֣������ AquariusPhoto.cpp
//

class CAquariusPhotoApp : public CWinApp
{
public:
	CAquariusPhotoApp();


// ��д
public:
	virtual BOOL InitInstance();
// ʵ��
	afx_msg void OnAppAbout();
	DECLARE_MESSAGE_MAP()
};

extern CAquariusPhotoApp theApp;