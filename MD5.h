#include "windows.h"

typedef DWORD MD5Count[2];
typedef DWORD MD5State[4];
typedef DWORD MD5Block[16];
typedef BYTE MD5CBits[8];
typedef BYTE MD5Digest[16];
typedef BYTE MD5Buffer[64];
typedef unsigned long longword;

struct MD5Context
{
	MD5State State;
	MD5Count Count;
	MD5Buffer Buffer;
};

void MD5Init(MD5Context *Context);

void MD5Update(MD5Context *Context, BYTE *Input, longword Length);

void MD5Final(MD5Context *Context, MD5Digest *Digest);

//MD5Digest *MD5String(CString *M);
void MD5String(CString *M, MD5Digest *Digest);

void MD5File(CString *N, MD5Digest *Digest);

void MD5Print(MD5Digest D, CString *strPrint);

void MD5StringPrint(CString *M, CString *strPrint);

void MD5FilePrint(CString *M, CString *strPrint);

BOOL MD5Match(MD5Digest D1,MD5Digest D2);

