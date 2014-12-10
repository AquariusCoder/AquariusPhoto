#include "StdAfx.h"
#include "Tool_SelectRegion.h"
#include "AquariusPhotoDoc.h"
#include "ToolManager.h"
#include "MainFrm.h"
#include "resource.h"

CTool_SelectRegion::CTool_SelectRegion(void)
{
}

CTool_SelectRegion::~CTool_SelectRegion(void)
{
}

void CTool_SelectRegion::OnLButtonDown( CView* pView, UINT nFlags, CPoint& point )
{
	CToolBase::OnLButtonDown(pView, nFlags, point);
}

void CTool_SelectRegion::OnLButtonUp( CView* pView, UINT nFlages, CPoint& point )
{
	CToolBase::OnLButtonUp(pView, nFlages, point);
	CAquariusPhotoDoc* pDoc = dynamic_cast<CAquariusPhotoDoc*>(pView->GetDocument());
	ASSERT(pDoc);

	pDoc->SetSelectRegion(m_firstPoint, m_lastPoint);

	CRect selRc = pDoc->GetSelectRegion();
	if(selRc.Width() == 0 || selRc.Height() == 0)
		pDoc->ClearSelectRegion();

	pView->Invalidate(FALSE);

}

void CTool_SelectRegion::OnMouseMove( CView* pView, UINT nFlages, CPoint& point )
{
	if(!m_bFirstDown)
		return;

	CToolBase::OnMouseMove(pView, nFlages, point);
	CAquariusPhotoDoc* pDoc = dynamic_cast<CAquariusPhotoDoc*>(pView->GetDocument());
	ASSERT(pDoc);

	pDoc->SetSelectRegion(m_firstPoint, m_lastPoint);
	pView->Invalidate(FALSE);

	// update statusbar -- select size
	CXTPStatusBar* pStatusBar = ((CMainFrame*)AfxGetMainWnd())->GetStatusBar();
	CXTPStatusBarPane* pPane = pStatusBar->FindPane(ID_INDICATOR_SELECT_SIZE);
	if (!pPane)
		return;

	CString str;
	CRect selRc = pDoc->GetSelectRegion();
	str.Format(_T("%d x %d ÏñËØ"), selRc.Width(), selRc.Height());
	pPane->SetText((LPCTSTR)str);
}

void CTool_SelectRegion::OnLButtonDblClk( CView* pView, UINT nFlages, CPoint& point )
{
	
}

void CTool_SelectRegion::OnActivate(CView* pView)
{
	
}

void CTool_SelectRegion::OnInactivate(CView* pView)
{
	CAquariusPhotoDoc* pDoc = dynamic_cast<CAquariusPhotoDoc *>(pView->GetDocument());
	ASSERT(pDoc);

	pDoc->ClearSelectRegion();

	pView->Invalidate(FALSE);
}
