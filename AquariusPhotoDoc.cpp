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

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


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

BOOL CAquariusPhotoDoc::Draw( Image* pImg )
{
	Graphics* pGraphics = Graphics::FromImage(pImg);

	DrawImage(pGraphics);

	DrawShape(pGraphics);

	DrawSelectRegion(pGraphics);

	return TRUE;
}

BOOL CAquariusPhotoDoc::DrawImage( Graphics* pGraphics )
{
	if (m_pImage == NULL)
		return FALSE;

	COperManager::Instance()->Do();

	pGraphics->DrawImage(COperManager::Instance()->GetImage(), 0, 0);

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

BOOL CAquariusPhotoDoc::OnClip()
{
	CElement_Clip* pElem = new CElement_Clip(m_selectRegioon);
	COperManager::Instance()->Add(pElem);

	return TRUE;
}

void CAquariusPhotoDoc::OnRedo(CView* pView)
{
	COperManager::Instance()->ReDo();

	pView->Invalidate(TRUE);
}

void CAquariusPhotoDoc::OnUndo(CView* pView)
{
	COperManager::Instance()->UnDo();

	pView->Invalidate(TRUE);
}


// CAquariusPhotoDoc 命令
