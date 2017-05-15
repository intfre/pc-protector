
// Proxy_Server.h : PROJECT_NAME 应用程序的主头文件
//

#pragma once

#ifndef __AFXWIN_H__
	#error "在包含此文件之前包含“stdafx.h”以生成 PCH 文件"
#endif

#include "resource.h"		// 主符号


// CProxy_ServerApp: 
// 有关此类的实现，请参阅 Proxy_Server.cpp
//

class CProxy_ServerApp : public CWinApp
{
public:
	CProxy_ServerApp();

// 重写
public:
	virtual BOOL InitInstance();

// 实现

	DECLARE_MESSAGE_MAP()
};

extern CProxy_ServerApp theApp;