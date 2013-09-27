// CppInterfaceDemoDlg.cpp : ʵ���ļ�
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


// ����Ӧ�ó��򡰹��ڡ��˵���� CAboutDlg �Ի���

class CAboutDlg : public CDialog
{
public:
	CAboutDlg();

// �Ի�������
	enum { IDD = IDD_ABOUTBOX };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

// ʵ��
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


// CCppInterfaceDemoDlg �Ի���




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


// CCppInterfaceDemoDlg ��Ϣ�������

BOOL CCppInterfaceDemoDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	// ��������...���˵�����ӵ�ϵͳ�˵��С�

	// IDM_ABOUTBOX ������ϵͳ���Χ�ڡ�
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

	// ���ô˶Ի����ͼ�ꡣ��Ӧ�ó��������ڲ��ǶԻ���ʱ����ܽ��Զ�
	//  ִ�д˲���
	SetIcon(m_hIcon, TRUE);			// ���ô�ͼ��
	SetIcon(m_hIcon, FALSE);		// ����Сͼ��

	// TODO: �ڴ���Ӷ���ĳ�ʼ������
	m_strServerUrl=_T("http://www.fingersms.com/index.php?");
	m_strUsername=_T("demo");
	m_strPwd=_T("demo");

	m_strMobileList=_T("+8613912345678,+8613912345678");

	m_strContent=_T("�ӿڲ��Զ���");

	m_SendResultList.SetExtendedStyle(m_SendResultList.GetExtendedStyle() | 
		LVS_EX_GRIDLINES | LVS_EX_FULLROWSELECT );

	m_SendResultList.InsertColumn( 0, _T("����ID"));
	m_SendResultList.InsertColumn( 1, _T("����ʱ��"));
	m_SendResultList.InsertColumn( 2, _T("����"));
	m_SendResultList.InsertColumn( 3, _T("δ��"));
	m_SendResultList.InsertColumn( 4, _T("�ڷ�"));
	m_SendResultList.InsertColumn( 5, _T("�ɹ�"));
	m_SendResultList.InsertColumn( 6, _T("ʧ��"));

	m_SendResultList.SetColumnWidth( 0, 150);
	m_SendResultList.SetColumnWidth( 1, 130);
	m_SendResultList.SetColumnWidth( 2, 40);
	m_SendResultList.SetColumnWidth( 3, 40);
	m_SendResultList.SetColumnWidth( 4, 40);
	m_SendResultList.SetColumnWidth( 5, 40);
	m_SendResultList.SetColumnWidth( 6, 40);

	UpdateData(FALSE);

	return TRUE;  // ���ǽ��������õ��ؼ������򷵻� TRUE
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

// �����Ի��������С����ť������Ҫ����Ĵ���
//  �����Ƹ�ͼ�ꡣ����ʹ���ĵ�/��ͼģ�͵� MFC Ӧ�ó���
//  �⽫�ɿ���Զ���ɡ�

void CCppInterfaceDemoDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // ���ڻ��Ƶ��豸������

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// ʹͼ���ڹ��������о���
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// ����ͼ��
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialog::OnPaint();
	}
}

//���û��϶���С������ʱϵͳ���ô˺���ȡ�ù����ʾ��
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
		AfxMessageBox(_T("�������Ϊ��"));
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
			AfxMessageBox(_T("�ɹ���¼"));
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
			AfxMessageBox(_T("��ע��"));
		}
	}
}

void CCppInterfaceDemoDlg::OnBnClickedButtonSendSms()
{
	UpdateData(TRUE);

	if (m_strMobileList.IsEmpty() || m_strContent.IsEmpty())
	{
		AfxMessageBox(_T("���벻��Ϊ��"));
		return;
	}

	CHttpRequestClient HttpClient;

	// ע�⣺�ύ������Ҫ�ѼӺ��滻Ϊ%2B
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
					AfxMessageBox(_T("�����ύ�ɹ�"));
				else
					AfxMessageBox(_T("�����ύʧ��"));
			}
		}
		else
			AfxMessageBox(CString(_T("�������ʧ�ܣ�"))+value["msg"].asCString());
	}	
}

void CCppInterfaceDemoDlg::OnBnClickedButtonPostSend()
{
	UpdateData(TRUE);

	if (m_strMobileList.IsEmpty() || m_strContent.IsEmpty())
	{
		AfxMessageBox(_T("���벻��Ϊ��"));
		return;
	}

	CHttpRequestClient HttpClient;

	// ע�⣺�ύ������Ҫ�ѼӺ��滻Ϊ%2B
	m_strMobileList.Replace("+", "%2B");

	// ע����DoPost�������ת�������ﲻ��Ҫ��
	//m_strMobileList=HttpClient.Convert(m_strMobileList, 936, CP_UTF8);
	//m_strContent=HttpClient.Convert(m_strContent, 936, CP_UTF8);
	CString strUrl=m_strServerUrl+_T("r=site/SendSMSJSON");


	// ���԰��˺�һ���ύ�������ɲ����ȵ�¼
	//HttpClient.AddPostParam("LoginForm[username]","demo");
	//HttpClient.AddPostParam("LoginForm[password]","demo");
	HttpClient.AddPostParam("MobileList",m_strMobileList);
	HttpClient.AddPostParam("SMSContent",m_strContent);
	HttpClient.AddPostParam("MobileListDone","true");
	// ѡ���ö���ȷ��
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
			AfxMessageBox(CString(_T("�������ʧ�ܣ�"))+value["msg"].asCString());
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

			AfxMessageBox(_T("��ֵ������")+strChargedCnt+_T("�� ʹ�õ�����")+strUsedCnt);
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

