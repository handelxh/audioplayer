
// audioplayerDlg.h : ͷ�ļ�
//
#include "stdafx.h"
#include <string.h>
#include "Mmdeviceapi.h"   //IMMDevice interface
#include  "Audioclient.h" //WASAPI
#include "Audiopolicy.h"  //WASAPI interface
#include <EndpointVolume.h>
#include "CWaveFile.h"
#include <process.h>  
#include "windows.h"
extern unsigned int WINAPI CoreAudioRender(void *);
extern DWORD flags;
#pragma once


// CaudioplayerDlg �Ի���
class CaudioplayerDlg : public CDialogEx
{
// ����
public:

public:
	CaudioplayerDlg(CWnd* pParent = NULL);	// ��׼���캯��

// �Ի�������
	enum { IDD = IDD_AUDIOPLAYER_DIALOG };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV ֧��


// ʵ��
protected:
	HICON m_hIcon;

	// ���ɵ���Ϣӳ�亯��
	virtual BOOL OnInitDialog();
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedButton1();
	afx_msg void OnNMCustomdrawSliderScd(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnBnClickedstop();
};
