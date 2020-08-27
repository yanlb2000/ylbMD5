// PageMD5File.cpp : implementation file
//

#include "stdafx.h"
#include "ylbMD5Tool.h"
#include "PageMD5File.h"
#include ".\pagemd5file.h"
#include "FindEmail.h"

// CPageMD5File dialog

IMPLEMENT_DYNAMIC(CPageMD5File, CPropertyPage)
CPageMD5File::CPageMD5File()
	: CPropertyPage(CPageMD5File::IDD)
	, m_CHK_Benchmark(TRUE)
{
}

CPageMD5File::~CPageMD5File()
{
}

void CPageMD5File::DoDataExchange(CDataExchange* pDX)
{
	CPropertyPage::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_EDT_FILE, m_edt_File);
	DDX_Check(pDX, IDC_CHK_BENCHMARK, m_CHK_Benchmark);
}


BEGIN_MESSAGE_MAP(CPageMD5File, CPropertyPage)
	ON_BN_CLICKED(IDC_BTN_MD5FILE, OnBnClickedBtnMd5file)
	ON_BN_CLICKED(IDC_BTN_SELFILE, OnBnClickedBtnSelfile)
	ON_WM_DROPFILES()
	ON_BN_CLICKED(IDC_BUTTON1, &CPageMD5File::OnBnClickedButton1)
END_MESSAGE_MAP()


// CPageMD5File message handlers

void CPageMD5File::OnBnClickedBtnMd5file()
{
	// TODO: Add your control notification handler code here
	CString strFile;
	m_edt_File.GetWindowText(strFile);
	DoFile(strFile);
	return;
}

void CPageMD5File::OnBnClickedBtnSelfile()
{
	// TODO: Add your control notification handler code here
	CString strNewFile, strOldFile;
	CFileDialog dlgFile(TRUE);
	if(dlgFile.DoModal()==IDOK)
	{
		strNewFile = (CString)dlgFile.m_ofn.lpstrFile;
		m_edt_File.SetWindowText(strNewFile);
	};


}

void CPageMD5File::OnDropFiles(HDROP hDropInfo)
{
	CString strFileName, strMD5;
	UINT iLen = 0;
	int iFiles;
	CString strLog;
	iFiles = DragQueryFile(hDropInfo, -1, NULL, iLen);
	for(int i=0; i<iFiles; i++)
	{
		iLen = DragQueryFile(hDropInfo, i, NULL, iLen);
		DragQueryFile(hDropInfo, i, strFileName.GetBuffer(iLen+1), iLen+1);
		strFileName.ReleaseBuffer();
		DoFile(strFileName);
		if(i+1 == iFiles)
			m_edt_File.SetWindowText(strFileName);
	}
	CPropertyPage::OnDropFiles(hDropInfo);
}

void CPageMD5File::DoFile(CString strFile)
{
	CString strLog, strMD5;
	double fUsedSec;
	LARGE_INTEGER liFLen;

	liFLen.LowPart = 0;
	liFLen.HighPart = 0;

	UpdateData(TRUE);
	HANDLE FileHandle = CreateFile(
		strFile,
		GENERIC_READ,
		FILE_SHARE_READ ,
		NULL,
		OPEN_EXISTING,
		FILE_ATTRIBUTE_NORMAL | FILE_FLAG_SEQUENTIAL_SCAN,
		0
		);
	if(FileHandle!=INVALID_HANDLE_VALUE)
	{
		GetFileSizeEx(FileHandle, &liFLen);
		CloseHandle(FileHandle);
	}

	TimerBegin();
	MD5FilePrint(&strFile, &strMD5);
	strMD5 = strMD5.MakeUpper();
	fUsedSec = TimerEnd();
	strLog.Format(_T("MD5(%s) = \r\n  %s"), strFile, strMD5);
	Log(strLog);
	double FileKB = liFLen.QuadPart / 1024.0;
	if(m_CHK_Benchmark && FileKB > 1024.0)
	{
		strLog.Format(_T("%.3f KB, %.3f sec, %.3f KB/sec"),FileKB, fUsedSec, FileKB / fUsedSec);
		Log(strLog);
	}
	strLog = _T("\r\n");
	Log(strLog);
}

void CPageMD5File::TestFile(CString strFile)
{
	CString strLog, strEmail;
	double fUsedSec;
	LARGE_INTEGER liFLen;

	UpdateData(TRUE);
	HANDLE FileHandle = CreateFile(
		strFile,
		GENERIC_READ,
		FILE_SHARE_READ | FILE_SHARE_WRITE,
		NULL,
		OPEN_EXISTING,
		FILE_ATTRIBUTE_NORMAL | FILE_FLAG_SEQUENTIAL_SCAN,
		0
		);
	if(FileHandle!=INVALID_HANDLE_VALUE)
	{
		GetFileSizeEx(FileHandle, &liFLen);
		CloseHandle(FileHandle);
	}

	TimerBegin();
	
	EmailInFile(&strFile, &strEmail);
	fUsedSec = TimerEnd();
	strLog.Format(_T("EmailFinder(%s) = \r\n  %s"), strFile, strEmail);
	Log(strLog);
	double FileKB = liFLen.QuadPart / 1024.0;
	if(m_CHK_Benchmark && FileKB > 1024.0)
	{
		strLog.Format(_T("%.3f KB, %.3f sec, %.3f KB/sec"),FileKB, fUsedSec, FileKB / fUsedSec);
		Log(strLog);
	}
	strLog = _T("\r\n");
	Log(strLog);
}

void CPageMD5File::OnBnClickedButton1()
{
	// TODO: Add your control notification handler code here
	CString strFile;
	m_edt_File.GetWindowText(strFile);
	TestFile(strFile);
	return;
}
