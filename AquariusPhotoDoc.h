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
	BOOL DrawSelectRegion(Graphics* pGraphics);;

	void OnRedo(CView* pView);
	void OnUndo(CView* pView);
	BOOL OnRotateImage(CView* pView, RotateFlipType type);
	BOOL OnClip();

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

	// shape 
	void SetDynamicShape(IShape* pShap);
	void SetDynamicShape(std::vector<IShape*> vec);
	void ClearDynamicShape();
	void SetFocusRect(CRect* pRect);

#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:
	BOOL m_bHasSelectRegion;
	CRect m_selectRegioon;
	Image* m_pImage;

	std::vector<IShape*> m_dynamicShapeList;

	BOOL m_bFocusRect;
	CRect m_focusRect;
	Pen* m_pFocusPen;

protected:
	DECLARE_MESSAGE_MAP()
};


