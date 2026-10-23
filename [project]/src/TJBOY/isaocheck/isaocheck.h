#ifndef __ISAOCHECK_H__
#define __ISAOCHECK_H__
#ifndef USE_PROSTONTALE		//=========== 프리스턴소스인경우
#include "..\\clanmenu\\cE_CViewClanInfo.h"
#endif

#define SAVE_DIR	"savedata\\clanDATA"
#define SAVE_EXE	"cl"

// 이것은 ＩＳＡＯ의 에러 코드이므로
// 변경하고 네 비방하고．

enum{
	ISAO_REJECT_OK = 0,			// 에러없음．성공．
	ISAO_REJECT_BADPASS = 1,	// 패스 워드 불량．
	ISAO_REJECT_BADNAME = 2,	// 유저미 등록．
	ISAO_REJECT_LOCKOUT = 3,	// 일정 기간내에 연속 패스 워드 오버 등으로 로크아웃
	ISAO_REJECT_TEST_OVER = 4,	// 본 등록을 하지 않았기 때문에 ，임시 패스 워드의 유효 기한이 끊어졌다
	ISAO_REJECT_UNFAIR = 5,		// 부정 유저 때문에 ，거부．
	ISAO_REJECT_STOPPING = 6,	// 한때 정지
	ISAO_REJECT_EXPIRE = 7,		// 기한 마감
	ISAO_REJECT_NOT_BOUGHT = 8, // 제품미 구입
	ISAO_REJECT_DB_MAINTE = 9,	// ＤＢ 메인터넌스 중(속)
	ISAO_REJECT_OTHER = 10,		// 기타의 에러

	//주의 ===============================================================================
	//ktj : 100~999번 사이는 웹서버의 php에서 리턴코드로 사용하므로 여기서 절대 정의하지말것.
	//주의 ===============================================================================


	// 이 아래는 변경할 수 있다．
	ISAO_ERR_TOP = 1000,
	ISAO_ERR_WINSOCK_NOERR,		//처음셋팅시사용 
	ISAO_ERR_WINSOCK_START,		//윈속 START시 에러발생					//window only
	ISAO_ERR_WINSOCK_VERSION,	//윈속버전이 틀려 구동되지못함.			//window only
	ISAO_ERR_TCP_CONNECT,		// 보통 사용되는 ＴＣＰ코네쿠토에라
	ISAO_ERR_SSL_READ,			// ＳＳＬ 리드 할 수 없었다．
	ISAO_ERR_SSL_TIMEOUT,		// 타임 아웃했다．
	ISAO_ERR_NONE_REEJCT,		// 응답 번호가 문자열에 들어가고 있지 않다.
	ISAO_ERR_UNKNOWN_REJECT,	// 응답 번호가 인식할 수 없다.
	ISAO_ERR_RECONNECT,			// 리컨넥트하도록한다.
	ISAO_ERR_END
};

int IsaoCheck( char *id, char *pass, int iCheckOn, int iTimeOut , char *ipaddr, long StartTime );
int IsaoCheck2( char *id, char *pass, int iCheckOn, int iTimeOut , char *ipaddr, long usertime);

int UserLogin(char* UserID, char* PassWord);
int InsertUser(char* UserID,char* PassWord,char* UserName);
int DeleteUser(char* UserID);
int LoginCheck(char* UserID,char* start_time);
int Login_List(char* UserID,char* end_time,char* use_time);
#endif


//해외
bool MultiByteToWideCharByCodePage(char *szMBCS,wchar_t *szUnicode,int iCodePage);
bool WideCharToMultiByteByCodePage(wchar_t *szUnicode,char *szMBCS,int iCodePage);
bool MBCSToUTF8(char *szMBCS,char *szUTF8,int iMBCSCodePage);
bool UTF8ToWeb(char *szUTF8,char *szWeb);
bool MBCSToWeb(char *szMBCS,char *szWeb,int iMBCSCodePage);





class ISAOCHECK
{
public:
	int iCheckFlag;
	int iTimeOut;
	int hostPORT;
	char hostName[64];
	//ygy
	int hostPORT1;
	char hostName1[64];
	//end ygy


	int HSsu;					//헥사스트링의 갯수
	char *fileDATA_HS[30];		//총 2048binary의 데이타가 4096 hexa string으로 바뀌어들어감.


    ISAOCHECK();
	~ISAOCHECK();

	void init();								//겜 처음에서 셋팅
	void init(int iCheckON, int iTimeOUT );		//웹db사용시마다 셋팅
	void init_HostName_Port(char *hostname, int port);
	void init_HostName_Port1(char *hostname, int port);
	void close();
	void main();
	
	
	//주로 스트링을 받는 함수
	int connect_WEBdb( char *szWriteBuffer, char *szReadBuffer);
	//ygy
	int connect_WEBdb_1( char *szWriteBuffer, char *szReadBuffer);
	//파일등의 바이나리를 읽는 함수.
	int connect_WEBdb_BINARY( char *szWriteBuffer, char *szReadBuffer, int *dataSize);

	int chk_readbuffer(char *cmd, char *readbuf, int *iRejectC );			//읽은 데이타에서 답변용 int데이타를 뽑아낸다.
	int chk_readStrBuf(char *cmd, char *readbuf, char *retBuf);	//읽은 데이타에서 답변용 스트링데이타를 뽑아낸다.

	int IsaoCheck( char *id, char *pass, char *ipaddr, long StartTime);
	int IsaoCheck_GameExit(char *id, char *pass, char *ipaddr, long usertime);



	//클랜원인가의 여부(클랜칲이면 클랜이름전해줌.)
	//int isClanMember(char *id, char *ret_Buf);
	int isClanMember(char *ClanZang_id, char* gserver,char* ClanZang_ChaName,char* ret_Buf);

	//호동Clan추가
	int isCheckClanMember(char *pszServerName, char *pszUserName);

	//멤버 추가
	//int joinClanMember(char *clanName, char *memId, char *memCharId);
	//int joinClanMember(char *ClanZang_ID, char *gserver, char *ClanZang_ChaName,char* ClanName,char* ClanWon,char* ClanWon_ID);
	int joinClanMember(char *ClanZang_ID, char *gserver, char *ClanZang_ChaName,char* ClanName,char* ClanWon,char* ClanWon_ID,int level,DWORD chtype,int joinlevel,char *chipflag);
	
	

	//YGY:isCheckClanJang //관련된 계정이 클랜장으로 등록되어 있는지 체크
	int isCheckClanJang(char *ClanZang_ID, char *gserver);
	//YGY: 클랜이름 중복체크
	int isCheckClanName(char *ClanName,char *gserver);
	//makeClan
	int makeClan(char *ClanZang_ID, char* gserver,char* ClanZang_ChaName,char* ClanName,char *explanation,DWORD chtype,int level,char* data);
	//clanName버퍼에서 원하는 칼럼 데이타를 읽어옴.
	int read_ClanName(int readClumns);
	//int read_ClanData(char *clname, char *ret_Buf);		//clanName의 데이타를 가져옴.
	int read_ClanData(char *ClanZang_ID,char* gserver,char* ClanZang_ChaName,char* ret_Buf);		//clanName의 데이타를 가져옴.
	//int BreakUPClan(char *clname);			//클랜을 삭제함.
	int BreakUPClan(char *ClanZang_ID,char *gserver,char *ClanZang_ChaName, char *ClanName);			//클랜을 삭제함.
	//int Banishiment(char *clname, char *banisID);	//특정id을 추방함.
	int Banishiment(char *ClanZang_ID,char *gserver,char *ClanZang_ChaName,char *ClanName, char *ClanWon);
	int WarhouseRight(char *clname, char *right_str);	//창고권한 바꿈.
	int updateFlag(char *clname);
	
	int Secession(char* clanzang,char* gserver,char* SecessionID,char* clanname);	//탈퇴
	//ygy : 초기 창 관련 플래그
	int isPFlag(char *userid,char *clanname, char *chaname,char *gserver,char *pflag, char *gunbun);
	//ygy : 클랜마크번호로 클랜 정보 갖어오기
	int ReadClanInfo(char *clanNum,char* ret_Buf);


	//서버의 특정 디렉토리안의 클랜의 그림을 읽어들인다.
	int read_ClanImage(char *clanname,char *filename);
	//다른 클랜마크 이미지 읽어 들임
	int read_ClanImage2(char *clanname,char *filename,CLANINFO *ci);	
	//특정디렉토리안의 클랜그림을 세이브함 : 모두 hexa 스트링으로 서버에 저장됨.
	int write_ClanImageFile(char *filename);		

	//특정 캐릭터에 대한 경고문
	//int connect_WEBdb_NOTICE1(char *severIP, int port, char *szWriteBuffer, char *szReadBuffer);
	int connect_WEBdb_NOTICE(char *szWriteBuffer, char *szReadBuffer);
	int ReadNotice(char *userid,char *chaname,char *gserver,char *buf);	
	//이임
	int LeaveClan(char *clanname,char *chaname,char *gserver);

	//이임관련 플래
	int isKPFlag(char *userid,char *clanname, char *chaname,char *gserver,char *pflag, char *gunbun);

	int AppointSubChip(char *gserver,char* clanwon);//부클랜장 임명
	int ReleaseSubChip(char *gserver,char* clanwon); //부 클랜장 해임

	//서버에서 웨이브파일 가져오기
	int	GetWavFile(char* FileName);


	void Convert_bin2HS(char *sorce, int len);
	int WEBdb_Exam();
	






	int read_sod2(char *usid, char *charname,char *gserver, int index , char *szSod2Buff);

};




#undef EXTERN
#ifdef __ISAOCHECK_CPP__
	#define EXTERN
#else
	#define EXTERN extern
#endif

EXTERN ISAOCHECK iCheck;




