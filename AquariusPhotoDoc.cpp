// AquariusPhotoDoc.cpp : CAquariusPhotoDoc 类的实现
//

#include "stdafx.h"
#include "AquariusPhoto.h"

#include "AquariusPhotoDoc.h"
#include "ToolManager.h"
#include "MainFrm.h"
#include <algorithm>
#include "OperManager.h"
#include "Element_Rotate.h"
#include "Element_Clip.h"
#include "Element_Open.h"
#include "GeometryAlgorithm.h"

// #ifdef _DEBUG
// #define new DEBUG_NEW
// #endif


// CAquariusPhotoDoc

IMPLEMENT_DYNCREATE(CAquariusPhotoDoc, CDocument)

BEGIN_MESSAGE_MAP(CAquariusPhotoDoc, CDocument)
	//{{AFX_MSG_MAP(CAquariusPhotoDoc)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()


// CAquariusPhotoDoc 构造/析构

CAquariusPhotoDoc::CAquariusPhotoDoc()
{
	m_pImage = NULL;
	m_bHasSelectRegion = FALSE;

	m_dynamicShape = NULL;

	m_bFocusRect = FALSE;
	m_pFocusPen = new Pen(Color::Gray);
	m_pFocusPen->SetDashStyle(DashStyleDot);
}

CAquariusPhotoDoc::~CAquariusPhotoDoc()
{
	if(m_dynamicShape != NULL)
	{
		delete m_dynamicShape;
		m_dynamicShape = NULL;
	}
}

BOOL CAquariusPhotoDoc::OnNewDocument()
{
	if (!CDocument::OnNewDocument())
		return FALSE;

	return TRUE;
}

BOOL CAquariusPhotoDoc::OnOpenDocument( LPCTSTR lpszPathName )
{
	CDocument::OnOpenDocument(lpszPathName);
	if(m_pImage != NULL)
		delete m_pImage;

	m_pImage = Image::FromFile(lpszPathName);

	// clear operations
	COperManager::Instance()->Clear();
	
	// open operation
	CElement_Open* pElem = new CElement_Open(m_pImage);
	COperManager::Instance()->Add(pElem);

	// update status bar -- image size
	UpdateStatusBarImageSize();

	// update status bar -- file size
	UpdateStatusBarFileSize(lpszPathName);

	return TRUE;
}


// CAquariusPhotoDoc 序列化

void CAquariusPhotoDoc::Serialize(CArchive& ar)
{
	if (ar.IsStoring())
	{
		
	}
	else
	{
		
	}
}


// CAquariusPhotoDoc 诊断

#ifdef _DEBUG
void CAquariusPhotoDoc::AssertValid() const
{
	CDocument::AssertValid();
}

void CAquariusPhotoDoc::Dump(CDumpContext& dc) const
{
	CDocument::Dump(dc);
}

#endif //_DEBUG

BOOL CAquariusPhotoDoc::Draw( Graphics* pGraphics )
{
	if (m_pImage == NULL)
		return FALSE;

	// draw operation queue
	COperManager::Instance()->Do();
	pGraphics->DrawImage(COperManager::Instance()->GetImage(), 0, 0);

	// draw select region
	DrawSelectRegion(pGraphics);

	// draw dynamic shape
	if (m_dynamicShape != NULL)	
		m_dynamicShape->DrawShape(pGraphics);

	// draw focus Rect
	if (m_bFocusRect)
		pGraphics->DrawRectangle(m_pFocusPen, Rect(m_focusRect.left, m_focusRect.top, m_focusRect.Width(), m_focusRect.Height()));

	return TRUE;
}

BOOL CAquariusPhotoDoc::DrawSelectRegion( Graphics* pGraphics )
{
	if (!m_bHasSelectRegion)
		return TRUE;

	Pen pen(Color::Black);
	pen.SetDashStyle(DashStyleDashDot);
	pGraphics->DrawRectangle(&pen, m_selectRegioon.left, m_selectRegioon.top, m_selectRegioon.Width(), m_selectRegioon.Height());

	return TRUE;
}

BOOL CAquariusPhotoDoc::SetSelectRegion( CPoint& p1, CPoint& p2 )
{
	::SetRectByPoint(p1.x, p1.y, p2.x, p2.y, m_selectRegioon);

	m_bHasSelectRegion = TRUE;
	return TRUE;
}

Image* CAquariusPhotoDoc::GetImage()
{
	return m_pImage;
}

BOOL CAquariusPhotoDoc::OnRotateImage(CView* pView, RotateFlipType type)
{
	if (m_pImage == NULL)
		return FALSE;

	CElement_Rotate* pElem = new CElement_Rotate(type);
	COperManager::Instance()->Add(pElem);

	// Clear Select Region
	ClearSelectRegion();

	pView->Invalidate(TRUE);

	return TRUE;
}

void CAquariusPhotoDoc::ClearSelectRegion()
{
	m_bHasSelectRegion = FALSE;
}

BOOL CAquariusPhotoDoc::HasSelectRegion()
{
	return m_bHasSelectRegion;
}

CRect& CAquariusPhotoDoc::GetSelectRegion()
{
	return m_selectRegioon;
}

BOOL CAquariusPhotoDoc::HasOpened()
{
	return m_pImage != NULL;
}

BOOL CAquariusPhotoDoc::GetDrawImageRect( CRect& rc )
{
	if (!HasOpened())
		return FALSE;

	Image* pImg = COperManager::Instance()->GetImage();
	if (pImg == NULL)
		return FALSE;

	rc.SetRect(0, 0, pImg->GetWidth(), pImg->GetHeight());

	return TRUE;
}

BOOL CAquariusPhotoDoc::UpdateStatusBarImageSize()
{
	CXTPStatusBar* pStatusBar = ((CMainFrame*)AfxGetMainWnd())->GetStatusBar();
	CXTPStatusBarPane* pPane = pStatusBar->FindPane(ID_INDICATOR_IMAGE_SIZE);
	if (!pPane)
		return FALSE;

	Image* pImg = COperManager::Instance()->GetImage();
	if (pImg == NULL)
		return FALSE;

	CString str;
	str.Format(_T("%d x %d 像素"), pImg->GetWidth(), pImg->GetHeight());

	pPane->SetText((LPCTSTR)str);

	return TRUE;
}

BOOL CAquariusPhotoDoc::UpdateStatusBarFileSize(LPCTSTR lpszPathName)
{
	CXTPStatusBar* pStatusBar = ((CMainFrame*)AfxGetMainWnd())->GetStatusBar();
	CXTPStatusBarPane* pPane = pStatusBar->FindPane(ID_INDICATOR_FILE_SIZE);
	if (!pPane)
		return FALSE;

	CString str;
	CFileStatus stat;
	ZeroMemory(&stat, sizeof(stat));
	CFile::GetStatus(lpszPathName, stat);

	str.Format(_T("大小：%.1f KB"), (double)stat.m_size / 1024);

	pPane->SetText((LPCTSTR)str);

	return TRUE;
}



BOOL CAquariusPhotoDoc::OnClip()
{
	CElement_Clip* pElem = new CElement_Clip(m_selectRegioon);
	COperManager::Instance()->Add(pElem);

	return TRUE;
}

void CAquariusPhotoDoc::OnRedo(CView* pView)
{
	COperManager::Instance()->ReDo();

	pView->Invalidate(FALSE);
}

void CAquariusPhotoDoc::OnUndo(CView* pView)
{
	COperManager::Instance()->UnDo();

	pView->Invalidate(FALSE);
}

void CAquariusPhotoDoc::SetDynamicShape(IShape* pShap)
{
	if(m_dynamicShape != NULL)
	{
		m_dynamicShape->Erase();
		m_dynamicShape = NULL;
	}

	m_dynamicShape = pShap;
}

void CAquariusPhotoDoc::SetFocusRect(CRect* pRect)
{
	if (pRect == NULL)
		m_bFocusRect = FALSE;
	else
	{
		m_bFocusRect = TRUE;
		m_focusRect = *pRect;
	}
}


// CAquariusPhotoDoc 命令
