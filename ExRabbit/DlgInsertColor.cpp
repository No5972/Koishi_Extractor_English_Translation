// DlgInsert4.cpp : 实现文件
//

#include "stdafx.h"
#include "ExRabbit.h"
#include "DlgInsertColor.h"
#include "afxdialogex.h"
#include "ExRabbitDlg.h"

// CDlgInsertColor 对话框

IMPLEMENT_DYNAMIC(CDlgInsertColor, CDialogEx)

CDlgInsertColor::CDlgInsertColor(CWnd* pParent /*=NULL*/)
	: CDialogEx(CDlgInsertColor::IDD, pParent)
{
	getting = false;
}

CDlgInsertColor::~CDlgInsertColor()
{
}

void CDlgInsertColor::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_CHECK1, m_c1);
	DDX_Control(pDX, IDC_CHECK2, m_c2);
	DDX_Control(pDX, IDC_CHECK3, m_c3);
	DDX_Control(pDX, IDC_EDIT1, m_e1);
	DDX_Control(pDX, IDC_EDIT2, m_e2);
	DDX_Control(pDX, IDC_EDIT4, m_e3);
	DDX_Control(pDX, IDC_EDIT5, m_e4);
}


BEGIN_MESSAGE_MAP(CDlgInsertColor, CDialogEx)
	ON_BN_CLICKED(IDC_BUTTON1, &CDlgInsertColor::OnBnClickedButton1)
	ON_BN_CLICKED(IDOK, &CDlgInsertColor::OnBnClickedOk)
	ON_BN_CLICKED(IDCANCEL, &CDlgInsertColor::OnBnClickedCancel)
	ON_EN_CHANGE(IDC_EDIT2, &CDlgInsertColor::OnEnChangeEdit2)
	ON_EN_CHANGE(IDC_EDIT4, &CDlgInsertColor::OnEnChangeEdit4)
	ON_EN_CHANGE(IDC_EDIT5, &CDlgInsertColor::OnEnChangeEdit5)
	ON_BN_CLICKED(IDC_CHECK1, &CDlgInsertColor::OnBnClickedCheck1)
	ON_BN_CLICKED(IDC_CHECK2, &CDlgInsertColor::OnBnClickedCheck2)
	ON_BN_CLICKED(IDC_CHECK3, &CDlgInsertColor::OnBnClickedCheck3)
	ON_BN_CLICKED(IDC_BUTTON2, &CDlgInsertColor::OnBnClickedButton2)
	ON_WM_LBUTTONUP()
	ON_WM_MOUSEMOVE()
END_MESSAGE_MAP()


// CDlgInsertColor 消息处理程序


void CDlgInsertColor::OnBnClickedButton1()
{
	// TODO: 在此添加控件通知处理程序代码
	b8 r,g,b;
	CString cstr;
	m_e2.GetWindowText(cstr);
	r = (b8)_ttoi(cstr);
	m_e3.GetWindowText(cstr);
	g = (b8)_ttoi(cstr);
	m_e4.GetWindowText(cstr);
	b = (b8)_ttoi(cstr);
	COLORREF sclr = RGB(r,g,b);
	CColorDialog dlg(sclr, CC_FULLOPEN | CC_RGBINIT, this);
	if(dlg.DoModal()){
		sclr = dlg.GetColor();
		r = GetRValue(sclr);
		g = GetGValue(sclr);
		b = GetBValue(sclr);
		m_e2.SetWindowText(NumToCStr(r));
		m_e3.SetWindowText(NumToCStr(g));
		m_e4.SetWindowText(NumToCStr(b));
	}
}


void CDlgInsertColor::OnBnClickedOk()
{
	// TODO: 在此添加控件通知处理程序代码
	CExRabbitDlg* dlg = (CExRabbitDlg*)GetParent();
	int pos = 0;
	int clrID = dlg->m_cbPro.GetCurSel();
	if(clrID<0)
		clrID = 0;
	if(m_c1.GetCheck()){
		pos = dlg->m_lColor.GetSelectionMark();
	}else if(m_c2.GetCheck()){
		pos = dlg->m_lColor.GetSelectionMark()+1;
	}else if(m_c3.GetCheck()){
		pos = dlg->io.paletteData[clrID].size();
	}
	b8 a,r,g,b;
	CString cstr;
	m_e1.GetWindowText(cstr);
	a = (b8)_ttoi(cstr);
	m_e2.GetWindowText(cstr);
	r = (b8)_ttoi(cstr);
	m_e3.GetWindowText(cstr);
	g = (b8)_ttoi(cstr);
	m_e4.GetWindowText(cstr);
	b = (b8)_ttoi(cstr);
	color clr(a,r,g,b);
	if(iORr == __INSERT){
		dlg->io.CLRinsert(pos, clr, clrID);
		MessageBox(L"插入颜色完毕！",L"提示喵");
	}else{
		dlg->io.CLRreplace(pos, clr, clrID);
		MessageBox(L"替换颜色完毕！",L"提示喵");
	}
	dlg->updateCLRlist();
	dlg->updateNPKInfo();
	dlg->updatePICInfo();
	dlg->draw();
	dlg->dlgColor.setClr(&dlg->io.paletteData[dlg->m_cbPro.GetCurSel()]);
	dlg->dlgColor.draw();
	ShowWindow(SW_HIDE);
}


void CDlgInsertColor::OnBnClickedCancel()
{
	// TODO: 在此添加控件通知处理程序代码
	ShowWindow(SW_HIDE);
}


void CDlgInsertColor::OnEnChangeEdit2()
{
	// TODO:  如果该控件是 RICHEDIT 控件，它将不
	// 发送此通知，除非重写 CDialogEx::OnInitDialog()
	// 函数并调用 CRichEditCtrl().SetEventMask()，
	// 同时将 ENM_CHANGE 标志“或”运算到掩码中。
	draw();
	// TODO:  在此添加控件通知处理程序代码
}


void CDlgInsertColor::OnEnChangeEdit4()
{
	// TODO:  如果该控件是 RICHEDIT 控件，它将不
	// 发送此通知，除非重写 CDialogEx::OnInitDialog()
	// 函数并调用 CRichEditCtrl().SetEventMask()，
	// 同时将 ENM_CHANGE 标志“或”运算到掩码中。
	draw();
	// TODO:  在此添加控件通知处理程序代码
}


void CDlgInsertColor::OnEnChangeEdit5()
{
	// TODO:  如果该控件是 RICHEDIT 控件，它将不
	// 发送此通知，除非重写 CDialogEx::OnInitDialog()
	// 函数并调用 CRichEditCtrl().SetEventMask()，
	// 同时将 ENM_CHANGE 标志“或”运算到掩码中。
	draw();
	// TODO:  在此添加控件通知处理程序代码
}

void CDlgInsertColor::draw()
{
	b8 r,g,b;
	CString cstr;
	m_e2.GetWindowText(cstr);
	r = (b8)_ttoi(cstr);
	m_e3.GetWindowText(cstr);
	g = (b8)_ttoi(cstr);
	m_e4.GetWindowText(cstr);
	b = (b8)_ttoi(cstr);
	int i,j;
	CImage img;
	img.Create(180, 120, 32);
	UCHAR* pst = (UCHAR*)img.GetBits();
	int pit = img.GetPitch();
	CDC *pDC = GetDC();
	for(i=0;i<180;i++){
		for(j=0;j<120;j++){
			*(pst + pit*j + 4*i + 0) = b;
			*(pst + pit*j + 4*i + 1) = g;
			*(pst + pit*j + 4*i + 2) = r;
		}
	}
	////////////////////////
	if(iORr == __REPLACE){
		CExRabbitDlg* dlg = (CExRabbitDlg*)GetParent();
		int pos = dlg->m_lColor.GetSelectionMark();
		int clrID = dlg->m_cbPro.GetCurSel();
		if(clrID<0)
			clrID = 0;
		if(pos>=0){
			b = dlg->io.paletteData[clrID][pos].get_B();
			g = dlg->io.paletteData[clrID][pos].get_G();
			r = dlg->io.paletteData[clrID][pos].get_R();
			for(i=90;i<180;i++){
				for(j=0;j<120;j++){
					*(pst + pit*j + 4*i + 0) = b;
					*(pst + pit*j + 4*i + 1) = g;
					*(pst + pit*j + 4*i + 2) = r;
				}
			}
		}
	}

	img.Draw(pDC->m_hDC,150,30);
	img.Destroy();
	ReleaseDC(pDC);
}


void CDlgInsertColor::OnBnClickedCheck1()
{
	// TODO: 在此添加控件通知处理程序代码
	m_c1.SetCheck(1);
	m_c2.SetCheck(0);
	m_c3.SetCheck(0);
}


void CDlgInsertColor::OnBnClickedCheck2()
{
	// TODO: 在此添加控件通知处理程序代码
	m_c1.SetCheck(0);
	m_c2.SetCheck(1);
	m_c3.SetCheck(0);
}


void CDlgInsertColor::OnBnClickedCheck3()
{
	// TODO: 在此添加控件通知处理程序代码
	m_c1.SetCheck(0);
	m_c2.SetCheck(0);
	m_c3.SetCheck(1);
}


void CDlgInsertColor::OnBnClickedButton2()
{
	// TODO: 在此添加控件通知处理程序代码
	SetCapture();
	getting = true;
}


void CDlgInsertColor::OnLButtonUp(UINT nFlags, CPoint point)
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值
	getting = false;
	//CDialogEx::OnLButtonUp(nFlags,point);
	ReleaseCapture();
}


void CDlgInsertColor::OnMouseMove(UINT nFlags, CPoint point)
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值
	if(!getting)
		return;
	COLORREF clr;
	HDC hDC = ::GetDC(NULL);
	ClientToScreen(&point);
	clr = ::GetPixel(hDC, point.x, point.y);
	m_e2.SetWindowText(NumToCStr(GetRValue(clr)));
	m_e3.SetWindowText(NumToCStr(GetGValue(clr)));
	m_e4.SetWindowText(NumToCStr(GetBValue(clr)));
	draw();
	CDialogEx::OnMouseMove(nFlags, point);
}
