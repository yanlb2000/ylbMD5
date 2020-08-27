#pragma once


// CPageMD5Time dialog

class CPageMD5Time : public CPropertyPage
{
	DECLARE_DYNAMIC(CPageMD5Time)

public:
	CPageMD5Time();
	virtual ~CPageMD5Time();

// Dialog Data
	enum { IDD = IDD_PAGE_MD5TIME };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
public:
	DWORD m_edt_BlockSize;
	DWORD m_edt_BlockCount;
	afx_msg void OnBnClickedBtnStartTime();
};
