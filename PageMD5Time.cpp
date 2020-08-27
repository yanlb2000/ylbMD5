// PageMD5Time.cpp : implementation file
//

#include "stdafx.h"
#include "ylbMD5Tool.h"
#include "PageMD5Time.h"
#include ".\pagemd5time.h"


// CPageMD5Time dialog

IMPLEMENT_DYNAMIC(CPageMD5Time, CPropertyPage)
CPageMD5Time::CPageMD5Time()
	: CPropertyPage(CPageMD5Time::IDD)
	, m_edt_BlockSize(10000)
	, m_edt_BlockCount(1000)
{
}

CPageMD5Time::~CPageMD5Time()
{
}

void CPageMD5Time::DoDataExchange(CDataExchange* pDX)
{
	CPropertyPage::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_EDT_BLOCK_SIZE, m_edt_BlockSize);
	DDX_Text(pDX, IDC_EDT_BLOCK_COUNT, m_edt_BlockCount);
}


BEGIN_MESSAGE_MAP(CPageMD5Time, CPropertyPage)
	ON_BN_CLICKED(IDC_BTN_START_TIME, OnBnClickedBtnStartTime)
END_MESSAGE_MAP()


// CPageMD5Time message handlers

void CPageMD5Time::OnBnClickedBtnStartTime()
{
	// TODO: Add your control notification handler code here
	DWORD BLength, BCount;
	void *Block;
	BYTE *B;
	DWORD I;
	longword Ticks;
	CString strLog, strSpeed, strMD5;

	MD5Context Context;
	MD5Digest Digest;

	UpdateData(TRUE);
	BLength = m_edt_BlockSize;
	BCount = m_edt_BlockCount;

	Block = malloc(BLength+1);

	strLog.Format(_T("Start MD5 time trial. With %d bytes * %d blocks."), BLength, BCount);
	B = (BYTE *)Block;
	I = 0;
	while(I<BLength)
	{
		*B = I & 0xff;
		B++;
		I++;
	}
	Ticks = GetTickCount();
	MD5Init(&Context);
	I = 0;
	while(I<BLength)
	{
		MD5Update(&Context, (BYTE *)&Block, BLength);
		I++;
	}
	MD5Final(&Context, &Digest);
	Ticks = GetTickCount() - Ticks;
	if(BLength * BCount ==0 || Ticks == 0 )
		Log(_T("Speed: n/a"));
	else
	{
		strSpeed.Format(_T("%d bytes/sec"), BLength * BCount * 1000 / Ticks);
		MD5Print(Digest, &strMD5);
		strLog.Format(_T(" Digest: %s"), strMD5);
		Log(strLog);
		strLog.Format(_T(" Time: %d seconds"), Ticks /1000);
		Log(strLog);
		strLog.Format(_T(" Speed: %s"),strSpeed);
		Log(strLog);
	}
	free(Block);
	return;
}
