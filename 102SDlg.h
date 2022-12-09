
// 102SDlg.h: 头文件
//

#pragma once

// add by itas109
#include "CSerialPort/SerialPort.h"
#include "CSerialPort/SerialPortInfo.h"

using namespace itas109;




// CMy102SDlg 对话框
class CMy102SDlg : public CDialogEx, public CSerialPortListener
{
// 构造
public:
	CMy102SDlg(CWnd* pParent = nullptr);	// 标准构造函数

// 对话框数据
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_MY102S_DIALOG };
#endif

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV 支持

	void onReadEvent(const char* portName, unsigned int readBufferLen); // About CSerialPort

	int onRadarInfo(const char* buffer, int len);


// 实现
protected:
	HICON m_hIcon;

	// 生成的消息映射函数
	virtual BOOL OnInitDialog();
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()

public:
	afx_msg void OnBnClickedButtonOpenClose();
	afx_msg void OnBnClickedButtonRefresh();
	CComboBox m_PortNr;
	

	CButton m_OpenCloseCtrl;

	CSerialPort m_SerialPort;
	CStatic m_RadarInfo;
};
