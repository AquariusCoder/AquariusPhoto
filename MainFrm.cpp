// MainFrm.cpp : CMainFrame 类的实现
//

#include "stdafx.h"
#include "AquariusPhoto.h"

#include "MainFrm.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CMainFrame

IMPLEMENT_DYNCREATE(CMainFrame, CXTPFrameWnd)

BEGIN_MESSAGE_MAP(CMainFrame, CXTPFrameWnd)
	ON_WM_CREATE()
	ON_XTP_CREATECONTROL()
END_MESSAGE_MAP()

static UINT indicators[] =
{
	ID_SEPARATOR,           // 状态行指示器
	ID_INDICATOR_CAPS,
	ID_INDICATOR_NUM,
	ID_INDICATOR_SCRL,
};


// CMainFrame 构造/析构

CMainFrame::CMainFrame()
{
	TCHAR szStylesPath[_MAX_PATH];

	VERIFY(::GetModuleFileName(
		AfxGetApp()->m_hInstance, szStylesPath, _MAX_PATH));		

	CString csStylesPath(szStylesPath);
	int nIndex  = csStylesPath.ReverseFind(_T('\\'));
	if (nIndex > 0) {
		csStylesPath = csStylesPath.Left(nIndex);
	}
	else {
		csStylesPath.Empty();
	}
	m_csStylesPath += csStylesPath + _T("\\Styles\\");
}

CMainFrame::~CMainFrame()
{
}


int CMainFrame::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CXTPFrameWnd::OnCreate(lpCreateStruct) == -1)
		return -1;

	if (!CreateStatusBar())
		return -1;

	if(!InitCommandBars())
		return -1;

	CXTPCommandBars* pCommandBars = GetCommandBars();
	m_wndStatusBar.SetCommandBars(pCommandBars);
	pCommandBars->SetTheme(xtpThemeRibbon);

	CXTPToolTipContext* pToolTipContext = pCommandBars->GetToolTipContext();
	pToolTipContext->SetStyle(xtpToolTipResource);
	pToolTipContext->ShowTitleAndDescription();
	pToolTipContext->ShowImage(TRUE, 0);
	pToolTipContext->SetMargin(CRect(2, 2, 2, 2));
	pToolTipContext->SetMaxTipWidth(180);
	pToolTipContext->SetFont(pCommandBars->GetPaintManager()->GetIconFont());
	pToolTipContext->SetDelayTime(TTDT_INITIAL, 900);

	// load Icons
	LoadIcons();

	pCommandBars->GetPaintManager()->m_bAutoResizeIcons = TRUE;

	pCommandBars->GetCommandBarsOptions()->bToolBarAccelTips = TRUE;

	pCommandBars->GetShortcutManager()->SetAccelerators(IDI_MAINFRAME);

	// Create Galleries
	CreateGalleries();

	// Create Ribbon
	CreateRibbon();

	// Docking panel
	CreateDockingPane();

	// SetStyle
	SetOptionsStyle();

	pCommandBars->GetCommandBarsOptions()->bShowKeyboardTips = TRUE;

	return 0;
}

BOOL CMainFrame::PreCreateWindow(CREATESTRUCT& cs)
{
	if( !CXTPFrameWnd::PreCreateWindow(cs) )
		return FALSE;
	
	cs.lpszClass = _T("XTPMainFrame");
	CXTPDrawHelpers::RegisterWndClass(AfxGetInstanceHandle(), cs.lpszClass, 
		CS_DBLCLKS, AfxGetApp()->LoadIcon(IDI_MAINFRAME));

	cs.dwExStyle &= ~WS_EX_CLIENTEDGE;

	return TRUE;
}


// CMainFrame 诊断

#ifdef _DEBUG
void CMainFrame::AssertValid() const
{
	CXTPFrameWnd::AssertValid();
}

void CMainFrame::Dump(CDumpContext& dc) const
{
	CXTPFrameWnd::Dump(dc);
}

#endif //_DEBUG

void CMainFrame::LoadIcons()
{
	CXTPCommandBars* pCommandBars = GetCommandBars();

	pCommandBars->GetImageManager()->SetIcons(IDR_LARGE_ICONS);
											  
	// Quick Access
	UINT uiQuickAccess[] = { ID_FILE_SAVE, ID_EDIT_REDO, ID_EDIT_UNDO };
	pCommandBars->GetImageManager()->SetIcons(IDR_QUICK_ACCESS, uiQuickAccess, _countof(uiQuickAccess), CSize(16, 16));

	// Image Group
	UINT uiSelectRegion[] = { ID_SELECT_REGION_RECT, ID_SELECT_REGION_ANY };
	pCommandBars->GetImageManager()->SetIcons(IDP_SELECT_REGION_16, uiSelectRegion, _countof(uiSelectRegion), CSize(16, 16));

	UINT uiGroupImage[] = { ID_CLIPE, ID_RESET_SIZE };
	pCommandBars->GetImageManager()->SetIcons(IDR_GROUP_IMAGE, uiGroupImage, _countof(uiGroupImage), CSize(16, 16));
	// Shape Group
	UINT uiShapeGroup[] = { ID_ROTATE_LEFT, ID_ROTATE_RIGHT, ID_ROTATE_180, ID_ROTATE_H_R, ID_ROTATE_V_R };
	pCommandBars->GetImageManager()->SetIcons(IDR_ROTATE_16, uiShapeGroup, _countof(uiShapeGroup), CSize(16, 16));
}

void CMainFrame::CreateRibbon()
{
	CXTPCommandBars* pCommandBars = GetCommandBars();

	CMenu menu;
	menu.Attach(::GetMenu(m_hWnd));
	SetMenu(NULL);

	// Create Ribbon
	CXTPRibbonBar* pRibbonBar = (CXTPRibbonBar*)pCommandBars->Add(_T("The Ribbon"), xtpBarTop, RUNTIME_CLASS(CXTPRibbonBar));
	if (!pRibbonBar)
		return;

	pRibbonBar->EnableDocking(0);

	// Menu
	CXTPControlPopup* pControlFile = (CXTPControlPopup*)pRibbonBar->AddSystemButton(ID_MENU_FILE);

	// Create Backstage
	CreateBackstage();

	// - AddTab
	CXTPRibbonTab* pTabHome = pRibbonBar->AddTab(ID_TAB_HOME);
	CXTPRibbonGroup* pGroupImg = pTabHome->AddGroup(ID_GROUP_IMAGE);
	CXTPControlPopup* pPopupSelectRegion = (CXTPControlPopup*)pGroupImg->Add(xtpControlSplitButtonPopup, ID_SELECT_REGION_RECT);
	pPopupSelectRegion->GetCommandBar()->GetControls()->Add(xtpControlButton, ID_SELECT_REGION_RECT);
	pPopupSelectRegion->GetCommandBar()->GetControls()->Add(xtpControlButton, ID_SELECT_REGION_ANY);

	pGroupImg->Add(xtpControlButton, ID_CLIPE);

	pGroupImg->Add(xtpControlButton, ID_RESET_SIZE);

 	CXTPControlPopup* pPopupRotate = (CXTPControlPopup*)pGroupImg->Add(xtpControlButtonPopup, ID_ROTATE_LEFT);
	

	CXTPRibbonGroup* pGroupShape = pTabHome->AddGroup(ID_GROUP_SHAPE);
	pGroupShape->Add(xtpControlButton, ID_SELECT_SHAPE);

	CXTPControlGallery* pControlGallery = (CXTPControlGallery*)pGroupShape->Add(new CXTPControlGallery(), ID_GALLERY_SHAPES);	
	pControlGallery->SetControlSize(CSize(138, 60));
	pControlGallery->SetResizable(FALSE, TRUE);
	pControlGallery->ShowBorders(TRUE);
	pControlGallery->SetItemsMargin(0, -1, 0, -1);
	pControlGallery->ShowLabels(FALSE);
	pControlGallery->SetItems(m_pItemsShapes);

	CMenu menuShapes;
	menuShapes.LoadMenu(ID_INSERT_SHAPES);

	CXTPPopupBar* pPopupBar = CXTPPopupBar::CreatePopupBar(GetCommandBars());
	pPopupBar->LoadMenu(menuShapes.GetSubMenu(0));

	pControlGallery->SetCommandBar(pPopupBar);
	pPopupBar->EnableAnimation();
	pPopupBar->InternalRelease();

	pRibbonBar->GetQuickAccessControls()->Add(xtpControlButton, ID_FILE_SAVE);
	pRibbonBar->GetQuickAccessControls()->Add(xtpControlButton, ID_EDIT_UNDO);
	pRibbonBar->GetQuickAccessControls()->Add(xtpControlButton, ID_EDIT_REDO);
	pRibbonBar->GetQuickAccessControls()->CreateOriginalControls();

	pRibbonBar->SetCloseable(FALSE);
	pRibbonBar->EnableFrameTheme();
	pRibbonBar->EnableCustomization(TRUE);

}

void CMainFrame::CreateDockingPane()
{
	m_paneManager.InstallDockingPanes(this);
	m_paneManager.SetTheme(xtpPaneThemeOffice2007Word);
	m_paneManager.SetThemedFloatingFrames(TRUE);
	m_paneManager.SetShowContentsWhileDragging(TRUE);
	m_paneManager.UseSplitterTracker(FALSE);

	CXTPDockingPaneMiniWnd::m_bShowPinButton = FALSE;

	CXTPDockingPane* pwndPane1 = m_paneManager.CreatePane(ID_LEFT_TOOL_BOX, CRect(0, 0,50, 120), xtpPaneDockLeft);
	pwndPane1->SetTitle(_T("工具"));
	pwndPane1->SetOptions(xtpPaneNoCloseable | xtpPaneNoCaption);

	//m_paneDlg.Create(CTreePaneDlg::IDD, this);
	//pwndPane1->Attach(&m_paneDlg);
}

void CMainFrame::SetOptionsStyle()
{
	CXTPCommandBars* pCommandBars = GetCommandBars();
	pCommandBars->SetTheme(xtpThemeRibbon);

	HMODULE hModule = AfxGetInstanceHandle();

	LPCTSTR lpszIniFile = 0;

	hModule = LoadLibrary(m_csStylesPath + _T("Office2010.dll"));;
	lpszIniFile = _T("OFFICE2010SILVER.INI"); 

	if (hModule != 0)
	{
		((CXTPOffice2007Theme*)GetCommandBars()->GetPaintManager())->SetImageHandle(hModule, lpszIniFile);
	}

	m_paneManager.SetTheme(xtpPaneThemeOffice2007Word);
	m_paneManager.GetPaintManager()->RefreshMetrics();
	m_paneManager.RedrawPanes();

	pCommandBars->GetImageManager()->RefreshAll();
	pCommandBars->RedrawCommandBars();
	SendMessage(WM_NCPAINT);

	RedrawWindow(0, 0, RDW_ALLCHILDREN|RDW_INVALIDATE);

}

void CMainFrame::CreateBackstage()
{
	CXTPRibbonBar* pRibbonBar = DYNAMIC_DOWNCAST(CXTPRibbonBar, GetCommandBars()->GetMenuBar());
	ASSERT (pRibbonBar);

	pRibbonBar->GetSystemButton()->SetStyle(xtpButtonCaption);

	CXTPRibbonControlSystemButton* pButton = pRibbonBar->GetSystemButton();

	CXTPRibbonBackstageView* pView = CXTPRibbonBackstageView::CreateBackstageView(GetCommandBars());

	pView->AddCommand(ID_FILE_SAVE);
	pView->AddCommand(ID_FILE_SAVE_AS);
	pView->AddCommand(ID_FILE_OPEN);

	// Add Pages

	pView->AddCommand(ID_APP_OPTIONS);
	pView->AddCommand(ID_APP_EXIT);

	UINT nIDIcons[] = {ID_FILE_SAVE, ID_FILE_SAVE_AS, ID_FILE_OPEN, ID_FILE_CLOSE, ID_APP_OPTIONS, ID_APP_EXIT};
	pView->GetImageManager()->SetIcons(IDB_BACKSTAGEICONS, nIDIcons, 6, CSize(0, 0));

	pButton->SetCommandBar(pView);

	pView->InternalRelease();

}

BOOL CMainFrame::CreateStatusBar()
{
	if (!m_wndStatusBar.Create(this))
	{
		TRACE0("Failed to create status bar\n");
		return FALSE;      // fail to create
	}

	int nPaddingLeft = 2;
	int nWidth = 150;
	CXTPStatusBarPane* pPane = NULL;
	// Position
	pPane = m_wndStatusBar.AddIndicator(ID_INDICATOR_POSITION);
	pPane->SetPadding(nPaddingLeft, 0, 8, 0);
	pPane->SetBeginGroup(TRUE);
	pPane->ShowBorders(FALSE);
	pPane->SetWidth(nWidth);
	// Draw Size
	pPane = m_wndStatusBar.AddIndicator(ID_INDICATOR_SELECT_SIZE);
	pPane->SetPadding(nPaddingLeft, 0, 8, 0);
	pPane->SetBeginGroup(TRUE);
	pPane->ShowBorders(FALSE);
	pPane->SetWidth(nWidth);
	// Image Size
	pPane = m_wndStatusBar.AddIndicator(ID_INDICATOR_IMAGE_SIZE);
	pPane->SetPadding(nPaddingLeft, 0, 8, 0);
	pPane->SetBeginGroup(TRUE);
	pPane->ShowBorders(FALSE);
	pPane->SetWidth(nWidth);
	// File Size
	pPane = m_wndStatusBar.AddIndicator(ID_INDICATOR_FILE_SIZE);
	pPane->SetPadding(nPaddingLeft, 0, 8, 0);
	pPane->SetBeginGroup(TRUE);
	pPane->ShowBorders(FALSE);
	pPane->SetStyle(SBPS_STRETCH | SBPS_NOBORDERS);
	pPane->SetWidth(nWidth);
	// Zoom
	pPane = m_wndStatusBar.AddIndicator(ID_INDICATOR_ZOOM);
	pPane->SetText(_T("100%"));
	pPane->SetTextAlignment(DT_CENTER);
	pPane->SetPadding(8, 0, 8, 0);
	pPane->SetWidth(pPane->GetBestFit());
	pPane->SetCaption(_T("&Zoom"));
	pPane->SetBeginGroup(FALSE);
	// Zoom Pane
	CXTPStatusBarSliderPane* pZoomPane = (CXTPStatusBarSliderPane*)m_wndStatusBar.AddIndicator(new CXTPStatusBarSliderPane(), ID_INDICATOR_ZOOMSLIDER);
	pZoomPane->SetBeginGroup(FALSE);
	pZoomPane->SetWidth(130);
	pZoomPane->SetPos(500); // We will convert 0 -> 10%, 500 -> 100 %, 1000 -> 500 %
	pZoomPane->SetRange(0, 1000);
	pZoomPane->SetCaption(_T("&Zoom Slider"));
	pZoomPane->SetTooltip(_T("Zoom"));
	pZoomPane->SetTooltipPart(XTP_HTSCROLLUP, _T("Zoom Out"));
	pZoomPane->SetTooltipPart(XTP_HTSCROLLDOWN, _T("Zoom In"));


	m_wndStatusBar.SetDrawDisabledText(FALSE);
	m_wndStatusBar.EnableCustomization();

	CXTPToolTipContext* pToolTipContext = m_wndStatusBar.GetToolTipContext();
	pToolTipContext->SetStyle(xtpToolTipResource);
	pToolTipContext->SetFont(m_wndStatusBar.GetPaintManager()->GetIconFont());

	return TRUE;
}

void CMainFrame::CreateGalleries()
{
	m_pItemsShapes = CXTPControlGalleryItems::CreateItems(GetCommandBars(), ID_GALLERY_SHAPES);
	m_pItemsShapes->SetItemSize(CSize(20, 20));

	m_pItemsShapes->GetImageManager()->SetIcons(ID_GALLERY_SHAPES, 0, 0, CSize(20, 20));

	CXTPControlGalleryItem* pItem = m_pItemsShapes->AddLabel(0);
	pItem->SetCaption(_T("Lines"));

	int nShape;
	for (nShape = 0; nShape < 12; nShape++)
		m_pItemsShapes->AddItem(nShape, nShape);

	pItem = m_pItemsShapes->AddLabel(0);
	pItem->SetCaption(_T("Basic Shapes"));

	for (; nShape < 12 + 32; nShape++)
		m_pItemsShapes->AddItem(nShape, nShape);

	pItem = m_pItemsShapes->AddLabel(0);
	pItem->SetCaption(_T("Block Arrows"));

	for (; nShape < 12 + 32 + 27; nShape++)
		m_pItemsShapes->AddItem(nShape, nShape);

	pItem = m_pItemsShapes->AddLabel(0);
	pItem->SetCaption(_T("Flowchart"));

	for (; nShape < 12 + 32 + 27 + 28; nShape++)
		m_pItemsShapes->AddItem(nShape, nShape);

	pItem = m_pItemsShapes->AddLabel(0);
	pItem->SetCaption(_T("Callouts"));

	for (; nShape < 12 + 32 + 27 + 28 + 20; nShape++)
		m_pItemsShapes->AddItem(nShape, nShape);

	pItem = m_pItemsShapes->AddLabel(0);
	pItem->SetCaption(_T("Stars and Banners"));

	for (; nShape < 12 + 32 + 27 + 28 + 20 + 16; nShape++)
		m_pItemsShapes->AddItem(nShape, nShape);

}


int CMainFrame::OnCreateControl( LPCREATECONTROLSTRUCT lpCreateControl )
{
	if (lpCreateControl->nID == ID_GALLERY_SHAPES)
	{
		CXTPControlGallery* pControlGallery = new CXTPControlGallery();
		pControlGallery->SetControlSize(CSize(200, 200));
		pControlGallery->SetResizable(TRUE, TRUE);
		pControlGallery->ShowLabels(TRUE);
		pControlGallery->SetItems(m_pItemsShapes);

		lpCreateControl->pControl = pControlGallery;
		return TRUE;
	}
	

	return TRUE;
}

CXTPStatusBar* CMainFrame::GetStatusBar()
{
	return &m_wndStatusBar;
}



// CMainFrame 消息处理程序



