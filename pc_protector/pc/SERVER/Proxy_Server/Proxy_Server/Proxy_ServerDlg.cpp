
// Proxy_ServerDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "Proxy_Server.h"
#include "Proxy_ServerDlg.h"
#include "afxdialogex.h"
#ifdef _DEBUG
#define new DEBUG_NEW
#endif

#define BufLength 1024

// 用于应用程序“关于”菜单项的 CAboutDlg 对话框

static DWORD WINAPI RecvThread(LPVOID lp);

void get_addr( sockaddr_in sockaddr_, WCHAR* ip, WCHAR* port);
LRESULT CProxy_ServerDlg::OnMyMessage(WPARAM wParam, LPARAM lParam) {
	TCHAR * str = (TCHAR *)wParam;
	GetDlgItem(IDC_STATIC)->SetWindowTextW(str);


	return 0;
}

class CAboutDlg : public CDialogEx
{
public:
	CAboutDlg();

// 对话框数据
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_ABOUTBOX };
#endif

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

// 实现
protected:
	DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDialogEx(IDD_ABOUTBOX)
{
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialogEx)
END_MESSAGE_MAP()


// CProxy_ServerDlg 对话框



CProxy_ServerDlg::CProxy_ServerDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(IDD_PROXY_SERVER_DIALOG, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CProxy_ServerDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CProxy_ServerDlg, CDialogEx)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_MESSAGE(WM_MY_MESSAGE, OnMyMessage)
	ON_BN_CLICKED(IDOK, &CProxy_ServerDlg::OnBnClickedOk)
	ON_BN_CLICKED(IDCANCEL, &CProxy_ServerDlg::OnBnClickedCancel)
	ON_BN_CLICKED(IDC_BUTTON1, &CProxy_ServerDlg::OnBnClickedButton1)
END_MESSAGE_MAP()


// CProxy_ServerDlg 消息处理程序

BOOL CProxy_ServerDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// 将“关于...”菜单项添加到系统菜单中。

	// IDM_ABOUTBOX 必须在系统命令范围内。
	ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
	ASSERT(IDM_ABOUTBOX < 0xF000);

	CMenu* pSysMenu = GetSystemMenu(FALSE);
	if (pSysMenu != NULL)
	{
		BOOL bNameValid;
		CString strAboutMenu;
		bNameValid = strAboutMenu.LoadString(IDS_ABOUTBOX);
		ASSERT(bNameValid);
		if (!strAboutMenu.IsEmpty())
		{
			pSysMenu->AppendMenu(MF_SEPARATOR);
			pSysMenu->AppendMenu(MF_STRING, IDM_ABOUTBOX, strAboutMenu);
		}
	}

	// 设置此对话框的图标。  当应用程序主窗口不是对话框时，框架将自动
	//  执行此操作
	SetIcon(m_hIcon, TRUE);			// 设置大图标
	SetIcon(m_hIcon, FALSE);		// 设置小图标

	// TODO: 在此添加额外的初始化代码
//	HWND hWnd = GetSafeHwnd();
//	PostMessage(WM_MY_MESSAGE, (WPARAM)&L"wait connect...", (LPARAM)NULL);

	return TRUE;  // 除非将焦点设置到控件，否则返回 TRUE
}

void CProxy_ServerDlg::OnSysCommand(UINT nID, LPARAM lParam)
{
	if ((nID & 0xFFF0) == IDM_ABOUTBOX)
	{
		CAboutDlg dlgAbout;
		dlgAbout.DoModal();
	}
	else
	{
		CDialogEx::OnSysCommand(nID, lParam);
	}
}

// 如果向对话框添加最小化按钮，则需要下面的代码
//  来绘制该图标。  对于使用文档/视图模型的 MFC 应用程序，
//  这将由框架自动完成。

void CProxy_ServerDlg::OnPaint()
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
HCURSOR CProxy_ServerDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}

class THREADPARAM {
public:
	SOCKET clientsocket[2] ;
	CEdit * pCedit_Port;
	CStatic * pCstatic_text;
	HWND hWnd;
	int Num_Of_Client = 0;

};


static DWORD WINAPI AcceptThread(LPVOID lp) {
	
	THREADPARAM* threadparam = (THREADPARAM*)lp;
	CEdit* Porttext = threadparam->pCedit_Port ;
	CStatic* Cs_text = threadparam->pCstatic_text;
	HWND hWnd = threadparam->hWnd;
	CString cs_port;
//	Porttext->GetWindowTextW(cs_port);
//	Cs_text->SetWindowTextW(L"create accept_thread ...");
	u_short port = _ttoi(cs_port);
	SOCKET serversocket;
	sockaddr_in addrSrv;
	sockaddr_in addrCli;
	WSADATA wsaData;
	int Num_Of_Client = 0;
	PostMessage(hWnd, WM_MY_MESSAGE, (WPARAM)&L"wait connect...", (LPARAM)NULL);
	WSAStartup(MAKEWORD(2, 2), &wsaData);
	addrSrv.sin_addr.s_addr = htonl(INADDR_ANY);
	addrSrv.sin_family = AF_INET;
	addrSrv.sin_port = htons(827);
	serversocket = socket(AF_INET, SOCK_STREAM, 0);
	bind(serversocket, (sockaddr*)&addrSrv, sizeof(sockaddr));
	listen(serversocket, 5);//
	int len = sizeof(addrCli);
	
	TCHAR TID[100] = {0};
	TCHAR TID2[200] = { 0 };
	DWORD id;
	THREADPARAM Recv_Thread_Param;
	Recv_Thread_Param.hWnd = hWnd;
		//AfxGetMainWnd()->m_hWnd;
	//sockaddr_in sin;
	WCHAR addr_ip[20] = { 0 };
	WCHAR addr_prot[6] = {0};
	//Recv_Thread_Param.pCstatic_text = threadparam->pCstatic_text;
	while (TRUE) {
		
		
		Recv_Thread_Param.clientsocket[Num_Of_Client]=accept(serversocket, (sockaddr*)&addrCli, &len);
		Recv_Thread_Param.Num_Of_Client = Num_Of_Client;
		get_addr(addrCli,addr_ip,addr_prot);
		if (Recv_Thread_Param.clientsocket[Num_Of_Client] != INVALID_SOCKET) {
	//	Cs_text->SetWindowTextW(L"accept one connect"+Num_Of_Client);
			++Num_Of_Client;
			if (Num_Of_Client == 2) {
				CreateThread(NULL, 0, RecvThread, (LPVOID)&Recv_Thread_Param, 0, &id);
				send(Recv_Thread_Param.clientsocket[0], "已成功建立连接", 17, 0);
				send(Recv_Thread_Param.clientsocket[1], "已成功建立连接", 17, 0);
				wsprintf(TID,L"Create Recv_thread %d\n ip %s:%s",id,addr_ip,addr_prot);
			//	wsprintf(TID, L"ip %s:%s", addr_ip, addr_prot);
				PostMessage(hWnd, WM_MY_MESSAGE, (WPARAM)&TID, (LPARAM)NULL);
				//Porttext->SetWindowTextW(TID);
				
				THREADPARAM Recv_Thread_Param_2;
				Recv_Thread_Param_2.hWnd = AfxGetMainWnd()->GetSafeHwnd();
				Recv_Thread_Param_2.Num_Of_Client = 0;
				Recv_Thread_Param_2.clientsocket[0] = Recv_Thread_Param.clientsocket[0];
				Recv_Thread_Param_2.clientsocket[1] = Recv_Thread_Param.clientsocket[1];
				get_addr(addrCli, addr_ip, addr_prot);
				Recv_Thread_Param_2.pCstatic_text= threadparam->pCstatic_text;
				CreateThread(NULL, 0, RecvThread, (LPVOID)&Recv_Thread_Param_2, 0, &id);
				wsprintf(TID2, L"%s\nCreate Recv_thread %d\r\n  %s:%s", TID, id, addr_ip, addr_prot);
				PostMessage(hWnd, WM_MY_MESSAGE, (WPARAM)&TID2, (LPARAM)NULL);
				//Cs_text->SetWindowTextW(TID);
				Num_Of_Client = 0;
			}
			else if (Num_Of_Client == 1) {
			
				send(Recv_Thread_Param.clientsocket[0],"等待对方上线...",17,0);
			
			}
		}

	}
}

static DWORD WINAPI RecvThread(LPVOID lp) {

	THREADPARAM * Recv_threadparam = (THREADPARAM*)lp;
	CStatic* Cs_text = Recv_threadparam->pCstatic_text;
	
	//Cs_text->SetWindowTextW(L"create recv_thread...");
	SOCKET clientsocket = Recv_threadparam->clientsocket[Recv_threadparam->Num_Of_Client];
	int Other_Client_Flag = 1;
	if (Recv_threadparam->Num_Of_Client == 1) {
		Other_Client_Flag = 0;
	}
	SOCKET othersocket=Recv_threadparam->clientsocket[Other_Client_Flag];
	
	char szRecvBuf_char[BufLength] = {0};
	TCHAR szRecvBuf[BufLength] = {0};
	while (true) {
		memset(szRecvBuf_char, 0, BufLength);
	//	memset(szRecvBuf, 0, BufLength*2);
		if (recv(clientsocket, szRecvBuf_char, BufLength, 0)>0) {
			//	MultiByteToWideChar(CP_ACP, 0, szRecvBuf_char, -1, szRecvBuf, sizeof(szRecvBuf));
			//	Cs_text->SetWindowTextW(szRecvBuf);
			send(othersocket, szRecvBuf_char, BufLength, 0);
		}
		else {
			send(othersocket, "网络断开或对方已关闭...", BufLength, 0);
			closesocket(clientsocket);
			closesocket(othersocket);
			SendMessage(Recv_threadparam->hWnd, WM_MY_MESSAGE, (WPARAM)&L"Recv_thread 已关闭..\r\n等待新的连接...", (LPARAM)NULL);
			return 0;
		}
	}
		return 0;
}



void CProxy_ServerDlg::OnBnClickedOk()
{

	// TODO: 在此添加控件通知处理程序代码
	//CDialogEx::OnOK();
	CEdit* pCedit = (CEdit*)GetDlgItem(IDC_EDIT1);
	
	THREADPARAM* p_threadParam =new THREADPARAM ();
	p_threadParam->pCedit_Port =pCedit;
	p_threadParam->pCstatic_text = (CStatic *)GetDlgItem(IDC_STATIC);
	p_threadParam->hWnd = GetSafeHwnd();
	CreateThread(NULL,0,AcceptThread,(LPVOID)p_threadParam,0,NULL );

}


void CProxy_ServerDlg::OnBnClickedCancel()
{
	
	// TODO: 在此添加控件通知处理程序代码
	
	CDialogEx::OnCancel();
}


void CProxy_ServerDlg::OnBnClickedButton1()
{
	// TODO: 在此添加控件通知处理程序代码
}


void get_addr(sockaddr_in sockaddr_, WCHAR* ip, WCHAR* port) {
	sockaddr_in sin;
	char port_char[6];
	
	//memcpy(&sin,&sockaddr_,sizeof(sin));
	_itoa_s((int)sockaddr_.sin_port,port_char,10 );
	MultiByteToWideChar(CP_ACP, 0, inet_ntoa(sockaddr_.sin_addr), -1, ip , 20);
	MultiByteToWideChar(CP_ACP, 0, port_char, -1, port, strlen(port_char)+1);
	//PostMessage(hWnd,WM_MY_MESSAGE,(WPARAM)&ip_w,NULL);
}