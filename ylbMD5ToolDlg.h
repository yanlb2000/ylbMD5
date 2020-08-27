// ylbMD5ToolDlg.h : header file
//

#pragma once

#include "afxwin.h"

#include "PageMD5String.h"
#include "PageMD5File.h"
#include "PageMD5Time.h"

// CylbMD5ToolDlg dialog
class CylbMD5ToolDlg : public CDialog
{
// Construction
public:
	CylbMD5ToolDlg(CWnd* pParent = NULL);	// standard constructor

// Dialog Data
	enum { IDD = IDD_YLBMD5TOOL_DIALOG };
	CPropertySheet m_sheet;
	CPageMD5String m_PageString;
	CPageMD5File m_PageFile;
	CPageMD5Time m_PageMD5Time;

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV support


// Implementation
protected:
	HICON m_hIcon;

	// Generated message map functions
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()
public:
	CEdit m_edt_String;
	afx_msg void OnBnClickedBtnClear();
	afx_msg void OnDropFiles(HDROP hDropInfo);
};

