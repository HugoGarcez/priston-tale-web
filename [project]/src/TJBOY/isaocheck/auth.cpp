/* 10
 * auth.cpp
 * ISAO 라는 인증을 하는 스레드   
 *
 */
#define __AUTH_CPP__

#include <windowsx.h>
#include <windows.h>
#include <stdio.h>
#include <time.h>
#include <string.h>
#include "..\\..\\sinbaram\\sinLinkHeader.h"

#pragma comment(lib,"ws2_32.lib")



#include "../clanmenu/tjclanDEF.h"

#ifdef USE_PROSTONTALE		//=========== 프리스턴소스인경우

	#include "..\\..\\sinbaram\\SinLinkHeader.h"
	#include "..\\..\\HoBaram\\HoLinkHeader.h"
	#include "..\\..\\language.h"
	#include "..\\..\\HoBaram\\HoTextFile.h"
	#include "..\\clanmenu\\cE_CViewClanInfo.h"


#endif
#include "..\\clanmenu\\tjclanDEF.h"
#include "..\\clanmenu\\tjclan.h"


#include "auth.h"
#include "isaocheck.h"

extern char szConnServerName[16];

void fd( char *fmt, ...);
void fd2( char *fmt, ...);


BOOL workfound;		//일할게 없으면 0임.


int AUTH_QUE_nums=0;			//큐 갯수넣음.
CRITICAL_SECTION g_cs;

AUTH_QUE *AUTH_que;
AUTHLINKTBL aAuthLinkTbl[AUTHLINKTBL_MAX];

DWORD WINAPI AUTH_MainLoop(void* pVoid);

static BOOL AUTH_proc( int mode, int quenum, int threadindex);


//ktj : 클랜 멤버인지 알아낸다.  : AUTH_QUE_CMD_CL_MAKE
int AUTH_proc_isClanMember( int quenum, int threadindex);
int AUTH_proc_isClanMember_Result( int quenum, int threadindex);

//ktj : 클랜 만들기 : AUTH_QUE_CMD_CL_MAKE
int AUTH_proc_ClanMake( int quenum, int threadindex);
int AUTH_proc_ClanMake_Result( int quenum, int threadindex);
//ktj : 클랜 멤버 만들기 : AUTH_QUE_CMD_CL_MEMBER_MAKE
int AUTH_proc_Mmake( int quenum, int threadindex);
int AUTH_proc_Mmake_Result( int quenum, int threadindex);

//ktj : 클랜데이타를 읽는다. : AUTH_QUE_CMD_CL_MEMBER_MAKE
int AUTH_proc_ReadClan( int quenum, int threadindex);
int AUTH_proc_ReadClan_Result( int quenum, int threadindex);


//ktj : 클랜을 해체한다.    AUTH_QUE_CMD_BreakUP
int AUTH_proc_BreakUP( int quenum, int threadindex);
int AUTH_proc_BreakUP_Result( int quenum, int threadindex);


//ktj : 특정아디 추방.    AUTH_QUE_CMD_BreakUP
int AUTH_proc_Banishiment( int quenum, int threadindex);
int AUTH_proc_Banishiment_Result( int quenum, int threadindex);

//ktj : 창고권한.    AUTH_QUE_CMD_WarhouseR
int AUTH_proc_WarhouseR( int quenum, int threadindex);
int AUTH_proc_WarhouseR_Result( int quenum, int threadindex);


//ktj :클랜결성 완료용 AUTH_QUE_CMD_updateFlag
int AUTH_proc_updateFlag( int quenum, int threadindex);
int AUTH_proc_updateFlag_Result( int quenum, int threadindex);



//ktj :클랜탈퇴 AUTH_QUE_CMD_Secession
int AUTH_proc_Secession( int quenum, int threadindex);
int AUTH_proc_Secession_Result( int quenum, int threadindex);



//ktj :클랜마크읽기 AUTH_QUE_CMD_MarkREAD
int AUTH_proc_MarkREAD( int quenum, int threadindex);
int AUTH_proc_MarkREAD_Result( int quenum, int threadindex);

//ygy : 다른 클랜마크읽기

int AUTH_proc_MarkREAD2( int quenum, int threadindex);
int AUTH_proc_MarkREAD2_Result( int quenum, int threadindex);

//ygy : 계정이 클랜장으로 등록되어 있는지 체크
int AUTH_proc_isCheckClanJang( int quenum, int threadindex);
int AUTH_proc_isCheckClanJang_Result( int quenum, int threadindex);

//ygy : 클랜이름이 중복되었는지 체크
int AUTH_proc_isCheckClanName( int quenum, int threadindex);
int AUTH_proc_isCheckClanName_Result( int quenum, int threadindex);

//ygy : 초기 화면 관련 플래그
int AUTH_proc_isPFlag( int quenum, int threadindex);
int AUTH_proc_isPFlag_Result( int quenum, int threadindex);

//ygy :이임관련 플래그
int AUTH_proc_isKPFlag( int quenum, int threadindex);
int AUTH_proc_isKPFlag_Result( int quenum, int threadindex);

//ygy : 클랜마크번호로 클랜 정보 갖어오기
int AUTH_proc_ReadClanInfo( int quenum, int threadindex);
int AUTH_proc_ReadClanInfo_Result( int quenum, int threadindex);

//ygy : cldata 업데이트
int AUTH_proc_Updatcldata( int quenum, int threadindex);
int AUTH_proc_Updatcldata_Result( int quenum, int threadindex);
//ygy : notice
int AUTH_proc_ReadNotice( int quenum, int threadindex);
int AUTH_proc_ReadNotice_Result( int quenum, int threadindex);

//이임
int AUTH_proc_LeaveClan( int quenum, int threadindex);
int AUTH_proc_LeaveClan_Result( int quenum, int threadindex);

//부클랜장 임명
int AUTH_proc_AppointSubChip( int quenum, int threadindex);
int AUTH_proc_AppointSubChip_Result( int quenum, int threadindex);

//부클랜장 해임
int AUTH_proc_ReleaseSubChip( int quenum, int threadindex);
int AUTH_proc_ReleaseSubChip_Result( int quenum, int threadindex);

//Wav파일 가져오기
int AUTH_proc_GetWavFile( int quenum, int threadindex);
int AUTH_proc_GetWavFile_Result( int quenum, int threadindex);





//호동Clan추가
int AUTH_proc_sodindex( int quenum, int threadindex);
int AUTH_proc_sodindex_Result( int quenum, int threadindex);

//호동Clan추가
int AUTH_proc_isCheckClanMember(int quenum, int threadindex);
int AUTH_proc_isCheckClanMember_Result(int quenum, int threadindex);

// function jump table
typedef int (*FUNC)( int, int);
FUNC AUTH_proc_jmptbl[][AUTH_QUE_CMD_ALLNUM] = //3
{	
	{
		NULL,						// AUTH_QUE_COMMAND_NONE,
		AUTH_proc_isClanMember,
		AUTH_proc_ClanMake,			//클랜 만들기
		AUTH_proc_Mmake,			//클랜 멤버만들기
		AUTH_proc_ReadClan,			//클랜데이타를 읽는다.
		AUTH_proc_BreakUP,			//클랜을 해체한다.
		AUTH_proc_Banishiment,		//클랜원추방
		AUTH_proc_WarhouseR,
		AUTH_proc_updateFlag,		//클랜이 완전결성된것으로 업데이트됨.

		AUTH_proc_Secession,		//클랜탈퇴
		

		AUTH_proc_MarkREAD,
		AUTH_proc_isCheckClanJang,		//클랜짱 등록되어 있는지 체크
		AUTH_proc_isCheckClanName,      //클랜이름 중복인지 체크
		AUTH_proc_isPFlag,		     //초기화면 플래그
		AUTH_proc_ReadClanInfo,
		AUTH_proc_MarkREAD2,
		AUTH_proc_Updatcldata,
		AUTH_proc_ReadNotice,
		AUTH_proc_LeaveClan,
		AUTH_proc_isKPFlag, //이임관련 플래그
		AUTH_proc_AppointSubChip,//부클랜장 임명
		AUTH_proc_ReleaseSubChip,//부클랜장 해임
		AUTH_proc_GetWavFile,//wav파일 가져오기
		AUTH_proc_sodindex,
		AUTH_proc_isCheckClanMember, //호동Clan추가
		
	},
	{
		NULL,
		AUTH_proc_isClanMember_Result,
		AUTH_proc_ClanMake_Result,		//시작 result
		AUTH_proc_Mmake_Result,		//끝   result
		AUTH_proc_ReadClan_Result,			//클랜데이타를 읽는다.
		AUTH_proc_BreakUP_Result,
		AUTH_proc_Banishiment_Result,
		AUTH_proc_WarhouseR_Result,
		AUTH_proc_updateFlag_Result,

		AUTH_proc_Secession_Result,	//클랜탈퇴
		
		AUTH_proc_MarkREAD_Result,
		AUTH_proc_isCheckClanJang_Result,
		AUTH_proc_isCheckClanName_Result,      //클랜이름 중복인지 체크
		AUTH_proc_isPFlag_Result,
		AUTH_proc_ReadClanInfo_Result,
		AUTH_proc_MarkREAD2_Result,
		AUTH_proc_Updatcldata_Result,
		AUTH_proc_ReadNotice_Result,
		AUTH_proc_LeaveClan_Result,
		AUTH_proc_isKPFlag_Result,
		AUTH_proc_AppointSubChip_Result, //부클랜장 임명
		AUTH_proc_ReleaseSubChip_Result,//부클랜장 해임
		AUTH_proc_GetWavFile_Result,//Wav파일 가져오기
		AUTH_proc_sodindex_Result,
        AUTH_proc_isCheckClanMember_Result, //호동Clan추가
	}
};

//ktj : AUTH_proc_jmptbl[mode][command]로 사용되며
//		모드는 0:인증, 1은 인증후 리턴값에 대한 함수에 접근해 사용됨.
//		예)인증 시		AUTH_proc_jmptbl[0][AUTH_QUE_COMMAND_AUTH]사용
//			인증리턴후 	AUTH_proc_jmptbl[1][AUTH_QUE_COMMAND_AUTH]사용



//AUTH_QUE 관련 데이타 처리함수 ======================================
int AUTH_QUE_AppendTail( AUTH_QUE *pNow, int type );
AUTH_QUE*  AUTH_QUE_getTop( int type );
int AUTH_QUE_Remove( AUTH_QUE *pNow );
int AUTH_QUE_Count( int type );
int AUTH_addQue( AUTH_QUE *que);


// AUTH DB 큐 링크 테이블의 맨뒤에 한개 추가．
int AUTH_QUE_AppendTail( AUTH_QUE *pNow, int type )
{	
	AUTH_QUE *pOldTail;
	// 가장 최후의 한 개 전．이것 뒤에 추가한다．
	pOldTail = aAuthLinkTbl[type].QueTail.pPrev;
	// 금 최후이였다나 뿔 다음은 자신
	pOldTail->pNext = pNow;

	// 최후의 한 개전을 자신에게 한다．
	aAuthLinkTbl[type].QueTail.pPrev = pNow;

	// 자신의 한 개전은 금 최후이였다나 나루터．
	pNow->pPrev = pOldTail;
	// 자신의 다음은 가장 최후
	pNow->pNext = &aAuthLinkTbl[type].QueTail;

	return TRUE;
}


// DB 큐 링크 테이블의 선두를 돌려 준다．
AUTH_QUE*  AUTH_QUE_getTop( int type )
{
	// MUTEX LOCK

	// 톱에 연결되고る 남자 하인을 조사한다．
	// 그것은 최후 꼬리이였다．だっ 대구 아무것도 없다．
	if( aAuthLinkTbl[type].QueTop.pNext == &aAuthLinkTbl[type].QueTail ){
		return NULL;
	}

	// 이것이다．
	return aAuthLinkTbl[type].QueTop.pNext;
}

// 링크에서는 두 동물이 쉬는 장소．
int AUTH_QUE_Remove( AUTH_QUE *pNow )
{
	AUTH_QUE *pOldNext, *pOldPrev;

	// 자신의 전후를 기억한다．
	pOldNext = pNow->pNext;
	pOldPrev = pNow->pPrev;
	// 전후를 지워 없앤다．
	pNow->pNext = NULL;
	pNow->pPrev = NULL;
	// 연결할지 얻는다．
	pOldNext->pPrev = pOldPrev;
	pOldPrev->pNext = pOldNext;

	return TRUE;
}

// 링크를 센다.
int AUTH_QUE_Count( int type )
{
	int iCount = 0;
	AUTH_QUE *pNow;
	// 선두의 위치
	pNow = aAuthLinkTbl[type].QueTop.pNext;
	// 있을 때까지 센다．
	for( ; pNow->pNext; pNow=pNow->pNext ){
		iCount ++;
	}

	return iCount;
}


//-------------------------------------------------------------------------
// 큐에 처리를 추가한다.
// 인수
// AUTH_QUE *que   등록하는 큐
// 돌아오고 값:
// 성공 :0 이상．세트한 que의 번호
// 실패 :-1
//-------------------------------------------------------------------------

int AUTH_addQue( AUTH_QUE *que)
{    
	AUTH_QUE *pNew;
	EnterCriticalSection(&g_cs);			//ktj : window
	//pthread_mutex_lock( &AUTH_que_mutex);	//ktj : linux

	// freeList의 선두
	pNew = AUTH_QUE_getTop( AUTHLINKTBL_FREE );  //0
	if( pNew == NULL ){
		//eprint( "err:cannot add auth que\n");
		LeaveCriticalSection(&g_cs);				//ktj : window
		//pthread_mutex_unlock( &AUTH_que_mutex);	//ktj : linux
		return -1;
	}

	// 데이터 카피
	pNew->command = que->command;
	pNew->fdid = que->fdid;
	pNew->result = AUTH_ERROR_OTHER;
	//ktj : 일단 삭제
	//pNew->query_time = NowTime.tv_sec;
	pNew->query_time = 10;
	strcpy( pNew->cdkey, que->cdkey);
	strcpy( pNew->isaokey, que->isaokey);
	strcpy( pNew->password, que->password);

	//ktj : 새로 넣음. --s
	strcpy( pNew->ip_address, que->ip_address);
	strcpy( pNew->id, que->id);
	strcpy( pNew->expl, que->expl);
	strcpy( pNew->clanName, que->clanName);
	strcpy( pNew->charId, que->charId);
	//ygy
	strcpy( pNew->ClanZang_ID,que->ClanZang_ID);
	strcpy( pNew->ClanZang_ChaName, que->ClanZang_ChaName);
	strcpy(pNew->gserver, que->gserver);
	strcpy(pNew->ClanWon_ID,que->ClanWon_ID);
	strcpy(pNew->ClanWon_ChaName,que->ClanWon_ChaName);
	strcpy(pNew->Mark,que->Mark);
	strcpy(pNew->Gubun,que->Gubun);
	strcpy(pNew->PFlag,que->PFlag);
	
	pNew->howmanyTime = que->howmanyTime;
	pNew->nNum = que->nNum;
	pNew->ci = que->ci;
	pNew->job_code = que->job_code;
	pNew->level = que->level;
	pNew->joinlevel = que->joinlevel;
	pNew->joinjobcode = que->joinjobcode;
	strcpy(pNew->chipflag , que->chipflag);
	strcpy(pNew->szWavFileName,que->szWavFileName);


	/*char gserver[64]; //현재 군서버
	char ClanWon[64];   //현재 캐릭터명
	char ClanZang[64];  //클랜 짱
*/

	//ktj : 새로 넣음. --e

	pNew->status = AUTH_QUE_STATUS_EXIST;  //1
	pNew->sod2index = que->sod2index;



	// 링크에서는 두 동물이 쉬는 장소．
	AUTH_QUE_Remove( pNew );
	// 개시 기다리고 중(속)의 뒤에 추가한다．
	AUTH_QUE_AppendTail( pNew, AUTHLINKTBL_START_WAIT );

	LeaveCriticalSection(&g_cs);				//ktj : window

	// 배열의 번호
	return pNew->index;
}















/**************************************************************************
 *
 * DWORD WINAPI AUTH_MainLoop(void* pVoid)
 * AUTH 스레드 메인루프(쓰레드에 의해 불리워짐)
 *
 *
 **************************************************************************/
DWORD WINAPI AUTH_MainLoop(void* pVoid)
{
	THREADstruct *thr = (THREADstruct *)pVoid;

	int cnt = 0;
	//BOOL found;
	static struct timeval prev = {0,0};	
	unsigned long thread_index = -1;
	int ret = 0;
	
	//fd2("쓰레드메인루프while 들어가기 직전 ");


	while(1){
		Sleep(10);

		long myTime;
		time( &myTime );
		//fd2("쓰레드메인루프cnt %d  myTime : %d ", cnt++, myTime);

		switch(thr->flag) {
        case FLAG_WAIT:
			time( &myTime );
			//fd2("쓰레드메인루프 FLAG_WAIT 시작. %d  ", myTime );


			thr->state=FLAG_WAIT;
			//EnterCriticalSection(&g_cs);
            SuspendThread( (void *)thr->thrHandle );    //WAIT값이 들어오면 자동중지시킴.
														//재개는 쓰레드 밖에서 해줌.
			//LeaveCriticalSection(&g_cs);

			time( &myTime );
			fd2("\276\262\267\271\265\345\270\336\300\316\267\347\307\301 FLAG_WAIT \263\241. %d ", myTime);
            break;

        case FLAG_EXIT:
            goto _EXIT;

		case FLAG_EXEC:
			thr->state=FLAG_EXEC;
			//fd2("쓰레드메인루프 FLAG_EXEC 들어감. ");

			/*
			EnterCriticalSection(&g_cs);
			
			LeaveCriticalSection(&g_cs);
			*/



			workfound = FALSE;
			EnterCriticalSection(&g_cs);			//ktj : windows //ktj:432줄에서 해체함에주의

			// 대기중인 스트럭처의 맨앞을 가져옴(AUTHLINKTBL_START_WAIT : 1)
			AUTH_QUE *que;
			que = AUTH_QUE_getTop(AUTHLINKTBL_START_WAIT );
			
			if( que != NULL ){			// 대기중인 데이타가 있어 index를 가져옴.
				workfound = TRUE;			
				cnt = que->index;		
			}else{
				workfound = FALSE;			
			}
  
			// 발견된 데이타가 없으면 약간 휴식후 continue한다.
			if( !workfound ) {
				LeaveCriticalSection(&g_cs);				//window only
				continue;
			}

			// 만났던 때
			// 큐 상태 변경．다른 스레드에 블록하다．
			AUTH_que[cnt].status = AUTH_QUE_STATUS_BEGINAUTHPROC; //2
			//작업에 추가한다．
			AUTH_QUE_Remove( &AUTH_que[cnt] );
			AUTH_QUE_AppendTail( &AUTH_que[cnt], AUTHLINKTBL_WORKING );  //2

			LeaveCriticalSection(&g_cs);				//window only

			// AUTH 처리
			//ktj : FAUTH_proc_jmptbl[][AUTH_QUE_COMMAND_ALLNUM] 에 셋팅된 함수를 실행시켜준다
			//      웹사이트통한 asp,php파일을 구동하는 함수가 실행됨.
			if( !AUTH_proc( 0, cnt, thread_index)) {		
				fd2( "auth_proc error [que:%d command:%d]\n", cnt, AUTH_que[cnt].command);
				break;
			}

			// 큐 상태 변경
			EnterCriticalSection(&g_cs);			//window only
		
			// 일이 끝나서 큐를 삭제함.
			AUTH_QUE_Remove( &AUTH_que[cnt] );
			AUTH_QUE_AppendTail( &AUTH_que[cnt], AUTHLINKTBL_FINISH ); //3

			AUTH_que[cnt].status = AUTH_QUE_STATUS_ENDOFAUTHPROC; //3
			LeaveCriticalSection(&g_cs);


			break;
        }
	}
_EXIT:
    thr->flag = FLAG_DEAD;

    ExitThread( TRUE );
    return TRUE;

	

	/*

	//실제 스레드 메인 루프
	//ktj : 종료부분은 외부함수에서 종료하도록 되어있음
    while(1){
		
		found = FALSE;
		EnterCriticalSection(&g_cs);			//ktj : windows //ktj:432줄에서 해체함에주의

		// 대기중인 스트럭처의 맨앞을 가져옴(AUTHLINKTBL_START_WAIT : 1)
		AUTH_QUE *que;
	    que = AUTH_QUE_getTop(AUTHLINKTBL_START_WAIT );
			
		if( que != NULL ){			// 대기중인 데이타가 있어 index를 가져옴.
			found = TRUE;			
			cnt = que->index;		
		}else{
			found = FALSE;			
		}
  
		// 발견된 데이타가 없으면 약간 휴식후 continue한다.
		if( !found) {
			LeaveCriticalSection(&g_cs);				//window only
			continue;
		}

		// 만났던 때
		// 큐 상태 변경．다른 스레드에 블록하다．
		AUTH_que[cnt].status = AUTH_QUE_STATUS_BEGINAUTHPROC; //2
		//작업에 추가한다．
		AUTH_QUE_Remove( &AUTH_que[cnt] );
		AUTH_QUE_AppendTail( &AUTH_que[cnt], AUTHLINKTBL_WORKING );  //2

        LeaveCriticalSection(&g_cs);				//window only

		// AUTH 처리
		//ktj : FAUTH_proc_jmptbl[][AUTH_QUE_COMMAND_ALLNUM] 에 셋팅된 함수를 실행시켜준다
		//      웹사이트통한 asp,php파일을 구동하는 함수가 실행됨.
		if( !AUTH_proc( 0, cnt, thread_index)) {		
			fd2( "auth_proc error [que:%d command:%d]\n", cnt, AUTH_que[cnt].command);
			break;
		}

		
		// 큐 상태 변경
		EnterCriticalSection(&g_cs);			//window only
		
		// 일이 끝나서 큐를 삭제함.
		AUTH_QUE_Remove( &AUTH_que[cnt] );
		AUTH_QUE_AppendTail( &AUTH_que[cnt], AUTHLINKTBL_FINISH ); //3

		AUTH_que[cnt].status = AUTH_QUE_STATUS_ENDOFAUTHPROC; //3
		LeaveCriticalSection(&g_cs);

	}
	
	fd2("쓰레드메인루프 ExitThread  ");
	ExitThread( TRUE );

	return 0;
	*/
}
	

//-------------------------------------------------------------------------
// AUTH 함수 실행 시킴
//AUTH_MainLoop(void* pVoid),AUTH_getResultLoop( void) 의 두개 함수에서 불리워져
//함수포인터를 불러 실행시켜줌.
//-------------------------------------------------------------------------
BOOL AUTH_proc( int mode, int quenum, int threadindex)
{	

	if(mode==0)
		mode = 0;

	if(mode==1)
		mode = 1;

    FUNC	func;
	BOOL	ret = FALSE;

	// 에러 체크
	if( quenum < 0 || quenum > AUTH_QUE_nums ) {
		//eprint( "err: quenum \n");
		fd2( "err: quenum \n");
		return FALSE;
	} 

	if( AUTH_que[quenum].command <= AUTH_QUE_CMD_NONE || AUTH_que[quenum].command >= AUTH_QUE_CMD_ALLNUM )
	{
		fd2( "err: command \n");
		return FALSE;
	}
	
	func = AUTH_proc_jmptbl[mode][AUTH_que[quenum].command];
	if( func) {
		//isaocheck등의 웹에 접근하는 함수들이 함수포인터에 의해 실행되어짐.
		//===============================================================
		ret = func( quenum, threadindex);	
		//===============================================================
	}
	else {
		fd2( "err: no such function\n");
	}
	if( !ret)
		fd2("err func:\n");
	return ret;
	
}










































WebDB_Thread::WebDB_Thread()
{
	
}
WebDB_Thread::~WebDB_Thread()
{

	//######################################################################################
	//작 성 자 : 오 영 석
	AUTH_close();
	//######################################################################################
}
void WebDB_Thread::init()
{
}

void WebDB_Thread::main()
{
	AUTH_getResultLoop();
}


void WebDB_Thread::end()
{
	
}





/*///////////////////////////////////////////////////////////////////////////
: 클라스 명: AUTH_init
: 하 는 일 : 쓰레드와 데이타 셋팅
: 반 환 값 :
///////////////////////////////////////////////////////////////////////////*/
int WebDB_Thread::AUTH_init(int Num)
{
	fd2( "AUTH_init()\307\324\274\366 start" );
	
	//ktj 교체
	AUTH_QUE_nums = Num;//config.authquenum;

	int i;

	// 메시지 큐의 확보
	//AUTH_que = allocateMemory( sizeof( AUTH_QUE) * config.authquenum);	//원본
	AUTH_que = new AUTH_QUE[AUTH_QUE_nums ];								//ktj : setup.cf에서 읽어확보함.
	
	if( AUTH_que == NULL ) {
		//eprint( "can not allocate memory %d\n", sizeof( AUTH_QUE)*config.authquenum);
		fd2("can not allocatte memory %d\n","new memory");
		return FALSE;
	}


	// 메시지 큐의 초기화
	//for( i = 0; i < (int)config.authquenum; i ++ ) {
	for( i = 0; i < (int)AUTH_QUE_nums; i ++ ) {
		AUTH_que[i].status = AUTH_QUE_STATUS_NONE;
		AUTH_que[i].command = AUTH_QUE_CMD_NONE;
		AUTH_que[i].index = i;							//버퍼번호
		AUTH_que[i].fdid = -1;
		AUTH_que[i].cdkey[0] = '\0';
		AUTH_que[i].isaokey[0] = '\0';
		AUTH_que[i].password[0] = '\0';
		
		AUTH_que[i].result = AUTH_ERROR_OTHER;
	}


	//ktj: 크리티컬 섹션의 초기화
	InitializeCriticalSection(&g_cs);
//	pthread_mutex_init( &AUTH_que_mutex, NULL);
//	pthread_mutex_init( &AUTH_que_wait_mutex, NULL);
    
	// AUTH 큐 링크 테이블을 초기화	(AUTHLINKTBL_MAX==현재 4임)
	for( i = 0; i < AUTHLINKTBL_MAX; i ++ ){
		aAuthLinkTbl[i].QueTop.pNext = &aAuthLinkTbl[i].QueTail;	// 선두의 다음은 최후
		aAuthLinkTbl[i].QueTop.pPrev = NULL;						// 선두의 전(앞)은 이루고

		aAuthLinkTbl[i].QueTail.pPrev = &aAuthLinkTbl[i].QueTop;	// 최후의 전(앞)은 선두
		aAuthLinkTbl[i].QueTail.pNext = NULL;						// 최후의 다음은 이루고
	}

	// 전부를 하늘 링크 리스트에 연결한다．
	for( i = 0; i < (int)AUTH_QUE_nums; i ++ ) {
		AUTH_QUE_AppendTail( &AUTH_que[i], AUTHLINKTBL_FREE );
	}
	
	
	//ktj : 윈도우용 쓰레드로 새로 제작
	//AUTH 쓰레드를 시작.(AUTH_MainLoop()함수가 계속 돌아감)
	DWORD dwSendMsgId;
	ThreadS.thrHandle  = 0;
    ThreadS.flag =0;
	
    ThreadS.cnt = 0;
	DWORD thrHandle = (unsigned long)CreateThread(NULL,0,AUTH_MainLoop, (void*)&ThreadS,
													0,&dwSendMsgId);
	ThreadS.thrHandle = thrHandle;

	if(ThreadS.thrHandle ==0) 
		return 0;


	fd2( "AUTH_init()\307\324\274\366 end" );

	return TRUE;
}
/*///////////////////////////////////////////////////////////////////////////
: 함수  명 : AUTH_close()
: 하는  일 : 종료 처리
: 인    자 :
: 반환  값 :
///////////////////////////////////////////////////////////////////////////*/
int WebDB_Thread::AUTH_close( void)
{
	//AUTH 쓰레드를 끝낸다.(TerminateThread():외부에서 쓰레드를 끝내는함수)
    TerminateThread( (void*)ThreadS.thrHandle , 0 );

	delete AUTH_que;
	AUTH_que = NULL;

	DeleteCriticalSection(&g_cs);

	fd2( "AUTH_close()\307\324\274\366 " );
	return TRUE;
}


/*///////////////////////////////////////////////////////////////////////////
: 함수  명 :
: 하는  일 : 쓰레드의 수행중지, 재개함수
: 인    자 :
: 반환  값 :
///////////////////////////////////////////////////////////////////////////*/
void WebDB_Thread::Suspend()     //중지 : ktj : 처리할 데이타가 없는것을 확인한후 중지할것
{
	//먼가 있으면 그냥 간다.
	//if(	workfound != FALSE ||   AUTH_QUE_getTop( AUTHLINKTBL_FINISH ) != NULL)
	//	return;

	fd2("\301\337\301\366\307\324\274\366 Suspend() 00 ");
	ThreadS.flag = FLAG_WAIT;

	fd2("\301\337\301\366\307\324\274\366 Suspend() 11 ");
}

void WebDB_Thread::Resume()      //재개
{
	fd2("\300\347\260\263\307\324\274\366 Resume() \265\351\276\356\260\250 ");

	if(ThreadS.flag == FLAG_EXEC) return; //flag가 실행상태이면 그냥 간다.

		
	fd2("\300\347\260\263\307\324\274\366 Resume() while \275\303\300\333 ");
    while(1) {
		fd2("\300\347\260\263\307\324\274\366 Resume() while \301\337 ");
		
		if(ThreadS.flag == FLAG_WAIT) {

			fd2("\300\347\260\263\307\324\274\366 Resume() while \301\337 break; ");


			break;  //flag가 대기상태일때만 수행함.
		}
    }

	ThreadS.flag = FLAG_EXEC;				//활동재개(주의:플래그를 ResumeThread()앞에쓸것 컴이 너무빠르면 쓰레드가 엉킬위험있음.)
    ResumeThread( (void *)ThreadS.thrHandle  );

	fd2("\300\347\260\263\307\324\274\366 Resume()  \263\241");
}




/**************************************************************************
 * 여기에서는 게임 스레드가 처리하는 부분.
 * AUTH_MainLoop에 의해 처리된 각 결과를 처리하는 함수
 * 공유 메모리를 만지는 것은 게임 스레드
 *  severNT.cpp의 메인에서 계속불리워져 처리됨에 주의
 **************************************************************************/
int WebDB_Thread::AUTH_getResultLoop( void)
{
	int i;
	static	int cnt = 0;

	AUTH_QUE *que;
	cnt = -1;

	EnterCriticalSection(&g_cs);				//window only
	
	for( i = 0; i < AUTH_QUE_nums; i ++ )
	{
		que = AUTH_QUE_getTop( AUTHLINKTBL_FINISH ); //처리가 끝난 큐를 가져온다.
		if( que != NULL ){
			cnt = que->index;	// 배열 번호

			AUTH_QUE_Remove( que );
			// Result 처리한다．
			if( !AUTH_proc( 1,cnt, -1)) {
				//eprint( "auth_proc error [%d]\n", cnt);
			}
			AUTH_que[cnt].status = AUTH_QUE_STATUS_NONE;
			// 하늘 링크에
			AUTH_QUE_AppendTail( &AUTH_que[cnt], AUTHLINKTBL_FREE );
		}else{
			break;
		}
	}
	LeaveCriticalSection(&g_cs);					//window only
	return  TRUE;
}











/*/////////////////////////////////////////////////////////////////////////// START
: 하 는 일 :  웹db에 접속하는 명령처리 함수 
			  함수포인터때문에 C형함수를 선언해수 불리워짐에 주의
///////////////////////////////////////////////////////////////////////////*/


/*///////////////////////////////////////
: 함 수 명 :  
: 하 는 일 :  경고문 읽어오기
: 서    식 :
: 인    자 :
/////////////////////////////////////////*/

int WebDB_Thread::ReadNotice(char *userid,char *chaname,char *gserver)
{
	AUTH_QUE AuthQueTmp;
	ZeroMemory(&AuthQueTmp, sizeof(AuthQueTmp));

	AuthQueTmp.fdid = 100;
	AuthQueTmp.command = AUTH_QUE_CMD_ReadNotice;          // 클랜멤버로 가입

	//ygy	
	strncpy( AuthQueTmp.ClanZang_ID, userid, sizeof( AuthQueTmp.ClanZang_ID ) );
	strncpy( AuthQueTmp.gserver, gserver, sizeof( AuthQueTmp.gserver ) );
	strncpy( AuthQueTmp.ClanZang_ChaName, chaname, sizeof( AuthQueTmp.ClanZang_ChaName ) );	


	//큐에 등록
	if( AUTH_addQue( &AuthQueTmp ) < 0 ){
		// 실패하면 에러를 돌려 준다．기타의 에러．
		//nrproto_CharLogin_send( fd, AUTH_ERROR_OTHER, "authorization required\n" );

		//ktj : 프테때문에 일단 삭제
		//nrproto_ClientLogin_send( fd, 0, "make_Clan() 에러\n" );
		fd2("make_Clan() \277\241\267\257   : \305\245\277\241 \265\356\267\317\276\310\265\312");
		return 0;
	}
	return 1;
}
int AUTH_proc_ReadNotice( int quenum, int threadindex)
{
	return WebDB.AUTH_proc_ReadNotice( quenum, threadindex);
}
int WebDB_Thread::AUTH_proc_ReadNotice( int quenum, int threadindex)
{
	char *chaname,*gserver,*userid;
	int knret = 1, iBadMsg = 0;	

	userid = AUTH_que[quenum].ClanZang_ID;
	chaname = AUTH_que[quenum].ClanZang_ChaName;
	gserver = AUTH_que[quenum].gserver;


	fd2("AUTH_proc_ReadClanInfo %s ", AUTH_que[quenum].Mark);

	//int nMarkNum;
	//id에 해당하는 자가 클랜멤버라면, 100(클랜멤버), 101(클랜칲:클랜명을가져옴), 아니면 102(클랜에 들어있지않음)
	knret = iCheck.ReadNotice(userid,chaname,gserver,AUTH_que[quenum].ret_Buf);
	
	
	// 결과를 격납
	AUTH_que[quenum].result = knret;
	// 결과를 격납
	AUTH_que[quenum].badmsg = iBadMsg;
	return TRUE;
}
int AUTH_proc_ReadNotice_Result( int quenum, int threadindex)
{
	return WebDB.AUTH_proc_ReadNotice_Result( quenum, threadindex);
}

int WebDB_Thread::AUTH_proc_ReadNotice_Result( int quenum, int threadindex)
{
	char *aszAuthErrStr[] = {
	"OK",
	"Err CDKEY", 
	"Err ISAO_ID", 
	"Err PASSWORD", 
	"Err EXPIRE", 
	"Err LOCOUT", 
	"Err ISAO_DB", 
	"Err OTHER", 
	""
	};

	char szData[128];
	
	if( AUTH_que[quenum].result == 0 ){
		szData[0] = '\0';
	}else{
		sprintf( szData, "faild" );
	}

	char szBuffer[256];

	// 에러 메시지 테이블의 범위내라면 문자로 표시
	if( 0 <= AUTH_que[quenum].result && AUTH_que[quenum].result < AUTH_ERROR_END ){
		sprintf( szBuffer, "%s", aszAuthErrStr[AUTH_que[quenum].result] );
	}else{
		sprintf( szBuffer, "ExtraCode=%d", AUTH_que[quenum].result );
	}

	int iResultCode;
	iResultCode = AUTH_que[quenum].result;	// 디폴트는 기타의 에러

	
	// 최종적으로 클라이언트에게 돌려 주는 코드

	//iResultCode = 1;
	//strcpy(AUTH_que[quenum].ret_Buf,"안녕하세요. 당신은 클랜의 범죄자로써 이번 게임을 실행할 수 없습니다. 다시 접속하여주십시오당신은 클랜의 범죄자로써 이번 게임을 실행할 수 없습니다.당신은 클랜의 범죄자로써 이번 게임을 실행할 수 없습니다.당신은 클랜의 범죄자로써 이번 게임을 실행할 수 없습니다.당신은 클랜의 범죄자로써 이번 게임을 실행할 수 없습니다.당신은 클랜의 범죄자로써 이번 게임을 실행할 수 없습니다.당신은 클랜의 범죄자로써 이번 게임을 실행할 수 없습니다.당신은 클랜의 범죄자로써 이번 게임을 실행할 수 없습니다.당신은 클랜의 범죄자로써 이번 게임을 실행할 수 없습니다.당신은 클랜의 범죄자로써 이번 게임을 실행할 수 없습니다.당신은 클랜의 범죄자로써 이번 게임을 실행할 수 없습니다.");
	//AUTH_que[quenum].ret_Buf[0] = 0;

	switch( iResultCode ){
	case 0:
		ANSdata.ret_val = iResultCode;				//tjclan.cpp에 정의된 변수에 답변을 넣는다.
		break;
	case 1:
		ANSdata.ret_val = iResultCode;				//tjclan.cpp에 정의된 변수에 답변을 넣는다.
		strcpy(ANSdata.ret_Buf, AUTH_que[quenum].ret_Buf);		
		break;	
	case 100:				//일반 멤버,
		ANSdata.ret_val = iResultCode;
		break;
	case 1004:
		ANSdata.ret_val = iResultCode;
		break;
	default:
		ANSdata.ret_val = AUTH_ERROR_OTHER;	// 기타의 에러
		break;
	}


	//ktj : 일단 삭제
	// 결과를 조사하고 돌려 준다．
	//ktj : 돌려주는부분을 넣어야함.
	/*
	nrproto_ClientLogin_send( 
			fd, 
			iResultCode,
			szData);
	//nrproto_ClientLogin_send( fd, 1, "OK" );
	*/
	//ANSdata.ret_val = iResultCode;				//tjclan.cpp에 정의된 변수에 답변을 넣는다.
	fd2("\305\254\267\243\270\342\271\366\277\251\272\316\300\307 \264\344\272\257\300\273 \271\336\300\275. :  ret_val %d  ", ANSdata.ret_val);

	

	// 범위내에 들어가고 이르다 OK.
	if( 0 <= AUTH_que[quenum].result && AUTH_que[quenum].result < AUTH_ERROR_END ){
	}else{
		AUTH_que[quenum].result = AUTH_ERROR_OTHER;
	}
	return TRUE;
}


/*///////////////////////////////////////
: 함 수 명 :  
: 하 는 일 :  클랜 마크 번호로 클랜 정보 갖어오기
: 서    식 :
: 인    자 :
/////////////////////////////////////////*/
//ygy : 클랜마크번호로 클랜 정보 갖어오기
int AUTH_proc_ReadClanInfo( int quenum, int threadindex)
{
	return WebDB.AUTH_proc_ReadClanInfo( quenum, threadindex);
}

int WebDB_Thread::AUTH_proc_ReadClanInfo( int quenum, int threadindex)
{
	char *szMarkNum;
	CLANINFO* claninfo;
	//int SaveNum;
	int knret = 1, iBadMsg = 0;
	char* clnaName;

	//id = AUTH_que[quenum].id;
	szMarkNum = AUTH_que[quenum].Mark;
	claninfo = AUTH_que[quenum].ci;
	//SaveNum = AUTH_que[quenum].nNum;



	fd2("AUTH_proc_ReadClanInfo %s ", AUTH_que[quenum].Mark);

	//int nMarkNum;
	//id에 해당하는 자가 클랜멤버라면, 100(클랜멤버), 101(클랜칲:클랜명을가져옴), 아니면 102(클랜에 들어있지않음)
	knret = iCheck.ReadClanInfo(szMarkNum,AUTH_que[quenum].ret_Buf);
	if(knret == 1){
		char* clanINFO_result(char *marknum,char *szInfo,CLANINFO* ci);
		clnaName = clanINFO_result(AUTH_que[quenum].Mark,AUTH_que[quenum].ret_Buf,claninfo);
		knret = iCheck.read_ClanImage2(clnaName,AUTH_que[quenum].Mark,claninfo);
		if(knret ==1)
		{
			claninfo->nStatus = 2;
		}
		else
		{
			claninfo->nStatus = 3;
		}
		//??????????????????????????????????????????????????????????????????????????
		//nCheckDownLoad = 2;
		//??????????????????????????????????????????????????????????????????????????
	}
	else		//다운로드 실패함.
	{
		claninfo->nStatus = 3;
	}
	
	// 결과를 격납
	AUTH_que[quenum].result = knret;
	// 결과를 격납
	AUTH_que[quenum].badmsg = iBadMsg;
	return TRUE;
}
int AUTH_proc_ReadClanInfo_Result( int quenum, int threadindex)
{
	return WebDB.AUTH_proc_ReadClanInfo_Result(quenum, threadindex);
}
int WebDB_Thread::AUTH_proc_ReadClanInfo_Result( int quenum, int threadindex)
{
	char *aszAuthErrStr[] = {
	"OK",
	"Err CDKEY", 
	"Err ISAO_ID", 
	"Err PASSWORD", 
	"Err EXPIRE", 
	"Err LOCOUT", 
	"Err ISAO_DB", 
	"Err OTHER", 
	""
	};

	char szData[128];
	
	if( AUTH_que[quenum].result == 0 ){
		szData[0] = '\0';
	}else{
		sprintf( szData, "faild" );
	}

	char szBuffer[256];

	// 에러 메시지 테이블의 범위내라면 문자로 표시
	if( 0 <= AUTH_que[quenum].result && AUTH_que[quenum].result < AUTH_ERROR_END ){
		sprintf( szBuffer, "%s", aszAuthErrStr[AUTH_que[quenum].result] );
	}else{
		sprintf( szBuffer, "ExtraCode=%d", AUTH_que[quenum].result );
	}

	int iResultCode;
	iResultCode = AUTH_que[quenum].result;	// 디폴트는 기타의 에러

	int clanINFO_result2(int ret_code, char *marknum,CLANINFO *ci);
	//clanINFO_result2(ANSdata.ret_val, AUTH_que[quenum].Mark);


	clanINFO_result2(iResultCode, AUTH_que[quenum].Mark,AUTH_que[quenum].ci);


	//ktj : 임시
	//fd2("AUTH_proc_ReadClanInfo_Result :  ret_val %d  AUTH_que[quenum].Mark : %s ",   ANSdata.ret_val, AUTH_que[quenum].Mark);
	
	// 범위내에 들어가고 이르다 OK.
	if( 0 <= AUTH_que[quenum].result && AUTH_que[quenum].result < AUTH_ERROR_END ){
	}else{
		AUTH_que[quenum].result = AUTH_ERROR_OTHER;
	}
	return TRUE;
}


int WebDB_Thread::ReadClanInfo(char *clanNum,CLANINFO *ci)
{
	//ktj : 임시
	static int cnt=0;
	fd2("ReadClanInfo:  \305\254\267\243\270\266\305\251\271\370\310\243 %s  (cnt %d )",   clanNum,  cnt);
	cnt++;


	AUTH_QUE AuthQueTmp;
	ZeroMemory(&AuthQueTmp,sizeof(AUTH_QUE));

	AuthQueTmp.fdid = 100;
	AuthQueTmp.command = AUTH_QUE_CMD_ReadClanInfo;          // 인증하다
	
	
	strncpy( AuthQueTmp.Mark, clanNum, sizeof( AuthQueTmp.Mark) );
	//AuthQueTmp.nNum =  num;
	AuthQueTmp.ci = ci;

		

	long myTime;
	time( &myTime );
	AuthQueTmp.howmanyTime = myTime;
	
	// 큐를 등록
	if( AUTH_addQue( &AuthQueTmp ) < 0 ){
		// 실패하면 에러를 돌려 준다．기타의 에러．
		//nrproto_CharLogin_send( fd, AUTH_ERROR_OTHER, "authorization required\n" );

		//ktj : 프테때문에 일단 삭제
		//nrproto_ClientLogin_send( fd, 0, "make_Clan() 에러\n" );
		fd2("make_Clan() \277\241\267\257   : \305\245\277\241 \265\356\267\317\276\310\265\312");
		return 0;
	}
	return 1;
}

//호동Clan추가
//해당 아이디가 클랜 탈퇴 후 재가입 기간(4일)이 경과되지 않았을 때의 클랜 가입시의 메세지
//1이면 4일이 안지난 사람이고 0이면 4일이 지난 사람
int WebDB_Thread::isCheckClanMember(char *pszServerName, char *pszUserName)
{
	AUTH_QUE AuthQueTmp;
	ZeroMemory(&AuthQueTmp,sizeof(AUTH_QUE));

	AuthQueTmp.fdid = 100;
	AuthQueTmp.command = AUTH_QUE_CMD_isCheckClanMember;          

	//strncpy( AuthQueTmp.ClanZang_ID,      pszUserID, sizeof( AuthQueTmp.ClanZang_ID ) );
	strncpy( AuthQueTmp.ClanZang_ChaName, pszUserName, sizeof( AuthQueTmp.ClanZang_ChaName ) );
	strncpy( AuthQueTmp.gserver,          pszServerName, sizeof( AuthQueTmp.gserver) );

	//큐에 등록
	if( AUTH_addQue( &AuthQueTmp ) < 0 ){
		fd2("make_Clan() \277\241\267\257   : \305\245\277\241 \265\356\267\317\276\310\265\312");
		return 0;
	}
	return 1;

}

int AUTH_proc_isCheckClanMember_Result( int quenum, int threadindex)
{
	return WebDB.AUTH_proc_isCheckClanMember_Result(quenum, threadindex);	
}

int AUTH_proc_isCheckClanMember(int quenum, int threadindex)
{
	return WebDB.AUTH_proc_isCheckClanMember(quenum, threadindex);
}



int WebDB_Thread::AUTH_proc_isCheckClanMember(int quenum, int threadindex)
{
	char *ClanZang_ID,*ClanZang_ChaName,*gserver;
	int knret = 1, iBadMsg = 0;

	//id = AUTH_que[quenum].id;
	ClanZang_ID      = AUTH_que[quenum].ClanZang_ID;
	ClanZang_ChaName = AUTH_que[quenum].ClanZang_ChaName;
	gserver          = AUTH_que[quenum].gserver;
	
 	knret = iCheck.isCheckClanMember(gserver,ClanZang_ChaName);
	 //knret = iCheck.isCheckClanMember(ClanZang_ID,gserver,ClanZang_ChaName);
	
	// 결과를 격납
	AUTH_que[quenum].result = knret;
	// 결과를 격납
	AUTH_que[quenum].badmsg = iBadMsg;
	
	return TRUE;
}

int WebDB_Thread::AUTH_proc_isCheckClanMember_Result(int quenum, int threadindex)
{
	int iResultCode;
	iResultCode = AUTH_que[quenum].result;	// 디폴트는 기타의 에러


	// 최종적으로 클라이언트에게 돌려 주는 코드
	switch( iResultCode ){
	case 0:
		ANSdata.ret_val = iResultCode;				//tjclan.cpp에 정의된 변수에 답변을 넣는다.
		break;
	case 1:
		ANSdata.ret_val = iResultCode;				//tjclan.cpp에 정의된 변수에 답변을 넣는다.		
		break;	
	
	default :
		ANSdata.ret_val = AUTH_ERROR_OTHER;	// 기타의 에러
		break;
	}
	return TRUE;
}


/*///////////////////////////////////////
: 함 수 명 :  

: 하 는 일 :  클랜멤버인지를 알아내는함수
: 서    식 :
: 인    자 :
/////////////////////////////////////////*/
int AUTH_proc_isClanMember( int quenum, int threadindex)
{
	return WebDB.AUTH_proc_isClanMember( quenum, threadindex);
}

int WebDB_Thread::AUTH_proc_isClanMember( int quenum, int threadindex)
{
	char *ClanZang_ID,*ClanZang_ChaName,*gserver;
	int knret = 1, iBadMsg = 0;

	//id = AUTH_que[quenum].id;
	ClanZang_ID = AUTH_que[quenum].ClanZang_ID;
	ClanZang_ChaName = AUTH_que[quenum].ClanZang_ChaName;
	gserver = AUTH_que[quenum].gserver;
	
	//id에 해당하는 자가 클랜멤버라면, 100(클랜멤버), 101(클랜칲:클랜명을가져옴), 아니면 102(클랜에 들어있지않음)
	knret = iCheck.isClanMember(ClanZang_ID,gserver,ClanZang_ChaName, AUTH_que[quenum].ret_Buf);


	// 결과를 격납
	AUTH_que[quenum].result = knret;
	// 결과를 격납
	AUTH_que[quenum].badmsg = iBadMsg;
	return TRUE;
}
   

int AUTH_proc_isClanMember_Result( int quenum, int threadindex)
{
	return WebDB.AUTH_proc_isClanMember_Result(quenum, threadindex);
}
int WebDB_Thread::AUTH_proc_isClanMember_Result( int quenum, int threadindex)
{
	char *aszAuthErrStr[] = {
	"OK",
	"Err CDKEY", 
	"Err ISAO_ID", 
	"Err PASSWORD", 
	"Err EXPIRE", 
	"Err LOCOUT", 
	"Err ISAO_DB", 
	"Err OTHER", 
	""
	};

	char szData[128];
	
	if( AUTH_que[quenum].result == 0 ){
		szData[0] = '\0';
	}else{
		sprintf( szData, "faild" );
	}

	char szBuffer[256];

	// 에러 메시지 테이블의 범위내라면 문자로 표시
	if( 0 <= AUTH_que[quenum].result && AUTH_que[quenum].result < AUTH_ERROR_END ){
		sprintf( szBuffer, "%s", aszAuthErrStr[AUTH_que[quenum].result] );
	}else{
		sprintf( szBuffer, "ExtraCode=%d", AUTH_que[quenum].result );
	}

	int iResultCode;
	iResultCode = AUTH_que[quenum].result;	// 디폴트는 기타의 에러

	
	// 최종적으로 클라이언트에게 돌려 주는 코드
	switch( iResultCode ){
	case 0:
		ANSdata.ret_val = iResultCode;				//tjclan.cpp에 정의된 변수에 답변을 넣는다.
		strcpy(ANSdata.ret_Buf, AUTH_que[quenum].ret_Buf);
		break;
	case 1:
		ANSdata.ret_val = iResultCode;				//tjclan.cpp에 정의된 변수에 답변을 넣는다.
		strcpy(ANSdata.ret_Buf, AUTH_que[quenum].ret_Buf);
		break;
	case 5:
	case 4:
	case 3:
	case 2:
		ANSdata.ret_val = iResultCode;				//tjclan.cpp에 정의된 변수에 답변을 넣는다.
		strcpy(ANSdata.ret_Buf, AUTH_que[quenum].ret_Buf);
		break;
	case 100:				//일반 멤버,
		ANSdata.ret_val = iResultCode;
		break;
	case 1004:
		ANSdata.ret_val = iResultCode;
		break;
	default:
		ANSdata.ret_val = AUTH_ERROR_OTHER;	// 기타의 에러
		break;
	}


	//ktj : 일단 삭제
	// 결과를 조사하고 돌려 준다．
	//ktj : 돌려주는부분을 넣어야함.
	/*
	nrproto_ClientLogin_send( 
			fd, 
			iResultCode,
			szData);
	//nrproto_ClientLogin_send( fd, 1, "OK" );
	*/
	//ANSdata.ret_val = iResultCode;				//tjclan.cpp에 정의된 변수에 답변을 넣는다.
	fd2("\305\254\267\243\270\342\271\366\277\251\272\316\300\307 \264\344\272\257\300\273 \271\336\300\275. :  ret_val %d  ", ANSdata.ret_val);

	

	// 범위내에 들어가고 이르다 OK.
	if( 0 <= AUTH_que[quenum].result && AUTH_que[quenum].result < AUTH_ERROR_END ){
	}else{
		AUTH_que[quenum].result = AUTH_ERROR_OTHER;
	}
	return TRUE;
}


/*///////////////////////////////////////
: 함 수 명 :  
: 하 는 일 :  초기 화면 관련 플래그
: 서    식 :
: 인    자 :
/////////////////////////////////////////*/

/*///////////////////////////////////////
: 함 수 명 :  
: 하 는 일 :  클랜멤버인지를 알아내는함수
: 서    식 :
: 인    자 :
/////////////////////////////////////////*/
int AUTH_proc_isPFlag( int quenum, int threadindex)
{
	return WebDB.AUTH_proc_isPFlag( quenum, threadindex);
}

int WebDB_Thread::AUTH_proc_isPFlag( int quenum, int threadindex)
{
	char *ClanZang_ID,*ClanZang_ChaName,*gserver,*PFlag,*Gubun,*ClanName;
	int knret = 1, iBadMsg = 0;

	//id = AUTH_que[quenum].id;
	ClanZang_ID = AUTH_que[quenum].ClanZang_ID;
	ClanZang_ChaName = AUTH_que[quenum].ClanZang_ChaName;
	gserver = AUTH_que[quenum].gserver;
	PFlag = AUTH_que[quenum].PFlag;
	Gubun = AUTH_que[quenum].Gubun;
	ClanName = AUTH_que[quenum].clanName;
	
	//id에 해당하는 자가 클랜멤버라면, 100(클랜멤버), 101(클랜칲:클랜명을가져옴), 아니면 102(클랜에 들어있지않음)
	knret = iCheck.isPFlag(ClanZang_ID,ClanName,ClanZang_ChaName,gserver, PFlag,Gubun);


	// 결과를 격납
	AUTH_que[quenum].result = knret;
	// 결과를 격납
	AUTH_que[quenum].badmsg = iBadMsg;
	return TRUE;
}

int AUTH_proc_isPFlag_Result( int quenum, int threadindex)
{
	return WebDB.AUTH_proc_isPFlag_Result( quenum, threadindex);
}

int WebDB_Thread::AUTH_proc_isPFlag_Result( int quenum, int threadindex)
{
	char *aszAuthErrStr[] = {
	"OK",
	"Err CDKEY", 
	"Err ISAO_ID", 
	"Err PASSWORD", 
	"Err EXPIRE", 
	"Err LOCOUT", 
	"Err ISAO_DB", 
	"Err OTHER", 
	""
	};

	char szData[128];
	
	if( AUTH_que[quenum].result == 0 ){
		szData[0] = '\0';
	}else{
		sprintf( szData, "faild" );
	}

	char szBuffer[256];

	// 에러 메시지 테이블의 범위내라면 문자로 표시
	if( 0 <= AUTH_que[quenum].result && AUTH_que[quenum].result < AUTH_ERROR_END ){
		sprintf( szBuffer, "%s", aszAuthErrStr[AUTH_que[quenum].result] );
	}else{
		sprintf( szBuffer, "ExtraCode=%d", AUTH_que[quenum].result );
	}

	int iResultCode;
	iResultCode = AUTH_que[quenum].result;	// 디폴트는 기타의 에러

	
	// 최종적으로 클라이언트에게 돌려 주는 코드
	switch( iResultCode ){
	case 0:
		ANSdata.ret_val = iResultCode;				//tjclan.cpp에 정의된 변수에 답변을 넣는다.
		break;
	case 1:
		ANSdata.ret_val = iResultCode;				//tjclan.cpp에 정의된 변수에 답변을 넣는다.		
		break;	
	case 100:				//일반 멤버,
		ANSdata.ret_val = iResultCode;
		break;
	case 1004:
		ANSdata.ret_val = iResultCode;
		break;		
	default :
		ANSdata.ret_val = AUTH_ERROR_OTHER;	// 기타의 에러
		break;
	}


	//ktj : 일단 삭제
	// 결과를 조사하고 돌려 준다．
	//ktj : 돌려주는부분을 넣어야함.
	/*
	nrproto_ClientLogin_send( 
			fd, 
			iResultCode,
			szData);
	//nrproto_ClientLogin_send( fd, 1, "OK" );
	*/
	//ANSdata.ret_val = iResultCode;				//tjclan.cpp에 정의된 변수에 답변을 넣는다.
	fd2("\305\254\267\243\270\342\271\366\277\251\272\316\300\307 \264\344\272\257\300\273 \271\336\300\275. :  ret_val %d  ", ANSdata.ret_val);

	

	// 범위내에 들어가고 이르다 OK.
	if( 0 <= AUTH_que[quenum].result && AUTH_que[quenum].result < AUTH_ERROR_END ){
	}else{
		AUTH_que[quenum].result = AUTH_ERROR_OTHER;
	}
	return TRUE;
}

int WebDB_Thread::isPFlag(char *userid, char *clName, char *chName,char *gserver, char *PFlag, char *Gubun)
{
	AUTH_QUE AuthQueTmp;
	ZeroMemory(&AuthQueTmp,sizeof(AUTH_QUE));

	AuthQueTmp.fdid = 100;
	AuthQueTmp.command = AUTH_QUE_CMD_isPFlag;          // 인증하다
	
	strncpy( AuthQueTmp.Gubun, Gubun, sizeof( AuthQueTmp.Gubun ) );
	strncpy(AuthQueTmp.ClanZang_ID, userid, sizeof( AuthQueTmp.ClanZang_ID ));
	strncpy( AuthQueTmp.clanName, clName, sizeof( AuthQueTmp.clanName ) );	
	strncpy( AuthQueTmp.ClanZang_ChaName, chName, sizeof( AuthQueTmp.ClanZang_ChaName ) );	
	strncpy( AuthQueTmp.gserver, gserver, sizeof( AuthQueTmp.gserver ) );
	strncpy( AuthQueTmp.PFlag, PFlag, sizeof( AuthQueTmp.PFlag ) );
	

	

	long myTime;
	time( &myTime );
	AuthQueTmp.howmanyTime = myTime;
	
	// 큐를 등록
	if( AUTH_addQue( &AuthQueTmp ) < 0 ){
		// 실패하면 에러를 돌려 준다．기타의 에러．
		//nrproto_CharLogin_send( fd, AUTH_ERROR_OTHER, "authorization required\n" );

		//ktj : 프테때문에 일단 삭제
		//nrproto_ClientLogin_send( fd, 0, "make_Clan() 에러\n" );
		fd2("make_Clan() \277\241\267\257   : \305\245\277\241 \265\356\267\317\276\310\265\312");
		return 0;
	}
	return 1;
}

/*///////////////////////////////////////
: 함 수 명 :  
: 하 는 일 :  관련된 계정이 클랜장으로 등록되어 있는지 체크
: 서    식 :
: 인    자 :
/////////////////////////////////////////*/
int WebDB_Thread::isCheckClanJang(char *ClanZang_ID, char *gserver)
{
	AUTH_QUE AuthQueTmp;
	ZeroMemory(&AuthQueTmp,sizeof(AUTH_QUE));

	AuthQueTmp.fdid = 100;
	AuthQueTmp.command = AUTH_QUE_CMD_isCheckClanJang;          // 인증하다
		
	strncpy( AuthQueTmp.ClanZang_ID, ClanZang_ID, sizeof( AuthQueTmp.ClanZang_ID ) );	
	strncpy( AuthQueTmp.gserver, gserver, sizeof( AuthQueTmp.gserver ) );

	long myTime;
	time( &myTime );
	AuthQueTmp.howmanyTime = myTime;
	
	// 큐를 등록
	if( AUTH_addQue( &AuthQueTmp ) < 0 ){
		// 실패하면 에러를 돌려 준다．기타의 에러．
		//nrproto_CharLogin_send( fd, AUTH_ERROR_OTHER, "authorization required\n" );

		//ktj : 프테때문에 일단 삭제
		//nrproto_ClientLogin_send( fd, 0, "make_Clan() 에러\n" );
		fd2("make_Clan() \277\241\267\257   : \305\245\277\241 \265\356\267\317\276\310\265\312");
		return 0;
	}
	return 1;
}
int AUTH_proc_isCheckClanJang( int quenum, int threadindex)
{
	return WebDB.AUTH_proc_isCheckClanJang( quenum, threadindex);
}
int WebDB_Thread::AUTH_proc_isCheckClanJang( int quenum, int threadindex)
{
	char* ClanZang_ID,*gserver;
	int knret = 1, iBadMsg = 0;

	ClanZang_ID = AUTH_que[quenum].ClanZang_ID;	
	gserver= AUTH_que[quenum].gserver;
	
	
	knret = iCheck.isCheckClanJang(ClanZang_ID,gserver);


	//ktj : 예제이므로 나중에 지우시오.
	//칼럼에 따른 데이타읽기
	//knret = iCheck.read_ClanName(0);
	//이미지읽기 : OK
	//knret = iCheck.write_ClanImageFile("a.bmp");
	///knret = iCheck.read_ClanImage("a.bmp");


	
	// 결과를 격납
	AUTH_que[quenum].result = knret;
	// 결과를 격납
	AUTH_que[quenum].badmsg = iBadMsg;
	return TRUE;
}


int AUTH_proc_isCheckClanJang_Result( int quenum, int threadindex)
{
	return WebDB.AUTH_proc_isCheckClanJang_Result(quenum, threadindex);
}
int WebDB_Thread::AUTH_proc_isCheckClanJang_Result( int quenum, int threadindex)
{
	char *aszAuthErrStr[] = {
	"OK",
	"Err CDKEY", 
	"Err ISAO_ID", 
	"Err PASSWORD", 
	"Err EXPIRE", 
	"Err LOCOUT", 
	"Err ISAO_DB", 
	"Err OTHER", 
	""
	};

	char szData[128];
	
	if( AUTH_que[quenum].result == 0 ){
		szData[0] = '\0';
	}else{
		sprintf( szData, "faild" );
	}

	char szBuffer[256];

	
	// 에러 메시지 테이블의 범위내라면 문자로 표시
	if( 0 <= AUTH_que[quenum].result && AUTH_que[quenum].result < AUTH_ERROR_END ){
		sprintf( szBuffer, "%s", aszAuthErrStr[AUTH_que[quenum].result] );
	}else{
		sprintf( szBuffer, "ExtraCode=%d", AUTH_que[quenum].result );
	}
	
	int iResultCode;
	iResultCode = AUTH_que[quenum].result;	// 디폴트는 기타의 에러

	
	// 최종적으로 클라이언트에게 돌려 주는 코드
	switch( iResultCode ){
		case 0://트랜잭션 오류
			iResultCode = AUTH_que[quenum].result;	// 디폴트는 기타의 에러
			break;
		case 1://삽입 성공
			iResultCode = AUTH_que[quenum].result;	// 디폴트는 기타의 에러
			break;
		/*case 2://캐릭터가 이미 다른 클랜에 소속
			iResultCode = AUTH_que[quenum].result;	// 디폴트는 기타의 에러
			break;
		case 3://클랜이름 중복
			iResultCode = AUTH_que[quenum].result;	// 디폴트는 기타의 에러
			break;*/
		case 100:				//인자값 에라
		iResultCode = AUTH_que[quenum].result;	// 디폴트는 기타의 에러
			break;
		case 1004:
		iResultCode = ANSdata.ret_val;
			break;
	case 101:				//클랜등록이 안됨.
	case 102:				//클랜장으로 등록이 안됨.
	case 103:				//클랜장으로 등록이 안됨.
		break;
//ygy
	//case AUTH_OK:
	//	break;
	default :
		iResultCode = AUTH_ERROR_OTHER;	// 기타의 에러
		break;
	}


	//ktj : 일단 삭제
	// 결과를 조사하고 돌려 준다．
	//ktj : 돌려주는부분을 넣어야함.
	/*
	nrproto_ClientLogin_send( 
			fd, 
			iResultCode,
			szData);
	//nrproto_ClientLogin_send( fd, 1, "OK" );
	*/
	ANSdata.ret_val = iResultCode;				//tjclan.cpp에 정의된 변수에 답변을 넣는다.
	fd2("\305\254\267\243\270\342\271\366\277\251\272\316\300\307 \264\344\272\257\300\273 \271\336\300\275. :  ret_val %d  ", ANSdata.ret_val );

	


	// 범위내에 들어가고 이르다 OK.
	if( 0 <= AUTH_que[quenum].result && AUTH_que[quenum].result < AUTH_ERROR_END ){
	}else{
		AUTH_que[quenum].result = AUTH_ERROR_OTHER;
	}
	return TRUE;
}





/*///////////////////////////////////////
: 함 수 명 :  
: 하 는 일 :  클랜이름이 중복인지 체크
: 서    식 :
: 인    자 :
/////////////////////////////////////////*/
int WebDB_Thread::isCheckClanName(char *ClanName,char *gserver)
{
	AUTH_QUE AuthQueTmp;
	ZeroMemory(&AuthQueTmp,sizeof(AUTH_QUE));

	AuthQueTmp.fdid = 100;
	AuthQueTmp.command = AUTH_QUE_CMD_isCheckClanName;          // 인증하다
		
	strncpy( AuthQueTmp.clanName, ClanName, sizeof( AuthQueTmp.clanName ) );	
	strncpy( AuthQueTmp.gserver, gserver, sizeof( AuthQueTmp.gserver ) );

	long myTime;
	time( &myTime );
	AuthQueTmp.howmanyTime = myTime;
	
	// 큐를 등록
	if( AUTH_addQue( &AuthQueTmp ) < 0 ){
		// 실패하면 에러를 돌려 준다．기타의 에러．
		//nrproto_CharLogin_send( fd, AUTH_ERROR_OTHER, "authorization required\n" );

		//ktj : 프테때문에 일단 삭제
		//nrproto_ClientLogin_send( fd, 0, "make_Clan() 에러\n" );
		fd2("make_Clan() \277\241\267\257   : \305\245\277\241 \265\356\267\317\276\310\265\312");
		return 0;
	}
	return 1;
}
int AUTH_proc_isCheckClanName( int quenum, int threadindex)
{
	return WebDB.AUTH_proc_isCheckClanName( quenum, threadindex);
}
int WebDB_Thread::AUTH_proc_isCheckClanName( int quenum, int threadindex)
{
	char* ClanName,*gserver;
	int knret = 1, iBadMsg = 0;

	ClanName = AUTH_que[quenum].clanName;	
	gserver= AUTH_que[quenum].gserver;
	
	
	knret = iCheck.isCheckClanName(ClanName,gserver);


	//ktj : 예제이므로 나중에 지우시오.
	//칼럼에 따른 데이타읽기
	//knret = iCheck.read_ClanName(0);
	//이미지읽기 : OK
	//knret = iCheck.write_ClanImageFile("a.bmp");
	///knret = iCheck.read_ClanImage("a.bmp");


	
	// 결과를 격납
	AUTH_que[quenum].result = knret;
	// 결과를 격납
	AUTH_que[quenum].badmsg = iBadMsg;
	return TRUE;
}


int AUTH_proc_isCheckClanName_Result( int quenum, int threadindex)
{
	return WebDB.AUTH_proc_isCheckClanName_Result(quenum, threadindex);
}
int WebDB_Thread::AUTH_proc_isCheckClanName_Result( int quenum, int threadindex)
{
	char *aszAuthErrStr[] = {
	"OK",
	"Err CDKEY", 
	"Err ISAO_ID", 
	"Err PASSWORD", 
	"Err EXPIRE", 
	"Err LOCOUT", 
	"Err ISAO_DB", 
	"Err OTHER", 
	""
	};

	char szData[128];
	
	if( AUTH_que[quenum].result == 0 ){
		szData[0] = '\0';
	}else{
		sprintf( szData, "faild" );
	}

	char szBuffer[256];

	
	// 에러 메시지 테이블의 범위내라면 문자로 표시
	if( 0 <= AUTH_que[quenum].result && AUTH_que[quenum].result < AUTH_ERROR_END ){
		sprintf( szBuffer, "%s", aszAuthErrStr[AUTH_que[quenum].result] );
	}else{
		sprintf( szBuffer, "ExtraCode=%d", AUTH_que[quenum].result );
	}
	
	int iResultCode;
	iResultCode = AUTH_que[quenum].result;	// 디폴트는 기타의 에러

	
	// 최종적으로 클라이언트에게 돌려 주는 코드
	switch( iResultCode ){
		case 0://클랜이름이 중복되지 않음
			iResultCode = AUTH_que[quenum].result;	// 디폴트는 기타의 에러
			break;
		case 1://클랜이름이 중복
			iResultCode = AUTH_que[quenum].result;	// 디폴트는 기타의 에러
			break;
		/*case 2://캐릭터가 이미 다른 클랜에 소속
			iResultCode = AUTH_que[quenum].result;	// 디폴트는 기타의 에러
			break;
		case 3://클랜이름 중복
			iResultCode = AUTH_que[quenum].result;	// 디폴트는 기타의 에러
			break;*/
		case 100:				//인자값 에라
		iResultCode = AUTH_que[quenum].result;	// 디폴트는 기타의 에러
			break;
		case 1004:
		iResultCode = ANSdata.ret_val;
			break;
	case 101:				//클랜등록이 안됨.
	case 102:				//클랜장으로 등록이 안됨.
	case 103:				//클랜장으로 등록이 안됨.
		break;
//ygy
	//case AUTH_OK:
	//	break;
	default :
		iResultCode = AUTH_ERROR_OTHER;	// 기타의 에러
		break;
	}


	//ktj : 일단 삭제
	// 결과를 조사하고 돌려 준다．
	//ktj : 돌려주는부분을 넣어야함.
	/*
	nrproto_ClientLogin_send( 
			fd, 
			iResultCode,
			szData);
	//nrproto_ClientLogin_send( fd, 1, "OK" );
	*/
	ANSdata.ret_val = iResultCode;				//tjclan.cpp에 정의된 변수에 답변을 넣는다.
	fd2("\305\254\267\243\270\342\271\366\277\251\272\316\300\307 \264\344\272\257\300\273 \271\336\300\275. :  ret_val %d  ", ANSdata.ret_val );

	


	// 범위내에 들어가고 이르다 OK.
	if( 0 <= AUTH_que[quenum].result && AUTH_que[quenum].result < AUTH_ERROR_END ){
	}else{
		AUTH_que[quenum].result = AUTH_ERROR_OTHER;
	}
	return TRUE;
}



/*///////////////////////////////////////
: 함 수 명 :  
: 하 는 일 :  클랜을 만드는함수
: 서    식 :
: 인    자 :
/////////////////////////////////////////*/
int AUTH_proc_ClanMake( int quenum, int threadindex)
{
	return WebDB.AUTH_proc_ClanMake( quenum, threadindex);
}
int WebDB_Thread::AUTH_proc_ClanMake( int quenum, int threadindex)
{
	//char *userId, *clanName, *expl, *userCid;
	char* ClanZang_ID,*ClanZang_ChaName,*gserver,*ClanName,*expl;
	int knret = 1, iBadMsg = 0;
	DWORD chtype;
	int level;

	ClanZang_ID = AUTH_que[quenum].ClanZang_ID;
	ClanZang_ChaName= AUTH_que[quenum].ClanZang_ChaName;
	ClanName = AUTH_que[quenum].clanName;
	expl = AUTH_que[quenum].expl;
	gserver= AUTH_que[quenum].gserver;
	chtype = AUTH_que[quenum].job_code;
	level = AUTH_que[quenum].level;
	
	
	knret = iCheck.makeClan(ClanZang_ID,gserver,ClanZang_ChaName,ClanName,expl,chtype,level,AUTH_que[quenum].ret_Buf);


	//ktj : 예제이므로 나중에 지우시오.
	//칼럼에 따른 데이타읽기
	//knret = iCheck.read_ClanName(0);
	//이미지읽기 : OK
	//knret = iCheck.write_ClanImageFile("a.bmp");
	///knret = iCheck.read_ClanImage("a.bmp");


	
	// 결과를 격납
	AUTH_que[quenum].result = knret;
	// 결과를 격납
	AUTH_que[quenum].badmsg = iBadMsg;
	return TRUE;
}
   

int AUTH_proc_ClanMake_Result( int quenum, int threadindex)
{
	return WebDB.AUTH_proc_ClanMake_Result(quenum, threadindex);
}
int WebDB_Thread::AUTH_proc_ClanMake_Result( int quenum, int threadindex)
{
	char *aszAuthErrStr[] = {
	"OK",
	"Err CDKEY", 
	"Err ISAO_ID", 
	"Err PASSWORD", 
	"Err EXPIRE", 
	"Err LOCOUT", 
	"Err ISAO_DB", 
	"Err OTHER", 
	""
	};

	char szData[128];
	
	if( AUTH_que[quenum].result == 0 ){
		szData[0] = '\0';
	}else{
		sprintf( szData, "faild" );
	}

	char szBuffer[256];

	
	// 에러 메시지 테이블의 범위내라면 문자로 표시
	if( 0 <= AUTH_que[quenum].result && AUTH_que[quenum].result < AUTH_ERROR_END ){
		sprintf( szBuffer, "%s", aszAuthErrStr[AUTH_que[quenum].result] );
	}else{
		sprintf( szBuffer, "ExtraCode=%d", AUTH_que[quenum].result );
	}
	
	int iResultCode;
	iResultCode = AUTH_que[quenum].result;	// 디폴트는 기타의 에러

	
	// 최종적으로 클라이언트에게 돌려 주는 코드
	switch( iResultCode ){
		case 0://트랜잭션 오류
			iResultCode = AUTH_que[quenum].result;	// 디폴트는 기타의 에러
			break;
		case 1://삽입 성공
			iResultCode = AUTH_que[quenum].result;	// 디폴트는 기타의 에러
			strcpy(ANSdata.ret_Buf, AUTH_que[quenum].ret_Buf); //클랜 비용을 가지고 온다.
			break;
		case 2://캐릭터가 이미 다른 클랜에 소속
			iResultCode = AUTH_que[quenum].result;	// 디폴트는 기타의 에러
			break;
		case 3://클랜이름 중복
			iResultCode = AUTH_que[quenum].result;	// 디폴트는 기타의 에러
			break;
		case 100:				//인자값 에라
		iResultCode = AUTH_que[quenum].result;	// 디폴트는 기타의 에러
			break;
		case 1004:
		iResultCode = ANSdata.ret_val;
			break;
	case 101:				//클랜등록이 안됨.
	case 102:				//클랜장으로 등록이 안됨.
	case 103:				//클랜장으로 등록이 안됨.
		break;
//ygy
	//case AUTH_OK:
	//	break;
	default :
		iResultCode = AUTH_ERROR_OTHER;	// 기타의 에러
		break;
	}


	//ktj : 일단 삭제
	// 결과를 조사하고 돌려 준다．
	//ktj : 돌려주는부분을 넣어야함.
	/*
	nrproto_ClientLogin_send( 
			fd, 
			iResultCode,
			szData);
	//nrproto_ClientLogin_send( fd, 1, "OK" );
	*/
	ANSdata.ret_val = iResultCode;				//tjclan.cpp에 정의된 변수에 답변을 넣는다.
	fd2("\305\254\267\243\270\342\271\366\277\251\272\316\300\307 \264\344\272\257\300\273 \271\336\300\275. :  ret_val %d  ", ANSdata.ret_val );

	


	// 범위내에 들어가고 이르다 OK.
	if( 0 <= AUTH_que[quenum].result && AUTH_que[quenum].result < AUTH_ERROR_END ){
	}else{
		AUTH_que[quenum].result = AUTH_ERROR_OTHER;
	}
	return TRUE;
}









/*/////////////////////////////////////////
: 함 수 명 :  
: 하 는 일 :  클랜가입시 멤버를 만드는함수
: 서    식 :
: 인    자 :
//////////////////////////////////////////*/
int AUTH_proc_Mmake( int quenum, int threadindex)
{
	return WebDB.AUTH_proc_Mmake(quenum, threadindex);
}
int WebDB_Thread::AUTH_proc_Mmake( int quenum, int threadindex)
{
	char *clanName, *memId, *memCharId;
	char *ClanZang_ID,*ClanZang_ChaName,*gserver,*ClanWon_ID,*ClanWon_ChaName,*chipflag;
	int knret = 1, iBadMsg = 0;
	int level =0;
	int joinlevel;
	DWORD job_code;


	clanName = AUTH_que[quenum].clanName;
	memId     = AUTH_que[quenum].id;				//임시로 아무버퍼에나 넣어둠.
	memCharId = AUTH_que[quenum].charId;
	ClanZang_ID = AUTH_que[quenum].ClanZang_ID;
	ClanZang_ChaName = AUTH_que[quenum].ClanZang_ChaName;
	gserver = AUTH_que[quenum].gserver;
	ClanWon_ID = AUTH_que[quenum].ClanWon_ID;
	ClanWon_ChaName = AUTH_que[quenum].ClanWon_ChaName;
	level = AUTH_que[quenum].level;
	job_code = AUTH_que[quenum].joinjobcode;
	joinlevel = AUTH_que[quenum].joinlevel;
	chipflag = AUTH_que[quenum].chipflag;
	

	//ygy
	//knret = iCheck.joinClanMember(clanName, memId, memCharId);
	//char *ClanZang_ID, char *gserver, char *ClanZang_ChaName,char* ClanName,char* ClanWon,char* ClanWon_ID
	knret = iCheck.joinClanMember(ClanZang_ID,gserver,ClanZang_ChaName,clanName,ClanWon_ChaName,ClanWon_ID,level,job_code,joinlevel,chipflag);
    
	// 결과를 격납
	AUTH_que[quenum].result = knret;
	// 결과를 격납
	AUTH_que[quenum].badmsg = iBadMsg;

	return TRUE;
}


int AUTH_proc_Mmake_Result( int quenum, int threadindex)
{
	return WebDB.AUTH_proc_Mmake_Result( quenum, threadindex);
}
int WebDB_Thread::AUTH_proc_Mmake_Result( int quenum, int threadindex)
{
	char szData[128];

	if( AUTH_que[quenum].result == 0 ){
		szData[0] = '\0';
	}else{
		sprintf( szData, "faild" );
	}

	int iResultCode;
	iResultCode = AUTH_que[quenum].result;	// 디폴트는 기타의 에러
	
	// 최종적으로 클라이언트에게 돌려 주는 코드
	switch( iResultCode ){
		case 0://트랜잭션 오류
			iResultCode = AUTH_que[quenum].result;	// 디폴트는 기타의 에러
			break;
		case 1://클랜원 가입 성공
			iResultCode = AUTH_que[quenum].result;	// 디폴트는 기타의 에러
			break;
		case 2://현재 가입시키려는 클랜원이 이미 다른 클랜에 가입해있을 경우(자기 클랜포함)
			iResultCode = AUTH_que[quenum].result;	// 디폴트는 기타의 에러
			break;
		case 3://클랜이 존재하지 않거나 클랜 장이 아닌경우
			iResultCode = AUTH_que[quenum].result;	// 디폴트는 기타의 에러
			break;
		case 4://클랜 멤버가 32 이상일 경우(Max : 32)
			iResultCode = AUTH_que[quenum].result;	// 디폴트는 기타의 에러
			break;
		case 100:
			iResultCode = AUTH_que[quenum].result;
			break;
		case 1004:
			iResultCode = AUTH_que[quenum].result;
			break;
	case 101:
	case 102:
	case 103:
		break;
//ygy
	//case AUTH_OK:
	//	break;
	default :
		iResultCode = AUTH_ERROR_OTHER;	// 기타의 에러
		break;
	}

	ANSdata.ret_val = iResultCode;				//tjclan.cpp에 정의된 변수에 답변을 넣는다.
	fd2("\305\254\267\243\270\342\271\366\277\251\272\316\300\307 \264\344\272\257\300\273 \271\336\300\275. :  ret_val %d  ", ANSdata.ret_val);

	// 범위내에 들어가고 이르다 OK.
	if( 0 <= AUTH_que[quenum].result && AUTH_que[quenum].result < AUTH_ERROR_END ){
	}else{
		AUTH_que[quenum].result = AUTH_ERROR_OTHER;
	}
	return 1;
}





/*/////////////////////////////////////////
: 함 수 명 :  
: 하 는 일 :  만들어진 클랜 데이타를 읽는다.
: 서    식 :
: 인    자 :
//////////////////////////////////////////*/

int AUTH_proc_ReadClan( int quenum, int threadindex)
{
	return WebDB.AUTH_proc_ReadClan(quenum, threadindex);
}
int WebDB_Thread::AUTH_proc_ReadClan( int quenum, int threadindex)
{
	char *ClanZang_ID,*ClanZang_ChaName,*gserver;
	int knret = 1, iBadMsg = 0;

	ClanZang_ID = AUTH_que[quenum].ClanZang_ID;
	ClanZang_ChaName = AUTH_que[quenum].ClanZang_ChaName;
	gserver = AUTH_que[quenum].gserver;
	
	//id에 해당하는 자가 클랜멤버라면, 100(클랜멤버), 101(클랜칲:클랜명을가져옴), 아니면 102(클랜에 들어있지않음)
	knret = iCheck.read_ClanData(ClanZang_ID,gserver,ClanZang_ChaName, AUTH_que[quenum].ret_Buf);


	// 결과를 격납
	AUTH_que[quenum].result = knret;
	// 결과를 격납
	AUTH_que[quenum].badmsg = iBadMsg;
	return TRUE;
}


int AUTH_proc_ReadClan_Result( int quenum, int threadindex)
{
	return WebDB.AUTH_proc_ReadClan_Result( quenum, threadindex);
}
int WebDB_Thread::AUTH_proc_ReadClan_Result( int quenum, int threadindex)
{
	char szData[128];

	if( AUTH_que[quenum].result == 0 ){
		szData[0] = '\0';
	}else{
		sprintf( szData, "faild" );
	}

	int iResultCode;
	iResultCode = AUTH_que[quenum].result;	// 디폴트는 기타의 에러
	
	// 최종적으로 클라이언트에게 돌려 주는 코드
	switch( iResultCode ){
		//ygy
		case 0: //가입한 클랜이 없음
			iResultCode = AUTH_que[quenum].result;	// 디폴트는 기타의 에러
			break;
		case 1://정상 출력
			iResultCode = AUTH_que[quenum].result;	// 디폴트는 기타의 에러
			strcpy(ANSdata.ret_Buf, AUTH_que[quenum].ret_Buf);
			break;
		case 100:				//일반 멤버,
			ANSdata.ret_val = iResultCode;
			break;
		case	1004:
			ANSdata.ret_val = iResultCode;
			break;
		case 101:				//클랜장일경우 속한 클랜이름도 알아냄.
		case 102:				//멤버아님.
			strcpy(ANSdata.ret_Buf, AUTH_que[quenum].ret_Buf);
			break;

			//ygy
		//case AUTH_OK:
		//	break;
		default :
			iResultCode = AUTH_ERROR_OTHER;	// 기타의 에러
			break;
	}


	//ktj : 일단 삭제
	// 결과를 조사하고 돌려 준다．
	//ktj : 돌려주는부분을 넣어야함.
	/*
	nrproto_ClientLogin_send( 
			fd, 
			iResultCode,
			szData);
	//nrproto_ClientLogin_send( fd, 1, "OK" );
	*/
	ANSdata.ret_val = iResultCode;				//tjclan.cpp에 정의된 변수에 답변을 넣는다.
	fd2("\305\254\267\243\270\342\271\366\277\251\272\316\300\307 \264\344\272\257\300\273 \271\336\300\275. :  ret_val %d  ", ANSdata.ret_val);

	

	// 범위내에 들어가고 이르다 OK.
	if( 0 <= AUTH_que[quenum].result && AUTH_que[quenum].result < AUTH_ERROR_END ){
	}else{
		AUTH_que[quenum].result = AUTH_ERROR_OTHER;
	}
	return TRUE;
}



/*/////////////////////////////////////////
: 함 수 명 :  
: 하 는 일 :  클랜을 해체한다.
: 서    식 :
: 인    자 :
//////////////////////////////////////////*/

int AUTH_proc_BreakUP( int quenum, int threadindex)
{
	return WebDB.AUTH_proc_BreakUP(quenum, threadindex);
}
int WebDB_Thread::AUTH_proc_BreakUP( int quenum, int threadindex)
{
	char *ClanZang_ID,*ClanZang_ChaName,*gserver,*ClanName;
	int knret = 1, iBadMsg = 0;

	ClanName = AUTH_que[quenum].clanName;
	ClanZang_ID = AUTH_que[quenum].ClanZang_ID;
	ClanZang_ChaName = AUTH_que[quenum].ClanZang_ChaName;
	gserver= AUTH_que[quenum].gserver;
	
	//id에 해당하는 자가 클랜멤버라면, 100(클랜멤버), 101(클랜칲:클랜명을가져옴), 아니면 102(클랜에 들어있지않음)
	knret = iCheck.BreakUPClan(ClanZang_ID,gserver,ClanZang_ChaName,ClanName);

	// 결과를 격납
	AUTH_que[quenum].result = knret;
	// 결과를 격납
	AUTH_que[quenum].badmsg = iBadMsg;
	return TRUE;
}


int AUTH_proc_BreakUP_Result( int quenum, int threadindex)
{
	return WebDB.AUTH_proc_BreakUP_Result( quenum, threadindex);
}
int WebDB_Thread::AUTH_proc_BreakUP_Result( int quenum, int threadindex)
{
	char szData[128];

	if( AUTH_que[quenum].result == 0 ){
		szData[0] = '\0';
	}else{
		sprintf( szData, "faild" );
	}

	int iResultCode;
	iResultCode = AUTH_que[quenum].result;	// 디폴트는 기타의 에러
	
	// 최종적으로 클라이언트에게 돌려 주는 코드
	switch( iResultCode ){
		//ygy
		//case AUTH_OK:
		//break;
		case 0:
			iResultCode = AUTH_que[quenum].result;
			break;
		case 1: //삭제 성공
			iResultCode = AUTH_que[quenum].result;
			break;
		case 2: //현재 캐릭명이 현재 클랜의 장이 아니거나 해당클랜이 없음
			iResultCode = AUTH_que[quenum].result;
			break;
			//end ygy
	case 100:				//지우기성공
		iResultCode = AUTH_que[quenum].result;
		break;
	case 1004:
		iResultCode = AUTH_que[quenum].result;
		break;
	case 101:				//clanWon테이블에 클랜원있는데 clanWon테이블에 없는경우(치명에러)
	case 102:				//clanWon테이블에서 클랜원삭제실패>
	case 103:				//clanName안에 클랜 네임이 존재하지않을경우
	case 104:				//clanName테이블에서 클랜삭제 실패
		break;

	
	default :
		iResultCode = AUTH_ERROR_OTHER;	// 기타의 에러
		break;
	}


	//ktj : 일단 삭제
	// 결과를 조사하고 돌려 준다．
	//ktj : 돌려주는부분을 넣어야함.
	/*
	nrproto_ClientLogin_send( 
			fd, 
			iResultCode,
			szData);
	//nrproto_ClientLogin_send( fd, 1, "OK" );
	*/
	//ygy
	ANSdata.ret_val = iResultCode;				//tjclan.cpp에 정의된 변수에 답변을 넣는다.
	fd2("\305\254\267\243\270\342\271\366\277\251\272\316\300\307 \264\344\272\257\300\273 \271\336\300\275. :  ret_val %d  ", ANSdata.ret_val);

	

	// 범위내에 들어가고 이르다 OK.
	if( 0 <= AUTH_que[quenum].result && AUTH_que[quenum].result < AUTH_ERROR_END ){
	}else{
		AUTH_que[quenum].result = AUTH_ERROR_OTHER;
	}
	return TRUE;
}






/*/////////////////////////////////////////
: 함 수 명 :  
: 하 는 일 :  특정id가 클랜에서 탈퇴
: 서    식 :
: 인    자 :
//////////////////////////////////////////*/

int AUTH_proc_Secession( int quenum, int threadindex)
{
	return WebDB.AUTH_proc_Secession(quenum, threadindex);
}
int WebDB_Thread::AUTH_proc_Secession( int quenum, int threadindex)
{
	char *clanName, *secession_id,*gserver,*clanzang;
	int knret = 1, iBadMsg = 0;

	clanName = AUTH_que[quenum].clanName;
	//ygy
	//banish_id = AUTH_que[quenum].expl;
	clanzang = AUTH_que[quenum].ClanWon_ChaName;
	gserver = AUTH_que[quenum].gserver;
	secession_id = AUTH_que[quenum].ClanWon_ID;

	/*
	strncpy( AuthQueTmp.clanName, claname, sizeof( AuthQueTmp.clanName ) );
	strncpy( AuthQueTmp.gserver, gserver, sizeof( AuthQueTmp.gserver ) );
	strncpy( AuthQueTmp.ClanWon, SecessionID, sizeof( AuthQueTmp.ClanWon ) );
	strncpy( AuthQueTmp.ClanZang, clanzang, sizeof( AuthQueTmp.ClanZang ) );
	*/

	
	//id에 해당하는 자가 클랜멤버라면, 100(클랜멤버), 101(클랜칲:클랜명을가져옴), 아니면 102(클랜에 들어있지않음)
	knret = iCheck.Secession(secession_id,gserver,clanzang ,clanName);
	//int ISAOCHECK::Secession(char* clanzang,char* gserver,char* SecessionID,char* claname)

	// 결과를 격납
	AUTH_que[quenum].result = knret;
	// 결과를 격납
	AUTH_que[quenum].badmsg = iBadMsg;
	return TRUE;
}


int AUTH_proc_Secession_Result( int quenum, int threadindex)
{
	return WebDB.AUTH_proc_Secession_Result( quenum, threadindex);
}
int WebDB_Thread::AUTH_proc_Secession_Result( int quenum, int threadindex)
{
	char szData[128];

	if( AUTH_que[quenum].result == 0 ){
		szData[0] = '\0';
	}else{
		sprintf( szData, "faild" );
	}

	int iResultCode;
	iResultCode = AUTH_que[quenum].result;	// 디폴트는 기타의 에러
	
	// 최종적으로 클라이언트에게 돌려 주는 코드
	switch( iResultCode ){
	//ygy
	case 0: //트랜잭션 오류
			iResultCode = AUTH_que[quenum].result;
			break;
	case 1: //클랜원 탈퇴 성공
			iResultCode = AUTH_que[quenum].result;
			break;
	case 2://클랜이 존재하지 않거나 클랜 장이 아닌경우 또는 클랜원이 아닌경우
			iResultCode = AUTH_que[quenum].result;
			break;
	case 3://탈퇴는 성공했으나 클랜회원수가 6명이하로 떨어졌을 경우(LimitDate 설정)
			iResultCode = AUTH_que[quenum].result;
			break;
	case 100:	
			iResultCode = AUTH_que[quenum].result;
			break;			//추방 에러
	case ISAO_ERR_TCP_CONNECT: //서버 접속에러
			iResultCode = AUTH_que[quenum].result;
			break;
	case 101:				//추방불가
		break;
     //ygy
	//case AUTH_OK:
	//	break;
	default :
		iResultCode = AUTH_ERROR_OTHER;	// 기타의 에러
		break;
	}


	//ktj : 일단 삭제
	// 결과를 조사하고 돌려 준다．
	//ktj : 돌려주는부분을 넣어야함.
	/*
	nrproto_ClientLogin_send( 
			fd, 
			iResultCode,
			szData);
	//nrproto_ClientLogin_send( fd, 1, "OK" );
	*/
	ANSdata.ret_val = iResultCode;				//tjclan.cpp에 정의된 변수에 답변을 넣는다.
	fd2("\305\254\267\243\270\342\271\366\277\251\272\316\300\307 \264\344\272\257\300\273 \271\336\300\275. :  ret_val %d  ", ANSdata.ret_val);
	

	// 범위내에 들어가고 이르다 OK.
	if( 0 <= AUTH_que[quenum].result && AUTH_que[quenum].result < AUTH_ERROR_END ){
	}else{
		AUTH_que[quenum].result = AUTH_ERROR_OTHER;
	}
	return TRUE;
}





/*/////////////////////////////////////////
: 함 수 명 :  
: 하 는 일 :  특정id를 클랜에서 추방한다.
: 서    식 :
: 인    자 :
//////////////////////////////////////////*/

int AUTH_proc_Banishiment( int quenum, int threadindex)
{
	return WebDB.AUTH_proc_Banishiment(quenum, threadindex);
}
int WebDB_Thread::AUTH_proc_Banishiment( int quenum, int threadindex)
{
	char *clanName, *ClanWon,*gserver,*ClanZang_ID,*ClanZang_ChaName;
	int knret = 1, iBadMsg = 0;

	clanName = AUTH_que[quenum].clanName;
	ClanZang_ID = AUTH_que[quenum].ClanZang_ID;
	ClanZang_ChaName = AUTH_que[quenum].ClanZang_ChaName;
	gserver = AUTH_que[quenum].gserver;
	ClanWon = AUTH_que[quenum].ClanWon_ChaName;

	
	//id에 해당하는 자가 클랜멤버라면, 100(클랜멤버), 101(클랜칲:클랜명을가져옴), 아니면 102(클랜에 들어있지않음)
	knret = iCheck.Banishiment(ClanZang_ID,gserver,ClanZang_ChaName,clanName,ClanWon);

	// 결과를 격납
	AUTH_que[quenum].result = knret;
	// 결과를 격납
	AUTH_que[quenum].badmsg = iBadMsg;
	return TRUE;
}


int AUTH_proc_Banishiment_Result( int quenum, int threadindex)
{
	return WebDB.AUTH_proc_Banishiment_Result( quenum, threadindex);
}
int WebDB_Thread::AUTH_proc_Banishiment_Result( int quenum, int threadindex)
{
	char szData[128];

	if( AUTH_que[quenum].result == 0 ){
		szData[0] = '\0';
	}else{
		sprintf( szData, "faild" );
	}

	int iResultCode;
	iResultCode = AUTH_que[quenum].result;	// 디폴트는 기타의 에러
	
	// 최종적으로 클라이언트에게 돌려 주는 코드
	switch( iResultCode ){
		//ygy
		case 0: //트랜잭션 오류
			iResultCode = AUTH_que[quenum].result;
			break;
		case 1: //클랜원 탈퇴 성공
			iResultCode = AUTH_que[quenum].result;
			break;
		case 2://클랜이 존재하지 않거나 클랜 장이 아닌경우 또는 클랜원이 아닌경우
			iResultCode = AUTH_que[quenum].result;
			break;
		case 3://탈퇴는 성공했으나 클랜회원수가 6명이하로 떨어졌을 경우(LimitDate 설정)
			iResultCode = AUTH_que[quenum].result;
			break;
	case 100:				//추방완료
		iResultCode = AUTH_que[quenum].result;
		break;
	case 1004:
		iResultCode = AUTH_que[quenum].result;
		break;
	case 101:				//추방불가
		break;
     //ygy
	//case AUTH_OK:
	//	break;
	default :
		iResultCode = AUTH_ERROR_OTHER;	// 기타의 에러
		break;
	}


	//ktj : 일단 삭제
	// 결과를 조사하고 돌려 준다．
	//ktj : 돌려주는부분을 넣어야함.
	/*
	nrproto_ClientLogin_send( 
			fd, 
			iResultCode,
			szData);
	//nrproto_ClientLogin_send( fd, 1, "OK" );
	*/
	ANSdata.ret_val = iResultCode;				//tjclan.cpp에 정의된 변수에 답변을 넣는다.
	fd2("\305\254\267\243\270\342\271\366\277\251\272\316\300\307 \264\344\272\257\300\273 \271\336\300\275. :  ret_val %d  ", ANSdata.ret_val);
	

	// 범위내에 들어가고 이르다 OK.
	if( 0 <= AUTH_que[quenum].result && AUTH_que[quenum].result < AUTH_ERROR_END ){
	}else{
		AUTH_que[quenum].result = AUTH_ERROR_OTHER;
	}
	return TRUE;
}





/*/////////////////////////////////////////
: 함 수 명 :  
: 하 는 일 :  특정id를 클랜에서 추방한다.
: 서    식 :
: 인    자 :
//////////////////////////////////////////*/

int AUTH_proc_WarhouseR( int quenum, int threadindex)
{
	return WebDB.AUTH_proc_WarhouseR(quenum, threadindex);
}
int WebDB_Thread::AUTH_proc_WarhouseR( int quenum, int threadindex)
{
	char *clanName, *banish_id;
	int knret = 1, iBadMsg = 0;

	clanName = AUTH_que[quenum].clanName;
	banish_id = AUTH_que[quenum].expl;
	
	//id에 해당하는 자가 클랜멤버라면, 100(클랜멤버), 101(클랜칲:클랜명을가져옴), 아니면 102(클랜에 들어있지않음)
	knret = iCheck.WarhouseRight(clanName, banish_id);

	// 결과를 격납
	AUTH_que[quenum].result = knret;
	// 결과를 격납
	AUTH_que[quenum].badmsg = iBadMsg;
	return TRUE;
}


int AUTH_proc_WarhouseR_Result( int quenum, int threadindex)
{
	return WebDB.AUTH_proc_WarhouseR_Result( quenum, threadindex);
}
int WebDB_Thread::AUTH_proc_WarhouseR_Result( int quenum, int threadindex)
{
	char szData[128];

	if( AUTH_que[quenum].result == 0 ){
		szData[0] = '\0';
	}else{
		sprintf( szData, "faild" );
	}

	int iResultCode;
	iResultCode = AUTH_que[quenum].result;	// 디폴트는 기타의 에러
	
	// 최종적으로 클라이언트에게 돌려 주는 코드
	switch( iResultCode ){
	case 100:				//완료
	case 101:				//
	case 102:				//
		break;

	case AUTH_OK:
		break;
	default :
		iResultCode = AUTH_ERROR_OTHER;	// 기타의 에러
		break;
	}


	//ktj : 일단 삭제
	// 결과를 조사하고 돌려 준다．
	//ktj : 돌려주는부분을 넣어야함.
	/*
	nrproto_ClientLogin_send( 
			fd, 
			iResultCode,
			szData);
	//nrproto_ClientLogin_send( fd, 1, "OK" );
	*/
	ANSdata.ret_val = iResultCode;				//tjclan.cpp에 정의된 변수에 답변을 넣는다.
	fd2("\303\242\260\355\261\307\307\321 \264\344\272\257\300\273 \271\336\300\275. :  ret_val %d  ", ANSdata.ret_val);
	

	// 범위내에 들어가고 이르다 OK.
	if( 0 <= AUTH_que[quenum].result && AUTH_que[quenum].result < AUTH_ERROR_END ){
	}else{
		AUTH_que[quenum].result = AUTH_ERROR_OTHER;
	}
	return TRUE;
}




/*/////////////////////////////////////////
: 함 수 명 :  
: 하 는 일 :  클랜결성끝 플래그 셋팅
: 서    식 :
: 인    자 :
//////////////////////////////////////////*/

int AUTH_proc_updateFlag( int quenum, int threadindex)
{
	return WebDB.AUTH_proc_updateFlag(quenum, threadindex);
}
int WebDB_Thread::AUTH_proc_updateFlag( int quenum, int threadindex)
{
	char *clanName, *banish_id;
	int knret = 1, iBadMsg = 0;

	clanName = AUTH_que[quenum].clanName;
	banish_id = AUTH_que[quenum].expl;
	
	knret = iCheck.updateFlag(clanName);

	// 결과를 격납
	AUTH_que[quenum].result = knret;
	// 결과를 격납
	AUTH_que[quenum].badmsg = iBadMsg;
	return TRUE;
}

int AUTH_proc_updateFlag_Result( int quenum, int threadindex)
{
	return WebDB.AUTH_proc_updateFlag_Result( quenum, threadindex);
}
int WebDB_Thread::AUTH_proc_updateFlag_Result( int quenum, int threadindex)
{
	char szData[128];

	if( AUTH_que[quenum].result == 0 ){
		szData[0] = '\0';
	}else{
		sprintf( szData, "faild" );
	}

	int iResultCode;
	iResultCode = AUTH_que[quenum].result;	// 디폴트는 기타의 에러
	
	// 최종적으로 클라이언트에게 돌려 주는 코드
	switch( iResultCode ){
	case 100:				//완료
	case 101:				//클랜없음.
	case 102:				//
		break;

	case AUTH_OK:
		iResultCode = AUTH_que[quenum].result;
		break;
	default :
		iResultCode = AUTH_ERROR_OTHER;	// 기타의 에러
		break;
	}


	//ktj : 일단 삭제
	// 결과를 조사하고 돌려 준다．
	//ktj : 돌려주는부분을 넣어야함.
	/*
	nrproto_ClientLogin_send( 
			fd, 
			iResultCode,
			szData);
	//nrproto_ClientLogin_send( fd, 1, "OK" );
	*/
	ANSdata.ret_val = iResultCode;				//tjclan.cpp에 정의된 변수에 답변을 넣는다.
	fd2("\303\242\260\355\261\307\307\321 \264\344\272\257\300\273 \271\336\300\275. :  ret_val %d  ", ANSdata.ret_val);
	

	// 범위내에 들어가고 이르다 OK.
	if( 0 <= AUTH_que[quenum].result && AUTH_que[quenum].result < AUTH_ERROR_END ){
	}else{
		AUTH_que[quenum].result = AUTH_ERROR_OTHER;
	}
	return TRUE;
}








/*/////////////////////////////////////////
: 함 수 명 :  
: 하 는 일 :  클랜마크읽기
: 서    식 :
: 인    자 :
//////////////////////////////////////////*/

int AUTH_proc_MarkREAD( int quenum, int threadindex)
{
	return WebDB.AUTH_proc_MarkREAD(quenum, threadindex);
}
int WebDB_Thread::AUTH_proc_MarkREAD( int quenum, int threadindex)
{
	//char *clanName, *banish_id;
	char *Mark,*clanname;
	int knret = 1, iBadMsg = 0;

	Mark = AUTH_que[quenum].Mark;
	clanname = AUTH_que[quenum].clanName;
	//banish_id = AUTH_que[quenum].expl;
	
	knret = iCheck.read_ClanImage(clanname,Mark);

	// 결과를 격납
	AUTH_que[quenum].result = knret;
	// 결과를 격납
	AUTH_que[quenum].badmsg = iBadMsg;
	return TRUE;
}

int AUTH_proc_MarkREAD_Result( int quenum, int threadindex)
{
	return WebDB.AUTH_proc_MarkREAD_Result( quenum, threadindex);
}
int WebDB_Thread::AUTH_proc_MarkREAD_Result( int quenum, int threadindex)
{
	char szData[128];

	if( AUTH_que[quenum].result == 0 ){
		szData[0] = '\0';
	}else{
		sprintf( szData, "faild" );
	}

	int iResultCode;	

	//ktj : 임시 실패
	//AUTH_que[quenum].result = 2;

	iResultCode = AUTH_que[quenum].result;	// 디폴트는 기타의 에러
	
	
	// 최종적으로 클라이언트에게 돌려 주는 코드
	switch( iResultCode ){
	case 1: //클랜 마크 읽기 성공
			
		iResultCode =AUTH_que[quenum].result ;	// 디폴트는 기타의 에러
		break;
	case 2://클랜 마크 읽기 실패
		iResultCode = AUTH_que[quenum].result ;	// 디폴트는 기타의 에러
		break;

	case 100:				//완료
		iResultCode = AUTH_que[quenum].result;
		break;
	case 1004:
		iResultCode = AUTH_que[quenum].result;
		break;
	case 101:				//클랜없음.
	case 102:				//
		break;
	default:
		iResultCode = AUTH_ERROR_OTHER;	// 기타의 에러
		break;

//	case AUTH_OK:
//		break;
	//default :
	//	iResultCode = AUTH_que[quenum].result;	// 디폴트는 기타의 에러
	//	AUTH_que[quenum].result = iResultCode;	// 디폴트는 기타의 에러
		break;
	}


	//ktj : 일단 삭제
	// 결과를 조사하고 돌려 준다．
	//ktj : 돌려주는부분을 넣어야함.
	/*
	nrproto_ClientLogin_send( 
			fd, 
			iResultCode,
			szData);
	//nrproto_ClientLogin_send( fd, 1, "OK" );
	*/
	ANSdata.ret_val = iResultCode;				//tjclan.cpp에 정의된 변수에 답변을 넣는다.
	fd2("\303\242\260\355\261\307\307\321 \264\344\272\257\300\273 \271\336\300\275. :  ret_val %d  ", ANSdata.ret_val);
	

	// 범위내에 들어가고 이르다 OK.
	if( 0 <= AUTH_que[quenum].result && AUTH_que[quenum].result < AUTH_ERROR_END ){
	}else{
		AUTH_que[quenum].result = AUTH_ERROR_OTHER;
	}
	return TRUE;
}

/*/////////////////////////////////////////
: 함 수 명 :  
: 하 는 일 :  다른 클랜마크읽기
: 서    식 :
: 인    자 :
//////////////////////////////////////////*/

int AUTH_proc_MarkREAD2( int quenum, int threadindex)
{
	return WebDB.AUTH_proc_MarkREAD2(quenum, threadindex);
}
int WebDB_Thread::AUTH_proc_MarkREAD2( int quenum, int threadindex)
{
	//char *clanName, *banish_id;
	char *Mark,*clanname;
	int knret = 1, iBadMsg = 0;

	Mark = AUTH_que[quenum].Mark;
	clanname = AUTH_que[quenum].clanName;
	//banish_id = AUTH_que[quenum].expl;
	
	//knret = iCheck.read_ClanImage2(clanname,Mark);





	// 결과를 격납
	AUTH_que[quenum].result = knret;
	// 결과를 격납
	AUTH_que[quenum].badmsg = iBadMsg;
	return TRUE;
}

int AUTH_proc_MarkREAD2_Result( int quenum, int threadindex)
{
	return WebDB.AUTH_proc_MarkREAD2_Result( quenum, threadindex);
}
int WebDB_Thread::AUTH_proc_MarkREAD2_Result( int quenum, int threadindex)
{
	char szData[128];

	if( AUTH_que[quenum].result == 0 ){
		szData[0] = '\0';
	}else{
		sprintf( szData, "faild" );
	}

	int iResultCode;	
	iResultCode = AUTH_que[quenum].result;	// 디폴트는 기타의 에러
	
	// 최종적으로 클라이언트에게 돌려 주는 코드
	switch( iResultCode ){
		case 1: //클랜 마크 읽기 성공
			
			iResultCode =AUTH_que[quenum].result ;	// 디폴트는 기타의 에러
			break;
		case 2://클랜 마크 읽기 실패
			iResultCode = AUTH_que[quenum].result ;	// 디폴트는 기타의 에러
			break;
	case 100:				//완료
		iResultCode = AUTH_que[quenum].result;
		break;
	case 1004:
		iResultCode = AUTH_que[quenum].result;
		break;
	case 101:				//클랜없음.
	case 102:				//
		break;
	default:
		iResultCode = AUTH_ERROR_OTHER;	// 기타의 에러
		break;

//	case AUTH_OK:
//		break;
	//default :
	//	iResultCode = AUTH_que[quenum].result;	// 디폴트는 기타의 에러
	//	AUTH_que[quenum].result = iResultCode;	// 디폴트는 기타의 에러
	//	break;
	}


	//ktj : 일단 삭제
	// 결과를 조사하고 돌려 준다．
	//ktj : 돌려주는부분을 넣어야함.
	/*
	nrproto_ClientLogin_send( 
			fd, 
			iResultCode,
			szData);
	//nrproto_ClientLogin_send( fd, 1, "OK" );
	*/
	ANSdata.ret_val = iResultCode;				//tjclan.cpp에 정의된 변수에 답변을 넣는다.
	fd2("\303\242\260\355\261\307\307\321 \264\344\272\257\300\273 \271\336\300\275. :  ret_val %d  ", ANSdata.ret_val);
	

	// 범위내에 들어가고 이르다 OK.
	if( 0 <= AUTH_que[quenum].result && AUTH_que[quenum].result < AUTH_ERROR_END ){
	}else{
		AUTH_que[quenum].result = AUTH_ERROR_OTHER;
	}
	return TRUE;
}



int AUTH_proc_Updatcldata( int quenum, int threadindex)
{
	return WebDB.AUTH_proc_Updatcldata(quenum, threadindex);
}

int WebDB_Thread::AUTH_proc_Updatcldata(int quenum, int threadindex)
{
	char *id,*chaname,*gserver;
	int knret = 1, iBadMsg = 0;

	//id = AUTH_que[quenum].id;
	id = AUTH_que[quenum].ClanZang_ID;
	chaname = AUTH_que[quenum].ClanZang_ChaName;
	gserver = AUTH_que[quenum].gserver;
	
	//id에 해당하는 자가 클랜멤버라면, 100(클랜멤버), 101(클랜칲:클랜명을가져옴), 아니면 102(클랜에 들어있지않음)
	knret = iCheck.isClanMember(id,gserver,chaname, AUTH_que[quenum].ret_Buf);
	if((knret == 0)||(knret == 2)||(knret == 1)||(knret == 3)||(knret == 4)||(knret == 5)){
		void ParsingIsClanMember(char* data,int myposition);
		ParsingIsClanMember(AUTH_que[quenum].ret_Buf,knret);
		if((knret == 2)||(knret == 1)||(knret == 5)){
			knret=iCheck.read_ClanData(id,gserver,chaname, AUTH_que[quenum].ret_Buf);
			if(knret == 1){
				void ParsigClanWon(char* data);
				ParsigClanWon(AUTH_que[quenum].ret_Buf);
				knret = iCheck.read_ClanImage(chaname,cldata.ClanMark);
			}
		}
	}


	// 결과를 격납
	AUTH_que[quenum].result = knret;
	// 결과를 격납
	AUTH_que[quenum].badmsg = iBadMsg;

	return 1;
}

int AUTH_proc_Updatcldata_Result( int quenum, int threadindex)
{
	return WebDB.AUTH_proc_Updatcldata_Result(quenum, threadindex);
}

int WebDB_Thread::AUTH_proc_Updatcldata_Result( int quenum, int threadindex)
{
	char szData[128];
	char GServer;
	char szNum[64];
	char szFile[256];
	char szFile2[256];
	ZeroMemory(szNum,sizeof(szNum));
	ZeroMemory(szFile,sizeof(szFile));
	ZeroMemory(szFile2,sizeof(szFile2));

	if( AUTH_que[quenum].result == 0 ){
		szData[0] = '\0';
	}else{
		sprintf( szData, "faild" );
	}

	int iResultCode;
	iResultCode = AUTH_que[quenum].result;	// 디폴트는 기타의 에러
	
	// 최종적으로 클라이언트에게 돌려 주는 코드
	switch( iResultCode ){
		//ygy
		case 0: //가입한 클랜이 없음
			iResultCode = AUTH_que[quenum].result;	// 디폴트는 기타의 에러
			break;
		case 1://정상 출력
			iResultCode = AUTH_que[quenum].result;	// 디폴트는 기타의 에러			
			GServer = cldata.ClanMark[0];
			strcpy(szNum , &cldata.ClanMark[3]);
			wsprintf(szFile,"%s\\%s\\%c_%s.%s",SAVE_DIR,szConnServerName,GServer,szNum,SAVE_EXE );
			wsprintf(szFile2,"%s\\%s\\%c_%s_16.%s",SAVE_DIR,szConnServerName,GServer,szNum,SAVE_EXE);
#ifdef  USE_PROSTONTALE						
			if(cldata.hClanMark16) {
				cldata.hClanMark16->Release();
				cldata.hClanMark16 = NULL;
			}

			if(cldata.hClanMark){
				cldata.hClanMark->Release();
				cldata.hClanMark = NULL;
			}

			cldata.hClanMark16= LoadDibSurfaceOffscreen(szFile2);
			cldata.hClanMark= LoadDibSurfaceOffscreen(szFile);
#endif

			break;
		case 100:				//일반 멤버,
			ANSdata.ret_val = iResultCode;
			break;
		case	1004:
			ANSdata.ret_val = iResultCode;
			break;
		case 101:				//클랜장일경우 속한 클랜이름도 알아냄.
		case 102:				//멤버아님.			
			break;

			//ygy
		//case AUTH_OK:
		//	break;
		default :
			iResultCode = AUTH_ERROR_OTHER;	// 기타의 에러
			break;
	}


	//ktj : 일단 삭제
	// 결과를 조사하고 돌려 준다．
	//ktj : 돌려주는부분을 넣어야함.
	/*
	nrproto_ClientLogin_send( 
			fd, 
			iResultCode,
			szData);
	//nrproto_ClientLogin_send( fd, 1, "OK" );
	*/
	


	ANSdata.ret_val = iResultCode;				//tjclan.cpp에 정의된 변수에 답변을 넣는다.
	fd2("\305\254\267\243\270\342\271\366\277\251\272\316\300\307 \264\344\272\257\300\273 \271\336\300\275. :  ret_val %d  ", ANSdata.ret_val);

	

	// 범위내에 들어가고 이르다 OK.
	if( 0 <= AUTH_que[quenum].result && AUTH_que[quenum].result < AUTH_ERROR_END ){
	}else{
		AUTH_que[quenum].result = AUTH_ERROR_OTHER;
	}


	//cE_Updatecldata.cpp와 관련
	void g_UpdateCldata_Close(); //쓰레드 죽임
	g_UpdateCldata_Close(); //쓰레드 죽임

	return TRUE;
}

int WebDB_Thread::Updatcldata(char *id, char *gserver, char *chaname)
{
	AUTH_QUE AuthQueTmp;
	ZeroMemory(&AuthQueTmp,sizeof(AUTH_QUE));


	AuthQueTmp.fdid = 100;
	AuthQueTmp.command = AUTH_QUE_CMD_Updatcldata;          // 클랜멤버인지를 알려달라..	
	
	//ygy
	//strncpy( AuthQueTmp.id, id, sizeof( AuthQueTmp.id ) );
	strncpy( AuthQueTmp.ClanZang_ID, id, sizeof( AuthQueTmp.ClanZang_ID ) );
	strncpy( AuthQueTmp.ClanZang_ChaName, chaname, sizeof( AuthQueTmp.ClanZang_ChaName ) );
	strncpy( AuthQueTmp.gserver, gserver, sizeof( AuthQueTmp.gserver) );
		
	

	//큐에 등록
	if( AUTH_addQue( &AuthQueTmp ) < 0 ){
		// 실패하면 에러를 돌려 준다．기타의 에러．
		//nrproto_CharLogin_send( fd, AUTH_ERROR_OTHER, "authorization required\n" );

		//ktj : 프테때문에 일단 삭제
		//nrproto_ClientLogin_send( fd, 0, "make_Clan() 에러\n" );
		fd2("make_Clan() \277\241\267\257   : \305\245\277\241 \265\356\267\317\276\310\265\312");
		return 0;
	}
	return 1;
}

/*/////////////////////////////////////////////////////////////////////////// END
: 하 는 일 :  웹db에 접속하는 명령처리 함수 
///////////////////////////////////////////////////////////////////////////*/















/*/////////////////////////////////////////////////////////////////////////// START
: 하 는 일 :  WebDB에 접속해서 할일을 지시하는 지시함수들.
///////////////////////////////////////////////////////////////////////////*/

//해당 id가 클랜원인지의 여부를 알아냄.
int WebDB_Thread::isClanMember(char *ClanZang_id, char* gserver,char* ClanZang_ChaName)
{
	AUTH_QUE AuthQueTmp;
	ZeroMemory(&AuthQueTmp,sizeof(AUTH_QUE));

	AuthQueTmp.fdid = 100;
	AuthQueTmp.command = AUTH_QUE_CMD_isClanMember;          // 클랜멤버인지를 알려달라..	
	
	//ygy
	//strncpy( AuthQueTmp.id, id, sizeof( AuthQueTmp.id ) );
	strncpy( AuthQueTmp.ClanZang_ID, ClanZang_id, sizeof( AuthQueTmp.ClanZang_ID ) );
	strncpy( AuthQueTmp.ClanZang_ChaName, ClanZang_ChaName, sizeof( AuthQueTmp.ClanZang_ChaName ) );
	strncpy( AuthQueTmp.gserver, gserver, sizeof( AuthQueTmp.gserver) );

	//큐에 등록
	if( AUTH_addQue( &AuthQueTmp ) < 0 ){
		// 실패하면 에러를 돌려 준다．기타의 에러．
		//nrproto_CharLogin_send( fd, AUTH_ERROR_OTHER, "authorization required\n" );

		//ktj : 프테때문에 일단 삭제
		//nrproto_ClientLogin_send( fd, 0, "make_Clan() 에러\n" );
		fd2("make_Clan() \277\241\267\257   : \305\245\277\241 \265\356\267\317\276\310\265\312");
		return 0;
	}
	return 1;
}




//클랜멤버로 가입시킨다.
int WebDB_Thread::joinClanMember(char *ClanZang_ID, char *gserver, char *ClanZang_ChaName,char* ClanName,char* ClanWon,char* ClanWon_ID,int level,DWORD chtype,int joinlevel,char *chipflag)
{
	AUTH_QUE AuthQueTmp;
	ZeroMemory(&AuthQueTmp, sizeof(AuthQueTmp));

	AuthQueTmp.fdid = 100;
	AuthQueTmp.command = AUTH_QUE_CMD_CL_MEMBER_MAKE;          // 클랜멤버로 가입

	//ygy	
	strncpy( AuthQueTmp.ClanZang_ID, ClanZang_ID, sizeof( AuthQueTmp.ClanZang_ID ) );
	strncpy( AuthQueTmp.gserver, gserver, sizeof( AuthQueTmp.gserver ) );
	strncpy( AuthQueTmp.ClanZang_ChaName, ClanZang_ChaName, sizeof( AuthQueTmp.ClanZang_ChaName ) );
	strncpy( AuthQueTmp.clanName, ClanName, sizeof( AuthQueTmp.clanName ) );
	strncpy( AuthQueTmp.ClanWon_ChaName, ClanWon, sizeof( AuthQueTmp.ClanWon_ChaName ) );
	strncpy( AuthQueTmp.ClanWon_ID, ClanWon_ID, sizeof( AuthQueTmp.ClanWon_ID ) );
	strncpy( AuthQueTmp.chipflag, chipflag, sizeof( AuthQueTmp.chipflag ) );
	
	AuthQueTmp.level = level;
	AuthQueTmp.joinlevel = joinlevel;
	AuthQueTmp.joinjobcode = (int)chtype;



	//큐에 등록
	if( AUTH_addQue( &AuthQueTmp ) < 0 ){
		// 실패하면 에러를 돌려 준다．기타의 에러．
		//nrproto_CharLogin_send( fd, AUTH_ERROR_OTHER, "authorization required\n" );

		//ktj : 프테때문에 일단 삭제
		//nrproto_ClientLogin_send( fd, 0, "make_Clan() 에러\n" );
		fd2("make_Clan() \277\241\267\257   : \305\245\277\241 \265\356\267\317\276\310\265\312");
		return 0;
	}
	return 1;
}





//클랜을 만들고싶다면 만든다.
int WebDB_Thread::make_Clan(char *ClanZang_ID, char* gserver,char* ClanZang_ChaName,char* ClanName,char *explanation,DWORD chtype,int level)
{
	AUTH_QUE AuthQueTmp;
	ZeroMemory(&AuthQueTmp,sizeof(AUTH_QUE));

	AuthQueTmp.fdid = 100;
	AuthQueTmp.command = AUTH_QUE_CMD_CL_MAKE;          // 인증하다
		
	strncpy( AuthQueTmp.ClanZang_ID, ClanZang_ID, sizeof( AuthQueTmp.ClanZang_ID ) );
	strncpy( AuthQueTmp.expl, explanation, sizeof( AuthQueTmp.expl ) );
	strncpy( AuthQueTmp.clanName, ClanName, sizeof( AuthQueTmp.clanName ) );
	strncpy( AuthQueTmp.ClanZang_ChaName, ClanZang_ChaName, sizeof( AuthQueTmp.ClanZang_ChaName ) );
	strncpy( AuthQueTmp.gserver, gserver, sizeof( AuthQueTmp.gserver ) );

	AuthQueTmp.level = level;
	AuthQueTmp.job_code = chtype;

	long myTime;
	time( &myTime );
	AuthQueTmp.howmanyTime = myTime;
	
	// 큐를 등록
	if( AUTH_addQue( &AuthQueTmp ) < 0 ){
		// 실패하면 에러를 돌려 준다．기타의 에러．
		//nrproto_CharLogin_send( fd, AUTH_ERROR_OTHER, "authorization required\n" );

		//ktj : 프테때문에 일단 삭제
		//nrproto_ClientLogin_send( fd, 0, "make_Clan() 에러\n" );
		fd2("make_Clan() \277\241\267\257   : \305\245\277\241 \265\356\267\317\276\310\265\312");
		return 0;
	}
	return 1;
}





//클랜을 읽는다.
int WebDB_Thread::ReadClan(char *ClanZang_ID,char* gserver,char* ClanZang_ChaName)
{
	AUTH_QUE AuthQueTmp;
	ZeroMemory(&AuthQueTmp,sizeof(AUTH_QUE));

	AuthQueTmp.fdid = 100;
	AuthQueTmp.command = AUTH_QUE_CMD_ReadClan;          // 인증하다
	
	//ygy	
	strncpy(AuthQueTmp.ClanZang_ID,ClanZang_ID,sizeof(AuthQueTmp.ClanZang_ID));
	strncpy(AuthQueTmp.gserver,gserver,sizeof(AuthQueTmp.gserver));
	strncpy(AuthQueTmp.ClanZang_ChaName,ClanZang_ChaName,sizeof(AuthQueTmp.ClanZang_ChaName));


	long myTime;
	time( &myTime );
	AuthQueTmp.howmanyTime = myTime;
	
	// 큐를 등록
	if( AUTH_addQue( &AuthQueTmp ) < 0 ){
		// 실패하면 에러를 돌려 준다．기타의 에러．
		//nrproto_CharLogin_send( fd, AUTH_ERROR_OTHER, "authorization required\n" );

		//ktj : 프테때문에 일단 삭제
		//nrproto_ClientLogin_send( fd, 0, "make_Clan() 에러\n" );
		fd2("make_Clan() \277\241\267\257   : \305\245\277\241 \265\356\267\317\276\310\265\312");
		return 0;
	}
	return 1;
}


//클랜을 헤체한다.
int WebDB_Thread::BreakUP_Clan(char *ClanZang_ID,char *gserver,char *ClanZang_ChaName, char *ClanName)
{
	AUTH_QUE AuthQueTmp;
	ZeroMemory(&AuthQueTmp,sizeof(AUTH_QUE));

	AuthQueTmp.fdid = 100;
	AuthQueTmp.command = AUTH_QUE_CMD_BreakUP;          // 인증하다
	
	//ygy	
	strncpy( AuthQueTmp.ClanZang_ID, ClanZang_ID, sizeof( AuthQueTmp.ClanZang_ID ) );
	strncpy( AuthQueTmp.gserver, gserver, sizeof( AuthQueTmp.gserver ) );
	strncpy( AuthQueTmp.ClanZang_ChaName, ClanZang_ChaName, sizeof( AuthQueTmp.ClanZang_ChaName) );
	strncpy( AuthQueTmp.clanName, ClanName, sizeof( AuthQueTmp.clanName ) );

	long myTime;
	time( &myTime );
	AuthQueTmp.howmanyTime = myTime;
	
	// 큐를 등록
	if( AUTH_addQue( &AuthQueTmp ) < 0 ){
		// 실패하면 에러를 돌려 준다．기타의 에러．
		//nrproto_CharLogin_send( fd, AUTH_ERROR_OTHER, "authorization required\n" );

		//ktj : 프테때문에 일단 삭제
		//nrproto_ClientLogin_send( fd, 0, "make_Clan() 에러\n" );
		fd2("BreakUP_Clan() \277\241\267\257   : \305\245\277\241 \265\356\267\317\276\310\265\312");
		return 0;
	}
	return 1;
}



//특정 아디를 추방한다.
int WebDB_Thread::Banishiment(char *ClanZang_ID,char *gserver,char *ClanZang_ChaName,char *ClanName, char *ClanWon)
{
	AUTH_QUE AuthQueTmp;
	ZeroMemory(&AuthQueTmp,sizeof(AUTH_QUE));

	AuthQueTmp.fdid = 100;
	AuthQueTmp.command = AUTH_QUE_CMD_Banishiment;          // 인증하다	

	//ygy
	strncpy( AuthQueTmp.clanName, ClanName, sizeof( AuthQueTmp.clanName ) );
	strncpy( AuthQueTmp.ClanZang_ID, ClanZang_ID, sizeof( AuthQueTmp.ClanZang_ID ) );
	strncpy( AuthQueTmp.ClanZang_ChaName, ClanZang_ChaName, sizeof( AuthQueTmp.ClanZang_ChaName ) );
	strncpy( AuthQueTmp.gserver, gserver, sizeof( AuthQueTmp.gserver) );
	strncpy( AuthQueTmp.ClanWon_ChaName, ClanWon, sizeof( AuthQueTmp.ClanWon_ChaName) );
	//strncpy( AuthQueTmp.expl, id_str, strlen( id_str ) );			//임시로 .expl에다 넣음.

	long myTime;
	time( &myTime );
	AuthQueTmp.howmanyTime = myTime;
	
	// 큐를 등록
	if( AUTH_addQue( &AuthQueTmp ) < 0 ){
		// 실패하면 에러를 돌려 준다．기타의 에러．
		//nrproto_CharLogin_send( fd, AUTH_ERROR_OTHER, "authorization required\n" );

		//ktj : 프테때문에 일단 삭제
		//nrproto_ClientLogin_send( fd, 0, "make_Clan() 에러\n" );
		fd2("BreakUP_Clan() \277\241\267\257   : \305\245\277\241 \265\356\267\317\276\310\265\312");
		return 0;
	}
	return 1;
}



//초기화면 플래그

//탈퇴한다.
int WebDB_Thread::Secession(char* clanzang,char* gserver,char* SecessionID,char* claname)
{
	AUTH_QUE AuthQueTmp;
	ZeroMemory(&AuthQueTmp, sizeof( AUTH_QUE  ) );

	AuthQueTmp.fdid = 100;
	AuthQueTmp.command = AUTH_QUE_CMD_Secession;          // 인증하다

	strncpy( AuthQueTmp.clanName, claname, sizeof( AuthQueTmp.clanName ) );
	strncpy( AuthQueTmp.gserver, gserver, sizeof( AuthQueTmp.gserver ) );
	strncpy( AuthQueTmp.ClanWon_ChaName, SecessionID, sizeof( AuthQueTmp.ClanWon_ChaName ) );
	strncpy( AuthQueTmp.ClanWon_ID, clanzang, sizeof( AuthQueTmp.ClanZang_ID ) );

	//ygy
	//strncpy( AuthQueTmp.expl, id_str, strlen( id_str ) );			//임시로 .expl에다 넣음.

	long myTime;
	time( &myTime );
	AuthQueTmp.howmanyTime = myTime;
	
	// 큐를 등록
	if( AUTH_addQue( &AuthQueTmp ) < 0 ){
		// 실패하면 에러를 돌려 준다．기타의 에러．
		//nrproto_CharLogin_send( fd, AUTH_ERROR_OTHER, "authorization required\n" );

		//ktj : 프테때문에 일단 삭제
		//nrproto_ClientLogin_send( fd, 0, "make_Clan() 에러\n" );
		fd2("BreakUP_Clan() \277\241\267\257   : \305\245\277\241 \265\356\267\317\276\310\265\312");
		return 0;
	}
	return 1;
}




//창고권한 바뀜
int WebDB_Thread::Warhouse_right(char *clanName, char *right_str)
{
	AUTH_QUE AuthQueTmp;
	ZeroMemory(&AuthQueTmp,sizeof(AUTH_QUE));

	AuthQueTmp.fdid = 100;
	AuthQueTmp.command = AUTH_QUE_CMD_WarhouseR			;          // 인증하다
	

	strncpy( AuthQueTmp.clanName, clanName, sizeof( AuthQueTmp.clanName ) );
	strncpy( AuthQueTmp.expl, right_str, strlen( right_str ) );			//임시로 .expl에다 넣음.

	long myTime;
	time( &myTime );
	AuthQueTmp.howmanyTime = myTime;
	
	// 큐를 등록
	if( AUTH_addQue( &AuthQueTmp ) < 0 ){
		// 실패하면 에러를 돌려 준다．기타의 에러．
		//nrproto_CharLogin_send( fd, AUTH_ERROR_OTHER, "authorization required\n" );

		//ktj : 프테때문에 일단 삭제
		//nrproto_ClientLogin_send( fd, 0, "make_Clan() 에러\n" );
		fd2("Warhouse_right() \277\241\267\257   : \305\245\277\241 \265\356\267\317\276\310\265\312");
		return 0;
	}
	return 1;
}


//이임================================================================
int AUTH_proc_LeaveClan( int quenum, int threadindex)
{
	return WebDB.AUTH_proc_LeaveClan( quenum, threadindex);
}
int AUTH_proc_LeaveClan_Result( int quenum, int threadindex)
{
	return WebDB.AUTH_proc_LeaveClan_Result( quenum, threadindex);
}

int WebDB_Thread::AUTH_proc_LeaveClan( int quenum, int threadindex)
{
	
	char *clanname, *chaname,*gserver;
	int knret = 1, iBadMsg = 0;

	chaname = AUTH_que[quenum].ClanZang_ChaName;
	gserver = AUTH_que[quenum].gserver;
	clanname = AUTH_que[quenum].clanName;
	
	knret = iCheck.LeaveClan(clanname,chaname,gserver);

	// 결과를 격납
	AUTH_que[quenum].result = knret;
	// 결과를 격납
	AUTH_que[quenum].badmsg = iBadMsg;
	return 1;
}
int WebDB_Thread::AUTH_proc_LeaveClan_Result( int quenum, int threadindex)
{
	char szData[128];

	if( AUTH_que[quenum].result == 0 ){
		szData[0] = '\0';
	}else{
		sprintf( szData, "faild" );
	}

	int iResultCode;
	iResultCode = AUTH_que[quenum].result;	// 디폴트는 기타의 에러
	
	// 최종적으로 클라이언트에게 돌려 주는 코드
	switch( iResultCode ){
	case 1:				//완료
	case 2:				//클랜원이 아님
	case 3:				//존재하지 않는 클랜
	case 4:				//레벨 50미만
	case 5:				//권한 자격만 성공 ???????????????????????
	case 6:				//지정캐릭터의 포용최대인원수 보다 현재 인원수가 큼
	case 100:			//인자값 빠진에러리턴값
		break;
	case 1004:				//서버접근안된경우
		break;
	default :
		iResultCode = AUTH_ERROR_OTHER;	// 기타의 에러
		break;
	}


	//ktj : 일단 삭제
	// 결과를 조사하고 돌려 준다．
	//ktj : 돌려주는부분을 넣어야함.
	/*
	nrproto_ClientLogin_send( 
			fd, 
			iResultCode,
			szData);
	//nrproto_ClientLogin_send( fd, 1, "OK" );
	*/
	ANSdata.ret_val = iResultCode;				//tjclan.cpp에 정의된 변수에 답변을 넣는다.
	fd2("\303\242\260\355\261\307\307\321 \264\344\272\257\300\273 \271\336\300\275. :  ret_val %d  ", ANSdata.ret_val);
	

	// 범위내에 들어가고 이르다 OK.
	if( 0 <= AUTH_que[quenum].result && AUTH_que[quenum].result < AUTH_ERROR_END ){
	}else{
		AUTH_que[quenum].result = AUTH_ERROR_OTHER;
	}
	return TRUE;
	
}
int WebDB_Thread::LeaveClan(char *clanname,char *chaname,char *gserver)
{
	AUTH_QUE AuthQueTmp;
	ZeroMemory(&AuthQueTmp, sizeof(AuthQueTmp));

	AuthQueTmp.fdid = 100;
	AuthQueTmp.command = AUTH_QUE_CMD_LeaveClan;          // 클랜멤버로 가입

	//ygy	
	strncpy( AuthQueTmp.clanName, clanname, sizeof( AuthQueTmp.ClanZang_ID ) );
	strncpy( AuthQueTmp.gserver, gserver, sizeof( AuthQueTmp.gserver ) );
	strncpy( AuthQueTmp.ClanZang_ChaName, chaname, sizeof( AuthQueTmp.ClanZang_ChaName ) );	


	//큐에 등록
	if( AUTH_addQue( &AuthQueTmp ) < 0 ){
		// 실패하면 에러를 돌려 준다．기타의 에러．
		//nrproto_CharLogin_send( fd, AUTH_ERROR_OTHER, "authorization required\n" );

		//ktj : 프테때문에 일단 삭제
		//nrproto_ClientLogin_send( fd, 0, "make_Clan() 에러\n" );
		fd2("make_Clan() \277\241\267\257   : \305\245\277\241 \265\356\267\317\276\310\265\312");
		return 0;
	}
	return 1;	
}


//클랜결성완료를 위한 플래그셋팅
int WebDB_Thread::updateFlag(char *clanName)
{
	AUTH_QUE AuthQueTmp;
	ZeroMemory(&AuthQueTmp,sizeof(AUTH_QUE));

	AuthQueTmp.fdid = 100;
	AuthQueTmp.command = AUTH_QUE_CMD_updateFlag;		//클랜결성완료를 위한 플래그셋팅	
	
	strncpy( AuthQueTmp.clanName, clanName, sizeof( AuthQueTmp.clanName ) );

	long myTime;
	time( &myTime );
	AuthQueTmp.howmanyTime = myTime;
	
	// 큐를 등록
	if( AUTH_addQue( &AuthQueTmp ) < 0 ){
		// 실패하면 에러를 돌려 준다．기타의 에러．
		//nrproto_CharLogin_send( fd, AUTH_ERROR_OTHER, "authorization required\n" );

		//ktj : 프테때문에 일단 삭제
		//nrproto_ClientLogin_send( fd, 0, "make_Clan() 에러\n" );
		fd2("Warhouse_right() \277\241\267\257   : \305\245\277\241 \265\356\267\317\276\310\265\312");
		return 0;
	}
	return 1;
}



//클랜마크읽기
int WebDB_Thread::MarkREAD(char *clanname,char *mark)
{
	AUTH_QUE AuthQueTmp;
	ZeroMemory(&AuthQueTmp,sizeof(AUTH_QUE));

	AuthQueTmp.fdid = 100;
	AuthQueTmp.command = AUTH_QUE_CMD_MarkREAD;		//클랜결성완료를 위한 플래그셋팅	
	
	strncpy( AuthQueTmp.Mark, mark, sizeof( AuthQueTmp.Mark ) );
	strncpy(AuthQueTmp.clanName,clanname,sizeof(AuthQueTmp.clanName));

	long myTime;
	time( &myTime );
	AuthQueTmp.howmanyTime = myTime;
	
	// 큐를 등록
	if( AUTH_addQue( &AuthQueTmp ) < 0 ){
		// 실패하면 에러를 돌려 준다．기타의 에러．
		//nrproto_CharLogin_send( fd, AUTH_ERROR_OTHER, "authorization required\n" );

		//ktj : 프테때문에 일단 삭제
		//nrproto_ClientLogin_send( fd, 0, "make_Clan() 에러\n" );
		fd2("Warhouse_right() \277\241\267\257   : \305\245\277\241 \265\356\267\317\276\310\265\312");
		return 0;
	}
	return 1;
}

//다른 클랜마크읽기
int WebDB_Thread::MarkREAD2(char *clanname,char *mark)
{
	AUTH_QUE AuthQueTmp;
	ZeroMemory(&AuthQueTmp,sizeof(AUTH_QUE));

	AuthQueTmp.fdid = 100;
	AuthQueTmp.command = AUTH_QUE_CMD_MarkREAD2;		//클랜결성완료를 위한 플래그셋팅	
	
	strncpy( AuthQueTmp.Mark, mark, sizeof( AuthQueTmp.Mark ) );
	strncpy(AuthQueTmp.clanName,clanname,sizeof(AuthQueTmp.clanName));

	long myTime;
	time( &myTime );
	AuthQueTmp.howmanyTime = myTime;
	
	// 큐를 등록
	if( AUTH_addQue( &AuthQueTmp ) < 0 ){
		// 실패하면 에러를 돌려 준다．기타의 에러．
		//nrproto_CharLogin_send( fd, AUTH_ERROR_OTHER, "authorization required\n" );

		//ktj : 프테때문에 일단 삭제
		//nrproto_ClientLogin_send( fd, 0, "make_Clan() 에러\n" );
		fd2("Warhouse_right() \277\241\267\257   : \305\245\277\241 \265\356\267\317\276\310\265\312");
		return 0;
	}
	return 1;
}




//ktj : 셋팅된 호스트이름이 NULL이면 클랜메뉴를 '점검중'으로 뛰어준다.
extern BOOL bCheckHostNameandPort ;			//tjclan.cpp
void WebDB_Thread::CheckHostNamePort(char *hostname, int port)
{
	if(hostname == NULL) {
		bCheckHostNameandPort = FALSE;
		return;
	}
	if(port == 0){
		bCheckHostNameandPort = FALSE;
		return;
	}

	bCheckHostNameandPort = TRUE;
}
















int WebDB_Thread::start_GOLDmoney(int fd, char *cdkey, char *accountid, char *passwd, char *ip_address)
{
	return 0;
		//ktj : 끝내기 과금처리용을 새로 만들어 넣음.
		AUTH_QUE AuthQueTmp;
		ZeroMemory(&AuthQueTmp,sizeof(AUTH_QUE));

		/*
		//ktj : 원판임.
		if(Connect[fd].howmanyTime==0) return 0;		//제대로 로긴이 안되서 끝내기 과금처리 안함.

		// 큐에 정보를 넣는다．
		AuthQueTmp.fdid = Connect[fd].fdid;
		//AuthQueTmp.command = AUTH_QUE_COMMAND_AUTH;
		//ktj : 새로넣음.
		AuthQueTmp.command = AUTH_QUE_COMMAND_AUTH_KTJEND;
		
		strncpy( AuthQueTmp.cdkey, Connect[fd].cdkey, sizeof( AuthQueTmp.cdkey ) );
		strncpy( AuthQueTmp.isaokey, Connect[fd].accountid, sizeof( AuthQueTmp.isaokey ) );
		strncpy( AuthQueTmp.password, Connect[fd].passwd, sizeof( AuthQueTmp.password ) );
		strncpy( AuthQueTmp.ip_address, Connect[fd].ip_address, sizeof( AuthQueTmp.ip_address ) );
		AuthQueTmp.howmanyTime = Connect[fd].howmanyTime;
		*/

		AuthQueTmp.fdid = 100;
		//AuthQueTmp.command = AUTH_QUE_COMMAND_AUTH;
		//ktj : 새로넣음.
		//AuthQueTmp.command = AUTH_QUE_COMMAND_AUTH_KTJEND;
		AuthQueTmp.command = AUTH_QUE_CMD_CL_MAKE;          // 인증하다
		
		strncpy( AuthQueTmp.cdkey, cdkey, sizeof( AuthQueTmp.cdkey ) );
		strncpy( AuthQueTmp.isaokey, accountid, sizeof( AuthQueTmp.isaokey ) );
		strncpy( AuthQueTmp.password, passwd, sizeof( AuthQueTmp.password ) );
		strncpy( AuthQueTmp.ip_address, ip_address, sizeof( AuthQueTmp.ip_address ) );

		long myTime;
		time( &myTime );
		AuthQueTmp.howmanyTime = myTime;//Connect[fd].howmanyTime = myTime;
	
		// 큐를 등록
	if( AUTH_addQue( &AuthQueTmp ) < 0 ){
		// 실패하면 에러를 돌려 준다．기타의 에러．
		//nrproto_CharLogin_send( fd, AUTH_ERROR_OTHER, "authorization required\n" );

		//ktj : 프테때문에 일단 삭제
		//nrproto_ClientLogin_send( fd, 0, "start_GOLDmoney에러\n" );
		fd2("\263\241\263\273\261\342 \260\372\261\335 \277\241\267\257 \305\245\277\241 \265\356\267\317\276\310\265\312. %s \n", accountid );
		return 0;
	}
	return 1;
}

//ktj 새로 만듦.: 끝 과금처리 
int WebDB_Thread::end_GOLDmoney(int fd, char *cdkey, char *accountid, char *passwd, char *ip_address)
{
		//ktj : 끝내기 과금처리용을 새로 만들어 넣음.
		AUTH_QUE AuthQueTmp;


		/*
		//ktj : 원판임.
		if(Connect[fd].howmanyTime==0) return 0;		//제대로 로긴이 안되서 끝내기 과금처리 안함.

		// 큐에 정보를 넣는다．
		AuthQueTmp.fdid = Connect[fd].fdid;
		//AuthQueTmp.command = AUTH_QUE_COMMAND_AUTH;
		//ktj : 새로넣음.
		AuthQueTmp.command = AUTH_QUE_COMMAND_AUTH_KTJEND;
		
		strncpy( AuthQueTmp.cdkey, Connect[fd].cdkey, sizeof( AuthQueTmp.cdkey ) );
		strncpy( AuthQueTmp.isaokey, Connect[fd].accountid, sizeof( AuthQueTmp.isaokey ) );
		strncpy( AuthQueTmp.password, Connect[fd].passwd, sizeof( AuthQueTmp.password ) );
		strncpy( AuthQueTmp.ip_address, Connect[fd].ip_address, sizeof( AuthQueTmp.ip_address ) );
		AuthQueTmp.howmanyTime = Connect[fd].howmanyTime;
		*/

		AuthQueTmp.fdid = 100;
		//AuthQueTmp.command = AUTH_QUE_COMMAND_AUTH;
		//ktj : 새로넣음.
		AuthQueTmp.command = AUTH_QUE_CMD_CL_MEMBER_MAKE;
		
		strncpy( AuthQueTmp.cdkey, cdkey, sizeof( AuthQueTmp.cdkey ) );
		strncpy( AuthQueTmp.isaokey, accountid, sizeof( AuthQueTmp.isaokey ) );
		strncpy( AuthQueTmp.password, passwd, sizeof( AuthQueTmp.password ) );
		strncpy( AuthQueTmp.ip_address, ip_address, sizeof( AuthQueTmp.ip_address ) );
		AuthQueTmp.howmanyTime = 100;//Connect[fd].howmanyTime;
	
		// 큐를 등록
	if( AUTH_addQue( &AuthQueTmp ) < 0 ){
		// 실패하면 에러를 돌려 준다．기타의 에러．
		//nrproto_CharLogin_send( fd, AUTH_ERROR_OTHER, "authorization required\n" );


		//ktj : 프테때문에 일단 삭제함.
		//nrproto_ClientLogin_send( fd, 0, "end_GOLDmoney 에러\n" );

		fd2("\263\241\263\273\261\342 \260\372\261\335 \277\241\267\257 \305\245\277\241 \265\356\267\317\276\310\265\312. %s \n", accountid );
		return 0;
	}
	return 1;
}
/*/////////////////////////////////////////////////////////////////////////// END
: 하 는 일 :  WebDB에 접속해서 할일을 지시하는 지시함수들.
///////////////////////////////////////////////////////////////////////////*/

char web_DBip0[64] = {0,};
int  web_DBport0 = 0;
char web_DBip1[64] = {0,};
int  web_DBport1 = 0;
int  nClanTicket = 0; //ticket번호



void web_DB_ip_port_init(char *ip0, int port0, char *ip1, int port1)
{
	strcpy(web_DBip0, ip0);
	strcpy(web_DBip1, ip1);

	web_DBport0 = port0;
	web_DBport1 = port1;

}

void web_DB_ip_port_init(char *ip0, int port0, char *ip1, int port1,int ticket)
{
	strcpy(web_DBip0, ip0);
	strcpy(web_DBip1, ip1);

	web_DBport0 = port0;
	web_DBport1 = port1;
#ifdef __CLANSERVER__
	nClanTicket = 9999;
#else
	nClanTicket = ticket;
#endif
}

//winmain.cpp에서 셋팅됨.
void web_DB_start(char *hostN0, int port0, char *hostN1, int port1);
void web_DB_start()
{
	//dns
	//char hostN0[] ="clanserver.pristontale.com" ; //"211.39.144.157"
	//char hostN1[] = "image.pristontale.com";//"211.39.144.158";

	//ip
	char hostN0[] ="211.39.144.157";
	char hostN1[] ="211.39.144.158";



	//char hostN0[] = "211.39.144.155";
	//char hostN1[] = "211.39.144.154";

	int port0 = 80;
	int port1 = 80;

	//ktj : 원본 : 무조건 web_DBip0를 사용토록 바꿧음(설정을 안해도 되도록)
	//if(web_DBip0[0]==0) {		//웹디비설정안되있으면 그냥 로칼 변수사용함.
	//	 web_DB_start(hostN0, port0, hostN1, port1 );
	//}
	//else {
		web_DB_start(web_DBip0, web_DBport0 , web_DBip1, web_DBport1 );
	//}
	
	//ktj : 임시
	//web_DB_start(hostN0, port0, hostN1, port1 );

	//web_DB_start(NULL, port0, NULL, port1  );
}


BOOL bip_port_error = FALSE;
//KTJ : 임시 함수들
////ktj : winmain.cpp의 int SetGameMode( int mode ) 에 셋팅되어져 있는것임
void web_DB_start(char *hostN0, int port0, char *hostN1, int port1)
{		
	if((hostN0 == NULL)||(port0 == 0) || (hostN1 == NULL)|| (port1 == 0)) {
		bip_port_error = TRUE;


		//메시지 경고문때문에 웹디비는 설정되어야함.
		WebDB.AUTH_init(10);
		WebDB.ThreadS.flag = FLAG_WAIT;
		WebDB.Suspend();     //일단 중지

		while(1) {
			if(WebDB.ThreadS.flag == FLAG_WAIT)
				break;
		}

		return;
	}
	static int cnt=0;
	if(cnt==1) return;
	cnt=1;
	/*
	char hostN[] =  "211.39.144.157";
	char hostN1[] = "211.39.144.158";
	int port = 80;
	int port1 = 80;
	*/



	//===================================================================================
	//주의 : 셋팅 해야함.================================================================
	//===================================================================================

	//iCheck.init(1, 30);		//웹db사용시마다 셋팅
	iCheck.init(1, 5);		//웹db사용시마다 셋팅

	//클랜서버 호스트 아이피가 0, 그리고 포트가 0 일 경우 확인하는 함수
	WebDB.CheckHostNamePort(hostN0,port0); 

	iCheck.init_HostName_Port (hostN0, port0);
	iCheck.init_HostName_Port1(hostN1 ,port1);		//셋팅할것.

	//===================================================================================
	//주의 : 셋팅 해야함.================================================================
	//===================================================================================

	WebDB.AUTH_init(10);
	WebDB.ThreadS.flag = FLAG_WAIT;
	WebDB.Suspend();     //일단 중지

	while(1) {
		if(WebDB.ThreadS.flag == FLAG_WAIT)
			break;
	}
}


int web_DB_state_FLAG_WAIT()
{
	if(WebDB.ThreadS.flag == FLAG_WAIT)
		return 1;

	return 0;
}


void web_DB_main()
{
	WebDB.main();
}

void web_DB_end()
{
	WebDB.end();
}



//이임관련 플래그================================================================




int AUTH_proc_isKPFlag( int quenum, int threadindex)
{
	return WebDB.AUTH_proc_isKPFlag( quenum, threadindex);
}

int WebDB_Thread::AUTH_proc_isKPFlag( int quenum, int threadindex)
{
	char *ClanZang_ID,*ClanZang_ChaName,*gserver,*PFlag,*Gubun,*ClanName;
	int knret = 1, iBadMsg = 0;

	//id = AUTH_que[quenum].id;
	ClanZang_ID = AUTH_que[quenum].ClanZang_ID;
	ClanZang_ChaName = AUTH_que[quenum].ClanZang_ChaName;
	gserver = AUTH_que[quenum].gserver;
	PFlag = AUTH_que[quenum].PFlag;
	Gubun = AUTH_que[quenum].Gubun;
	ClanName = AUTH_que[quenum].clanName;
	
	//id에 해당하는 자가 클랜멤버라면, 100(클랜멤버), 101(클랜칲:클랜명을가져옴), 아니면 102(클랜에 들어있지않음)
	knret = iCheck.isKPFlag(ClanZang_ID,ClanName,ClanZang_ChaName,gserver, PFlag,Gubun);


	// 결과를 격납
	AUTH_que[quenum].result = knret;
	// 결과를 격납
	AUTH_que[quenum].badmsg = iBadMsg;
	return TRUE;
}

int AUTH_proc_isKPFlag_Result( int quenum, int threadindex)
{
	return WebDB.AUTH_proc_isKPFlag_Result( quenum, threadindex);
}

int WebDB_Thread::AUTH_proc_isKPFlag_Result( int quenum, int threadindex)
{
	char *aszAuthErrStr[] = {
	"OK",
	"Err CDKEY", 
	"Err ISAO_ID", 
	"Err PASSWORD", 
	"Err EXPIRE", 
	"Err LOCOUT", 
	"Err ISAO_DB", 
	"Err OTHER", 
	""
	};

	char szData[128];
	
	if( AUTH_que[quenum].result == 0 ){
		szData[0] = '\0';
	}else{
		sprintf( szData, "faild" );
	}

	char szBuffer[256];

	// 에러 메시지 테이블의 범위내라면 문자로 표시
	if( 0 <= AUTH_que[quenum].result && AUTH_que[quenum].result < AUTH_ERROR_END ){
		sprintf( szBuffer, "%s", aszAuthErrStr[AUTH_que[quenum].result] );
	}else{
		sprintf( szBuffer, "ExtraCode=%d", AUTH_que[quenum].result );
	}

	int iResultCode;
	iResultCode = AUTH_que[quenum].result;	// 디폴트는 기타의 에러

	
	// 최종적으로 클라이언트에게 돌려 주는 코드
	switch( iResultCode ){
	case 0:
		ANSdata.ret_val = iResultCode;				//tjclan.cpp에 정의된 변수에 답변을 넣는다.
		break;
	case 1:
		ANSdata.ret_val = iResultCode;				//tjclan.cpp에 정의된 변수에 답변을 넣는다.		
		break;	
	case 100:				//일반 멤버,
		ANSdata.ret_val = iResultCode;
		break;
	case 1004:
		ANSdata.ret_val = iResultCode;
		break;		
	default :
		ANSdata.ret_val = AUTH_ERROR_OTHER;	// 기타의 에러
		break;
	}


	//ktj : 일단 삭제
	// 결과를 조사하고 돌려 준다．
	//ktj : 돌려주는부분을 넣어야함.
	/*
	nrproto_ClientLogin_send( 
			fd, 
			iResultCode,
			szData);
	//nrproto_ClientLogin_send( fd, 1, "OK" );
	*/
	//ANSdata.ret_val = iResultCode;				//tjclan.cpp에 정의된 변수에 답변을 넣는다.
	fd2("\305\254\267\243\270\342\271\366\277\251\272\316\300\307 \264\344\272\257\300\273 \271\336\300\275. :  ret_val %d  ", ANSdata.ret_val);

	

	// 범위내에 들어가고 이르다 OK.
	if( 0 <= AUTH_que[quenum].result && AUTH_que[quenum].result < AUTH_ERROR_END ){
	}else{
		AUTH_que[quenum].result = AUTH_ERROR_OTHER;
	}
	return TRUE;
}

int WebDB_Thread::isKPFlag(char *userid, char *clName, char *chName,char *gserver, char *PFlag, char *Gubun)
{
	AUTH_QUE AuthQueTmp;
	ZeroMemory(&AuthQueTmp,sizeof(AUTH_QUE));

	AuthQueTmp.fdid = 100;
	AuthQueTmp.command = AUTH_QUE_CMD_isKPFlag;          // 인증하다
	
	strncpy( AuthQueTmp.Gubun, Gubun, sizeof( AuthQueTmp.Gubun ) );
	strncpy(AuthQueTmp.ClanZang_ID, userid, sizeof( AuthQueTmp.ClanZang_ID ));
	strncpy( AuthQueTmp.clanName, clName, sizeof( AuthQueTmp.clanName ) );	
	strncpy( AuthQueTmp.ClanZang_ChaName, chName, sizeof( AuthQueTmp.ClanZang_ChaName ) );	
	strncpy( AuthQueTmp.gserver, gserver, sizeof( AuthQueTmp.gserver ) );
	strncpy( AuthQueTmp.PFlag, PFlag, sizeof( AuthQueTmp.PFlag ) );
	

	

	long myTime;
	time( &myTime );
	AuthQueTmp.howmanyTime = myTime;
	
	// 큐를 등록
	if( AUTH_addQue( &AuthQueTmp ) < 0 ){
		// 실패하면 에러를 돌려 준다．기타의 에러．
		//nrproto_CharLogin_send( fd, AUTH_ERROR_OTHER, "authorization required\n" );

		//ktj : 프테때문에 일단 삭제
		//nrproto_ClientLogin_send( fd, 0, "make_Clan() 에러\n" );
		fd2("make_Clan() \277\241\267\257   : \305\245\277\241 \265\356\267\317\276\310\265\312");
		return 0;
	}
	return 1;
}

int WebDB_Thread::AppointSubChip(char* userid,char* chaname,char* clanname,char *gserver,char* clanwon)//부클랜장 임명
{
	AUTH_QUE AuthQueTmp;
	ZeroMemory(&AuthQueTmp,sizeof(AUTH_QUE));

	AuthQueTmp.fdid = 100;
	AuthQueTmp.command = AUTH_QUE_CMD_AppointSubChip;          // 인증하다
		
	strncpy(AuthQueTmp.ClanZang_ID, userid, sizeof( AuthQueTmp.ClanZang_ID ));
	strncpy( AuthQueTmp.clanName, clanname, sizeof( AuthQueTmp.clanName ) );	
	strncpy( AuthQueTmp.ClanZang_ChaName, chaname, sizeof( AuthQueTmp.ClanZang_ChaName ) );	
	strncpy( AuthQueTmp.gserver, gserver, sizeof( AuthQueTmp.gserver ) );
	strncpy(AuthQueTmp.ClanWon_ChaName,clanwon,sizeof(AuthQueTmp.ClanWon_ChaName));;
	
	

	long myTime;
	time( &myTime );
	AuthQueTmp.howmanyTime = myTime;
	
	// 큐를 등록
	if( AUTH_addQue( &AuthQueTmp ) < 0 ){
		// 실패하면 에러를 돌려 준다．기타의 에러．
		//nrproto_CharLogin_send( fd, AUTH_ERROR_OTHER, "authorization required\n" );

		//ktj : 프테때문에 일단 삭제
		//nrproto_ClientLogin_send( fd, 0, "make_Clan() 에러\n" );
		fd2("make_Clan() \277\241\267\257   : \305\245\277\241 \265\356\267\317\276\310\265\312");
		return 0;
	}
	return 1;
}



int AUTH_proc_AppointSubChip( int quenum, int threadindex)
{
	return WebDB.AUTH_proc_AppointSubChip( quenum, threadindex);
}

int WebDB_Thread::AUTH_proc_AppointSubChip( int quenum, int threadindex)
{
	char *ClanZang_ID,*ClanZang_ChaName,*gserver,*ClanName;
	int knret = 1, iBadMsg = 0;
	//char *gserver;
	char *clanwon;

	//id = AUTH_que[quenum].id;
	ClanZang_ID = AUTH_que[quenum].ClanZang_ID;
	ClanZang_ChaName = AUTH_que[quenum].ClanZang_ChaName;
	gserver = AUTH_que[quenum].gserver;	
	clanwon = AUTH_que[quenum].ClanWon_ChaName;
	ClanName = AUTH_que[quenum].clanName;
	
	
	//id에 해당하는 자가 클랜멤버라면, 100(클랜멤버), 101(클랜칲:클랜명을가져옴), 아니면 102(클랜에 들어있지않음)
	knret = iCheck.AppointSubChip(gserver,clanwon);

	//부클랜칩 임명이 성공되면 플래그값 세팅(플래그값 : 300)
	//if(knret == 1)
	//{
	//	knret = iCheck.isKPFlag(ClanZang_ID,ClanZang_ChaName,ClanName,gserver,"300","0");
	//}

	

	// 결과를 격납
	AUTH_que[quenum].result = knret;
	// 결과를 격납
	AUTH_que[quenum].badmsg = iBadMsg;
	return TRUE;
}

int AUTH_proc_AppointSubChip_Result( int quenum, int threadindex)
{
	return WebDB.AUTH_proc_AppointSubChip_Result( quenum, threadindex);
}

int WebDB_Thread::AUTH_proc_AppointSubChip_Result( int quenum, int threadindex)
{
	char *aszAuthErrStr[] = {
	"OK",
	"Err CDKEY", 
	"Err ISAO_ID", 
	"Err PASSWORD", 
	"Err EXPIRE", 
	"Err LOCOUT", 
	"Err ISAO_DB", 
	"Err OTHER", 
	""
	};

	char szData[128];
	
	if( AUTH_que[quenum].result == 0 ){
		szData[0] = '\0';
	}else{
		sprintf( szData, "faild" );
	}

	char szBuffer[256];

	// 에러 메시지 테이블의 범위내라면 문자로 표시
	if( 0 <= AUTH_que[quenum].result && AUTH_que[quenum].result < AUTH_ERROR_END ){
		sprintf( szBuffer, "%s", aszAuthErrStr[AUTH_que[quenum].result] );
	}else{
		sprintf( szBuffer, "ExtraCode=%d", AUTH_que[quenum].result );
	}

	int iResultCode;
	iResultCode = AUTH_que[quenum].result;	// 디폴트는 기타의 에러

	
	// 최종적으로 클라이언트에게 돌려 주는 코드
	switch( iResultCode ){
	case 0://클랜원이 아님
		ANSdata.ret_val = iResultCode;				//tjclan.cpp에 정의된 변수에 답변을 넣는다.
		break;
	case 3://부클랜칩이 존재함
		ANSdata.ret_val = iResultCode;				//tjclan.cpp에 정의된 변수에 답변을 넣는다.		
		break;
	case 2: //클랜칩이거나 
		ANSdata.ret_val = iResultCode;				//tjclan.cpp에 정의된 변수에 답변을 넣는다.		
		break;	
	case 1: //변경성공
		ANSdata.ret_val = iResultCode;				//tjclan.cpp에 정의된 변수에 답변을 넣는다.		
		break;

	case 100:				//일반 멤버,
		ANSdata.ret_val = iResultCode;
		break;
	case 1004:
		ANSdata.ret_val = iResultCode;
		break;		
	default :
		ANSdata.ret_val = AUTH_ERROR_OTHER;	// 기타의 에러
		break;
	}


	//ktj : 일단 삭제
	// 결과를 조사하고 돌려 준다．
	//ktj : 돌려주는부분을 넣어야함.
	/*
	nrproto_ClientLogin_send( 
			fd, 
			iResultCode,
			szData);
	//nrproto_ClientLogin_send( fd, 1, "OK" );
	*/
	//ANSdata.ret_val = iResultCode;				//tjclan.cpp에 정의된 변수에 답변을 넣는다.
	fd2("\305\254\267\243\270\342\271\366\277\251\272\316\300\307 \264\344\272\257\300\273 \271\336\300\275. :  ret_val %d  ", ANSdata.ret_val);

	

	// 범위내에 들어가고 이르다 OK.
	if( 0 <= AUTH_que[quenum].result && AUTH_que[quenum].result < AUTH_ERROR_END ){
	}else{
		AUTH_que[quenum].result = AUTH_ERROR_OTHER;
	}
	return TRUE;
}

	
int WebDB_Thread::ReleaseSubChip(char* userid,char* chaname,char* clanname,char *gserver,char* clanwon) //부 클랜장 해임
{
	AUTH_QUE AuthQueTmp;
	ZeroMemory(&AuthQueTmp,sizeof(AUTH_QUE));

	AuthQueTmp.fdid = 100;
	AuthQueTmp.command = AUTH_QUE_CMD_ReleaseSubChip;          // 인증하다
		
	strncpy(AuthQueTmp.ClanZang_ID, userid, sizeof( AuthQueTmp.ClanZang_ID ));
	strncpy( AuthQueTmp.clanName, clanname, sizeof( AuthQueTmp.clanName ) );	
	strncpy( AuthQueTmp.ClanZang_ChaName, chaname, sizeof( AuthQueTmp.ClanZang_ChaName ) );	
	strncpy( AuthQueTmp.gserver, gserver, sizeof( AuthQueTmp.gserver ) );
	strncpy(AuthQueTmp.ClanWon_ChaName,clanwon,sizeof(AuthQueTmp.ClanWon_ChaName));;
	
	

	long myTime;
	time( &myTime );
	AuthQueTmp.howmanyTime = myTime;
	
	// 큐를 등록
	if( AUTH_addQue( &AuthQueTmp ) < 0 ){
		// 실패하면 에러를 돌려 준다．기타의 에러．
		//nrproto_CharLogin_send( fd, AUTH_ERROR_OTHER, "authorization required\n" );

		//ktj : 프테때문에 일단 삭제
		//nrproto_ClientLogin_send( fd, 0, "make_Clan() 에러\n" );
		fd2("make_Clan() \277\241\267\257   : \305\245\277\241 \265\356\267\317\276\310\265\312");
		return 0;
	}
	return 1;
}

//부클랜장 해임
int AUTH_proc_ReleaseSubChip( int quenum, int threadindex)
{
	return WebDB.AUTH_proc_ReleaseSubChip(quenum, threadindex);
}

int WebDB_Thread::AUTH_proc_ReleaseSubChip( int quenum, int threadindex)
{
	int knret = 1, iBadMsg = 0;
	char *gserver;
	char *clanwon;
	char *ClanZang_ID,*ClanZang_ChaName,*ClanName;

	//id = AUTH_que[quenum].id;
	ClanZang_ID = AUTH_que[quenum].ClanZang_ID;
	ClanZang_ChaName = AUTH_que[quenum].ClanZang_ChaName;
	gserver = AUTH_que[quenum].gserver;	
	clanwon = AUTH_que[quenum].ClanWon_ChaName;
	ClanName = AUTH_que[quenum].clanName;
	
	
	//id에 해당하는 자가 클랜멤버라면, 100(클랜멤버), 101(클랜칲:클랜명을가져옴), 아니면 102(클랜에 들어있지않음)
	knret = iCheck.ReleaseSubChip(gserver,clanwon);
	//if(knret == 1) //부클랜칩 해임관련 플래그 세팅(301)
	//{
	//	knret = iCheck.isKPFlag(ClanZang_ID,ClanZang_ChaName,ClanName,gserver,"301","0");
	//}

	// 결과를 격납
	AUTH_que[quenum].result = knret;
	// 결과를 격납
	AUTH_que[quenum].badmsg = iBadMsg;
	return TRUE;
}


int AUTH_proc_ReleaseSubChip_Result( int quenum, int threadindex)
{
	return WebDB.AUTH_proc_ReleaseSubChip_Result(quenum, threadindex);
}

int WebDB_Thread::AUTH_proc_ReleaseSubChip_Result( int quenum, int threadindex)
{
	char *aszAuthErrStr[] = {
	"OK",
	"Err CDKEY", 
	"Err ISAO_ID", 
	"Err PASSWORD", 
	"Err EXPIRE", 
	"Err LOCOUT", 
	"Err ISAO_DB", 
	"Err OTHER", 
	""
	};

	char szData[128];
	
	if( AUTH_que[quenum].result == 0 ){
		szData[0] = '\0';
	}else{
		sprintf( szData, "faild" );
	}

	char szBuffer[256];

	// 에러 메시지 테이블의 범위내라면 문자로 표시
	if( 0 <= AUTH_que[quenum].result && AUTH_que[quenum].result < AUTH_ERROR_END ){
		sprintf( szBuffer, "%s", aszAuthErrStr[AUTH_que[quenum].result] );
	}else{
		sprintf( szBuffer, "ExtraCode=%d", AUTH_que[quenum].result );
	}

	int iResultCode;
	iResultCode = AUTH_que[quenum].result;	// 디폴트는 기타의 에러

	
	// 최종적으로 클라이언트에게 돌려 주는 코드
	switch( iResultCode ){
	case 0: //클랜원이 아닐경우
		ANSdata.ret_val = iResultCode;				//tjclan.cpp에 정의된 변수에 답변을 넣는다.
		break;
	case 1://해임 성공
		ANSdata.ret_val = iResultCode;				//tjclan.cpp에 정의된 변수에 답변을 넣는다.
		break;
	case 2: //클랜장이거나 부클랜일 경우
		ANSdata.ret_val = iResultCode;				//tjclan.cpp에 정의된 변수에 답변을 넣는다.		
		break;	
	case 100:				//일반 멤버,
		ANSdata.ret_val = iResultCode;
		break;
	case 1004:
		ANSdata.ret_val = iResultCode;
		break;		
	default :
		ANSdata.ret_val = AUTH_ERROR_OTHER;	// 기타의 에러
		break;
	}


	//ktj : 일단 삭제
	// 결과를 조사하고 돌려 준다．
	//ktj : 돌려주는부분을 넣어야함.
	/*
	nrproto_ClientLogin_send( 
			fd, 
			iResultCode,
			szData);
	//nrproto_ClientLogin_send( fd, 1, "OK" );
	*/
	//ANSdata.ret_val = iResultCode;				//tjclan.cpp에 정의된 변수에 답변을 넣는다.
	fd2("\305\254\267\243\270\342\271\366\277\251\272\316\300\307 \264\344\272\257\300\273 \271\336\300\275. :  ret_val %d  ", ANSdata.ret_val);

	

	// 범위내에 들어가고 이르다 OK.
	if( 0 <= AUTH_que[quenum].result && AUTH_que[quenum].result < AUTH_ERROR_END ){
	}else{
		AUTH_que[quenum].result = AUTH_ERROR_OTHER;
	}
	return TRUE;
}

/*////////////////////////////////////////////////////////////////////////////////
WAV 파일 가져오기
////////////////////////////////////////////////////////////////////////////////*/
int	WebDB_Thread::GetWavFile(char* FileName)
{
	AUTH_QUE AuthQueTmp;
	ZeroMemory(&AuthQueTmp,sizeof(AUTH_QUE));

	AuthQueTmp.fdid = 100;
	AuthQueTmp.command = AUTH_QUE_CMD_GetWavFile;          // 인증하다
		
	strncpy(AuthQueTmp.szWavFileName, FileName, sizeof( AuthQueTmp.szWavFileName ));
	
	long myTime;
	time( &myTime );
	AuthQueTmp.howmanyTime = myTime;
	
	// 큐를 등록
	if( AUTH_addQue( &AuthQueTmp ) < 0 ){
		// 실패하면 에러를 돌려 준다．기타의 에러．
		//nrproto_CharLogin_send( fd, AUTH_ERROR_OTHER, "authorization required\n" );

		//ktj : 프테때문에 일단 삭제
		//nrproto_ClientLogin_send( fd, 0, "make_Clan() 에러\n" );
		fd2("make_Clan() \277\241\267\257   : \305\245\277\241 \265\356\267\317\276\310\265\312");
		return 0;
	}
	return 1;
}
int AUTH_proc_GetWavFile( int quenum, int threadindex)
{
	return  WebDB.AUTH_proc_GetWavFile(quenum,threadindex);
}

int WebDB_Thread::AUTH_proc_GetWavFile( int quenum, int threadindex)
{
	int knret = 1, iBadMsg = 0;
	char *szFileName;	

	szFileName = AUTH_que[quenum].szWavFileName;
	
	
	//id에 해당하는 자가 클랜멤버라면, 100(클랜멤버), 101(클랜칲:클랜명을가져옴), 아니면 102(클랜에 들어있지않음)
	knret = iCheck.GetWavFile(szFileName);
	//if(knret == 1) //부클랜칩 해임관련 플래그 세팅(301)
	//{
	//	knret = iCheck.isKPFlag(ClanZang_ID,ClanZang_ChaName,ClanName,gserver,"301","0");
	//}

	// 결과를 격납
	AUTH_que[quenum].result = knret;
	// 결과를 격납
	AUTH_que[quenum].badmsg = iBadMsg;
	return TRUE;
}
int AUTH_proc_GetWavFile_Result( int quenum, int threadindex)
{
	return WebDB.AUTH_proc_GetWavFile_Result(quenum,threadindex);
}

int WebDB_Thread::AUTH_proc_GetWavFile_Result( int quenum, int threadindex)
{
	char *aszAuthErrStr[] = {
	"OK",
	"Err CDKEY", 
	"Err ISAO_ID", 
	"Err PASSWORD", 
	"Err EXPIRE", 
	"Err LOCOUT", 
	"Err ISAO_DB", 
	"Err OTHER", 
	""
	};

	char szData[128];
	
	if( AUTH_que[quenum].result == 0 ){
		szData[0] = '\0';
	}else{
		sprintf( szData, "faild" );
	}

	char szBuffer[256];

	// 에러 메시지 테이블의 범위내라면 문자로 표시
	if( 0 <= AUTH_que[quenum].result && AUTH_que[quenum].result < AUTH_ERROR_END ){
		sprintf( szBuffer, "%s", aszAuthErrStr[AUTH_que[quenum].result] );
	}else{
		sprintf( szBuffer, "ExtraCode=%d", AUTH_que[quenum].result );
	}

	int iResultCode;
	iResultCode = AUTH_que[quenum].result;	// 디폴트는 기타의 에러

	
	// 최종적으로 클라이언트에게 돌려 주는 코드
	switch( iResultCode ){
	case 0: //클랜원이 아닐경우
		ANSdata.ret_val = iResultCode;				//tjclan.cpp에 정의된 변수에 답변을 넣는다.
		break;
	case 1://해임 성공
		ANSdata.ret_val = iResultCode;				//tjclan.cpp에 정의된 변수에 답변을 넣는다.
		break;
	case 2: //클랜장이거나 부클랜일 경우
		ANSdata.ret_val = iResultCode;				//tjclan.cpp에 정의된 변수에 답변을 넣는다.		
		break;	
	case 100:				//일반 멤버,
		ANSdata.ret_val = iResultCode;
		break;
	case 1004:
		ANSdata.ret_val = iResultCode;
		break;		
	default :
		ANSdata.ret_val = AUTH_ERROR_OTHER;	// 기타의 에러
		break;
	}


	//ktj : 일단 삭제
	// 결과를 조사하고 돌려 준다．
	//ktj : 돌려주는부분을 넣어야함.
	/*
	nrproto_ClientLogin_send( 
			fd, 
			iResultCode,
			szData);
	//nrproto_ClientLogin_send( fd, 1, "OK" );
	*/
	//ANSdata.ret_val = iResultCode;				//tjclan.cpp에 정의된 변수에 답변을 넣는다.
	fd2("\305\254\267\243\270\342\271\366\277\251\272\316\300\307 \264\344\272\257\300\273 \271\336\300\275. :  ret_val %d  ", ANSdata.ret_val);

	

	// 범위내에 들어가고 이르다 OK.
	if( 0 <= AUTH_que[quenum].result && AUTH_que[quenum].result < AUTH_ERROR_END ){
	}else{
		AUTH_que[quenum].result = AUTH_ERROR_OTHER;
	}

	return TRUE;
}




//=================================================================



//sod 셋팅
int sod2INFOindex(char *UserId, char* CharName,char* GServer , int Index)
{
	WebDB.sod2INFOindex(UserId, CharName,GServer , Index);
	return TRUE;
}


//sod 셋팅
int WebDB_Thread::sod2INFOindex(char *UserId, char* CharName,char* GServer , int Index)
{
	Resume();      //재개


	AUTH_QUE AuthQueTmp;
	ZeroMemory(&AuthQueTmp,sizeof(AUTH_QUE));

	AuthQueTmp.fdid = 100;
	AuthQueTmp.command = AUTH_QUE_CMD_SOD2;          // 클랜멤버인지를 알려달라..	
	

	strncpy( AuthQueTmp.id, UserId, sizeof( AuthQueTmp.id ) );
	strncpy( AuthQueTmp.charId, CharName, sizeof( AuthQueTmp.charId ) );
	strncpy( AuthQueTmp.gserver, GServer, sizeof( AuthQueTmp.gserver) );
	AuthQueTmp.sod2index = Index;
	
	//큐에 등록
	if( AUTH_addQue( &AuthQueTmp ) < 0 ){
		// 실패하면 에러를 돌려 준다．기타의 에러．
		//nrproto_CharLogin_send( fd, AUTH_ERROR_OTHER, "authorization required\n" );

		//ktj : 프테때문에 일단 삭제
		//nrproto_ClientLogin_send( fd, 0, "make_Clan() 에러\n" );
		fd2("make_Clan() \277\241\267\257   : \305\245\277\241 \265\356\267\317\276\310\265\312");
		return 0;
	}
	return 1;
}





/*///////////////////////////////////////
: 함 수 명 :  

: 하 는 일 :  클랜멤버인지를 알아내는함수
: 서    식 :
: 인    자 :
/////////////////////////////////////////*/
int AUTH_proc_sodindex( int quenum, int threadindex)
{
	return WebDB.AUTH_proc_sodindex( quenum, threadindex);
}

int WebDB_Thread::AUTH_proc_sodindex( int quenum, int threadindex)
{

	int knret = 1, iBadMsg = 0;
	knret = iCheck.read_sod2(AUTH_que[quenum].id, AUTH_que[quenum].charId,AUTH_que[quenum].gserver, AUTH_que[quenum].sod2index  , AUTH_que[quenum].szSod2Buff);

	// 결과를 격납
	AUTH_que[quenum].result = knret;
	// 결과를 격납
	AUTH_que[quenum].badmsg = iBadMsg;
	return TRUE;
}
   

int AUTH_proc_sodindex_Result( int quenum, int threadindex)
{
	return WebDB.AUTH_proc_sodindex_Result(quenum, threadindex);
}
int WebDB_Thread::AUTH_proc_sodindex_Result( int quenum, int threadindex)
{

	cSinSod2.RecvWebData(AUTH_que[quenum].sod2index , AUTH_que[quenum].szSod2Buff);

	/*

	char *aszAuthErrStr[] = {
	"OK",
	"Err CDKEY", 
	"Err ISAO_ID", 
	"Err PASSWORD", 
	"Err EXPIRE", 
	"Err LOCOUT", 
	"Err ISAO_DB", 
	"Err OTHER", 
	""
	};

	char szData[128];
	
	if( AUTH_que[quenum].result == 0 ){
		szData[0] = '\0';
	}else{
		sprintf( szData, "faild" );
	}

	char szBuffer[256];

	// 에러 메시지 테이블의 범위내라면 문자로 표시
	if( 0 <= AUTH_que[quenum].result && AUTH_que[quenum].result < AUTH_ERROR_END ){
		sprintf( szBuffer, "%s", aszAuthErrStr[AUTH_que[quenum].result] );
	}else{
		sprintf( szBuffer, "ExtraCode=%d", AUTH_que[quenum].result );
	}

	int iResultCode;
	iResultCode = AUTH_que[quenum].result;	// 디폴트는 기타의 에러

	
	// 최종적으로 클라이언트에게 돌려 주는 코드
	switch( iResultCode ){
	case 0:
		ANSdata.ret_val = iResultCode;				//tjclan.cpp에 정의된 변수에 답변을 넣는다.
		strcpy(ANSdata.ret_Buf, AUTH_que[quenum].ret_Buf);
		break;
	case 1:
		ANSdata.ret_val = iResultCode;				//tjclan.cpp에 정의된 변수에 답변을 넣는다.
		strcpy(ANSdata.ret_Buf, AUTH_que[quenum].ret_Buf);
		break;
	case 5:
	case 4:
	case 3:
	case 2:
		ANSdata.ret_val = iResultCode;				//tjclan.cpp에 정의된 변수에 답변을 넣는다.
		strcpy(ANSdata.ret_Buf, AUTH_que[quenum].ret_Buf);
		break;
	case 100:				//일반 멤버,
		ANSdata.ret_val = iResultCode;
		break;
	case 1004:
		ANSdata.ret_val = iResultCode;
		break;
	default:
		ANSdata.ret_val = AUTH_ERROR_OTHER;	// 기타의 에러
		break;
	}


	fd2("클랜멤버여부의 답변을 받음. :  ret_val %d  ", ANSdata.ret_val);

	

	// 범위내에 들어가고 이르다 OK.
	if( 0 <= AUTH_que[quenum].result && AUTH_que[quenum].result < AUTH_ERROR_END ){
	}else{
		AUTH_que[quenum].result = AUTH_ERROR_OTHER;
	}
	*/

	return TRUE;
}
