
#define CE_CJOIN_CPP
#include <windows.h>
#include <stdio.h>
#include <tchar.h>
#include <math.h>

#include "tjclanDEF.h"

#ifdef USE_PROSTONTALE		//=========== 프리스턴소스인경우

#include "..\\..\\sinbaram\\SinLinkHeader.h"
#include "..\\..\\HoBaram\\HoLinkHeader.h"
#include "..\\..\\language.h"
#include "..\\..\\HoBaram\\HoTextFile.h"
#endif

#include "tjclan.h"
#include "cE_report.h"
#include "cE_CJoin.h"
#include "cE_CViewClanInfo.h"
#include "..\\isaocheck\\auth.h"
#include "..\\isaocheck\\isaocheck.h"

//void CheckMsgPos(CLANMSG* clanmsg,int width, int height);

CLANEntity_JOIN::CLANEntity_JOIN()
{
	nJoinAnswer = 0;
	ZeroMemory(szTemp,sizeof(szTemp));
	ZeroMemory(szTemp1,sizeof(szTemp1));
}

CLANEntity_JOIN::~CLANEntity_JOIN()
{
}

int CLANEntity_JOIN::Init(int Life)
{
	life = Life;
	ZeroMemory(szUserJoin,sizeof(szUserJoin));
	return 1;
}

int CLANEntity_JOIN::Clear()
{
	return 1;
}

void CLANEntity_JOIN::Main()
{
	int i;
	//char		szErrorMsg[256];
	switch(C_menuN)
	{
	case CLAN_CLANWONOVER:
		i = chkeckMenuN();
		switch(i) {
		case 0:
			menuMouseOver[0] = 1;
			break;
		default:
			memset(menuMouseOver,-1,sizeof(int)*30);
			break;
		}
		if(MOUSE0())			
		{	
			MOUSE0()=0;			
			switch(i) 
			{
			case 0:
				C_menuN = CLAN_AllEnd;
				//C_menuN = CLAN_ERROR_ret;		//나가기 : 끝내는걸로간다.
				//menuInit(C_menuN );

				C_menuN2 =-1;
				CLAN_ERROR_ret=-1;
				break;	
			default:
				break;

			}
		}
		break;		
	case CLAN_JOIN_before:

		switch(C_menuN2) {
		case 0:
			ANSdata.ret_val= -1;
			
			WebDB.Resume() ;		//웹db를 재개한다.
			WebDB.isClanMember(cldata.szID,cldata.gserver,cldata.ChaName);		//이 id가 클랜원인지를 파악한다.
			C_menuN2=1;
#ifdef CLAN_DEBUG
			DebugMessage("CLAN_JOIN_before\277\241\274\255 WebDB.isClanMember(\276\306\300\314\265\360 : %s,\260\324\300\323\274\255\271\366 : %s,\304\263\270\257\305\315\300\314\270\247 : %s)",cldata.szID,cldata.gserver,cldata.ChaName);
#endif
			break;
		case 1:
			switch(ANSdata.ret_val) 
			{
			case 100:						//일반 클랜원
				
				WebDB.Suspend(); //웹db 중지

				C_menuN = CLAN_JOIN_After;
				menuInit(C_menuN );
				joinAnswer = 100;			//이미다른클랜에가입되있어 가입불가

				C_menuN2=0;
#ifdef CLAN_DEBUG
			DebugMessage("CLAN_JOIN_before\277\241\274\255 \270\256\305\317\304\332\265\345 100");
#endif
				break;

			case 1:                         //클랜칩
			case 2:                         //클랜장
			case 5:						//부클랜
			case 101:						//클랜칲( 데이타읽기로 간다)
			case 103:						//클랜칲 심사전
			case 104:						//클랜칲 심사전
				WebDB.Suspend();
				C_menuN = CLAN_JOIN_After;		
				menuInit(C_menuN );
				joinAnswer = 101;			//이미다른클랜에 클랜칲이라서 가입불가

				C_menuN2=0;
#ifdef CLAN_DEBUG
			DebugMessage("CLAN_JOIN_before\277\241\274\255 \305\254\267\243 \260\241\300\324\272\322\260\241(%d)",ANSdata.ret_val);
#endif

				break;
			case 4:
			case 3:
			case 0:						//무등록.
			case 105:						//에러
				WebDB.Suspend(); 
				C_menuN = CLAN_JOIN;
				//C_menuN = CLANUSER_JOIN;
				menuInit(C_menuN );

				C_menuN2=0;
#ifdef CLAN_DEBUG
			DebugMessage("CLAN_JOIN_before\277\241\274\255 \305\254\267\243\260\241\300\324\260\241\264\311(%d)",ANSdata.ret_val);
#endif

				break;
			case ISAO_ERR_TCP_CONNECT:
				WebDB.Suspend();
				
				C_menuN = CLAN_ERROR;
				wsprintf(szErrorMsg,"%s (55)", clanszError_Msg.szMsg);
				cClanReport.InitMsg(szErrorMsg);		
				cClanReport.menuInit(C_menuN,clanszError_Msg.pos.x,clanszError_Msg.pos.y);
#ifdef CLAN_DEBUG
			DebugMessage("CLAN_JOIN_before\277\241\274\255 \274\255\271\366\277\241 \301\242\274\323\307\322\274\366 \276\370\300\275(%d)",ANSdata.ret_val);
#endif
				break;
			default:
				if(ANSdata.ret_val != -1) {		//전혀엉뚱한 답이 와서 에러가 났다.
					WebDB.Suspend();

					C_menuN = CLAN_ERROR;				//클랜결성못하니 돌아가라고 하는메뉴로 간다.
					wsprintf(szErrorMsg,"%s (56)", clanszError_Msg.szMsg);
					cClanReport.InitMsg(szErrorMsg);		
					cClanReport.menuInit(C_menuN,clanszError_Msg.pos.x,clanszError_Msg.pos.y);

					C_menuN2=0;
#ifdef CLAN_DEBUG
			DebugMessage("CLAN_JOIN_before\277\241\274\255 \265\360\306\372\306\256\277\241\267\257(%d)",ANSdata.ret_val);
#endif
				}
				break;
			}
			break;
		}
		break;

	case CLANUSER_JOIN:

		i = chkeckMenuN();
		switch(i) {
		case 0:
			menuMouseOver[0] = 1;
			break;
		default:
			memset(menuMouseOver,-1,sizeof(int)*30);
			break;
		}
		if(MOUSE0())			
		{	
			MOUSE0()=0;
			i = i;
			switch(i) 
			{
			case 0:
				C_menuN = CLAN_AllEnd;
				//C_menuN = CLAN_ERROR_ret;		//나가기 : 끝내는걸로간다.
				//menuInit(C_menuN );

				C_menuN2 =-1;
				CLAN_ERROR_ret=-1;
				break;	
			default:
				i = i;
				break;

			}
		}
		break;
		

	case CLAN_JOIN:
		i = chkeckMenuN();				//================================================
		memset(menuMouseOver,-1,sizeof(int)*30);
		switch(i) {
		case 0:		//yes
			menuMouseOver[0] = 1;
			break;
		case 1:		//no
			menuMouseOver[1] = 1;
			break;
		default:
			memset(menuMouseOver,-1,sizeof(int)*30);
			break;
		}
		if(MOUSE0())
		{	
			MOUSE0()=0;
			switch(i) 
			{
			case 0:		
				
				wsprintf(szUserJoin, clanClanJoinMsg.szMsg,cldata.szJoinClanName,cldata.szJoinClanChipName,cldata.ChaName);				
				//void CheckMsgPos(CLANMSG* clanmsg);
				CheckMsgPos(&clanClanJoinMsg,BOX_MAX_WIDTH,0);								
				C_menuN = CLANUSER_JOIN;
				
				menuInit(C_menuN);
				CLAN_ERROR_ret=CLAN_JOIN_After;		//에러메뉴보여준후 다시 돌아갈곳.
				nJoinAnswer = 1;

#ifdef USE_PROSTONTALE		//=========== 프리스턴소스인경우

				SendJoinClan();						//클랜 가입 승인 메세지 보내기 ( 장순목 )
#endif

				break;
			case 1:
				nJoinAnswer = 0;
				C_menuN = CLAN_AllEnd;			//N : 클랜에 가입안한다.
				break;

			default:
				break;
			}
		}
		break;

	case CLAN_JOIN_After:						//일단 끝낸다.
		C_menuN = CLAN_AllEnd;
		break;

	//호동Clan추가
	case CLAN_IS_CHECK_MEMBER_END:
		C_menuN = CLAN_AllEnd;
		break;

	//(클랜을 탈퇴한지 4일이 되었는지 검사)
	case CLAN_IS_CHECK_MEMBER:
		switch(C_menuN2)
		{
		case 0:
			ANSdata.ret_val = -1;
			WebDB.Resume();
			WebDB.isCheckClanMember(cldata.gserver, cldata.TargetChar->smCharInfo.szName);
			C_menuN2 = 1;
			break;
		case 1:
			if(ANSdata.ret_val != -1)
			{
				//클랜 가입을 시켜도 되는 캐릭터
       			if(ANSdata.ret_val == 0)
				{
					WebDB.Suspend();
					//에러 출력
					//A유저는 클랜 탈퇴 후 4일이 경과 되지 않아 클랜원 등록을 할수 없습니다.
					CLANMSG clanMsg;
					wsprintf(clanMsg.szMsg, "%s", "Error");
					clanMsg.pos.x = 0;
					clanMsg.pos.y = 0;
					CheckMsgPos(&clanMsg,BOX_MAX_WIDTH,0);
		
					C_menuN = CLAN_ERROR;
					
					char TempBufferMsg[256];
					wsprintf(TempBufferMsg, ClanIs4DayCheckMsg.szMsg, cldata.TargetChar->smCharInfo.szName); 
					cClanReport.InitMsg(TempBufferMsg);
					cClanReport.menuInit(C_menuN,clanszError_Msg.pos.x,clanszError_Msg.pos.y);
					
					CLAN_ERROR_ret=CLAN_IS_CHECK_MEMBER_END;		//에러메뉴보여준후 다시 돌아갈곳.

				}
				else if(ANSdata.ret_val == 1)
				{
 					WebDB.Suspend();
					SendClanJoinService( 1 , cldata.TargetChar );
					
					C_menuN = CLAN_IS_CHECK_MEMBER_END;
					C_menuN2= 0;
					OpenFlag = 1;
					joinAnswer = 0;
					ClanCharMove = 1;

				}
				else
				{
 					WebDB.Suspend();
					//에러 출력
					//A유저는 클랜 탈퇴 후 4일이 경과 되지 않아 클랜원 등록을 할수 없습니다.
					CLANMSG clanMsg;
					wsprintf(clanMsg.szMsg, "%s", "Error");
					clanMsg.pos.x = 0;
					clanMsg.pos.y = 0;
					CheckMsgPos(&clanMsg,BOX_MAX_WIDTH,0);
		
					C_menuN = CLAN_ERROR;
					
					char TempBufferMsg[256];
					//wsprintf(TempBufferMsg, ClanIs4DayCheckMsg.szMsg, cldata.TargetChar->smCharInfo.szName); 
					#ifdef	_LANGUAGE_JAPANESE
					wsprintf(TempBufferMsg, "\203N\203\211\203\223\222E\221\336\n4\223\372\214o\211\337\202\314\203`\203F\203b\203N\222\206Error");  // クラン脱退\n4日経過のチェック中Error
					#else
   					wsprintf(TempBufferMsg, "\305\254\267\243\305\273\305\360\n4\300\317\260\346\260\372\303\274\305\251\301\337Error");
					#endif
					cClanReport.InitMsg(TempBufferMsg);
					cClanReport.menuInit(C_menuN,clanszError_Msg.pos.x,clanszError_Msg.pos.y);
					
					CLAN_ERROR_ret=CLAN_IS_CHECK_MEMBER_END;		//에러메뉴보여준후 다시 돌아갈곳.
				}
			}
		}
		break;
										//================================================
	case CLAN_JOIN_chip:				//가입하겟다는 사람은 클랜에 등록시킨다.
		switch(C_menuN2) {				//================================================
		case 0:
			ANSdata.ret_val= -1;
			
			WebDB.Resume() ;		//웹db를 재개한다.//클랜원으로 가입시킴
#ifdef USE_PROSTONTALE		//=========== 프리스턴소스인경우
					WebDB.joinClanMember(cldata.szID,cldata.gserver,cldata.ChaName,
				//클랜이름	//클랜원으로 등록할사람의 캘릭터이름, 클랜원으로 등록할사람id
				cldata.name, cldata.clanJoinCharID,  cldata.clanJoinID, sinChar->Level, cldata.clanJoinJob ,cldata.clanJoinLevel,cldata.isSubChip);//타임
#ifdef CLAN_DEBUG
			DebugMessage("CLAN_JOIN_chip\277\241\274\255 WebDB.joinClanMember(\276\306\300\314\265\360 : %s,\260\324\300\323\274\255\271\366 : %s,\305\254\267\243\300\345 : %s,\305\254\267\243\300\314\270\247 : %s, \260\241\300\324\307\322 \304\263\270\257\305\315 : %s,  \260\241\300\324\307\322 \276\306\300\314\265\360 : %s, \267\271\272\247 : %d, \300\342 : %d" ,cldata.szID,cldata.gserver,cldata.ChaName,cldata.name, cldata.clanJoinCharID,  cldata.clanJoinID, sinChar->Level, sinChar->JOB_CODE );
#endif

#else
					WebDB.joinClanMember(cldata.szID,cldata.gserver,cldata.ChaName,
				//클랜이름	//클랜원으로 등록할사람의 캘릭터이름, 클랜원으로 등록할사람id
				cldata.name, cldata.clanJoinCharID,  cldata.clanJoinID,70  ,1,50,cldata.isSubChip);
#endif

	


			C_menuN2=1;

			break;
		case 1:
			if(ANSdata.ret_val != -1) {
				ANSdata.ret_val = ANSdata.ret_val;

			}

			switch(ANSdata.ret_val) 
			{
				//ygy
			case 0: //트랜잭션 오류
				WebDB.Suspend();

				//wsprintf(szErrorMsg, szTranzationError);
				C_menuN = CLAN_ERROR;
				menuInit(C_menuN);
				wsprintf(szErrorMsg,"%s (64)",clanszError_Msg.szMsg);
				cClanReport.InitMsg(szErrorMsg);		
				cClanReport.menuInit(C_menuN,clanszError_Msg.pos.x,clanszError_Msg.pos.y);
				CLAN_ERROR_ret=CLAN_JOIN_After;		//에러메뉴보여준후 다시 돌아갈곳.

				C_menuN2=0;
#ifdef CLAN_DEBUG
			DebugMessage("CLAN_JOIN_chip\277\241\274\255 \306\256\267\243\300\350\274\307\277\300\267\371");
#endif
				break;
			case 1: //가입 성공
				WebDB.Suspend(); //웹db 중지
				C_menuN = CLAN_JOIN_chip_YES;
				menuInit(C_menuN );
				
				AddClanWon();

				void SetClanWon();
				//SetClanWon();


				C_menuN2=0;
#ifdef CLAN_DEBUG
			DebugMessage("CLAN_JOIN_chip\277\241\274\255 \260\241\300\324\274\272\260\370");
#endif
				break;
			case 2: //현재 가입시키려는 클랜원이 이미 다른 클랜에 가입해있을 경우(자기 클랜포함)
				WebDB.Suspend();


				
				C_menuN = CLAN_ERROR;				
				wsprintf(szErrorMsg,"%s (49)",clanszError_Msg.szMsg);
				cClanReport.InitMsg(szErrorMsg);
				cClanReport.menuInit(C_menuN,clanszError_Msg.pos.x,clanszError_Msg.pos.y);
				CLAN_ERROR_ret=CLAN_JOIN_After;		//에러메뉴보여준후 다시 돌아갈곳.

				C_menuN2=0;
#ifdef CLAN_DEBUG
			DebugMessage("CLAN_JOIN_chip\277\241\274\255 \307\366\300\347 \260\241\300\324\275\303\305\260\267\301\264\302 \305\254\267\243\277\370\300\314 \300\314\271\314 \264\331\270\245 \305\254\267\243\277\241 \260\241\300\324\307\330\300\326\300\273 \260\346\277\354(\300\332\261\342 \305\254\267\243\306\367\307\324)");
#endif
				break;
			case 3://클랜이 존재하지 않거나 클랜 장이 아닌경우
				WebDB.Suspend();

				
				C_menuN = CLAN_ERROR;
				wsprintf(szErrorMsg,"%s (50)",clanszError_Msg.szMsg);
				cClanReport.InitMsg(szErrorMsg);		
				cClanReport.menuInit(C_menuN,clanszError_Msg.pos.x,clanszError_Msg.pos.y);
				CLAN_ERROR_ret=CLAN_JOIN_After;		//에러메뉴보여준후 다시 돌아갈곳.

				C_menuN2=0;
#ifdef CLAN_DEBUG
			DebugMessage("CLAN_JOIN_chip\277\241\274\255 \305\254\267\243\300\314 \301\270\300\347\307\317\301\366 \276\312\260\305\263\252 \305\254\267\243 \300\345\300\314 \276\306\264\321\260\346\277\354");
#endif
				break;
			case 4://클랜 멤버 오버플로우 
				WebDB.Suspend();
				C_menuN = CLAN_CLANWONOVER;
				cClanJoin.menuInit(C_menuN);
				
				//C_menuN = CLAN_ERROR;
				//wsprintf(szErrorMsg,"%s (51)",clanszError_Msg.szMsg);
				//cClanReport.InitMsg(szErrorMsg);		
				//cClanReport.menuInit(C_menuN,clanszError_Msg.pos.x,clanszError_Msg.pos.y);
				CLAN_ERROR_ret=CLAN_JOIN_After;		//에러메뉴보여준후 다시 돌아갈곳.

				C_menuN2=0;
#ifdef CLAN_DEBUG
			DebugMessage("CLAN_JOIN_chip\277\241\274\255 \305\254\267\243 \270\342\271\366 \277\300\271\366\307\303\267\316\277\354");
#endif
				break;
			case 100:
				WebDB.Suspend();
				C_menuN = CLAN_ERROR;
				wsprintf(szErrorMsg,"%s (52)",clanszError_Msg.szMsg);
				cClanReport.InitMsg(szErrorMsg);						
				cClanReport.menuInit(C_menuN,clanszError_Msg.pos.x,clanszError_Msg.pos.y);
				CLAN_ERROR_ret=CLAN_JOIN_After;		//에러메뉴보여준후 다시 돌아갈곳.

				C_menuN2=0;
#ifdef CLAN_DEBUG
			DebugMessage("CLAN_JOIN_chip\277\241\274\255 \270\256\305\317\304\332\265\345 100");
#endif

				break;
			case ISAO_ERR_TCP_CONNECT:
				WebDB.Suspend();
				C_menuN = CLAN_ERROR;
				wsprintf(szErrorMsg,"%s (53)",clanszError_Msg.szMsg);
				cClanReport.InitMsg(szErrorMsg);		
				cClanReport.menuInit(C_menuN,clanszError_Msg.pos.x,clanszError_Msg.pos.y);
#ifdef CLAN_DEBUG
			DebugMessage("CLAN_JOIN_chip\277\241\274\255 \274\255\271\366\277\241 \301\242\274\323 \276\310\265\312");
#endif
				break;
				//end ygy

			
			default:
				if(ANSdata.ret_val != -1) {		//전혀엉뚱한 답이 와서 에러가 났다.
					WebDB.Suspend();

					
					C_menuN = CLAN_ERROR;
					wsprintf(szErrorMsg,"%s (54)",clanszError_Msg.szMsg);
					cClanReport.InitMsg(szErrorMsg);		
					cClanReport.menuInit(C_menuN,clanszError_Msg.pos.x,clanszError_Msg.pos.y);
					CLAN_ERROR_ret=CLAN_JOIN_After;		//에러메뉴보여준후 다시 돌아갈곳.

					C_menuN2=0;
#ifdef CLAN_DEBUG
			DebugMessage("CLAN_JOIN_chip\277\241\274\255 \265\360\306\372\306\256\277\241\267\257");
#endif
				}
				break;
			}
			break;
		}
		break;

	case CLAN_JOIN_chip_NO:				//가입하지않겟다고함.		
		wsprintf(szTemp, clanClanJoinCancelMsg.szMsg, cldata.clanJoinCharID);
		
		

		strcpy(szTemp1,clanszError_Msg.szMsg);
		strcpy(clanszError_Msg.szMsg,szTemp);

		//void CheckMsgPos(CLANMSG* clanmsg);
		CheckMsgPos(&clanszError_Msg,BOX_MAX_WIDTH,0);
		
		C_menuN = CLAN_ERROR;
		cClanReport.InitMsg(szTemp);		
		cClanReport.menuInit(C_menuN,clanszError_Msg.pos.x,clanszError_Msg.pos.y);

		strcpy(clanszError_Msg.szMsg,szTemp1);

		CLAN_ERROR_ret=CLAN_AllEnd;		//에러메뉴보여준후 다시 돌아갈곳.
		break;
	case CLAN_JOIN_chip_YES:				//가입한다함.  //권유당한 캐릭아디
		wsprintf(szTemp, clanClanJoinsucesssMsg.szMsg, cldata.clanJoinCharID);

		strcpy(szTemp1,clanszError_Msg.szMsg);
		strcpy(clanszError_Msg.szMsg,szTemp);
		
		//void CheckMsgPos(CLANMSG* clanmsg);
		CheckMsgPos(&clanszError_Msg,BOX_MAX_WIDTH,0);

		strcpy(szTemp,clanszError_Msg.szMsg);

		C_menuN = CLAN_ERROR;
		cClanReport.InitMsg(szTemp);		
		cClanReport.menuInit(C_menuN,clanszError_Msg.pos.x,clanszError_Msg.pos.y);
		strcpy(clanszError_Msg.szMsg,szTemp1);
		CLAN_ERROR_ret=CLAN_AllEnd;		//에러메뉴보여준후 다시 돌아갈곳.




		void SetClanWon();
		SetClanWon();

		break;

	}
}

void CLANEntity_JOIN::menuInit(int num)
{
	int bx =0, by=0;
	switch(num){		
		case CLAN_CLANWONOVER:
			menuSu = 3;
			#ifdef USE_PROSTONTALE		//=========== 포리스턴소스인경우
			bx = 32;
			by = 16;
			#else
			bx = 32;
			by = 16;
			#endif
			menuPos[0].left = BackStartPos.x + BUTTON1_X;		//확인
			menuPos[0].top = BackStartPos.y + BUTTON1_Y;
			menuPos[0].right = menuPos[0].left + bx;
			menuPos[0].bottom = menuPos[0].top + by;

			//텍스트 출력 위치
			menuPos[1].left = BackStartPos.x + clanClanWonOver.pos.x;
			menuPos[1].top = BackStartPos.y + clanClanWonOver.pos.y;

					#ifdef USE_PROSTONTALE		//=========== 포리스턴소스인경우
			bx = 64;
			by = 16;
			#else
			bx = 64;
			by = 16;
			#endif
			menuPos[2].left = BackStartPos.x + BUTTON1_BOX_X;		//확인
			menuPos[2].top = BackStartPos.y + BUTTON1_BOX_Y;
			menuPos[2].right = menuPos[2].left + bx;
			menuPos[2].bottom = menuPos[2].top + by;
			break;
		case CLANUSER_JOIN:
			menuSu = 3;
			#ifdef USE_PROSTONTALE		//=========== 포리스턴소스인경우
			bx = 32;
			by = 16;
			#else
			bx = 32;
			by = 16;
			#endif
			menuPos[0].left = BackStartPos.x + BUTTON1_X;		//확인
			menuPos[0].top = BackStartPos.y + BUTTON1_Y;
			menuPos[0].right = menuPos[0].left + bx;
			menuPos[0].bottom = menuPos[0].top + by;

			//텍스트 출력 위치
			menuPos[1].left = BackStartPos.x + clanClanJoinMsg.pos.x;
			menuPos[1].top = BackStartPos.y + clanClanJoinMsg.pos.y;

					#ifdef USE_PROSTONTALE		//=========== 포리스턴소스인경우
			bx = 64;
			by = 16;
			#else
			bx = 64;
			by = 16;
			#endif
			menuPos[2].left = BackStartPos.x + BUTTON1_BOX_X;		//확인
			menuPos[2].top = BackStartPos.y + BUTTON1_BOX_Y;
			menuPos[2].right = menuPos[2].left + bx;
			menuPos[2].bottom = menuPos[2].top + by;

			break;
		case CLAN_JOIN:
			//클랜칲이 가입하라고 권유할때 상대편에게 뜨는 메뉴  =================================START
	
			menuSu = 5;
			#ifdef USE_PROSTONTALE		//=========== 포리스턴소스인경우
			bx = 32;
			by = 16;
			#else
			bx = 32;
			by = 16;
			#endif

			menuPos[0].left = BackStartPos.x +  BUTTON2_X;		//yes
			menuPos[0].top =  BackStartPos.y +   BUTTON2_Y;
			menuPos[0].right = menuPos[0].left + bx;
			menuPos[0].bottom = menuPos[0].top + by;


			#ifdef USE_PROSTONTALE		//=========== 포리스턴소스인경우
			bx = 32;
			by = 16;
			#else
			bx = 32;
			by = 16;
			#endif

			menuPos[1].left = BackStartPos.x +  BUTTON3_X;		//no
			menuPos[1].top =  BackStartPos.y +   BUTTON3_Y;
			menuPos[1].right = menuPos[1].left+ bx;
			menuPos[1].bottom = menuPos[1].top + by;

			menuPos[2].left = BackStartPos.x +  150;		//클랜 마크
			menuPos[2].top =  BackStartPos.y +   150;
			menuPos[2].right = menuPos[2].left+ bx;
			menuPos[2].bottom = menuPos[2].top + by;

			//텍스트 출력 위치
			menuPos[3].left = BackStartPos.x + clanJoin.pos.x;
			menuPos[3].top = BackStartPos.y + clanJoin.pos.y;
			menuPos[3].right = menuPos[3].left + 280;
			menuPos[3].bottom = menuPos[3].top + 250;

			#ifdef USE_PROSTONTALE		//=========== 포리스턴소스인경우
			bx = 64;
			by = 16;
			#else
			bx = 64;
			by = 16;
			#endif

			menuPos[4].left = BackStartPos.x + BUTTON2_BOX_X;		//yes 테드리 박스
			menuPos[4].top = BackStartPos.y +BUTTON2_BOX_Y;
			menuPos[4].right = menuPos[4].left + bx;
			menuPos[4].bottom = menuPos[4].top + by;

			#ifdef USE_PROSTONTALE		//=========== 포리스턴소스인경우
			bx = 64;
			by = 16;
			#else
			bx = 64;
			by = 16;
			#endif

			menuPos[5].left = BackStartPos.x + BUTTON3_BOX_X;		//no 테드리 박스
			menuPos[5].top = BackStartPos.y +BUTTON3_BOX_Y;
			menuPos[5].right = menuPos[6].left + bx;
			menuPos[5].bottom = menuPos[6].top + by;			
		//클랜칲이 가입하라고 권유할때 상대편에게 뜨는 메뉴  =================================END
			break;
	}

}

void CLANEntity_JOIN::Draw()
{
	char string[256];
#ifdef USE_PROSTONTALE		//=========== 포리스턴소스인경우
	switch(C_menuN){
		case CLAN_CLANWONOVER:
			DrawSprite(menuPos[2].left,menuPos[2].top, hButton_Box,  0, 0, 48, 23, 1);		
			if(menuMouseOver[0]>0) DrawSprite(menuPos[0].left,menuPos[0].top, hOk_Yellow,  0, 0, 32, 16, 1);
			else	DrawSprite(menuPos[0].left,menuPos[0].top, hOk_Gray,  0, 0, 32, 16, 1);
			
			Draw_C_Text(clanClanWonOver.szMsg, menuPos[1].left, menuPos[1].top);		
			break;
		case CLANUSER_JOIN:
			DrawSprite(menuPos[2].left,menuPos[2].top, hButton_Box,  0, 0, 48, 23, 1);		
			if(menuMouseOver[0]>0) DrawSprite(menuPos[0].left,menuPos[0].top, hOk_Yellow,  0, 0, 32, 16, 1);
			else	DrawSprite(menuPos[0].left,menuPos[0].top, hOk_Gray,  0, 0, 32, 16, 1);
			
			Draw_C_Text(szUserJoin, menuPos[1].left, menuPos[1].top);		
			break;
		case CLAN_JOIN:
			DrawSprite(menuPos[4].left,menuPos[4].top, hButton_Box,  0, 0, 48, 23, 1);
			DrawSprite(menuPos[5].left,menuPos[5].top, hButton_Box,  0, 0, 48, 23, 1);
			if(menuMouseOver[0]>0) DrawSprite(menuPos[0].left,menuPos[0].top, hYes_Yellow,  0, 0, 32, 16, 1);
			else	DrawSprite(menuPos[0].left,menuPos[0].top, hYes_Gray,  0, 0, 32, 16, 1);
			
			if(menuMouseOver[1]>0) DrawSprite(menuPos[1].left,menuPos[1].top, hCancel_Yellow,  0, 0, 32, 16, 1);	
			else	DrawSprite(menuPos[1].left,menuPos[1].top, hCancel_Gray,  0, 0, 32, 16, 1);			
			
			//wsprintf(string,szJoin,cldata.szJoinClanName,cldata.szJoinClanChipName);
			wsprintf(string,clanJoin.szMsg,cldata.szJoinClanName,cldata.szJoinClanChipName);
			Draw_C_Text(string, menuPos[3].left, menuPos[3].top);		
			break;
	}
#else
	HDC destdc;
	HDC hdc;
	HDC sorDC;
	BITMAP bit;
	int bx,by;
	switch(C_menuN){
		case CLAN_CLANWONOVER:
			hdc = GetDC(g_hwnd);
			sorDC  = CreateCompatibleDC(hdc);		
			destdc=CreateCompatibleDC(hdc);

			SelectObject(destdc, Dback.hb );		//off스크린설정.
			SelectObject(sorDC, hButton_Box);		
			GetObject(hButton_Box,sizeof(BITMAP),&bit);
			bx = bit.bmWidth;
			by = bit.bmHeight;	
			BitBlt(destdc,menuPos[2].left,menuPos[2].top,menuPos[2].right,menuPos[2].bottom,sorDC,0,0,SRCCOPY);

			if(menuMouseOver[0] > 0)
			{
				SelectObject(sorDC, hOk_Yellow);
				GetObject(hOk_Yellow,sizeof(BITMAP),&bit);
				bx = bit.bmWidth;
				by = bit.bmHeight;	
				BitBlt(destdc,menuPos[0].left,menuPos[0].top,menuPos[0].right,menuPos[0].bottom,sorDC,0,0,SRCCOPY);
			}
			else
			{
				SelectObject(sorDC, hOk_Gray);
				GetObject(hOk_Gray,sizeof(BITMAP),&bit);
				bx = bit.bmWidth;
				by = bit.bmHeight;	
				BitBlt(destdc,menuPos[0].left,menuPos[0].top,menuPos[0].right,menuPos[0].bottom,sorDC,0,0,SRCCOPY);
			}		
			
			//Draw_Text(menuPos[1].left,menuPos[1].top,szReport);
			Text_Out(destdc,menuPos[1].left,menuPos[1].top,clanClanWonOver.szMsg);

			
			
			DeleteDC(sorDC);
			DeleteDC(destdc);
			ReleaseDC(g_hwnd, hdc );
			break;
		case CLANUSER_JOIN:
			hdc = GetDC(g_hwnd);
			sorDC  = CreateCompatibleDC(hdc);		
			destdc=CreateCompatibleDC(hdc);

			SelectObject(destdc, Dback.hb );		//off스크린설정.
			SelectObject(sorDC, hButton_Box);		
			GetObject(hButton_Box,sizeof(BITMAP),&bit);
			bx = bit.bmWidth;
			by = bit.bmHeight;	
			BitBlt(destdc,menuPos[2].left,menuPos[2].top,menuPos[2].right,menuPos[2].bottom,sorDC,0,0,SRCCOPY);

			if(menuMouseOver[0] > 0)
			{
				SelectObject(sorDC, hOk_Yellow);
				GetObject(hOk_Yellow,sizeof(BITMAP),&bit);
				bx = bit.bmWidth;
				by = bit.bmHeight;	
				BitBlt(destdc,menuPos[0].left,menuPos[0].top,menuPos[0].right,menuPos[0].bottom,sorDC,0,0,SRCCOPY);
			}
			else
			{
				SelectObject(sorDC, hOk_Gray);
				GetObject(hOk_Gray,sizeof(BITMAP),&bit);
				bx = bit.bmWidth;
				by = bit.bmHeight;	
				BitBlt(destdc,menuPos[0].left,menuPos[0].top,menuPos[0].right,menuPos[0].bottom,sorDC,0,0,SRCCOPY);
			}		
			
			//Draw_Text(menuPos[1].left,menuPos[1].top,szReport);
			Text_Out(destdc,menuPos[1].left,menuPos[1].top,szUserJoin);

			
			
			DeleteDC(sorDC);
			DeleteDC(destdc);
			ReleaseDC(g_hwnd, hdc );
			break;

		case CLAN_JOIN:
		hdc = GetDC(g_hwnd);
		sorDC  = CreateCompatibleDC(hdc);
		destdc = CreateCompatibleDC(hdc);
		SelectObject(destdc, Dback.hb );		//off스크린설정.

		SelectObject(sorDC,hButton_Box);
		GetObject(hButton_Box,sizeof(BITMAP),&bit);
		bx = bit.bmWidth;
		by = bit.bmHeight;		
		BitBlt(destdc,menuPos[4].left,menuPos[4].top,menuPos[4].right,menuPos[4].bottom,sorDC,0,0,SRCCOPY);

		SelectObject(sorDC,hButton_Box);
		GetObject(hButton_Box,sizeof(BITMAP),&bit);
		bx = bit.bmWidth;
		by = bit.bmHeight;		
		BitBlt(destdc,menuPos[5].left,menuPos[5].top,menuPos[5].right,menuPos[5].bottom,sorDC,0,0,SRCCOPY);

		if(menuMouseOver[0]>0)
		{
			SelectObject(sorDC, hYes_Yellow);

			GetObject(hYes_Yellow,sizeof(BITMAP),&bit);
			bx = bit.bmWidth;
			by = bit.bmHeight;
			
			BitBlt(destdc,menuPos[0].left,menuPos[0].top,menuPos[0].right,menuPos[0].bottom,sorDC,0,0,SRCCOPY);			
		}
		else
		{
			SelectObject(sorDC, hYes_Gray);

			GetObject(hYes_Gray,sizeof(BITMAP),&bit);
			bx = bit.bmWidth;
			by = bit.bmHeight;
			
			BitBlt(destdc,menuPos[0].left,menuPos[0].top,menuPos[0].right,menuPos[0].bottom,sorDC,0,0,SRCCOPY);
		}
		if(menuMouseOver[1]>0)
		{
			SelectObject(sorDC, hCancel_Yellow);

			GetObject(hCancel_Gray,sizeof(BITMAP),&bit);
			bx = bit.bmWidth;
			by = bit.bmHeight;
			
			BitBlt(destdc,menuPos[1].left,menuPos[1].top,menuPos[1].right,menuPos[1].bottom,sorDC,0,0,SRCCOPY);
		}
		else
		{
			SelectObject(sorDC, hCancel_Gray);

			GetObject(hCancel_Gray,sizeof(BITMAP),&bit);
			bx = bit.bmWidth;
			by = bit.bmHeight;
			
			BitBlt(destdc,menuPos[1].left,menuPos[1].top,menuPos[1].right,menuPos[1].bottom,sorDC,0,0,SRCCOPY);
		}
		
		
		//wsprintf(string,szJoin,cldata.szJoinClanName,cldata.szJoinClanChipName);
		wsprintf(string,clanJoin.szMsg,cldata.szJoinClanName,cldata.szJoinClanChipName);
		//DrawText(GetDC(g_hwnd),string,-1,&rect,DT_LEFT | DT_WORDBREAK);
		Text_Out(destdc,menuPos[3].left,menuPos[3].top,string);
	

		DeleteDC(sorDC);
		DeleteDC(destdc);
		ReleaseDC(g_hwnd, hdc );
		break;
	}
#endif

}

void CLANEntity_JOIN::AddClanWon()
{
	int i;
	for(i =0; i < CLANWON_MAX-1; i++){	
		if(cldata.clanWon[i][0] == 0){
			strcpy(cldata.clanWon[i],cldata.clanJoinCharID);
			nClanWon_Num +=1;

			cldata.nTotalClanWonCount = nClanWon_Num ;
			cldata.nCurrentClanWon_Num +=1;
			break;
		}
	}




	//=============== ktj : 디버깅
	void fd2( char *fmt, ...);

	for(i =0; i < CLANWON_MAX-1; i++){	
		if(cldata.clanWon[i][0] != 0){
			fd2("\305\254\267\243\277\370\270\256\275\272\306\256  cldata.clanWon[%d] = %s  ", i, cldata.clanWon[i]);
		}
	}

}




//========================================================================================클랜원수
//레벨에따라 가입할수 있는 클랜원수 테이블 초기 세팅
//
//makeclanlevel : 처음 클랜을 만들수 있는 레벨 
//makeclanlevel_clanwon : 처음 클랜을 만들때 가입할수 있는 초기 클랜원수
//addclanwonlevel_start : 가입할 수 있는 클랜원 수가 처음 증가할때 레벨
//addlevel : 증가하는 레벨단계
//addclanwon_num : 증가하는 클랜원 단계
//maxlevel : 최대 레벨

//============================================================================
//ktj : 레벨50이상부터 클랜만들기가능할것
//============================================================================

//int nClanWonLevelTable[CLANWONLEVEL_TABLE] = {
int nClanWonLevelTable[] = {
		0,0,0,0,0,0,0,0,0,0, //0~9
		0,0,0,0,0,0,0,0,0,0, //10~19
		0,0,0,0,0,0,0,0,0,0, //20~29
		0,0,0,0,0,0,0,0,0,0, //30~39
		20,21,   //40~41
		22,23,   //42~43
		24,25,   //44~45
		26,27,   //46~47
		28,29,   //48~49
		30,31, //50~51
		32,33, //52~53
		34,35, //54~55
		36,37, //56~57
		38,39, //58~59
		40,41, //60~61
		42,43, //62~63
		44,45, //64~65
		46,47, //66~67
		48,49, //68~69
		50,51, //70,71
		52,53, //72,73
		54,55, //74,75
		56,57, //76,77
		58,59, //78,79
		60,61, //80,81
		62,63, //82.83
		64,65, //84,85
		66,67, //86,87
		68,69, //80~89
		70,71, //90,91
		72,73, //92,93
		74,75, //94,95
		76,77, //96,97
		78,80, //98~99

		/*
		2,2,2,2,2,2,2,2,2,2, //0~9
		2,2,2,2,2,2,2,2,2,2, //10~19
		2,2,2,2,2,2,2,2,2,2, //20~29
		2,2,2,2,2,2,2,2,2,2, //30~39
		2,2,2,2,2,2,2,2,2,2, //40~49
		2,2,2,2,2,2,2,2,2,2, //50~59
		2,2,2,2,2,2,2,2,2,2, //60~69
		3,//70
		4,//71
		5,//72
		6,//73
		7,//74
		8,//75
		9,//76
		10,//77
		11,//78
		12,//79
		20,20,20,20,20,20,20,20,20,20,//80~89
		2,20,20,20,20,20,20,20,20,20,//90~99
		*/
};

void CLANEntity_JOIN::SetLevelTable()
{	

	

	//ZeroMemory(cldata.nLevelTable,sizeof(cldata.nLevelTable));

	/*cldata.nLevelTable[0][0] = makeclanlevel;
	cldata.nLevelTable[0][1] = makeclanlevel_clanwon; 

	nMaxLevel = maxlevel - addclanwonlevel_start;
	nMaxLevel = (nMaxLevel / addlevel) + 1;

	for(cnt = 1; cnt <= nMaxLevel; cnt++)
	{
		cldata.nLevelTable[cnt][0] = addclanwonlevel_start + nAddLevel;
		nAddClanwon +=addclanwon_num;
		cldata.nLevelTable[cnt][1] = makeclanlevel_clanwon + nAddClanwon;
		nAddLevel += addlevel;
	}*/

}

BOOL CLANEntity_JOIN::CheckClanWon(int level)
{	
	int nMaxClanWonCount = 0;	
	nMaxClanWonCount = nClanWonLevelTable[level];
	if(nMaxClanWonCount > cldata.nCurrentClanWon_Num) {  //등록가능
	//if(nMaxClanWonCount > 15) {
		
		return TRUE;
	}


	/* 2003.8.5 부클랜칩이 가입권유할때 CLAN_CLANWONOVER이 메세지가 떠서 버그 수정
	C_menuN2 = 0;
	OpenFlag = 1;
	ClanCLCharMove = 1;	
	C_menuN = CLAN_CLANWONOVER;
	cClanJoin.menuInit(C_menuN);
	*/

	return TRUE;
}


int CLANEntity_JOIN::GetMaxClanWonCount(int level)
{
	return nClanWonLevelTable[level];	
}

//레벨 테이블 초기화

//레벨에따라 가입할수 있는 클랜원수 테이블 초기 세팅
//
//makeclanlevel : 처음 클랜을 만들수 있는 레벨 
//makeclanlevel_clanwon : 처음 클랜을 만들때 가입할수 있는 초기 클랜원수
//addclanwonlevel_start : 가입할 수 있는 클랜원 수가 처음 증가할때 레벨
//addlevel : 증가하는 레벨단계
//addclanwon_num : 증가하는 클랜원 단계
//maxlevel : 최대 레벨

void SetLevelTable(int makeclanlevel,int makeclanlevel_clanwon,int addclanwonlevel_start, int addlevel, int addclanwon_num,int maxlevel)
{
	cClanJoin.SetLevelTable();
}

// 0 이면 더이상 클랜원을 추가 할수 없음(자체메뉴에서 등록시킬수없다고 나온다)
BOOL CheckClanWon(int level)
{
	BOOL bReturn = 0;
	bReturn = cClanJoin.CheckClanWon(level);	
	return bReturn;
}

