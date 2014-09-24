
// audioplayerDlg.h : 头文件
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
#include "afxwin.h"
extern DWORD  WINAPI CoreAudioRender(LPVOID pM);
extern volatile DWORD flags;
#pragma once

// CaudioplayerDlg 对话框
class CaudioplayerDlg : public CDialogEx
{
// 构造
public:

public:
	CaudioplayerDlg(CWnd* pParent = NULL);	// 标准构造函数

// 对话框数据
	enum { IDD = IDD_AUDIOPLAYER_DIALOG };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV 支持


// 实现
protected:
	HICON m_hIcon;

	// 生成的消息映射函数
	virtual BOOL OnInitDialog();
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedButton1();
	afx_msg void OnNMCustomdrawSliderScd(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnBnClickedstop();
	afx_msg void OnBnClickedButton2();
	CEdit selc;
};
