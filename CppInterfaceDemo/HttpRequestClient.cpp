// HttpPostClient.cpp: implementation of the CHttpRequestClient class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
//#include "smssendmanage.h"
#include "HttpRequestClient.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CHttpRequestClient::CHttpRequestClient()
{

}

CHttpRequestClient::~CHttpRequestClient()
{

}


CString CHttpRequestClient::DoGet(CString href)
{
	CString httpsource=_T("");
	CInternetSession session(_T("FingerSMS C++ Interface Demo"), 0);
	CHttpFile* pHTTPFile=NULL;
	try
	{
		pHTTPFile=(CHttpFile*)session.OpenURL(href);
	}
	catch(CInternetException)
	{
		pHTTPFile=NULL;
	}

	if(pHTTPFile)
	{
		CString text;
		for(int i=0; pHTTPFile->ReadString(text); i++)
		{
			httpsource=httpsource+text+_T("\r\n");
		}
		pHTTPFile->Close();
		delete pHTTPFile;
	}
	else
	{
	}
	return httpsource;
} 

CString CHttpRequestClient::DoPost(CString href)
{
	CString httpsource=_T("");
	CInternetSession session;
	CHttpConnection* pConn=NULL;
	CHttpFile* pFile = NULL; 
	CString strServerName;
	CString strObject;
	INTERNET_PORT nPort;
	DWORD dwServiceType;
	AfxParseURL((LPCTSTR)href,dwServiceType, strServerName, strObject, nPort);
	DWORD retcode;  

	CString utfStr = Convert(m_sContentStr,936,CP_UTF8);
	char* outBuff = utfStr.GetBuffer(0);
	// Convert(str,CP_UTF8,936);

	try 
	{        
		pConn = session.GetHttpConnection(strServerName,nPort);    
		pFile = pConn->OpenRequest(0,strObject,NULL,1,NULL,_T("HTTP/1.1"),
			INTERNET_FLAG_EXISTING_CONNECT|INTERNET_FLAG_NO_AUTO_REDIRECT);    
		pFile -> AddRequestHeaders(_T("Content-Type: application/x-www-form-urlencoded"));    
		pFile -> AddRequestHeaders(_T("Accept: */*"));            
		pFile -> SendRequest(NULL,0,outBuff,strlen(outBuff)+1);        
		pFile -> QueryInfoStatusCode(retcode);        
	}             
	catch (CInternetException* e)
	{
	};
	if(pFile)
	{
		CString text;
		for(int i=0;pFile->ReadString(text);i++)
		{
			httpsource=httpsource+text+_T("\r\n");
		}
		pFile->Close();
	}
	else
	{

	}
	return httpsource;
	delete pFile;    
	delete pConn;    
	session.Close();
}

void CHttpRequestClient::AddPostParam(CString name, CString value)
{
	m_nameList.AddTail((LPCTSTR)name);
	m_valueList.AddTail((LPCTSTR)value);
	CString eq=_T("=");
	CString an=_T("&");
	m_sContentStr=m_sContentStr+name+eq+value+an;
	m_nContentLen=m_sContentStr.GetLength();
}
CString CHttpRequestClient::Convert(CString   str,   int   sourceCodepage,   int   targetCodepage)   
{   
	int   len=str.GetLength();   

	int   unicodeLen=MultiByteToWideChar(sourceCodepage,0,str,-1,NULL,0);   

	wchar_t *   pUnicode;   
	pUnicode=new   wchar_t[unicodeLen+1];   

	memset(pUnicode,0,(unicodeLen+1)*sizeof(wchar_t));   


	MultiByteToWideChar(sourceCodepage,0,str,-1,(LPWSTR)pUnicode,unicodeLen);   

	BYTE   *   pTargetData=NULL;   
	int   targetLen=WideCharToMultiByte(targetCodepage,0,(LPWSTR)pUnicode,-1,(char   *)pTargetData,0,NULL,NULL);   

	pTargetData=new   BYTE[targetLen+1];   
	memset(pTargetData,0,targetLen+1);   

	WideCharToMultiByte(targetCodepage,0,(LPWSTR)pUnicode,-1,(char   *)pTargetData,targetLen,NULL,NULL);   

	CString   rt;   
	rt.Format(_T("%s"),pTargetData);   

	delete   pUnicode;   
	delete   pTargetData;   
	return   rt;   

}   
