// MainFrm.h : CMainFrame 类的接口
//


#pragma once

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

// 实现
public:
	virtual ~CMainFrame();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:  // 控件条嵌入成员
	CXTPDockingPaneManager		m_paneManager;
	CXTPStatusBar				m_wndStatusBar;
	CToolBar					m_wndToolBar;
	CString						m_csStylesPath;

	CXTPControlGalleryItems*	m_pItemsShapes;

// 生成的消息映射函数
protected:
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);

	DECLARE_MESSAGE_MAP()
};


