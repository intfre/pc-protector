
// Proxy_ServerDlg.h : 头文件
//

#pragma once

#define WM_MY_MESSAGE (WM_USER+100)
// CProxy_ServerDlg 对话框
class CProxy_ServerDlg : public CDialogEx
{
// 构造
public:
	CProxy_ServerDlg(CWnd* pParent = NULL);	// 标准构造函数

// 对话框数据
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_PROXY_SERVER_DIALOG };
#endif

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV 支持


// 实现
protected:
	HICON m_hIcon;

	// 生成的消息映射函数
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()
	afx_msg LRESULT OnMyMessage(WPARAM wParam,LPARAM lParam);
public:
	afx_msg void OnBnClickedOk();

	afx_msg void OnBnClickedCancel();
	afx_msg void OnBnClickedButton1();
};
