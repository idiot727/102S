
// 102SDlg.cpp: 实现文件
//

#include "pch.h"
#include "framework.h"
#include "102S.h"
#include "102SDlg.h"
#include "afxdialogex.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CMy102SDlg 对话框



CMy102SDlg::CMy102SDlg(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_MY102S_DIALOG, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDI_ICON2);
}

void CMy102SDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_COMBO1, m_PortNr);
	DDX_Control(pDX, IDC_BUTTON2, m_OpenCloseCtrl);
	DDX_Control(pDX, IDC_STATIC8, m_RadarInfo);
	DDX_Control(pDX, IDC_BUTTON5, m_StartPause);
}

BEGIN_MESSAGE_MAP(CMy102SDlg, CDialogEx)
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()

	ON_BN_CLICKED(IDC_BUTTON2, &CMy102SDlg::OnBnClickedButtonOpenClose)
	ON_BN_CLICKED(IDC_BUTTON1, &CMy102SDlg::OnBnClickedButtonRefresh)

	ON_WM_TIMER()
	ON_WM_CTLCOLOR()
	ON_BN_CLICKED(IDC_BUTTON5, &CMy102SDlg::OnBnClickedButton5)
	ON_WM_SIZE()
END_MESSAGE_MAP()


// CMy102SDlg 消息处理程序

BOOL CMy102SDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// 设置此对话框的图标。  当应用程序主窗口不是对话框时，框架将自动
	//  执行此操作
	SetIcon(m_hIcon, TRUE);			// 设置大图标
	SetIcon(m_hIcon, FALSE);		// 设置小图标

	// TODO: 在此添加额外的初始化代码

		//获取串口号
	std::vector<SerialPortInfo> m_portsList = CSerialPortInfo::availablePortInfos();
	TCHAR m_regKeyValue[256];
	for (size_t i = 0; i < m_portsList.size(); i++)
	{
#ifdef UNICODE
		int iLength;
		const char* _char = m_portsList[i].portName;
		iLength = MultiByteToWideChar(CP_ACP, 0, _char, strlen(_char) + 1, NULL, 0);
		MultiByteToWideChar(CP_ACP, 0, _char, strlen(_char) + 1, m_regKeyValue, iLength);
#else
		strcpy_s(m_regKeyValue, 256, m_portsList[i].portName);
#endif
		m_PortNr.AddString(m_regKeyValue);
	}
	m_PortNr.SetCurSel(0);

	m_SerialPort.connectReadEvent(this);


	//m_Font.CreatePointFont(250, _T("Times New Roman"));
	//m_FontMax.CreatePointFont(500, _T("Times New Roman"));

	CFont* cf = GetFont();

	LOGFONT lf;
	cf->GetLogFont(&lf);
	m_FontDialog.CreateFontIndirectW(&lf);
	lf.lfHeight *= 2.5;
	m_Font.CreateFontIndirectW(&lf);


	m_Brush.CreateSolidBrush(GetSysColor(COLOR_3DFACE));
	m_Brush_white.CreateSolidBrush(RGB(255, 255, 255));

	m_RadarInfo.ModifyStyle(0, SS_CENTER);
	m_RadarInfo.SetWindowTextW(_T(" 0 cm "));

	bConn = FALSE;
	bShow = FALSE;

	CRect rect;

	GetClientRect(&rect);     //取客户区大小 

	Old.x = rect.right - rect.left;

	Old.y = rect.bottom - rect.top;

	m_bStatus = FALSE;


	return TRUE;  // 除非将焦点设置到控件，否则返回 TRUE
}

// 如果向对话框添加最小化按钮，则需要下面的代码
//  来绘制该图标。  对于使用文档/视图模型的 MFC 应用程序，
//  这将由框架自动完成。

void CMy102SDlg::OnPaint()
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
HCURSOR CMy102SDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}

void CMy102SDlg::OnBnClickedButtonOpenClose()
{
	// TODO: 在此添加控件通知处理程序代码
	if (m_SerialPort.isOpen())
	{
		m_SerialPort.close();
		m_OpenCloseCtrl.SetWindowText(_T("连接"));///设置按钮文字为"打开串口"
		bConn = FALSE;
	}
	///打开串口操作
	else if (m_PortNr.GetCount() > 0)///当前列表的内容个数
	{
		char portName[256] = { 0 };
		/*int SelBaudRate;
		int SelParity;
		int SelDataBits;
		int SelStop;*/

		UpdateData(true);
		CString temp;
		m_PortNr.GetWindowText(temp);
#ifdef UNICODE
		strcpy_s(portName, 256, CW2A(temp.GetString()));
#else
		strcpy_s(portName, 256, temp.GetBuffer());
#endif	

		m_SerialPort.init(portName, 256000);//itas109::BaudRate::BaudRate9600);
		m_SerialPort.open();

		if (m_SerialPort.isOpen())
		{
			m_OpenCloseCtrl.SetWindowText(_T("关闭"));
			bConn = TRUE;
		}
		else
		{
			m_OpenCloseCtrl.SetWindowText(_T("连接"));
			AfxMessageBox(_T("串口已被占用！"));
			bConn = FALSE;
		}
	}
	else
	{
		AfxMessageBox(_T("没有发现串口！"));
	}
}


void CMy102SDlg::OnBnClickedButtonRefresh()
{
	// TODO: 在此添加控件通知处理程序代码
	m_PortNr.ResetContent();

	std::vector<SerialPortInfo> m_portsList = CSerialPortInfo::availablePortInfos();
	TCHAR m_regKeyValue[256];
	for (size_t i = 0; i < m_portsList.size(); i++)
	{
#ifdef UNICODE
		int iLength;
		const char* _char = m_portsList[i].portName;
		iLength = MultiByteToWideChar(CP_ACP, 0, _char, strlen(_char) + 1, NULL, 0);
		MultiByteToWideChar(CP_ACP, 0, _char, strlen(_char) + 1, m_regKeyValue, iLength);
#else
		strcpy_s(m_regKeyValue, 256, m_portsList[i].portName);
#endif
		m_PortNr.AddString(m_regKeyValue);
	}

	m_PortNr.SetCurSel(0);
}

int CMy102SDlg::onRadarInfo(const char* buffer, int len)
{
	char head = 0xaa;
	if (buffer[0] == head)
	{

		unsigned short b;
		size_t size = sizeof(b);
		memcpy(&b, &buffer[1], size);
		return b;
	}
	return 0;
}


void CMy102SDlg::onReadEvent(const char* portName, unsigned int readBufferLen)
{
	if (!bShow) {
		return;
	}

	if (readBufferLen > 0)
	{
		char* data = new char[readBufferLen + 1]; // '\0'

		if (data)
		{
			int recLen = m_SerialPort.readData(data, readBufferLen);

			if (recLen > 0)
			{
				CString str2;
				str2.Format(_T(" %d CM "), onRadarInfo(data, recLen));
				this->m_RadarInfo.SetWindowTextW(str2);
				CRect rc;
				m_RadarInfo.GetWindowRect(&rc);
				ScreenToClient(&rc);
				InvalidateRect(&rc);

				/*	data[recLen] = '\0';

					CString str1(data);

					rx += str1.GetLength();

					m_ReceiveCtrl.SetSel(-1, -1);
					m_ReceiveCtrl.ReplaceSel(str1);

					CString str2;
					str2.Format(_T("%d"), rx);
					m_recvCountCtrl.SetWindowText(str2);*/
			}

			delete[] data;

			data = NULL;
		}
	}
}


//void CCommMFCDlg::OnBnClickedButtonSend()
//{
//	GetDlgItem(IDC_SendEdit)->SetFocus();
//	if (!m_SerialPort.isOpen()) ///没有打开串口
//	{
//		AfxMessageBox(_T("请首先打开串口"));
//		return;
//	}
//
//	CString temp;
//	m_Send.GetWindowText(temp);
//	int len = 0;
//	char* m_str = NULL;
//#ifdef UNICODE
//	// 兼容中文
//	CStringA strA(temp);
//	len = strA.GetLength();
//	m_str = strA.GetBuffer();
//#else
//	len = temp.GetLength();
//	m_str = temp.GetBuffer(0);
//#endif
//
//	m_SerialPort.writeData(m_str, len);
//
//	tx += len;
//
//	CString str2;
//	str2.Format(_T("%d"), tx);
//	m_sendCountCtrl.SetWindowText(str2);
//}




HBRUSH CMy102SDlg::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor)
{
	HBRUSH hbr = __super::OnCtlColor(pDC, pWnd, nCtlColor);

	//return hbr;

	CWnd* p = NULL;


	// TODO:  在此更改 DC 的任何特性

	// TODO:  如果默认的不是所需画笔，则返回另一个画笔

	CRect rc;
	if (pWnd->GetDlgCtrlID() == IDC_STATIC8)
	{


		pDC->SetTextColor(RGB(0, 0, 0));//
		pDC->SetBkColor(RGB(255, 255, 255));//
		if (m_bStatus)
			pDC->SelectObject(&m_FontMax);
		else
			pDC->SelectObject(&m_Font);//文字为初始化文字

		//m_Brush_white.DeleteObject();
		//m_Brush_white.CreateSolidBrush(RGB(0,0,0));


		return m_Brush;

	}

	if (pWnd->GetDlgCtrlID() == IDC_STATIC6)
	{

		pDC->SetTextColor(RGB(0, 0, 0));//
		pDC->SetBkColor(RGB(255, 255, 255));//
		//p = pWnd->GetDlgItem(IDC_STATIC6);

		pWnd->GetClientRect(&rc);
		pDC->FillSolidRect(rc, RGB(255, 255, 255));

		return m_Brush_white;

	}
	return hbr;
}


void CMy102SDlg::OnBnClickedButton5()
{
	// TODO: 在此添加控件通知处理程序代码
	if (!bShow && bConn) {
		this->m_StartPause.SetWindowTextW(_T("暂停"));
		bShow = TRUE;
	}
	else if (bShow) {
		bShow = FALSE;
		this->m_StartPause.SetWindowTextW(_T("开始"));
	}
}


void CMy102SDlg::OnSize(UINT nType, int cx, int cy)
{
	__super::OnSize(nType, cx, cy);
	//return;

	if (nType == SIZE_RESTORED) {
		ChangeCtrlSize(0);
		m_bStatus = FALSE;
		//this->ChangeSize(cx, cy);
	}
	if (nType == SIZE_MAXIMIZED) {
		ChangeCtrlSize(1);
		m_bStatus = TRUE;
	}



	// TODO: 在此处添加消息处理程序代码
}


void CMy102SDlg::ChangeCtrlSize(BOOL bMax)
{
	// TODO: 在此处添加实现代码.

	float fsp[2];

	POINT Newp; //获取现在对话框的大小

	CRect recta;

	GetClientRect(&recta);     //取客户区大小 

	Newp.x = recta.right - recta.left;

	Newp.y = recta.bottom - recta.top;

	fsp[0] = (float)Newp.x / Old.x;

	fsp[1] = (float)Newp.y / Old.y;

	if (bMax) {
		LOGFONT lf;
		m_FontDialog.GetLogFont(&lf);
		lf.lfWeight *= fsp[0];
		lf.lfHeight *= fsp[1];
		if(m_FontDialogMax.GetSafeHandle() == NULL)
			m_FontDialogMax.CreateFontIndirectW(&lf);
		m_Font.GetLogFont(&lf);
		lf.lfWeight *= fsp[0];
		lf.lfHeight *= fsp[1];
		if(m_FontMax.GetSafeHandle() == NULL)
			m_FontMax.CreateFontIndirectW(&lf);
			
	}

	CRect Rect;

	int woc;

	CPoint OldTLPoint, TLPoint; //左上角

	CPoint OldBRPoint, BRPoint; //右下角

	HWND  hwndChild = ::GetWindow(m_hWnd, GW_CHILD);  //列出所有控件 

	if (hwndChild)
	{
		//m_FontMax.CreatePointFont(510, _T("Times New Roman"));
	}

	while (hwndChild)

	{
		woc = ::GetDlgCtrlID(hwndChild);//取得ID

		GetDlgItem(woc)->GetWindowRect(Rect);

		ScreenToClient(Rect);

		OldTLPoint = Rect.TopLeft();

		TLPoint.x = long(OldTLPoint.x * fsp[0]);

		TLPoint.y = long(OldTLPoint.y * fsp[1]);

		OldBRPoint = Rect.BottomRight();

		BRPoint.x = long(OldBRPoint.x * fsp[0]);

		BRPoint.y = long(OldBRPoint.y * fsp[1]);

		Rect.SetRect(TLPoint, BRPoint);

		GetDlgItem(woc)->MoveWindow(Rect, TRUE);

		if (bMax) {

			GetDlgItem(woc)->SetFont(&m_FontDialogMax);
		}
		else
			GetDlgItem(woc)->SetFont(&m_FontDialog);

		hwndChild = ::GetWindow(hwndChild, GW_HWNDNEXT);
	}
	Old = Newp;


}
