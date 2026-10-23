#ifdef USE_PROSTONTALE		//=========== 프리스턴소스인경우
#else
#include <windows.h>
#endif
//유저의 클랜에서의 현재위치
#define CLANUSER					100		//일반유저
#define CLANCHIP					101		//칲
#define CLANNO_REGESTER				102		//무등록자
#define CLAN_NOCONNECT				103		//컨넥트가 안되서 설정불가
#define CLAN_SUBCHIP				104		//부클랜칩





#define CLAN_NPC_START 0 //클랜NPC를 누르면 시작하는 메뉴

#define CLANMESSAGEBUF_MAX 1024



#define CLAN_MAKE_is	1 //클랜 등록 의사 결정 메뉴
#define CLAN_MONEY_is	2 //클랜 만들 돈지불 결정 메뉴
#define CLAN_NAME_is	3 //클랜 이름 만들기 메뉴
#define CLAN_MARK_expl  4 //클랜 마크 설명 메뉴
#define CLAN_INSPECTION_expl	5 //클랜 심사를 알려주는 설명 메뉴
#define CLAN_NAME_BEING         6
#define CLAN_MAKE_FINISH        7
#define CLAN_CHIP_ReadDATA		8 //칩용메뉴를 위해 데이타를 찾는다.
#define CLAN_NONAME	            9
#define CLAN_GO_MAKE			10
#define CLAN_GO_NPCSTART          11
#define CLAN_NotMake			254 //클랜을 만들지 못하니 돌아가라
#define CLAN_AllEnd				255 //클랜함수를 나갈때 꼭 들리는 메뉴
#define CLAN_ERROR				1000//에러용 스트링을 보여준후 되돌아간다.




#define CLAN_CHECK_CLANMARK		200
#define CLAN_READ_CLANMARK		201
#define CLAN_REAMMAKR_ERROR       202  
#define CLANMAKE_REAMMAKR_ERROR       203  

#define CLAN_USER_DATA           300 //클랜 유저로 이동 시켜줌





//===============클랜칩 관련 디파인
#define CLAN_CHIP				20 //칩
#define CLAN_WAREHOUSE			21 //창고
#define CLAN_WAREHOUSE_RIGHT		22 //창고 권한
#define CLAN_MARK				23 //마크
#define CLAN_MARK1				24
#define CLAN_MARK2				25
#define CLAN_MARK3				26
#define CLAN_REMOVE				27 //추방
#define CLAN_RELEASE			28 //해체
#define CLAN_HELP				29 //도움말
#define CLAN_NOMAKE				30		//클랜원이 6인 이하일 경우 메뉴 첫머리에 알려준다
#define CLAN_HELP_CONTENT		31
#define CLAN_REMOVE_RESULT		32
#define CLAN_RECONF_RELEASE       33
#define CLAN_FINISH_RELEASE       34
#define CLAN_REMOVE_SHORTCLANWON   35
#define CLAN_RELEASE_LOADING       36
#define CLAN_REMOVE_SHORTCLANWON_LOADING 37
#define CLAN_REMOVE_LOADING        38
#define CLAN_REMOVE_SHORTCLANWON_RESULT   39 
//#define CLAN_RELEAE_BEFORE		35 //YGY:클랜 해체하기 전에 클랜관련 정보를 읽어온다.

//===============클랜유저 관련 디파인
#define CLAN_USER				40 //클랜 구성원
#define CLAN_JOIN				41 //클랜 가입
#define CLAN_SECESSION			42 //클랜 탈퇴
#define CLAN_USER_WAREHOUSE		43	//유저용 창고
#define CLAN_USER_HELP			44
#define CLAN_USER_NOMAKE			45
#define CLAN_USER_HELP_CONTENT	46
#define CLAN_RECONF_SECESSION    47
#define CLAN_FINISH_SECESSION     48
#define CLAN_USER_NOMAKE_FLAG      49
#define CLAN_USER_FLAG            50
#define CLAN_USER_ReadDATA        51
#define CLAN_RECONF_SECESSION_LOADING     52

#define CLANUSER_RELEASEMESSAGE	53 //해체된 클랜원
#define CLANUSER_GO_RELEASEMESSAGE 54
#define CLANUSER_RELEASE_FLAG		55

//챗리스트메인 플러그==========================================================
//#define CHATLIST_START           900 
#define CHATLIST_END			901
#define CHATLIST_ERROR			902
#define CHATLIST_READDATA		903
#define CLAN_DISPLAY_OPEN		904
#define CLAN_DISPLAY_END			905
#define CLAN_DISPLAY_INFO         906
#define CLAN_DISPLAY_CLOSE			907
#define CLAN_CLOSE_END_BEFORE			908
#define CLAN_CLOSE_ENDING             909
#define CHATLIST_READ_CLANMARK       910
#define CLAN_DISPLAY_BEFORE		911

//==============cldata업데이트==================
#define UPDATE_ISCLANMEMBER           1003
#define UPDATE_CLANWON				1004
#define ISPFLAG_USER				1005
#define ISPFLAG_CHIP				1006
#define CLAN_isCheckClanJang_LOADING   1007
#define CLANCHIP_DOWN6_CLANWON_MESSAGE 1008
#define CLANSERVER_INSPECT		    1009
#define CLANSERVER_INSPECT_BEFORE      1010
#define CLANINFO_READMARK			1011
#define CLANINFO_DOWNLOAD			1012
#define CLANINFO_END				1013
#define CLANMAKE_DOWN_INFO			1014
#define CLANMAKE_READ_CLANMARK		1015
#define CLAN_isCheckClanJang_Result	1016	
#define CLANMAKE_NOMONEY				1017
#define CLANUSER_JOIN				1018
#define CLANUSER_REMOVE				1019 //클랜원이 추방당했을경우
#define CLANUSER_AUTORELEASE			1020 //자동해체
#define MYCLAN_UPDATE				1021
#define MYCLAN_CLANMARK_UPDATE		1022
#define CLANCHIP_GO_RELEASEMESSAGE		1023
#define CLANCHIP_RELEASEMESSAGE		1024
#define CLANCHIP_RELEASE_FLAG			1025
#define CLANUSER_CHECKPFLAG			1026
#define CLAN_CLANWONOVER				1027 //클랜원수 체크
#define CLAN_ABILITYDOWN				1028
#define CLAN_NOLEVEL				1029
#define CLAN_NOTICE					1030
#define CLAN_DRAWNOTICE				1031
#define CLAN_DRAWNOTICE_END			1032
//이임관련========================START
#define CLANCHIP_LEAVECLAN			1033
#define CLANCHIP_LEAVECLAN_NOMONEY		1034
#define CLANCHIP_LEAVECLAN_SUCESS		1035
#define CLANCHIP_LEAVECLAN_CONFIRM		1036
#define CLANCHIP_LEAVECLAN_RECONFIRM	1037
#define CLANCHIP_LEAVECLAN_NOLEVEL		1038
#define CLANCHIP_LEAVECLAN_DO			1039
#define CLANCHIP_NOLEAVECLAN			1040
#define CLANCHIP_LEAVECLAN_PFLAG		1041
#define CLANCHIP_LEAVECLAN_MESSAGE		1042
#define CLANUSER_LEAVECLAN_MESSAGE		1043
#define CLANCHIP_LEAVECLAN_MESSAGEEND	1044
#define CLANUSER_LEAVECLAN_MESSAGEEND	1045  //
#define CLANCHIP_LEAVECLANDOWNLEVEL	1046  //클랜원이 레벨이 되지 않아서 클랜원을 받아들일수 없음
//=================================END


//부 클랜칩 임명 및 해임=====================START
#define CLANCHIP_SUBCHIP_LIST			1047  //클랜원들 리스트 보여줌 부 클랜칩이 있으면 따로 보여줌
#define CLANCHIP_RESUBCHIP_APPOINT		1048  //부 클랜칩 임명 재확인
#define CLANCHIP_RESUBCHIP_APPOINT_END	1049  //부 클랜칩임명 마지막 메세지
#define CLANCHIP_RELEASESUBCHIP		1050  //부 클랜칩해임 재확인
#define CLANCHIP_RELEASESUBCHIP_END	1051  //클랜부칩 해임 마지막 메세지
#define CLANCHIP_APPOINTSUBCHIP_DO		1052  //클랜부칩 임명 ASP호출
#define CLANCHIP_RELEASESUBCHIP_DO		1053  //클랜부칩 해임 ASP호출

#define CLANCHIP_SUBCHIP_APPOINTMSG	1054  //클랜부침이 임명되다고 한번 보여줌
#define CLANCHIP_SUBCHIP_RELEASEMSG	1055  //클랜부침이 해임되다고 한번 보여줌
#define CLANUSER_SUBCHIP_APPOINTMSG	1056  //클랜부침이 임명되다고 한번 보여줌
#define CLANUSER_SUBCHIP_RELEASEMSG	1057  //클랜부침이 해임되다고 한번 보여줌
#define CLANCHIP_SUBCHIP_APPOINT_FLAG	1058  //클랜부침이 임명되어다는 플래그세팅
#define CLANCHIP_SUBCHIP_RELEASE_FLAG	1059  //클랜부침이 해임되어다는 플래그세팅

#define CLANCHIP_SUBCHIP_READMSG_FLAG	1060  //클랜장이 부칩이 임명되어다는 메세지를 읽었을때
#define CLANUSER_SUBCHIP_READMSG_FLAG	1061  //클랜유저가 부침관련 메세지를 읽어다는 플래그 세팅

#define CLANCHIP_SUBCHIP_LOAD			1062  //클랜워리스트 갖어오기(아직 사용안함)
#define CLANCHIP_SUBCHIP_LOAD_DATA		1063
#define CLANCHIP_NOSUBCHIP			1064


//================================END
//부칩이
#define CLANSUBCHIP_INSERTCLANWON		2000 //부칩이 클랜원추가

#define CLANWON_MAX					100








#define CLAN_JOIN_before			60
//#define CLAN_JOIN				31
#define CLAN_JOIN_After			62

#define CLAN_JOIN_chip			63
#define CLAN_JOIN_chip_NO		64
#define CLAN_JOIN_chip_YES		65

//호동Clan추가
#define CLAN_IS_CHECK_MEMBER		100
#define CLAN_IS_CHECK_MEMBER_END	101


//처음 시작시 클랜멤버인지 아닌지를 미리 알아야함.
#define CLAN_isClanWon			66

#define CLAN_MAKE_examination		67		//클랜심사하기
#define CLAN_isCheckClanJang      68       //같은 계정에 클랜장이 있는지 체크
#define CLAN_LOADING			69
//===============클랜 접속시 관련 플래그 메세지
#define CLANCHIP_MAKE_FIRSTMESSAGE     70    //처음 클랜을 만들었을때 클랜장이 NPC를 클릭했을때 보여주는 메세지
//#define CLANUSER_MAKE_FIRSTMESSAGE     71    //처음 클랜을 만들었을때 클랜구성원이 NPC를 클릭했을때 보여주는 메세지
#define CLANCHIP_DOWN6_CLANWON         71   //클랜원이 6명 이하로 떨어짐
#define CLANCHIP_BREAKCLAN            72

#define CLANUSER_MAKE_FIRSTMESSAGE    73
#define CLANUSER_DOWN6_CLANWON_MESSAGE  74
#define CLANUSER_DOWN6_CLANWON         75





#define BUTTONBOX_WIDTH   68
#define BUTTONBOX_HEIGHT  27
#define BUTTONTEXT_WIDTH  32
#define BUTTONTEXT_HEIGHT 16

#define BOX_MAX_WIDTH (((4 * BOXCENTER_WIDTH) + (2*BOXCORNER_WIDTH) + 5) )

#define ICON_START       55
#define ICON_ADD		 20
#define SCROLLBAR_LEFT    236
#define SCROOBAR_TOP	54
#define DISPLAY_MAX		8

//클랜 이름 좌표
#define CLAN_NAME_POS_X     65
#define CLAN_NAME_POS_Y     8

//클랜 헬프 관련

#define CLANHELPICON_WIDTH   109
#define CLANHELPICON_HEIGHT   30
#define CLANHELPICON_CAP		 40
#define CLANHELPICON_START_Y   105
#define CLANHELPICON_DISPLAY_MAX		4
#define CLANHELPCONTENT_MARK_X   40
#define CLANHELPCONTENT_MARK_Y   40
#define CLANHELPCONTENT_X   100
#define CLANHELPCONTENT_Y   50
#define HELPWIDTH 256
#define HLEPDISPLAY_MAX		8
#define HELPCONTENT_X        19
#define HELPCONTENT_Y        56
#define HELPCONTENT_CAP      20
#define CLANCHIPHELPINF "image\\clanImage\\help\\ClanHelp.inf"
#define CLANUSERHELPINF "image\\clanImage\\help\\ClanUserHelp.inf"


#define SAVEDATA_DIR	"savedata"
#define CLANDATA_DIR   "savedata\\clanDATA"



//========================================================

#define BOXCORNER_WIDTH		64
#define BOXCORNER_HEIGHT		32
#define BOXCENTER_WIDTH		32
#define BOXCENTER_HEIGHT		32

//클랜 마크 이름 위치
#define CLANNAME_X			54  
#define CLANNAME_Y			12

#define CLANNAME_WIDTH		152
#define CLANNAME_HEIGHT		28

#define CLANNAMETEXT_X       128
#define CLANNAMETEXT_Y		20

//클랜 마크 박 위치
#define CLANMARKBOX_X		93 
#define CLANMARKBOX_Y		47

//클랜 마크 위치
#define CLANMARK_X		111
#define CLANMARK_Y		60

//BUTTON박스가 한개 일때 좌표
#define BUTTON1_BOX_X			104
#define BUTTON1_BOX_Y			222

#define BUTTON1_X				BUTTON1_BOX_X + 9
#define BUTTON1_Y				BUTTON1_BOX_Y + 4

//BUTTON박스가 두개 일때 좌표
#define BUTTON2_BOX_X			69
#define BUTTON2_BOX_Y			221

#define BUTTON2_X				BUTTON2_BOX_X + 9
#define BUTTON2_Y				BUTTON2_BOX_Y + 4

#define BUTTON3_BOX_X			139
#define BUTTON3_BOX_Y			221

#define BUTTON3_X				BUTTON3_BOX_X + 9
#define BUTTON3_Y				BUTTON3_BOX_Y + 4

//클랜칩 메인 메뉴
#define CLANCHIPMENU_RELEASE_X	104
#define CLANCHIPMENU_RELEASE_y	62

#define CLANCHIPMENU_REMOVE_X		104
#define CLANCHIPMENU_REMOVE_Y		92

//이임
#define CLANCHIPMENU_LC_X		104
#define CLANCHIPMENU_LC_Y		122
//도움말
#define CLANCHIPMENU_HELP_X		104
#define CLANCHIPMENU_HELP_Y		182
//부클랜칩
#define CLANCHIPMENU_SUBCHIP_X	104
#define CLANCHIPMENU_SUBCHIP_Y	152

//클랜 메뉴 타이틀
#define CLANTITLE_X				68
#define CLANTITLE_Y				12

//도움말 목록
#define CLANHELPLIST_X			68
#define CLANHELPLIST_Y			76

//추방 클랜원 리스트 위치
#define CLANWONLIST_X			50
#define CLANWONLIST_Y			60

//클랜유저 탈퇴
#define CLANUSER_SECESSION_X	64
#define CLANUSER_SECESSION_Y	127

#define CLANUSER_HELP_X			64
#define CLANUSER_HELP_Y			167

//클랜 결성 관련 텍스트 좌표
#define CLANMAKETEXT_X			25
#define CLANMAKETEXT_Y			40

#define LOADTEXT_X				70
#define LOADTEXT_Y				50

//클랜장 메뉴텍스트 관련
#define CLAN_RELEASE_X			60
#define CLAN_RELEASE_Y			80

#define CLAN_RECONF_RELEASE_X			30
#define CLAN_RECONF_RELEASE_Y			70

#define CLANNOMAKE_X		    30
#define CLANNOMAKE_Y		    60

#define CLANRELEASE_FINISH_X		50
#define CLANRELEASE_FINISH_Y		60

#define CLANUSERTEXT_X           45
#define CLANUSERTEXT_Y			60

#define CLANINPUTNAME_X			78
#define CLANINPUTNAME_Y			70


//========================================================




#define XCAP					20 // 텍스트 출력시 시작하는 x위치값 보정
#define YDOWN					-2  //텍스트 출력시 시작하는 y위치값 보정
#define REMOVE_ICON				20 //추방 아이콘 위치 이동
#define WHR_ICON				30
#define HELP1					"image\\Sinimage\\chipbutton\\help1.txt"
#define HELPMSG_MAX				256
#define CLANMARK_TXT_X           34
#define CLANMARK_TXT_Y           10
//크랜 헬프버턴의 텍스트 위치
#define CLANHELPTEXT_Y               25
//클랜텍스트 이미지 출력
#define CLANTEXT_IMAGE_X         68
#define CLANTEXT_IMAGE_Y          12
#define CLANTEXT_IMAGE_WIDTH         120
#define CLANTEXT_IMAGE_HEIGHT         30





#define CLANHELPICON_NUM		100




#define CHIP_BUTTON_X_MOVE		30
#define CHIP_BUTTON_Y_MOVE		20

#define MAKEMONEY 500000			//클랜결성을 위해 50만원필요
#define CLANCHIP_LEAVECLAN_MONEY	300000
//#define MAKEMONEY 3000			//클랜결성을 위해 4만원필요
#define ABILITY   10000 	        //클랜결성을 위해 10000 능력치가 필요
#define CLAN_MAKELEVEL  40		//클랜을 만들수 있는 레벨

//web db로 부터 답변을 받아 저장하는곳.
typedef struct {
	int  ret_val;					//web db에서 유저의 클랜에서의위치(클랜장,클랜원,가입안함)가져옴.
	char ret_Buf[2048];				//web db를 통해 들어온 스트링을 세이브해두었다가 필요할때 꺼내쓰기위한버퍼


} ANSWERDATA;

typedef struct _tagMyClanInfoHeader
{
	char		ClanMark_Num[40];   //클랜 마크 번호
	int		nClanMark;
	char		ClanName[40];   //클랜 이름		
	char		ClanReport[90];
	char		Gserver[4];
	char		UniqueClaClanInfo_nNum[64];

	
}MYCLANINFOHEADER;

//=============================================순수 윈도용
typedef struct {
	HBITMAP hb;
	void *pixBuf;
	char fname[64];
} DIBSectionBUF;


typedef struct {
	//===========================================================================================
	int  myPosition;			//100:일반 클랜원 101:클랜칲  102:가입안되어있음. 103: 클랜서버상의 에러
	
	int	 money;					//현재 유저가 가진 돈
	int  ability;





	//클랜장이 권유해서 클랜원으로 가입할 사람의 id
	char clanJoinID[64];		
	char clanJoinCharID[64];
	int  clanJoinJob, clanJoinLevel;

	char chipCharID[64];		//클랜에 가입을 권유한 클랜칲의 id를 저장해둔다.

    //ygy:가입을 권유하는 클랜과 클랜장의 정보를 일반인이 저장함
	char  szJoinClanName[64];		//조인 클랜 이름
	char  szJoinClanChipName[64];   //조인하는 클랜장 이름
			
	
	//===========================================================================================
	char name[20];				//클랜의 이름
	char explanation[128];		//클랜의 설명글(현재 클랜만들때 셋팅안하고 있음.)
	
	//ygy
	char stats[2];
	char sec[30];
	char szRegiD[50];
	char  limitD[30];
	char  DelActive[10];
	char  gserver[30];
	char  szID[64];       //일반 계정	//당 유저의 id : 게임메인에서 받아옴.
	char  ChaName[64];    //캐릭터 이름 //당 유저의 캐릭터id : 게임메인에서 받아옴.
	char  ClanZang_ID[64];
	char  ClanZang_Name[64];	
	char  ClanMark[64];	//100188

	int intClanMark;
	int nLastDay;  //클랜이 등록되기까지 남은 날
	BOOL bIsOneDay;
	int	nMinute;
	int	nHour;
	
	int nCurrentClanWon_Num;  //현재 클랜원수 [클랜장 포함]
	int nClanWon_Max;        //클랜원 최대수
	
	

	//bmp이름 넣을것.


	//===========================================================================================
	char  clanWon[CLANWON_MAX][30];        //클랜서버에서 보내준 클랜원을 저장(클랜장 제외)
	int	 clanWonStrLen[41];      //각 클랜원 문자열 길이
	char  szClanMarkName[50];     //클랜마크 비트맵이름
	int   nTotalClanWonCount;     //현재 클랜원수
	char  PFlag[5];             //클랜 첫 화면 플래그
	char  LeaveClanFlag[5];       //kflag
	int	 nKFlag;				//kflag
	int   nKFlag_Count;
	char  OldChipChaName[64];
	char  SubChip[64];			//부 클랜칩
	char	 OldSubChip[64];
	char	 isSubChip[2];

	//호동Clan추가
	smCHAR *TargetChar;        //Target Character


#ifdef USE_PROSTONTALE		//=========== 포리스턴소스인경우
	LPDIRECTDRAWSURFACE4	hClanMark;
	LPDIRECTDRAWSURFACE4	hClanMark16;

#else
	HBITMAP				hClanMark;
	HBITMAP				hClanMark16;
#endif


	//(status,StartMessageFlag) : 
	//(0,1) 클랜 결성 첫 메세지 띄움 (0,2)클랜 결성 첫메세지 읽음
	char  StartMessageFlag[1];      
	//end-ygy

 
	DWORD CNFlag; //sod주관 클랜의 멤버인지의 여부	//cE_isclanmember.cpp에서 셋팅됨
} CLANDATA;

typedef struct _tagClanWonList
{
	char  clanWon[CLANWON_MAX][30];

}CLANWONLIST;


typedef struct _tagHelpContent
{
	char szHelpContent[HELPWIDTH];
	BOOL bFlag;
}HELPCONTENTS;


typedef struct _tagClanHelpIcon
{	
	char ImagePath[255];
	char ImagePath2[255];	
	char ImagePath3[255];	
	char TextFilePath[255];
}CLANHELPICON;
//end-ygy
typedef struct _tagMenuBMP
{
	HBITMAP membmp;
}MENUBMP;

class CLAN_MENU
{
public:
	CLAN_MENU();
	~CLAN_MENU();


	
	
	int menu_joinOpen(char *chipCharID);	//칲의 캐릭터 id
	int menu_joinOpen_Chip(int joinOK, char *clanWonID, char *clanWonCharID, int clanWonJob, int clanWonLevel);		//칲의 캐릭터 id
		
	//호동Clan추가
	int IsCheckClanMember(smCHAR *pChar);


	//PartyInterface 리소스..
	#ifdef USE_PROSTONTALE		//=========== 포리스턴소스인경우
	int	MatPartyBackGround;	//배경
	//int	hBoxCorner,hBoxLine;	//박스그리기
	int	hBox1,hBox2,hBox3,hBox4,hBox5,hBox6,hBox7,hBox8,hBox9; //박스그리기



	int	hScl_Bar_tj;
	int	hScl_icon_tj;


	/*
	LPDIRECTDRAWSURFACE4				hBox1_ktj; //확인

	LPDIRECTDRAWSURFACE4				hOk_Gray,hOk_Yellow; //확인
	LPDIRECTDRAWSURFACE4				hCancel_Gray,hCancel_Yellow; //취소
	LPDIRECTDRAWSURFACE4				hYes_Gray,hYes_Yellow; // 예
	LPDIRECTDRAWSURFACE4				hNo_Gray,hNo_Yellow; //아니오

	LPDIRECTDRAWSURFACE4	hMark,hClanName;
	LPDIRECTDRAWSURFACE4	hSecession[2];
	LPDIRECTDRAWSURFACE4	hClose[2];
	LPDIRECTDRAWSURFACE4	hIcon;
	LPDIRECTDRAWSURFACE4	hIcon_Click;
	LPDIRECTDRAWSURFACE4	hHelp1;
	LPDIRECTDRAWSURFACE4	hClinetMark;
	LPDIRECTDRAWSURFACE4	hServerMark;
	LPDIRECTDRAWSURFACE4	hWareHouse[2]; //마우스가 위로 올라왔을때
	LPDIRECTDRAWSURFACE4	hWHRight[2];
	LPDIRECTDRAWSURFACE4	hClanMark[2];
	LPDIRECTDRAWSURFACE4	hRemove[2];
	LPDIRECTDRAWSURFACE4	hClanRelease[2];
	LPDIRECTDRAWSURFACE4	hHelp[2];
	LPDIRECTDRAWSURFACE4	hOut[2];

	LPDIRECTDRAWSURFACE4	hButton_Box;
	LPDIRECTDRAWSURFACE4	hClanTitle;
	LPDIRECTDRAWSURFACE4	hRemoveToMenu[2];
	LPDIRECTDRAWSURFACE4	hScl_Bar;
	LPDIRECTDRAWSURFACE4	hScl_icon;
	LPDIRECTDRAWSURFACE4	hClanHelpIcon[CLANHELPICON_NUM][4];
	LPDIRECTDRAWSURFACE4    hClanNameInput;

	LPDIRECTDRAWSURFACE4   hTxt_Remove[2]; //추방
	LPDIRECTDRAWSURFACE4   hTxt_Release[2]; //해체
	LPDIRECTDRAWSURFACE4   hTxt_Mark[2]; //마크
	LPDIRECTDRAWSURFACE4   hTxt_Help[2]; //도움말
	LPDIRECTDRAWSURFACE4   hTxt_Secession[2];//탈퇴
	*/

	
	char		szHelpMsg1[HELPMSG_MAX];
	char		szErrorMsg[256];

	#else
	HBITMAP	hBox1,hBox2,hBox3,hBox4,hBox6,hBox7,hBox8,hBox9;

	HBITMAP	MatPartyBackGround;
	//HBITMAP	hBoxCorner,hBoxLine,hBoxLine1,OldBmp;
	HBITMAP	hOk_Gray,hOk_Yellow; //확인
	HBITMAP	hCancel_Gray,hCancel_Yellow; //취소
	HBITMAP	hYes_Gray,hYes_Yellow; // 예
	HBITMAP	hNo_Gray,hNo_Yellow; //아니오

	HBITMAP	hMark,hClanName;
	HBITMAP	hSecession[2];
	HBITMAP	hClose[2];
	HBITMAP	hIcon;
	HBITMAP   hIcon_Click;
	HBITMAP	hHelp1;
	HBITMAP	hClinetMark;
	HBITMAP	hServerMark;
	HBITMAP	hWareHouse[2]; //마우스가 위로 올라왔을때
	HBITMAP	hWHRight[2];
	HBITMAP	hClanMark[2];
	HBITMAP	hRemove[2];
	HBITMAP	hClanRelease[2];
	HBITMAP	hHelp[2];
	HBITMAP	hOut[2];
	HBITMAP	hButton_Box;
	HBITMAP	hClanTitle;
	HBITMAP	hRemoveToMenu[2];
	HBITMAP	hScl_Bar;
	HBITMAP	hScl_icon;
	HBITMAP   hClanHelpIcon[CLANHELPICON_NUM][4]; //이차원 배열로 처리

#ifndef _LANGUAGE_BRAZIL //브라질 클랜 //해외
	HBITMAP   hTxt_Remove[2]; //추방
	HBITMAP   hTxt_Release[2]; //해체
#endif
	HBITMAP   hTxt_Mark[2]; //마크
	HBITMAP   hTxt_Help[2]; //도움말
	HBITMAP   hTxt_Secession[2]; //탈퇴

	//HWND		m_hClanName;	
	char		szErrorMsg[256];

	#endif

	
	int  nReleaseClanWon;		//삭제 클랜원
	
	
	int nScl_Bar;           //스크롤바
	//int g_nClanIconNum;			//헬프아이콘 총수
	int g_HelpIconNumber;    //현재 스코롤 위치를 저장
	//int g_HelpDisPlay;       //헬프 총갯수
	char szHelpContent[1024]; //헬프 내용 저장
	


	
	char     szRemoveClanWonResultMsg[1024];
	char     szReconfReleaseMsg[2048];
	
	

	//창고 권한, 추방 아이콘 관련 구조체
	typedef struct _tagButtonClick
	{
		char szClanWon[20];
		BOOL bIsClick;
	}BUTTONCLICK;
	BUTTONCLICK buttonclick[30];
	BUTTONCLICK removeclick[30];
	int	nClanWon[40];
	//추방할 클랜원 저장
	char szRemoveClanWon[500];
	//클랜 헬프 아이콘 구조체
	CLANHELPICON ClanHelpIcon[CLANHELPICON_NUM];

	MENUBMP *membmp;

	

	int MatTitle[4];		//타이틀.(퀘스트,채팅,동료
	
	int xLeft, xRight;

	
	RECT scrIconPos;		//스크롤바의 움직이는아이콘위치설정
	//int menuSu;
	void InitMenuSet(int menuN);
	int chk_readStrBuf(char *cmd, char *readbuf, char *retBuf);		//웹 db에서 온 스트링에서 원하는 스트링을 뽑아냄.


	void Init();
	void NoticeMessage_Init();
	void Init2();
	void NoticeMessage_Init2();

	void Load();
	void Close();
	
	int	 Main(int sw);
	int	 Draw();
	int	 DrawBox(int x,int y,int x_width,int y_height);
	void menuInit(int num);
	void Draw_C_menuN();
	void Draw_C_Text(char *str, int x, int y, int sw);


	
	void RenderMenu();
	//부수적 함수들 ( 마우스로 누른 메뉴 번호 첵크)
	int chkMenu(int x, int y, RECT *r) ;
	int chkeckMenuN();
	void menuSet(int sw);



	int		nChkMark;


	//ygy
	int chkMark(char *clanName);
	
	void SetUserData(char* id, char* chaname, char* gserver,int money, int ability);
	//클랜에 가입할것을 권유받는 메뉴의 기초 함수
	void SetJoinData(char *joinclanname, char *joinclanchip);
	
	//클랜 가입
	void clanCheckMark_main();
	
	void clanJoin_main(); //클랜조인 메세지처리
	//서버 접근 중일때 보내는 함수
	void Loading();
	//join관련 함수
	void ClanJoin();
	//클랜장이 클랜원을 가입시키는 함수
	void joinClanMember();




	// 외부에서 사용되는 클랜의 구동 셋팅 함수들===================================start
	void CLANmenu_INIT();					//처음 init
	int  CLANmenu_clanPOSITION();			//처음구동시 나의 포지션
	int  CLANmenu_meetNPC();				//npc와 만남.

	void CLANmenu_ChatList(int sw);				//클랜원들과의 챗리스트 메뉴 구동함수들
	void CLANmenu_DeleteCha();				//캐릭터를 지울때 클랜원인지알아내사용함.
	// 외부에서 사용되는 클랜의 구동 셋팅 함수들===================================end

	void GuideHelpDraw(); /////////////////////// 도움말 그리기////////////////////////
};

typedef struct _tagClanMsg
{
	POINT pos;
	char szMsg[1024];
}CLANMSG;

void Draw_Text(int x, int y,char *str);
void Text_Out(HDC hdc,int x, int y,char *str);





#ifdef TJCLAN_CPP

CLAN_MENU	Interface_clanmenu;




//auth.cpp에서 답변을 받아오는 변수 ( class에 넣으면 답이 이상하게 나옴)
ANSWERDATA ANSdata;

CLANDATA cldata;		//web 디비에서 받은 스트링을 파싱해서 넣어둔곳
CLANDATA chacldata[5];	//계정에 만들어진 캐릭터들이 가입한 클랜정보 저장




//=======================================출력 메세지======================start

//클랜 만들기 관련 메세지
CLANMSG clanMake1;
CLANMSG clanMake2;
CLANMSG clanMake3;
CLANMSG clanMake4;
CLANMSG clanMake5;
CLANMSG clanMake6;
CLANMSG clanMsg21;
CLANMSG clanMsg254;
CLANMSG clanMsg15;
CLANMSG clanJoin;
CLANMSG clanSecession;
CLANMSG clanServerConnectErrorMsg;
CLANMSG clanszError_Msg;
CLANMSG clanDeleteChaError;
CLANMSG clanCancelMakeClan;
CLANMSG clanClanNameError;
CLANMSG clanSecession6DownMsg;
CLANMSG clanClanJoinMsg;
CLANMSG clanClanJoinCancelMsg;
CLANMSG clanClanJoinsucesssMsg;
CLANMSG clanszReMoveClanWonMsg;
CLANMSG clanReMoveHeaderMsg;
CLANMSG clanszReConfReleaseMsg;
CLANMSG clanNoRecordReleaseMsg;
CLANMSG clanFinishReleaseMsg;
CLANMSG clanSecessionMsg;
CLANMSG clanFinishSecessionMsg;
CLANMSG clanisCheckClanJangErrorMsg;
CLANMSG clanMoneyErrorMsg;
CLANMSG clanAbilityErrorMsg;
CLANMSG clanszRemoveShortClanWonMsg;
CLANMSG clanNoName;
CLANMSG clanLoading;
CLANMSG clanMsg6;
CLANMSG clanMsg7;
CLANMSG clanClanWon6Down;
CLANMSG clanClanUserNoMakeMsg;
CLANMSG clanMakeFinishClanWon;
CLANMSG clanClanServerInspect;
CLANMSG clanClanUserRelease;
CLANMSG clanClanUserReleaseMsg;
CLANMSG clanClanMakedowninfo;
CLANMSG clanClanUserRemove;
CLANMSG clanClanWonOver;
CLANMSG clanNoLevel;
CLANMSG clanMakeFinishClan;
CLANMSG clanRemoveShowrtClanWonResultMsg;
CLANMSG clanRemoveSecond;
CLANMSG clanRemoveSecondResult;
CLANMSG LeaveClanConfirm;
CLANMSG LeaveClanNoMoney;
CLANMSG LeaveClanSuccess;
CLANMSG LeaveClanNoLevel;
CLANMSG LeaveClanReConfirm;
CLANMSG NoLeaveClan;
CLANMSG LeaveClanMessage;
CLANMSG LeaveClanDownLevel;
CLANMSG SubChipAppoint;
CLANMSG SubChipAppointEnd;
CLANMSG SubChipRelease;
CLANMSG SubChipReleaseEnd;
CLANMSG NoSubChip;

//호동Clan추가
CLANMSG ClanIs4DayCheckMsg;


char szMsg1[CLANMESSAGEBUF_MAX];
char szMsg2[CLANMESSAGEBUF_MAX];
char szMsg21[CLANMESSAGEBUF_MAX];
char szMsg3[CLANMESSAGEBUF_MAX];
char szMsg4[CLANMESSAGEBUF_MAX];
char szMsg5[CLANMESSAGEBUF_MAX];
char szClanMakeFinishMsg[CLANMESSAGEBUF_MAX];
char szMsg254[CLANMESSAGEBUF_MAX];
char szMsg15[CLANMESSAGEBUF_MAX];
char szMark1[CLANMESSAGEBUF_MAX];
char szMark2[CLANMESSAGEBUF_MAX];
char szMark3[CLANMESSAGEBUF_MAX];
char szJoin[CLANMESSAGEBUF_MAX];
char szSecession[CLANMESSAGEBUF_MAX];
char szMsg6[CLANMESSAGEBUF_MAX];
char szServerConnectErrorMsg[CLANMESSAGEBUF_MAX];
char szReadClanMarkErrorMsg[CLANMESSAGEBUF_MAX];
char szCancelMakeClan[CLANMESSAGEBUF_MAX];
char szTranzationError[CLANMESSAGEBUF_MAX];
char szInsertClanWonError1[CLANMESSAGEBUF_MAX];
char szClanNameError[CLANMESSAGEBUF_MAX];
char sz100Error[CLANMESSAGEBUF_MAX];
char szDefaultError[CLANMESSAGEBUF_MAX];
char szReleseError[CLANMESSAGEBUF_MAX];
char szSecessionSuccess[CLANMESSAGEBUF_MAX];
char szSecession6DownMsg[CLANMESSAGEBUF_MAX];
char szSecessionChipMsg[CLANMESSAGEBUF_MAX];
char szSecession2ErrorMsg[CLANMESSAGEBUF_MAX];
char szClanJoinMsg[CLANMESSAGEBUF_MAX];
char szClanJoinError2Msg[CLANMESSAGEBUF_MAX];
char szClanJoinError3Msg[CLANMESSAGEBUF_MAX];
char szClanJoinError4Msg[CLANMESSAGEBUF_MAX];
char szClanJoinCancelMsg[CLANMESSAGEBUF_MAX];
char szClanJoinsucesssMsg[CLANMESSAGEBUF_MAX];
char szReMoveClanWonMsg[CLANMESSAGEBUF_MAX];
char szReMoveHeaderMsg[CLANMESSAGEBUF_MAX];
char szReConfReleaseMsg[CLANMESSAGEBUF_MAX];

char szNoRecordReleaseMsg[CLANMESSAGEBUF_MAX];
char szFinishReleaseMsg[CLANMESSAGEBUF_MAX];
char szSecessionMsg[CLANMESSAGEBUF_MAX];
char szFinishSecessionMsg[CLANMESSAGEBUF_MAX];
char szisCheckClanJangErrorMsg[CLANMESSAGEBUF_MAX];
char szMoneyErrorMsg[CLANMESSAGEBUF_MAX];
char szAbilityErrorMsg[CLANMESSAGEBUF_MAX];
char szRemoveShortClanWonMsg[CLANMESSAGEBUF_MAX];
char szRemoveShowrtClanWonResultMsg[CLANMESSAGEBUF_MAX];
char szNoName[CLANMESSAGEBUF_MAX];
char szLoading[CLANMESSAGEBUF_MAX];
/////////캐릭터 삭제 메세지
char szDeleteChaReleaseBefore[CLANMESSAGEBUF_MAX];
char szDeleteChaReleaseFinish[CLANMESSAGEBUF_MAX];
char szDeleteChaSecessionBefore[CLANMESSAGEBUF_MAX];
char szDeleteChaSecessionFinish[CLANMESSAGEBUF_MAX];

char szDeleteChaError[CLANMESSAGEBUF_MAX];
char szMakeFinishClan[CLANMESSAGEBUF_MAX];
char szClanWon6Down[CLANMESSAGEBUF_MAX];
char szClanUserNoMakeMsg[CLANMESSAGEBUF_MAX];
char szMakeFinishClanWon[CLANMESSAGEBUF_MAX];
char ClanServerInspect[CLANMESSAGEBUF_MAX];
char szClanUserRelease[CLANMESSAGEBUF_MAX];
char Replay[CLANMESSAGEBUF_MAX];
char szClanMakedowninfo[CLANMESSAGEBUF_MAX];
char szClanUserReleaseMsg[CLANMESSAGEBUF_MAX];
char szClanUserRemove[CLANMESSAGEBUF_MAX];
char szClanWonOver[CLANMESSAGEBUF_MAX];
char szError_Msg[CLANMESSAGEBUF_MAX];
char szMsg7[CLANMESSAGEBUF_MAX];
char szNoLevel[CLANMESSAGEBUF_MAX];
//=======================================출력 메세지======================end



	int C_menuN;			//클랜의 메뉴는 여러가지가 있기때문에 번호를 셋팅해서 나눠준다.
	int C_menuN2;			//웹서버와연결시 쓰레드의 데이타를 기다리기위해 셋팅되는 첵크용번호임.
	int CLAN_ERROR_ret;		//에러메뉴보여준후 찾아갈곳 정한다.



	int  nClanWon_Num;			//클랜원파싱후 클랜원의 명수(클랜장제외)
	POINT BackStartPos;		//인터페이스 기본위치
	int g_HelpIconNumber;    //현재 스코롤 위치를 저장
	HELPCONTENTS HelpContents[100];
	int g_nClanIconNum;
	int g_HelpDisPlay;
	int g_HelpIConNumber;
	RECT menuPos[30];		//메뉴셋팅.
	int		menuMouseOver[30];
	int menuSu;
	CLANHELPICON ClanHelpIcon[CLANHELPICON_NUM];
	char szRemoveClanWonResult[5][64];
	int keydownEnt =0;
	int TJwheel =0;// 휠데이타
	int OpenFlag =0;
	char banisBuf1[64];
	char banisBuf2[512];
	int  banisCnt;

	DIBSectionBUF Dback;		//백버퍼용.
	int joinAnswer;
	BOOL bIsLoading = FALSE; 
	BOOL bOpenChatList = FALSE;
	BOOL bIsReadData = FALSE;
	int  ChatList_CmenuN = 0;
	int	ChatList_CmenuN2 = 0;

	int TJmb[3] = { 0,0,0 };
	int TJmb2[3] = { 0,0,0 };

	BOOL bCheckHostNameandPort = FALSE;
	MYCLANINFOHEADER MyClanInfo;

	CLANWONLIST ClanWonList;
	BOOL		bcldataclanmark = FALSE;






	int isDrawClanMenu=0;	//메뉴가 떠있으면 1

	char  szChatClanWon[CLANWON_MAX][30];        //클랜서버에서 보내준 클랜원을 저장(클랜장 제외)

#else	//=================================================================//




extern BOOL		bcldataclanmark;
extern CLANWONLIST ClanWonList;
extern MYCLANINFOHEADER MyClanInfo;

extern BOOL bCheckHostNameandPort;  //클랜 서버 아이피주소과 포트 상태를 체크
extern int TJmb[3] ;
extern int TJmb2[3];

extern int  ChatList_CmenuN;
extern int  ChatList_CmenuN2;
extern BOOL bIsReadData;
extern BOOL bOpenChatList;
extern BOOL bIsLoading; 
extern int joinAnswer;
extern DIBSectionBUF Dback;		//백버퍼용.


//extern HBITMAP membmp;




extern int TJwheel; //ktj : 휠데이타
extern int menuSu;
extern HWND			g_hwnd;
extern POINT		pCursorPos;
extern HINSTANCE	g_hInst;

extern CLAN_MENU	Interface_clanmenu;
extern ANSWERDATA	ANSdata;
extern int OpenFlag;



extern    int C_menuN;			//클랜의 메뉴는 여러가지가 있기때문에 번호를 셋팅해서 나눠준다.
extern 	int C_menuN2;			//웹서버와연결시 쓰레드의 데이타를 기다리기위해 셋팅되는 첵크용번호임.
extern 	int CLAN_ERROR_ret;		//에러메뉴보여준후 찾아갈곳 정한다.
extern int  banisF[100];
extern char banisBuf1[64];
extern char banisBuf2[512];
extern int  banisCnt;
extern CLANDATA cldata;		//web 디비에서 받은 스트링을 파싱해서 넣어둔곳
extern CLANDATA chacldata[5];	//계정에 만들어진 캐릭터들이 가입한 클랜정보 저장


extern int  nClanWon_Num;			//클랜원파싱후 클랜원의 명수(클랜장제외)
extern POINT BackStartPos;		//인터페이스 기본위치
extern int g_HelpIconNumber;    //현재 스코롤 위치를 저장
extern HELPCONTENTS HelpContents[100];
extern int g_nClanIconNum;
extern int g_HelpDisPlay;
extern int g_HelpIConNumber;
extern RECT menuPos[30];		//메뉴셋팅.
extern int		menuMouseOver[30];
extern int MouseButton[3] ;	//마우스버튼눌림첵크 : winmain.cpp에 정의함
extern CLANHELPICON ClanHelpIcon[CLANHELPICON_NUM];
extern char szRemoveClanWonResult[5][64];
#define MOUSE0() MouseButton[0]
extern int keydownEnt;



extern CLANMSG clanMake1;
extern CLANMSG clanMake2;
extern CLANMSG clanMake3;
extern CLANMSG clanMake4;
extern CLANMSG clanMake5;
extern CLANMSG clanMake6;


extern CLANMSG clanMsg21;
extern CLANMSG clanMsg254;
extern CLANMSG clanMsg15;
extern CLANMSG clanJoin;
extern CLANMSG clanSecession;
extern CLANMSG clanServerConnectErrorMsg;
extern CLANMSG clanszError_Msg;
extern CLANMSG clanDeleteChaError;
extern CLANMSG clanCancelMakeClan;
extern CLANMSG clanClanNameError;
extern CLANMSG clanSecession6DownMsg;
extern CLANMSG clanClanJoinMsg;
extern CLANMSG clanClanJoinCancelMsg;
extern CLANMSG clanClanJoinsucesssMsg;
extern CLANMSG clanszReMoveClanWonMsg;
extern CLANMSG clanReMoveHeaderMsg;
extern CLANMSG clanszReConfReleaseMsg;
extern CLANMSG clanNoRecordReleaseMsg;
extern CLANMSG clanFinishReleaseMsg;
extern CLANMSG clanSecessionMsg;
extern CLANMSG clanFinishSecessionMsg;
extern CLANMSG clanisCheckClanJangErrorMsg;
extern CLANMSG clanMoneyErrorMsg;
extern CLANMSG clanAbilityErrorMsg;
extern CLANMSG clanszRemoveShortClanWonMsg;
extern CLANMSG clanNoName;
extern CLANMSG clanLoading;
extern CLANMSG clanMsg6;
extern CLANMSG clanMsg7;
extern CLANMSG clanClanWon6Down;
extern CLANMSG clanClanUserNoMakeMsg;
extern CLANMSG clanMakeFinishClanWon;
extern CLANMSG clanClanServerInspect;
extern CLANMSG clanClanUserRelease;
extern CLANMSG clanClanUserReleaseMsg;
extern CLANMSG clanClanMakedowninfo;
extern CLANMSG clanClanUserRemove;
extern CLANMSG clanClanWonOver;
extern CLANMSG clanNoLevel;
extern CLANMSG clanMakeFinishClan;
extern CLANMSG clanRemoveShowrtClanWonResultMsg;
extern CLANMSG clanRemoveSecond;
extern CLANMSG clanRemoveSecondResult;
extern CLANMSG LeaveClanConfirm;
extern CLANMSG LeaveClanNoMoney;
extern CLANMSG LeaveClanSuccess;
extern CLANMSG LeaveClanNoLevel;
extern CLANMSG LeaveClanReConfirm;
extern CLANMSG NoLeaveClan;
extern CLANMSG LeaveClanMessage;
extern CLANMSG LeaveClanDownLevel;
extern CLANMSG SubChipAppoint;
extern CLANMSG SubChipAppointEnd;
extern CLANMSG SubChipRelease;
extern CLANMSG SubChipReleaseEnd;
extern CLANMSG NoSubChip;

////호동Clan추가
extern CLANMSG ClanIs4DayCheckMsg;


extern char szMsg1[CLANMESSAGEBUF_MAX];
extern char szMsg2[CLANMESSAGEBUF_MAX];
extern char szMsg21[CLANMESSAGEBUF_MAX];
extern char szMsg3[CLANMESSAGEBUF_MAX];
extern char szMsg4[CLANMESSAGEBUF_MAX];
extern char szMsg5[CLANMESSAGEBUF_MAX]; 
extern char szMsg6[CLANMESSAGEBUF_MAX];
extern char sz100Error[CLANMESSAGEBUF_MAX];
extern char szAbilityErrorMsg[CLANMESSAGEBUF_MAX];
extern char szCancelMakeClan[CLANMESSAGEBUF_MAX];
extern char szMsg254[CLANMESSAGEBUF_MAX];
extern char szClanMakeFinishMsg[CLANMESSAGEBUF_MAX];
extern char szMsg15[CLANMESSAGEBUF_MAX]; 
extern char szMark1[CLANMESSAGEBUF_MAX]; 
extern char szMark2[CLANMESSAGEBUF_MAX]; 
extern char szMark3[CLANMESSAGEBUF_MAX]; 
extern char szJoin[CLANMESSAGEBUF_MAX];
extern char szSecession[CLANMESSAGEBUF_MAX]; 
extern char szServerConnectErrorMsg[CLANMESSAGEBUF_MAX]; 
extern char szReadClanMarkErrorMsg[CLANMESSAGEBUF_MAX];
extern char szTranzationError[CLANMESSAGEBUF_MAX];
extern char szInsertClanWonError1[CLANMESSAGEBUF_MAX];
extern char szClanNameError[CLANMESSAGEBUF_MAX];
extern char szDefaultError[CLANMESSAGEBUF_MAX];
extern char szReleseError[CLANMESSAGEBUF_MAX];
extern char szSecessionSuccess[CLANMESSAGEBUF_MAX];
extern char szSecession6DownMsg[CLANMESSAGEBUF_MAX];
extern char szSecessionChipMsg[CLANMESSAGEBUF_MAX];
extern char szSecession2ErrorMsg[CLANMESSAGEBUF_MAX];
extern char szClanJoinMsg[CLANMESSAGEBUF_MAX];
extern char szClanJoinError2Msg[CLANMESSAGEBUF_MAX]; 
extern char szClanJoinError3Msg[CLANMESSAGEBUF_MAX];
extern char szClanJoinError4Msg[CLANMESSAGEBUF_MAX];
extern char szClanJoinCancelMsg[CLANMESSAGEBUF_MAX];
extern char szClanJoinsucesssMsg[CLANMESSAGEBUF_MAX];
extern char szReMoveClanWonMsg[CLANMESSAGEBUF_MAX];
extern char szReMoveHeaderMsg[CLANMESSAGEBUF_MAX];
extern char szReConfReleaseMsg[CLANMESSAGEBUF_MAX];
extern char szNoRecordReleaseMsg[CLANMESSAGEBUF_MAX];
extern char szFinishReleaseMsg[CLANMESSAGEBUF_MAX];
extern char szSecessionMsg[CLANMESSAGEBUF_MAX];
extern char szFinishSecessionMsg[CLANMESSAGEBUF_MAX];
extern char szisCheckClanJangErrorMsg[CLANMESSAGEBUF_MAX];
extern char szMoneyErrorMsg[CLANMESSAGEBUF_MAX];
extern char szRemoveShortClanWonMsg[CLANMESSAGEBUF_MAX];
extern char szRemoveShowrtClanWonResultMsg[CLANMESSAGEBUF_MAX];
extern char szClanUserNoMakeMsg[CLANMESSAGEBUF_MAX];
extern char szNoName[CLANMESSAGEBUF_MAX];
extern char szLoading[CLANMESSAGEBUF_MAX];

extern char szDeleteChaReleaseBefore[CLANMESSAGEBUF_MAX];
extern char szDeleteChaReleaseFinish[CLANMESSAGEBUF_MAX];
extern char szDeleteChaSecessionBefore[CLANMESSAGEBUF_MAX];
extern char szDeleteChaSecessionFinish[CLANMESSAGEBUF_MAX];

extern char szDeleteChaError[CLANMESSAGEBUF_MAX];
extern char szMakeFinishClan[CLANMESSAGEBUF_MAX];
extern char szClanWon6Down[CLANMESSAGEBUF_MAX];
extern char szMakeFinishClanWon[CLANMESSAGEBUF_MAX];
extern char ClanServerInspect[CLANMESSAGEBUF_MAX];
extern char szClanUserRelease[CLANMESSAGEBUF_MAX]; 
extern char Replay[CLANMESSAGEBUF_MAX];
extern char szClanMakedowninfo[CLANMESSAGEBUF_MAX]; 
extern char szClanUserReleaseMsg[CLANMESSAGEBUF_MAX];
extern char szClanUserRemove[CLANMESSAGEBUF_MAX];
extern char szClanWonOver[CLANMESSAGEBUF_MAX];
extern char szError_Msg[CLANMESSAGEBUF_MAX];

extern char szMsg7[CLANMESSAGEBUF_MAX];
extern char szNoLevel[CLANMESSAGEBUF_MAX];

extern int ClanCharMove;	//메뉴판안의 마우스의 움직임에 캐릭터가 영향없도록 한다.
extern int ClanCLCharMove;//




extern int isDrawClanMenu;	//메뉴가 떠있으면 1



extern int ktj_imsiDRAWinfo ;



extern char  szChatClanWon[CLANWON_MAX][30];        //클랜서버에서 보내준 클랜원을 저장(클랜장 제외)


#endif


void ClanJoin(char *oneClanName, char *oneChipCharid);
void menu_joinOpen_Chip(int isOK, char *clanWonID, char *clanWonCharID);
void menu_joinOpen_Chip(int isOK, char *clanWonID, char *clanWonCharID, int clanWonJob, int clanWonLevel);

//호동Clan추가
void g_IsCheckClanMember(smCHAR *pChar);

//cldata 없데이트용
void Updatecldata();



//클랜마크 위치 받기
int ReadClanInfo(DWORD dwClanNum);
int ReadClanInfo_32X32(DWORD dwClanNum);
//클랜마크의 사용법 
//받은번호 = ReadClanInfo(100189);
//if(받은번호 == -1) 현재 다운받을수없음.
//if(ClanInfo[ 받은번호 ].nStatus == 2) {
//			사용가능함
//}


//web DB의 포트와 ip셋팅시 사용함(캐릭터 선택화면 전에 넣을것)
void web_DB_ip_port_init(char *ip0, int port0, char *ip1, int port1);
void web_DB_ip_port_init(char *ip0, int port0, char *ip1, int port1,int ticket);



//ktj :  1. 캐릭 지우기전에 셋팅하는 함수
void CLANmenu_DeleteCha_SetCharacter(char *id, char *gserver, char *chaname);


//HWND hFocusWnd를 사용하고 있는지의 여부를 알아냄. 0:안사용 1:사용
extern int usehFocus;



//스크롤에 휠키사용여부
int TJBscrollWheel(int wheel);		//사용중:1  미사용중 0


//채팅리스트스피커의 플래그를 알아낸다.
int chatlistSPEAKERflag();
void chatlistSPEAKERflagChg(int flag);//0,1을 넣을수있음.







//=============겜들어가기직전에 경고 메세지=====================
//1. 셋팅함수
int GetNoticeUrl(char *url);








#ifdef TJCLAN_CPP


#ifndef USE_PROSTONTALE		//=============윈도에서만 사용함


extern HWND    g_hwnd;
/*///////////////////////////////////////////////////////////////////////////
: 함수  명 :  HBITMAP MakeDIBSection(char *Path)
: 하는  일 :  
: 인    자 : bmp파일을읽어서  DIB 섹션을 만들어 핸들을 리턴한다.
: 반환  값 :  
///////////////////////////////////////////////////////////////////////////*/
HBITMAP MakeDIBSection(DIBSectionBUF *Dibbuf)
{
	HANDLE hFile;
	DWORD FileSize, dwRead;
	BITMAPFILEHEADER fh;
	HBITMAP hBit;
	BITMAPINFO *ih;
	PVOID pRaster;

	// 파일을 연다
	hFile=CreateFile(Dibbuf->fname,GENERIC_READ,0,NULL,
		OPEN_EXISTING,FILE_ATTRIBUTE_NORMAL,NULL);
	if (hFile==INVALID_HANDLE_VALUE) {
		return NULL;
	}

	// 파일 헤더와 정보 구조체(색상 테이블 포함)를 읽어들인다.
	ReadFile(hFile,&fh,sizeof(BITMAPFILEHEADER),&dwRead,NULL);
	FileSize=fh.bfOffBits-sizeof(BITMAPFILEHEADER);
	ih=(BITMAPINFO *)malloc(FileSize);
	ReadFile(hFile,ih,FileSize,&dwRead,NULL);

	//ih->bmiHeader.biHeight =  -ih->bmiHeader.biHeight;
	// DIB 섹션을 만들고 버퍼 메모리를 할당한다.
	hBit = CreateDIBSection(NULL,ih,DIB_RGB_COLORS,&pRaster,NULL,0);

	if(Dibbuf != NULL) {				//정보를 넘겨준다.
		Dibbuf->hb = hBit;
		Dibbuf->pixBuf = (void *)pRaster;
	}


	// 래스터 데이터를 읽어들인다.
	ReadFile(hFile,pRaster,fh.bfSize-fh.bfOffBits,&dwRead,NULL);
	free(ih);
	CloseHandle(hFile);
	
	return hBit;
}

void window_renderINIT()				//한번
{
	static cnt=0;
	if(cnt != 0) return;
	cnt = 1;

	Dback.hb = 0;
	Dback.pixBuf = 0;
	wsprintf(Dback.fname, "stg0.bmp" );		//임시
	MakeDIBSection(&Dback);			//dib Section
}

void window_renderFLIP()					//한번
{
	HDC hdc, sorDC;
	hdc = GetDC(g_hwnd);
	sorDC =CreateCompatibleDC(hdc );
	SelectObject(sorDC , Dback.hb );

	//플리핑.
	BitBlt(hdc, 0,0,
			800, 600,
			sorDC,0,0,SRCCOPY);

	DeleteDC(sorDC);

	ReleaseDC( g_hwnd, hdc);


	//if(Dback.hb) {
	//	DeleteObject( Dback.hb );
	//}
}
#endif


#endif


void CheckMsgPos(CLANMSG* clanmsg,int width, int height);
void DebugMessage(char* msg,...);
void InitHelp();
void GoNotice();
int GetNoticeUrl(char *url);

extern int GoNotice_chking;

