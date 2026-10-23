#define ZIP_CPP
#include <windows.h>
#include "Zip.h"
#include "Zlib.h"

#pragma comment(lib,"zlib.lib")


CZip::CZip()
{
	Init();
}

CZip::~CZip()
{
}

VOID CZip::Init()
{
	m_hFile = NULL;
	m_cData = NULL;
	
	ZeroMemory(&m_sZi,sizeof(ZIPINFO));
	ZeroMemory(m_szFileName,sizeof(m_szFileName));
	//m_cCompressData = NULL;
	//m_cUnCompreeData = NULL;
	//m_ulCompressLen = 0;
	//m_ulComprLen = 0;
	//m_ulUnComprLen = 0;
	m_dwFileSize = 0;


}

BOOL CZip::CompOpen(char* FileName)
{
	DWORD dwRead = 0;

	DeleteData();
	Init();
	
	m_hFile = CreateFile( FileName, GENERIC_READ, FILE_SHARE_READ,
                        NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL , NULL );

	if(m_hFile == INVALID_HANDLE_VALUE)
	{
		return FALSE;
	}

	GetFileName(FileName,'.');

	m_dwFileSize = GetFileSize(m_hFile,NULL);

	m_cData = (char*) new char[m_dwFileSize];	

	//m_cCompressData = (char*) new char[dwFileSize];
	//m_cUnCompreeData = (char*) new char[dwFileSize];
	//m_ulComprLen = dwFileSize * sizeof(int);

	ZeroMemory(m_cData,sizeof(m_cData));
	//ZeroMemory(m_cCompressData,sizeof(m_cCompressData));
	//ZeroMemory(m_cUnCompreeData,sizeof(m_cUnCompreeData));
	if(!m_cData)
	{
		return FALSE;
	}

	ReadFile(m_hFile,m_cData,m_dwFileSize,&dwRead,NULL);

	int i;
	i = lstrlen(m_cData);
	

	CloseHandle(m_hFile);

	if(ZipCompression(m_cData,FileName,m_dwFileSize))
	{
		char szTemp[100];
		ZeroMemory(szTemp,sizeof(szTemp));

		wsprintf(szTemp,"%s.yul",m_szFileName);
		Write(szTemp,&m_sZi,m_sZi.dwFileSize);
	}
	
	

	DeleteData();
	

	return TRUE;

}

//압축해제 열기
BOOL CZip::DeComOpen(char* FileName)
{
	DWORD dwRead = 0;
	DeleteData();
	Init();

	if(CheckFile(FileName,'.',"yul") != 0) 
	{
		MessageBox(NULL,"\306\304\300\317 \310\256\300\345\300\332\260\241 \306\262\270\263\264\317\264\331. \264\331\275\303 \307\321\271\370 \310\256\300\316\307\317\277\251\301\326\274\274\277\344","\306\304\300\317 \310\256\300\345\300\332 \277\241\266\363",MB_OK|MB_ICONSTOP);
		return FALSE;
	}

	m_hFile = CreateFile( FileName, GENERIC_READ, FILE_SHARE_READ,
                        NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL , NULL );

	if(m_hFile == INVALID_HANDLE_VALUE)
	{
		return FALSE;
	}	

	

	//m_sZi.dwFileSize = GetFileSize(m_hFile,NULL);
	

	m_dwFileSize = GetFileSize(m_hFile,NULL);
	m_cData = (char*)new char[m_dwFileSize];
	//m_sZi.cData = (char*) new char[m_sZi.dwFileSize];	

	//m_cCompressData = (char*) new char[dwFileSize];
	//m_cUnCompreeData = (char*) new char[dwFileSize];
	//m_ulComprLen = dwFileSize * sizeof(int);	

	//ReadFile(m_hFile,m_cData,m_dwFileSize,&dwRead,NULL);
	ReadFile(m_hFile,&m_sZi,m_dwFileSize,&dwRead,NULL);
	CloseHandle(m_hFile);

	//m_sZi = (ZIPINFO*)m_cData;
	//memcpy(&m_sZi,m_cData,sizeof(ZIPINFO));

	ZipDecompression(m_sZi.cData);

	return TRUE;

}

//압축
BOOL CZip::ZipCompression(char* data,char* FileName,DWORD len)
{
	unsigned long ulLen = 0;	
	int nRev = 0;

	ulLen = (unsigned long)len;//(unsigned long)strlen(data);

	//정보를 저장
	
	ZeroMemory(&m_sZi,sizeof(ZIPINFO));
	m_sZi.dwFileSize = m_dwFileSize;				//압축할 파일 사이즈
	m_sZi.dwVersion = YULVERSION;					//자체 포맷 버전
	strcpy(m_sZi.szZipVersion,ZLIB_VERSION);		//zlib버전
	strcpy(m_sZi.cUnCompFilName,FileName);			//압축할 파일 이름

	//m_sZi.cData = (char*)new char[m_dwFileSize];	
	m_sZi.ulCompLen	= ulLen * sizeof(int);
	m_sZi.dwDecopLen = len + 1;
	

	

	nRev = compress((Bytef*)&m_sZi.cData ,&m_sZi.ulCompLen,(Bytef*)data,ulLen);

	m_sZi.dwFileSize = (sizeof(char) * 64)*2 + sizeof(DWORD) * 4 + sizeof(unsigned long) + m_sZi.ulCompLen;

	if(nRev != 0) 
	{
		ZipCheckError(nRev);
		return FALSE;
	}
	
	return TRUE;
}



BOOL CZip::ZipDecompression(char* data)
{
	m_ulUnComprLen = m_ulComprLen;
	unsigned long nLen;
	int nRev = 0;
	
	nLen = (unsigned long)(m_dwFileSize * sizeof(int));
	m_cData = (char*)new char[nLen];
	ZeroMemory(m_cData,sizeof(m_cData));
	
	nRev = uncompress((Bytef*)m_cData,&nLen,(Bytef*)data,m_sZi.ulCompLen);

	if(nRev != 0) 
	{
		ZipCheckError(nRev);
		return FALSE;
	}

	Write(m_sZi.cUnCompFilName,m_cData,nLen);
	DeleteData();


	return TRUE;
}

BOOL CZip::Write(char* FileName,LPCVOID data,int len)
{
	DWORD dwWrite = 0;
	DWORD dwError = 0;
	char cTemp[256];	

	m_hFile = CreateFile( FileName, GENERIC_WRITE, FILE_SHARE_WRITE,
                        NULL, CREATE_NEW, FILE_ATTRIBUTE_NORMAL , NULL );

jump:
	if(m_hFile == INVALID_HANDLE_VALUE)
	{
		dwError = GetLastError();
		if(dwError == 80) //같은 파일이 존재 할 경우
		{		
			ZeroMemory(cTemp,sizeof(cTemp));

			wsprintf(cTemp,"\273\347\272\273-%s",FileName);

			m_hFile = CreateFile( cTemp, GENERIC_WRITE, FILE_SHARE_WRITE,
                        NULL, CREATE_NEW, FILE_ATTRIBUTE_NORMAL , NULL );

			goto jump;
		}
		

		return FALSE;
	}

	WriteFile(m_hFile,data,len,&dwWrite,NULL);

	CloseHandle(m_hFile);

	return TRUE;
}

VOID CZip::DeleteData()
{
	if(m_cData != NULL)
	{
		delete[] m_cData;
		m_cData = NULL;
	}

	//delete[] m_cCompressData;
	
	

	//m_cCompressData = NULL;
}

VOID CZip::ZipCheckError(int err)
{	
	char Temp[1024];
	switch(err)
	{
	case -1: //Z_ERRNO
		break;
	case -2: //Z_STREAM_ERROR
		break;
	case -3: //Z_DATA_ERROR
		strcpy(Temp,"\265\245\300\314\305\270\260\241 \277\241\267\257\300\324\264\317\264\331.");
		break;
	case -4: //Z_MEM_ERROR
		strcpy(Temp,"\270\336\270\360\270\256\260\241 \272\316\301\267\307\325\264\317\264\331.");
		break;
	case -5: //Z_BUF_ERROR		
		strcpy(Temp,"\307\317\265\345\277\353\267\256\300\314 \272\316\301\267\307\325\264\317\264\331.");
		break; 
	case -6: //Z_VERSION_ERROR
		break;
	}
	MessageBox(NULL,Temp,"Error",MB_OK|MB_ICONSTOP);
}

int CZip::CheckFile(char* FileName,int nMatch,char* cMath)
{
	char* cBuf;
	char cExet[10];
	int  nLen = 0;

	ZeroMemory(cExet,sizeof(cExet));
	strcpy(m_szFileName,FileName);

	cBuf = strchr(FileName,nMatch);

	nLen = lstrlen(cBuf);
	ZeroMemory(&m_szFileName[nLen],nLen);	
	strcpy(cExet,++cBuf);

	return strcmp(cExet,cMath);
}

VOID CZip::GetFileName(char* FileName,int nMatch)
{
	char* cBuf;	
	int  nLen = 0,nLen1 = 0;;

	
	strcpy(m_szFileName,FileName);

	nLen1 = lstrlen(FileName);

	cBuf = strchr(FileName,nMatch);

	nLen = lstrlen(cBuf);
	ZeroMemory(&m_szFileName[nLen1 -nLen],nLen);	
	
}


BOOL CZip::WriteYUL(char* FileName,char* data)
{
	DWORD dwWrite = 0;
	DWORD dwLen = 0;
	char szTemp[256];

	ZeroMemory(szTemp,sizeof(szTemp));

	wsprintf(szTemp,"%s\\%s.yul",FileName);

	m_hFile = CreateFile( szTemp, GENERIC_WRITE, FILE_SHARE_WRITE,
                        NULL, CREATE_NEW, FILE_ATTRIBUTE_NORMAL , NULL );

	if(m_hFile == INVALID_HANDLE_VALUE)
	{
		return FALSE;
	}

	dwLen = (DWORD)strlen(data);
	WriteFile(m_hFile,data,dwLen,&dwWrite,NULL);

	CloseHandle(m_hFile);
	return TRUE;
}