
// Proxy_Server.h : PROJECT_NAME Ӧ�ó������ͷ�ļ�
//

#pragma once

#ifndef __AFXWIN_H__
	#error "�ڰ������ļ�֮ǰ������stdafx.h�������� PCH �ļ�"
#endif

#include "resource.h"		// ������


// CProxy_ServerApp: 
// �йش����ʵ�֣������ Proxy_Server.cpp
//

class CProxy_ServerApp : public CWinApp
{
public:
	CProxy_ServerApp();

// ��д
public:
	virtual BOOL InitInstance();

// ʵ��

	DECLARE_MESSAGE_MAP()
};

extern CProxy_ServerApp theApp;