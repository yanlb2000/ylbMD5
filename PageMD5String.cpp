// PageMD5String.cpp : implementation file
//

#include "stdafx.h"
#include "ylbMD5Tool.h"
#include "PageMD5String.h"
#include ".\pagemd5string.h"


//#include "ylbMD5ToolDlg.h"

//extern CylbMD5ToolDlg dlg;


// CPageMD5String dialog

IMPLEMENT_DYNAMIC(CPageMD5String, CPropertyPage)
CPageMD5String::CPageMD5String()
	: CPropertyPage(CPageMD5String::IDD)
{
}

CPageMD5String::~CPageMD5String()
{
}

void CPageMD5String::DoDataExchange(CDataExchange* pDX)
{
	CPropertyPage::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_EDT_STRING, m_edt_String);
}


BEGIN_MESSAGE_MAP(CPageMD5String, CPropertyPage)
	ON_BN_CLICKED(IDC_BTN_STRING, OnBnClickedBtnString)
END_MESSAGE_MAP()


// CPageMD5String message handlers

void CPageMD5String::OnBnClickedBtnString()
{
	// TODO: Add your control notification handler code here
	CString strSource, strMD5, strLog;
	m_edt_String.GetWindowText(strSource);
	MD5StringPrint(&strSource, &strMD5);
	strMD5 = strMD5.MakeUpper();
	strLog.Format(_T("MD5('%s') = %s"), strSource, strMD5);
	Log(strLog);
	return;
}
