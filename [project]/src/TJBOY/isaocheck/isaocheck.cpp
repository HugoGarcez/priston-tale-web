//-----------------------------------------------------------------------
//
//  isao의 서버에 ssl로 체크.
//
//-----------------------------------------------------------------------
#define __ISAOCHECK_CPP__
#include <windows.h>
#include <stdlib.h>
#include <stdio.h>
#include <Winsock.h>
#include <wininet.h>
#include <time.h>

#include "../clanmenu/tjclanDEF.h"
#include "../../language.h"

#ifdef USE_PROSTONTALE		//=========== 프리스턴소스인경우
	#include "..\\..\\sinbaram\\SinLinkHeader.h"
	#include "..\\..\\HoBaram\\HoLinkHeader.h"
	#include "..\\..\\language.h"
	#include "..\\..\\HoBaram\\HoTextFile.h"
	#include "..\\clanmenu\\cE_CViewClanInfo.h"
#endif


#include "..\\clanmenu\\BMPsmooth.h"
#include "..\\clanmenu\\tjclan.h"
#include "..\\clanmenu\\cE_CViewClanInfo.h"
//#include "..\\clanmenu\\Zip.h"
#include <wininet.h>
#pragma comment(lib,"wininet.lib")


//ktj : 원래 쓰레드로 사용해야 윈속 블럭킹이 안됨에 주의할것.
#include "isaocheck.h"

extern void fd( char *fmt, ...);
extern void fd2( char *fmt, ...);

#define BUF_LEN 4096

extern char szConnServerName[16];
extern int  nClanTicket; //ticket번호 (개발용 임시 티켓번호 9999)

extern BOOL	g_bDebugOption;
void debug_fd2( char *fmt, ...)
{
	if(!g_bDebugOption) return;

	char buff[4096];
	
    va_list args;

    va_start( args, fmt );
    vsprintf( buff, fmt, args );
    va_end( args );

    strcat( buff, "\r\n" );


	int errorMSGdr=0;
	if(strlen(buff)>2048 ) {
		buff[1024] = 0;
		errorMSGdr= 1;




		HANDLE hFile;
		DWORD dwAcess =0;
		hFile = CreateFile( "DDD2.txt", GENERIC_WRITE, FILE_SHARE_READ|FILE_SHARE_WRITE,
							NULL, OPEN_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL );

		if( hFile != INVALID_HANDLE_VALUE ) {
			SetFilePointer( hFile , 0 , NULL , FILE_END );

			//데이타 쓰기     버퍼    쓸크기    실제로 쓰여진 바이트수
			int chk = WriteFile( hFile , buff , lstrlen(buff) , &dwAcess , NULL );


			//데이타 쓰기     버퍼    쓸크기    실제로 쓰여진 바이트수
			if(errorMSGdr== 1) {
				char msg[256] = "..\n\270\336\275\303\301\366\260\241 \263\312\271\253\261\346\276\356\274\255 \300\337\266\372\300\275";
				WriteFile( hFile , msg, lstrlen(msg) , &dwAcess , NULL );
			}
			

			//파일 닫기
			CloseHandle( hFile );
		}




		return;
		
	}

	HANDLE hFile;
    DWORD dwAcess =0;
    hFile = CreateFile( "DDD2.txt", GENERIC_WRITE, FILE_SHARE_READ|FILE_SHARE_WRITE,
                        NULL, OPEN_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL );

    if( hFile != INVALID_HANDLE_VALUE ) {
        SetFilePointer( hFile , 0 , NULL , FILE_END );
        //데이타 쓰기     버퍼    쓸크기    실제로 쓰여진 바이트수
        int chk = WriteFile( hFile , buff , lstrlen(buff) , &dwAcess , NULL );


		if(errorMSGdr== 1) {
			char msg[256] = "\n\270\336\275\303\301\366\260\241 \263\312\271\253\261\346\276\356\274\255 \300\337\266\372\300\275";
			WriteFile( hFile , msg, lstrlen(msg) , &dwAcess , NULL );
		}
		

        //파일 닫기
        CloseHandle( hFile );
    }
}










BOOL GetHttpRequest(LPSTR httpname,LPSTR Request,DWORD MaxBuffer,int *len)
{
	char szSizeBuffer[32];
    DWORD dwLengthSizeBuffer = sizeof(szSizeBuffer);
    DWORD dwFileSize = 0;
    DWORD dwBytesRead = 0;
    BOOL bSuccessful = FALSE;
    //int i;
 
    HINTERNET m_Session = NULL;
    m_Session = InternetOpen("AppName",  //어플리케이션의 이름
							INTERNET_OPEN_TYPE_PRECONFIG,  
							// Use registry settings.
                            NULL,
							// Proxy name. NULL indicates use default.
                            NULL,  
							// List of local servers. NULL indicates default.
                            0) ;
 	HINTERNET hHttpFile = NULL;
    hHttpFile = InternetOpenUrl(m_Session, (const char *) httpname, NULL,
								0, 0, 0);
    if (hHttpFile)
    {
		BOOL bQuery = HttpQueryInfo(hHttpFile,HTTP_QUERY_CONTENT_LENGTH, szSizeBuffer,
					    &dwLengthSizeBuffer, NULL) ;
	    //BOOL bQuery = InternetQueryDataAvailable(hHttpFile,&dwLengthSizeBuffer,0,0);
        if(bQuery==TRUE)
        {
			dwFileSize=atol(szSizeBuffer);                                                          
            dwFileSize > MaxBuffer ? dwFileSize = MaxBuffer :
			dwFileSize = dwFileSize;
 
            BOOL bRead = InternetReadFile(hHttpFile, Request,dwFileSize, &dwBytesRead);
            if (bRead)
            {
				bSuccessful = TRUE;
				*len = dwBytesRead;
		
				//i = lstrlen(Request);
            }
            InternetCloseHandle(hHttpFile);
		}
	}
    if(hHttpFile) InternetCloseHandle(hHttpFile);
    if(m_Session) InternetCloseHandle(m_Session);
    return bSuccessful;
 }            




/*///////////////////////////////////////////////////////////////////////////
: 클라스 명: class ISAOCHECK
: 하 는 일 : 웹db를 이용 데이타를 가져온다.
: 서    식 :
: 인    자 :
: 반 환 값 :
: 논    평 : 웹db를 사용해 데이타를 뽑아내야 할경우 본 클라스를 이용한다.
: 버    전 :
: PROJECT  :
///////////////////////////////////////////////////////////////////////////*/
ISAOCHECK::ISAOCHECK()
{

	HSsu = 0;						//헥사스트링의 갯수
	for(int i=0; i<30; i++)
		fileDATA_HS[i]= NULL;		//총 2048binary의 데이타가 4096 hexa string으로 바뀌어들어감.
}

ISAOCHECK::~ISAOCHECK()
{
}

void ISAOCHECK::init()
{
	iCheckFlag = 0;
	hostPORT = 0;
}

void ISAOCHECK::init_HostName_Port(char *hostname, int port )
{
	if(hostname==NULL) return;

	hostPORT = port;
	ZeroMemory(hostName, sizeof(char)*64 );
//#ifdef __CLANSERVER__
	//strcpy(hostName, "211.44.231.10");
//#else
	strcpy(hostName, hostname);
///#endif
}

void ISAOCHECK::init_HostName_Port1(char *hostname, int port )
{
	if(hostname==NULL) return;

	hostPORT1 = port;
	ZeroMemory(hostName1, sizeof(char)*64 );
	strcpy(hostName1, hostname);
}

void ISAOCHECK::init(int iCheckON, int iTimeOUT )
{
	iCheckFlag = iCheckON;	//config.isaocheck에서 첵크됨
	iTimeOut = iTimeOUT;
}

void ISAOCHECK::close()
{
}


void ISAOCHECK::main()
{

}



/*///////////////////////////////////////////////////////////////////////////
: 함 수 명 :  int connect_WEBdb( char *szWriteBuffer, char *szReadBuffer , long StartTime )
: 하 는 일 :  웹db에 접속해 데이타를 구하는 함수
: 서    식 :
: 인    자 :
: 반 환 값 :
: 논    평 :  이 클라스의 핵심 함수임.
: 버    전 :
: PROJECT  :
///////////////////////////////////////////////////////////////////////////*/

int ISAOCHECK::connect_WEBdb( char *szWriteBuffer, char *szReadBuffer)
{
	char szSizeBuffer[32];
    DWORD dwLengthSizeBuffer = sizeof(szSizeBuffer);
    DWORD dwFileSize = 0;
    DWORD dwBytesRead = 0;
    
    int iRet= ISAO_ERR_WINSOCK_NOERR;

    ZeroMemory(szReadBuffer,BUF_LEN);
    //int i;
 
    HINTERNET m_Session = NULL;
    
    m_Session = InternetOpen("AppName",  //어플리케이션의 이름
							INTERNET_OPEN_TYPE_PRECONFIG,  
							// Use registry settings.
                            NULL,
							// Proxy name. NULL indicates use default.
                            NULL,  
							// List of local servers. NULL indicates default.
                            0) ;
    if(!m_Session) 
    {
	    iRet = ISAO_ERR_TCP_CONNECT;
	    return iRet;
    }
 	HINTERNET hHttpFile = NULL;
    hHttpFile = InternetOpenUrl(m_Session, (const char *) szWriteBuffer, NULL,
								0, 0, 0);
    if (hHttpFile)
    {
		BOOL bQuery = HttpQueryInfo(hHttpFile,HTTP_QUERY_CONTENT_LENGTH, szSizeBuffer,
					    &dwLengthSizeBuffer, NULL) ;
	    //BOOL bQuery = InternetQueryDataAvailable(hHttpFile,&dwLengthSizeBuffer,0,0);
        if(bQuery==TRUE)
        {
			dwFileSize=atol(szSizeBuffer);                                                          
            dwFileSize > BUF_LEN ? dwFileSize = BUF_LEN :
			dwFileSize = dwFileSize;
 
            BOOL bRead = InternetReadFile(hHttpFile, szReadBuffer,dwFileSize, &dwBytesRead);
            if (bRead)
            {
				
				if((bRead == TRUE)&&(dwFileSize !=dwBytesRead))
				{			
					iRet = ISAO_ERR_RECONNECT;
				}
		
				
            }
		   else iRet = ISAO_ERR_RECONNECT;
            InternetCloseHandle(hHttpFile);
		}
	}
    else
    {
	    iRet = ISAO_ERR_TCP_CONNECT;
    }
    if(hHttpFile) InternetCloseHandle(hHttpFile);
    if(m_Session) InternetCloseHandle(m_Session);
    return iRet;
}

int ISAOCHECK::connect_WEBdb_1( char *szWriteBuffer, char *szReadBuffer)
{
	char szSizeBuffer[32];
    DWORD dwLengthSizeBuffer = sizeof(szSizeBuffer);
    DWORD dwFileSize = 0;
    DWORD dwBytesRead = 0;    
    int iRet= ISAO_ERR_WINSOCK_NOERR;

    ZeroMemory(szReadBuffer,BUF_LEN);
    //int i;
 
    HINTERNET m_Session = NULL;
    m_Session = InternetOpen("AppName",  //어플리케이션의 이름
							INTERNET_OPEN_TYPE_PRECONFIG,  
							// Use registry settings.
                            NULL,
							// Proxy name. NULL indicates use default.
                            NULL,  
							// List of local servers. NULL indicates default.
                            0) ;
    if(!m_Session) 
    {
	    iRet = ISAO_ERR_TCP_CONNECT;
	    return iRet;
    }
 	HINTERNET hHttpFile = NULL;
    hHttpFile = InternetOpenUrl(m_Session, (const char *) szWriteBuffer, NULL,
								0, 0, 0);
    if (hHttpFile)
    {
		BOOL bQuery = HttpQueryInfo(hHttpFile,HTTP_QUERY_CONTENT_LENGTH, szSizeBuffer,
					    &dwLengthSizeBuffer, NULL) ;
	    //BOOL bQuery = InternetQueryDataAvailable(hHttpFile,&dwLengthSizeBuffer,0,0);
        if(bQuery==TRUE)
        {
			dwFileSize=atol(szSizeBuffer);                                                          
            dwFileSize > BUF_LEN ? dwFileSize = BUF_LEN :
			dwFileSize = dwFileSize;
 
            BOOL bRead = InternetReadFile(hHttpFile, szReadBuffer,dwFileSize, &dwBytesRead);
            if (bRead)
            {
				if((bRead == TRUE)&&(dwFileSize !=dwBytesRead))
				{			
					iRet = ISAO_ERR_RECONNECT;
				}		
				
            }
		   else iRet = ISAO_ERR_RECONNECT;
            InternetCloseHandle(hHttpFile);
		}
	}
    else
    {
	    iRet = ISAO_ERR_TCP_CONNECT;
	    return iRet;
    }
    if(hHttpFile) InternetCloseHandle(hHttpFile);
    if(m_Session) InternetCloseHandle(m_Session);
    return iRet;
}
/*
int ISAOCHECK::connect_WEBdb( char *szWriteBuffer, char *szReadBuffer)
{
	struct timeval tmv;    //timeval
	struct sockaddr_in server;
	char Value = '\0';
	char Msg[1024] = "";
	SOCKET Fd;
	
	int iLen;
	int iRet= ISAO_ERR_WINSOCK_NOERR;
	int iSelectRet;
	
    fd_set fdset;

	//-----------------------------------------------
	// 보통 사용되는 TCP 소켓 작성
	//-----------------------------------------------
	WORD         wSocketVersion;
	WSADATA      wsaData;
    int          iError;

	wSocketVersion = MAKEWORD(1,1);
	iError = WSAStartup(wSocketVersion,&wsaData);
	if(iError != 0)
	{
		return ISAO_ERR_WINSOCK_START;
	}
	if(HIBYTE(wsaData.wVersion) != 1 || LOBYTE(wsaData.wVersion) != 1)
	{
		WSACleanup();
		return ISAO_ERR_WINSOCK_VERSION;
	}	

	Fd = socket (AF_INET,SOCK_STREAM,0);
	memset(&server,0, sizeof(server));
	server.sin_family = AF_INET;
	server.sin_port = htons(hostPORT );					//클라스내에서 셋팅 
	server.sin_addr.s_addr = inet_addr(hostName);		//hostPORT hostName

	//ygy : 수정 부분
	//struct hostent* ClanHostName = gethostbyname(hostName);
	//memcpy(&server.sin_addr.s_addr , ClanHostName->h_addr_list[0],ClanHostName->h_length);
	//end ygy
	//connect
	if (connect( Fd, (struct sockaddr *)&server,sizeof(server)) == -1){
		return ISAO_ERR_TCP_CONNECT;
	}


   	
	//ktj : 임시
	//fd2( "send( %s )", szWriteBuffer  );
	send(Fd,szWriteBuffer,strlen(szWriteBuffer)+1,0);

	tmv.tv_sec = iTimeOut;		// 보낸후 대기시간만큼만 답변을 기다림
	tmv.tv_usec = 0;
	FD_ZERO( &fdset );			// read select
    FD_SET( Fd, &fdset );		// fd의 플래그 셋팅
	iSelectRet = select( Fd + 1, &fdset, (fd_set*)NULL,(fd_set*)NULL,&tmv);		// 답변을 받는동안 계속 대기함.

	if( iSelectRet > 0 && FD_ISSET( Fd, &fdset ) ){		// 성공했다

	}else{												// 실패했다
		iRet = ISAO_ERR_SSL_TIMEOUT;
		goto IsaoCheck_Free;
	}




	ZeroMemory(szReadBuffer,sizeof(szReadBuffer));
	long startTime, endTime ;
	time( &startTime  );

	int rcnt=0;
	while(1) {
		iLen = recv(Fd,szReadBuffer,BUF_LEN-1,0);		//실제 읽는다.
		//iLen = recv(Fd,szReadBuffer,20,0);		//실제 읽는다.
		
		if(iLen==0) {
			Sleep(100);
		}
		else if(iLen == SOCKET_ERROR)				// error
		{
	        int error = WSAGetLastError();
			if(error==10054)    {
				iRet = ISAO_ERR_SSL_TIMEOUT;
				goto IsaoCheck_Free;
			}
		}
		else {					//데이타받았으므로 끝낸다.
			break;
		}

		time( &endTime);
		if( (endTime-startTime) > 2) {		//2초정도기다려서 답이안오면 리컨넥하도록함.
			iRet = ISAO_ERR_RECONNECT;
			goto IsaoCheck_Free;
		}
	}


	if( iLen < 0 ){
		iRet = ISAO_ERR_SSL_READ;				// 읽을 수 없었다．실패이다．

		
		goto IsaoCheck_Free;
	}
	szReadBuffer[iLen] = 0;
	
	//디버그용으로 표시함.
	//fd2( "\nFrom server: ---from--\n%s--end--\n 읽은 길이는 %d ",szReadBuffer, iLen );
	
IsaoCheck_Free:

	closesocket( Fd );
	fd2("\n 웹db 접근 끝남." );

	return iRet;
}


//ygy
int ISAOCHECK::connect_WEBdb_1( char *szWriteBuffer, char *szReadBuffer)
{
	struct timeval tmv;    //timeval
	struct sockaddr_in server;
	char Value = '\0';
	char Msg[1024] = "";
	SOCKET Fd;
	
	int iLen;
	int iRet= ISAO_ERR_WINSOCK_NOERR;
	int iSelectRet;
	
    fd_set fdset;

	//-----------------------------------------------
	// 보통 사용되는 TCP 소켓 작성
	//-----------------------------------------------
	WORD         wSocketVersion;
	WSADATA      wsaData;
    int          iError;

	wSocketVersion = MAKEWORD(1,1);
	iError = WSAStartup(wSocketVersion,&wsaData);
	if(iError != 0)
	{
		return ISAO_ERR_WINSOCK_START;
	}
	if(HIBYTE(wsaData.wVersion) != 1 || LOBYTE(wsaData.wVersion) != 1)
	{
		WSACleanup();
		return ISAO_ERR_WINSOCK_VERSION;
	}	

	Fd = socket (AF_INET,SOCK_STREAM,0);
	memset(&server,0, sizeof(server));
	server.sin_family = AF_INET;
	server.sin_port = htons(hostPORT1 );
	server.sin_addr.s_addr = inet_addr(hostName1);		//hostPORT hostName//클라스내에서 셋팅 	

	//ygy : 수정 부분
	//struct hostent* ClanHostName = gethostbyname(hostName1);
	//memcpy(&server.sin_addr.s_addr , ClanHostName->h_addr_list[0],ClanHostName->h_length);
	//ygy

	//connect
	if (connect( Fd, (struct sockaddr *)&server,sizeof(server)) == -1){
		return ISAO_ERR_TCP_CONNECT;
	}


   	
	//ktj : 임시
	//fd2( "send( %s )", szWriteBuffer  );
	send(Fd,szWriteBuffer,strlen(szWriteBuffer)+1,0);

	tmv.tv_sec = iTimeOut;		// 보낸후 대기시간만큼만 답변을 기다림
	tmv.tv_usec = 0;
	FD_ZERO( &fdset );			// read select
    FD_SET( Fd, &fdset );		// fd의 플래그 셋팅
	iSelectRet = select( Fd + 1, &fdset, (fd_set*)NULL,(fd_set*)NULL,&tmv);		// 답변을 받는동안 계속 대기함.

	if( iSelectRet > 0 && FD_ISSET( Fd, &fdset ) ){		// 성공했다

	}else{												// 실패했다
		iRet = ISAO_ERR_SSL_TIMEOUT;
		goto IsaoCheck_Free;
	}


	


	long startTime, endTime ;
	time( &startTime  );

	int rcnt=0;
	while(1) {
		iLen = recv(Fd,szReadBuffer,BUF_LEN,0);		//실제 읽는다.
		//iLen = recv(Fd,szReadBuffer,20,0);		//실제 읽는다.
		
		if(iLen==0) {
			Sleep(100);
		}
		else if(iLen == SOCKET_ERROR)				// error
		{
	        int error = WSAGetLastError();
			if(error==10054)    {
				iRet = ISAO_ERR_SSL_TIMEOUT;
				goto IsaoCheck_Free;
			}
		}
		else {					//데이타받았으므로 끝낸다.
			break;
		}

		time( &endTime);
		if( (endTime-startTime) > 2) {		//2초정도기다려서 답이안오면 리컨넥하도록함.
			iRet = ISAO_ERR_RECONNECT;
			goto IsaoCheck_Free;
		}
	}


	if( iLen < 0 ){
		iRet = ISAO_ERR_SSL_READ;				// 읽을 수 없었다．실패이다．

		
		goto IsaoCheck_Free;
	}
	szReadBuffer[iLen] = 0;
	
	//디버그용으로 표시함.
	//fd2( "\nFrom server: ---from--\n%s--end--\n 읽은 길이는 %d ",szReadBuffer, iLen );
	
IsaoCheck_Free:

	closesocket( Fd );
	fd2("\n 웹db 접근 끝남." );

	return iRet;
}
*/


//binary데이타를 읽는경우에 사용함.(파일등..)
int ISAOCHECK::connect_WEBdb_BINARY( char *szWriteBuffer, char *szReadBuffer, int *dataSize)
{
	struct timeval tmv;    //timeval
	struct sockaddr_in server;
	char Value = '\0';
	char Msg[1024] = "";
	SOCKET Fd;
	
	int iLen;
	int iRet= ISAO_ERR_WINSOCK_NOERR;
	int iSelectRet;
	
    fd_set fdset;
		
	//-----------------------------------------------
	// 보통 사용되는 TCP 소켓 작성
	//-----------------------------------------------
	WORD         wSocketVersion;
	WSADATA      wsaData;
    int          iError;
    
	wSocketVersion = MAKEWORD(1,1);
	iError = WSAStartup(wSocketVersion,&wsaData);
	if(iError != 0)
	{
		return ISAO_ERR_WINSOCK_START;
	}
	if(HIBYTE(wsaData.wVersion) != 1 || LOBYTE(wsaData.wVersion) != 1)
	{
		WSACleanup();
		return ISAO_ERR_WINSOCK_VERSION;
	}	

	Fd = socket (AF_INET,SOCK_STREAM,0);
	memset(&server,0, sizeof(server));
	server.sin_family = AF_INET;
	server.sin_port = htons(hostPORT1 );					//클라스내에서 셋팅 
	//server.sin_addr.s_addr = inet_addr(hostName);		//hostPORT hostName
	//server.sin_addr.s_addr = inet_addr(hostName1);		//hostPORT hostName
	//ygy
	struct hostent* ClanHostName = gethostbyname(hostName1);
	memcpy(&server.sin_addr.s_addr , ClanHostName->h_addr_list[0],ClanHostName->h_length);
	//ygy
	//connect
	if (connect( Fd, (struct sockaddr *)&server,sizeof(server)) == -1){
		return ISAO_ERR_TCP_CONNECT;
	}
   	
	//ktj : 임시
	//fd2( "send( %s )", szWriteBuffer  );
	send(Fd,szWriteBuffer,strlen(szWriteBuffer)+1,0);

	tmv.tv_sec = iTimeOut;		// 보낸후 대기시간만큼만 답변을 기다림
	tmv.tv_usec = 0;
	FD_ZERO( &fdset );			// read select
    FD_SET( Fd, &fdset );		// fd의 플래그 셋팅
	iSelectRet = select( Fd + 1, &fdset, (fd_set*)NULL,(fd_set*)NULL,&tmv);		// 답변을 받는동안 계속 대기함.

	if( iSelectRet > 0 && FD_ISSET( Fd, &fdset ) ){		// 성공했다

	}else{												// 실패했다
		iRet = ISAO_ERR_SSL_TIMEOUT;
		goto IsaoCheck_Free;
	}

	//ktj : 고침

	
	/*
	int rcnt; rcnt=0;		// no initializer: the goto above jumps past this (standard C++)
	while(1) {
		iLen = recv(Fd,&szReadBuffer[rcnt],BUF_LEN,0);		//실제 읽는다.
		rcnt += iLen;
		if(iLen==0) break;
	}
	*/

	long startTime, endTime ;
	time( &startTime  );

	
	int rcnt; rcnt=0;		// no initializer: the goto above jumps past this (standard C++)

	/*
	//ktj : 원본
	while(1) {
		iLen = recv(Fd,szReadBuffer,BUF_LEN,0);		//실제 읽는다.

		rcnt+=iLen;
		//iLen = recv(Fd,szReadBuffer,20,0);		//실제 읽는다.
		
		if(iLen==0) {
			Sleep(100);
		}
		else if(iLen == SOCKET_ERROR)				// error
		{
	        int error = WSAGetLastError();
			if(error==10054)    {
				iRet = ISAO_ERR_SSL_TIMEOUT;
				goto IsaoCheck_Free;
			}
		}
		else {					//데이타받았으므로 끝낸다.
			if(rcnt >= 3128) break;
		}

		
		time( &endTime);
		if( (endTime-startTime) > 2) {		//2초정도기다려서 답이안오면 리컨넥하도록함.
			iRet = ISAO_ERR_RECONNECT;
			goto IsaoCheck_Free;
		}
	}

	*/

	
	while(1) {
		iLen = recv(Fd,&szReadBuffer[rcnt],100,0);		//실제 읽는다.
		rcnt+=iLen;
		//iLen = recv(Fd,szReadBuffer,20,0);		//실제 읽는다.
		if(iLen==0) {
			//Sleep(100);
			if(rcnt >= 3126)		//데이타받았으므로 끝낸다.
				break;
		}
		else if(iLen == SOCKET_ERROR)				// error
		{
	        int error = WSAGetLastError();
			if(error==10054)    {
				iRet = ISAO_ERR_SSL_TIMEOUT;
				goto IsaoCheck_Free;
			}
		}
		else 
		{					

		}

		time( &endTime);
		if( (endTime-startTime) > 10) {		//10초정도기다려서 답이안오면 리컨넥하도록함.
			iRet = ISAO_ERR_RECONNECT;
			goto IsaoCheck_Free;
		}
	}







	*dataSize = rcnt ;
	

	if( iLen < 0 ){
		iRet = ISAO_ERR_SSL_READ;				// 읽을 수 없었다．실패이다．
		
		goto IsaoCheck_Free;
	}
	//szReadBuffer[iLen] = 0;
	
	//디버그용으로 표시함.
	//fd2( "\nFrom server: ---from--\n%s--end--\n 읽은 길이는 %d ",szReadBuffer, iLen );
	
IsaoCheck_Free:

	closesocket( Fd );
	fd2("\n \300\245db \301\242\261\331 \263\241\263\262." );

	return iRet;
}



/*
if( (i = send(iSock, (char*)data, l, 0)) == SOCKET_ERROR)
    //if( (i = send(iSock, (char*)cmd.data, cmd.dataLen, 0)) == SOCKET_ERROR)
    {
        error = WSAGetLastError();
        if(error==10054) {
			//ktj
            //fd(" sW Error %d: cmd.dataLen %d ret %d  closeCLIENT", error, cmd.dataLen);
            closeCLIENT();   //일단 닫음
        }
        if(error == 10035) {
            fdWrite = 0;            //쓰기 금지한다.
        }
        return 0;
    }



//binary데이타를 읽는경우에 사용함.(파일등..)
int ISAOCHECK::connect_WEBdb_BINARY( char *szWriteBuffer, char *szReadBuffer, int *dataSize)
{
	struct timeval tmv;    //timeval
	struct sockaddr_in server;
	char Value = '\0';
	char Msg[1024] = "";
	SOCKET Fd;
	
	int iLen;
	int iRet= ISAO_ERR_WINSOCK_NOERR;
	int iSelectRet;
	
    fd_set fdset;
		
	//-----------------------------------------------
	// 보통 사용되는 TCP 소켓 작성
	//-----------------------------------------------
	WORD         wSocketVersion;
	WSADATA      wsaData;
    int          iError;
    
	wSocketVersion = MAKEWORD(1,1);
	iError = WSAStartup(wSocketVersion,&wsaData);
	if(iError != 0)
	{
		return ISAO_ERR_WINSOCK_START;
	}
	if(HIBYTE(wsaData.wVersion) != 1 || LOBYTE(wsaData.wVersion) != 1)
	{
		WSACleanup();
		return ISAO_ERR_WINSOCK_VERSION;
	}	

	Fd = socket (AF_INET,SOCK_STREAM,0);
	memset(&server,0, sizeof(server));
	server.sin_family = AF_INET;
	server.sin_port = htons(hostPORT );					//클라스내에서 셋팅 
	server.sin_addr.s_addr = inet_addr(hostName);		//hostPORT hostName
	//connect
	if (connect( Fd, (struct sockaddr *)&server,sizeof(server)) == -1){
		return ISAO_ERR_TCP_CONNECT;
	}
   	
	//ktj : 임시
	//fd2( "send( %s )", szWriteBuffer  );
	send(Fd,szWriteBuffer,strlen(szWriteBuffer)+1,0);

	tmv.tv_sec = iTimeOut;		// 보낸후 대기시간만큼만 답변을 기다림
	tmv.tv_usec = 0;
	FD_ZERO( &fdset );			// read select
    FD_SET( Fd, &fdset );		// fd의 플래그 셋팅
	iSelectRet = select( Fd + 1, &fdset, (fd_set*)NULL,(fd_set*)NULL,&tmv);		// 답변을 받는동안 계속 대기함.

	if( iSelectRet > 0 && FD_ISSET( Fd, &fdset ) ){		// 성공했다

	}else{												// 실패했다
		iRet = ISAO_ERR_SSL_TIMEOUT;
		goto IsaoCheck_Free;
	}

	//ktj : 고침
	
	int rcnt=0;
	while(1) {
		iLen = recv(Fd,&szReadBuffer[rcnt],BUF_LEN,0);		//실제 읽는다.
		rcnt += iLen;
		if(iLen==0) break;
	}
	*dataSize = rcnt ;

	if( iLen < 0 ){
		iRet = ISAO_ERR_SSL_READ;				// 읽을 수 없었다．실패이다．
		
		goto IsaoCheck_Free;
	}
	//szReadBuffer[iLen] = 0;
	
	//디버그용으로 표시함.
	//fd2( "\nFrom server: ---from--\n%s--end--\n 읽은 길이는 %d ",szReadBuffer, iLen );
	
IsaoCheck_Free:

	closesocket( Fd );
	fd2("\n 웹db 접근 끝남." );

	return iRet;
}
*/




//웹db에서 되돌아온스트링에서 int형 데이타를 뽑아냄.
int ISAOCHECK::chk_readbuffer(char *cmd, char *readbuf, int *iRejectC )
{
	int iRejectCode = 111111;			//1은 제대로 들어온것임.
	char *pBuf;

	//pBuf = strstr(readbuf,"account=");
	pBuf = strstr(readbuf,cmd);
    if( pBuf == NULL)	//발견할 수 없었다.
	{
		return ISAO_ERR_NONE_REEJCT;
	}

	//pBuf = pBuf + strlen( "account:" );
	pBuf = pBuf + strlen( cmd );
	while(1) {			//스페이스를 없앤다.
		if(*pBuf != ' ') break;
		pBuf ++;
	}
	sscanf( pBuf , "%d", &iRejectCode );

	return iRejectCode;
}


//웹 db에서 온 스트링에서 원하는 스트링을 뽑아냄.
int ISAOCHECK::chk_readStrBuf(char *cmd, char *readbuf, char *retBuf)
{
	int iRejectCode = 111111;			//1은 제대로 들어온것임.
	char *pBuf;

	//pBuf = strstr(readbuf,"account=");
	pBuf = strstr(readbuf,cmd);
    if( pBuf == NULL)	//발견할 수 없었다.
	{
		return ISAO_ERR_NONE_REEJCT;
	}

	//pBuf = pBuf + strlen( "account:" );
	pBuf = pBuf + strlen( cmd );
	while(1) {			//스페이스를 없앤다.
		if(*pBuf != ' ') break;
		pBuf ++;
	}
	sscanf( pBuf , "%s", retBuf );

	return iRejectCode;
}









/*///////////////////////////////////////////////////////////////////////////
: 함 수 명 :  int IsaoCheck()
: 하 는 일 :  id와 pwd의 유효여부를 알아내는 함수로
: 서    식 :  유저가 겜서버에 최초접속시 id,pwd가있는 웹db서버에 들어가 알아낸다.
: 인    자 :  
: 반 환 값 :
: 논    평 :  실제 사용함수 
: 버    전 :
: PROJECT  :
///////////////////////////////////////////////////////////////////////////*/
int ISAOCHECK::IsaoCheck( char *id, char *pass, char *ipaddr, long StartTime )
{
	int ret_chk;
	int iRejectCode = 111111;
	char szWriteBuffer[BUF_LEN];
	char szReadBuffer [BUF_LEN];
	
	
	sprintf( szWriteBuffer,
		//"POST http://%s/login.php?UserID=%s&PassWord=%s\r\n\r\n\r\n", hostName, id,pass );
		"http://%s/login.php?UserID=%s&PassWord=%s", hostName, id,pass );
	ret_chk = connect_WEBdb( szWriteBuffer, szReadBuffer);
	
	//ktj : 임시
	fd2(" \300\245 db\277\241\274\255 \300\320\300\272\260\315 %s ", szReadBuffer);

	
	//ktj : 아래 첵크는 모두 임시적으로 운영됨

	//winsock 상의 에러 첵크함.  ================================== start
	switch(ret_chk) {
	case ISAO_ERR_WINSOCK_NOERR:		//에러 없음.
		//ktj : 임시로 서버에서 온 데이타를 뿌려봄.
		fd2( "From server: ---from--\n%s--end--\n ",szReadBuffer);			
		break;
	case ISAO_ERR_WINSOCK_START:		//윈속 START시 에러발생				//window only
		fd2("WSAStartup()\277\241\266\363");
		goto IsaoCheck_winsockERR;
	case ISAO_ERR_WINSOCK_VERSION:		//윈속버전이 틀려 구동되지못함.			//window only
		fd2("\271\366\300\374\300\314 \270\302\301\366 \276\312\300\275");
		goto IsaoCheck_winsockERR;
	case ISAO_ERR_TCP_CONNECT:			// 보통 사용되는 ＴＣＰ코네쿠토에라
		fd2( "connect() \301\242\274\323 \275\307\306\320" );
		goto IsaoCheck_winsockERR;
	case ISAO_ERR_SSL_READ:				// ＳＳＬ 리드 할 수 없었다．
		fd2(" recv()  \300\320\261\342 \275\307\306\320" );
		goto IsaoCheck_winsockERR;
	case ISAO_ERR_SSL_TIMEOUT:			// 타임 아웃했다．
		fd2("---------------- select() \305\270\300\323\276\306\277\364 \275\307\306\320" );
		goto IsaoCheck_winsockERR;
	};
	//winsock 상의 에러 첵크함.  ================================== end

	
	ret_chk = chk_readbuffer("account=", szReadBuffer, &iRejectCode );
	switch(ret_chk)
	{
	case 0: //아이디, 패스워드 정확	
		//ret_chk = LoginCheck(id,StartTime);			//리턴 값이 13이면 Login 테이블에 등록 14 이면 등록 실패
		sprintf(szWriteBuffer,
			//"POST http://%s/LoginCheck.php?UserID=%s&StartTime=%d\r\n\r\n\r\n",hostName, id, StartTime );
			"http://%s/LoginCheck.php?UserID=%s&StartTime=%d", hostName, id, StartTime );
		ret_chk = connect_WEBdb( szWriteBuffer, szReadBuffer);
		ret_chk = chk_readbuffer("account=", szReadBuffer, &iRejectCode );
		switch(ret_chk)
		{
		case 13:
			goto IsaoCheck_Free;
		case 14:
			goto IsaoCheck_Free;
		}
		goto IsaoCheck_Free;
	case 1: //패스워드 불량		
		goto IsaoCheck_Free;
	case 2: //아이디가 등록되지 않았음
		 //ret_chk = InsertUser(id,pass,"aaaa");
		 sprintf(szWriteBuffer,
			//"POST http://%s/InsertUser.php?UserID=%s&PassWord=%s&UserName=%s\r\n\r\n\r\n",hostName, id,pass,"aaaa");
			"http://%s/InsertUser.php?UserID=%s&PassWord=%s&UserName=%s",hostName, id,pass,"aaaa");
		ret_chk = connect_WEBdb( szWriteBuffer, szReadBuffer);
		ret_chk = chk_readbuffer("account=", szReadBuffer, &iRejectCode );

		switch(ret_chk)
		{
		case 3: //등록 정확 
			//ret_chk = LoginCheck(id,12);		//리턴 값이 5이면 Login 테이블에 등록 6 이면 등록 실패
			sprintf(szWriteBuffer,
				//"POST http://%s/LoginCheck.php?UserID=%s&StartTime=%d\r\n\r\n\r\n",hostName, id, StartTime );
			"http://%s/LoginCheck.php?UserID=%s&StartTime=%d",hostName, id, StartTime );
			ret_chk = connect_WEBdb( szWriteBuffer, szReadBuffer);
			ret_chk = chk_readbuffer("account=", szReadBuffer, &iRejectCode );
		    switch(ret_chk)
			{
		    case 5:
			    goto IsaoCheck_Free;
		    case 6:
			    goto IsaoCheck_Free;
			}
			goto IsaoCheck_Free;
		case 4: //등록실패
			goto IsaoCheck_Free;
		 }	
		goto IsaoCheck_Free;


	case ISAO_ERR_NONE_REEJCT:					//응답번호가 없음.
		fd2("ISAO_ERR_NONE_REEJCT  : \300\300\264\344\271\370\310\243\260\241 \276\370\300\275" );
		goto IsaoCheck_Free;
	}
	

IsaoCheck_Free:
	return ret_chk;


IsaoCheck_winsockERR:			//winsock으로 연결시에러남
	return ret_chk ;
}








/*///////////////////////////////////////////////////////////////////////////
: 함 수 명 :  int IsaoCheck2()
: 하 는 일 :  유저가 접속을 끊을때 사용됨
: 서    식 :  
: 인    자 :
: 반 환 값 :
: 논    평 :
: 버    전 :
: PROJECT  :
///////////////////////////////////////////////////////////////////////////*/
//과금서버에 id, ip, 접속하고 있는 초를 넘겨준다.
int ISAOCHECK::IsaoCheck_GameExit(char *id, char *pass, char *ipaddr, long usertime)
{
	int ret_chk;
	int iRejectCode = 111111;
	char szWriteBuffer[BUF_LEN];
	char szReadBuffer [BUF_LEN];
	
	
	
	sprintf( szWriteBuffer, 
	//"POST http://%s/Login_End.php?UserID=%s&IP=%s&EndTime=%d&UseTime=%d\r\n\r\n\r\n",hostName, id,ipaddr,iTimeOut,usertime);
	"http://%s/Login_End.php?UserID=%s&IP=%s&EndTime=%d&UseTime=%d",hostName, id,ipaddr,iTimeOut,usertime);
	ret_chk = connect_WEBdb( szWriteBuffer, szReadBuffer);

	//ktj : 임시
	fd2(" \300\245 db\277\241\274\255 \300\320\300\272\260\315 %s ", szReadBuffer);
	
	//ktj : 아래 첵크는 모두 임시적으로 운영됨

	//winsock 상의 에러 첵크함.  ================================== start
	switch(ret_chk) {
	case ISAO_ERR_WINSOCK_NOERR:		//에러 없음.
		//ktj : 임시로 서버에서 온 데이타를 뿌려봄.
		fd2( "From server: ---from--\n%s--end--\n ",szReadBuffer);			
		break;
	case ISAO_ERR_WINSOCK_START:		//윈속 START시 에러발생				//window only
		fd2("WSAStartup()\277\241\266\363");
		goto IsaoCheck_winsockERR;
	case ISAO_ERR_WINSOCK_VERSION:		//윈속버전이 틀려 구동되지못함.			//window only
		fd2("\271\366\300\374\300\314 \270\302\301\366 \276\312\300\275");
		goto IsaoCheck_winsockERR;
	case ISAO_ERR_TCP_CONNECT:			// 보통 사용되는 ＴＣＰ코네쿠토에라
		fd2( "connect() \301\242\274\323 \275\307\306\320" );
		goto IsaoCheck_winsockERR;
	case ISAO_ERR_SSL_READ:				// ＳＳＬ 리드 할 수 없었다．
		fd2(" recv()  \300\320\261\342 \275\307\306\320" );
		goto IsaoCheck_winsockERR;
	case ISAO_ERR_SSL_TIMEOUT:			// 타임 아웃했다．
		fd2("---------------- select() \305\270\300\323\276\306\277\364 \275\307\306\320" );
		goto IsaoCheck_winsockERR;
	};
	//winsock 상의 에러 첵크함.  ================================== end

	ret_chk = chk_readbuffer("account=", szReadBuffer, &iRejectCode );
	switch(ret_chk)
	{
	case 7: //리턴 값이 7이면 Log테이블(누적 테이블)에 등록 성공하면서 Login 테이블에 있는 데이터는 삭제
		goto IsaoCheck_Free;

	case 8: //리턴 값이 8이면 Log테이블(누적 테이블)에 등록 실패
		goto IsaoCheck_Free;
		
	case ISAO_ERR_NONE_REEJCT:					//응답번호가 없음.
		fd2("ISAO_ERR_NONE_REEJCT  : \300\300\264\344\271\370\310\243\260\241 \276\370\300\275" );
		goto IsaoCheck_Free;
	}
	

IsaoCheck_Free:

	return ret_chk ;


IsaoCheck_winsockERR:			//winsock으로 연결시에러남
	return ret_chk ;
}

















//ygy
/*///////////////////////////////////////////////////////////////////////////
: 함 수 명 :  int isCheckClanJang()
: 하 는 일 :  계정에 클랜장으로 등록되어 있는지 체크(100:클랜멤버, 101:클랜장  102:클랜원이 아니다)
: 서    식 :  
: 인    자 :  id, 게임서버
: 반 환 값 :
: 논    평 :  실제 사용함수 
: 버    전 :  100,101은 클랜이름도 가져옴.
: PROJECT  :  tjboyh/isClanMember.php와 연동됨.
///////////////////////////////////////////////////////////////////////////*/
int ISAOCHECK::isCheckClanJang(char *ClanZang_id, char* gserver)
{
	int ret_chk;
	int iRejectCode = 111111;
	char szWriteBuffer[BUF_LEN];
	char szReadBuffer [BUF_LEN];
	
	//일본클랜
	sprintf( szWriteBuffer,
	"http://%s/ServerMain/isCheckClanJang.asp?userid=%s&gserver=%s&ticket=%d", hostName,ClanZang_id,gserver,nClanTicket);	
	
	int cnt=0;
	while(1) {		//컨넥트되도 답이 안오는경우를 위해 5번정도 재접한다.
		ret_chk = connect_WEBdb( szWriteBuffer, szReadBuffer);
		if(ret_chk != ISAO_ERR_RECONNECT) break;
		if(cnt>5) break;
		cnt++;
	}
	
	//ktj : 임시
	
	debug_fd2( "isCheckClanJang() \272\270\263\275\260\315 \261\346\300\314 : %d(ret_chk %d) \263\273\277\353 : %s ", strlen(szWriteBuffer), ret_chk, szWriteBuffer);
	debug_fd2( "isCheckClanJang() \271\336\300\272\260\315 \261\346\300\314 : %d \263\273\277\353 : %s ", strlen(szReadBuffer), szReadBuffer);
	



	//ktj : 아래 첵크는 모두 임시적으로 운영됨

	//winsock 상의 에러 첵크함.  ================================== start
	switch(ret_chk) {
	case ISAO_ERR_WINSOCK_NOERR:		//에러 없음.
		//ktj : 임시로 서버에서 온 데이타를 뿌려봄.
		//fd2( "From server: ---from--\n%s--end--\n ",szReadBuffer);			

		//ret_chk = chk_readbuffer("account=", szReadBuffer, &iRejectCode );
		ret_chk = chk_readbuffer("Code=", szReadBuffer, &iRejectCode );
		switch(ret_chk)
		{
		case 0: //클랜이 등록되어 있지 않음
			break;
		case 1: //클랜원 등록
			break;
		case 5:
			break;
		
		/*
		case 100:	//클랜원
		case 101:	//클랜칲(결성심사완료후)
		case 102:	//클랜원으로 가입되있지 않음
		case 103:	//결성심사전이며 10일이 지난경우
		case 104:	//결성심사전이며 10일이 안지난경우
		case 105:	//접속에러
		*/			
			break;
			

		case ISAO_ERR_NONE_REEJCT:					//응답번호가 없음.
			fd2("ISAO_ERR_NONE_REEJCT  : \300\300\264\344\271\370\310\243\260\241 \276\370\300\275" );
			goto IsaoCheck_Free;
		}



		break;
	case ISAO_ERR_WINSOCK_START:		//윈속 START시 에러발생				//window only
		fd2("WSAStartup()\277\241\266\363");
		goto IsaoCheck_winsockERR;
	case ISAO_ERR_WINSOCK_VERSION:		//윈속버전이 틀려 구동되지못함.			//window only
		fd2("\271\366\300\374\300\314 \270\302\301\366 \276\312\300\275");
		goto IsaoCheck_winsockERR;
	case ISAO_ERR_TCP_CONNECT:			// 보통 사용되는 ＴＣＰ코네쿠토에라
		fd2( "connect() \301\242\274\323 \275\307\306\320" );
		goto IsaoCheck_winsockERR;
	case ISAO_ERR_SSL_READ:				// ＳＳＬ 리드 할 수 없었다．
		fd2(" recv()  \300\320\261\342 \275\307\306\320" );
		goto IsaoCheck_winsockERR;
	case ISAO_ERR_SSL_TIMEOUT:			// 타임 아웃했다．
		fd2("---------------- select() \305\270\300\323\276\306\277\364 \275\307\306\320" );
		goto IsaoCheck_winsockERR;
	};
	//winsock 상의 에러 첵크함.  ================================== end

	

IsaoCheck_Free:
	return ret_chk;


IsaoCheck_winsockERR:			//winsock으로 연결시에러남
	return ret_chk ;
}

/*///////////////////////////////////////////////////////////////////////////
: 함 수 명 :  int isCheckClanName()
: 하 는 일 :  클랜 이름이 중복되었는지 체크
: 서    식 :  
: 인    자 :  id, 게임서버
: 반 환 값 :
: 논    평 :  실제 사용함수 
: 버    전 :  
: PROJECT  :  
///////////////////////////////////////////////////////////////////////////*/
int ISAOCHECK::isCheckClanName(char *ClanName,char *gserver)
{
	int ret_chk;
	int iRejectCode = 111111;
	char szWriteBuffer[BUF_LEN];
	char szReadBuffer [BUF_LEN];

/*	
//일본클랜이름검사
#ifdef _LANGUAGE_JAPANESE

	char UTF8_ClanName[1024];				memset(UTF8_ClanName,0,sizeof(UTF8_ClanName));
	MBCSToWeb(ClanName,				UTF8_ClanName,932);

	
	sprintf( szWriteBuffer,
	"http://%s/ServerMain/isCheckClanName.asp?ClName=%s&gserver=%s&ticket=%d", hostName, 
	UTF8_ClanName,
	gserver,nClanTicket);

#else
*/	
	sprintf( szWriteBuffer,
	"http://%s/ServerMain/isCheckClanName.asp?ClName=%s&gserver=%s&ticket=%d", hostName, ClanName,gserver,nClanTicket);
//#endif
	

	int cnt=0;
	while(1) {		//컨넥트되도 답이 안오는경우를 위해 5번정도 재접한다.
		ret_chk = connect_WEBdb( szWriteBuffer, szReadBuffer);
		if(ret_chk != ISAO_ERR_RECONNECT) break;
		if(cnt>5) break;
		cnt++;
	}

	//ktj : 임시
	

	debug_fd2( "isCheckClanName() \272\270\263\275\260\315 \261\346\300\314 : %d \263\273\277\353 : %s ", strlen(szWriteBuffer), szWriteBuffer);
	debug_fd2( "isCheckClanName() \271\336\300\272\260\315 \261\346\300\314 : %d \263\273\277\353 : %s ", strlen(szReadBuffer), szReadBuffer);
	

	
	//ktj : 아래 첵크는 모두 임시적으로 운영됨

	//winsock 상의 에러 첵크함.  ================================== start
	switch(ret_chk) {
	case ISAO_ERR_WINSOCK_NOERR:		//에러 없음.
		//ktj : 임시로 서버에서 온 데이타를 뿌려봄.
		//fd2( "From server: ---from--\n%s--end--\n ",szReadBuffer);			

		//ret_chk = chk_readbuffer("account=", szReadBuffer, &iRejectCode );
		ret_chk = chk_readbuffer("Code=", szReadBuffer, &iRejectCode );
		switch(ret_chk)
		{
		case 0: //클랜이 등록되어 있지 않음
			break;
		case 1: //클랜원 등록
			break;
		
		/*
		case 100:	//클랜원
		case 101:	//클랜칲(결성심사완료후)
		case 102:	//클랜원으로 가입되있지 않음
		case 103:	//결성심사전이며 10일이 지난경우
		case 104:	//결성심사전이며 10일이 안지난경우
		case 105:	//접속에러
		*/				

		case ISAO_ERR_NONE_REEJCT:					//응답번호가 없음.
			fd2("ISAO_ERR_NONE_REEJCT  : \300\300\264\344\271\370\310\243\260\241 \276\370\300\275" );
			goto IsaoCheck_Free;
		}



		break;
	case ISAO_ERR_WINSOCK_START:		//윈속 START시 에러발생				//window only
		fd2("WSAStartup()\277\241\266\363");
		goto IsaoCheck_winsockERR;
	case ISAO_ERR_WINSOCK_VERSION:		//윈속버전이 틀려 구동되지못함.			//window only
		fd2("\271\366\300\374\300\314 \270\302\301\366 \276\312\300\275");
		goto IsaoCheck_winsockERR;
	case ISAO_ERR_TCP_CONNECT:			// 보통 사용되는 ＴＣＰ코네쿠토에라
		fd2( "connect() \301\242\274\323 \275\307\306\320" );
		goto IsaoCheck_winsockERR;
	case ISAO_ERR_SSL_READ:				// ＳＳＬ 리드 할 수 없었다．
		fd2(" recv()  \300\320\261\342 \275\307\306\320" );
		goto IsaoCheck_winsockERR;
	case ISAO_ERR_SSL_TIMEOUT:			// 타임 아웃했다．
		fd2("---------------- select() \305\270\300\323\276\306\277\364 \275\307\306\320" );
		goto IsaoCheck_winsockERR;
	};
	//winsock 상의 에러 첵크함.  ================================== end

	

IsaoCheck_Free:
	return ret_chk;


IsaoCheck_winsockERR:			//winsock으로 연결시에러남
	return ret_chk ;
}


/*///////////////////////////////////////////////////////////////////////////
: 함 수 명 :  int ReadClanInfo(char *clanNum)
: 하 는 일 :  클랜마크 번호로 클랜 정보 읽어오기
: 서    식 :  
: 인    자 :  clannum
: 반 환 값 :
: 논    평 :  실제 사용함수 
: 버    전 :  
: PROJECT  :  
///////////////////////////////////////////////////////////////////////////*/
int ISAOCHECK::ReadClanInfo(char *clanNum,char* ret_Buf)
{
	int ret_chk;
	int iRejectCode = 111111;
	char szWriteBuffer[BUF_LEN];
	char szReadBuffer [BUF_LEN];
	
	//일본클랜번호
	sprintf( szWriteBuffer,
	"http://%s/ServerMain/isCheckClanNum.asp?num=%s&ticket=%d", hostName,clanNum,nClanTicket);
	
	

	int cnt=0;
	while(1) {		//컨넥트되도 답이 안오는경우를 위해 5번정도 재접한다.
		ret_chk = connect_WEBdb( szWriteBuffer, szReadBuffer);
		if(ret_chk != ISAO_ERR_RECONNECT) break;
		if(cnt>5) break;
		cnt++;
	}

	//ktj : 임시
	
	debug_fd2( "ReadClanInfo() \272\270\263\275\260\315 \261\346\300\314 : %d \263\273\277\353 : %s ", strlen(szWriteBuffer), szWriteBuffer);
	debug_fd2( "ReadClanInfo() \271\336\300\272\260\315 \261\346\300\314 : %d \263\273\277\353 : %s ", strlen(szReadBuffer), szReadBuffer);
	


	
	//ktj : 아래 첵크는 모두 임시적으로 운영됨

	//winsock 상의 에러 첵크함.  ================================== start
	switch(ret_chk) {
	case ISAO_ERR_WINSOCK_NOERR:		//에러 없음.
		//ktj : 임시로 서버에서 온 데이타를 뿌려봄.
		//fd2( "From server: ---from--\n%s--end--\n ",szReadBuffer);			

		//ret_chk = chk_readbuffer("account=", szReadBuffer, &iRejectCode );
		ret_chk = chk_readbuffer("Code=", szReadBuffer, &iRejectCode );
		switch(ret_chk)
		{
		case 0: //클랜이 등록되어 있지 않음
			break;
		case 1: //클랜 정보 읽어오기 성공
			strcpy( ret_Buf, szReadBuffer);
			break;			

		case ISAO_ERR_NONE_REEJCT:					//응답번호가 없음.
			fd2("ISAO_ERR_NONE_REEJCT  : \300\300\264\344\271\370\310\243\260\241 \276\370\300\275" );
			goto IsaoCheck_Free;
		}



		break;
	case ISAO_ERR_WINSOCK_START:		//윈속 START시 에러발생				//window only
		fd2("WSAStartup()\277\241\266\363");
		goto IsaoCheck_winsockERR;
	case ISAO_ERR_WINSOCK_VERSION:		//윈속버전이 틀려 구동되지못함.			//window only
		fd2("\271\366\300\374\300\314 \270\302\301\366 \276\312\300\275");
		goto IsaoCheck_winsockERR;
	case ISAO_ERR_TCP_CONNECT:			// 보통 사용되는 ＴＣＰ코네쿠토에라
		fd2( "connect() \301\242\274\323 \275\307\306\320" );
		goto IsaoCheck_winsockERR;
	case ISAO_ERR_SSL_READ:				// ＳＳＬ 리드 할 수 없었다．
		fd2(" recv()  \300\320\261\342 \275\307\306\320" );
		goto IsaoCheck_winsockERR;
	case ISAO_ERR_SSL_TIMEOUT:			// 타임 아웃했다．
		fd2("---------------- select() \305\270\300\323\276\306\277\364 \275\307\306\320" );
		goto IsaoCheck_winsockERR;
	};
	//winsock 상의 에러 첵크함.  ================================== end

	

IsaoCheck_Free:
	return ret_chk;


IsaoCheck_winsockERR:			//winsock으로 연결시에러남
	return ret_chk ;
}



//호동Clan추가
int ISAOCHECK::isCheckClanMember(char *pszServerName, char *pszUserName)
{
	int		iRejectCode = 111111;
	char	szWriteBuffer[BUF_LEN];
	char	szReadBuffer[BUF_LEN];
	int		receiveCode;

	ZeroMemory(szReadBuffer, sizeof(szReadBuffer));
/*
//일본클랜멤버검사
#ifdef _LANGUAGE_JAPANESE

	char UTF8_pszUserName[1024];		memset(UTF8_pszUserName,0,sizeof(UTF8_pszUserName));
	MBCSToWeb(pszUserName,		UTF8_pszUserName,932);

	
	sprintf(szWriteBuffer,
	"http://%s/ServerMain/isCheckClanwon.asp?gserver=%s&clwon=%s",hostName,
	pszServerName,UTF8_pszUserName);

#else
*/
	sprintf(szWriteBuffer,
	"http://%s/ServerMain/isCheckClanwon.asp?gserver=%s&clwon=%s",hostName,
	pszServerName,pszUserName);
//#endif
	
	int cnt=0;
	while(1) {		//컨넥트되도 답이 안오는경우를 위해 5번정도 재접한다.
		receiveCode = connect_WEBdb( szWriteBuffer, szReadBuffer);
		if(receiveCode != ISAO_ERR_RECONNECT) break;
		if(cnt>5) break;
		cnt++;
	}
	
	if(strlen(szReadBuffer) > 2048) {
		szReadBuffer[2048] = 0;
		debug_fd2( "isClanMember() \263\312\271\253\261\346\276\356\274\255 \300\337\266\372\264\331.");
	}

	switch(receiveCode)
	{
		case ISAO_ERR_WINSOCK_NOERR:
		{
			receiveCode = chk_readbuffer("Code=", szReadBuffer, &iRejectCode );
			if(receiveCode == ISAO_ERR_NONE_REEJCT)
			{
				fd2("ISAO_ERR_NONE_REEJCT  : \300\300\264\344\271\370\310\243\260\241 \276\370\300\275" );
			}
		}

		case ISAO_ERR_WINSOCK_START:		//윈속 Start 에러 발생
		{
			fd2("WSAStartup()\277\241\266\363");
			break;
		}
		case ISAO_ERR_WINSOCK_VERSION:		//윈속 버전이 틀림
		{
			fd2("\271\366\300\374\300\314 \270\302\301\366 \276\312\300\275");
			break;
		}

		case ISAO_ERR_TCP_CONNECT:			//접속 실패
		{
			fd2( "connect() \301\242\274\323 \275\307\306\320" );
			break;
		}
		case ISAO_ERR_SSL_READ:				//Read 실패
		{
			fd2(" recv()  \300\320\261\342 \275\307\306\320" );
			break;
		}
		case ISAO_ERR_SSL_TIMEOUT:			//TimeOut
		{
			fd2("---------------- select() \305\270\300\323\276\306\277\364 \275\307\306\320" );
			break;
		}
	}
	
	return receiveCode;
}

/*///////////////////////////////////////////////////////////////////////////
: 함 수 명 :  int isClanMember()
: 하 는 일 :  클랜멤버인지여부(100:클랜멤버, 101:클랜장  102:클랜원이 아니다)
: 서    식 :  
: 인    자 :  id, 클랜이름받을 버퍼
: 반 환 값 :
: 논    평 :  실제 사용함수 
: 버    전 :  100,101은 클랜이름도 가져옴.
: PROJECT  :  tjboyh/isClanMember.php와 연동됨.
///////////////////////////////////////////////////////////////////////////*/

//ktj : isClanMember.php: 를 만들어넣어야함.
int ISAOCHECK::isClanMember(char *ClanZang_id, char* gserver,char* ClanZang_ChaName,char* ret_Buf)
{

	int ret_chk;
	int iRejectCode = 111111;
	char szWriteBuffer[BUF_LEN];
	char szReadBuffer [BUF_LEN];
	
	ZeroMemory(szReadBuffer , sizeof(szReadBuffer ));
/*
//일본클랜상퇴검사	
#ifdef _LANGUAGE_JAPANESE

	char UTF8_ClanZang_ChaName[1024];		memset(UTF8_ClanZang_ChaName,0,sizeof(UTF8_ClanZang_ChaName));
	MBCSToWeb(ClanZang_ChaName,		UTF8_ClanZang_ChaName,932);

	
	sprintf( szWriteBuffer,
	"http://%s/ServerMain/isClanMember.asp?userid=%s&gserver=%s&chname=%s&ticket=%d", 
	hostName,ClanZang_id,gserver,
	UTF8_ClanZang_ChaName,
	nClanTicket);

#else
*/
	sprintf( szWriteBuffer,
	"http://%s/ServerMain/isClanMember.asp?userid=%s&gserver=%s&chname=%s&ticket=%d", hostName,ClanZang_id,gserver,ClanZang_ChaName,nClanTicket);
/*		"http://pristonclan.hangame.com/ServerMain/isClanMember.asp?userid=광범솔&gserver=러씨디&chname=광우전사&ticket=411");*/ // 문제클랜
//#endif
	

	int cnt=0;
	while(1) {		//컨넥트되도 답이 안오는경우를 위해 5번정도 재접한다.
		ret_chk = connect_WEBdb( szWriteBuffer, szReadBuffer);
		if(ret_chk != ISAO_ERR_RECONNECT) break;
		if(cnt>5) break;
		cnt++;
	}

	
	//for(cnt=0; cnt<BUF_LEN;cnt++) {
	//	szReadBuffer[cnt] = 'a';
	//}
	if(strlen(szReadBuffer) > 2048) {
		szReadBuffer[2048] = 0;
		debug_fd2( "isClanMember() \263\312\271\253\261\346\276\356\274\255 \300\337\266\372\264\331.");
	}
	//ktj : 임시
	
	debug_fd2( "isClanMember() \272\270\263\275\260\315 \261\346\300\314 : %d \263\273\277\353 : %s ", strlen(szWriteBuffer), szWriteBuffer);
	debug_fd2( "isClanMember() \271\336\300\272\260\315 \261\346\300\314 : %d \263\273\277\353 : %s ", strlen(szReadBuffer), szReadBuffer);
	

	
	//ktj : 아래 첵크는 모두 임시적으로 운영됨

	//winsock 상의 에러 첵크함.  ================================== start
	switch(ret_chk) {
	case ISAO_ERR_WINSOCK_NOERR:		//에러 없음.
		//ktj : 임시로 서버에서 온 데이타를 뿌려봄.
		//fd2( "From server: ---from--\n%s--end--\n ",szReadBuffer);			

		//ret_chk = chk_readbuffer("account=", szReadBuffer, &iRejectCode );
		ret_chk = chk_readbuffer("Code=", szReadBuffer, &iRejectCode );
		switch(ret_chk)
		{
		case 0: //클랜이 등록되어 있지 않음
			strcpy( ret_Buf, szReadBuffer);
			break;
		case 1: //클랜원 등록
//			break;
		case 5:
		case 4:
		case 3:
		case 2: //클랜장 등록
			strcpy( ret_Buf, szReadBuffer);
			break;
		/*
		case 100:	//클랜원
		case 101:	//클랜칲(결성심사완료후)
		case 102:	//클랜원으로 가입되있지 않음
		case 103:	//결성심사전이며 10일이 지난경우
		case 104:	//결성심사전이며 10일이 안지난경우
		case 105:	//접속에러
		*/

			strcpy( ret_Buf, szReadBuffer);
			break;
			

		case ISAO_ERR_NONE_REEJCT:					//응답번호가 없음.
			fd2("ISAO_ERR_NONE_REEJCT  : \300\300\264\344\271\370\310\243\260\241 \276\370\300\275" );
			goto IsaoCheck_Free;
		}



		break;
	case ISAO_ERR_WINSOCK_START:		//윈속 START시 에러발생				//window only
		fd2("WSAStartup()\277\241\266\363");
		goto IsaoCheck_winsockERR;
	case ISAO_ERR_WINSOCK_VERSION:		//윈속버전이 틀려 구동되지못함.			//window only
		fd2("\271\366\300\374\300\314 \270\302\301\366 \276\312\300\275");
		goto IsaoCheck_winsockERR;
	case ISAO_ERR_TCP_CONNECT:			// 보통 사용되는 ＴＣＰ코네쿠토에라
		fd2( "connect() \301\242\274\323 \275\307\306\320" );
		goto IsaoCheck_winsockERR;
	case ISAO_ERR_SSL_READ:				// ＳＳＬ 리드 할 수 없었다．
		fd2(" recv()  \300\320\261\342 \275\307\306\320" );
		goto IsaoCheck_winsockERR;
	case ISAO_ERR_SSL_TIMEOUT:			// 타임 아웃했다．
		fd2("---------------- select() \305\270\300\323\276\306\277\364 \275\307\306\320" );
		goto IsaoCheck_winsockERR;
	};
	//winsock 상의 에러 첵크함.  ================================== end

	

IsaoCheck_Free:
	return ret_chk;


IsaoCheck_winsockERR:			//winsock으로 연결시에러남
	return ret_chk ;
}




/*///////////////////////////////////////////////////////////////////////////
: 함 수 명 :  int joinClanMember()
: 하 는 일 :  클랜에 멤버로 등록
: 서    식 :  
: 인    자 :  클랜원 id, 클랜원의 캐릭터 id
: 반 환 값 :
: 논    평 :  실제 사용함수 
: 버    전 :  
: PROJECT  : 
///////////////////////////////////////////////////////////////////////////*/
int ISAOCHECK::joinClanMember(char *ClanZang_ID, char *gserver, char *ClanZang_ChaName,char* ClanName,char* ClanWon,char* ClanWon_ID,int level,DWORD chtype,int joinlevel,char *chipflag)
{
	int ret_chk;
	int iRejectCode = 111111;
	char szWriteBuffer[BUF_LEN];
	char szReadBuffer [BUF_LEN];
/*
//일본클랜가입
#ifdef _LANGUAGE_JAPANESE

	char UTF8_ClanZang_ChaName[1024];		memset(UTF8_ClanZang_ChaName,0,sizeof(UTF8_ClanZang_ChaName));
	char UTF8_ClanName[1024];				memset(UTF8_ClanName,0,sizeof(UTF8_ClanName));
	char UTF8_ClanWon[1024];			memset(UTF8_ClanWon,0,sizeof(UTF8_ClanWon));
	
	MBCSToWeb(ClanZang_ChaName,		UTF8_ClanZang_ChaName,932);
	MBCSToWeb(ClanName,				UTF8_ClanName,932);
	MBCSToWeb(ClanWon,				UTF8_ClanWon,932);

	
	sprintf( szWriteBuffer,
			"http://%s/ServerMain/clanInsertClanWon.asp?userid=%s&gserver=%s&chname=%s&clName=%s&clwon=%s&clwonUserid=%s&lv=%d&ticket=%d&chtype=%d&chlv=%d&chipflag=%s", 
			hostName,
			ClanZang_ID,
			gserver,
				UTF8_ClanZang_ChaName,
				UTF8_ClanName,
				UTF8_ClanWon,
			ClanWon_ID,level,nClanTicket,chtype,joinlevel,chipflag);

#else
*/
	sprintf( szWriteBuffer,
			"http://%s/ServerMain/clanInsertClanWon.asp?userid=%s&gserver=%s&chname=%s&clName=%s&clwon=%s&clwonUserid=%s&lv=%d&ticket=%d&chtype=%d&chlv=%d&chipflag=%s", 
			hostName,ClanZang_ID,gserver,ClanZang_ChaName,ClanName,ClanWon,ClanWon_ID,level,nClanTicket,chtype,joinlevel,chipflag);
//#endif



	int cnt=0;
	while(1) {		//컨넥트되도 답이 안오는경우를 위해 5번정도 재접한다.
		ret_chk = connect_WEBdb( szWriteBuffer, szReadBuffer);
		if(ret_chk != ISAO_ERR_RECONNECT) break;
		if(cnt>5) break;
		cnt++;
	}

	//ktj : 임시
	
	debug_fd2( "joinClanMember() \272\270\263\275\260\315 \261\346\300\314 : %d \263\273\277\353 : %s ", strlen(szWriteBuffer), szWriteBuffer);
	debug_fd2( "joinClanMember() \271\336\300\272\260\315 \261\346\300\314 : %d \263\273\277\353 : %s ", strlen(szReadBuffer), szReadBuffer);
	


	
	//ktj : 아래 첵크는 모두 임시적으로 운영됨

	//winsock 상의 에러 첵크함.  ================================== start
	switch(ret_chk) {
	case ISAO_ERR_WINSOCK_NOERR:		//에러 없음.
		//ktj : 임시로 서버에서 온 데이타를 뿌려봄.
		//fd2( "From server: ---from--\n%s--end--\n ",szReadBuffer);			

		ret_chk = chk_readbuffer("Code=", szReadBuffer, &iRejectCode );
		switch(ret_chk)
		{
		case 0: //트랜잭션 오류
			break;
		case 1: //클랜원 가입 성공
			break;
		case 2: //현재 가입시키려는 클랜원이 이미 다른 클랜에 가입해있을 경우(자기 클랜포함)
			break;
		case 3: //클랜이 존재하지 않거나 클랜 장이 아닌경우
			break;
		case 4: //클랜 멤버가 32 이상일 경우(Max : 32)
			break;
		case 100:	//성공
		case 101:	//클랜이름 존재안함.
		case 102:	//클랜원추가실패
		case 103:	//
			break;

		case ISAO_ERR_NONE_REEJCT:					//응답번호가 없음.
			fd2("ISAO_ERR_NONE_REEJCT  : \300\300\264\344\271\370\310\243\260\241 \276\370\300\275" );
			goto IsaoCheck_Free;
		}


		break;
	case ISAO_ERR_WINSOCK_START:		//윈속 START시 에러발생				//window only
		fd2("WSAStartup()\277\241\266\363");
		goto IsaoCheck_winsockERR;
	case ISAO_ERR_WINSOCK_VERSION:		//윈속버전이 틀려 구동되지못함.			//window only
		fd2("\271\366\300\374\300\314 \270\302\301\366 \276\312\300\275");
		goto IsaoCheck_winsockERR;
	case ISAO_ERR_TCP_CONNECT:			// 보통 사용되는 ＴＣＰ코네쿠토에라
		fd2( "connect() \301\242\274\323 \275\307\306\320" );
		goto IsaoCheck_winsockERR;
	case ISAO_ERR_SSL_READ:				// ＳＳＬ 리드 할 수 없었다．
		fd2(" recv()  \300\320\261\342 \275\307\306\320" );
		goto IsaoCheck_winsockERR;
	case ISAO_ERR_SSL_TIMEOUT:			// 타임 아웃했다．
		fd2("---------------- select() \305\270\300\323\276\306\277\364 \275\307\306\320" );
		goto IsaoCheck_winsockERR;
	};
	//winsock 상의 에러 첵크함.  ================================== end

	

IsaoCheck_Free:
	return ret_chk;


IsaoCheck_winsockERR:			//winsock으로 연결시에러남
	return ret_chk ;
}




/*///////////////////////////////////////////////////////////////////////////
: 함 수 명 :  int makeClan()
: 하 는 일 :  클랜을 새로만들경우의 함수
: 서    식 :  
: 인    자 :  클랜이름, 설명, 클랜장의 id가 필요함.
: 반 환 값 :
: 논    평 :  실제 사용함수 
: 버    전 :
: PROJECT  :
///////////////////////////////////////////////////////////////////////////*/
int ISAOCHECK::makeClan(char *ClanZang_ID, char* gserver,char* ClanZang_ChaName,char* ClanName,char *explanation,DWORD chtype,int level,char* data)
{
	int ret_chk;
	int iRejectCode = 111111;
	char szWriteBuffer[BUF_LEN];
	char szReadBuffer [BUF_LEN];

	int chkCnt=0;
	

//일본클랜생성	
 #ifdef _LANGUAGE_JAPANESE

	char UTF8_ClanZang_ChaName[1024];		memset(UTF8_ClanZang_ChaName,0,sizeof(UTF8_ClanZang_ChaName));
	char UTF8_ClanName[1024];				memset(UTF8_ClanName,0,sizeof(UTF8_ClanName));
	char UTF8_explanation[1024];			memset(UTF8_explanation,0,sizeof(UTF8_explanation));
	
	MBCSToWeb(ClanZang_ChaName,		UTF8_ClanZang_ChaName,932);
	MBCSToWeb(ClanName,				UTF8_ClanName,932);
	MBCSToWeb(explanation,			UTF8_explanation,932);
	
	sprintf	//이부분만 남겨놓음
	(
	szWriteBuffer,
	"http://%s/Clan/clanInsert.aspx?userid=%s&gserver=%s&chname=%s&clName=%s&expl=%s&ticket=%d&chtype=%d&lv=%d",
	hostName1,ClanZang_ID, gserver,
		UTF8_ClanZang_ChaName,
		UTF8_ClanName,
		UTF8_explanation,
		nClanTicket,chtype,level 
	);

 #else  

	sprintf
	(
	szWriteBuffer,
	"http://%s/Clan/clanInsert.aspx?userid=%s&gserver=%s&chname=%s&clName=%s&expl=%s&ticket=%d&chtype=%d&lv=%d",
	hostName1,ClanZang_ID, gserver,ClanZang_ChaName,ClanName,explanation,nClanTicket,chtype,level 
	);

#endif

	int cnt=0;
	while(1) {		//컨넥트되도 답이 안오는경우를 위해 5번정도 재접한다.
		ret_chk = connect_WEBdb_1( szWriteBuffer, szReadBuffer);
		if(ret_chk != ISAO_ERR_RECONNECT) break;
		if(cnt>5) break;
		cnt++;
	}

	//ktj : 임시
	
	debug_fd2( "makeClan() \272\270\263\275\260\315 \261\346\300\314 : %d \263\273\277\353 : %s ", strlen(szWriteBuffer), szWriteBuffer);
	debug_fd2( "makeClan() \271\336\300\272\260\315 \261\346\300\314 : %d \263\273\277\353 : %s ", strlen(szReadBuffer), szReadBuffer);
	



	
	
	//ktj : 아래 첵크는 모두 임시적으로 운영됨

	//winsock 상의 에러 첵크함.  ================================== start
	switch(ret_chk) {
	case ISAO_ERR_WINSOCK_NOERR:		//에러 없음.
		//ktj : 임시로 서버에서 온 데이타를 뿌려봄.
		fd2( "From server: ---from--\n%s--end--\n ",szReadBuffer);			
		break;
	case ISAO_ERR_WINSOCK_START:		//윈속 START시 에러발생				//window only
		fd2("WSAStartup()\277\241\266\363");
		goto IsaoCheck_winsockERR;
	case ISAO_ERR_WINSOCK_VERSION:		//윈속버전이 틀려 구동되지못함.			//window only
		fd2("\271\366\300\374\300\314 \270\302\301\366 \276\312\300\275");
		goto IsaoCheck_winsockERR;
	case ISAO_ERR_TCP_CONNECT:			// 보통 사용되는 ＴＣＰ코네쿠토에라
		fd2( "connect() \301\242\274\323 \275\307\306\320" );
		goto IsaoCheck_winsockERR;
	case ISAO_ERR_SSL_READ:				// ＳＳＬ 리드 할 수 없었다．
		fd2(" recv()  \300\320\261\342 \275\307\306\320" );
		goto IsaoCheck_winsockERR;
	case ISAO_ERR_SSL_TIMEOUT:			// 타임 아웃했다．
		fd2("---------------- select() \305\270\300\323\276\306\277\364 \275\307\306\320" );
		goto IsaoCheck_winsockERR;
	};
	//winsock 상의 에러 첵크함.  ================================== end

	//ret_chk = chk_readbuffer("account=", szReadBuffer, &iRejectCode );
	//ygy
	ret_chk = chk_readbuffer("Code=", szReadBuffer, &iRejectCode );
	switch(ret_chk)
	{
		//ygy
	case 0: //트랜잭션 오류
		break;
	case 1: //클랜원 가입성공
		strcpy( data, szReadBuffer);
		break;
	case 2: //현재 가입시키려는 클랜원이 이미 다른 클랜에 가입해있을 경우(자기 클랜포함)
		break;
	case 3: //클랜이 존재하지 않거나 클랜 장이 아닌경우	
		break;
	case 4: //클랜 멤버가 32 이상일 경우(Max : 32)
		break;
	case 100:
		break;
		/*
	case 0: 
		goto IsaoCheck_Free;
	case 1: //패스워드 불량		
		goto IsaoCheck_Free;
	case 2: //아이디가 등록되지 않았음
		goto IsaoCheck_Free;*/

	case ISAO_ERR_NONE_REEJCT:					//응답번호가 없음.
		fd2("ISAO_ERR_NONE_REEJCT  : \300\300\264\344\271\370\310\243\260\241 \276\370\300\275" );
		goto IsaoCheck_Free;
	}
	

IsaoCheck_Free:
	return ret_chk;


IsaoCheck_winsockERR:			//winsock으로 연결시에러남
	return ret_chk ;
}




/*///////////////////////////////////////////////////////////////////////////
: 함 수 명 :int ISAOCHECK::read_ClanName(int readClumns)
: 하 는 일 :
: 서    식 : 컬럼을 넣으면 해당 데이타 리턴
: 인    자 :
: 반 환 값 :
: 논    평 :  실제 사용함수 
: 버    전 :
: PROJECT  :
///////////////////////////////////////////////////////////////////////////*/
int ISAOCHECK::read_ClanName(int readClumns)
{
	char *clumns[] = {
		"name",
		"explanation",
		"date",
		"Image",
		"clanZang",
		"clanWon1",
		"clanWon2",
		"clanWon3",
		"clanWon4",
		"clanWon5",
		"clanWon6",
	};

	int ret_chk;
	int iRejectCode = 111111;
	char szWriteBuffer[BUF_LEN];
	char szReadBuffer [BUF_LEN];
	
	sprintf( szWriteBuffer,
		//"POST http://%s/tjboyh/clan_readClanName.php?columnName=%s&ticket=%d\r\n\r\n\r\n", hostName, clumns[readClumns] ,nClanTicket);
		"http://%s/tjboyh/clan_readClanName.php?columnName=%s&ticket=%d", hostName, clumns[readClumns] ,nClanTicket);
	ret_chk = connect_WEBdb( szWriteBuffer, szReadBuffer);
	
	//ktj : 임시
	fd2(" \300\245 db\277\241\274\255 \300\320\300\272\260\315 %s ", szReadBuffer);


	
	//ktj : 아래 첵크는 모두 임시적으로 운영됨

	//winsock 상의 에러 첵크함.  ================================== start
	switch(ret_chk) {
	case ISAO_ERR_WINSOCK_NOERR:		//에러 없음.
		//ktj : 임시로 서버에서 온 데이타를 뿌려봄.
		fd2( "From server: ---from--\n%s--end--\n ",szReadBuffer);			
		break;
	case ISAO_ERR_WINSOCK_START:		//윈속 START시 에러발생				//window only
		fd2("WSAStartup()\277\241\266\363");
		goto IsaoCheck_winsockERR;
	case ISAO_ERR_WINSOCK_VERSION:		//윈속버전이 틀려 구동되지못함.			//window only
		fd2("\271\366\300\374\300\314 \270\302\301\366 \276\312\300\275");
		goto IsaoCheck_winsockERR;
	case ISAO_ERR_TCP_CONNECT:			// 보통 사용되는 ＴＣＰ코네쿠토에라
		fd2( "connect() \301\242\274\323 \275\307\306\320" );
		goto IsaoCheck_winsockERR;
	case ISAO_ERR_SSL_READ:				// ＳＳＬ 리드 할 수 없었다．
		fd2(" recv()  \300\320\261\342 \275\307\306\320" );
		goto IsaoCheck_winsockERR;
	case ISAO_ERR_SSL_TIMEOUT:			// 타임 아웃했다．
		fd2("---------------- select() \305\270\300\323\276\306\277\364 \275\307\306\320" );
		goto IsaoCheck_winsockERR;
	};
	//winsock 상의 에러 첵크함.  ================================== end

	ret_chk = chk_readbuffer("account:", szReadBuffer, &iRejectCode );
	switch(ret_chk)
	{
	case 0: 
		goto IsaoCheck_Free;
	case 1: //패스워드 불량		
		goto IsaoCheck_Free;
	case 2: //아이디가 등록되지 않았음
		goto IsaoCheck_Free;

	case ISAO_ERR_NONE_REEJCT:					//응답번호가 없음.
		fd2("ISAO_ERR_NONE_REEJCT  : \300\300\264\344\271\370\310\243\260\241 \276\370\300\275" );
		goto IsaoCheck_Free;
	}
	

IsaoCheck_Free:
	return ret_chk;


IsaoCheck_winsockERR:			//winsock으로 연결시에러남
	return ret_chk ;
}






/*///////////////////////////////////////////////////////////////////////////
: 함 수 명 :int ISAOCHECK::read_ClanName(int readClumns)
: 하 는 일 :
: 서    식 : 클라이언트 이름을 넣어서 전 칼럼 출력
: 인    자 :
: 반 환 값 :
: 논    평 :  실제 사용함수 
: 버    전 :
: PROJECT  :
///////////////////////////////////////////////////////////////////////////*/
int ISAOCHECK::read_ClanData(char *ClanZang_ID,char* gserver,char* ClanZang_ChaName,char* ret_Buf)
{
	int ret_chk;
	int iRejectCode = 111111;
	char szWriteBuffer[BUF_LEN];
	char szReadBuffer [BUF_LEN];
/*
//일본클랜 클랜원전부보여줌
#ifdef _LANGUAGE_JAPANESE

	char UTF8_ClanZang_ChaName[1024];		memset(UTF8_ClanZang_ChaName,0,sizeof(UTF8_ClanZang_ChaName));
	
	MBCSToWeb(ClanZang_ChaName,		UTF8_ClanZang_ChaName,932);

	
	sprintf( szWriteBuffer,
		"http://%s/ServerMain/GetAllMyClanMember.asp?userid=%s&gserver=%s&chname=%s&ticket=%d", 
		hostName,ClanZang_ID,gserver,
		UTF8_ClanZang_ChaName,
		nClanTicket);

#else
*/
	sprintf( szWriteBuffer,
		"http://%s/ServerMain/GetAllMyClanMember.asp?userid=%s&gserver=%s&chname=%s&ticket=%d", hostName,ClanZang_ID,gserver,ClanZang_ChaName,nClanTicket);
//#endif
	
	int cnt=0;
	while(1) {		//컨넥트되도 답이 안오는경우를 위해 5번정도 재접한다.
		ret_chk = connect_WEBdb( szWriteBuffer, szReadBuffer);
		if(ret_chk != ISAO_ERR_RECONNECT) break;
		if(cnt>5) break;
		cnt++;
	}

	
	//ktj : 임시
	
	debug_fd2( "read_ClanData() \272\270\263\275\260\315 \261\346\300\314 : %d \263\273\277\353 : %s ", strlen(szWriteBuffer), szWriteBuffer);
	debug_fd2( "read_ClanData() \271\336\300\272\260\315 \261\346\300\314 : %d \263\273\277\353 : %s ", strlen(szReadBuffer), szReadBuffer);
	

	//fd2(" 웹 db에서 읽은것 %s ", szReadBuffer);
	
	//ktj : 아래 첵크는 모두 임시적으로 운영됨

	//winsock 상의 에러 첵크함.  ================================== start
	switch(ret_chk) {
	case ISAO_ERR_WINSOCK_NOERR:		//에러 없음.
		//ktj : 임시로 서버에서 온 데이타를 뿌려봄.
		fd2( "From server: ---from--\n%s--end--\n ",szReadBuffer);			
		break;
	case ISAO_ERR_WINSOCK_START:		//윈속 START시 에러발생				//window only
		fd2("WSAStartup()\277\241\266\363");
		goto IsaoCheck_winsockERR;
	case ISAO_ERR_WINSOCK_VERSION:		//윈속버전이 틀려 구동되지못함.			//window only
		fd2("\271\366\300\374\300\314 \270\302\301\366 \276\312\300\275");
		goto IsaoCheck_winsockERR;
	case ISAO_ERR_TCP_CONNECT:			// 보통 사용되는 ＴＣＰ코네쿠토에라
		fd2( "connect() \301\242\274\323 \275\307\306\320" );
		goto IsaoCheck_winsockERR;
	case ISAO_ERR_SSL_READ:				// ＳＳＬ 리드 할 수 없었다．
		fd2(" recv()  \300\320\261\342 \275\307\306\320" );
		goto IsaoCheck_winsockERR;
	case ISAO_ERR_SSL_TIMEOUT:			// 타임 아웃했다．
		fd2("---------------- select() \305\270\300\323\276\306\277\364 \275\307\306\320" );
		goto IsaoCheck_winsockERR;
	};
	//winsock 상의 에러 첵크함.  ================================== end

	//ret_chk = chk_readbuffer("account=", szReadBuffer, &iRejectCode );
	ret_chk = chk_readbuffer("Code=", szReadBuffer, &iRejectCode );
	switch(ret_chk)
	{
		//ygy
	case 0: //가입한 클랜이 없음
		break;
	case 1://정상 출력
		strcpy( ret_Buf, szReadBuffer);
		break;
	case 100:					//ok
		strcpy( ret_Buf, szReadBuffer);
	case 101:					//못읽음.
		goto IsaoCheck_Free;
		
	case ISAO_ERR_NONE_REEJCT:					//응답번호가 없음.
		fd2("ISAO_ERR_NONE_REEJCT  : \300\300\264\344\271\370\310\243\260\241 \276\370\300\275" );
		goto IsaoCheck_Free;
	}
	

IsaoCheck_Free:
	return ret_chk;


IsaoCheck_winsockERR:			//winsock으로 연결시에러남
	return ret_chk ;
}



/*///////////////////////////////////////////////////////////////////////////
: 함 수 명 :int ISAOCHECK::BreakUPClan()
: 하 는 일 :
: 서    식 : 클랜을 삭제함.
: 인    자 :
: 반 환 값 :
: 논    평 : 실제 사용함수 
: 버    전 :
: PROJECT  :
///////////////////////////////////////////////////////////////////////////*/
int ISAOCHECK::BreakUPClan(char *ClanZang_ID,char *gserver,char *ClanZang_ChaName, char *ClanName)
{
	//return -1;		//만들어야함.

	int ret_chk;
	int iRejectCode = 111111;
	char szWriteBuffer[BUF_LEN];
	char szReadBuffer [BUF_LEN];
/*
//일본클랜삭제
#ifdef _LANGUAGE_JAPANESE
	char UTF8_ClanZang_ChaName[1024];		memset(UTF8_ClanZang_ChaName,0,sizeof(UTF8_ClanZang_ChaName));
	char UTF8_ClanName[1024];				memset(UTF8_ClanName,0,sizeof(UTF8_ClanName));
	char UTF8_explanation[1024];			memset(UTF8_explanation,0,sizeof(UTF8_explanation));
	
	MBCSToWeb(ClanZang_ChaName,		UTF8_ClanZang_ChaName,932);
	MBCSToWeb(ClanName,				UTF8_ClanName,932);
	
	sprintf( szWriteBuffer,
	"http://%s/ServerMain/clanRemove.asp?userid=%s&gserver=%s&chname=%s&clName=%s&ticket=%d",
	hostName,ClanZang_ID,gserver,
		UTF8_ClanZang_ChaName, 
		UTF8_ClanName,
	nClanTicket);

#else
*/
	sprintf( szWriteBuffer,
	"http://%s/ServerMain/clanRemove.asp?userid=%s&gserver=%s&chname=%s&clName=%s&ticket=%d",
	hostName,ClanZang_ID,gserver,ClanZang_ChaName, ClanName,nClanTicket);
//#endif

	int cnt=0;
	while(1) {		//컨넥트되도 답이 안오는경우를 위해 5번정도 재접한다.
		ret_chk = connect_WEBdb( szWriteBuffer, szReadBuffer);
		if(ret_chk != ISAO_ERR_RECONNECT) break;
		if(cnt>5) break;
		cnt++;
	}

	
	//ktj : 임시
	fd2(" \300\245 db\277\241\274\255 \300\320\300\272\260\315 %s ", szReadBuffer);
	
	//ktj : 아래 첵크는 모두 임시적으로 운영됨

	//winsock 상의 에러 첵크함.  ================================== start
	switch(ret_chk) {
	case ISAO_ERR_WINSOCK_NOERR:		//에러 없음.
		//ktj : 임시로 서버에서 온 데이타를 뿌려봄.
		fd2( "From server: ---from--\n%s--end--\n ",szReadBuffer);			
		break;
	case ISAO_ERR_WINSOCK_START:		//윈속 START시 에러발생				//window only
		fd2("WSAStartup()\277\241\266\363");
		goto IsaoCheck_winsockERR;
	case ISAO_ERR_WINSOCK_VERSION:		//윈속버전이 틀려 구동되지못함.			//window only
		fd2("\271\366\300\374\300\314 \270\302\301\366 \276\312\300\275");
		goto IsaoCheck_winsockERR;
	case ISAO_ERR_TCP_CONNECT:			// 보통 사용되는 ＴＣＰ코네쿠토에라
		fd2( "connect() \301\242\274\323 \275\307\306\320" );
		goto IsaoCheck_winsockERR;
	case ISAO_ERR_SSL_READ:				// ＳＳＬ 리드 할 수 없었다．
		fd2(" recv()  \300\320\261\342 \275\307\306\320" );
		goto IsaoCheck_winsockERR;
	case ISAO_ERR_SSL_TIMEOUT:			// 타임 아웃했다．
		fd2("---------------- select() \305\270\300\323\276\306\277\364 \275\307\306\320" );
		goto IsaoCheck_winsockERR;
	};
	//winsock 상의 에러 첵크함.  ================================== end

	//ret_chk = chk_readbuffer("account=", szReadBuffer, &iRejectCode );
	ret_chk = chk_readbuffer("Code=", szReadBuffer, &iRejectCode );
	switch(ret_chk)
	{
	case 0: //트랜잭션 오류
		break;
	case 1: //삭제 성공
		break;
	case 2: //현재 캐릭명이 현재 클랜의 장이 아니거나 해당클랜이 없음
		break;
	/*case 100:					//성공
	case 101:					//clanName 테이블에 클랜원이 있는데 clanWon테이블에 없음.
	case 102:					//clanWon테이블에서 클랜원삭제 실패
	case 103:					//clanName안에 클랜이름 존재안함.
	case 104:					//clanName테이블에서 클랜삭제 실패
		goto IsaoCheck_Free;
		*/
		
	case ISAO_ERR_NONE_REEJCT:					//응답번호가 없음.
		fd2("ISAO_ERR_NONE_REEJCT  : \300\300\264\344\271\370\310\243\260\241 \276\370\300\275" );
		goto IsaoCheck_Free;
	}
	

IsaoCheck_Free:
	return ret_chk;


IsaoCheck_winsockERR:			//winsock으로 연결시에러남
	return ret_chk ;
}


/*///////////////////////////////////////////////////////////////////////////
: 함 수 명 :int ISAOCHECK::ReadNotice()
: 하 는 일 :
: 서    식 : 클랜원을 추방한다.
: 인    자 :
: 반 환 값 :
: 논    평 : 실제 사용함수 
: 버    전 :
: PROJECT  :
///////////////////////////////////////////////////////////////////////////*/

extern char szNoticeUrl[255];


/*///////////////////////////////////////////////////////////////////////////
//notice전용
///////////////////////////////////////////////////////////////////////////*/
int ISAOCHECK::connect_WEBdb_NOTICE(char *szWriteBuffer, char *szReadBuffer)
{
		char szSizeBuffer[32];
    DWORD dwLengthSizeBuffer = sizeof(szSizeBuffer);
    DWORD dwFileSize = 0;
    DWORD dwBytesRead = 0;    
    int iRet= ISAO_ERR_WINSOCK_NOERR;

    ZeroMemory(szReadBuffer,BUF_LEN);
    //int i;
 
    HINTERNET m_Session = NULL;
    m_Session = InternetOpen("AppName",  //어플리케이션의 이름
							INTERNET_OPEN_TYPE_PRECONFIG,  
							// Use registry settings.
                            NULL,
							// Proxy name. NULL indicates use default.
                            NULL,  
							// List of local servers. NULL indicates default.
                            0) ;
    if(!m_Session) 
    {
	    iRet = ISAO_ERR_TCP_CONNECT;
	    return iRet;
    }
 	HINTERNET hHttpFile = NULL;
    hHttpFile = InternetOpenUrl(m_Session, (const char *) szWriteBuffer, NULL,
								0, 0, 0);
    if (hHttpFile)
    {
		BOOL bQuery = HttpQueryInfo(hHttpFile,HTTP_QUERY_CONTENT_LENGTH, szSizeBuffer,
					    &dwLengthSizeBuffer, NULL) ;
	    //BOOL bQuery = InternetQueryDataAvailable(hHttpFile,&dwLengthSizeBuffer,0,0);
        if(bQuery==TRUE)
        {
			dwFileSize=atol(szSizeBuffer);                                                          
            dwFileSize > BUF_LEN ? dwFileSize = BUF_LEN :
			dwFileSize = dwFileSize;
 
            BOOL bRead = InternetReadFile(hHttpFile, szReadBuffer,dwFileSize, &dwBytesRead);
            if (bRead)
            {
				if((bRead == TRUE)&&(dwFileSize !=dwBytesRead))
				{			
					iRet = ISAO_ERR_RECONNECT;
				}
		
				
            }
		   else iRet = ISAO_ERR_RECONNECT;
		}
	}
    else
    {
	    iRet = ISAO_ERR_TCP_CONNECT;
	    return iRet;
    }
    if(hHttpFile) InternetCloseHandle(hHttpFile);
    if(m_Session) InternetCloseHandle(m_Session);
    return iRet;
}
/*
int ISAOCHECK::connect_WEBdb_NOTICE(char *szServerDomain, int port, char *szWriteBuffer, char *szReadBuffer)
{
	struct timeval tmv;    //timeval
	struct sockaddr_in server;
	char Value = '\0';
	char Msg[1024] = "";
	SOCKET Fd;
	
	int iLen;
	int iRet= ISAO_ERR_WINSOCK_NOERR;
	int iSelectRet;
	
    fd_set fdset;

	//-----------------------------------------------
	// 보통 사용되는 TCP 소켓 작성
	//-----------------------------------------------
	WORD         wSocketVersion;
	WSADATA      wsaData;
    int          iError;

	wSocketVersion = MAKEWORD(1,1);
	iError = WSAStartup(wSocketVersion,&wsaData);
	if(iError != 0)
	{
		return ISAO_ERR_WINSOCK_START;
	}
	if(HIBYTE(wsaData.wVersion) != 1 || LOBYTE(wsaData.wVersion) != 1)
	{
		WSACleanup();
		return ISAO_ERR_WINSOCK_VERSION;
	}	



	
	PHOSTENT phe;
	char szIP[16];
	in_addr  ia;
	int cnt,len;
	// 도메인이 IP주소인지 확인
	len = lstrlen(szServerDomain);
	for(cnt=0;cnt<len;cnt++) {
		if ( szServerDomain[cnt]!='.' && (szServerDomain[cnt]<'0' || szServerDomain[cnt]>'9') ) break;
	}
	if ( cnt<len ) {
		//DNS 구하기
		phe = gethostbyname( szServerDomain );
		if ( phe ) {
			ia.S_un.S_addr = ((unsigned long *)phe->h_addr)[0];
			lstrcpy( szIP , inet_ntoa (ia) );
		}
		else
			return FALSE;
	}
	else {
		//도메인이 IP로 확인
		lstrcpy( szIP , szServerDomain );
	}



	Fd = socket (AF_INET,SOCK_STREAM,0);
	memset(&server,0, sizeof(server));
	server.sin_family = AF_INET;
	server.sin_port = htons( port);					//클라스내에서 셋팅 
	//server.sin_addr.s_addr = inet_addr(severIP);		//hostPORT hostName
	server.sin_addr.S_un.S_addr = inet_addr ( szIP );

	//connect
	if (connect( Fd, (struct sockaddr *)&server,sizeof(server)) == -1){
		return ISAO_ERR_TCP_CONNECT;
	}


   	
	//ktj : 임시
	//fd2( "send( %s )", szWriteBuffer  );
	send(Fd,szWriteBuffer,strlen(szWriteBuffer)+1,0);

	tmv.tv_sec = iTimeOut;		// 보낸후 대기시간만큼만 답변을 기다림
	tmv.tv_usec = 0;
	FD_ZERO( &fdset );			// read select
    FD_SET( Fd, &fdset );		// fd의 플래그 셋팅
	iSelectRet = select( Fd + 1, &fdset, (fd_set*)NULL,(fd_set*)NULL,&tmv);		// 답변을 받는동안 계속 대기함.

	if( iSelectRet > 0 && FD_ISSET( Fd, &fdset ) ){		// 성공했다

	}else{												// 실패했다
		iRet = ISAO_ERR_SSL_TIMEOUT;
		goto IsaoCheck_Free;
	}




	ZeroMemory(szReadBuffer,sizeof(szReadBuffer));
	long startTime, endTime ;
	time( &startTime  );

	int rcnt=0;
	while(1) {
		iLen = recv(Fd,szReadBuffer,BUF_LEN-1,0);		//실제 읽는다.
		//iLen = recv(Fd,szReadBuffer,20,0);		//실제 읽는다.
		
		if(iLen==0) {
			Sleep(100);
		}
		else if(iLen == SOCKET_ERROR)				// error
		{
	        int error = WSAGetLastError();
			if(error==10054)    {
				iRet = ISAO_ERR_SSL_TIMEOUT;
				goto IsaoCheck_Free;
			}
		}
		else {					//데이타받았으므로 끝낸다.
			break;
		}

		time( &endTime);
		if( (endTime-startTime) > 2) {		//2초정도기다려서 답이안오면 리컨넥하도록함.
			iRet = ISAO_ERR_RECONNECT;
			goto IsaoCheck_Free;
		}
	}


	if( iLen < 0 ){
		iRet = ISAO_ERR_SSL_READ;				// 읽을 수 없었다．실패이다．

		
		goto IsaoCheck_Free;
	}
	szReadBuffer[iLen] = 0;
	
	//디버그용으로 표시함.
	//fd2( "\nFrom server: ---from--\n%s--end--\n 읽은 길이는 %d ",szReadBuffer, iLen );
	
IsaoCheck_Free:

	closesocket( Fd );
	fd2("\n 웹db 접근 끝남." );

	return iRet;
}
*/

int ConvertURL( char *szURL , char *szServer , char *szPath )
{
	int  cnt,len;
	char *lpServer=0;
	char *lpPath=0;
	char szbuff[256];

	lstrcpy( szbuff, szURL );

	len = lstrlen( szbuff );

	for(cnt=0;cnt<len;cnt++) {
		//(http://)
		if( !lpServer && szbuff[cnt]=='/' && szbuff[cnt+1]=='/' ) {
			lpServer = &szbuff[cnt+2];
			cnt+=2;
		}

		if ( !lpPath && szbuff[cnt]=='/' && szbuff[cnt+1]!='/' ) {
			lpPath = &szbuff[cnt];
		}
	}

	if ( lpPath ) {
		if ( szPath ) 
			lstrcpy( szPath , lpPath );
		lpPath[0] = 0;
	}
	else
		if ( szPath ) szPath[0]=0;

	if ( szServer ) {
		if ( lpServer ) {
			lstrcpy( szServer , lpServer );
		}
		else {
			lstrcpy( szServer , szbuff );
		}
	}

	return TRUE;
}


int ISAOCHECK::ReadNotice(char *userid,char *chaname,char *gserver,char *buf)
{
	int ret_chk;
	int iRejectCode = 111111;
	char szWriteBuffer[BUF_LEN];
	char szReadBuffer [BUF_LEN];


	char szServer[256], szPath[256];
	ConvertURL( szNoticeUrl, szServer , szPath );

	sprintf( szWriteBuffer,
		"%s?userid=%s&gserver=%s&chname=%s&ticket=%d\r\n\r\n\r\n", szNoticeUrl,userid,gserver,chaname,nClanTicket);

	//sprintf( szWriteBuffer,
	//	"POST %s?userid=%s&gserver=%s&chname=%s\r\n\r\n\r\n", szNoticeUrl, "yuri", "테스트서버", "유리");

	int cnt=0;
	while(1) {		//컨넥트되도 답이 안오는경우를 위해 5번정도 재접한다.
		//ret_chk = connect_WEBdb_NOTICE( szServer, 80, szWriteBuffer, szReadBuffer);
		ret_chk = connect_WEBdb_NOTICE(szWriteBuffer, szReadBuffer);
		if(ret_chk != ISAO_ERR_RECONNECT) break;		
		if(cnt>5) break;
		cnt++;
	}

	
	//ktj : 임시
	
	debug_fd2( "ReadNotice() \272\270\263\275\260\315 \261\346\300\314 : %d(ret_chk %d) \263\273\277\353 : %s ", strlen(szWriteBuffer), ret_chk, szWriteBuffer);
	debug_fd2( "ReadNotice() \271\336\300\272\260\315 \261\346\300\314 : %d \263\273\277\353 : %s ", strlen(szReadBuffer), szReadBuffer);
	



	//winsock 상의 에러 첵크함.  ================================== start
	switch(ret_chk) {
	case ISAO_ERR_WINSOCK_NOERR:		//에러 없음.
		//ktj : 임시로 서버에서 온 데이타를 뿌려봄.
		fd2( "From server: ---from--\n%s--end--\n ",szReadBuffer);			
		break;
	case ISAO_ERR_WINSOCK_START:		//윈속 START시 에러발생				//window only
		fd2("WSAStartup()\277\241\266\363");
		goto IsaoCheck_winsockERR;
	case ISAO_ERR_WINSOCK_VERSION:		//윈속버전이 틀려 구동되지못함.			//window only
		fd2("\271\366\300\374\300\314 \270\302\301\366 \276\312\300\275");
		goto IsaoCheck_winsockERR;
	case ISAO_ERR_TCP_CONNECT:			// 보통 사용되는 ＴＣＰ코네쿠토에라
		fd2( "connect() \301\242\274\323 \275\307\306\320" );
		goto IsaoCheck_winsockERR;
	case ISAO_ERR_SSL_READ:				// ＳＳＬ 리드 할 수 없었다．
		fd2(" recv()  \300\320\261\342 \275\307\306\320" );
		goto IsaoCheck_winsockERR;
	case ISAO_ERR_SSL_TIMEOUT:			// 타임 아웃했다．
		fd2("---------------- select() \305\270\300\323\276\306\277\364 \275\307\306\320" );
		goto IsaoCheck_winsockERR;
	};
	//winsock 상의 에러 첵크함.  ================================== end

	//ret_chk = chk_readbuffer("account=", szReadBuffer, &iRejectCode );
	//ygy
	ret_chk = chk_readbuffer("Code=", szReadBuffer, &iRejectCode );
	
	//for(int index = 0; index < 10; index++)
		//strcat(szReadBuffer, "skjdflsdklfjlksdjf\n");

	switch(ret_chk)
	{
		//ygy
	case 0: //트랜잭션 오류
		break;
	case 1:
		strcpy( buf, szReadBuffer);
		break;
	case 2: 
		break;
	case 3: 
		break;
	case 100:					//성공
	case 101:					//그런클랜이름없음.
	case 102:					//clanName에 추방 유저 업데이트 실패
	case 103:					//clanWon에 접근 실패
		goto IsaoCheck_Free;
		
	case ISAO_ERR_NONE_REEJCT:					//응답번호가 없음.
		fd2("ISAO_ERR_NONE_REEJCT  : \300\300\264\344\271\370\310\243\260\241 \276\370\300\275" );
		goto IsaoCheck_Free;
	}
	

IsaoCheck_Free:
	return ret_chk;


IsaoCheck_winsockERR:			//winsock으로 연결시에러남
	return ret_chk ;
}
/*///////////////////////////////////////////////////////////////////////////
: 함 수 명 :int ISAOCHECK::Banishiment()
: 하 는 일 :
: 서    식 : 클랜원을 추방한다.
: 인    자 :
: 반 환 값 :
: 논    평 : 실제 사용함수 
: 버    전 :
: PROJECT  :
///////////////////////////////////////////////////////////////////////////*/
int ISAOCHECK::Banishiment(char *ClanZang_ID,char *gserver,char *ClanZang_ChaName,char *ClanName, char *ClanWon)
{
	int ret_chk;
	int iRejectCode = 111111;
	char szWriteBuffer[BUF_LEN];
	char szReadBuffer [BUF_LEN];
/*
//일본클랜원추방
#ifdef _LANGUAGE_JAPANESE

	char UTF8_ClanZang_ChaName[1024];		memset(UTF8_ClanZang_ChaName,0,sizeof(UTF8_ClanZang_ChaName));
	char UTF8_ClanName[1024];				memset(UTF8_ClanName,0,sizeof(UTF8_ClanName));
	char UTF8_ClanWon[1024];			memset(UTF8_ClanWon,0,sizeof(UTF8_ClanWon));
	
	MBCSToWeb(ClanZang_ChaName,		UTF8_ClanZang_ChaName,932);
	MBCSToWeb(ClanName,				UTF8_ClanName,932);
	MBCSToWeb(ClanWon,				UTF8_ClanWon,932);
	
	sprintf( szWriteBuffer,
	"http://%s/ServerMain/clanWonRelease.asp?userid=%s&gserver=%s&chname=%s&clName=%s&%s&ticket=%d",
	hostName,ClanZang_ID,gserver,
	UTF8_ClanZang_ChaName,
	UTF8_ClanName,
	UTF8_ClanWon,
	nClanTicket);
#else
*/
	sprintf( szWriteBuffer,
	"http://%s/ServerMain/clanWonRelease.asp?userid=%s&gserver=%s&chname=%s&clName=%s&%s&ticket=%d", hostName,ClanZang_ID,gserver,ClanZang_ChaName,ClanName,ClanWon,nClanTicket);
//#endif
		

	
	int cnt=0;
	while(1) {		//컨넥트되도 답이 안오는경우를 위해 5번정도 재접한다.
		ret_chk = connect_WEBdb( szWriteBuffer, szReadBuffer);
		if(ret_chk != ISAO_ERR_RECONNECT) break;
		if(cnt>5) break;
		cnt++;
	}

	
	//ktj : 임시
	
	debug_fd2( "Banishiment() \272\270\263\275\260\315 \261\346\300\314 : %d(ret_chk %d) \263\273\277\353 : %s ", strlen(szWriteBuffer), ret_chk, szWriteBuffer);
	debug_fd2( "Banishiment() \271\336\300\272\260\315 \261\346\300\314 : %d \263\273\277\353 : %s ", strlen(szReadBuffer), szReadBuffer);
	



	//winsock 상의 에러 첵크함.  ================================== start
	switch(ret_chk) {
	case ISAO_ERR_WINSOCK_NOERR:		//에러 없음.
		//ktj : 임시로 서버에서 온 데이타를 뿌려봄.
		fd2( "From server: ---from--\n%s--end--\n ",szReadBuffer);			
		break;
	case ISAO_ERR_WINSOCK_START:		//윈속 START시 에러발생				//window only
		fd2("WSAStartup()\277\241\266\363");
		goto IsaoCheck_winsockERR;
	case ISAO_ERR_WINSOCK_VERSION:		//윈속버전이 틀려 구동되지못함.			//window only
		fd2("\271\366\300\374\300\314 \270\302\301\366 \276\312\300\275");
		goto IsaoCheck_winsockERR;
	case ISAO_ERR_TCP_CONNECT:			// 보통 사용되는 ＴＣＰ코네쿠토에라
		fd2( "connect() \301\242\274\323 \275\307\306\320" );
		goto IsaoCheck_winsockERR;
	case ISAO_ERR_SSL_READ:				// ＳＳＬ 리드 할 수 없었다．
		fd2(" recv()  \300\320\261\342 \275\307\306\320" );
		goto IsaoCheck_winsockERR;
	case ISAO_ERR_SSL_TIMEOUT:			// 타임 아웃했다．
		fd2("---------------- select() \305\270\300\323\276\306\277\364 \275\307\306\320" );
		goto IsaoCheck_winsockERR;
	};
	//winsock 상의 에러 첵크함.  ================================== end

	//ret_chk = chk_readbuffer("account=", szReadBuffer, &iRejectCode );
	//ygy
	ret_chk = chk_readbuffer("Code=", szReadBuffer, &iRejectCode );
	switch(ret_chk)
	{
		//ygy
	case 0: //트랜잭션 오류
		break;
	case 1: //클랜원 탈퇴 성공
		break;
	case 2: //클랜이 존재하지 않거나 클랜 장이 아닌경우 또는 클랜원이 아닌경우
		break;
	case 3: //탈퇴는 성공했으나 클랜회원수가 6명이하로 떨어졌을 경우(LimitDate 설정)
		break;
	case 100:					//성공
	case 101:					//그런클랜이름없음.
	case 102:					//clanName에 추방 유저 업데이트 실패
	case 103:					//clanWon에 접근 실패
		goto IsaoCheck_Free;
		
	case ISAO_ERR_NONE_REEJCT:					//응답번호가 없음.
		fd2("ISAO_ERR_NONE_REEJCT  : \300\300\264\344\271\370\310\243\260\241 \276\370\300\275" );
		goto IsaoCheck_Free;
	}
	

IsaoCheck_Free:
	return ret_chk;


IsaoCheck_winsockERR:			//winsock으로 연결시에러남
	return ret_chk ;
}





//부클랜장 임명==================================================================
int ISAOCHECK::AppointSubChip(char *gserver,char* clanwon)
{
	int ret_chk;
	int iRejectCode = 111111;
	char szWriteBuffer[BUF_LEN];
	char szReadBuffer [BUF_LEN];

/*
//일본클랜부클랜장임명
#ifdef _LANGUAGE_JAPANESE

	char UTF8_clanwon[1024];		memset(UTF8_clanwon,0,sizeof(UTF8_clanwon));
	MBCSToWeb(clanwon,		UTF8_clanwon,932);

	
	sprintf( szWriteBuffer,
	"http://%s/ServerMain/ClanSubChipUpdate.asp?chname=%s&gserver=%s&ticket=%d", hostName,
	UTF8_clanwon,
	gserver,9999);//nClanTicket);

#else
*/
	sprintf( szWriteBuffer,
	"http://%s/ServerMain/ClanSubChipUpdate.asp?chname=%s&gserver=%s&ticket=%d", hostName,clanwon,gserver,9999);//nClanTicket);
//#endif
	
	int cnt=0;
	while(1) {		//컨넥트되도 답이 안오는경우를 위해 5번정도 재접한다.
		ret_chk = connect_WEBdb( szWriteBuffer, szReadBuffer);
		if(ret_chk != ISAO_ERR_RECONNECT) break;
		if(cnt>5) break;
		cnt++;
	}

	
	//ktj : 임시
	debug_fd2( "AppointSubChip() \272\270\263\275\260\315 \261\346\300\314 : %d \263\273\277\353 : %s ", strlen(szWriteBuffer), szWriteBuffer);
	debug_fd2( "AppointSubChip() \271\336\300\272\260\315 \261\346\300\314 : %d \263\273\277\353 : %s ", strlen(szReadBuffer), szReadBuffer);


	//ktj : 아래 첵크는 모두 임시적으로 운영됨

	//winsock 상의 에러 첵크함.  ================================== start
	switch(ret_chk) {
	case ISAO_ERR_WINSOCK_NOERR:		//에러 없음.
		//ktj : 임시로 서버에서 온 데이타를 뿌려봄.
		fd2( "From server: ---from--\n%s--end--\n ",szReadBuffer);			
		break;
	case ISAO_ERR_WINSOCK_START:		//윈속 START시 에러발생				//window only
		fd2("WSAStartup()\277\241\266\363");
		goto IsaoCheck_winsockERR;
	case ISAO_ERR_WINSOCK_VERSION:		//윈속버전이 틀려 구동되지못함.			//window only
		fd2("\271\366\300\374\300\314 \270\302\301\366 \276\312\300\275");
		goto IsaoCheck_winsockERR;
	case ISAO_ERR_TCP_CONNECT:			// 보통 사용되는 ＴＣＰ코네쿠토에라
		fd2( "connect() \301\242\274\323 \275\307\306\320" );
		goto IsaoCheck_winsockERR;
	case ISAO_ERR_SSL_READ:				// ＳＳＬ 리드 할 수 없었다．
		fd2(" recv()  \300\320\261\342 \275\307\306\320" );
		goto IsaoCheck_winsockERR;
	case ISAO_ERR_SSL_TIMEOUT:			// 타임 아웃했다．
		fd2("---------------- select() \305\270\300\323\276\306\277\364 \275\307\306\320" );
		goto IsaoCheck_winsockERR;
	}
	//winsock 상의 에러 첵크함.  ================================== end

	//ret_chk = chk_readbuffer("account=", szReadBuffer, &iRejectCode );
	//ygy
	ret_chk = chk_readbuffer("Code=", szReadBuffer, &iRejectCode );
	switch(ret_chk)
	{
		//ygy	
	case 0:  //클랜원이 아님
		break;
	case 2:	//클랜칩일때
		break;
	case 1:	//부클랜칩 임명 성공
		break;	
	case 3: //부클랜칩이 존재함
		break;
	case 100:					//성공
	case 101:					//그런클랜이름없음.
	case 102:					//clanName에 추방 유저 업데이트 실패
	case 103:					//clanWon에 접근 실패
		goto IsaoCheck_Free;
		
	case ISAO_ERR_NONE_REEJCT:					//응답번호가 없음.
		fd2("ISAO_ERR_NONE_REEJCT  : \300\300\264\344\271\370\310\243\260\241 \276\370\300\275" );
		goto IsaoCheck_Free;
	}
	

IsaoCheck_Free:
	return ret_chk;


IsaoCheck_winsockERR:			//winsock으로 연결시에러남
	return ret_chk ;
}

//부클랜장 해임==================================================================
int ISAOCHECK::ReleaseSubChip(char *gserver,char* clanwon)
{
	int ret_chk;
	int iRejectCode = 111111;
	char szWriteBuffer[BUF_LEN];
	char szReadBuffer [BUF_LEN];

/*
//일본클랜부클랜장해임
#ifdef _LANGUAGE_JAPANESE

	char UTF8_clanwon[1024];		memset(UTF8_clanwon,0,sizeof(UTF8_clanwon));
	MBCSToWeb(clanwon,		UTF8_clanwon,932);

	
	sprintf( szWriteBuffer,
		"http://%s/ServerMain/ClanSubChipRelease.asp?chname=%s&gserver=%s&ticket=%d", 
		hostName,
		UTF8_clanwon,
		gserver,9999);//nClanTicket);

#else
*/
	sprintf( szWriteBuffer,
		"http://%s/ServerMain/ClanSubChipRelease.asp?chname=%s&gserver=%s&ticket=%d", hostName,clanwon,gserver,9999);//nClanTicket);
//#endif
	
	int cnt=0;
	while(1) {		//컨넥트되도 답이 안오는경우를 위해 5번정도 재접한다.
		ret_chk = connect_WEBdb( szWriteBuffer, szReadBuffer);
		if(ret_chk != ISAO_ERR_RECONNECT) break;
		if(cnt>5) break;
		cnt++;
	}

	
	//ktj : 임시
	debug_fd2( "ReleaseSubChip() \272\270\263\275\260\315 \261\346\300\314 : %d \263\273\277\353 : %s ", strlen(szWriteBuffer), szWriteBuffer);
	debug_fd2( "ReleaseSubChip() \271\336\300\272\260\315 \261\346\300\314 : %d \263\273\277\353 : %s ", strlen(szReadBuffer), szReadBuffer);


	//ktj : 아래 첵크는 모두 임시적으로 운영됨

	//winsock 상의 에러 첵크함.  ================================== start
	switch(ret_chk) {
	case ISAO_ERR_WINSOCK_NOERR:		//에러 없음.
		//ktj : 임시로 서버에서 온 데이타를 뿌려봄.
		fd2( "From server: ---from--\n%s--end--\n ",szReadBuffer);			
		break;
	case ISAO_ERR_WINSOCK_START:		//윈속 START시 에러발생				//window only
		fd2("WSAStartup()\277\241\266\363");
		goto IsaoCheck_winsockERR;
	case ISAO_ERR_WINSOCK_VERSION:		//윈속버전이 틀려 구동되지못함.			//window only
		fd2("\271\366\300\374\300\314 \270\302\301\366 \276\312\300\275");
		goto IsaoCheck_winsockERR;
	case ISAO_ERR_TCP_CONNECT:			// 보통 사용되는 ＴＣＰ코네쿠토에라
		fd2( "connect() \301\242\274\323 \275\307\306\320" );
		goto IsaoCheck_winsockERR;
	case ISAO_ERR_SSL_READ:				// ＳＳＬ 리드 할 수 없었다．
		fd2(" recv()  \300\320\261\342 \275\307\306\320" );
		goto IsaoCheck_winsockERR;
	case ISAO_ERR_SSL_TIMEOUT:			// 타임 아웃했다．
		fd2("---------------- select() \305\270\300\323\276\306\277\364 \275\307\306\320" );
		goto IsaoCheck_winsockERR;
	}
	//winsock 상의 에러 첵크함.  ================================== end

	//ret_chk = chk_readbuffer("account=", szReadBuffer, &iRejectCode );
	//ygy
	ret_chk = chk_readbuffer("Code=", szReadBuffer, &iRejectCode );
	switch(ret_chk)
	{
		//ygy	
	case 0: //클랜원이 아님
		break;
	case 2: //클랜칩이거나 부클랜
		break;
	case 1: //해임성공
		break;
	
	case 100:					//성공
	case 101:					//그런클랜이름없음.
	case 102:					//clanName에 추방 유저 업데이트 실패
	case 103:					//clanWon에 접근 실패
		goto IsaoCheck_Free;
		
	case ISAO_ERR_NONE_REEJCT:					//응답번호가 없음.
		fd2("ISAO_ERR_NONE_REEJCT  : \300\300\264\344\271\370\310\243\260\241 \276\370\300\275" );
		goto IsaoCheck_Free;
	}
	

IsaoCheck_Free:
	return ret_chk;


IsaoCheck_winsockERR:			//winsock으로 연결시에러남
	return ret_chk ;
}

//이임==============================================================
int ISAOCHECK::LeaveClan(char *clanname,char *chaname,char *gserver)
{
	int ret_chk;
	int iRejectCode = 111111;
	char szWriteBuffer[BUF_LEN];
	char szReadBuffer [BUF_LEN];
/*
//일본클랜클랜탈퇴
#ifdef _LANGUAGE_JAPANESE
	char UTF8_chaname[1024];				memset(UTF8_chaname,0,sizeof(UTF8_chaname));
	char UTF8_clanname[1024];			memset(UTF8_clanname,0,sizeof(UTF8_clanname));
	

	MBCSToWeb(chaname,				UTF8_chaname,932);
	MBCSToWeb(clanname,			UTF8_clanname,932);
	
	sprintf( szWriteBuffer,
		"http://%s/ServerMain/clanChipChange.asp?chname=%s&gserver=%s&clName=%s&ticket=%d", 
		hostName,
		UTF8_chaname,
		gserver,
		UTF8_clanname,
		nClanTicket);

#else
*/
	sprintf( szWriteBuffer,
		"http://%s/ServerMain/clanChipChange.asp?chname=%s&gserver=%s&clName=%s&ticket=%d", hostName,chaname,gserver,clanname,nClanTicket);
//#endif
	
	int cnt=0;
	while(1) {		//컨넥트되도 답이 안오는경우를 위해 5번정도 재접한다.
		ret_chk = connect_WEBdb( szWriteBuffer, szReadBuffer);
		if(ret_chk != ISAO_ERR_RECONNECT) break;
		if(cnt>5) break;
		cnt++;
	}

	
	//ktj : 임시
	debug_fd2( "LeaveClan() \272\270\263\275\260\315 \261\346\300\314 : %d \263\273\277\353 : %s ", strlen(szWriteBuffer), szWriteBuffer);
	debug_fd2( "LeaveClan() \271\336\300\272\260\315 \261\346\300\314 : %d \263\273\277\353 : %s ", strlen(szReadBuffer), szReadBuffer);


	//ktj : 아래 첵크는 모두 임시적으로 운영됨

	//winsock 상의 에러 첵크함.  ================================== start
	switch(ret_chk) {
	case ISAO_ERR_WINSOCK_NOERR:		//에러 없음.
		//ktj : 임시로 서버에서 온 데이타를 뿌려봄.
		fd2( "From server: ---from--\n%s--end--\n ",szReadBuffer);			
		break;
	case ISAO_ERR_WINSOCK_START:		//윈속 START시 에러발생				//window only
		fd2("WSAStartup()\277\241\266\363");
		goto IsaoCheck_winsockERR;
	case ISAO_ERR_WINSOCK_VERSION:		//윈속버전이 틀려 구동되지못함.			//window only
		fd2("\271\366\300\374\300\314 \270\302\301\366 \276\312\300\275");
		goto IsaoCheck_winsockERR;
	case ISAO_ERR_TCP_CONNECT:			// 보통 사용되는 ＴＣＰ코네쿠토에라
		fd2( "connect() \301\242\274\323 \275\307\306\320" );
		goto IsaoCheck_winsockERR;
	case ISAO_ERR_SSL_READ:				// ＳＳＬ 리드 할 수 없었다．
		fd2(" recv()  \300\320\261\342 \275\307\306\320" );
		goto IsaoCheck_winsockERR;
	case ISAO_ERR_SSL_TIMEOUT:			// 타임 아웃했다．
		fd2("---------------- select() \305\270\300\323\276\306\277\364 \275\307\306\320" );
		goto IsaoCheck_winsockERR;
	}
	//winsock 상의 에러 첵크함.  ================================== end

	//ret_chk = chk_readbuffer("account=", szReadBuffer, &iRejectCode );
	//ygy
	ret_chk = chk_readbuffer("Code=", szReadBuffer, &iRejectCode );
	switch(ret_chk)
	{
		//ygy	
	case 1: //변경 성공
		break;
	case 2: //클랜원이 아님
		break;
	case 3: //존재하지않는 클랜
		break;
	case 4://레벨50미만
		break;
	case 5://권한 자격만 성공
		break;
	case 6: //지정캐릭터이ㅡ 포용 최대인원수 보다 현재 인원수가 큼
		break;
	case 100:					//성공
	case 101:					//그런클랜이름없음.
	case 102:					//clanName에 추방 유저 업데이트 실패
	case 103:					//clanWon에 접근 실패
		goto IsaoCheck_Free;
		
	case ISAO_ERR_NONE_REEJCT:					//응답번호가 없음.
		fd2("ISAO_ERR_NONE_REEJCT  : \300\300\264\344\271\370\310\243\260\241 \276\370\300\275" );
		goto IsaoCheck_Free;
	}
	

IsaoCheck_Free:
	return ret_chk;


IsaoCheck_winsockERR:			//winsock으로 연결시에러남
	return ret_chk ;
}





/*///////////////////////////////////////////////////////////////////////////
: 함 수 명 :int ISAOCHECK::Secession()
: 하 는 일 :
: 서    식 : 클랜원을 탈퇴한다.
: 인    자 :
: 반 환 값 :
: 논    평 : 실제 사용함수 
: 버    전 :
: PROJECT  :
///////////////////////////////////////////////////////////////////////////*/
int ISAOCHECK::Secession(char* clanzang,char* gserver,char* SecessionID,char* clanname)
{
	int ret_chk;
	int iRejectCode = 111111;
	char szWriteBuffer[BUF_LEN];
	char szReadBuffer [BUF_LEN];
/*
//일본클랜탈퇴
#ifdef _LANGUAGE_JAPANESE

	char UTF8_clanzang[1024];			memset(UTF8_clanzang,0,sizeof(UTF8_clanzang));
	char UTF8_SecessionID[1024];		memset(UTF8_SecessionID,0,sizeof(UTF8_SecessionID));
	char UTF8_clanname[1024];			memset(UTF8_clanname,0,sizeof(UTF8_clanname));
	
	MBCSToWeb(clanzang,			UTF8_clanzang,932);
	MBCSToWeb(SecessionID,		UTF8_SecessionID,932);
	MBCSToWeb(clanname,			UTF8_clanname,932);


	sprintf( szWriteBuffer,
		"http://%s/ServerMain/clanWonSelfLeave.asp?userid=%s&gserver=%s&chname=%s&clName=%s&ticket=%d",
		hostName,
		UTF8_clanzang,
		gserver,
		UTF8_SecessionID,
		UTF8_clanname,
		nClanTicket);

#else
*/
	sprintf( szWriteBuffer,
		"http://%s/ServerMain/clanWonSelfLeave.asp?userid=%s&gserver=%s&chname=%s&clName=%s&ticket=%d",
		hostName,clanzang,gserver,SecessionID,clanname,nClanTicket);
//#endif

	int cnt=0;
	while(1) {		//컨넥트되도 답이 안오는경우를 위해 5번정도 재접한다.
		ret_chk = connect_WEBdb( szWriteBuffer, szReadBuffer);
		if(ret_chk != ISAO_ERR_RECONNECT) break;
		if(cnt>5) break;
		cnt++;
	}

	
	//ktj : 임시
	fd2("Secession() \300\245 db\277\241\274\255 \300\320\300\272\260\315 %s ", szReadBuffer);
	//ktj : 아래 첵크는 모두 임시적으로 운영됨

	//winsock 상의 에러 첵크함.  ================================== start
	switch(ret_chk) {
	case ISAO_ERR_WINSOCK_NOERR:		//에러 없음.
		//ktj : 임시로 서버에서 온 데이타를 뿌려봄.
		fd2( "From server: ---from--\n%s--end--\n ",szReadBuffer);			
		break;
	case ISAO_ERR_WINSOCK_START:		//윈속 START시 에러발생				//window only
		fd2("WSAStartup()\277\241\266\363");
		goto IsaoCheck_winsockERR;
	case ISAO_ERR_WINSOCK_VERSION:		//윈속버전이 틀려 구동되지못함.			//window only
		fd2("\271\366\300\374\300\314 \270\302\301\366 \276\312\300\275");
		goto IsaoCheck_winsockERR;
	case ISAO_ERR_TCP_CONNECT:			// 보통 사용되는 ＴＣＰ코네쿠토에라
		fd2( "connect() \301\242\274\323 \275\307\306\320" );
		goto IsaoCheck_winsockERR;
	case ISAO_ERR_SSL_READ:				// ＳＳＬ 리드 할 수 없었다．
		fd2(" recv()  \300\320\261\342 \275\307\306\320" );
		goto IsaoCheck_winsockERR;
	case ISAO_ERR_SSL_TIMEOUT:			// 타임 아웃했다．
		fd2("---------------- select() \305\270\300\323\276\306\277\364 \275\307\306\320" );
		goto IsaoCheck_winsockERR;
	}
	//winsock 상의 에러 첵크함.  ================================== end

	//ret_chk = chk_readbuffer("account=", szReadBuffer, &iRejectCode );
	//ygy
	ret_chk = chk_readbuffer("Code=", szReadBuffer, &iRejectCode );
	switch(ret_chk)
	{
		//ygy
	case 0: //트랜잭션 오류
		break;
	case 1: //클랜원 탈퇴 성공
		break;
	case 2: //클랜이 존재하지 않거나 클랜 장이 아닌경우 또는 클랜원이 아닌경우
		break;
	case 3: //탈퇴는 성공했으나 클랜회원수가 6명이하로 떨어졌을 경우(LimitDate 설정)
		break;
	case 4://탈퇴하려는 자가 현재 클랜의 클랜장일 경우(오류)
		break;
	case 100:					//성공
	case 101:					//그런클랜이름없음.
	case 102:					//clanName에 추방 유저 업데이트 실패
	case 103:					//clanWon에 접근 실패
		goto IsaoCheck_Free;
		
	case ISAO_ERR_NONE_REEJCT:					//응답번호가 없음.
		fd2("ISAO_ERR_NONE_REEJCT  : \300\300\264\344\271\370\310\243\260\241 \276\370\300\275" );
		goto IsaoCheck_Free;
	}
	

IsaoCheck_Free:
	return ret_chk;


IsaoCheck_winsockERR:			//winsock으로 연결시에러남
	return ret_chk ;
}


/*///////////////////////////////////////////////////////////////////////////
: 함 수 명 :int 
: 하 는 일 :
: 서    식 : 
: 인    자 :
: 반 환 값 :
: 논    평 : 이임 관련 플래그 
: 버    전 :
: PROJECT  :
///////////////////////////////////////////////////////////////////////////*/
int ISAOCHECK::isKPFlag(char *userid,char *clanname, char *chaname,char *gserver,char *pflag, char *gunbun)
{
	int ret_chk;
	int iRejectCode = 111111;
	char szWriteBuffer[BUF_LEN];
	char szReadBuffer [BUF_LEN];
/*
//일본클랜KPFlag
#ifdef _LANGUAGE_JAPANESE

	char UTF8_clannamee[1024];			memset(UTF8_clannamee,0,sizeof(UTF8_clannamee));
	char UTF8_chaname[1024];			memset(UTF8_chaname,0,sizeof(UTF8_chaname));

	MBCSToWeb(clanname,			UTF8_clannamee,932);
	MBCSToWeb(chaname,			UTF8_chaname,932);
	
	sprintf( szWriteBuffer,
		"http://%s/ServerMain/isKflag.asp?userid=%s&clName=%s&chName=%s&gserver=%s&KFlag=%s&Gubun=%s&ticket=%d",
		 hostName,userid,
		 UTF8_clannamee, 
		 UTF8_chaname,
		 gserver,pflag, gunbun,nClanTicket);

#else
*/
	sprintf( szWriteBuffer,
		"http://%s/ServerMain/isKflag.asp?userid=%s&clName=%s&chName=%s&gserver=%s&KFlag=%s&Gubun=%s&ticket=%d",
		 hostName,userid,clanname, chaname,gserver,pflag, gunbun,nClanTicket);
//#endif
	
	int cnt=0;
	while(1) {		//컨넥트되도 답이 안오는경우를 위해 5번정도 재접한다.
		ret_chk = connect_WEBdb( szWriteBuffer, szReadBuffer);
		if(ret_chk != ISAO_ERR_RECONNECT) break;
		if(cnt>5) break;
		cnt++;
	}

	
	//ktj : 임시
	
	debug_fd2( "isPFlag() \272\270\263\275\260\315 \261\346\300\314 : %d \263\273\277\353 : %s ", strlen(szWriteBuffer), szWriteBuffer);
	debug_fd2( "isPFlag() \271\336\300\272\260\315 \261\346\300\314 : %d \263\273\277\353 : %s ", strlen(szReadBuffer), szReadBuffer);
	

	//ktj : 아래 첵크는 모두 임시적으로 운영됨

	//winsock 상의 에러 첵크함.  ================================== start
	switch(ret_chk) {
	case ISAO_ERR_WINSOCK_NOERR:		//에러 없음.
		//ktj : 임시로 서버에서 온 데이타를 뿌려봄.
		fd2( "From server: ---from--\n%s--end--\n ",szReadBuffer);			
		break;
	case ISAO_ERR_WINSOCK_START:		//윈속 START시 에러발생				//window only
		fd2("WSAStartup()\277\241\266\363");
		goto IsaoCheck_winsockERR;
	case ISAO_ERR_WINSOCK_VERSION:		//윈속버전이 틀려 구동되지못함.			//window only
		fd2("\271\366\300\374\300\314 \270\302\301\366 \276\312\300\275");
		goto IsaoCheck_winsockERR;
	case ISAO_ERR_TCP_CONNECT:			// 보통 사용되는 ＴＣＰ코네쿠토에라
		fd2( "connect() \301\242\274\323 \275\307\306\320" );
		goto IsaoCheck_winsockERR;
	case ISAO_ERR_SSL_READ:				// ＳＳＬ 리드 할 수 없었다．
		fd2(" recv()  \300\320\261\342 \275\307\306\320" );
		goto IsaoCheck_winsockERR;
	case ISAO_ERR_SSL_TIMEOUT:			// 타임 아웃했다．
		fd2("---------------- select() \305\270\300\323\276\306\277\364 \275\307\306\320" );
		goto IsaoCheck_winsockERR;
	}
	//winsock 상의 에러 첵크함.  ================================== end

	ret_chk = chk_readbuffer("Code=", szReadBuffer, &iRejectCode );
	switch(ret_chk)
	{
	case 100:					//성공
	case 101:					//그런클랜이름없음.
	case 102:					//clanName에 권한 업데이트 실패
		goto IsaoCheck_Free;
	case ISAO_ERR_TCP_CONNECT:		
	case ISAO_ERR_NONE_REEJCT:					//응답번호가 없음.
		fd2("ISAO_ERR_NONE_REEJCT  : \300\300\264\344\271\370\310\243\260\241 \276\370\300\275" );
		goto IsaoCheck_Free;
	}
	

IsaoCheck_Free:
	return ret_chk;


IsaoCheck_winsockERR:			//winsock으로 연결시에러남
	return ret_chk ;
}

/*///////////////////////////////////////////////////////////////////////////
: 함 수 명 :int 
: 하 는 일 :
: 서    식 : 
: 인    자 :
: 반 환 값 :
: 논    평 : 초기화면 관련 플래그 
: 버    전 :
: PROJECT  :
///////////////////////////////////////////////////////////////////////////*/
int ISAOCHECK::isPFlag(char *userid,char *clanname, char *chaname,char *gserver,char *pflag, char *gunbun)
{
	int ret_chk;
	int iRejectCode = 111111;
	char szWriteBuffer[BUF_LEN];
	char szReadBuffer [BUF_LEN];
/*
//일본클랜초기화플래그
#ifdef _LANGUAGE_JAPANESE

	char UTF8_clanname[1024];			memset(UTF8_clanname,0,sizeof(UTF8_clanname));
	char UTF8_chaname[1024];			memset(UTF8_chaname,0,sizeof(UTF8_chaname));
	
	MBCSToWeb(clanname,				UTF8_clanname,932);
	MBCSToWeb(chaname,				UTF8_chaname,932);

	
	sprintf( szWriteBuffer,
		"http://%s/ServerMain/isPFlag.asp?userid=%s&clName=%s&chName=%s&gserver=%s&PFlag=%s&Gubun=%s&ticket=%d",
		 hostName,userid,
		 UTF8_clanname, 
		 UTF8_chaname,
		 gserver,pflag, gunbun,nClanTicket);

#else
*/
	sprintf( szWriteBuffer,
		"http://%s/ServerMain/isPFlag.asp?userid=%s&clName=%s&chName=%s&gserver=%s&PFlag=%s&Gubun=%s&ticket=%d",
		 hostName,userid,clanname, chaname,gserver,pflag, gunbun,nClanTicket);
//#endif
	

	int cnt=0;
	while(1) {		//컨넥트되도 답이 안오는경우를 위해 5번정도 재접한다.
		ret_chk = connect_WEBdb( szWriteBuffer, szReadBuffer);
		if(ret_chk != ISAO_ERR_RECONNECT) break;
		if(cnt>5) break;
		cnt++;
	}

	
	//ktj : 임시
	
	debug_fd2( "isPFlag() \272\270\263\275\260\315 \261\346\300\314 : %d \263\273\277\353 : %s ", strlen(szWriteBuffer), szWriteBuffer);
	debug_fd2( "isPFlag() \271\336\300\272\260\315 \261\346\300\314 : %d \263\273\277\353 : %s ", strlen(szReadBuffer), szReadBuffer);
	

	//ktj : 아래 첵크는 모두 임시적으로 운영됨

	//winsock 상의 에러 첵크함.  ================================== start
	switch(ret_chk) {
	case ISAO_ERR_WINSOCK_NOERR:		//에러 없음.
		//ktj : 임시로 서버에서 온 데이타를 뿌려봄.
		fd2( "From server: ---from--\n%s--end--\n ",szReadBuffer);			
		break;
	case ISAO_ERR_WINSOCK_START:		//윈속 START시 에러발생				//window only
		fd2("WSAStartup()\277\241\266\363");
		goto IsaoCheck_winsockERR;
	case ISAO_ERR_WINSOCK_VERSION:		//윈속버전이 틀려 구동되지못함.			//window only
		fd2("\271\366\300\374\300\314 \270\302\301\366 \276\312\300\275");
		goto IsaoCheck_winsockERR;
	case ISAO_ERR_TCP_CONNECT:			// 보통 사용되는 ＴＣＰ코네쿠토에라
		fd2( "connect() \301\242\274\323 \275\307\306\320" );
		goto IsaoCheck_winsockERR;
	case ISAO_ERR_SSL_READ:				// ＳＳＬ 리드 할 수 없었다．
		fd2(" recv()  \300\320\261\342 \275\307\306\320" );
		goto IsaoCheck_winsockERR;
	case ISAO_ERR_SSL_TIMEOUT:			// 타임 아웃했다．
		fd2("---------------- select() \305\270\300\323\276\306\277\364 \275\307\306\320" );
		goto IsaoCheck_winsockERR;
	}
	//winsock 상의 에러 첵크함.  ================================== end

	ret_chk = chk_readbuffer("Code=", szReadBuffer, &iRejectCode );
	switch(ret_chk)
	{
	case 100:					//성공
	case 101:					//그런클랜이름없음.
	case 102:					//clanName에 권한 업데이트 실패
		goto IsaoCheck_Free;
	case ISAO_ERR_TCP_CONNECT:		
	case ISAO_ERR_NONE_REEJCT:					//응답번호가 없음.
		fd2("ISAO_ERR_NONE_REEJCT  : \300\300\264\344\271\370\310\243\260\241 \276\370\300\275" );
		goto IsaoCheck_Free;
	}
	

IsaoCheck_Free:
	return ret_chk;


IsaoCheck_winsockERR:			//winsock으로 연결시에러남
	return ret_chk ;
}





/*///////////////////////////////////////////////////////////////////////////
: 함 수 명 :int 
: 하 는 일 :
: 서    식 : 
: 인    자 :
: 반 환 값 :
: 논    평 : 실제 사용함수 
: 버    전 :
: PROJECT  :
///////////////////////////////////////////////////////////////////////////*/
int ISAOCHECK::WarhouseRight(char *clname, char *right_str)
{
	int ret_chk;
	int iRejectCode = 111111;
	char szWriteBuffer[BUF_LEN];
	char szReadBuffer [BUF_LEN];
	
	sprintf( szWriteBuffer,
		//"POST http://%s/tjboyh/clan_WarehouseRight.php?clName=%s&whRight=%s&ticket=%d\r\n\r\n\r\n", hostName, clname,right_str ,nClanTicket);
		"http://%s/tjboyh/clan_WarehouseRight.php?clName=%s&whRight=%s&ticket=%d", hostName, clname,right_str ,nClanTicket);
	
	int cnt=0;
	while(1) {		//컨넥트되도 답이 안오는경우를 위해 5번정도 재접한다.
		ret_chk = connect_WEBdb( szWriteBuffer, szReadBuffer);
		if(ret_chk != ISAO_ERR_RECONNECT) break;
		if(cnt>5) break;
		cnt++;
	}

	
	//ktj : 임시
	fd2("WarhouseRight() \300\245 db\277\241\274\255 \300\320\300\272\260\315 %s ", szReadBuffer);
	//ktj : 아래 첵크는 모두 임시적으로 운영됨

	//winsock 상의 에러 첵크함.  ================================== start
	switch(ret_chk) {
	case ISAO_ERR_WINSOCK_NOERR:		//에러 없음.
		//ktj : 임시로 서버에서 온 데이타를 뿌려봄.
		fd2( "From server: ---from--\n%s--end--\n ",szReadBuffer);			
		break;
	case ISAO_ERR_WINSOCK_START:		//윈속 START시 에러발생				//window only
		fd2("WSAStartup()\277\241\266\363");
		goto IsaoCheck_winsockERR;
	case ISAO_ERR_WINSOCK_VERSION:		//윈속버전이 틀려 구동되지못함.			//window only
		fd2("\271\366\300\374\300\314 \270\302\301\366 \276\312\300\275");
		goto IsaoCheck_winsockERR;
	case ISAO_ERR_TCP_CONNECT:			// 보통 사용되는 ＴＣＰ코네쿠토에라
		fd2( "connect() \301\242\274\323 \275\307\306\320" );
		goto IsaoCheck_winsockERR;
	case ISAO_ERR_SSL_READ:				// ＳＳＬ 리드 할 수 없었다．
		fd2(" recv()  \300\320\261\342 \275\307\306\320" );
		goto IsaoCheck_winsockERR;
	case ISAO_ERR_SSL_TIMEOUT:			// 타임 아웃했다．
		fd2("---------------- select() \305\270\300\323\276\306\277\364 \275\307\306\320" );
		goto IsaoCheck_winsockERR;
	}
	//winsock 상의 에러 첵크함.  ================================== end

	ret_chk = chk_readbuffer("account=", szReadBuffer, &iRejectCode );
	switch(ret_chk)
	{
	case 100:					//성공
	case 101:					//그런클랜이름없음.
	case 102:					//clanName에 권한 업데이트 실패
		goto IsaoCheck_Free;
		
	case ISAO_ERR_NONE_REEJCT:					//응답번호가 없음.
		fd2("ISAO_ERR_NONE_REEJCT  : \300\300\264\344\271\370\310\243\260\241 \276\370\300\275" );
		goto IsaoCheck_Free;
	}
	

IsaoCheck_Free:
	return ret_chk;


IsaoCheck_winsockERR:			//winsock으로 연결시에러남
	return ret_chk ;
}


/*///////////////////////////////////////////////////////////////////////////
: 함 수 명 :int 
: 하 는 일 :
: 서    식 : 
: 인    자 :
: 반 환 값 :
: 논    평 : 실제 사용함수 
: 버    전 :
: PROJECT  :
///////////////////////////////////////////////////////////////////////////*/
int ISAOCHECK::updateFlag(char *clname)
{
	int ret_chk;
	int iRejectCode = 111111;
	char szWriteBuffer[BUF_LEN];
	char szReadBuffer [BUF_LEN];
	
	sprintf( szWriteBuffer,
		//"POST http://%s/tjboyh/updateFlag.php?clName=%s&ticket=%d\r\n\r\n\r\n", hostName, clname,nClanTicket);
		"http://%s/tjboyh/updateFlag.php?clName=%s&ticket=%d", hostName, clname,nClanTicket);
	
	int cnt=0;
	while(1) {		//컨넥트되도 답이 안오는경우를 위해 5번정도 재접한다.
		ret_chk = connect_WEBdb( szWriteBuffer, szReadBuffer);
		if(ret_chk != ISAO_ERR_RECONNECT) break;
		if(cnt>5) break;
		cnt++;
	}

	
	//ktj : 임시
	fd2("updateFlag() \300\245 db\277\241\274\255 \300\320\300\272\260\315 %s ", szReadBuffer);
	//ktj : 아래 첵크는 모두 임시적으로 운영됨

	//winsock 상의 에러 첵크함.  ================================== start
	switch(ret_chk) {
	case ISAO_ERR_WINSOCK_NOERR:		//에러 없음.
		//ktj : 임시로 서버에서 온 데이타를 뿌려봄.
		fd2( "From server: ---from--\n%s--end--\n ",szReadBuffer);			
		break;
	case ISAO_ERR_WINSOCK_START:		//윈속 START시 에러발생				//window only
		fd2("WSAStartup()\277\241\266\363");
		goto IsaoCheck_winsockERR;
	case ISAO_ERR_WINSOCK_VERSION:		//윈속버전이 틀려 구동되지못함.			//window only
		fd2("\271\366\300\374\300\314 \270\302\301\366 \276\312\300\275");
		goto IsaoCheck_winsockERR;
	case ISAO_ERR_TCP_CONNECT:			// 보통 사용되는 ＴＣＰ코네쿠토에라
		fd2( "connect() \301\242\274\323 \275\307\306\320" );
		goto IsaoCheck_winsockERR;
	case ISAO_ERR_SSL_READ:				// ＳＳＬ 리드 할 수 없었다．
		fd2(" recv()  \300\320\261\342 \275\307\306\320" );
		goto IsaoCheck_winsockERR;
	case ISAO_ERR_SSL_TIMEOUT:			// 타임 아웃했다．
		fd2("---------------- select() \305\270\300\323\276\306\277\364 \275\307\306\320" );
		goto IsaoCheck_winsockERR;
	};
	//winsock 상의 에러 첵크함.  ================================== end

	ret_chk = chk_readbuffer("account=", szReadBuffer, &iRejectCode );
	switch(ret_chk)
	{
	case 100:					//성공
	case 101:					//그런클랜이름없음.
	case 102:					//업데이트 실패
		goto IsaoCheck_Free;
		
	case ISAO_ERR_NONE_REEJCT:					//응답번호가 없음.
		fd2("ISAO_ERR_NONE_REEJCT  : \300\300\264\344\271\370\310\243\260\241 \276\370\300\275" );
		goto IsaoCheck_Free;
	}
	

IsaoCheck_Free:
	return ret_chk;


IsaoCheck_winsockERR:			//winsock으로 연결시에러남
	return ret_chk ;
}












void clanDATA_save(char *fname, char *buff, int dataLen)
{
    HANDLE hFile;
	char allFname[64];
	sprintf( allFname,
		"%s\\%s\\%s",SAVE_DIR,szConnServerName, fname);

    DWORD dwAcess =0;
    hFile = CreateFile( allFname, GENERIC_WRITE, FILE_SHARE_READ|FILE_SHARE_WRITE,
                        NULL, OPEN_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL );

    if( hFile != INVALID_HANDLE_VALUE ) {
        //데이타 쓰기     버퍼    쓸크기    실제로 쓰여진 바이트수
        int chk = WriteFile( hFile , buff , dataLen , &dwAcess , NULL );
        //파일 닫기
        CloseHandle( hFile );
    }
}

void clanDATA_save2(char *fname, char *buff, int dataLen)
{
    HANDLE hFile;
	char allFname[64];
	sprintf( allFname,
		"%s\\%s",CLANINFO_DIR,  fname);

    DWORD dwAcess =0;
    hFile = CreateFile( allFname, GENERIC_WRITE, FILE_SHARE_READ|FILE_SHARE_WRITE,
                        NULL, OPEN_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL );

    if( hFile != INVALID_HANDLE_VALUE ) {
        //데이타 쓰기     버퍼    쓸크기    실제로 쓰여진 바이트수
        int chk = WriteFile( hFile , buff , dataLen , &dwAcess , NULL );
        //파일 닫기
        CloseHandle( hFile );
    }
}

















/*///////////////////////////////////////////////////////////////////////
NPC Wav파일 가져오기
///////////////////////////////////////////////////////////////////////*/

//http://211.39.144.158/NPCSound/darksanc_amb.yul
extern char szWavUrl[255];
int	ISAOCHECK::GetWavFile(char* FileName)
{
	int ret_chk;
	int iRejectCode = 111111;
	char szWriteBuffer[BUF_LEN];
	char szReadBuffer [BUF_LEN];
	
	strcpy(szNoticeUrl,"211.39.144.158/NPCSound");
	sprintf(szWriteBuffer,"http://%s/%s.yul",szNoticeUrl,FileName);

	int cnt=0;
	while(1) {		//컨넥트되도 답이 안오는경우를 위해 5번정도 재접한다.
		ret_chk = connect_WEBdb( szWriteBuffer, szReadBuffer);
		if(ret_chk != ISAO_ERR_RECONNECT) break;
		if(cnt>5) break;
		cnt++;
	}

	//g_zip.DeComOpen("darksanc_amb.yul");

	
	//ktj : 임시
	
	debug_fd2( "GetWavFile() \272\270\263\275\260\315 \261\346\300\314 : %d \263\273\277\353 : %s ", strlen(szWriteBuffer), szWriteBuffer);
	debug_fd2( "GetWavFile() \271\336\300\272\260\315 \261\346\300\314 : %d \263\273\277\353 : %s ", strlen(szReadBuffer), szReadBuffer);
	

	//ktj : 아래 첵크는 모두 임시적으로 운영됨

	//winsock 상의 에러 첵크함.  ================================== start
	switch(ret_chk) {
	case ISAO_ERR_WINSOCK_NOERR:		//에러 없음.
		//ktj : 임시로 서버에서 온 데이타를 뿌려봄.
		fd2( "From server: ---from--\n%s--end--\n ",szReadBuffer);			
		break;
	case ISAO_ERR_WINSOCK_START:		//윈속 START시 에러발생				//window only
		fd2("WSAStartup()\277\241\266\363");
		goto IsaoCheck_winsockERR;
	case ISAO_ERR_WINSOCK_VERSION:		//윈속버전이 틀려 구동되지못함.			//window only
		fd2("\271\366\300\374\300\314 \270\302\301\366 \276\312\300\275");
		goto IsaoCheck_winsockERR;
	case ISAO_ERR_TCP_CONNECT:			// 보통 사용되는 ＴＣＰ코네쿠토에라
		fd2( "connect() \301\242\274\323 \275\307\306\320" );
		goto IsaoCheck_winsockERR;
	case ISAO_ERR_SSL_READ:				// ＳＳＬ 리드 할 수 없었다．
		fd2(" recv()  \300\320\261\342 \275\307\306\320" );
		goto IsaoCheck_winsockERR;
	case ISAO_ERR_SSL_TIMEOUT:			// 타임 아웃했다．
		fd2("---------------- select() \305\270\300\323\276\306\277\364 \275\307\306\320" );
		goto IsaoCheck_winsockERR;
	}
	//winsock 상의 에러 첵크함.  ================================== end

	ret_chk = chk_readbuffer("Code=", szReadBuffer, &iRejectCode );
	switch(ret_chk)
	{
	case 100:					//성공
	case 101:					//그런클랜이름없음.
	case 102:					//clanName에 권한 업데이트 실패
		goto IsaoCheck_Free;
	case ISAO_ERR_TCP_CONNECT:		
	case ISAO_ERR_NONE_REEJCT:					//응답번호가 없음.
		fd2("ISAO_ERR_NONE_REEJCT  : \300\300\264\344\271\370\310\243\260\241 \276\370\300\275" );
		goto IsaoCheck_Free;
	}
	

IsaoCheck_Free:
	return ret_chk;


IsaoCheck_winsockERR:			//winsock으로 연결시에러남
	return ret_chk ;
}


/*
//ktj : 서버에있는 파일을 다운받아 파일로 저장하는 함수임.
*/
int ISAOCHECK::read_ClanImage(char *clanname,char *filename)
{
	int dataSize=0;
	int ret_chk;
	int iRejectCode = 111111;
	char szWriteBuffer[BUF_LEN];
	char szReadBuffer [65536];	//임시
	char szBMPName[255];
	//char GSever;
	//char UniqueClanNum[64];

	ZeroMemory(szBMPName,sizeof(char)*255);




/*

	//============원본 start

	//ktj : 이렇게 해도 파일을 가져오는데 지장없슴. "GET http://%s/tjboyh/a.bmp\n\r\n\r\n", hostName);
	sprintf( szWriteBuffer,
		  "GET http://%s/ClanContent/%s.bmp\r\n\r\n\r\n\r\n", hostName1, filename);

	int cnt=0;
	while(1) {		//컨넥트되도 답이 안오는경우를 위해 5번정도 재접한다.
		//ret_chk = connect_WEBdb( szWriteBuffer, szReadBuffer);
		ret_chk = connect_WEBdb_BINARY( szWriteBuffer, szReadBuffer, &dataSize);
		if(ret_chk != ISAO_ERR_RECONNECT) break;
		if(cnt>5) break;
		cnt++;
	}
	//============원본 end
*/
	
	//최근 고친것
	sprintf( szWriteBuffer,
		//"http://211.39.144.158/ClanContent/%s.bmp",filename);
		"http://%s/ClanContent/%s.bmp", hostName1, filename);
		  //"GET http://211.39.144.158/ClanContent/713000102.bmp\r\n\r\n\r\n\r\n");


	int cnt=0;
	BOOL bret_chk = FALSE;
	while(1) {		//컨넥트되도 답이 안오는경우를 위해 5번정도 재접한다.
		bret_chk = GetHttpRequest(szWriteBuffer,szReadBuffer,65536,&dataSize);
		if(bret_chk) break;
		else cnt++;
		if(cnt>5) break;
		
	}
	




	//ktj : 임시
	
	debug_fd2( "read_ClanImage() \272\270\263\275\260\315 \261\346\300\314 : %d \263\273\277\353 : %s ", strlen(szWriteBuffer), szWriteBuffer);
	debug_fd2( "read_ClanImage() \271\336\300\272\260\315 \261\346\300\314 : %d \263\273\277\353 : bmp\306\304\300\317\276\325\300\3072\260\263\270\270\302\357\300\275 %c%c. ", strlen(szReadBuffer), szReadBuffer[0],szReadBuffer[1]);
	




	
	
	

	//임시
	if((szReadBuffer[0] == 'B')&&(szReadBuffer[1] == 'M'))
	{
		char server[4];
		char uniqueclannum[64];
		
		//server = filename[0];
		//strcpy(uniqueclannum,&filename[3]);

		int nClanMarklen =0;
		
		ZeroMemory(server,sizeof(server));
		nClanMarklen = lstrlen(filename);
		if(nClanMarklen == 9) {
			server[0] = filename[0];
			strcpy(uniqueclannum,&filename[3]);
		}
		else{
			server[0] = filename[0];
			server[1] = filename[1];
			strcpy(uniqueclannum,&filename[4]);
		}

		//wsprintf(szBMPName,"%s.bmp",filename);
		wsprintf(szBMPName,"%s_%s.%s",server ,uniqueclannum,SAVE_EXE);
		clanDATA_save(szBMPName, szReadBuffer, dataSize);

		char path[128];
		char path2[128];
		int bmpSAVE(char *loadfname, char *savefname);
		wsprintf(path,"%s\\%s\\%s",SAVE_DIR,szConnServerName,szBMPName);
		wsprintf(path2,"%s\\%s\\%s_%s_16.%s",SAVE_DIR,szConnServerName,server ,uniqueclannum,SAVE_EXE);

		bmpSAVE(path, path2);

		ret_chk = 1;
	}
	else
	{
		ret_chk = 2;
	}

	/*char *Convert_HS2bin(char *hs_buf, int len);
	char *banaryData = Convert_HS2bin(szReadBuffer, dataSize/2);


	char saveFname[64];
	sprintf( saveFname, "CLAN_%s", filename);		//받은파일은 CLAN_이라는 이름이 맨앞에 붙음.
	clanDATA_save(saveFname, banaryData , dataSize/2);
	delete banaryData ;
	*/



	
	//ktj : 아래 첵크는 모두 임시적으로 운영됨

	//winsock 상의 에러 첵크함.  ================================== start
	switch(ret_chk) {
	case ISAO_ERR_WINSOCK_NOERR:		//에러 없음.
		//ktj : 임시로 서버에서 온 데이타를 뿌려봄.
		//ktj : 데이타 워낙길어 프린트금지
		fd2( "\274\255\271\366\267\316 \272\316\305\315 \300\320\300\272 dataSize : %d ", dataSize);
		break;
	case ISAO_ERR_WINSOCK_START:		//윈속 START시 에러발생				//window only
		fd2("WSAStartup()\277\241\266\363");
		goto IsaoCheck_winsockERR;
	case ISAO_ERR_WINSOCK_VERSION:		//윈속버전이 틀려 구동되지못함.			//window only
		fd2("\271\366\300\374\300\314 \270\302\301\366 \276\312\300\275");
		goto IsaoCheck_winsockERR;
	case ISAO_ERR_TCP_CONNECT:			// 보통 사용되는 ＴＣＰ코네쿠토에라
		fd2( "connect() \301\242\274\323 \275\307\306\320" );
		goto IsaoCheck_winsockERR;
	case ISAO_ERR_SSL_READ:				// ＳＳＬ 리드 할 수 없었다．
		fd2(" recv()  \300\320\261\342 \275\307\306\320" );
		goto IsaoCheck_winsockERR;
	case ISAO_ERR_SSL_TIMEOUT:			// 타임 아웃했다．
		fd2("---------------- select() \305\270\300\323\276\306\277\364 \275\307\306\320" );
		goto IsaoCheck_winsockERR;
	};
	//winsock 상의 에러 첵크함.  ================================== end


IsaoCheck_winsockERR:			//winsock으로 연결시에러남
	return ret_chk ;
}

int ISAOCHECK::read_ClanImage2(char *clanname,char *filename,CLANINFO *ci)
{
	int dataSize=0;
	int ret_chk;
	int iRejectCode = 111111;
	char szWriteBuffer[BUF_LEN];
	char szReadBuffer [65536];	//임시
	char szBMPName[255];

	ZeroMemory(szBMPName,sizeof(char)*255);





	/*
	//============원본 s

	//ktj : 이렇게 해도 파일을 가져오는데 지장없슴. "GET http://%s/tjboyh/a.bmp\n\r\n\r\n", hostName);
	sprintf( szWriteBuffer,
		//"POST http://%s/tjboyh/fileDOWN.php?filename=%s\r\n\r\n\r\n", hostName, filename);
		//"GET http://%s/tjboyh/clanimage/%s\n\r\n\r\n", hostName, filename);	//ktj.:OK -> 이렇게 하면 파일만 온다.
		  "GET http://%s/ClanContent/%s.bmp\r\n\r\n\r\n\r\n", hostName1,filename);
	
	int cnt=0;
	
	while(1) {		//컨넥트되도 답이 안오는경우를 위해 5번정도 재접한다.
		//ret_chk = connect_WEBdb( szWriteBuffer, szReadBuffer);
		ret_chk = connect_WEBdb_BINARY( szWriteBuffer, szReadBuffer, &dataSize);
		if(ret_chk != ISAO_ERR_RECONNECT) break;
		if(cnt>5) break;
		cnt++;
	}

	//============원본 e
	*/





	
	//최근 고친것

	sprintf( szWriteBuffer,
		"http://%s/ClanContent/%s.bmp", hostName1,filename);
		  //"GET http://211.39.144.158/ClanContent/713000102.bmp\r\n\r\n\r\n\r\n");

	int cnt=0;
	BOOL bret_chk = FALSE;
	while(1) {		//컨넥트되도 답이 안오는경우를 위해 5번정도 재접한다.
		bret_chk = GetHttpRequest(szWriteBuffer,szReadBuffer,65536,&dataSize);
		if(bret_chk) break;
		else cnt++;
		//ret_chk = connect_WEBdb( szWriteBuffer, szReadBuffer);
		//ret_chk = connect_WEBdb_BINARY( szWriteBuffer, szReadBuffer, &dataSize);
		//if(ret_chk != ISAO_ERR_RECONNECT) break;
		if(cnt>5) break;
		//cnt++;
	}
	



	//ktj : 임시
	
	debug_fd2( "read_ClanImage2() \272\270\263\275\260\315 \261\346\300\314 : %d \263\273\277\353 : %s ", strlen(szWriteBuffer), szWriteBuffer);
	debug_fd2( "read_ClanImage2() \271\336\300\272\260\315 \261\346\300\314 : %d \263\273\277\353 : bmp\306\304\300\317\276\325\300\3072\260\263\270\270\302\357\300\275 %c%c. ", strlen(szReadBuffer), szReadBuffer[0],szReadBuffer[1]);
	





	
	char path[128];
	char path1[128];
	//임시
	if((szReadBuffer[0] == 'B')&&(szReadBuffer[1] == 'M'))
	{

		//wsprintf(szFile,"%s\\%c_%s.bmp",CLANINFO_DIR,ClanInfo[ClanInfo_nNum].ClanInfoHeader.Gserver,ClanInfo[ClanInfo_nNum].ClanInfoHeader.UniqueClaClanInfo_nNum);
		//wsprintf(szBMPName,"%s.bmp",filename);	
		

		wsprintf(szBMPName,"%s_%s.%s",ci->ClanInfoHeader.Gserver ,
								ci->ClanInfoHeader.UniqueClaClanInfo_nNum,
								SAVE_EXE);
		clanDATA_save(szBMPName, szReadBuffer, dataSize);

		
		int bmpSAVE(char *loadfname, char *savefname);
		wsprintf(path,"%s\\%s\\%s",SAVE_DIR,szConnServerName,szBMPName);
		wsprintf(path1,"%s\\%s\\%s_%s_16.%s",SAVE_DIR,szConnServerName,ci->ClanInfoHeader.Gserver ,ci->ClanInfoHeader.UniqueClaClanInfo_nNum,SAVE_EXE);

		bmpSAVE(path, path1);

		ret_chk = 1;
	}
	else
	{
		ret_chk = 2;
	}

	/*char *Convert_HS2bin(char *hs_buf, int len);
	char *banaryData = Convert_HS2bin(szReadBuffer, dataSize/2);


	char saveFname[64];
	sprintf( saveFname, "CLAN_%s", filename);		//받은파일은 CLAN_이라는 이름이 맨앞에 붙음.
	clanDATA_save(saveFname, banaryData , dataSize/2);
	delete banaryData ;
	*/



	
	//ktj : 아래 첵크는 모두 임시적으로 운영됨

	//winsock 상의 에러 첵크함.  ================================== start
	switch(ret_chk) {
	case ISAO_ERR_WINSOCK_NOERR:		//에러 없음.
		//ktj : 임시로 서버에서 온 데이타를 뿌려봄.
		//ktj : 데이타 워낙길어 프린트금지
		fd2( "\274\255\271\366\267\316 \272\316\305\315 \300\320\300\272 dataSize : %d ", dataSize);
		break;
	case ISAO_ERR_WINSOCK_START:		//윈속 START시 에러발생				//window only
		fd2("WSAStartup()\277\241\266\363");
		goto IsaoCheck_winsockERR;
	case ISAO_ERR_WINSOCK_VERSION:		//윈속버전이 틀려 구동되지못함.			//window only
		fd2("\271\366\300\374\300\314 \270\302\301\366 \276\312\300\275");
		goto IsaoCheck_winsockERR;
	case ISAO_ERR_TCP_CONNECT:			// 보통 사용되는 ＴＣＰ코네쿠토에라
		fd2( "connect() \301\242\274\323 \275\307\306\320" );
		goto IsaoCheck_winsockERR;
	case ISAO_ERR_SSL_READ:				// ＳＳＬ 리드 할 수 없었다．
		fd2(" recv()  \300\320\261\342 \275\307\306\320" );
		goto IsaoCheck_winsockERR;
	case ISAO_ERR_SSL_TIMEOUT:			// 타임 아웃했다．
		fd2("---------------- select() \305\270\300\323\276\306\277\364 \275\307\306\320" );
		goto IsaoCheck_winsockERR;
	};
	//winsock 상의 에러 첵크함.  ================================== end


IsaoCheck_winsockERR:			//winsock으로 연결시에러남
	return ret_chk ;
}











//바이너리 sorce를 dest에 hexa스트링으로 바꿔저장함.
void ISAOCHECK::Convert_bin2HS(char *sorce, int len)
{
	int i, j, Len;
	for(i=0; i<30; i++) {			//데이타가 있으면 지운다.
		if(fileDATA_HS[i])	delete fileDATA_HS[i];
	}

	HSsu = len / 2048;
	if(len % 2048) HSsu++;
	char byte0, byte1;


	for(j=0; j<HSsu; j++)
	{
		if(j== (HSsu-1) ) {
			fileDATA_HS[j] = new char [ (len%2048)*2+1];			//hexa 스트링은 2배가 됨.
			Len = len%2048;
		}
		else {
			fileDATA_HS[j] = new char [   (2048*2)+1];
			Len = 2048;
		}

		for(i=0; i<Len; i++) {
			byte0 = *sorce & 0x0f;
			byte1 = (*sorce & 0xf0)>>4;
			sorce++;

			if(byte0<10) {
				fileDATA_HS[j][i*2 + 0] = byte0+'0';
			}
			else {
				fileDATA_HS[j][i*2 + 0] = (byte0-10)+'A';
			}
	
			if(byte1<10) {
				fileDATA_HS[j][i*2 + 1] = byte1+'0';
			}
			else {
				fileDATA_HS[j][i*2 + 1] = (byte1-10)+'A';
			}
		}
		fileDATA_HS[j][i*2] = 0;		//스트링이라 마지막에 0을 붙여야함.
	}
}


//hexa 스트링을 바이너리로 바꿔저장.
char *Convert_HS2bin(char *hs_buf, int len)
{
	char *dest;
	char byte0, byte1;
	dest = new char [len];			

	for(int i=0; i<len; i++) {
		byte0 = hs_buf[i*2 + 0];		//hexa스트링은 길이의 두배라서..
		byte1 = hs_buf[i*2 + 1];
			
		if( byte0 <='9') {
			 byte0 -= '0';
		}
		else {
			 byte0 -= 'A';
			 byte0 += 10;
		}

		if( byte1 <='9') {
			 byte1 -= '0';
		}
		else {
			 byte1 -= 'A';
			 byte1 += 10;
		}
		dest[i] = (byte1<<4) | byte0;
	}
	return dest;
}



char *fileREAD(char *fileName, char *buf, int *len)
{
    HANDLE hFile;
    DWORD dwAcess =0;
	char *newBuf;
    hFile = CreateFile( fileName, GENERIC_READ, FILE_SHARE_READ|FILE_SHARE_WRITE,
                        NULL, OPEN_ALWAYS, FILE_ATTRIBUTE_NORMAL , NULL );

    if ( hFile == INVALID_HANDLE_VALUE ) return 0;

    //SetFilePointer( hFile , 0, NULL , FILE_BEGIN );
	*len = GetFileSize( hFile , NULL );
	if(*len==0) goto closeJMP;

	if(buf == NULL) {
		newBuf= new char[*len];
	}
	else {
		newBuf= buf ;
	}
	ReadFile( hFile , newBuf , *len , &dwAcess , NULL );

    

closeJMP:
    CloseHandle( hFile );

    return newBuf;
}






//파일이미지를 읽어서 hexa스트링으로 바꿔서 변수에 넣어보낸다.
int ISAOCHECK::write_ClanImageFile(char *fname)
{
	int i;
	int dataSize=0;
	int ret_chk;
	int iRejectCode = 111111;
	char szWriteBuffer[BUF_LEN*5];
	char szReadBuffer [BUF_LEN*5];	//임시
	

	//char fileName[] = "image\\clanDATA\\a222.txt";
	char fileName[64];
	char OrigFName[] = "a.bmp";
	char *fileBuf=NULL;
	int len;

	sprintf( fileName,"%s\\%s\\%s",SAVE_DIR,szConnServerName, fname	);


	fileBuf = fileREAD(fileName, NULL, &len);

	Convert_bin2HS(fileBuf, len);		//binary->hexa string으로 변환
	if(fileBuf) 
		delete fileBuf;


	
	//이건 나중에 파일을 읽어오는 함수에 넣을것.
	//Convert_HS2bin(hs_fileBuf , len);

	/*
	//file_DATA부분이 길면 서버에서 too large 에러를 보냄......
	sprintf( szWriteBuffer,
		"POST http://%s/tjboyh/clan_insertClanImage.php?file_name=%s&file_DATA=%s\r\n\r\n\r\n", hostName,OrigFName, fileDATA_HS[0]);
	//"GET http://%s/tjboyh/a.bmp\n\r\n\r\n", hostName);
	//"GET /tjboyh/ktj_login.php http/1.0\n\r\n\r\n");	//ktj.:OK
	//"GET http://%s/tjboyh/abc.txt\n\r\n\r\n", hostName);	//ktj.:OK -> 이렇게 하면 파일만 온다.
	//"GET /tjboyh/abc.txt http/1.0\n\r\n\r\n");	//ktj.:OK -> 이렇게 하면 서버파일의 정보와함께 파일이온다.
	
	while(1) {
	ret_chk = connect_WEBdb( szWriteBuffer, szReadBuffer);
	if(szReadBuffer[0]!=0) 
		break;
	}

	//ktj : 임시
	fd2(" 웹 db에서 읽은것 %s ", szReadBuffer);
	*/







	for(i=0; i<HSsu; i++) 
	{
		sprintf( szWriteBuffer,	//file_name=파일이름&file_HSsu=2048바이너리(4096헥사스트링)덩어리갯수&file_cnt=덩어리카운트&file_DATA=파일의 데이타.
			"POST http://%s/tjboyh/clan_insertClanImage.php?file_name=%s&file_HSsu=%d&file_cnt=%d&file_DATA=%s\r\n\r\n\r\n", hostName,OrigFName,    HSsu,i+1,fileDATA_HS[i]);

		while(1) {
			ret_chk = connect_WEBdb( szWriteBuffer, szReadBuffer);
			if(szReadBuffer[0]!=0) 
				break;
		}
	}

	
	//ktj : 아래 첵크는 모두 임시적으로 운영됨
	//winsock 상의 에러 첵크함.  ================================== start
	switch(ret_chk) {
	case ISAO_ERR_WINSOCK_NOERR:		//에러 없음.
		//ktj : 임시로 서버에서 온 데이타를 뿌려봄.
		//fd2( "From server: ---from--\n%s--end--\n ",szReadBuffer);			
		break;
	case ISAO_ERR_WINSOCK_START:		//윈속 START시 에러발생				//window only
		fd2("WSAStartup()\277\241\266\363");
		goto IsaoCheck_winsockERR;
	case ISAO_ERR_WINSOCK_VERSION:		//윈속버전이 틀려 구동되지못함.			//window only
		fd2("\271\366\300\374\300\314 \270\302\301\366 \276\312\300\275");
		goto IsaoCheck_winsockERR;
	case ISAO_ERR_TCP_CONNECT:			// 보통 사용되는 ＴＣＰ코네쿠토에라
		fd2( "connect() \301\242\274\323 \275\307\306\320" );
		goto IsaoCheck_winsockERR;
	case ISAO_ERR_SSL_READ:				// ＳＳＬ 리드 할 수 없었다．
		fd2(" recv()  \300\320\261\342 \275\307\306\320" );
		goto IsaoCheck_winsockERR;
	case ISAO_ERR_SSL_TIMEOUT:			// 타임 아웃했다．
		fd2("---------------- select() \305\270\300\323\276\306\277\364 \275\307\306\320" );
		goto IsaoCheck_winsockERR;
	};
	//winsock 상의 에러 첵크함.  ================================== end


IsaoCheck_winsockERR:			//winsock으로 연결시에러남
	return ret_chk ;
}























/*///////////////////////////////////////////////////////////////////////////
: 함 수 명 :  int Exam()
: 하 는 일 :  웹디비와 연결하기위한 example로
: 서    식 :  이함수에서 검증을 거쳐본다.
: 인    자 :
: 반 환 값 :
: 논    평 :
: 버    전 :
: PROJECT  :
///////////////////////////////////////////////////////////////////////////*/

int ISAOCHECK::WEBdb_Exam()
{
	int ret_chk;
	int iRejectCode = 111111;
	char szWriteBuffer[1024];
	char szReadBuffer [1024]={0,};
	
	
	sprintf( szWriteBuffer,
		"POST http://%s/tjboyh/ktj_login.php\r\n\r\n\r\n" ,hostName);
	ret_chk = connect_WEBdb( szWriteBuffer, szReadBuffer);
	
	//ktj : 임시
	fd2(" \300\245 db\277\241\274\255 \300\320\300\272\260\315 %s ", szReadBuffer);
	
	return ret_chk ;
}


void WEBdb_Exam()
{
	iCheck.init_HostName_Port("211.192.180.113", 80);
	iCheck.init(1, 20);
	iCheck.WEBdb_Exam();
}


/*
//ktj : 서버에있는 파일을 다운받아 파일로 저장하는 함수임.
*/
int ISAOCHECK::read_sod2(char *usid, char *charname,char *gserver, int index , char *szSod2Buff)
{
	int dataSize=0;
	int ret_chk = 0; 
	int iRejectCode = 111111;
	char szWriteBuffer[BUF_LEN];
	char szReadBuffer [65536];	//임시
	memset(szSod2Buff,0,sizeof(szSod2Buff));
	memset(szWriteBuffer,0,sizeof(szWriteBuffer[BUF_LEN]));
	memset(szReadBuffer,0,sizeof(szReadBuffer));

/*
//일본클랜이아니고SOD
#ifdef _LANGUAGE_JAPANESE

	char UTF8_charname[1024];				memset(UTF8_charname,0,sizeof(UTF8_charname));

	MBCSToWeb(charname,				UTF8_charname,932);
	
	sprintf( szWriteBuffer,
	"http://%s/servermain/sod2info.asp?userid=%s&chname=%s&gserver=%s&index=%d", hostName, usid, 
	UTF8_charname, 
	gserver, index);

#else
*/
	sprintf( szWriteBuffer,
	"http://%s/servermain/sod2info.asp?userid=%s&chname=%s&gserver=%s&index=%d", hostName, usid, charname, gserver, index);
//#endif

	int cnt=0;
	BOOL bret_chk = FALSE;
	while(1) {		//컨넥트되도 답이 안오는경우를 위해 5번정도 재접한다.
		bret_chk = GetHttpRequest(szWriteBuffer,szSod2Buff,65536,&dataSize);
		if(bret_chk) break;
		else cnt++;
		if(cnt>5) break;
		 
	}

	//ktj : 임시
	//debug_fd2( "read_ClanImage() 보낸것 길이 : %d 내용 : %s ", strlen(szWriteBuffer), szWriteBuffer);
	//debug_fd2( "read_ClanImage() 받은것 길이 : %d 내용 : bmp파일앞의2개만찍음 %c%c. ", strlen(szReadBuffer), szReadBuffer[0],szReadBuffer[1]);
	
	//ktj : 아래 첵크는 모두 임시적으로 운영됨

	//winsock 상의 에러 첵크함.  ================================== start
	switch(ret_chk) {
	case ISAO_ERR_WINSOCK_NOERR:		//에러 없음.
		//ktj : 임시로 서버에서 온 데이타를 뿌려봄.
		//ktj : 데이타 워낙길어 프린트금지
		fd2( "\274\255\271\366\267\316 \272\316\305\315 \300\320\300\272 dataSize : %d ", dataSize);
		break;
	case ISAO_ERR_WINSOCK_START:		//윈속 START시 에러발생				//window only
		fd2("WSAStartup()\277\241\266\363");
		goto IsaoCheck_winsockERR;
	case ISAO_ERR_WINSOCK_VERSION:		//윈속버전이 틀려 구동되지못함.			//window only
		fd2("\271\366\300\374\300\314 \270\302\301\366 \276\312\300\275");
		goto IsaoCheck_winsockERR;
	case ISAO_ERR_TCP_CONNECT:			// 보통 사용되는 ＴＣＰ코네쿠토에라
		fd2( "connect() \301\242\274\323 \275\307\306\320" );
		goto IsaoCheck_winsockERR;
	case ISAO_ERR_SSL_READ:				// ＳＳＬ 리드 할 수 없었다．
		fd2(" recv()  \300\320\261\342 \275\307\306\320" );
		goto IsaoCheck_winsockERR;
	case ISAO_ERR_SSL_TIMEOUT:			// 타임 아웃했다．
		fd2("---------------- select() \305\270\300\323\276\306\277\364 \275\307\306\320" );
		goto IsaoCheck_winsockERR;
	};
	//winsock 상의 에러 첵크함.  ================================== end

IsaoCheck_winsockERR:			//winsock으로 연결시에러남
	return ret_chk ;
}



bool UTF8ToWeb(char *szUTF8,char *szWeb)
{
	int iCount = 0;
	char chHigh;
	char chLow;
	for(int i=0;szUTF8[i]!=0;i++)
	{
		chHigh = chLow = 0;
		

		// %삽입
		szWeb[iCount] = '%';
		iCount++;

		//상위 비트
		chHigh = (szUTF8[i] & 0xf0) >> 4;

		if( (0<= chHigh) &&	(chHigh <= 9))
		{
			szWeb[iCount] = '0' + chHigh;
			iCount++;
		}
		else
		{
			szWeb[iCount] = 'a' + chHigh - 0x0a;
			iCount++;
		}

		//하위 비트
		chLow = szUTF8[i] & 0x0f;

		if( (0<= chLow) && (chLow <= 9) )
		{
			szWeb[iCount] = '0' +  chLow;
			iCount++;
		}
		else
		{
			szWeb[iCount] = 'a' + chLow - 0x0a;
			iCount++;
		}

	}
	szWeb[iCount] = 0;
	
	return true;	
}

bool MBCSToUTF8(char *szMBCS,char *szUTF8,int iMBCSCodePage)
{
	wchar_t wUniCode[1024];	memset(wUniCode,0,sizeof(wUniCode));

	//유니코드로 변환 => 값은 wUnicode에
	MultiByteToWideCharByCodePage(szMBCS,wUniCode,iMBCSCodePage);

	//UTF8로 변환 => 값은 szUTF8에
	WideCharToMultiByteByCodePage(wUniCode,szUTF8,CP_UTF8);

	return true;
}

bool	MultiByteToWideCharByCodePage(char *szMBCS,wchar_t *szUnicode,int iCodePage)
{
	int iLen = MultiByteToWideChar(iCodePage, 0, szMBCS, lstrlen(szMBCS), NULL, NULL);
	MultiByteToWideChar(iCodePage, 0, szMBCS, lstrlen(szMBCS), szUnicode, iLen);

	return true;
}



bool	WideCharToMultiByteByCodePage(wchar_t *szUnicode,char *szMBCS,int iCodePage)
{//wcslen
	int iLen = WideCharToMultiByte(iCodePage, 0, szUnicode, wcslen(szUnicode), NULL, NULL,NULL,NULL);
	WideCharToMultiByte(iCodePage, 0, szUnicode, wcslen(szUnicode), szMBCS, iLen,NULL,NULL);

	return true;
}

bool MBCSToWeb(char *szMBCS,char *szWeb,int iMBCSCodePage)
{
	char szUTF8[1024];	memset(szUTF8,0,sizeof(szUTF8));
	MBCSToUTF8(szMBCS,szUTF8,iMBCSCodePage);
	UTF8ToWeb(szUTF8,szWeb);
	return true;
}

