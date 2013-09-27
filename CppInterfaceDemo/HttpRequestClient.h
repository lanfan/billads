// HttpPostClient.h: interface for the CHttpPostClient class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_HTTPPOSTCLIENT_H__488E2A59_BF0D_40AF_B627_973DDF938DC1__INCLUDED_)
#define AFX_HTTPPOSTCLIENT_H__488E2A59_BF0D_40AF_B627_973DDF938DC1__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "wininet.h"
#include "afxinet.h"

class CHttpRequestClient  
{
public:
	CHttpRequestClient();
	virtual ~CHttpRequestClient();

public:
	void AddPostParam(CString name,CString value);
	CString DoPost(CString href);
	CString DoGet(CString href);
	CString Convert(CString   str,   int   sourceCodepage,   int   targetCodepage);
	
private:
	CString m_sContentStr;
	int m_nContentLen;
	CStringList m_valueList;
	CStringList m_nameList;

	


};

#endif // !defined(AFX_HTTPPOSTCLIENT_H__488E2A59_BF0D_40AF_B627_973DDF938DC1__INCLUDED_)
