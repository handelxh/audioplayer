
// audioplayerDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "audioplayer.h"
#include "audioplayerDlg.h"
#include "afxdialogex.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif
FILE *frp; 
int scdu;
// CaudioplayerDlg 对话框




CaudioplayerDlg::CaudioplayerDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(CaudioplayerDlg::IDD, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CaudioplayerDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_SLIDER_SCD, schedule);
}

BEGIN_MESSAGE_MAP(CaudioplayerDlg, CDialogEx)
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(IDC_BUTTON1, &CaudioplayerDlg::OnBnClickedButton1)
	ON_NOTIFY(NM_CUSTOMDRAW, IDC_SLIDER_SCD, &CaudioplayerDlg::OnNMCustomdrawSliderScd)
END_MESSAGE_MAP()


// CaudioplayerDlg 消息处理程序

BOOL CaudioplayerDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// 设置此对话框的图标。当应用程序主窗口不是对话框时，框架将自动
	//  执行此操作
	SetIcon(m_hIcon, TRUE);			// 设置大图标
	SetIcon(m_hIcon, FALSE);		// 设置小图标

	// TODO: 在此添加额外的初始化代码
	schedule.SetRange( 0,100); 
    schedule.SetTicFreq( 1); 
    schedule.SetPos( 0); 
    // m_f##Key = (float)Pos;

	return TRUE;  // 除非将焦点设置到控件，否则返回 TRUE
}

// 如果向对话框添加最小化按钮，则需要下面的代码
//  来绘制该图标。对于使用文档/视图模型的 MFC 应用程序，
//  这将由框架自动完成。

void CaudioplayerDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // 用于绘制的设备上下文

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// 使图标在工作区矩形中居中
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// 绘制图标
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialogEx::OnPaint();
	}
}

//当用户拖动最小化窗口时系统调用此函数取得光标
//显示。
HCURSOR CaudioplayerDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}



void CaudioplayerDlg::OnBnClickedButton1()
{
	string fname = "F:\\github\\core-audio-apis\\wavsource\\caiqin.wav";
    if ((frp = fopen(fname.c_str(), "rb")) == NULL)
    {
        printf("can not open this wave file\n");
    }
    HANDLE handle = (HANDLE)_beginthreadex(NULL, 0, CoreAudioRender, NULL, 0, NULL);   
    //WaitForSingleObject(handle, INFINITE);  
}


void CaudioplayerDlg::OnNMCustomdrawSliderScd(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMCUSTOMDRAW pNMCD = reinterpret_cast<LPNMCUSTOMDRAW>(pNMHDR);
	// TODO: 在此添加控件通知处理程序代码
	*pResult = 0;
	scdu = schedule.GetPos();
}
