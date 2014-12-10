// AquariusPhotoView.cpp : CAquariusPhotoView 类的实现
//

#include "stdafx.h"
#include "AquariusPhoto.h"

#include "AquariusPhotoDoc.h"
#include "AquariusPhotoView.h"
#include "ToolManager.h"
#include "MainFrm.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


IMPLEMENT_DYNCREATE(CAquariusPhotoView, CView)

BEGIN_MESSAGE_MAP(CAquariusPhotoView, CView)
	ON_COMMAND(ID_FILE_PRINT, &CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_DIRECT, &CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_PREVIEW, &CView::OnFilePrintPreview)

	ON_COMMAND(ID_EDIT_REDO, &CAquariusPhotoView::OnEditRedo)
	ON_COMMAND(ID_EDIT_UNDO, &CAquariusPhotoView::OnEditUndo)

	ON_COMMAND(ID_SELECT_REGION_RECT, &CAquariusPhotoView::OnSelectRegionRect)
	ON_COMMAND(ID_SELECT_REGION_ANY, &CAquariusPhotoView::OnSelectRegionAny)

	ON_COMMAND(ID_CLIPE, &CAquariusPhotoView::OnClipImage)
	ON_COMMAND(ID_RESET_SIZE, &CAquariusPhotoView::OnResetSize)

	ON_COMMAND(ID_ROTATE_LEFT, &CAquariusPhotoView::OnRotateLeft)
	ON_COMMAND(ID_ROTATE_RIGHT, &CAquariusPhotoView::OnRotateRight)
	ON_COMMAND(ID_ROTATE_180, &CAquariusPhotoView::OnRotate180)
	ON_COMMAND(ID_ROTATE_H_R, &CAquariusPhotoView::OnRotateHReverse)
	ON_COMMAND(ID_ROTATE_V_R, &CAquariusPhotoView::OnRotateVReverse)

	ON_COMMAND(ID_SELECT_SHAPE, &CAquariusPhotoView::OnSelectShapeTool)
	ON_COMMAND(ID_CANCEL_SEL_REGION, &CAquariusPhotoView::OnViewMenuCancelSelReg)

	ON_XTP_EXECUTE(ID_GALLERY_SHAPES, OnGalleryShapes)
	ON_UPDATE_COMMAND_UI(ID_GALLERY_SHAPES, OnUpdateGalleryShapes)

	ON_WM_LBUTTONDOWN()
	ON_WM_LBUTTONUP()
	ON_WM_MOUSEMOVE()
	ON_WM_RBUTTONDOWN()
END_MESSAGE_MAP()


CAquariusPhotoView::CAquariusPhotoView()
{
	m_nShape = -1;

}

CAquariusPhotoView::~CAquariusPhotoView()
{
}

BOOL CAquariusPhotoView::PreCreateWindow(CREATESTRUCT& cs)
{
	return CView::PreCreateWindow(cs);
}


void CAquariusPhotoView::OnDraw(CDC* pDC)
{
	CAquariusPhotoDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	if (!pDoc)
		return;

	CRect rc;
	GetClientRect(rc);
	
	// double buffer
	Bitmap bmp(rc.Width(), rc.Height());
	Graphics* pGraphics = Graphics::FromImage(&bmp);
	SolidBrush sBrush(Color::White);
	pGraphics->FillRectangle(&sBrush, 0, 0, rc.Width(), rc.Height()); // erase the backgroud
	pDoc->Draw(pGraphics);

	// start draw
	Graphics graphics(pDC->m_hDC);
	graphics.DrawImage(&bmp, 0, 0);
	graphics.ReleaseHDC(pDC->m_hDC); // need to call ?

}


BOOL CAquariusPhotoView::OnPreparePrinting(CPrintInfo* pInfo)
{
	return DoPreparePrinting(pInfo);
}

void CAquariusPhotoView::OnBeginPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	
}

void CAquariusPhotoView::OnEndPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	
}


#ifdef _DEBUG
void CAquariusPhotoView::AssertValid() const
{
	CView::AssertValid();
}

void CAquariusPhotoView::Dump(CDumpContext& dc) const
{
	CView::Dump(dc);
}

CAquariusPhotoDoc* CAquariusPhotoView::GetDocument() const // 非调试版本是内联的
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CAquariusPhotoDoc)));
	return (CAquariusPhotoDoc*)m_pDocument;
}

#endif //_DEBUG

void CAquariusPhotoView::OnSelectRegionRect()
{
	CToolManager::Instance()->SetToolMode(this, TM_SELECT_REGION_RECT);
}

void CAquariusPhotoView::OnSelectRegionAny()
{
	CToolManager::Instance()->SetToolMode(this, TM_SELECT_REGION_RECT);
}

void CAquariusPhotoView::OnRotateLeft()
{
	CAquariusPhotoDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	if (!pDoc)
		return;

	pDoc->OnRotateImage(this, Rotate270FlipNone);
}

void CAquariusPhotoView::OnRotateRight()
{
	CAquariusPhotoDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	if (!pDoc)
		return;

	pDoc->OnRotateImage(this, Rotate90FlipNone);
}

void CAquariusPhotoView::OnRotate180()
{
	CAquariusPhotoDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	if (!pDoc)
		return;

	pDoc->OnRotateImage(this, Rotate180FlipNone);
}

void CAquariusPhotoView::OnRotateVReverse()
{
	CAquariusPhotoDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	if (!pDoc)
		return;

	pDoc->OnRotateImage(this, Rotate180FlipX);
}

void CAquariusPhotoView::OnRotateHReverse()
{
	CAquariusPhotoDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	if (!pDoc)
		return;

	pDoc->OnRotateImage(this, Rotate180FlipY);
}

void CAquariusPhotoView::OnLButtonDown( UINT nFlages, CPoint point )
{
	SetCapture();
	CToolManager::Instance()->OnLButtonDown(this, nFlages, point);
}

void CAquariusPhotoView::OnLButtonUp( UINT nFlages, CPoint point )
{
	ReleaseCapture();
	CToolManager::Instance()->OnLButtonUp(this, nFlages, point);
}

void CAquariusPhotoView::OnMouseMove( UINT nFlages, CPoint point )
{
	CToolManager::Instance()->OnMouseMove(this, nFlages, point);

	// update statusbar -- position
	CXTPStatusBar* pStatusBar = ((CMainFrame*)AfxGetMainWnd())->GetStatusBar();
	CXTPStatusBarPane* pPane = pStatusBar->FindPane(ID_INDICATOR_POSITION);
	if (!pPane)
		return;

	CString str;
	str.Format(_T("%d, %d 像素"), point.x, point.y);
	pPane->SetText((LPCTSTR)str);

}

void CAquariusPhotoView::OnClipImage()
{
	CAquariusPhotoDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	if (!pDoc)
		return;

	if (!pDoc->HasSelectRegion())
		return;

	pDoc->OnClip();
	pDoc->ClearSelectRegion();

	Invalidate(TRUE);
}

void CAquariusPhotoView::OnResetSize()
{
	
}

void CAquariusPhotoView::OnGalleryShapes( NMHDR* pNMHDR, LRESULT* pResult )
{
	NMXTPCONTROL* tagNMCONTROL = (NMXTPCONTROL*)pNMHDR;

	CXTPControlGallery* pGallery = DYNAMIC_DOWNCAST(CXTPControlGallery, tagNMCONTROL->pControl);

	if (pGallery)
	{
		CXTPControlGalleryItem* pItem = pGallery->GetItem(pGallery->GetSelectedItem());
		if (pItem)
		{
			m_nShape = pItem->GetID();

			CToolManager::Instance()->SetToolMode(this, TM_DRAW_LINE);
		}

		*pResult = TRUE; // Handled
	}
}

void CAquariusPhotoView::OnUpdateGalleryShapes( CCmdUI* pCmdUI )
{
	CXTPControlGallery* pGallery = 	DYNAMIC_DOWNCAST(CXTPControlGallery, CXTPControl::FromUI(pCmdUI));

	if (pGallery)
	{
		pGallery->SetCheckedItem(m_nShape);
	}
	pCmdUI->Enable(TRUE);
}

void CAquariusPhotoView::OnRButtonDown( UINT nFlags, CPoint point )
{
	CMenu menu;
	menu.LoadMenu(IDR_MENU_VIEW);
	CMenu* pMenu = menu.GetSubMenu(0);
	ClientToScreen(&point);

	pMenu->TrackPopupMenu(TPM_LEFTALIGN | TPM_TOPALIGN | TPM_RIGHTBUTTON, point.x, point.y, this);

	
}

void CAquariusPhotoView::OnViewMenuCancelSelReg()
{
	CAquariusPhotoDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	if(!pDoc)
		return;

	pDoc->ClearSelectRegion();

	Invalidate(FALSE);
}

void CAquariusPhotoView::OnEditRedo()
{
	CAquariusPhotoDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	if(!pDoc)
		return;

	pDoc->OnRedo(this);
}

void CAquariusPhotoView::OnEditUndo()
{
	CAquariusPhotoDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	if(!pDoc)
		return;

	pDoc->OnUndo(this);
}

void CAquariusPhotoView::OnSelectShapeTool()
{
	CToolManager::Instance()->SetToolMode(this, TM_SELECT_SHAPE);
}




// CAquariusPhotoView 消息处理程序
