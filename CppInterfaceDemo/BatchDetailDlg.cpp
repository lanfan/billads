// BatchDetailDlg.cpp : implementation file
//

#include "stdafx.h"
#include "CppInterfaceDemo.h"
#include "BatchDetailDlg.h"
#include "CppInterfaceDemoDlg.h"
#include "HttpRequestClient.h"
#include "json/json.h"


// CBatchDetailDlg dialog

IMPLEMENT_DYNAMIC(CBatchDetailDlg, CDialog)

CBatchDetailDlg::CBatchDetailDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CBatchDetailDlg::IDD, pParent)
{
	m_strTableName=_T("");
}

CBatchDetailDlg::~CBatchDetailDlg()
{
}

void CBatchDetailDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_LIST_BATCH_LIST, m_BatchDetailList);
}


BEGIN_MESSAGE_MAP(CBatchDetailDlg, CDialog)
	ON_BN_CLICKED(IDOK, &CBatchDetailDlg::OnBnClickedOk)
END_MESSAGE_MAP()


// CBatchDetailDlg message handlers

BOOL CBatchDetailDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	m_BatchDetailList.InsertColumn( 0, _T("操作时间"));
	m_BatchDetailList.InsertColumn( 1, _T("手机"));
	m_BatchDetailList.InsertColumn( 2, _T("结果"));
	m_BatchDetailList.InsertColumn( 3, _T("内容"));

	m_BatchDetailList.SetColumnWidth( 0, 120);
	m_BatchDetailList.SetColumnWidth( 1, 120);
	m_BatchDetailList.SetColumnWidth( 2, 60);
	m_BatchDetailList.SetColumnWidth( 3, 180);

	m_BatchDetailList.SetExtendedStyle(m_BatchDetailList.GetExtendedStyle() | 
		LVS_EX_GRIDLINES | LVS_EX_FULLROWSELECT );

	AfxBeginThread(HttpGetThreadProc, this, THREAD_PRIORITY_NORMAL);

	return TRUE;
}

void CBatchDetailDlg::OnBnClickedOk()
{
	// TODO: Add your control notification handler code here
	OnOK();
}

UINT CBatchDetailDlg::HttpGetThreadProc(LPVOID lpParam)
{
	CBatchDetailDlg* pThis=(CBatchDetailDlg*)lpParam;

	pThis->m_BatchDetailList.InsertItem(0, _T("正在读取服务器。。。"));

	CCppInterfaceDemoDlg* pMainwnd=(CCppInterfaceDemoDlg*)AfxGetApp()->GetMainWnd();

	CString strUrl=pMainwnd->m_strServerUrl+_T("r=site/ViewBatchStatJSON&tableName=")+pThis->m_strTableName;
	CHttpRequestClient HttpClient;
	CString strResponse=HttpClient.DoGet(strUrl);

	Json::Reader reader;
	Json::Value value;

	if (reader.parse(strResponse.GetBuffer(0), value))
	{
		if (value["success"].asBool() == TRUE)
		{
			pThis->m_BatchDetailList.DeleteAllItems();

			Json::Value SmsList(Json::arrayValue); // []
			SmsList=value["dataList"];
			for (UINT i=0; i<SmsList.size(); i++)
			{
				CString strTemp;

				strTemp=SmsList[i]["number"].asCString();
				CString strMobile=HttpClient.Convert(strTemp, CP_UTF8, 936);

				strTemp=SmsList[i]["content"].asCString();
				CString strContent=HttpClient.Convert(strTemp, CP_UTF8, 936);

				CString strStatus=SmsList[i]["status"].asCString();

				CString strOpTime=SmsList[i]["optime"].asCString();

				pThis->m_BatchDetailList.InsertItem(i, strOpTime);
				pThis->m_BatchDetailList.SetItemText(i, 1, strMobile);
				pThis->m_BatchDetailList.SetItemText(i, 2, strStatus);
				pThis->m_BatchDetailList.SetItemText(i, 3, strContent);	
			}
		}
		else
			AfxMessageBox(CString(_T("网络操作失败："))+value["msg"].asCString());
	}

	pThis->GetDlgItem(IDOK)->EnableWindow(TRUE);

	return 0;
}

void CBatchDetailDlg::SetTableName(CString strTableName)
{
	m_strTableName=strTableName;
}