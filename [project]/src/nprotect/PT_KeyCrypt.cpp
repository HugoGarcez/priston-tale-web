#define PT_NPKCRYPT_CPP

#include <windows.h>
#include "PT_KeyCrypt.h"

#define IDS_ERR_DRIVERVERSION "\274\263\304\241\265\310 \265\345\266\363\300\314\271\366 \271\366\300\374\300\314 \270\302\301\366 \276\312\275\300\264\317\264\331.\n\n\303\326\275\305 \276\367\265\245\300\314\306\256\270\246 \271\336\300\270\275\305 \310\304 \304\304\307\273\305\315\270\246 \300\347 \275\303\300\333 \307\330 \301\326\275\312\275\303\277\344."

#define IDS_ERR_VERIFYVERSION "\307\301\267\316\261\327\267\245\277\241 \310\243\310\257\274\272 \270\360\265\345\260\241 \303\274\305\251\265\307\276\356 \300\326\275\300\264\317\264\331. \303\274\305\251\270\246 \307\330\301\246\307\317\275\305 \310\304 \264\331\275\303 \275\307\307\340\307\330 \301\326\275\312\275\303\277\344."

#define IDS_ERR_LOADFAIL "\265\345\266\363\300\314\271\366\260\241 \303\312\261\342\310\255 \265\307\301\366 \276\312\276\322\275\300\264\317\264\331.\n\n\303\312\261\342\310\255 \271\366\306\260\300\273 \264\255\267\257\274\255 \265\345\266\363\300\314\271\366\270\246 \303\312\261\342\310\255 \307\330 \301\326\275\312\275\303\277\344."

CKeyCrypt::CKeyCrypt(void)
{
}

CKeyCrypt::~CKeyCrypt(void)
{
}

BOOL CKeyCrypt::OnLoadKeyCrypt()
{
	// TODO: Add your control notification handler code here
	char szFileName[MAX_PATH] = {0,};
	char szGameFileName[MAX_PATH] = {0,};

	BOOL	bStatus = FALSE;

	// 실 적용시 game Client 실행파일의 Full Path.
	char szSysPath[MAX_PATH];
	if( GetCurrentDirectory(sizeof(szSysPath), (char*)szSysPath) == 0 ) 
		return FALSE;

	wsprintf((char*)szGameFileName, "%s\\game.exe", szSysPath);

	//MessageBox(NULL,szGameFileName,"",MB_OK);

	if(NPKGetAppCompatFlag(szGameFileName) != apcfNone)
    {
		//OutputDebugString("--> ReStart Program\n");

		char	strErrMsg[256] = {0,};

		NPKSetAppCompatFlag(szGameFileName, apcfNone);
		
		//wsprintf(strErrMsg, "%s Program 종료후 재 실행 해주세요.!!\n", szGameFileName);
		//::MessageBox(NULL, strErrMsg, "nProtect KeyCrypt", MB_OK | MB_ICONERROR);
		PostQuitMessage(0);
		return FALSE;
    }
    else
	{
		//OutputDebugString("--> Load KeyCrypt\n");

		bStatus = NPKGetLoadStartup();

		GetModuleFileName(NULL, szFileName, sizeof szFileName);
		char *ptr = strrchr(szFileName, '\\');
		if(ptr != NULL)
			*ptr = 0;

		NPKSetDrvPath(szFileName);
		m_hKCrypt = NPKOpenDriver();

		if(m_hKCrypt <= 0)
		{
			HLOCAL	hlocal = NULL;
			DWORD	dwError = GetLastError();
			char	strErrMsg[256] = {0,};
			
			BOOL fOk = ::FormatMessage(FORMAT_MESSAGE_FROM_SYSTEM | FORMAT_MESSAGE_ALLOCATE_BUFFER, 
				NULL, 
				dwError, 
				MAKELANGID(LANG_KOREAN, SUBLANG_ENGLISH_US), 
				(PTSTR) &hlocal, 
				0, 
				NULL);
			
			if(fOk)
			{
				//strcpy(strErrMsg,(PCTSTR) LocalLock(hlocal));
				::LocalFree(hlocal);
			}
			else
			{
				switch(dwError)
				{
				case NPK_ERROR_NOTADMIN:
					break;

				case NPK_ERROR_DRIVERVERSION:
					strcpy(strErrMsg,IDS_ERR_DRIVERVERSION);
					break;

				case NPK_ERROR_VERIFYVERSION:
					strcpy(strErrMsg,IDS_ERR_VERIFYVERSION);
					break;
				}
			}

			//::MessageBox(NULL, strErrMsg, "nProtect KeyCrypt", MB_OK | MB_ICONERROR);
			PostQuitMessage(0);
			return FALSE;
		} else {

			NPKLoadAtStartup(TRUE);
		}
	}

	return TRUE;
}

BOOL CKeyCrypt::OnExitKeyCrypt()
{
	// TODO: Add your control notification handler code here
	if(m_hKCrypt > 0)
		NPKCloseDriver(m_hKCrypt);
	else
	{
		/*char	strErrMsg[256] = {0,};
		strcpy(strErrMsg,IDS_ERR_LOADFAIL);
		::MessageBox(NULL, strErrMsg, "nProtect KeyCrypt", MB_OK | MB_ICONERROR);*/
		return FALSE;
	}
	return TRUE;
}
BOOL CKeyCrypt::OnRegisterwindow(HWND hEdit)
{
	// TODO: Add your control notification handler code here
	if(m_hKCrypt > 0)
		NPKRegCryptMsg(m_hKCrypt, hEdit, WM_USER + 1094);
	else
	{
		/*char	strErrMsg[256] = {0,};
		strcpy(strErrMsg,IDS_ERR_LOADFAIL);
		::MessageBox(NULL, strErrMsg, "nProtect KeyCrypt", MB_OK | MB_ICONERROR);*/
		return FALSE;
	}
	return TRUE;
}