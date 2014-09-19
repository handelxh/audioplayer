
// audioplayerDlg.cpp : ʵ���ļ�
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
// CaudioplayerDlg �Ի���




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


// CaudioplayerDlg ��Ϣ�������

BOOL CaudioplayerDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// ���ô˶Ի����ͼ�ꡣ��Ӧ�ó��������ڲ��ǶԻ���ʱ����ܽ��Զ�
	//  ִ�д˲���
	SetIcon(m_hIcon, TRUE);			// ���ô�ͼ��
	SetIcon(m_hIcon, FALSE);		// ����Сͼ��

	// TODO: �ڴ���Ӷ���ĳ�ʼ������
	schedule.SetRange( 0,100); 
    schedule.SetTicFreq( 1); 
    schedule.SetPos( 0); 
    // m_f##Key = (float)Pos;

	return TRUE;  // ���ǽ��������õ��ؼ������򷵻� TRUE
}

// �����Ի��������С����ť������Ҫ����Ĵ���
//  �����Ƹ�ͼ�ꡣ����ʹ���ĵ�/��ͼģ�͵� MFC Ӧ�ó���
//  �⽫�ɿ���Զ���ɡ�

void CaudioplayerDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // ���ڻ��Ƶ��豸������

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// ʹͼ���ڹ����������о���
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// ����ͼ��
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialogEx::OnPaint();
	}
}

//���û��϶���С������ʱϵͳ���ô˺���ȡ�ù��
//��ʾ��
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
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	*pResult = 0;
	scdu = schedule.GetPos();
}
