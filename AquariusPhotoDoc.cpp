// AquariusPhotoDoc.cpp : CAquariusPhotoDoc 类的实现
//

#include "stdafx.h"
#include "AquariusPhoto.h"

#include "AquariusPhotoDoc.h"
#include "ToolManager.h"
#include "MainFrm.h"
#include <algorithm>

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CAquariusPhotoDoc

IMPLEMENT_DYNCREATE(CAquariusPhotoDoc, CDocument)

BEGIN_MESSAGE_MAP(CAquariusPhotoDoc, CDocument)

END_MESSAGE_MAP()


// CAquariusPhotoDoc 构造/析构

CAquariusPhotoDoc::CAquariusPhotoDoc()
{
	m_pImage = NULL;
	m_bHasSelectRegion = FALSE;
}

CAquariusPhotoDoc::~CAquariusPhotoDoc()
{
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

	// set the ClipRect to the image rect
	m_clipRect.SetRect(0, 0, m_pImage->GetWidth(), m_pImage->GetHeight());

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

BOOL CAquariusPhotoDoc::Draw( Graphics* pGraphics )
{
	DrawImage(pGraphics);

	DrawShape(pGraphics);

	DrawSelectRegion(pGraphics);

	return TRUE;
}

BOOL CAquariusPhotoDoc::DrawImage( Graphics* pGraphics )
{
	//pGraphics->DrawImage(m_pImage, 0, 0);
	pGraphics->DrawImage(m_pImage, 0, 0, m_clipRect.left, m_clipRect.top, m_clipRect.Width(), m_clipRect.Height(), UnitPixel);

	return TRUE;
}

BOOL CAquariusPhotoDoc::DrawShape( Graphics* pGraphics )
{
	std::vector<IShape*>::iterator it = m_shapePtrList.begin();
	for(; it != m_shapePtrList.end(); it++)
		(*it)->DrawShape(pGraphics);

	return FALSE;
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
	if(p1.x < p2.x && p1.y < p2.y)
		m_selectRegioon.SetRect(p1, p2);
	else if (p1.x < p2.x && p1.y > p2.y)
		m_selectRegioon.SetRect(p1.x, p2.y, p2.x, p1.y);
	else if(p1.x > p2.x && p1.y < p2.y)
		m_selectRegioon.SetRect(p2.x, p1.y, p1.x, p2.y);
	else 
		m_selectRegioon.SetRect(p2.x, p2.y, p1.x, p1.y);

	m_bHasSelectRegion = TRUE;
	return TRUE;
}

Image* CAquariusPhotoDoc::GetImage()
{
	return m_pImage;
}

BOOL CAquariusPhotoDoc::RotateImage(CView* pView, RotateFlipType type)
{
	if (m_pImage == NULL)
		return FALSE;

	m_pImage->RotateFlip(type);

	// Clear Select Region
	ClearSelectRegion();

	pView->Invalidate(TRUE);

	return TRUE;
}

void CAquariusPhotoDoc::ClearSelectRegion()
{
	m_bHasSelectRegion = FALSE;
}

BOOL CAquariusPhotoDoc::InitClipRect()
{
	if (m_pImage == NULL)
		return FALSE;

	m_clipRect.SetRect(0, 0, m_pImage->GetWidth(), m_pImage->GetHeight());
	
	return TRUE;
}

BOOL CAquariusPhotoDoc::ResetClipRect()
{
	return InitClipRect();
}

BOOL CAquariusPhotoDoc::SetClipRect(CRect* pRc)
{
	m_clipRect = *pRc;

	return TRUE;
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

	rc = m_clipRect;

	return TRUE;
}

BOOL CAquariusPhotoDoc::UpdateStatusBarImageSize()
{
	CXTPStatusBar* pStatusBar = ((CMainFrame*)AfxGetMainWnd())->GetStatusBar();
	CXTPStatusBarPane* pPane = pStatusBar->FindPane(ID_INDICATOR_IMAGE_SIZE);
	if (!pPane)
		return FALSE;

	CString str;
	str.Format(_T("%d x %d 像素"), m_clipRect.Width(), m_clipRect.Height());

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

BOOL CAquariusPhotoDoc::AddShape( IShape* pShape )
{
	m_shapePtrList.push_back(pShape);

	return TRUE;
}

BOOL CAquariusPhotoDoc::RemoveShape( IShape* pShape )
{
	std::vector<IShape*>::iterator it = std::find(m_shapePtrList.begin(), m_shapePtrList.end(), pShape);
	if(it != m_shapePtrList.end())
	{
		m_shapePtrList.erase(it);
		delete pShape;
		return TRUE;
	}

	return FALSE;
}



#endif //_DEBUG


// CAquariusPhotoDoc 命令
