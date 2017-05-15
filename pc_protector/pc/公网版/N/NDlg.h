
// NDlg.h : ͷ�ļ�
//

#pragma once
#define  WM_Hook_Message (WM_USER+100)
#define  WM_UnHook_Message (WM_USER+101)
#define  WM_Show_Message (WM_USER+102)
#define  WM_Start_Message (WM_USER+103)
// CNDlg �Ի���
class CNDlg : public CDialogEx
{
// ����
public:
	CNDlg(CWnd* pParent = NULL);	// ��׼���캯��

// �Ի�������
	enum { IDD = IDD_N_DIALOG };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV ֧��


// ʵ��
protected:
	HICON m_hIcon;

	// ���ɵ���Ϣӳ�亯��
	virtual BOOL OnInitDialog();
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()
public:
	virtual LRESULT WindowProc(UINT message, WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT OnNotifyMsg(WPARAM wparam, LPARAM lparam); 
	afx_msg LRESULT OnHookMsg(WPARAM wparam, LPARAM lparam);
	afx_msg LRESULT OnUnHookMsg(WPARAM wparam, LPARAM lparam);
	afx_msg LRESULT OnShowMsg(WPARAM wparam, LPARAM lparam);
	afx_msg LRESULT OnStartMsg(WPARAM wparam, LPARAM lparam);
private:
	NOTIFYICONDATA m_tnid;
public:
	afx_msg void OnBnClickedOk();
	afx_msg void OnBnClickedCancel();
	afx_msg void OnBnClickedButton2();
	afx_msg void OnBnClickedButton3();
	void Start_Service();
	//int server();
	afx_msg void OnBnClickedButton1();
	afx_msg void OnBnClickedButton5();
};
