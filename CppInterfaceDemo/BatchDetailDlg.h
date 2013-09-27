#pragma once
#include "afxcmn.h"


// CBatchDetailDlg dialog

class CBatchDetailDlg : public CDialog
{
	DECLARE_DYNAMIC(CBatchDetailDlg)

public:
	CBatchDetailDlg(CWnd* pParent = NULL);   // standard constructor
	virtual ~CBatchDetailDlg();

	void SetTableName(CString strTableName);

// Dialog Data
	enum { IDD = IDD_DIALOG_BATCH_DETAIL };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	virtual BOOL OnInitDialog();

	DECLARE_MESSAGE_MAP()
public:
	CListCtrl m_BatchDetailList;
public:
	afx_msg void OnBnClickedOk();

private:
	static UINT HttpGetThreadProc(LPVOID lpParam);
	CString m_strTableName;
};
