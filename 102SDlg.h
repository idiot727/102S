
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
	BOOL bConn;
	BOOL bShow;

	CSerialPort m_SerialPort;
	CStatic m_RadarInfo;
	afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);

	CBrush m_Brush;
	CBrush m_Brush_white;
	CFont m_FontDialog;
	CFont m_FontDialogMax;
	CFont m_Font;
	CFont m_FontMax;
	afx_msg void OnBnClickedButton5();
	CButton m_StartPause;
	afx_msg void OnSize(UINT nType, int cx, int cy);

	void ChangeCtrlSize(BOOL bMax);

	CPoint Old;

	BOOL m_bStatus;

};
