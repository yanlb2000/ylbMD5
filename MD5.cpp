#include "stdafx.h"

//#include "MD5.h"

MD5Buffer PADDING={
	0x80, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
};

inline DWORD F(DWORD x, DWORD y, DWORD z)
{
	return (x & y) | ((~x) & z);
}

inline DWORD G(DWORD x, DWORD y, DWORD z)
{
	return (x & z) | (y & (~z));
}

inline DWORD H(DWORD x, DWORD y, DWORD z)
{
	return x ^ y ^ z;
}

inline DWORD I(DWORD x, DWORD y, DWORD z)
{
	return y ^ (x | (~z));
}


inline void rot(DWORD *x, BYTE n)
{
	*x = (*x<<n) | (*x>>(32-n));
} ;


inline void FF(DWORD *a, DWORD b, DWORD c, DWORD d, DWORD x, BYTE s, DWORD ac)
{
	*a += (F(b, c, d) + x + ac);
	rot(a, s);
	*a += b;
}

inline void GG(DWORD *a, DWORD b, DWORD c, DWORD d, DWORD x, BYTE s, DWORD ac)
{
	*a += (G(b, c, d) + x + ac);
	rot(a, s);
	*a += b;
}

inline void HH(DWORD *a, DWORD b, DWORD c, DWORD d, DWORD x, BYTE s, DWORD ac)
{
	*a += (H(b, c, d) + x + ac);
	rot(a, s);
	*a += b;
}
inline void II(DWORD *a, DWORD b, DWORD c, DWORD d, DWORD x, BYTE s, DWORD ac)
{
	*a += (I(b, c, d) + x + ac);
	rot(a, s);
	*a += b;
}

// Encode Count bytes at Source into (Count / 4) DWORDs at Target
void Encode(void *Source, void *Target, longword Count)
{
	BYTE *S;
	DWORD *T;
	longword I;

	S = (BYTE *)Source;
	T = (DWORD *)Target;
	for(I=1;I<=Count/4;I++)
	{
		*T = *S;
		S++;
		*T = *T | (*S << 8);
		S++;
		*T = *T | (*S <<16);
		S++;
		*T = *T | (*S <<24);
		S++;
		T++;
	}
}

// Decode Count DWORDs at Source into (Count * 4) Bytes at Target
void Decode(void *Source, void *Target, longword Count)
{
	DWORD *S;
	BYTE *T;
	longword I;

	S = (DWORD *)Source;
	T = (BYTE *)Target;
	for(I=1;I<=Count;I++)
	{
		*T = *S & 0xff;
		T++;
		*T = (*S >> 8) & 0xff;
		T++;
		*T = (*S >> 16) & 0xff;
		T++;
		*T = (*S >> 24) & 0xff;
		T++;
		S++;
	}
}

// Transform State according to first 64 bytes at Buffer
void Transform(void *Buffer, DWORD *State)
{
	DWORD a, b, c, d;
	MD5Block Block;

	Encode(Buffer, Block, 64);
	a = State[0];
	b = State[1];
	c = State[2];
	d = State[3];
	FF (&a, b, c, d, Block[ 0],  7, 0xd76aa478);
	FF (&d, a, b, c, Block[ 1], 12, 0xe8c7b756);
	FF (&c, d, a, b, Block[ 2], 17, 0x242070db);
	FF (&b, c, d, a, Block[ 3], 22, 0xc1bdceee);
	FF (&a, b, c, d, Block[ 4],  7, 0xf57c0faf);
	FF (&d, a, b, c, Block[ 5], 12, 0x4787c62a);
	FF (&c, d, a, b, Block[ 6], 17, 0xa8304613);
	FF (&b, c, d, a, Block[ 7], 22, 0xfd469501);
	FF (&a, b, c, d, Block[ 8],  7, 0x698098d8);
	FF (&d, a, b, c, Block[ 9], 12, 0x8b44f7af);
	FF (&c, d, a, b, Block[10], 17, 0xffff5bb1);
	FF (&b, c, d, a, Block[11], 22, 0x895cd7be);
	FF (&a, b, c, d, Block[12],  7, 0x6b901122);
	FF (&d, a, b, c, Block[13], 12, 0xfd987193);
	FF (&c, d, a, b, Block[14], 17, 0xa679438e);
	FF (&b, c, d, a, Block[15], 22, 0x49b40821);
	GG (&a, b, c, d, Block[ 1],  5, 0xf61e2562);
	GG (&d, a, b, c, Block[ 6],  9, 0xc040b340);
	GG (&c, d, a, b, Block[11], 14, 0x265e5a51);
	GG (&b, c, d, a, Block[ 0], 20, 0xe9b6c7aa);
	GG (&a, b, c, d, Block[ 5],  5, 0xd62f105d);
	GG (&d, a, b, c, Block[10],  9,  0x2441453);
	GG (&c, d, a, b, Block[15], 14, 0xd8a1e681);
	GG (&b, c, d, a, Block[ 4], 20, 0xe7d3fbc8);
	GG (&a, b, c, d, Block[ 9],  5, 0x21e1cde6);
	GG (&d, a, b, c, Block[14],  9, 0xc33707d6);
	GG (&c, d, a, b, Block[ 3], 14, 0xf4d50d87);
	GG (&b, c, d, a, Block[ 8], 20, 0x455a14ed);
	GG (&a, b, c, d, Block[13],  5, 0xa9e3e905);
	GG (&d, a, b, c, Block[ 2],  9, 0xfcefa3f8);
	GG (&c, d, a, b, Block[ 7], 14, 0x676f02d9);
	GG (&b, c, d, a, Block[12], 20, 0x8d2a4c8a);
	HH (&a, b, c, d, Block[ 5],  4, 0xfffa3942);
	HH (&d, a, b, c, Block[ 8], 11, 0x8771f681);
	HH (&c, d, a, b, Block[11], 16, 0x6d9d6122);
	HH (&b, c, d, a, Block[14], 23, 0xfde5380c);
	HH (&a, b, c, d, Block[ 1],  4, 0xa4beea44);
	HH (&d, a, b, c, Block[ 4], 11, 0x4bdecfa9);
	HH (&c, d, a, b, Block[ 7], 16, 0xf6bb4b60);
	HH (&b, c, d, a, Block[10], 23, 0xbebfbc70);
	HH (&a, b, c, d, Block[13],  4, 0x289b7ec6);
	HH (&d, a, b, c, Block[ 0], 11, 0xeaa127fa);
	HH (&c, d, a, b, Block[ 3], 16, 0xd4ef3085);
	HH (&b, c, d, a, Block[ 6], 23,  0x4881d05);
	HH (&a, b, c, d, Block[ 9],  4, 0xd9d4d039);
	HH (&d, a, b, c, Block[12], 11, 0xe6db99e5);
	HH (&c, d, a, b, Block[15], 16, 0x1fa27cf8);
	HH (&b, c, d, a, Block[ 2], 23, 0xc4ac5665);
	II (&a, b, c, d, Block[ 0],  6, 0xf4292244);
	II (&d, a, b, c, Block[ 7], 10, 0x432aff97);
	II (&c, d, a, b, Block[14], 15, 0xab9423a7);
	II (&b, c, d, a, Block[ 5], 21, 0xfc93a039);
	II (&a, b, c, d, Block[12],  6, 0x655b59c3);
	II (&d, a, b, c, Block[ 3], 10, 0x8f0ccc92);
	II (&c, d, a, b, Block[10], 15, 0xffeff47d);
	II (&b, c, d, a, Block[ 1], 21, 0x85845dd1);
	II (&a, b, c, d, Block[ 8],  6, 0x6fa87e4f);
	II (&d, a, b, c, Block[15], 10, 0xfe2ce6e0);
	II (&c, d, a, b, Block[ 6], 15, 0xa3014314);
	II (&b, c, d, a, Block[13], 21, 0x4e0811a1);
	II (&a, b, c, d, Block[ 4],  6, 0xf7537e82);
	II (&d, a, b, c, Block[11], 10, 0xbd3af235);
	II (&c, d, a, b, Block[ 2], 15, 0x2ad7d2bb);
	II (&b, c, d, a, Block[ 9], 21, 0xeb86d391);
	*State += a;
	*(State+1) += b;
	*(State+2) += c;
	*(State+3) += d;
	return;
}

// Initialize given Context
void MD5Init(MD5Context *Context)
{
	Context->State[0] = 0x67452301;
	Context->State[1] = 0xefcdab89;
	Context->State[2] = 0x98badcfe;
	Context->State[3] = 0x10325476;
	Context->Count[0] = 0;
	Context->Count[1] = 0;
	ZeroMemory(Context->Buffer, sizeof(MD5Buffer));
	return;
}

// Update given Context to include Length bytes of Input
void MD5Update(MD5Context *Context, BYTE *Input, longword Length)
{
	longword Index;
	longword PartLen;
	longword I;

	Index = (Context->Count[0]>>3) & 0x3f;
	(Context->Count[0]) += Length<<3;
	if(Context->Count[0]<(Length<<3))Context->Count[1]++;
	(Context->Count[1]) += Length>>29;
	PartLen = 64 - Index;

	if(Length>=PartLen)
	{
		CopyMemory(Context->Buffer + Index, Input, PartLen);
		Transform(&(Context->Buffer), Context->State);
		I = PartLen;
		while(I+63<Length)
		{
			Transform(Input+I, Context->State);
			I += 64;
		}
		Index = 0;
	}
	else I = 0;
	CopyMemory(&(Context->Buffer[Index]) , Input + I, Length -I);
	return;
}

// Finalize given Context, create Digest and zeroize Context
void MD5Final(MD5Context *Context, MD5Digest *Digest)
{
	MD5CBits Bits;
	longword Index;
	longword PadLen;

	Decode(&Context->Count, Bits, 2);
	Index = (Context->Count[0] >> 3) & 0x3f;
	PadLen = Index<56?(56-Index):(120-Index);
	MD5Update(Context, PADDING, PadLen);
	MD5Update(Context, Bits, 8);
	Decode(&Context->State, Digest, 4);
	ZeroMemory(Context, sizeof(MD5Context));
	return;
}


// Create hex representation of given Digest
void MD5Print(MD5Digest D, CString *strPrint)
{
	BYTE I;
	BYTE Digits[]={	'0', '1', '2', '3', '4', '5', '6', '7',
					'8', '9', 'a', 'b', 'c', 'd', 'e', 'f'};

	*strPrint = "";
	for(I=0;I<=15;I++)
	{
		*strPrint += Digits[(D[I]>>4) & 0x0f];
		*strPrint += Digits[D[I] & 0x0f];
	}
	return;
}

void MD5StringPrint(CString *M, CString *strPrint)
{
	MD5Digest md5dig;
	MD5String(M, &md5dig);
	MD5Print(md5dig, strPrint);
	return;
}

void MD5FilePrint(CString *M, CString *strPrint)
{
	MD5Digest md5dig;
	MD5File(M, &md5dig);
	MD5Print(md5dig, strPrint);
	return;
}

// Compare two Digests
BOOL MD5Match(MD5Digest D1,MD5Digest D2)
{
	BYTE I=0;
	BOOL Result=TRUE;
	while(Result & (I<16))
	{
		Result = (D1[I]==D2[I]);
		I++;
	}
	return Result;
}

// Create digest of given Message
void MD5String(CString *M, MD5Digest *Digest)
{
	MD5Context Context;

	MD5Init(&Context);
	MD5Update(&Context, (BYTE *)(LPCSTR)*M, M->GetLength());
	MD5Final(&Context, Digest);
	return;
}

// Create digest of file with given name
void MD5File(CString *N, MD5Digest *Digest)
{
	HANDLE FileHandle;
	HANDLE MapHandle;
	MD5Context Context;

	MD5Init(&Context);
	FileHandle = CreateFile(*N,
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
					MD5Update(&Context, (BYTE *)ViewPointer, BufferLength);
					UnmapViewOfFile(ViewPointer);
					pFileMap += BufferLength;
				}
			} while(pFileMap<FileSize);
			CloseHandle(MapHandle);
		}
		CloseHandle(FileHandle);
	}
	MD5Final(&Context, Digest);
	return ;
}
