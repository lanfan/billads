// CppInterfaceDemoDlg.h : 头文件
//

#pragma once
#include "afxwin.h"
#include "afxcmn.h"


// CCppInterfaceDemoDlg 对话框
class CCppInterfaceDemoDlg : public CDialog
{
// 构造
public:
	CCppInterfaceDemoDlg(CWnd* pParent = NULL);	// 标准构造函数

// 对话框数据
	enum { IDD = IDD_CPPINTERFACEDEMO_DIALOG };

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
