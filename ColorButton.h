#pragma once

class CColorButton : public CButton
{
public:
	CColorButton(void);
	~CColorButton(void);

	void SetColor(Color& clr);
	void BindBuddy(CColorButton* pBuddy);
	BOOL IsSelected();
	void InvalidateWithBuddy(BOOL bErase = TRUE);

protected:
	void SetBuddy(CColorButton* pBuddy);
	
	
protected:
	BOOL m_bForeground;
	CColorButton* m_pBuddy;
	BOOL m_bSel;
	Color m_color;

protected:
	afx_msg void OnPaint();
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	DECLARE_MESSAGE_MAP()
};
