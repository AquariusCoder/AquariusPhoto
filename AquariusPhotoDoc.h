// AquariusPhotoDoc.h : CAquariusPhotoDoc Interface
//


#pragma once
#include <vector>
#include "Interface.h"

class CAquariusPhotoDoc : public CDocument
{
protected:
	CAquariusPhotoDoc();
	virtual ~CAquariusPhotoDoc();
	DECLARE_DYNCREATE(CAquariusPhotoDoc);	

public:
	BOOL Draw(Graphics* pGraphics);
	BOOL DrawSelectRegion(Graphics* pGraphics);
	BOOL DrawImage(Graphics* pGraphics);
	BOOL DrawShape(Graphics* pGraphics);

	BOOL RotateImage(CView* pView, RotateFlipType type);

	BOOL UpdateStatusBarImageSize();
	BOOL UpdateStatusBarFileSize(LPCTSTR lpszPathName);

public:
	virtual BOOL OnNewDocument();
	virtual BOOL OnOpenDocument(LPCTSTR lpszPathName);
	virtual void Serialize(CArchive& ar);

public:
	Image* GetImage();
	BOOL HasOpened();
	BOOL GetDrawImageRect(CRect& rc);

	BOOL SetSelectRegion(CPoint& p1, CPoint& p2);
	CRect& GetSelectRegion();
	void ClearSelectRegion();
	BOOL HasSelectRegion();

	BOOL ResetClipRect();
	BOOL SetClipRect(CRect* pRc);

	// shape 
	BOOL AddShape(IShape* pShape);
	BOOL RemoveShape(IShape* pShape);

protected:
	BOOL InitClipRect();


#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:
	BOOL m_bHasSelectRegion;
	CRect m_selectRegioon;
	Image* m_pImage;
	CRect m_clipRect;

	std::vector<IShape*> m_shapePtrList;

protected:
	DECLARE_MESSAGE_MAP()
};


