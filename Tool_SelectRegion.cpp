#include "StdAfx.h"
#include "Tool_SelectRegion.h"
#include "AquariusPhotoDoc.h"
#include "ToolManager.h"
#include "MainFrm.h"
#include "resource.h"

CTool_SelectRegion::CTool_SelectRegion(void)
{
	m_bFirstDown = FALSE;
}

CTool_SelectRegion::~CTool_SelectRegion(void)
{
}

void CTool_SelectRegion::OnLButtonDown( CView* pView, UINT nFlags, CPoint point )
{
	CAquariusPhotoDoc* pDoc = dynamic_cast<CAquariusPhotoDoc *>(pView->GetDocument());
	if(pDoc == NULL)
		return;

	CRect rc;
	if (!pDoc->GetDrawImageRect(rc))
		return;

	AdjustPosition(point, rc);

	m_firstPoint = point;
	m_bFirstDown = TRUE;
}

void CTool_SelectRegion::OnLButtonUp( CView* pView, UINT nFlages, CPoint point )
{
	m_lastPoint = point;
	CAquariusPhotoDoc* pDoc = dynamic_cast<CAquariusPhotoDoc *>(pView->GetDocument());
	if(pDoc == NULL)
		return;

	CRect rc;
	if (!pDoc->GetDrawImageRect(rc))
		return;

	AdjustPosition(point, rc);

	m_lastPoint = point;
	pDoc->SetSelectRegion(m_firstPoint, m_lastPoint);

	CRect selRc = pDoc->GetSelectRegion();
	if(selRc.Width() == 0 || selRc.Height() == 0)
		pDoc->ClearSelectRegion();

	m_bFirstDown = FALSE;

	pView->Invalidate(FALSE);

}

void CTool_SelectRegion::OnMouseMove( CView* pView, UINT nFlages, CPoint point )
{
	if(!m_bFirstDown)
		return;

	CAquariusPhotoDoc* pDoc = dynamic_cast<CAquariusPhotoDoc *>(pView->GetDocument());
	if(pDoc == NULL)
		return;

	CRect rc;
	if (!pDoc->GetDrawImageRect(rc))
		return;

	// Adjust Position when out of Image
	AdjustPosition(point, rc);

	m_lastPoint = point;
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

void CTool_SelectRegion::OnLButtonDblClk( CView* pView, UINT nFlages, CPoint point )
{
	
}

void CTool_SelectRegion::AdjustPosition( CPoint& point, CRect& rc )
{
	if (point.x > rc.Width())
		point.x = rc.Width();

	if (point.y > rc.Height())
		point.y = rc.Height();

	if (point.x < 0)
		point.x = 0;
	
	if (point.y < 0)
		point.y = 0;
}
