#include "stdafx.h"

CEdit *edtLog;
CString strLog;

LARGE_INTEGER liBegin, liEnd, liFrequency;

double fSec;

void AssignLogEdt(CEdit *pedtLog)
{
	edtLog = pedtLog;
}

void Log(CString strLog)
{
	CString strT;
	edtLog->GetWindowText(strT);
	strT.Append(_T("\r\n"));
	strT.Append(strLog);
	edtLog->SetWindowText(strT);
	edtLog->LineScroll(edtLog->GetLineCount());
	return;
}



void TimerBegin()
{
	QueryPerformanceCounter(&liBegin);
}

double TimerEnd()
{
	int iFrequency;
	QueryPerformanceCounter(&liEnd);
	QueryPerformanceFrequency(&liFrequency);
	iFrequency=(int)liFrequency.QuadPart;
	fSec=(double)(liEnd.QuadPart -liBegin.QuadPart) / (double)liFrequency.QuadPart;
	return fSec;
}

