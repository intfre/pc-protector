
// Proxy_ServerDlg.h : ͷ�ļ�
//

#pragma once

#define WM_MY_MESSAGE (WM_USER+100)
// CProxy_ServerDlg �Ի���
class CProxy_ServerDlg : public CDialogEx
{
// ����
public:
	CProxy_ServerDlg(CWnd* pParent = NULL);	// ��׼���캯��

// �Ի�������
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_PROXY_SERVER_DIALOG };
#endif

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV ֧��


// ʵ��
protected:
	HICON m_hIcon;

	// ���ɵ���Ϣӳ�亯��
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
