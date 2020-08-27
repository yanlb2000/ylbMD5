#include "stdafx.h"
#include "FindEmail.h"

#define chMailCharFirst '!'
#define chMailCharLast '~'
#define chAT '@'
#define chSpace ' '
#define chNUL 0x0

#define MaxPrefixLen 20
#define MinPrefixLen 1
#define MaxSuffixLen 40
#define MinSuffixLen 4

#define EmailAppendFormat _T("%s@%s; ")

bool IsEmailChar(char ch)
{
	bool bRet = false;
	if( (ch > chMailCharFirst) 
		&& (ch < chMailCharLast) 
		&& (ch != chAT) 
		&& (ch != chSpace)
		&& (ch != '\'')
		&& (ch != '\"')
		&& (ch != '<')
		&& (ch != '>')
		&& (ch != ';')
		&& (ch != ':')
		&& (ch != '\\')
		&& (ch != '/')
		&& (ch != '?')
		&& (ch != '=')
		&& (ch != '&')
		&& (ch != '(')
		&& (ch != ')')
		)
		bRet = true;
	if(ch == chNUL)
		bRet = true;
	return bRet;
}


bool IsValidPrefix(CString str1)
{
	bool bRet = true;
	if(str1.GetLength() > MaxPrefixLen)
		return false;
	if(str1.GetLength() < MinPrefixLen)
		return false;
	return bRet;
}

bool IsValidSuffix(CString str1)
{
	bool bRet = true;
	if(str1.GetLength() > MaxSuffixLen)
		return false;
	if(str1.GetLength() < MinSuffixLen)
		return false;
	if(str1.Find('.') == -1)
		return false;
	if(str1.GetAt(0) == '.')
		return false;
	return bRet;
}

void ModifySuffix(CString &str1)
{
	int len = str1.GetLength();
	if(len < MinSuffixLen)
		return;
	int p = len - 1;
	char ch;
	do
	{
		ch = str1.GetAt(p);
		p--;
	}while(p>=0	&& (ch < 'A' || ch>'Z' && ch<'a' || ch>'z') );
	if(p +1 < len -1)
		str1 = str1.Mid(0, p);
}

CString GetSuffix(BYTE *Input, BYTE * p, longword Length)
{
	CString strRet;
	bool bStop = false;
	BYTE * pb = p;
	BYTE * pStop = p + Length;
	char c;
	while(!bStop)
	{
		pb++;
		if(pb <= pStop)
		{
			c = *(char*)pb;
		}
		if(IsEmailChar(c))
		{
			if(c != chNUL)
				strRet.AppendChar(c);
		}
		else bStop = true;
		if((pb - p)> MaxSuffixLen) bStop = true;
		if(pb>= pStop) bStop = true;
	}
	return strRet;
}


CString GetPrefix(BYTE *Input, BYTE * p, CString *strRemain)
{
	CString strRet;
	bool bStop = false;
	BYTE * pb = p;
	char c;
	int RemainLen = strRemain->GetLength();
	while(!bStop)
	{
		pb--;
		if(pb>=Input)
		{
			c = *(char*)pb;
		}
		else
		{
			c = strRemain->GetAt(RemainLen -(Input - pb));
		}
		if(IsEmailChar(c))
		{
			if(c != chNUL)
				strRet.Insert(0, c);
		}
		else bStop = true;
		if((p - pb)> MaxPrefixLen) bStop = true;
		if((RemainLen -(Input - pb))<=0) bStop = true;
	}
	return strRet;
}

CString EmailInBlock(BYTE *Input, longword Length, CString *strRemain)
{
	CString strRet;
	BYTE *p = Input;
	CString strPrefix, strSuffix;
	char c;
	while(p<(Input + Length))
	{
		c = *(char*)p;
		if(c == chAT)
		{
			strPrefix = GetPrefix(Input, p, strRemain);
			strSuffix = GetSuffix(Input, p, Length);
			ModifySuffix(strSuffix);
			if(IsValidPrefix(strPrefix) && IsValidSuffix(strSuffix))
				strRet.AppendFormat(EmailAppendFormat, strPrefix, strSuffix);
		}
		p++;
	}
	return strRet;
}

void EmailInFile(CString *strFile, CString *strEmail)
{
	HANDLE FileHandle;
	HANDLE MapHandle;
	CString strRet, strRemain; 

	strRet = "";
	FileHandle = CreateFile(*strFile,
		GENERIC_READ,
		FILE_SHARE_READ | FILE_SHARE_WRITE,
		NULL,
		OPEN_EXISTING,
		FILE_ATTRIBUTE_NORMAL | FILE_FLAG_SEQUENTIAL_SCAN,
		0);
	if(FileHandle!=INVALID_HANDLE_VALUE)
	{
		MapHandle = CreateFileMapping(FileHandle, NULL, PAGE_READONLY, 0, 0, NULL);
		if(MapHandle!=NULL)
		{
			ULONGLONG FileSize, pFileMap;
			LARGE_INTEGER FileSize2;
			longword BufferLength = 0;
			void *ViewPointer = NULL;
			MEMORY_BASIC_INFORMATION MemInfo;
			DWORD DesiredBufferLength = 0x10000000; // 256MB

			GetFileSizeEx(FileHandle, &FileSize2);
			FileSize = FileSize2.QuadPart;
			pFileMap = 0;
			do{			// main loop, map all file content to memory and process
				if( DesiredBufferLength > (FileSize - pFileMap))
					DesiredBufferLength = FileSize - pFileMap;
				do{		// loop until we successfully map file 

					ViewPointer = MapViewOfFile(MapHandle, 
						FILE_MAP_READ,  
						pFileMap >> 32, 
						pFileMap & 0xffffffff, 
						DesiredBufferLength);
					if(ViewPointer != NULL)
					{
						VirtualQuery(ViewPointer, &MemInfo, sizeof(MemInfo));
						BufferLength =  MemInfo.RegionSize;
						// maybe the DesiredBufferLength is too big to allocate
						if(BufferLength <= 0) DesiredBufferLength <<= 1;	
					}
				}while( BufferLength <= 0 && DesiredBufferLength >= 0x100000 /* 1MB */);
				if(BufferLength >0)
				{
					if(pFileMap + BufferLength > FileSize)
						BufferLength = FileSize - pFileMap;
					strRet.Append(EmailInBlock((BYTE *) ViewPointer, BufferLength, &strRemain));					
					UnmapViewOfFile(ViewPointer);
					pFileMap += BufferLength;
				}
			} while(pFileMap<FileSize);
			CloseHandle(MapHandle);
		}
		CloseHandle(FileHandle);
	}
	//MD5Final(&Context, Digest);
	strEmail->Append(strRet);
	return ;
}