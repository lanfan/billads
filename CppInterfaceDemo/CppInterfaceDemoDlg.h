// CppInterfaceDemoDlg.h : ͷ�ļ�
//

#pragma once
#include "afxwin.h"
#include "afxcmn.h"


// CCppInterfaceDemoDlg �Ի���
class CCppInterfaceDemoDlg : public CDialog
{
// ����
public:
	CCppInterfaceDemoDlg(CWnd* pParent = NULL);	// ��׼���캯��

// �Ի�������
	enum { IDD = IDD_CPPINTERFACEDEMO_DIALOG };

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
public:
	afx_msg void OnBnClickedButtonLogin();
public:
	CString m_strServerUrl;
public:
	CString m_strUsername;
public:
	CString m_strPwd;
public:
	CString m_strContent;
public:
	afx_msg void OnBnClickedButtonSendSms();
public:
	afx_msg void OnBnClickedButtonLogout();
public:
	CString m_strMobileList;

public:
	void Split(CString source, CStringArray& dest, CString division);
public:
	afx_msg void OnBnClickedButtonViewBalance();
public:
	CListCtrl m_SendResultList;
public:
	afx_msg void OnBnClickedButtonRefreshList();
public:
	afx_msg void OnNMDblclkListSendResult(NMHDR *pNMHDR, LRESULT *pResult);
public:
	afx_msg void OnBnClickedButtonPostSend();
};
