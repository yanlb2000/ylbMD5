#pragma once
#include "afxwin.h"


// CPageMD5File dialog

class CPageMD5File : public CPropertyPage
{
	DECLARE_DYNAMIC(CPageMD5File)

public:
	CPageMD5File();
	virtual ~CPageMD5File();

// Dialog Data
	enum { IDD = IDD_PAGE_MD5FILE };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
public:
	CEdit m_edt_File;
	afx_msg void OnBnClickedBtnMd5file();
	afx_msg void OnBnClickedBtnSelfile();
	afx_msg void OnDropFiles(HDROP hDropInfo);
	BOOL m_CHK_Benchmark;

	void DoFile(CString strFile);
	void TestFile(CString strFile);

public:
	afx_msg void OnBnClickedButton1();
};
