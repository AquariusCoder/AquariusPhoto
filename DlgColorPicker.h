#pragma once
#include "resource.h"
#include "ColorButton.h"

// CDlgColorPicker

class CDlgColorPicker : public CXTPDialog
{
	DECLARE_DYNAMIC(CDlgColorPicker)

public:
	CDlgColorPicker(CWnd* pParent = NULL);  
	virtual ~CDlgColorPicker();

	enum { IDD = IDD_COLOR };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);
	virtual BOOL OnInitDialog(); 
	virtual BOOL PreTranslateMessage(MSG* pMsg);

	void UpdateFBColor();

	CColorButton m_fClrBtn;
	CColorButton m_bClrBtn;
	
	CSliderCtrl m_wndSliderR;
	CSliderCtrl m_wndSliderG;
	CSliderCtrl m_wndSliderB;

	CXTPEdit m_wndEditR;
	CXTPEdit m_wndEditG;
	CXTPEdit m_wndEditB;


	int m_r;
	int m_b;
	int m_g;
public:
	afx_msg void OnEnChangeEditR();
	afx_msg void OnEnChangeEditG();
	afx_msg void OnEnChangeEditB();
	afx_msg void OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar);
	afx_msg void OnStnClickedForegroundColor();
	afx_msg void OnStnClickedBackgroundColor();

	DECLARE_MESSAGE_MAP()
};
