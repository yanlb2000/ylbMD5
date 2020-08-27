#pragma once
#include "afxwin.h"


// CPageMD5String dialog


class CPageMD5String : public CPropertyPage
{
	DECLARE_DYNAMIC(CPageMD5String)

public:
	CPageMD5String();
	virtual ~CPageMD5String();

// Dialog Data
	enum { IDD = IDD_PAGE_MD5STRING };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
public:
	CEdit m_edt_String;
	afx_msg void OnBnClickedBtnString();
};
