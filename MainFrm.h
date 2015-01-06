// MainFrm.h : CMainFrame ��Ľӿ�
//


#pragma once
#include "DlgColorPicker.h"

class CMainFrame : public CXTPFrameWnd
{
	CMainFrame();
	DECLARE_DYNCREATE(CMainFrame)

public:
	CXTPStatusBar* GetStatusBar();

public:
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
	virtual int OnCreateControl(LPCREATECONTROLSTRUCT lpCreateControl);

protected:
	BOOL CreateStatusBar();
	void LoadIcons();
	void CreateGalleries();
	void CreateRibbon();
	void CreateBackstage();
	void CreateDockingPane();
	void SetOptionsStyle();

// ʵ��
public:
	virtual ~CMainFrame();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:  // �ؼ���Ƕ���Ա
	CXTPDockingPaneManager		m_paneManager;
	CXTPStatusBar				m_wndStatusBar;
	CToolBar					m_wndToolBar;
	CString						m_csStylesPath;

	CXTPControlGalleryItems*	m_pItemsShapes;

	CDlgColorPicker m_paneColorPicker;

// ���ɵ���Ϣӳ�亯��
protected:
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);

	DECLARE_MESSAGE_MAP()
};


