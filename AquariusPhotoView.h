// AquariusPhotoView.h : CAquariusPhotoView ��Ľӿ�
//


#pragma once


class CAquariusPhotoView : public CView
{
protected: // �������л�����
	CAquariusPhotoView();
	DECLARE_DYNCREATE(CAquariusPhotoView)

// ����
public:
	CAquariusPhotoDoc* GetDocument() const;

// ����
public:

// ��д
public:
	virtual void OnDraw(CDC* pDC);

	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
	virtual BOOL OnPreparePrinting(CPrintInfo* pInfo);
	virtual void OnBeginPrinting(CDC* pDC, CPrintInfo* pInfo);
	virtual void OnEndPrinting(CDC* pDC, CPrintInfo* pInfo);


public:
	virtual ~CAquariusPhotoView();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:
	int m_nShape;

protected:
	afx_msg virtual void OnLButtonDown(UINT nFlages, CPoint point);
	afx_msg virtual void OnLButtonUp(UINT nFlages, CPoint point);
	afx_msg virtual void OnMouseMove(UINT nFlages, CPoint point);
	afx_msg virtual void OnRButtonDown(UINT nFlags, CPoint point);

	afx_msg virtual void OnSelectRegionRect();
	afx_msg virtual void OnSelectRegionAny();

	afx_msg virtual void OnClipImage();
	afx_msg virtual void OnResetSize();

	afx_msg virtual void OnRotateLeft();
	afx_msg virtual void OnRotateRight();
	afx_msg virtual void OnRotate180();
	afx_msg virtual void OnRotateHReverse();
	afx_msg virtual void OnRotateVReverse();

	afx_msg virtual void OnViewMenuCancelSelReg();

	afx_msg virtual void OnSelectShape();
	afx_msg virtual void OnGalleryShapes(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg virtual void OnUpdateGalleryShapes(CCmdUI* pCmdUI);

	DECLARE_MESSAGE_MAP()
};

#ifndef _DEBUG  // AquariusPhotoView.cpp �еĵ��԰汾
inline CAquariusPhotoDoc* CAquariusPhotoView::GetDocument() const
   { return reinterpret_cast<CAquariusPhotoDoc*>(m_pDocument); }
#endif

