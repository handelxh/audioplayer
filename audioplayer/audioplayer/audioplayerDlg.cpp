
// audioplayerDlg.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "audioplayer.h"
#include "audioplayerDlg.h"
#include "afxdialogex.h"
#include "define.h"
#include <string>
#ifdef _DEBUG
#define new DEBUG_NEW
#endif
using namespace  std;
// CaudioplayerDlg �Ի���
struct filedate fldta;
CSliderCtrl schedule;
int renderStatus = 0;
CaudioplayerDlg::CaudioplayerDlg(CWnd *pParent /*=NULL*/)
    : CDialogEx(CaudioplayerDlg::IDD, pParent)
{
    m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CaudioplayerDlg::DoDataExchange(CDataExchange *pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_SLIDER_SCD, schedule);
	DDX_Control(pDX, IDC_EDIT_SELC, selc);
}

BEGIN_MESSAGE_MAP(CaudioplayerDlg, CDialogEx)
    ON_WM_PAINT()
    ON_WM_QUERYDRAGICON()
    ON_BN_CLICKED(IDC_BUTTON1, &CaudioplayerDlg::OnBnClickedButton1)
    ON_NOTIFY(NM_CUSTOMDRAW, IDC_SLIDER_SCD, &CaudioplayerDlg::OnNMCustomdrawSliderScd)
    ON_BN_CLICKED(stop, &CaudioplayerDlg::OnBnClickedstop)
	ON_BN_CLICKED(IDC_BUTTON2, &CaudioplayerDlg::OnBnClickedButton2)
END_MESSAGE_MAP()


// CaudioplayerDlg ��Ϣ�������

BOOL CaudioplayerDlg::OnInitDialog()
{
    CDialogEx::OnInitDialog();

    // ���ô˶Ի����ͼ�ꡣ��Ӧ�ó��������ڲ��ǶԻ���ʱ����ܽ��Զ�
    //  ִ�д˲���
    SetIcon(m_hIcon, TRUE);         // ���ô�ͼ��
    SetIcon(m_hIcon, FALSE);        // ����Сͼ��

    // TODO: �ڴ���Ӷ���ĳ�ʼ������
    schedule.SetRange( 0, 100);
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



 HANDLE handle =NULL;
TCHAR FileName[FILENAME_MAX];
void CaudioplayerDlg::OnBnClickedButton1()
{
   
    // this->OnBnClickedstop();
     //flags = 0x02;
   //WaitForSingleObject(handle, INFINITE);

   // string fname = "F:\\github\\core-audio-apis\\wavsource\\caiqin.wav";
     //fname = FileName;_tfopen
    // Sleep(3000);
    if (renderStatus == 0 && FileName[0] != NULL)
    {

        if ((fldta.frp = _tfopen(FileName,_T("rb"))) == NULL)
        {
            printf("can not open this wave file\n");
        }
        BYTE datalen[4];
        fseek(fldta.frp, SUBCHUNK2SIZE, SEEK_SET);
        fread(datalen, sizeof(BYTE), 4, fldta.frp);
        fldta.datelen = ((int)datalen[3] << (8 * 3) ) | ((int)datalen[2] << (8 * 2)) | ((int)datalen[1] << 8) \
                        | ((int)datalen[0]);
        handle = CreateThread(NULL, 0, CoreAudioRender, NULL, 0, NULL);
    }
    //WaitForSingleObject(handle, INFINITE);
}


void CaudioplayerDlg::OnNMCustomdrawSliderScd(NMHDR *pNMHDR, LRESULT *pResult)
{
    LPNMCUSTOMDRAW pNMCD = reinterpret_cast<LPNMCUSTOMDRAW>(pNMHDR);
    // TODO: �ڴ���ӿؼ�֪ͨ����������
    *pResult = 0;
}



void CaudioplayerDlg::OnBnClickedstop()
{
    // TODO: �ڴ���ӿؼ�֪ͨ����������
    flags = 0x02;
}



void CaudioplayerDlg::OnBnClickedButton2()
{
    // TODO: �ڴ���ӿؼ�֪ͨ����������
    CString sFileType;
    sFileType = TEXT("*.wav|All Files (*.*)|*.*||");
    CFileDialog Dlg(TRUE, TEXT("*.wav"),  TEXT("*.wav"), OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT, sFileType);
    if (Dlg.DoModal() != IDOK)
    {
        return;
    }
     CString sFileName =Dlg.GetPathName();
    ULONG ulBufLen = Dlg.GetPathName().GetLength();
    _tcscpy( FileName, (LPCTSTR)Dlg.GetPathName());
	 flags = 0x02;
		selc.SetWindowText(sFileName);
		UpdateData(FALSE);
}
