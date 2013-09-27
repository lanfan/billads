// CppInterfaceDemoDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "CppInterfaceDemo.h"
#include "CppInterfaceDemoDlg.h"
#include "HttpRequestClient.h"
#include "json/json.h"
#include "BatchDetailDlg.h"

#pragma comment(lib, "json_vc71_libmtd.lib") 

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// 用于应用程序“关于”菜单项的 CAboutDlg 对话框

class CAboutDlg : public CDialog
{
public:
	CAboutDlg();

// 对话框数据
	enum { IDD = IDD_ABOUTBOX };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

// 实现
protected:
	DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDialog(CAboutDlg::IDD)
{
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialog)
END_MESSAGE_MAP()


// CCppInterfaceDemoDlg 对话框




CCppInterfaceDemoDlg::CCppInterfaceDemoDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CCppInterfaceDemoDlg::IDD, pParent)
	, m_strServerUrl(_T(""))
	, m_strUsername(_T(""))
	, m_strPwd(_T(""))
	, m_strMobileList(_T(""))
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CCppInterfaceDemoDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_EDIT_SERVER_URL, m_strServerUrl);
	DDX_Text(pDX, IDC_EDIT_USERNAME, m_strUsername);
	DDX_Text(pDX, IDC_EDIT_PWD, m_strPwd);
	DDX_Text(pDX, IDC_EDIT_SMS_CONTENT, m_strContent);
	DDX_Text(pDX, IDC_EDIT_MOBILE_LIST, m_strMobileList);
	DDX_Control(pDX, IDC_LIST_SEND_RESULT, m_SendResultList);
}

BEGIN_MESSAGE_MAP(CCppInterfaceDemoDlg, CDialog)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	//}}AFX_MSG_MAP
	ON_BN_CLICKED(IDC_BUTTON_LOGIN, &CCppInterfaceDemoDlg::OnBnClickedButtonLogin)
	ON_BN_CLICKED(IDC_BUTTON_SEND_SMS, &CCppInterfaceDemoDlg::OnBnClickedButtonSendSms)
	ON_BN_CLICKED(IDC_BUTTON_LOGOUT, &CCppInterfaceDemoDlg::OnBnClickedButtonLogout)
	ON_BN_CLICKED(IDC_BUTTON_VIEW_BALANCE, &CCppInterfaceDemoDlg::OnBnClickedButtonViewBalance)
	ON_BN_CLICKED(IDC_BUTTON_REFRESH_LIST, &CCppInterfaceDemoDlg::OnBnClickedButtonRefreshList)
	ON_NOTIFY(NM_DBLCLK, IDC_LIST_SEND_RESULT, &CCppInterfaceDemoDlg::OnNMDblclkListSendResult)
	ON_BN_CLICKED(IDC_BUTTON_POST_SEND, &CCppInterfaceDemoDlg::OnBnClickedButtonPostSend)
END_MESSAGE_MAP()


// CCppInterfaceDemoDlg 消息处理程序

BOOL CCppInterfaceDemoDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	// 将“关于...”菜单项添加到系统菜单中。

	// IDM_ABOUTBOX 必须在系统命令范围内。
	ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
	ASSERT(IDM_ABOUTBOX < 0xF000);

	CMenu* pSysMenu = GetSystemMenu(FALSE);
	if (pSysMenu != NULL)
	{
		CString strAboutMenu;
		strAboutMenu.LoadString(IDS_ABOUTBOX);
		if (!strAboutMenu.IsEmpty())
		{
			pSysMenu->AppendMenu(MF_SEPARATOR);
			pSysMenu->AppendMenu(MF_STRING, IDM_ABOUTBOX, strAboutMenu);
		}
	}

	// 设置此对话框的图标。当应用程序主窗口不是对话框时，框架将自动
	//  执行此操作
	SetIcon(m_hIcon, TRUE);			// 设置大图标
	SetIcon(m_hIcon, FALSE);		// 设置小图标

	// TODO: 在此添加额外的初始化代码
	m_strServerUrl=_T("http://www.fingersms.com/index.php?");
	m_strUsername=_T("demo");
	m_strPwd=_T("demo");

	m_strMobileList=_T("+8613912345678,+8613912345678");

	m_strContent=_T("接口测试短信");

	m_SendResultList.SetExtendedStyle(m_SendResultList.GetExtendedStyle() | 
		LVS_EX_GRIDLINES | LVS_EX_FULLROWSELECT );

	m_SendResultList.InsertColumn( 0, _T("批次ID"));
	m_SendResultList.InsertColumn( 1, _T("创建时间"));
	m_SendResultList.InsertColumn( 2, _T("总数"));
	m_SendResultList.InsertColumn( 3, _T("未发"));
	m_SendResultList.InsertColumn( 4, _T("在发"));
	m_SendResultList.InsertColumn( 5, _T("成功"));
	m_SendResultList.InsertColumn( 6, _T("失败"));

	m_SendResultList.SetColumnWidth( 0, 150);
	m_SendResultList.SetColumnWidth( 1, 130);
	m_SendResultList.SetColumnWidth( 2, 40);
	m_SendResultList.SetColumnWidth( 3, 40);
	m_SendResultList.SetColumnWidth( 4, 40);
	m_SendResultList.SetColumnWidth( 5, 40);
	m_SendResultList.SetColumnWidth( 6, 40);

	UpdateData(FALSE);

	return TRUE;  // 除非将焦点设置到控件，否则返回 TRUE
}

void CCppInterfaceDemoDlg::OnSysCommand(UINT nID, LPARAM lParam)
{
	if ((nID & 0xFFF0) == IDM_ABOUTBOX)
	{
		CAboutDlg dlgAbout;
		dlgAbout.DoModal();
	}
	else
	{
		CDialog::OnSysCommand(nID, lParam);
	}
}

// 如果向对话框添加最小化按钮，则需要下面的代码
//  来绘制该图标。对于使用文档/视图模型的 MFC 应用程序，
//  这将由框架自动完成。

void CCppInterfaceDemoDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // 用于绘制的设备上下文

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// 使图标在工作矩形中居中
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// 绘制图标
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialog::OnPaint();
	}
}

//当用户拖动最小化窗口时系统调用此函数取得光标显示。
//
HCURSOR CCppInterfaceDemoDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}


void CCppInterfaceDemoDlg::OnBnClickedButtonLogin()
{
	UpdateData(TRUE);

	if (m_strServerUrl.IsEmpty() || m_strUsername.IsEmpty() || m_strPwd.IsEmpty())
	{
		AfxMessageBox(_T("输入框不能为空"));
		return;
	}

	CString strUrl=
		m_strServerUrl+
		_T("r=site/loginJSON&LoginForm[username]=")+m_strUsername+
		_T("&LoginForm[password]=")+m_strPwd;

	CHttpRequestClient HttpClient;
	CString strResponse=HttpClient.DoGet(strUrl);

	//AfxMessageBox(strResponse);
	Json::Reader reader;
	Json::Value value;

	if (reader.parse(strResponse.GetBuffer(0), value))
	{
		if (value["success"].asBool() == TRUE)
		{
			GetDlgItem(IDC_BUTTON_LOGOUT)->EnableWindow(TRUE);
			GetDlgItem(IDC_BUTTON_SEND_SMS)->EnableWindow(TRUE);
			GetDlgItem(IDC_BUTTON_POST_SEND)->EnableWindow(TRUE);
			GetDlgItem(IDC_BUTTON_VIEW_BALANCE)->EnableWindow(TRUE);
			GetDlgItem(IDC_BUTTON_REFRESH_LIST)->EnableWindow(TRUE);
			AfxMessageBox(_T("成功登录"));
		}
	}
}

void CCppInterfaceDemoDlg::OnBnClickedButtonLogout()
{
	CString strUrl=m_strServerUrl+_T("r=site/logoutJSON");

	CHttpRequestClient HttpClient;
	CString strResponse=HttpClient.DoGet(strUrl);

	//AfxMessageBox(strResponse);
	Json::Reader reader;
	Json::Value value;

	if (reader.parse(strResponse.GetBuffer(0), value))
	{
		if (value["success"].asBool() == TRUE)
		{
			GetDlgItem(IDC_BUTTON_LOGOUT)->EnableWindow(FALSE);
			GetDlgItem(IDC_BUTTON_SEND_SMS)->EnableWindow(FALSE);
			GetDlgItem(IDC_BUTTON_POST_SEND)->EnableWindow(FALSE);
			GetDlgItem(IDC_BUTTON_VIEW_BALANCE)->EnableWindow(FALSE);
			GetDlgItem(IDC_BUTTON_REFRESH_LIST)->EnableWindow(FALSE);
			AfxMessageBox(_T("已注销"));
		}
	}
}

void CCppInterfaceDemoDlg::OnBnClickedButtonSendSms()
{
	UpdateData(TRUE);

	if (m_strMobileList.IsEmpty() || m_strContent.IsEmpty())
	{
		AfxMessageBox(_T("输入不能为空"));
		return;
	}

	CHttpRequestClient HttpClient;

	// 注意：提交号码需要把加号替换为%2B
	m_strMobileList.Replace("+", "%2B");
	m_strMobileList=HttpClient.Convert(m_strMobileList, 936, CP_UTF8);
	m_strContent=HttpClient.Convert(m_strContent, 936, CP_UTF8);

	CString strUrl=
		m_strServerUrl+
		_T("r=site/SendSMSJSON&MobileList=")+
		m_strMobileList+
		_T("&SMSContent=")+
		m_strContent+
		_T("&MobileListDone=true");

	
	CString strUTF8=HttpClient.DoGet(strUrl);
	CString strResponse = HttpClient.Convert(strUTF8, CP_UTF8, 936);

	Json::Reader reader;
	Json::Value value;

	if (reader.parse(strResponse.GetBuffer(0), value))
	{
		if (value["success"].asBool() == TRUE)
		{
			CString strMsg=value["msg"].asCString();
			if (AfxMessageBox(strMsg, MB_YESNO) == IDNO)
				return;

			CString strConfirmUrl=m_strServerUrl+_T("r=site/ConfirmToSendJSON");
			CString strSendResult=HttpClient.DoGet(strConfirmUrl);
			if (reader.parse(strSendResult.GetBuffer(0), value))
			{
				if (value["success"].asBool() == TRUE)
					AfxMessageBox(_T("短信提交成功"));
				else
					AfxMessageBox(_T("短信提交失败"));
			}
		}
		else
			AfxMessageBox(CString(_T("网络操作失败："))+value["msg"].asCString());
	}	
}

void CCppInterfaceDemoDlg::OnBnClickedButtonPostSend()
{
	UpdateData(TRUE);

	if (m_strMobileList.IsEmpty() || m_strContent.IsEmpty())
	{
		AfxMessageBox(_T("输入不能为空"));
		return;
	}

	CHttpRequestClient HttpClient;

	// 注意：提交号码需要把加号替换为%2B
	m_strMobileList.Replace("+", "%2B");

	// 注：在DoPost里面会做转换，这里不需要做
	//m_strMobileList=HttpClient.Convert(m_strMobileList, 936, CP_UTF8);
	//m_strContent=HttpClient.Convert(m_strContent, 936, CP_UTF8);
	CString strUrl=m_strServerUrl+_T("r=site/SendSMSJSON");


	// 可以把账号一起提交，这样可不用先登录
	//HttpClient.AddPostParam("LoginForm[username]","demo");
	//HttpClient.AddPostParam("LoginForm[password]","demo");
	HttpClient.AddPostParam("MobileList",m_strMobileList);
	HttpClient.AddPostParam("SMSContent",m_strContent);
	HttpClient.AddPostParam("MobileListDone","true");
	// 选择不用二次确认
	HttpClient.AddPostParam("ConfirmMe","false");
	CString strUTF8=HttpClient.DoPost(strUrl);

	CString strResponse = HttpClient.Convert(strUTF8, CP_UTF8, 936);

	Json::Reader reader;
	Json::Value value;

	if (reader.parse(strResponse.GetBuffer(0), value))
	{
		if (value["success"].asBool() == TRUE)
			AfxMessageBox(value["msg"].asCString());
		else
			AfxMessageBox(CString(_T("网络操作失败："))+value["msg"].asCString());
	}	
}

void CCppInterfaceDemoDlg::OnBnClickedButtonViewBalance()
{
	CString strUrl=m_strServerUrl+_T("r=site/ViewBalanceJSON");

	CHttpRequestClient HttpClient;
	CString strResponse=HttpClient.DoGet(strUrl);

	Json::Reader reader;
	Json::Value value;

	if (reader.parse(strResponse.GetBuffer(0), value))
	{
		if (value["success"].asBool() == TRUE)
		{
			CString strChargedCnt=value["chargedCount"].asCString();
			CString strUsedCnt=value["usedCount"].asCString();

			AfxMessageBox(_T("充值点数：")+strChargedCnt+_T("， 使用点数：")+strUsedCnt);
		}
	}
}

void CCppInterfaceDemoDlg::OnBnClickedButtonRefreshList()
{
	CString strUrl=m_strServerUrl+_T("r=site/ViewSendingStatusJSON");

	CHttpRequestClient HttpClient;
	CString strResponse=HttpClient.DoGet(strUrl);

	Json::Reader reader;
	Json::Value value;

	if (reader.parse(strResponse.GetBuffer(0), value))
	{
		if (value["success"].asBool() == TRUE)
		{
			m_SendResultList.DeleteAllItems();

			Json::Value BatchArray(Json::arrayValue); // []
			BatchArray=value["dataList"];
			for (UINT i=0; i<BatchArray.size(); i++)
			{
				CString strBatchName=BatchArray[i]["tableName"].asCString();
				CString strImportTime=BatchArray[i]["createTime"].asCString();
				CString strTotalCount=BatchArray[i]["totalCount"].asCString();
				CString strUnsendCount=BatchArray[i]["unsendCount"].asCString();
				CString strSendingCount=BatchArray[i]["sendingCount"].asCString();
				CString strSuccessCount=BatchArray[i]["successCount"].asCString();
				CString strFailedCount=BatchArray[i]["failureCount"].asCString();

				m_SendResultList.InsertItem(i, strBatchName);
				m_SendResultList.SetItemText(i, 1, strImportTime);
				m_SendResultList.SetItemText(i, 2, strTotalCount);
				m_SendResultList.SetItemText(i, 3, strUnsendCount);
				m_SendResultList.SetItemText(i, 4, strSendingCount);
				m_SendResultList.SetItemText(i, 5, strSuccessCount);
				m_SendResultList.SetItemText(i, 6, strFailedCount);			
			}
		}
	}
}

void CCppInterfaceDemoDlg::Split(CString source, CStringArray& dest, CString division)
{
	dest.RemoveAll();
	int pos = 0;
	int pre_pos = 0;
	while( -1 != pos ){
		pre_pos = pos;
		pos = source.Find(division,(pos+1));
		dest.Add(source.Mid(pre_pos,(pos-pre_pos)));
	}

}


void CCppInterfaceDemoDlg::OnNMDblclkListSendResult(NMHDR *pNMHDR, LRESULT *pResult)
{
	POSITION pos=m_SendResultList.GetFirstSelectedItemPosition();
	if (pos != NULL)
	{
		int nItem=m_SendResultList.GetNextSelectedItem(pos);
		CString strTableName=m_SendResultList.GetItemText(nItem, 0);

		CBatchDetailDlg dlg;
		dlg.SetTableName(strTableName);
		dlg.DoModal();
	}

	*pResult = 0;
}

