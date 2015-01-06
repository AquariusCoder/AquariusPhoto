#include "stdafx.h"
#include "AquariusPhoto.h"
#include "DlgColorPicker.h"
#include "ColorManager.h"


IMPLEMENT_DYNAMIC(CDlgColorPicker, CXTPDialog)

CDlgColorPicker::CDlgColorPicker(CWnd* pParent /*=NULL*/)
	: CXTPDialog(CDlgColorPicker::IDD, pParent)
{
	m_fClrBtn.SetColor(*CColorManager::Instance()->GetForegroundColor());
	m_bClrBtn.SetColor(*CColorManager::Instance()->GetBackgroundColor());
	
	m_fClrBtn.BindBuddy(&m_bClrBtn);

	m_r = CColorManager::Instance()->GetForegroundColor()->GetR();
	m_g = CColorManager::Instance()->GetForegroundColor()->GetG();
	m_b = CColorManager::Instance()->GetForegroundColor()->GetB();
}

CDlgColorPicker::~CDlgColorPicker()
{
}

void CDlgColorPicker::DoDataExchange(CDataExchange* pDX)
{
	CXTPDialog::DoDataExchange(pDX);

	DDX_Control(pDX, IDC_FOREGROUND_COLOR, m_fClrBtn);
	DDX_Control(pDX, IDC_BACKGROUND_COLOR, m_bClrBtn);
	DDX_Control(pDX, IDC_SLIDER_R, m_wndSliderR);
	DDX_Control(pDX, IDC_SLIDER_G, m_wndSliderG);
	DDX_Control(pDX, IDC_SLIDER_B, m_wndSliderB);
	DDX_Control(pDX, IDC_EDIT_CLR_R, m_wndEditR);
 	DDX_Control(pDX, IDC_EDIT_CLR_G, m_wndEditG);
 	DDX_Control(pDX, IDC_EDIT_CLR_B, m_wndEditB);

	DDX_Slider(pDX, IDC_SLIDER_R, m_r);
	DDX_Slider(pDX, IDC_SLIDER_G, m_g);
	DDX_Slider(pDX, IDC_SLIDER_B, m_b);

	CString str;
	int nValue = 0;
	m_wndEditR.GetWindowText(str);
	nValue = _wtoi(str);
	if (nValue == 0)
		m_wndEditR.SetWindowText(_T("0"));

	m_wndEditG.GetWindowText(str);
	nValue = _wtoi(str);
	if (nValue == 0)
		m_wndEditG.SetWindowText(_T("0"));

	m_wndEditB.GetWindowText(str);
	nValue = _wtoi(str);
	if (nValue == 0)
		m_wndEditB.SetWindowText(_T("0"));

 	DDX_Text(pDX, IDC_EDIT_CLR_R, m_r);
 	DDX_Text(pDX, IDC_EDIT_CLR_G, m_g);
 	DDX_Text(pDX, IDC_EDIT_CLR_B, m_b);
}


BEGIN_MESSAGE_MAP(CDlgColorPicker, CXTPDialog)
	ON_EN_CHANGE(IDC_EDIT_CLR_R, &CDlgColorPicker::OnEnChangeEditR)
	ON_EN_CHANGE(IDC_EDIT_CLR_G, &CDlgColorPicker::OnEnChangeEditG)
	ON_EN_CHANGE(IDC_EDIT_CLR_B, &CDlgColorPicker::OnEnChangeEditB)
	ON_WM_HSCROLL()
	ON_STN_CLICKED(IDC_FOREGROUND_COLOR, &CDlgColorPicker::OnStnClickedForegroundColor)
	ON_STN_CLICKED(IDC_BACKGROUND_COLOR, &CDlgColorPicker::OnStnClickedBackgroundColor)
END_MESSAGE_MAP()

BOOL CDlgColorPicker::OnInitDialog()
{
	BOOL bRt = CXTPDialog::OnInitDialog();

	m_wndSliderR.SetRange(0, 255);
	m_wndSliderG.SetRange(0, 255);
	m_wndSliderB.SetRange(0, 255);

	m_wndEditR.SetEditMask(_T("000"), _T("___"), _T("0"));
 	m_wndEditG.SetEditMask(_T("000"), _T("___"), _T("0"));
 	m_wndEditB.SetEditMask(_T("000"), _T("___"), _T("0"));

	UpdateFBColor();
	return bRt;
}

BOOL CDlgColorPicker::PreTranslateMessage(MSG* pMsg)
{
	if(pMsg->message == WM_KEYDOWN)
	{
		if (pMsg->wParam == VK_RETURN || pMsg->wParam == VK_ESCAPE)
			return TRUE;
	}

	return CXTPDialog::PreTranslateMessage(pMsg);
}


void CDlgColorPicker::OnEnChangeEditR()
{
	UpdateData(TRUE);

	if(m_r > 255)
		m_r = 255;

	UpdateFBColor();
	UpdateData(FALSE);
}

void CDlgColorPicker::OnEnChangeEditG()
{
	UpdateData(TRUE);

	if(m_g > 255)
		m_g = 255;

	UpdateFBColor();
	UpdateData(FALSE);
}

void CDlgColorPicker::OnEnChangeEditB()
{
	UpdateData(TRUE);

	if(m_b > 255)
		m_b = 255;

	UpdateFBColor();
	UpdateData(FALSE);
}

void CDlgColorPicker::UpdateFBColor()
{
	CColorButton* pBtn = m_fClrBtn.IsSelected() ? &m_fClrBtn : & m_bClrBtn;
	pBtn->SetColor(Color(m_r, m_g, m_b));
	pBtn->InvalidateWithBuddy(FALSE);

	if (m_fClrBtn.IsSelected())
		CColorManager::Instance()->GetForegroundColor()->SetValue(Color::MakeARGB(255, m_r, m_g, m_b));
	else
		CColorManager::Instance()->GetBackgroundColor()->SetValue(Color::MakeARGB(255, m_r, m_g, m_b));
}

void CDlgColorPicker::OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar)
{
	int nID = pScrollBar->GetDlgCtrlID();
	if (nID == IDC_SLIDER_R)
		m_r = m_wndSliderR.GetPos();
	else if (nID == IDC_SLIDER_G)
		m_g = m_wndSliderG.GetPos();
	else if(nID = IDC_SLIDER_B)
		m_b = m_wndSliderB.GetPos();

	UpdateFBColor();
	UpdateData(FALSE);
}

void CDlgColorPicker::OnStnClickedForegroundColor()
{
	Color* pClr = CColorManager::Instance()->GetForegroundColor();
	m_r = pClr->GetR();
	m_g = pClr->GetG();
	m_b = pClr->GetB();

	UpdateData(FALSE);
}


void CDlgColorPicker::OnStnClickedBackgroundColor()
{
	Color* pClr = CColorManager::Instance()->GetBackgroundColor();
	m_r = pClr->GetR();
	m_g = pClr->GetG();
	m_b = pClr->GetB();

	UpdateData(FALSE);
}
