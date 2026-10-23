
#define CE_USER_CPP
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
#include "cE_user.h"
#include "tjscroll.h"
#include "cE_report.h"
#include "cE_CViewClanInfo.h"
#include "..\\isaocheck\\auth.h"
#include "..\\isaocheck\\isaocheck.h"
#include "..\\ygy\\ygyheader.h"

//void CheckMsgPos(CLANMSG* clanmsg,int width, int height);
//*****************************************************************************
CLANEntity_user::CLANEntity_user()
{
	ZeroMemory(szTemp,sizeof(szTemp));

}

CLANEntity_user::~CLANEntity_user()
{

}


//*****************************************************************************
int CLANEntity_user::Init(int Life)
{
	life = Life;
	bSubChip = FALSE;
	ZeroMemory(szClanUserMsg,sizeof(szClanUserMsg));
	return 1;
}

int CLANEntity_user::Clear()
{
	return 1;
}


//*****************************************************************************
void CLANEntity_user::Main()
{
	int i;
	int PFlag = 0;
	if(life ==0) return;

	switch(C_menuN)
	{	
	//부칩관련 메세지 처리 ============
	case CLANUSER_SUBCHIP_READMSG_FLAG:
		switch(C_menuN2){
			case 0:
				ANSdata.ret_val= -1;			

				WebDB.Resume() ;		//웹db를 재개한다.
				
				//WebDB.isKPFlag(cldata.szID,cldata.name,cldata.ChaName,cldata.gserver,"201","1");
				if(cldata.nKFlag == 4) //부칩임명만 들어 왔을때
				{
					WebDB.isKPFlag(cldata.szID,cldata.name,cldata.ChaName,cldata.gserver,"8","1");
				}
				else if(cldata.nKFlag == 16) //부칩해임만 들어 왔을때
				{
					WebDB.isKPFlag(cldata.szID,cldata.name,cldata.ChaName,cldata.gserver,"32","1");
				}
				else if(cldata.nKFlag == 5) //칩위임 부칩임명만 들어 왔을때(칩위임 확인 부칩임명확인플래그 세팅)
				{
					WebDB.isKPFlag(cldata.szID,cldata.name,cldata.ChaName,cldata.gserver,"8","1");
				}
				else if((cldata.nKFlag == 7)&&(cldata.nKFlag_Count)) //칩위임만 확인했고 부침임명확인 안했어 추가로 플래그 세팅
				{
					WebDB.isKPFlag(cldata.szID,cldata.name,cldata.ChaName,cldata.gserver,"8","1");
				}
				else if((cldata.nKFlag == 17)&&(cldata.nKFlag_Count == 1)) //칩위임 부치해임
				{
					WebDB.isKPFlag(cldata.szID,cldata.name,cldata.ChaName,cldata.gserver,"32","1");
				}
				else if((cldata.nKFlag == 19)&&(cldata.nKFlag_Count == 1)) //칩위임 부칩해임에서 칩위임만 확인했을 경우
				{
					WebDB.isKPFlag(cldata.szID,cldata.name,cldata.ChaName,cldata.gserver,"32","1");
				}
				else if((cldata.nKFlag == 20)&&(cldata.nKFlag_Count ==1)&&(bSubChip==TRUE)) //부칩임명 부칩해임만 들어 왔을때(부칩해임만 확인)
				{
					WebDB.isKPFlag(cldata.szID,cldata.name,cldata.ChaName,cldata.gserver,"32","1");
				}
				else if((cldata.nKFlag == 20) &&(cldata.nKFlag_Count == 2)&&(bSubChip==TRUE)) //부칩임명확인  플래그 세팅
				{
					WebDB.isKPFlag(cldata.szID,cldata.name,cldata.ChaName,cldata.gserver,"8","1");
				}
				else if((cldata.nKFlag == 20)&&(cldata.nKFlag_Count ==1)&&(bSubChip==FALSE))
				{
					WebDB.isKPFlag(cldata.szID,cldata.name,cldata.ChaName,cldata.gserver,"8","1");
				}
				else if((cldata.nKFlag == 20) &&(cldata.nKFlag_Count == 2)&&(bSubChip==FALSE)) //부칩임명확인  플래그 세팅
				{
					WebDB.isKPFlag(cldata.szID,cldata.name,cldata.ChaName,cldata.gserver,"32","1");
				}
				else if(cldata.nKFlag == 52) //부칩해임만 확인하여 추가로 부침임명확인 플래그 세팅
				{
					WebDB.isKPFlag(cldata.szID,cldata.name,cldata.ChaName,cldata.gserver,"8","1");
				}
				else if((cldata.nKFlag == 21) && (cldata.nKFlag_Count == 1)&&(bSubChip == TRUE)) //클랜칩 위임 부칩해임 부칩임명 들어 왔을때(칩이임확인,부칩해임 확인)
				{
					WebDB.isKPFlag(cldata.szID,cldata.name,cldata.ChaName,cldata.gserver,"32","1");
				}
				else if((cldata.nKFlag == 21)&& (cldata.nKFlag_Count == 2)&&(bSubChip == TRUE)) //클랜칩 위임 부칩해임 부칩임명 들어 왔을때(칩이임확인,부칩해임 확인,부칩임명 확인)
				{
					WebDB.isKPFlag(cldata.szID,cldata.name,cldata.ChaName,cldata.gserver,"8","1");
				}
				else if((cldata.nKFlag == 21) && (cldata.nKFlag_Count == 1)&&(bSubChip == FALSE))
				{
					WebDB.isKPFlag(cldata.szID,cldata.name,cldata.ChaName,cldata.gserver,"8","1");
				}
				else if((cldata.nKFlag == 21)&& (cldata.nKFlag_Count == 2)&&(bSubChip == FALSE)) //클랜칩 위임 부칩해임 부칩임명 들어 왔을때(칩이임확인,부칩해임 확인,부칩임명 확인)
				{
					WebDB.isKPFlag(cldata.szID,cldata.name,cldata.ChaName,cldata.gserver,"32","1");
				}
				else if((cldata.nKFlag == 23)&&(cldata.nKFlag_Count == 1)) //클랜칩 위임 부칩해임 부칩임명 에서 클랜칩 확인만 했을 경우
				{
					WebDB.isKPFlag(cldata.szID,cldata.name,cldata.ChaName,cldata.gserver,"32","1");
				}
				else if((cldata.nKFlag == 23)&&(cldata.nKFlag_Count == 2))//클랜칩위임 부칩해임 부칩임명에서 마지막 부칩임명 확인
				{
					WebDB.isKPFlag(cldata.szID,cldata.name,cldata.ChaName,cldata.gserver,"8","1");
				}
				else if(cldata.nKFlag == 55) //클랜칩위임 부칩해임 부칩임명에서 클랜칩위임 부침해임만 확인하였을 경우
				{
					WebDB.isKPFlag(cldata.szID,cldata.name,cldata.ChaName,cldata.gserver,"8","1");
				}
				else if(cldata.nKFlag == 28) //부칩해임 확인 플래그세팅
				{
					WebDB.isKPFlag(cldata.szID,cldata.name,cldata.ChaName,cldata.gserver,"32","1");
				}
				else{}



				
				C_menuN2=1;
#ifdef CLAN_DEBUG
				DebugMessage("CLANUSER_LEAVECLAN_MESSAGEEND\277\241\274\255 WebDB.isPFlag(\276\306\300\314\265\360 : %s,\305\254\267\243\300\314\270\247 : %s,\304\263\270\257\305\315\300\314\270\247 : %s,\260\324\300\323\274\255\271\366 : %s,\307\303\267\241\261\327 : %s,0)",cldata.szID,cldata.name,cldata.ChaName,cldata.gserver,cldata.PFlag);
#endif
				break;
			case 1:
				switch(ANSdata.ret_val){
					case 0://플래그 삽입 실패
						WebDB.Suspend();
						C_menuN = CLAN_ERROR;
						wsprintf(szErrorMsg,"%s (84)",clanszError_Msg.szMsg);
						cClanReport.menuInit(C_menuN,clanszError_Msg.pos.x,clanszError_Msg.pos.y);
						cClanReport.InitMsg(szErrorMsg);
						C_menuN2=-1;
#ifdef CLAN_DEBUG
				DebugMessage("CLANUSER_LEAVECLAN_MESSAGEEND\277\241\274\255 \273\360\300\324\275\307\306\320");
#endif
						break;
					case 1: //플래그 삽입 성공							
						WebDB.Suspend();														
						C_menuN2=-1;	

						if((cldata.nKFlag == 20)&&(cldata.nKFlag_Count ==1)) //부칩임명 부칩해임만 들어 왔을때(부칩해임만 확인)
						{
							cldata.nKFlag_Count = 2;
							if(bSubChip==TRUE)	C_menuN = CLANUSER_SUBCHIP_APPOINTMSG;
							else C_menuN = CLANUSER_SUBCHIP_RELEASEMSG;
							menuInit(C_menuN);
							break;
						}
						else if((cldata.nKFlag == 21) && (cldata.nKFlag_Count == 1)) //클랜칩 위임 부칩해임 부칩임명 들어 왔을때(칩이임확인,부칩해임 확인)
						{
							cldata.nKFlag_Count = 2;
							if(bSubChip==TRUE)	C_menuN = CLANUSER_SUBCHIP_APPOINTMSG;
							else C_menuN = CLANUSER_SUBCHIP_RELEASEMSG;
							menuInit(C_menuN);
							break;
						}
						else if((cldata.nKFlag == 23)&&(cldata.nKFlag_Count == 1)) //클랜칩 위임 부치해임 부칩임명 에서 클랜칩 확인만 했을 경우
						{
							cldata.nKFlag_Count = 2;
							C_menuN = CLANUSER_SUBCHIP_APPOINTMSG;
							menuInit(C_menuN);
							break;
						}
						else{}
						if((cldata.nKFlag == 4) ||
							(cldata.nKFlag == 16)||
							(cldata.nKFlag == 17)||
							(cldata.nKFlag == 19)||
							(cldata.nKFlag == 5)||
							(cldata.nKFlag == 7)||
							(cldata.nKFlag == 52)||
							((cldata.nKFlag == 23)&&(cldata.nKFlag_Count == 2))||
							((cldata.nKFlag == 21)&& (cldata.nKFlag_Count == 2))||
							(cldata.nKFlag == 55))
						{
							cldata.nKFlag_Count = 0;
						}
						


						
						//============================================================

						if((strcmp(cldata.stats,"0") == 0)&&(strcmp(cldata.PFlag,"0") == 0))
						{
							C_menuN = CLAN_USER_NOMAKE;	//stats가 0이면 6명이하이므로 더 모집하라는메시지보냄
							menuInit(C_menuN );
							C_menuN2=-1;
						}
						else if((strcmp(cldata.stats,"1") == 0)&&(strcmp(cldata.PFlag,"0") == 0)) //클랜 결성되고 처음 보여주는 메세지
						{
							strcpy(cldata.PFlag,"2");
							wsprintf(string,clanMakeFinishClanWon.szMsg,cldata.name);
							strcpy(clanMakeFinishClanWon.szMsg,string);
							//void CheckMsgPos(CLANMSG* clanmsg);
							CheckMsgPos(&clanMakeFinishClanWon,BOX_MAX_WIDTH,0);					
							C_menuN = CLANUSER_MAKE_FIRSTMESSAGE;	//stats가 0이면 6명이하이므로 더 모집하라는메시지보냄

							menuInit(C_menuN );
							C_menuN2=-1;
						}				
						//else if((strcmp(cldata.stats,"0") == 0)&&(strcmp(cldata.PFlag,"2") == 0)) //클랜원이 6명이라로 떨어졌을 경우
						//{	
						//	strcpy(cldata.PFlag,"3");
						//	C_menuN = CLANUSER_DOWN6_CLANWON;	
						//	menuInit(C_menuN );									
						//	C_menuN2=-1;
						//}
						else if((strcmp(cldata.stats,"1") == 0)&&(strcmp(cldata.PFlag,"2") == 0)&&(nClanWon_Num <5))
						{
							C_menuN = CLANUSER_DOWN6_CLANWON_MESSAGE;	
							menuInit(C_menuN );									
							C_menuN2=-1;
						}
						//else if((strcmp(cldata.stats,"1") == 0)&&(strcmp(cldata.PFlag,"3") == 0))
						//{
						//	strcpy(cldata.PFlag,"4");
						//	C_menuN = ISPFLAG_USER;		//도움말로 돌아감.
						//	menuInit(C_menuN);
						//	C_menuN2 = 0;					
						//}				
						//else if((strcmp(cldata.stats,"0") == 0)&&(strcmp(cldata.PFlag,"4") == 0))
						//{
						//	strcpy(cldata.PFlag,"3");
						//	C_menuN = CLANUSER_DOWN6_CLANWON;	
						//	menuInit(C_menuN );									
						//	C_menuN2=-1;
						//}
						else // 1:1 or 1:3
						{
							C_menuN = CLAN_USER;	//바로 클랜칲메뉴로 간다.
							menuInit(C_menuN );									
							C_menuN2=-1;
						}
#ifdef CLAN_DEBUG
			DebugMessage("CLANUSER_LEAVECLAN_MESSAGEEND\277\241\274\255 \301\244\273\363\300\373\300\270\267\316 \303\263\270\256");
#endif

					break;
					default:

						if(ANSdata.ret_val != -1) {		//전혀엉뚱한 답이 와서 에러가 났다.
							WebDB.Suspend();
							C_menuN = CLAN_ERROR;
							wsprintf(szErrorMsg,"%s (85)",clanszError_Msg.szMsg);
							cClanReport.menuInit(C_menuN,clanszError_Msg.pos.x,clanszError_Msg.pos.y);
							cClanReport.InitMsg(szErrorMsg);
							C_menuN2 = -1;
#ifdef CLAN_DEBUG
				DebugMessage("CLANUSER_LEAVECLAN_MESSAGEEND\277\241\274\255 \265\360\306\372\306\256\277\241\267\257");
#endif
						}						
						break;
				}
		}		
		break;
	case CLANUSER_SUBCHIP_APPOINTMSG://클랜부침이 임명되다고 한번 보여줌
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
				C_menuN = CLANUSER_SUBCHIP_READMSG_FLAG;
				menuInit(C_menuN);
				C_menuN2 = 0;
				break;				
				
			default:
				i = i;
				break;

			}
		}		
		break;
		
	case CLANUSER_SUBCHIP_RELEASEMSG://클랜부침이 해임되다고 한번 보여줌
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
				C_menuN = CLANUSER_SUBCHIP_READMSG_FLAG;
				menuInit(C_menuN);
				C_menuN2 = 0;
				break;				
				
			default:
				i = i;
				break;

			}
		}		
		break;		
	//======================================
	case CLANUSER_LEAVECLAN_MESSAGEEND:
		switch(C_menuN2){
			case 0:
				ANSdata.ret_val= -1;			

				WebDB.Resume() ;		//웹db를 재개한다.
				
				//WebDB.isKPFlag(cldata.szID,cldata.name,cldata.ChaName,cldata.gserver,"","1");
				if((cldata.nKFlag == 1)&&(cldata.nKFlag_Count == 1)) //클랜칩 위임
				{
					WebDB.isKPFlag(cldata.szID,cldata.name,cldata.ChaName,cldata.gserver,"2","1");
				}				
				else if((cldata.nKFlag == 5)&&(cldata.nKFlag_Count == 1)) //칩이임 부침임명
				{
					WebDB.isKPFlag(cldata.szID,cldata.name,cldata.ChaName,cldata.gserver,"2","1");
				}
				else if((cldata.nKFlag == 17)&&(cldata.nKFlag_Count == 1)) //칩이임 부침해임
				{
					WebDB.isKPFlag(cldata.szID,cldata.name,cldata.ChaName,cldata.gserver,"2","1");
				}
				else if((cldata.nKFlag == 21)&&(cldata.nKFlag_Count == 1)) //칩이임 부침해임
				{
					WebDB.isKPFlag(cldata.szID,cldata.name,cldata.ChaName,cldata.gserver,"2","1");
				}
				else{}
				
				C_menuN2=1;
#ifdef CLAN_DEBUG
				DebugMessage("CLANUSER_LEAVECLAN_MESSAGEEND\277\241\274\255 WebDB.isPFlag(\276\306\300\314\265\360 : %s,\305\254\267\243\300\314\270\247 : %s,\304\263\270\257\305\315\300\314\270\247 : %s,\260\324\300\323\274\255\271\366 : %s,\307\303\267\241\261\327 : %s,0)",cldata.szID,cldata.name,cldata.ChaName,cldata.gserver,cldata.PFlag);
#endif
				break;
			case 1:
				switch(ANSdata.ret_val){
					case 0://플래그 삽입 실패
						WebDB.Suspend();
						C_menuN = CLAN_ERROR;
						wsprintf(szErrorMsg,"%s (84)",clanszError_Msg.szMsg);
						cClanReport.menuInit(C_menuN,clanszError_Msg.pos.x,clanszError_Msg.pos.y);
						cClanReport.InitMsg(szErrorMsg);
						C_menuN2=-1;
#ifdef CLAN_DEBUG
				DebugMessage("CLANUSER_LEAVECLAN_MESSAGEEND\277\241\274\255 \273\360\300\324\275\307\306\320");
#endif
						break;
					case 1: //플래그 삽입 성공							
						WebDB.Suspend();														
						C_menuN2=-1;			
						
						
						if((cldata.nKFlag == 1)&&(cldata.nKFlag_Count == 1)) //클랜칩 위임
						{
							cldata.nKFlag_Count = 0;
							
						}
						else if(cldata.nKFlag == 17) //부클랜칩 해임				
						{
							//cldata.nKFlag_Count +=2;
							cldata.nKFlag_Count = 1;
							
							C_menuN = CLANUSER_SUBCHIP_RELEASEMSG;
							menuInit(C_menuN);					
							break;
						}
						else if(cldata.nKFlag == 5) //부클랜칩 임명
						{
							cldata.nKFlag_Count = 1;
							C_menuN = CLANUSER_SUBCHIP_APPOINTMSG;
							menuInit(C_menuN);				
							break;
						}
						else if(cldata.nKFlag == 21) //부클랜칩 해임,임명
						{
							cldata.nKFlag_Count = 1;
							if(cldata.SubChip[0] == 0) 
							{
								bSubChip = FALSE;
								C_menuN = CLANUSER_SUBCHIP_APPOINTMSG;
							}
							else	{
								bSubChip = TRUE;
								C_menuN = CLANUSER_SUBCHIP_RELEASEMSG;
							}
							menuInit(C_menuN);					
							break;
						}
						else{}


					if((strcmp(cldata.stats,"0") == 0)&&(strcmp(cldata.PFlag,"0") == 0))
					{
						C_menuN = CLAN_USER_NOMAKE;	//stats가 0이면 6명이하이므로 더 모집하라는메시지보냄
						menuInit(C_menuN );
						C_menuN2=-1;
					}
					else if((strcmp(cldata.stats,"1") == 0)&&(strcmp(cldata.PFlag,"0") == 0)) //클랜 결성되고 처음 보여주는 메세지
					{
						strcpy(cldata.PFlag,"2");
						wsprintf(string,clanMakeFinishClanWon.szMsg,cldata.name);
						strcpy(clanMakeFinishClanWon.szMsg,string);
						//void CheckMsgPos(CLANMSG* clanmsg);
						CheckMsgPos(&clanMakeFinishClanWon,BOX_MAX_WIDTH,0);					
						C_menuN = CLANUSER_MAKE_FIRSTMESSAGE;	//stats가 0이면 6명이하이므로 더 모집하라는메시지보냄

						menuInit(C_menuN );
						C_menuN2=-1;
					}				
					//else if((strcmp(cldata.stats,"0") == 0)&&(strcmp(cldata.PFlag,"2") == 0)) //클랜원이 6명이라로 떨어졌을 경우
					//{	
					//	strcpy(cldata.PFlag,"3");
					//	C_menuN = CLANUSER_DOWN6_CLANWON;	
					//	menuInit(C_menuN );									
					//	C_menuN2=-1;
					//}
					else if((strcmp(cldata.stats,"1") == 0)&&(strcmp(cldata.PFlag,"2") == 0)&&(nClanWon_Num <5))
					{
						C_menuN = CLANUSER_DOWN6_CLANWON_MESSAGE;	
						menuInit(C_menuN );									
						C_menuN2=-1;
					}
					//else if((strcmp(cldata.stats,"1") == 0)&&(strcmp(cldata.PFlag,"3") == 0))
					//{
					//	strcpy(cldata.PFlag,"4");
					//	C_menuN = ISPFLAG_USER;		//도움말로 돌아감.
					//	menuInit(C_menuN);
					//	C_menuN2 = 0;					
					//}				
					//else if((strcmp(cldata.stats,"0") == 0)&&(strcmp(cldata.PFlag,"4") == 0))
					//{
					//	strcpy(cldata.PFlag,"3");
					//	C_menuN = CLANUSER_DOWN6_CLANWON;	
					//	menuInit(C_menuN );									
					//	C_menuN2=-1;
					//}
					else // 1:1 or 1:3
					{
						C_menuN = CLAN_USER;	//바로 클랜칲메뉴로 간다.
						menuInit(C_menuN );									
						C_menuN2=-1;
					}
#ifdef CLAN_DEBUG
			DebugMessage("CLANUSER_LEAVECLAN_MESSAGEEND\277\241\274\255 \301\244\273\363\300\373\300\270\267\316 \303\263\270\256");
#endif

					break;
					default:

						if(ANSdata.ret_val != -1) {		//전혀엉뚱한 답이 와서 에러가 났다.
							WebDB.Suspend();
							C_menuN = CLAN_ERROR;
							wsprintf(szErrorMsg,"%s (85)",clanszError_Msg.szMsg);
							cClanReport.menuInit(C_menuN,clanszError_Msg.pos.x,clanszError_Msg.pos.y);
							cClanReport.InitMsg(szErrorMsg);
							C_menuN2 = -1;
#ifdef CLAN_DEBUG
				DebugMessage("CLANUSER_LEAVECLAN_MESSAGEEND\277\241\274\255 \265\360\306\372\306\256\277\241\267\257");
#endif
						}						
						break;
				}
		}
		break;	
	case CLANUSER_LEAVECLAN_MESSAGE:
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
				C_menuN = CLANUSER_LEAVECLAN_MESSAGEEND;
				menuInit(C_menuN);
				C_menuN2 = 0;
				break;				
				
			default:
				i = i;
				break;

			}
		}		
		break;
	case CLANUSER_REMOVE ://클랜원이 추방당했을경우
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
				C_menuN = CLANUSER_RELEASE_FLAG;
				menuInit(C_menuN );
				C_menuN2 =0;			

				
				break;	
			default:
				i = i;
				break;

			}
		}
		break;		
		
	case	CLANUSER_AUTORELEASE://자동해체
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
				C_menuN = CLANUSER_RELEASE_FLAG;
				menuInit(C_menuN );
				C_menuN2 =0;
				break;	
			default:
				i = i;
				break;

			}
		}
		
		break;
	case CLANUSER_GO_RELEASEMESSAGE:
		C_menuN = CLANUSER_RELEASEMESSAGE;
		menuInit(C_menuN);
		C_menuN2= -1;
		break;
	case CLANUSER_CHECKPFLAG: //등록된 해체 경우 103 미등록 해체 101 자동해체 102
		PFlag = atoi(cldata.PFlag);
		if(PFlag ==101){

			strcpy(szTemp,clanClanUserReleaseMsg.szMsg);
			wsprintf(szClanUserMsg,clanClanUserReleaseMsg.szMsg,cldata.name);
			strcpy(clanClanUserReleaseMsg.szMsg,szClanUserMsg);
			//void CheckMsgPos(CLANMSG* clanmsg);
			CheckMsgPos(&clanClanUserReleaseMsg,BOX_MAX_WIDTH,0);
			C_menuN = CLANUSER_AUTORELEASE;
			menuInit(C_menuN);
			strcpy(clanClanUserReleaseMsg.szMsg,szTemp);
		}
		else if(PFlag ==102){
			strcpy(szTemp,clanClanUserReleaseMsg.szMsg);
			wsprintf(szClanUserMsg,clanClanUserReleaseMsg.szMsg,cldata.name);
			strcpy(clanClanUserReleaseMsg.szMsg,szClanUserMsg);
			
			//void CheckMsgPos(CLANMSG* clanmsg);
			CheckMsgPos(&clanClanUserReleaseMsg,BOX_MAX_WIDTH,0);

			C_menuN = CLANUSER_AUTORELEASE;
			menuInit(C_menuN);
			strcpy(clanClanUserReleaseMsg.szMsg,szTemp);
		}		
		else if(PFlag ==110){
			C_menuN = CLANUSER_REMOVE;
			menuInit(C_menuN);
		}
		else{
			C_menuN = CLANUSER_RELEASEMESSAGE;
			menuInit(C_menuN);
		}
		
		break;
	case CLANUSER_RELEASEMESSAGE:
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
				
				C_menuN = CLANUSER_RELEASE_FLAG;
				menuInit(C_menuN );
				C_menuN2 =0;
				//C_menuN = CLAN_AllEnd;
				//C_menuN = CLAN_ERROR_ret;		//나가기 : 끝내는걸로간다.
				

				
				
				break;	
			default:				
				break;

			}
		}
		break;
	case CLAN_USER_NOMAKE_FLAG:
		C_menuN = CLAN_USER_NOMAKE;
		menuInit(C_menuN );									
		C_menuN2=0;
		break;
	case CLAN_USER_FLAG:
		C_menuN = CLAN_USER_ReadDATA;
		//menuInit(C_menuN );									
		C_menuN2=0;
		break;
		//ycy
	//=========클랜 구성원
	case CLAN_USER_DATA:
		switch(C_menuN2){			
		case 0:
			C_menuN = CLAN_USER;
			menuInit(C_menuN );												
			break;
		}
		break;
	case CLAN_USER:
		i = chkeckMenuN();
		memset(menuMouseOver,-1,sizeof(int)*30);
		switch(i) {
		case 2: //탈퇴
			menuMouseOver[2] = 1;
			break;		
		case 3: //도우말
			menuMouseOver[3] = 1;
			break;
		case 4: //나가기
			menuMouseOver[4] = 1;
			break;		
		default:
			memset(menuMouseOver,-1,sizeof(int)*30);
			break;
		}
		//ygy : mouse0()살릴것
		if(MOUSE0())			
		{	
			MOUSE0()=0;
			switch(i) 
			{
			case 2:
				C_menuN = CLAN_SECESSION;		//탈퇴로간다.
				menuInit(CLAN_SECESSION);
				C_menuN2 = -1;
				break;						
			case 3:
				//ygy : 헬프 관련함수 살릴것
				//ReadClanHelpScr(CLANUSERHELPINF);
				//LoadHelpImage();
				//C_menuN = CLAN_USER_HELP;
				LoadHelp(CLANUSER_HELP);
				C_menuN = CLAN_USER_HELP_CONTENT;
				
				menuInit(C_menuN);				
				break;
			case 4:
				C_menuN = 255;						//나가기 : 끝내는걸로간다.				
				menuInit(C_menuN );
				C_menuN2=-1;
				break;
			default:
				break;
			}
		}
		break;
	case CLAN_USER_NOMAKE:
		i = chkeckMenuN();
		switch(i) {
		case 0:
			menuMouseOver[0] =1;
			break;
		default:
			memset(menuMouseOver,-1,sizeof(int)*30);
			break;
		}
		//ygy : 꼭 살릴것
		if(MOUSE0())			
		{	
			MOUSE0()=0;
			switch(i) 
			{
			case 0:
				C_menuN = CLAN_USER;		//도움말로 돌아감.
				menuInit(C_menuN);
				break;				
			default:
				break;
			}
		}

		break;

	case CLAN_USER_WAREHOUSE:
		i = chkeckMenuN();
		switch(i) {
		case 0:
			menuMouseOver[0] =1;
			break;	
		default:
			memset(menuMouseOver,-1,sizeof(int)*30);
			break;
		}
		//ygy : 꼭 살릴것
		if(MOUSE0())			
		{	
			MOUSE0()=0;
			switch(i) 
			{
			case 0: //확인			
				C_menuN = CLAN_USER;
				menuInit(CLAN_USER);
				break;		
			default:
				break;
			}
		}
		break;

	case CLAN_SECESSION://탈퇴
		
		i = chkeckMenuN();
		switch(i) {
		case 2:
			menuMouseOver[2] =1;
			break;
		case 3:
			menuMouseOver[3] =1;
			break;
		default:
			memset(menuMouseOver,-1,sizeof(int)*30);
			break;
		}
		//ygy : 꼭 살릴것
		if(MOUSE0())			
		{	
			MOUSE0()=0;
			switch(i) 
			{
			case 3:		//탈퇴 N : 
				C_menuN = CLAN_USER;		//CLAN_USER로 돌아간다
				menuInit(C_menuN );
				
				break;
			case 2:					//탈퇴 Y
				//C_menuN2 = 0;		//ktj : php로 추방할 데이타를  집어넣어 실행하는 루틴으로 간다.
				C_menuN = CLAN_RECONF_SECESSION;		//CLAN_USER로 돌아간다
				menuInit(C_menuN );
				
			default:
				break;
			}
		}
		break;
	case CLAN_RECONF_SECESSION_LOADING:
		if(ANSdata.ret_val !=-1) C_menuN = CLAN_RECONF_SECESSION;
		break;
	case CLAN_RECONF_SECESSION://탈퇴

		switch(C_menuN2) {
		case 0:
			ANSdata.ret_val= -1;
			
			WebDB.Resume() ;		//웹db를 재개한다.
			
			WebDB.Secession(cldata.szID,cldata.gserver,cldata.ChaName,cldata.name);
#ifdef CLAN_DEBUG
			DebugMessage("CLAN_RECONF_SECESSION\277\241\274\255 WebDB.Secession(\276\306\300\314\265\360 : %s,\260\324\300\323\274\255\271\366 : %s,\304\263\270\257\305\315\300\314\270\247 : %s,\305\254\267\243\300\314\270\247 : %s);",cldata.szID,cldata.gserver,cldata.ChaName,cldata.name);
#endif
			C_menuN2=1;
			break;
		case 1:

			switch(ANSdata.ret_val) 
			{
			case -1:
				C_menuN = CLAN_RECONF_SECESSION_LOADING;
				menuInit(C_menuN);
				break;
			case 0://트랜잭션 오류
				WebDB.Suspend();
				//ygy : 살릴것
				//wsprintf(szErrorMsg, szTranzationError);
				C_menuN = CLAN_ERROR;
				wsprintf(szErrorMsg,"%s (34)",clanszError_Msg.szMsg);
				cClanReport.InitMsg(szErrorMsg);
				cClanReport.menuInit(C_menuN,clanszError_Msg.pos.x,clanszError_Msg.pos.y);
				CLAN_ERROR_ret=CLAN_USER;		//클랜유저메뉴로 간다.
				C_menuN2=-1;
#ifdef CLAN_DEBUG
			DebugMessage("CLAN_RECONF_SECESSION\277\241\274\255 \270\256\305\317\304\332\265\345 0 \306\256\267\242\300\350\274\307 \277\300\267\371");
#endif
				break;
			case 3:
			case 1://클랜원 탈퇴 성공
				WebDB.Suspend(); //웹db 중지
				//ygy : 살릴것				
				cldata.myPosition = CLANNO_REGESTER;				
				//wsprintf(szErrorMsg, szSecessionSuccess);
				wsprintf(szClanUserMsg,clanFinishSecessionMsg.szMsg,cldata.name,cldata.ChaName);
				strcpy(clanFinishSecessionMsg.szMsg,szClanUserMsg);
				//void CheckMsgPos(CLANMSG* clanmsg);
				//CheckMsgPos(&clanFinishSecessionMsg);

				C_menuN = CLAN_FINISH_SECESSION;
				menuInit(C_menuN);
				//CLAN_ERROR_ret=255;		//에러메뉴보여준후그냥끝낸다.
				C_menuN2=-1;

				void SetClanWon();
				SetClanWon();

				/*
				//=============================================================
				cldata.myPosition = CLANNO_REGESTER;
				ZeroMemory(cldata.name, sizeof(cldata.name)  );
				ZeroMemory(cldata.explanation, sizeof(cldata.explanation)  );
				ZeroMemory(cldata.ClanZang_Name, sizeof(cldata.ClanZang_Name)  );
				ZeroMemory(cldata.ClanMark, sizeof(cldata.ClanMark)  );
				cldata.intClanMark = 0;
				ZeroMemory(cldata.PFlag, sizeof(cldata.PFlag));
				#ifdef USE_PROSTONTALE		//=========== 포리스턴소스인경우
				if(cldata.hClanMark) {
					cldata.hClanMark->Release(); 
					cldata.hClanMark = NULL;

				}
				if(cldata.hClanMark16){
					cldata.hClanMark16->Release(); 
					cldata.hClanMark16 = NULL;
				}
				#endif
				//=============================================================
				*/
#ifdef CLAN_DEBUG
			DebugMessage("CLAN_RECONF_SECESSION\277\241\274\255 \305\273\305\360\274\272\260\370(%d)",ANSdata.ret_val);
#endif

#ifdef VOICE_CHAT
				if(bCheckClan)
			    {
				   g_CtrlProcess.SetSelectCha(cldata.szID,cldata.ChaName,
					   NULL, 0, cldata.myPosition);
				   //bCheckClan = FALSE;

			    }
#endif


				break;
			case 2://클랜이 존재하지 않거나 클랜 장이 아닌경우 또는 클랜원이 아닌경우
				WebDB.Suspend(); //웹db 중지
				//ygy : 살릴것
				C_menuN = CLAN_ERROR;
				wsprintf(szErrorMsg,"%s (35)",clanszError_Msg.szMsg);
				cClanReport.InitMsg(szErrorMsg);
				cClanReport.menuInit(C_menuN,clanszError_Msg.pos.x,clanszError_Msg.pos.y);
				//CLAN_ERROR_ret=255;		//에러메뉴보여준후그냥끝낸다.
				C_menuN2=-1;
#ifdef CLAN_DEBUG
			DebugMessage("CLAN_RECONF_SECESSION\277\241\274\255 \305\254\267\243\300\314 \301\270\300\347\307\317\301\366 \276\312\260\305\263\252 \305\254\267\243 \300\345\300\314 \276\306\264\321\260\346\277\354 \266\307\264\302 \305\254\267\243\277\370\300\314 \276\306\264\321\260\346\277\354(%d)",ANSdata.ret_val);
#endif

				break;
			/*case 3://탈퇴는 성공했으나 클랜회원수가 6명이하로 떨어졌을 경우(LimitDate 설정)
				WebDB.Suspend(); //웹db 중지
				//ygy : 살릴것
				wsprintf(szErrorMsg, szSecession6DownMsg);
				C_menuN = CLAN_ERROR;
				cClanReport.InitMsg(szSecession6DownMsg);
				cClanReport.menuInit(C_menuN);

				//=============================================================

				void SetClanWon();
				SetClanWon();


				cldata.myPosition = CLANNO_REGESTER;
				ZeroMemory(cldata.name, sizeof(cldata.name)  );
				ZeroMemory(cldata.explanation, sizeof(cldata.explanation)  );
				ZeroMemory(cldata.ClanZang_Name, sizeof(cldata.ClanZang_Name)  );
				ZeroMemory(cldata.ClanMark, sizeof(cldata.ClanMark)  );
				cldata.intClanMark = 0;
				ZeroMemory(cldata.PFlag, sizeof(cldata.PFlag));
				//=============================================================

				
				CLAN_ERROR_ret=255;		//에러메뉴보여준후그냥끝낸다.
				C_menuN2=-1;
				break;*/
			case 4://탈퇴하려는 자가 현재 클랜의 클랜장일 경우(오류)
				WebDB.Suspend(); //웹db 중지
				//ygy : 살릴것
				C_menuN = CLAN_ERROR;
				wsprintf(szErrorMsg,"%s (36)",clanszError_Msg.szMsg);
				cClanReport.InitMsg(szErrorMsg);
				cClanReport.menuInit(C_menuN,clanszError_Msg.pos.x,clanszError_Msg.pos.y);
				CLAN_ERROR_ret=255;		//에러메뉴보여준후그냥끝낸다.
				C_menuN2=-1;
#ifdef CLAN_DEBUG
			DebugMessage("CLAN_RECONF_SECESSION\277\241\274\255 \305\273\305\360\307\317\267\301\264\302 \300\332\260\241 \307\366\300\347 \305\254\267\243\300\307 \305\254\267\243\300\345\300\317 \260\346\277\354(%d)",ANSdata.ret_val);
#endif
				break;
			case 100: //엉뚱한 값
				WebDB.Suspend(); //웹db 중지
				//ygy : 살릴것
				//wsprintf(szErrorMsg, sz100Error);
				C_menuN = CLAN_ERROR;
				wsprintf(szErrorMsg,"%s (37)",clanszError_Msg.szMsg);
				cClanReport.InitMsg(szErrorMsg);
				cClanReport.menuInit(C_menuN,clanszError_Msg.pos.x,clanszError_Msg.pos.y);
				CLAN_ERROR_ret=255;		//에러메뉴보여준후그냥끝낸다.
				C_menuN2=-1;
#ifdef CLAN_DEBUG
			DebugMessage("CLAN_RECONF_SECESSION\277\241\274\255 \270\256\305\317\304\332\265\345 100");
#endif
				break;
			case ISAO_ERR_TCP_CONNECT:
				WebDB.Suspend();
				//ygy : 살릴것
				//wsprintf(szErrorMsg, szServerConnectErrorMsg);
				C_menuN = CLAN_ERROR;
				wsprintf(szErrorMsg,"%s (38)",clanszError_Msg.szMsg);
				cClanReport.InitMsg(szErrorMsg);
				cClanReport.menuInit(C_menuN,clanszError_Msg.pos.x,clanszError_Msg.pos.y);
#ifdef CLAN_DEBUG
			DebugMessage("CLAN_RECONF_SECESSION\277\241\274\255 \270\256\305\317\304\332\265\345 1004");
#endif
				break;
			default:
				if(ANSdata.ret_val != -1) {		//전혀엉뚱한 답이 와서 에러가 났다.
					WebDB.Suspend();

					//ygy : 살릴것
					//wsprintf(szErrorMsg, szDefaultError);
					C_menuN = CLAN_ERROR;
					wsprintf(szErrorMsg,"%s (39)",clanszError_Msg.szMsg);
					cClanReport.InitMsg(szErrorMsg);
					cClanReport.menuInit(C_menuN,clanszError_Msg.pos.x,clanszError_Msg.pos.y);
					CLAN_ERROR_ret=CLAN_USER;		//클랜유저메뉴로 간다.
					C_menuN2=-1;
#ifdef CLAN_DEBUG
			DebugMessage("CLAN_RECONF_SECESSION\277\241\274\255 \265\360\306\372\306\256\277\241\267\257");
#endif
				}
				break;
			}
			break;
		}

		i = chkeckMenuN();
		switch(i) {
		case 2:
			menuMouseOver[2] =1;
			break;
		case 3:
			menuMouseOver[3] =1;
			break;
		default:
			memset(menuMouseOver,-1,sizeof(int)*30);
			break;
		}
		//ygy : 꼭 살릴것
		if(MOUSE0())			
		{	
			MOUSE0()=0;
			switch(i) 
			{
			case 3:		//탈퇴 N : 
				C_menuN = CLAN_USER;		//CLAN_USER로 돌아간다
				menuInit(C_menuN );
				
				break;
			case 2:					//탈퇴 Y
				C_menuN2 = 0;		//ktj : php로 추방할 데이타를  집어넣어 실행하는 루틴으로 간다.		
				
			default:
				break;
			}
		}
		break;
	case CLAN_FINISH_SECESSION:  //도움말
		i = chkeckMenuN();
		switch(i) {
		case 3:
			menuMouseOver[3] =1;
			break;
		default:
			memset(menuMouseOver,-1,sizeof(int)*30);
			break;

		}
		//ygy : 꼭 살릴것
		if(MOUSE0())			
		{	
			MOUSE0()=0;
			switch(i) 
			{
			case 3:			//확인	
				//=============================================================
				cldata.myPosition = CLANNO_REGESTER;
				ZeroMemory(cldata.name, sizeof(cldata.name)  );
				ZeroMemory(cldata.explanation, sizeof(cldata.explanation)  );
				ZeroMemory(cldata.ClanZang_Name, sizeof(cldata.ClanZang_Name)  );
				ZeroMemory(cldata.ClanMark, sizeof(cldata.ClanMark)  );
				cldata.intClanMark = 0;
				ZeroMemory(cldata.PFlag, sizeof(cldata.PFlag));
				ZeroMemory(szChatClanWon,sizeof(szChatClanWon));
				#ifdef USE_PROSTONTALE		//=========== 포리스턴소스인경우
				if(cldata.hClanMark) {
					cldata.hClanMark->Release(); 
					cldata.hClanMark = NULL;

				}
				if(cldata.hClanMark16){
					cldata.hClanMark16->Release(); 
					cldata.hClanMark16 = NULL;
				}
				#endif
				//=============================================================
				C_menuN =CLAN_AllEnd;;
				menuInit(C_menuN);
				break;				
			default:
				break;
			}
		}
	case CLAN_USER_HELP:
		i = chkeckMenuN();
		switch(i) {
		case 0:
			menuMouseOver[0] = 1;
			break;		
		case 2:	
			menuMouseOver[2] = 1;
			break;
		case 3:	
			menuMouseOver[3] = 1;
			break;
		case 4:	
			menuMouseOver[4] = 1;
			break;
		case 5:	
			menuMouseOver[5] = 1;
			break;					
		default:
			memset(menuMouseOver,-1,sizeof(int)*30);
			break;
		}
		//ygy : 꼭 살릴것
		if(MOUSE0())			
		{	
			
			switch(i) 
			{
			case 0:
				//ygy : 헬프관련 함수 살릴꺼
				DestroyClanHelpIcon();
				C_menuN = CLAN_USER;	//전단계로 돌아간다
				menuInit(C_menuN );
				//ygy : 꼭 살릴것
				MOUSE0()=0;
				break;	
			case 2:				
			case 3:				
			case 4:				
			case 5:
				//ygy : 살릴것
				//g_HelpIconNumber = 0;				
				//g_HelpIconNumber = i+nScl_Bar-2;
				//ycy : 헬프관련 함수 살릴껏
				LoadHelp(ClanHelpIcon[i+nScl_Bar-2].TextFilePath);
				C_menuN = CLAN_USER_HELP_CONTENT;		//도움말로 돌아감.
				menuInit(C_menuN);
				//ygy : 꼭 살릴것
				MOUSE0()=0;
				break;

			case 6:
				nScl_Bar = tscr.topScrollLineNum(pCursorPos.y);
				//ktj : 스크롤 아이콘 위치재설정
				scrIconPos.top =  pCursorPos.y-8;//BackStartPos.y + SCROOBAR_TOP;
				break;
			
			default:
				//ygy : 꼭 살릴것
				MOUSE0()=0;
				break;
			}
		}
		break;
	case CLAN_USER_HELP_CONTENT:
			i = chkeckMenuN();
			switch(i) {
			case 0:
				menuMouseOver[0] =1;
				break;					
			default:
				memset(menuMouseOver,-1,sizeof(int)*30);
				break;
			}
			//ygy : 꼭 살릴것
			if(TJwheel != 0) {			//ktj : wheel>0 휠 앞으로 굴리기  wheel<0 휠 뒤로 굴리기
				scrIconPos.top = tscr.ret_WHEELpoint(TJwheel , scrIconPos.top);
				nScl_Bar = tscr.topScrollLineNum(scrIconPos.top);
				TJwheel = 0;
			}
			//ygy : 꼭 살릴것
			if(MOUSE0())			
			{	
				switch(i) 
				{
				case 0:
					C_menuN = CLAN_USER;		//도움말로 돌아감.
					menuInit(C_menuN);
					//ygy : 꼭 살릴것
					MOUSE0()=0;
					//ktj 스크롤관련
					tscr.Init(0);

					break;	
				case 5: //스크롤바 버턴 움직이기
					nScl_Bar = tscr.topScrollLineNum(pCursorPos.y);
					//ktj : 스크롤 아이콘 위치재설정
					scrIconPos.top =  pCursorPos.y-8;//BackStartPos.y + SCROOBAR_TOP;
					break;
				default:
					//ygy : 꼭 살릴것
					MOUSE0()=0;
					break;
				}
			}		
		break;
	
	case CLANUSER_RELEASE_FLAG:
		switch(C_menuN2){
			case 0:
				ANSdata.ret_val= -1;			

				WebDB.Resume() ;		//웹db를 재개한다.
				WebDB.isPFlag(cldata.szID,cldata.name,cldata.ChaName,cldata.gserver,"100","1");		
#ifdef CLAN_DEBUG
				DebugMessage("CLANUSER_RELEASE_FLAG\277\241\274\255 WebDB.isPFlag(\276\306\300\314\265\360,\305\254\267\243\300\314\270\247 : %s,\304\263\270\257\305\315\300\314\270\247 : %s,\260\324\300\323\274\255\271\366 : %s,100,1)",cldata.szID,cldata.name,cldata.ChaName,cldata.gserver);
#endif
				C_menuN2=1;
				break;
			case 1:
				switch(ANSdata.ret_val){
					case 0://플래그 삽입 실패
						WebDB.Suspend();
						C_menuN = CLAN_ERROR;
						wsprintf(szErrorMsg,"%s (40)",clanszError_Msg.szMsg);
						cClanReport.InitMsg(szErrorMsg);
						cClanReport.menuInit(C_menuN,clanszError_Msg.pos.x,clanszError_Msg.pos.y);
						
						C_menuN2=-1;
#ifdef CLAN_DEBUG
				DebugMessage("CLANUSER_RELEASE_FLAG\277\241\274\255 \307\303\267\241\261\327 \273\360\300\324\275\307\306\320 \270\256\305\317\304\332\265\345 0");
#endif
						break;
					case 1: //플래그 삽입 성공

						WebDB.Suspend();
						C_menuN2=-1;
						C_menuN = CLAN_AllEnd;						


						//=============================================================
						/*cldata.myPosition = CLANNO_REGESTER;
						ZeroMemory(cldata.name, sizeof(cldata.name)  );
						ZeroMemory(cldata.explanation, sizeof(cldata.explanation)  );
						ZeroMemory(cldata.ClanZang_Name, sizeof(cldata.ClanZang_Name)  );
						ZeroMemory(cldata.ClanMark, sizeof(cldata.ClanMark)  );
						cldata.intClanMark = 0;
						ZeroMemory(cldata.PFlag, sizeof(cldata.PFlag));
						#ifdef USE_PROSTONTALE		//=========== 포리스턴소스인경우
						if(cldata.hClanMark) {
							cldata.hClanMark->Release(); 
							cldata.hClanMark = NULL;

						}
						if(cldata.hClanMark16){
							cldata.hClanMark16->Release(); 
							cldata.hClanMark16 = NULL;
						}
						#endif
						*/
#ifdef CLAN_DEBUG
				DebugMessage("CLANUSER_RELEASE_FLAG\277\241\274\255 \307\303\267\241\261\327 \273\360\300\324\274\272\260\370 1");
#endif

						break;
					default:
						if(ANSdata.ret_val != -1) {		//전혀엉뚱한 답이 와서 에러가 났다.
							WebDB.Suspend();
							C_menuN = CLAN_ERROR;
							wsprintf(szErrorMsg,"%s (41)",clanszError_Msg.szMsg);
							cClanReport.InitMsg(szErrorMsg);
							cClanReport.menuInit(C_menuN,clanszError_Msg.pos.x,clanszError_Msg.pos.y);
							C_menuN2=-1;
#ifdef CLAN_DEBUG
				DebugMessage("CLANUSER_RELEASE_FLAG\277\241\274\255 \265\360\306\372\306\256\277\241\267\257");
#endif
						}
						break;
				}
		}
		break;
	case ISPFLAG_USER:
		switch(C_menuN2){
			case 0:
				ANSdata.ret_val= -1;			

				WebDB.Resume() ;		//웹db를 재개한다.
				WebDB.isPFlag(cldata.szID,cldata.name,cldata.ChaName,cldata.gserver,cldata.PFlag,"1");		
				C_menuN2=1;
#ifdef CLAN_DEBUG
				DebugMessage("ISPFLAG_USER\277\241\274\255 WebDB.isPFlag(\276\306\300\314\265\360 : %s,\305\254\267\243\300\314\270\247 : %s,\304\263\270\257\305\315\300\314\270\247 : %s,\260\324\300\323\274\255\271\366 : %s,\307\303\267\241\261\327 : %s,1)",cldata.szID,cldata.name,cldata.ChaName,cldata.gserver,cldata.PFlag);
#endif
				break;
			case 1:
				switch(ANSdata.ret_val){
					case 0://플래그 삽입 실패
						WebDB.Suspend();
						C_menuN = CLAN_ERROR;
						wsprintf(szErrorMsg,"%s (42)",clanszError_Msg.szMsg);
						cClanReport.InitMsg(szErrorMsg);
						cClanReport.menuInit(C_menuN,clanszError_Msg.pos.x,clanszError_Msg.pos.y);
						C_menuN2=-1;
#ifdef CLAN_DEBUG
				DebugMessage("ISPFLAG_USER\277\241\274\255 \307\303\267\241\261\327 \273\360\300\324\275\307\306\320 \270\256\305\317\304\332\265\345 0");
#endif
						break;
					case 1: //플래그 삽입 성공
						WebDB.Suspend();
						C_menuN2=-1;
						C_menuN = CLAN_USER;
						menuInit(C_menuN);
#ifdef CLAN_DEBUG
				DebugMessage("ISPFLAG_USER\277\241\274\255 \307\303\267\241\261\327 \273\360\300\324\274\272\260\370");
#endif
						break;
					default:
						if(ANSdata.ret_val != -1){
							WebDB.Suspend();
							C_menuN2=-1;
							C_menuN = CLAN_ERROR;
							wsprintf(szErrorMsg,"%s (43)",clanszError_Msg.szMsg);
							cClanReport.InitMsg(szErrorMsg);
							cClanReport.menuInit(C_menuN,clanszError_Msg.pos.x,clanszError_Msg.pos.y);
#ifdef CLAN_DEBUG
				DebugMessage("ISPFLAG_USER\277\241\274\255 \265\360\306\372\306\256\277\241\267\257");
#endif
						}
						break;
				}
		}
		break;
	case CLANUSER_MAKE_FIRSTMESSAGE:  //처음 클랜이 만들어졌을때 한번 보여줌
		i = chkeckMenuN();
		switch(i) {
		case 0:
			menuMouseOver[0] =1;
			break;
		default:
			memset(menuMouseOver,-1,sizeof(int)*30);
			break;
		}
		//ygy : 살려야함
		if(MOUSE0())			
		{	
			MOUSE0()=0;
			switch(i) 
			{
			case 0:
				C_menuN = ISPFLAG_USER;		//도움말로 돌아감.
				menuInit(C_menuN);
				C_menuN2 = 0;
				break;				
			default:
				break;
			}
		}
		break;
	case CLANUSER_DOWN6_CLANWON_MESSAGE:
		i = chkeckMenuN();
		switch(i) {
		case 0:
			menuMouseOver[0] =1;
			break;
		default:
			memset(menuMouseOver,-1,sizeof(int)*30);
			break;
		}
		//ygy : 살려야함
		if(MOUSE0())			
		{	
			MOUSE0()=0;
			switch(i) 
			{
			case 0:
				C_menuN = CLAN_USER;		//도움말로 돌아감.
				menuInit(C_menuN);
				C_menuN2 = 0;
				break;				
			default:
				break;
			}
		}
		break;
		
	case CLANUSER_DOWN6_CLANWON:     //클랜원이 6명 이하
		i = chkeckMenuN();
		switch(i) {
		case 0:
			menuMouseOver[0] =1;
			break;
		default:
			memset(menuMouseOver,-1,sizeof(int)*30);
			break;
		}
		//ygy : 살려야함
		if(MOUSE0())			
		{	
			MOUSE0()=0;
			switch(i) 
			{
			case 0:
				C_menuN = ISPFLAG_USER;		//도움말로 돌아감.
				menuInit(C_menuN);
				C_menuN2 = 0;
				break;				
			default:
				break;
			}
		}
		break;

	case CLAN_USER_ReadDATA:

		switch(C_menuN2) {
		case 0:
			ANSdata.ret_val= -1;
			
			WebDB.Resume() ;		//웹db를 재개한다.
			//클랜의 멤버데이타를 읽는다.
			WebDB.ReadClan(cldata.szID,cldata.gserver,cldata.ClanZang_Name);
#ifdef CLAN_DEBUG
			DebugMessage("CLAN_USER_ReadDATA\277\241\274\255 WebDB.ReadClan(\276\306\300\314\265\360 : %s,\260\324\300\323\274\255\271\366 : %s,\304\263\270\257\305\315\300\314\270\247 : %s)",cldata.szID,cldata.gserver,cldata.ClanZang_Name);
#endif
			C_menuN2=1;
			break;
		case 1:
			
			if(ANSdata.ret_val != -1)
				ANSdata.ret_val = ANSdata.ret_val;

			switch(ANSdata.ret_val) 
			{
				//ygy
			case 0://가입한 클랜이 없음
				WebDB.Suspend();
				//wsprintf(szErrorMsg, "가입한 클랜이 없음");
				C_menuN = CLAN_ERROR;
				wsprintf(szErrorMsg,"%s (44)",clanszError_Msg.szMsg);
				cClanReport.InitMsg(szErrorMsg);
				cClanReport.menuInit(C_menuN,clanszError_Msg.pos.x,clanszError_Msg.pos.y);
				
				CLAN_ERROR_ret=255;		//에러메뉴보여준후그냥끝낸다.
				C_menuN2=-1;
#ifdef CLAN_DEBUG
			DebugMessage("CLAN_USER_ReadDATA\277\241\274\255 \260\241\300\324\307\321 \305\254\267\243\300\314 \276\370\300\275 \270\256\305\317\304\332\265\345 0");
#endif
				break;	
			case 1://정상적으로 클랜멤보데이타받아 파싱
				WebDB.Suspend();
				ParsigClanWon(ANSdata.ret_Buf);
				/*if(strcmp(cldata.stats,"0") == 0)
				{
					C_menuN = CLAN_USER_NOMAKE;	//stats가 0이면 6명이하이므로 더 모집하라는메시지보냄
					menuInit(C_menuN );
					C_menuN2=-1;
				}
				else
				{
					C_menuN = CLAN_USER;	//바로 클랜칲메뉴로 간다.
					menuInit(C_menuN );									
					C_menuN2=-1;
				}*/


//npc접근했을때 클랜칩 이임,부클랜칩 임명,해임 관련메세지 출력.......
						if(cldata.nKFlag == 1) //클랜칩 위임  확인 : 3
						{
							cldata.nKFlag_Count = 1;
							C_menuN = CLANUSER_LEAVECLAN_MESSAGE;
							menuInit(C_menuN);
							break;
						}
						else if(cldata.nKFlag == 16) //부클랜장 해임  확인 32
						{
							cldata.nKFlag_Count = 1;
							C_menuN = CLANUSER_SUBCHIP_RELEASEMSG;
							
							menuInit(C_menuN);
							break;
						}
						else if(cldata.nKFlag == 4) //부클랜장 임명 확인 8
						{
							cldata.nKFlag_Count = 1;
							C_menuN = CLANUSER_SUBCHIP_APPOINTMSG;
							menuInit(C_menuN);
							break;
						}
						else if(cldata.nKFlag == 17)//칩이임,부침해임에서 
						{
							cldata.nKFlag_Count = 1;
							C_menuN = CLANUSER_LEAVECLAN_MESSAGE;
							menuInit(C_menuN);
							break;
						}
						else if(cldata.nKFlag == 19) //칩이임,부침해임에서 칩이임만 했을경우
						{
							cldata.nKFlag_Count = 1;
							C_menuN = CLANUSER_SUBCHIP_RELEASEMSG;
							menuInit(C_menuN);
							break;
						}
						else if(cldata.nKFlag == 5) //칩이임,부침임명
						{
							cldata.nKFlag_Count = 1;
							C_menuN = CLANUSER_LEAVECLAN_MESSAGE;
							menuInit(C_menuN);
							break;
						}
						else if(cldata.nKFlag == 7) //칩이임과 부침임명에서 칩이임만 확앴으핸
						{
							cldata.nKFlag_Count = 1;
							C_menuN = CLANUSER_SUBCHIP_APPOINTMSG;
							menuInit(C_menuN);
							break;
						}
						else if(cldata.nKFlag == 20) //부칩해임 부칩임명
						{

							if(cldata.SubChip[0] ==0)
							{
								bSubChip = FALSE;
								cldata.nKFlag_Count = 1;
								C_menuN = CLANUSER_SUBCHIP_APPOINTMSG;
								menuInit(C_menuN);
							}
							else
							{
								bSubChip = TRUE;
								cldata.nKFlag_Count = 1;
								C_menuN = CLANUSER_SUBCHIP_RELEASEMSG;
								menuInit(C_menuN);
							}
							break;
						}
						else if(cldata.nKFlag == 28)
						{
							cldata.nKFlag_Count = 1;
							C_menuN = CLANUSER_SUBCHIP_RELEASEMSG;
							menuInit(C_menuN);
							break;
						}
						else if(cldata.nKFlag == 52)//부칩해임 부칩임명에서 부칩해임만 확인했을 경우
						{
							cldata.nKFlag_Count = 1;
							C_menuN = CLANUSER_SUBCHIP_APPOINTMSG;
							menuInit(C_menuN);
							break;
						}
						else if(cldata.nKFlag == 21) //칩이임,부침임명,부칩해임
						{
							cldata.nKFlag_Count = 1;
							C_menuN = CLANUSER_LEAVECLAN_MESSAGE;
							menuInit(C_menuN);
							break;
						}	
						else if(cldata.nKFlag == 23) //클랜칩이임 부클랜칩 해임 부클랜칩 임명에서 (클랜칩 이임만 확인)
						{
							cldata.nKFlag_Count = 1;
							C_menuN = CLANUSER_SUBCHIP_RELEASEMSG;
							menuInit(C_menuN);
							break;
						}
						else if(cldata.nKFlag == 55) //클랜칩이임 부클랜칩 해임 부클랜칩 임명에서 (클랜칩 이임, 부클랜칩 해임만 확인했을 경우)
						{
							cldata.nKFlag_Count = 1;
							C_menuN = CLANUSER_SUBCHIP_APPOINTMSG;
							menuInit(C_menuN);
							break;
						}
						else{}




				//if(strcmp(cldata.LeaveClanFlag,"200") == 0)
			//	{
				//	C_menuN = CLANUSER_LEAVECLAN_MESSAGE;
				//	menuInit(C_menuN );
				//	break;
				//}
				if((strcmp(cldata.stats,"0") == 0)&&(strcmp(cldata.PFlag,"0") == 0))
				{
					C_menuN = CLAN_USER_NOMAKE;	//stats가 0이면 6명이하이므로 더 모집하라는메시지보냄
					menuInit(C_menuN );
					C_menuN2=-1;
				}
				else if((strcmp(cldata.stats,"1") == 0)&&(strcmp(cldata.PFlag,"0") == 0)) //클랜 결성되고 처음 보여주는 메세지
				{
					strcpy(cldata.PFlag,"2");
					wsprintf(string,clanMakeFinishClanWon.szMsg,cldata.name);
					strcpy(clanMakeFinishClanWon.szMsg,string);
					//void CheckMsgPos(CLANMSG* clanmsg);
					CheckMsgPos(&clanMakeFinishClanWon,BOX_MAX_WIDTH,0);					
					C_menuN = CLANUSER_MAKE_FIRSTMESSAGE;	//stats가 0이면 6명이하이므로 더 모집하라는메시지보냄

					menuInit(C_menuN );
					C_menuN2=-1;
				}				
				//else if((strcmp(cldata.stats,"0") == 0)&&(strcmp(cldata.PFlag,"2") == 0)) //클랜원이 6명이라로 떨어졌을 경우
				//{	
				//	strcpy(cldata.PFlag,"3");
				//	C_menuN = CLANUSER_DOWN6_CLANWON;	
				//	menuInit(C_menuN );									
				//	C_menuN2=-1;
				//}
				else if((strcmp(cldata.stats,"1") == 0)&&(strcmp(cldata.PFlag,"2") == 0)&&(nClanWon_Num <5))
				{
					C_menuN = CLANUSER_DOWN6_CLANWON_MESSAGE;	
					menuInit(C_menuN );									
					C_menuN2=-1;
				}
				//else if((strcmp(cldata.stats,"1") == 0)&&(strcmp(cldata.PFlag,"3") == 0))
				//{
				//	strcpy(cldata.PFlag,"4");
				//	C_menuN = ISPFLAG_USER;		//도움말로 돌아감.
				//	menuInit(C_menuN);
				//	C_menuN2 = 0;					
				//}				
				//else if((strcmp(cldata.stats,"0") == 0)&&(strcmp(cldata.PFlag,"4") == 0))
				//{
				//	strcpy(cldata.PFlag,"3");
				//	C_menuN = CLANUSER_DOWN6_CLANWON;	
				//	menuInit(C_menuN );									
				//	C_menuN2=-1;
				//}
				else // 1:1 or 1:3
				{
					C_menuN = CLAN_USER;	//바로 클랜칲메뉴로 간다.
					menuInit(C_menuN );									
					C_menuN2=-1;
				}
#ifdef CLAN_DEBUG
			DebugMessage("CLAN_USER_ReadDATA\277\241\274\255 \301\244\273\363\300\373\300\270\267\316 \303\263\270\256");
#endif
				

				break;
			case 100:
				WebDB.Suspend();
				//ygy : 살려야함
				C_menuN = CLAN_ERROR;
				wsprintf(szErrorMsg,"%s (45)",clanszError_Msg.szMsg);
				cClanReport.InitMsg(szErrorMsg);
				cClanReport.menuInit(C_menuN,clanszError_Msg.pos.x,clanszError_Msg.pos.y);
				CLAN_ERROR_ret=CLAN_AllEnd;		//에러메뉴보여준후그냥끝낸다.			
				C_menuN2=-1;
#ifdef CLAN_DEBUG
			DebugMessage("CLAN_USER_ReadDATA\277\241\274\255 \270\256\305\317\304\332\265\345100");
#endif
				break;
			case ISAO_ERR_TCP_CONNECT:
				WebDB.Suspend();
				//ygy : 살려야함
				C_menuN = CLAN_ERROR;
				wsprintf(szErrorMsg,"%s (46)",clanszError_Msg.szMsg);
				cClanReport.InitMsg(szErrorMsg);
				cClanReport.menuInit(C_menuN,clanszError_Msg.pos.x,clanszError_Msg.pos.y);
#ifdef CLAN_DEBUG
			DebugMessage("CLAN_USER_ReadDATA\277\241\274\255 \274\255\271\366\277\241 \301\242\274\323 \307\322\274\366 \276\370\300\275");
#endif
				break;
			
			default:
				if(ANSdata.ret_val != -1) {		//전혀엉뚱한 답이 와서 에러가 났다.
					WebDB.Suspend();
					C_menuN = CLAN_AllEnd;				//클랜결성못하니 돌아가라고 하는메뉴로 간다.
					menuInit(C_menuN );

					C_menuN2=-1;
#ifdef CLAN_DEBUG
			DebugMessage("CLAN_USER_ReadDATA\277\241\274\255 \265\360\306\372\306\256\277\241\267\257");
#endif
				}
				break;
			}
			break;
		}
	}//end switch
	
	
}



//*****************************************************************************
void CLANEntity_user::RenderMain()
{
	if(life ==0) return;

}




//***************************************************************************************
void CLANEntity_user::menuInit(int num)
{
	int bx = 0, by = 0;
	int      nNext = 0,i,nIconCount = 0,nIconPos = 0,nAdd=0,nWidth = 0;
	int      nFirstIconSave = 0;
	
	switch(num){		
		case CLANUSER_SUBCHIP_APPOINTMSG://클랜부침이 임명되다고 한번 보여줌
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
			menuPos[1].left = BackStartPos.x + SubChipAppointEnd.pos.x;
			menuPos[1].top = BackStartPos.y + SubChipAppointEnd.pos.y;

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
		
		case CLANUSER_SUBCHIP_RELEASEMSG://클랜부침이 해임되다고 한번 보여줌
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
			menuPos[1].left = BackStartPos.x + SubChipReleaseEnd.pos.x;
			menuPos[1].top = BackStartPos.y + SubChipReleaseEnd.pos.y;

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
		
		case CLANUSER_LEAVECLAN_MESSAGE:
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
			menuPos[1].left = BackStartPos.x + LeaveClanMessage.pos.x;
			menuPos[1].top = BackStartPos.y + LeaveClanMessage.pos.y;

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
		
		case	CLANUSER_AUTORELEASE://자동해체
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
			menuPos[1].left = BackStartPos.x + 55;
			menuPos[1].top = BackStartPos.y + 70;

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
		case CLANUSER_REMOVE ://클랜원이 추방당했을경우
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
			menuPos[1].left = BackStartPos.x + clanClanUserRemove.pos.x;
			menuPos[1].top = BackStartPos.y + clanClanUserRemove.pos.y;

			#ifdef USE_PROSTONTALE		//=========== 포리스턴소스인경우
			bx = 48;
			by = 23;
			#else
			bx = 48;
			by = 23;
			#endif
			menuPos[2].left = BackStartPos.x + BUTTON1_BOX_X;		//확인
			menuPos[2].top = BackStartPos.y + BUTTON1_BOX_Y;
			menuPos[2].right = menuPos[2].left + bx;
			menuPos[2].bottom = menuPos[2].top + by;
			break;
	case CLAN_USER: //클랜 구성원
		menuSu = 13;
#ifdef USE_PROSTONTALE		//=========== 포리스턴소스인경우
			if(hMark == NULL) hMark = GetClanMark();
			
#else
			if(hMark == NULL) hMark = GetClanMark();
#endif
		
		#ifdef USE_PROSTONTALE		//=========== 포리스턴소스인경우
		bx = 208;
		by = 29;
		#else		
		bx = 208;
		by = 29;
		#endif

		menuPos[0].left = BackStartPos.x + CLANNAME_X;		//클랜 이름
		menuPos[0].top =  BackStartPos.y + CLANNAME_Y;
		menuPos[0].right = menuPos[0].left + bx;
		menuPos[0].bottom = menuPos[0].top + by;

		#ifdef USE_PROSTONTALE		//=========== 포리스턴소스인경우
		bx = 69;
		by = 59;
		#else
		bx = 69;
		by = 59;
		#endif

		menuPos[1].left = BackStartPos.x +CLANMARKBOX_X;		//마크
		menuPos[1].top	 = BackStartPos.y +CLANMARKBOX_Y;
		menuPos[1].right = menuPos[1].left + bx;
		menuPos[1].bottom = menuPos[1].top + by;

		#ifdef USE_PROSTONTALE		//=========== 포리스턴소스인경우
		bx = 32;
		by = 32;
		#else
		bx = 32;
		by = 32;
		#endif

		menuPos[6].left = BackStartPos.x +CLANMARK_X;		//마크
		menuPos[6].top	 = BackStartPos.y +CLANMARK_Y;
		menuPos[6].right = menuPos[6].left + bx;
		menuPos[6].bottom = menuPos[6].top + by;


		#ifdef USE_PROSTONTALE		//=========== 포리스턴소스인경우
		bx = CLANTEXT_IMAGE_WIDTH;
		by = CLANTEXT_IMAGE_HEIGHT;
		#else
		
		bx = CLANTEXT_IMAGE_WIDTH;
		by = CLANTEXT_IMAGE_HEIGHT;
		#endif

		menuPos[2].left = BackStartPos.x +CLANUSER_SECESSION_X;		//탈퇴
		menuPos[2].top = BackStartPos.y +CLANUSER_SECESSION_Y;
		menuPos[2].right = menuPos[2].left + bx;
		menuPos[2].bottom = menuPos[2].top + by;

	

		#ifdef USE_PROSTONTALE		//=========== 포리스턴소스인경우
		bx = CLANTEXT_IMAGE_WIDTH;
		by = CLANTEXT_IMAGE_HEIGHT;
		#else		
		bx = CLANTEXT_IMAGE_WIDTH;
		by = CLANTEXT_IMAGE_HEIGHT;
		#endif

		menuPos[3].left = BackStartPos.x +CLANUSER_HELP_X;		//도움말
		menuPos[3].top = BackStartPos.y +CLANUSER_HELP_Y;//180;
		menuPos[3].right = menuPos[3].left + bx;
		menuPos[3].bottom = menuPos[3].top + by;

		#ifdef USE_PROSTONTALE		//=========== 포리스턴소스인경우
		bx = 32;
		by = 16;
		#else
		bx = 32;
		by = 16;
		#endif

		menuPos[4].left = BackStartPos.x + BUTTON1_X;		//나가기
		menuPos[4].top = BackStartPos.y +BUTTON1_Y;
		menuPos[4].right = menuPos[4].left + bx;
		menuPos[4].bottom = menuPos[4].top + by;

		#ifdef USE_PROSTONTALE		//=========== 포리스턴소스인경우
		bx = 64;
		by = 16;
		#else
		bx = 64;
		by = 16;
		#endif

		menuPos[5].left = BackStartPos.x + BUTTON1_BOX_X;		//나가기 테드리 박스
		menuPos[5].top = BackStartPos.y +BUTTON1_BOX_Y;
		menuPos[5].right = menuPos[5].left + bx;
		menuPos[5].bottom = menuPos[5].top + by;

		#ifdef USE_PROSTONTALE		//=========== 포리스턴소스인경우
		bx = 64;
		by = 16;
		#else
		bx = 64;
		by = 16;
		#endif

		menuPos[7].left = BackStartPos.x + CLANNAMETEXT_X;		//클랜이름
		menuPos[7].top =  BackStartPos.y +CLANNAMETEXT_Y;
		menuPos[7].right = 0;//menuPos[7].left + bx;
		menuPos[7].bottom = 0;//menuPos[7].top + by;

		//작은 버턴 영역
		#ifdef USE_PROSTONTALE		//=========== 포리스턴소스인경우
		bx = 30;
		by = 30;
		#else
		
		bx = 30;
		by = 30;
		#endif

		menuPos[8].left = BackStartPos.x + 75;		//탈퇴 왼쪽
		menuPos[8].top =  BackStartPos.y +105;
		menuPos[8].right = menuPos[8].left + bx;
		menuPos[8].bottom = menuPos[8].top + by;

		#ifdef USE_PROSTONTALE		//=========== 포리스턴소스인경우
		bx = 30;
		by = 30;
		#else
		
		bx = 30;
		by = 30;
		#endif

		menuPos[9].left = BackStartPos.x + 216;		//탈퇴 오른쪽
		menuPos[9].top =  BackStartPos.y +105;
		menuPos[9].right = menuPos[9].left + bx;
		menuPos[9].bottom = menuPos[9].top + by;

		#ifdef USE_PROSTONTALE		//=========== 포리스턴소스인경우
		bx = 30;
		by = 30;
		#else
		
		bx = 30;
		by = 30;
		#endif

		menuPos[10].left = BackStartPos.x + 75;		//탈퇴 오른쪽
		menuPos[10].top =  BackStartPos.y +155;
		menuPos[10].right = menuPos[10].left + bx;
		menuPos[10].bottom = menuPos[10].top + by;

		#ifdef USE_PROSTONTALE		//=========== 포리스턴소스인경우
		bx = 30;
		by = 30;
		#else
		
		bx = 30;
		by = 30;
		#endif

		menuPos[11].left = BackStartPos.x + 216;		//탈퇴 오른쪽
		menuPos[11].top =  BackStartPos.y +155;
		menuPos[11].right = menuPos[11].left + bx;
		menuPos[11].bottom = menuPos[11].top + by;


		#ifdef USE_PROSTONTALE		//=========== 포리스턴소스인경우
		bx = 30;
		by = 30;
		#else
		
		bx = 30;
		by = 30;
		#endif

		menuPos[12].left = BackStartPos.x + 9;		//탈퇴 오른쪽
		menuPos[12].top =  BackStartPos.y +109;
		menuPos[12].right = menuPos[11].left + bx;
		menuPos[12].bottom = menuPos[11].top + by;



		

		break;
	//=====클랜원이 아직 6명 이하일 경우
	case CLANUSER_MAKE_FIRSTMESSAGE:	
		menuSu = 3;		
		#ifdef USE_PROSTONTALE		//=========== 포리스턴소스인경우
		bx = 64;
		by = 16;
		#else
		bx = 64;
		by = 16;
		#endif
		menuPos[0].left = BackStartPos.x +  BUTTON1_X;		//확인
		menuPos[0].top = BackStartPos.y +   BUTTON1_Y;
		menuPos[0].right = menuPos[0].left + bx;
		menuPos[0].bottom = menuPos[0].top + by;

		//텍스트 출력 위치
		menuPos[1].left = BackStartPos.x + clanMakeFinishClanWon.pos.x;
		menuPos[1].top = BackStartPos.y + clanMakeFinishClanWon.pos.y;

		#ifdef USE_PROSTONTALE		//=========== 포리스턴소스인경우
		bx = 64;
		by = 16;
		#else
		bx = 64;
		by = 16;
		#endif
		menuPos[2].left = BackStartPos.x + BUTTON1_BOX_X;		//확인 테두리
		menuPos[2].top =  BackStartPos.y + BUTTON1_BOX_Y;
		menuPos[2].right = menuPos[2].left + bx;
		menuPos[2].bottom = menuPos[2].top + by;
		
		break;
	case CLANUSER_DOWN6_CLANWON_MESSAGE:
	case CLANUSER_DOWN6_CLANWON:	
		menuSu = 3;		
		#ifdef USE_PROSTONTALE		//=========== 포리스턴소스인경우
		bx = 64;
		by = 16;
		#else
		bx = 64;
		by = 16;
		#endif
		menuPos[0].left = BackStartPos.x +  BUTTON1_X;		//확인
		menuPos[0].top = BackStartPos.y +   BUTTON1_Y;
		menuPos[0].right = menuPos[0].left + bx;
		menuPos[0].bottom = menuPos[0].top + by;

		//텍스트 출력 위치
		menuPos[1].left = BackStartPos.x + CLANUSERTEXT_X;
		menuPos[1].top = BackStartPos.y + CLANUSERTEXT_Y;

		#ifdef USE_PROSTONTALE		//=========== 포리스턴소스인경우
		bx = 64;
		by = 16;
		#else
		bx = 64;
		by = 16;
		#endif
		menuPos[2].left = BackStartPos.x + BUTTON1_BOX_X;		//확인 테두리
		menuPos[2].top =  BackStartPos.y + BUTTON1_BOX_Y;
		menuPos[2].right = menuPos[2].left + bx;
		menuPos[2].bottom = menuPos[2].top + by;
		break;
	case CLAN_USER_NOMAKE:
		menuSu = 3;		
		#ifdef USE_PROSTONTALE		//=========== 포리스턴소스인경우
		bx = 64;
		by = 16;
		#else
		bx = 64;
		by = 16;
		#endif
		menuPos[0].left = BackStartPos.x +  BUTTON1_X;		//확인
		menuPos[0].top = BackStartPos.y +   BUTTON1_Y;
		menuPos[0].right = menuPos[0].left + bx;
		menuPos[0].bottom = menuPos[0].top + by;

		//텍스트 출력 위치
		menuPos[1].left = BackStartPos.x + clanClanUserNoMakeMsg.pos.x;
		menuPos[1].top = BackStartPos.y + clanClanUserNoMakeMsg.pos.y;

		#ifdef USE_PROSTONTALE		//=========== 포리스턴소스인경우
		bx = 64;
		by = 16;
		#else
		bx = 64;
		by = 16;
		#endif
		menuPos[2].left = BackStartPos.x + BUTTON1_BOX_X;		//확인 테두리
		menuPos[2].top =  BackStartPos.y + BUTTON1_BOX_Y;
		menuPos[2].right = menuPos[2].left + bx;
		menuPos[2].bottom = menuPos[2].top + by;
		break;

	case CLAN_SECESSION://탈퇴
		menuSu = 4;

#ifdef USE_PROSTONTALE		//=========== 포리스턴소스인경우
			if(hMark == NULL) hMark = GetClanMark();
			
#else
			if(hMark == NULL) hMark = GetClanMark();
#endif
		
		#ifdef USE_PROSTONTALE		//=========== 포리스턴소스인경우
		bx = 64;
		by = 16;
		#else
		bx = 64;
		by = 16;
		#endif

		menuPos[0].left = BackStartPos.x + CLANNAME_X;		//클랜 이름
		menuPos[0].top =  BackStartPos.y + CLANNAME_Y;
		menuPos[0].right = menuPos[0].left + bx;
		menuPos[0].bottom = menuPos[0].top + by;

		#ifdef USE_PROSTONTALE		//=========== 포리스턴소스인경우
		bx = 16;
		by = 16;
		#else
		bx = 16;
		by = 16;
		#endif

		menuPos[1].left = BackStartPos.x +CLANMARKBOX_X;		//마크
		menuPos[1].top	 = BackStartPos.y +CLANMARKBOX_Y;
		menuPos[1].right = menuPos[1].left + bx;
		menuPos[1].bottom = menuPos[1].top + by;

		#ifdef USE_PROSTONTALE		//=========== 포리스턴소스인경우
		bx = 16;
		by = 16;
		#else
		bx = 16;
		by = 16;
		#endif

		menuPos[7].left = BackStartPos.x +CLANMARK_X;		//마크
		menuPos[7].top	 = BackStartPos.y +CLANMARK_Y;
		menuPos[7].right = menuPos[7].left + bx;
		menuPos[7].bottom = menuPos[7].top + by;

		#ifdef USE_PROSTONTALE		//=========== 포리스턴소스인경우
		bx = 32;
		by = 16;
		#else
		bx = 32;
		by = 16;
		#endif

		menuPos[2].left = BackStartPos.x + BUTTON2_X;		//취소
		menuPos[2].top = BackStartPos.y + BUTTON2_Y;
		menuPos[2].right = menuPos[2].left + bx;
		menuPos[2].bottom = menuPos[2].top + by;

		#ifdef USE_PROSTONTALE		//=========== 포리스턴소스인경우
		bx = 32;
		by = 16;
		#else
		bx = 32;
		by = 16;
		#endif

		menuPos[3].left = BackStartPos.x + BUTTON3_X;		//확인 
		menuPos[3].top = BackStartPos.y +BUTTON3_Y;
		menuPos[3].right = menuPos[3].left + bx;
		menuPos[3].bottom = menuPos[3].top + by;

		#ifdef USE_PROSTONTALE		//=========== 포리스턴소스인경우
		bx = 64;
		by = 16;
		#else
		bx = 64;
		by = 16;
		#endif

		menuPos[4].left = BackStartPos.x + BUTTON2_BOX_X;		//취소 테드리 박스
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

		menuPos[5].left = BackStartPos.x + BUTTON3_BOX_X;		//확인 테드리 박스
		menuPos[5].top = BackStartPos.y +BUTTON3_BOX_Y;
		menuPos[5].right = menuPos[6].left + bx;
		menuPos[5].bottom = menuPos[6].top + by;

		menuPos[6].left = BackStartPos.x + clanSecession.pos.x;		//지문
		menuPos[6].top = BackStartPos.y + 130;
		menuPos[6].right = menuPos[2].left + 200;
		menuPos[6].bottom = menuPos[2].top + 40;

		menuPos[8].left = BackStartPos.x + CLANNAMETEXT_X;		//클랜이름
		menuPos[8].top = BackStartPos.y + CLANNAMETEXT_Y;
		menuPos[8].right = 0;//menuPos[2].left + 200;
		menuPos[8].bottom = 0;//menuPos[2].top + 40;
		break;
	case CLAN_RECONF_SECESSION:
		menuSu = 4;
		
		#ifdef USE_PROSTONTALE		//=========== 포리스턴소스인경우
		bx = 64;
		by = 16;
		#else
		bx = 64;
		by = 16;
		#endif

		menuPos[0].left = BackStartPos.x + CLANNAME_X;		//클랜 이름
		menuPos[0].top =  BackStartPos.y + CLANNAME_Y;
		menuPos[0].right = menuPos[0].left + bx;
		menuPos[0].bottom = menuPos[0].top + by;

		#ifdef USE_PROSTONTALE		//=========== 포리스턴소스인경우
		bx = 16;
		by = 16;
		#else
		bx = 16;
		by = 16;
		#endif

		menuPos[1].left = BackStartPos.x +CLANMARKBOX_X;		//마크
		menuPos[1].top	 = BackStartPos.y +CLANMARKBOX_Y;
		menuPos[1].right = menuPos[1].left + bx;
		menuPos[1].bottom = menuPos[1].top + by;

		#ifdef USE_PROSTONTALE		//=========== 포리스턴소스인경우
		bx = 16;
		by = 16;
		#else
		bx = 16;
		by = 16;
		#endif

		menuPos[7].left = BackStartPos.x +CLANMARK_X;		//마크
		menuPos[7].top	 = BackStartPos.y +CLANMARK_Y;
		menuPos[7].right = menuPos[7].left + bx;
		menuPos[7].bottom = menuPos[7].top + by;

		#ifdef USE_PROSTONTALE		//=========== 포리스턴소스인경우
		bx = 32;
		by = 16;
		#else
		bx = 32;
		by = 16;
		#endif

		menuPos[2].left = BackStartPos.x + BUTTON2_X;		//취소
		menuPos[2].top = BackStartPos.y + BUTTON2_Y;
		menuPos[2].right = menuPos[2].left + bx;
		menuPos[2].bottom = menuPos[2].top + by;

		#ifdef USE_PROSTONTALE		//=========== 포리스턴소스인경우
		bx = 32;
		by = 16;
		#else
		bx = 32;
		by = 16;
		#endif

		menuPos[3].left = BackStartPos.x + BUTTON3_X;		//확인 
		menuPos[3].top = BackStartPos.y +BUTTON3_Y;
		menuPos[3].right = menuPos[3].left + bx;
		menuPos[3].bottom = menuPos[3].top + by;

		#ifdef USE_PROSTONTALE		//=========== 포리스턴소스인경우
		bx = 64;
		by = 16;
		#else
		bx = 64;
		by = 16;
		#endif

		menuPos[4].left = BackStartPos.x + BUTTON2_BOX_X;		//취소 테드리 박스
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

		menuPos[5].left = BackStartPos.x + BUTTON3_BOX_X;		//확인 테드리 박스
		menuPos[5].top = BackStartPos.y +BUTTON3_BOX_Y;
		menuPos[5].right = menuPos[6].left + bx;
		menuPos[5].bottom = menuPos[6].top + by;

		menuPos[6].left = BackStartPos.x + clanSecessionMsg.pos.x;		//지문
		menuPos[6].top = BackStartPos.y + 130;
		menuPos[6].right = menuPos[2].left + 200;
		menuPos[6].bottom = menuPos[2].top + 40;

		menuPos[8].left = BackStartPos.x + CLANNAMETEXT_X;		//클랜이름
		menuPos[8].top = BackStartPos.y + CLANNAMETEXT_Y;
		menuPos[8].right = 0;//menuPos[2].left + 200;
		menuPos[8].bottom = 0;//menuPos[2].top + 40;

		

		

		break;	
	case CLAN_FINISH_SECESSION:
		menuSu = 6;
		
		#ifdef USE_PROSTONTALE		//=========== 포리스턴소스인경우
		bx = 64;
		by = 16;
		#else
		bx = 64;
		by = 16;
		#endif

		menuPos[0].left = BackStartPos.x + CLANNAME_X;		//클랜 이름
		menuPos[0].top =  BackStartPos.y + CLANNAME_Y;
		menuPos[0].right = menuPos[0].left + bx;
		menuPos[0].bottom = menuPos[0].top + by;

		#ifdef USE_PROSTONTALE		//=========== 포리스턴소스인경우
		bx = 16;
		by = 16;
		#else
		bx = 16;
		by = 16;
		#endif

		menuPos[1].left = BackStartPos.x +CLANMARKBOX_X;		//마크
		menuPos[1].top	 = BackStartPos.y +CLANMARKBOX_Y;
		menuPos[1].right = menuPos[1].left + bx;
		menuPos[1].bottom = menuPos[1].top + by;

		#ifdef USE_PROSTONTALE		//=========== 포리스턴소스인경우
		bx = 16;
		by = 16;
		#else
		bx = 16;
		by = 16;
		#endif

		menuPos[2].left = BackStartPos.x +CLANMARK_X;		//마크
		menuPos[2].top	 = BackStartPos.y +CLANMARK_Y;
		menuPos[2].right = menuPos[2].left + bx;
		menuPos[2].bottom = menuPos[2].top + by;

		#ifdef USE_PROSTONTALE		//=========== 포리스턴소스인경우
		bx = 32;
		by = 16;
		#else
		bx = 32;
		by = 16;
		#endif

		menuPos[3].left = BackStartPos.x + BUTTON1_X;		//확인
		menuPos[3].top = BackStartPos.y + BUTTON1_Y;
		menuPos[3].right = menuPos[3].left + bx;
		menuPos[3].bottom = menuPos[3].top + by;
		
		#ifdef USE_PROSTONTALE		//=========== 포리스턴소스인경우
		bx = 64;
		by = 16;
		#else
		bx = 64;
		by = 16;
		#endif

		menuPos[4].left = BackStartPos.x + BUTTON1_BOX_X;		//확인 테드리 박스
		menuPos[4].top = BackStartPos.y +BUTTON1_BOX_Y;
		menuPos[4].right = menuPos[4].left + bx;
		menuPos[4].bottom = menuPos[4].top + by;


		//텍스트 출력 위치
		menuPos[5].left = BackStartPos.x + clanFinishSecessionMsg.pos.x;
		menuPos[5].top = BackStartPos.y + 130;
		menuPos[5].right = 0;
		menuPos[5].bottom = 0;

		menuPos[6].left = BackStartPos.x + CLANNAMETEXT_X;		//클랜이름
		menuPos[6].top = BackStartPos.y + CLANNAMETEXT_Y;
		menuPos[6].right = 0;//menuPos[2].left + 200;
		menuPos[6].bottom = 0;//menuPos[2].top + 40;

		break;


	//클랜 유저 창고
	case CLAN_USER_WAREHOUSE:
		menuSu = 3;
		
		#ifdef USE_PROSTONTALE		//=========== 포리스턴소스인경우
		bx = 64;
		by = 16;
		#else
		bx = 64;
		by = 16;
		#endif
		menuPos[0].left = BackStartPos.x + BUTTON3_X;		//확인
		menuPos[0].top = BackStartPos.y + BUTTON3_Y;
		menuPos[0].right = menuPos[0].left + bx;
		menuPos[0].bottom = menuPos[0].top + by;

		#ifdef USE_PROSTONTALE		//=========== 포리스턴소스인경우
		bx = 64;
		by = 16;
		#else
		bx = 64;
		by = 16;
		#endif
		menuPos[1].left = BackStartPos.x + BUTTON3_BOX_X;		//확인
		menuPos[1].top = BackStartPos.y + BUTTON3_BOX_Y;
		menuPos[1].right = menuPos[1].left + bx;
		menuPos[1].bottom = menuPos[1].top + by;

		
		menuPos[2].left = BackStartPos.x + 50;		//지문
		menuPos[2].top = BackStartPos.y + 70;
		menuPos[2].right = menuPos[2].left + 200;
		menuPos[2].bottom = menuPos[2].top + 40;
		break;
	case CLAN_USER_HELP:
		menuSu = 2 + CLANHELPICON_DISPLAY_MAX+3;
		
		
		nScl_Bar = 0;		

		#ifdef USE_PROSTONTALE		//=========== 포리스턴소스인경우
		bx = 64;
		by = 16;
		#else
		bx = 64;
		by = 16;
		#endif
		menuPos[0].left = BackStartPos.x + BUTTON1_X;		//확인
		menuPos[0].top =  BackStartPos.y + BUTTON1_Y;
		menuPos[0].right = menuPos[0].left + bx;
		menuPos[0].bottom = menuPos[0].top + by;

		#ifdef USE_PROSTONTALE		//=========== 포리스턴소스인경우
		bx = 64;
		by = 16;
		#else
		bx = 64;
		by = 16;
		#endif
		menuPos[1].left = BackStartPos.x + BUTTON1_BOX_X;		//확인 테두리
		menuPos[1].top =  BackStartPos.y + BUTTON1_BOX_Y;
		menuPos[1].right = menuPos[1].left + bx;
		menuPos[1].bottom = menuPos[1].top + by;

		nIconPos=2;
		nAdd = 0;
		for(i = 0; i<CLANHELPICON_DISPLAY_MAX; i++)
		{			
			#ifdef USE_PROSTONTALE		//=========== 포리스턴소스인경우
			bx = CLANTEXT_IMAGE_WIDTH;
			by = CLANTEXT_IMAGE_HEIGHT;
			#else
			
			bx = CLANTEXT_IMAGE_WIDTH;
			by = CLANTEXT_IMAGE_HEIGHT;
			#endif

			menuPos[nIconPos].left = BackStartPos.x +  CLANHELPLIST_X;		
			menuPos[nIconPos].top =  BackStartPos.y +  CLANHELPLIST_Y + nAdd;//BackStartPos.y +  CLANHELPICON_START_Y+nAdd+nWidth;
			menuPos[nIconPos].right = menuPos[nIconPos].left + CLANHELPICON_WIDTH;
			menuPos[nIconPos].bottom = menuPos[nIconPos].top + CLANHELPICON_HEIGHT;
			nIconPos+=1;
			nAdd += CLANHELPICON_CAP;
			
		}
		nIconPos = 6;

		//ktj 스크롤관련
		tscr.Init(0);

		//================스크롤 바
		if(g_nClanIconNum > CLANHELPICON_DISPLAY_MAX)		
		{
			
			#ifdef USE_PROSTONTALE		//=========== 포리스턴소스인경우
			bx = 16;
			by = 16;
			#else
			bx = 16;
			by = 16;
			#endif


			menuPos[nIconPos].left = BackStartPos.x + SCROLLBAR_LEFT;
			menuPos[nIconPos].top =  BackStartPos.y + SCROOBAR_TOP;
			menuPos[nIconPos].right = menuPos[nIconPos].left + 8;
			menuPos[nIconPos].bottom = menuPos[nIconPos].top  + 160;
			//nClanWon[nIconPos] = 1;

			//ktj 스크롤관련
			tscr.Init(1);
			tscr.Init(	menuPos[nIconPos].top, //BackStartPos.y + SCROOBAR_TOP, 
						menuPos[nIconPos].top + 160,//256,
					g_nClanIconNum, CLANHELPICON_DISPLAY_MAX);
					
			
			scrIconPos.left  = BackStartPos.x + SCROLLBAR_LEFT-4;//SCROLLBAR_LEFT-4;
			//scrIconPos.top =  BackStartPos.y  + 87 -8;//(BackStartPos.y + SCROOBAR_TOP)-8;
			//ktj : 바꿈.
			scrIconPos.top =  menuPos[nIconPos].top -8;//(BackStartPos.y + SCROOBAR_TOP)-8;
		}
		//텍스트 이미지 출력
		nIconPos +=1;

		#ifdef USE_PROSTONTALE		//=========== 포리스턴소스인경우
		bx = CLANTEXT_IMAGE_WIDTH;
		by = CLANTEXT_IMAGE_HEIGHT;
		#else
		
		bx = CLANTEXT_IMAGE_WIDTH;
		by = CLANTEXT_IMAGE_HEIGHT;
		#endif
		menuPos[nIconPos].left = BackStartPos.x +  CLANTEXT_IMAGE_X;	
		menuPos[nIconPos].top =	 BackStartPos.y +   CLANTEXT_IMAGE_Y;
		menuPos[nIconPos].right = menuPos[nIconPos].left + bx;
		menuPos[nIconPos].bottom = menuPos[nIconPos].top + by;
		break;		
	case CLAN_USER_HELP_CONTENT:
		menuSu = 1+HLEPDISPLAY_MAX+4;
		
		nScl_Bar = 0;
		#ifdef USE_PROSTONTALE		//=========== 포리스턴소스인경우
		bx = 64;
		by = 16;
		#else
		bx = 64;
		by = 16;
		#endif
		menuPos[0].left = BackStartPos.x +  BUTTON1_X;		//확인
		menuPos[0].top = BackStartPos.y +   BUTTON1_Y;
		menuPos[0].right = menuPos[0].left + bx;
		menuPos[0].bottom = menuPos[0].top + by;
		#ifdef USE_PROSTONTALE		//=========== 포리스턴소스인경우
		bx = 64;
		by = 16;
		#else
		bx = 64;
		by = 16;
		#endif
		menuPos[1].left = BackStartPos.x + BUTTON1_BOX_X;		//확인 테두리
		menuPos[1].top =  BackStartPos.y + BUTTON1_BOX_Y;
		menuPos[1].right = menuPos[1].left + bx;
		menuPos[1].bottom = menuPos[1].top + by;		

		#ifdef USE_PROSTONTALE		//=========== 포리스턴소스인경우
		bx = 120;
		by = 30;
		#else
		
		bx = 120;
		by = 30;
		#endif
		menuPos[2].left = BackStartPos.x + CLANTITLE_X;		//헬프 마크
		menuPos[2].top =  BackStartPos.y + CLANTITLE_Y;//CLANHELPCONTENT_MARK_Y;
		menuPos[2].right = menuPos[2].left + bx;
		menuPos[2].bottom = menuPos[2].top + by;		

		#ifdef USE_PROSTONTALE		//=========== 포리스턴소스인경우
		bx = 30;
		by = 30;
		#else

		bx = 30;
		by = 30;
		#endif
		
		menuPos[3].left = BackStartPos.x + 14;		
		menuPos[3].top =  BackStartPos.y + 12;
		menuPos[3].right = menuPos[3].left+bx;
		menuPos[3].bottom = menuPos[3].top+by;

		//===라인
		menuPos[4].left = BackStartPos.x + 9;		
		menuPos[4].top =  BackStartPos.y + 45;
		menuPos[4].right = menuPos[3].left+bx;
		menuPos[4].bottom = menuPos[3].top+by;

		//텍스트 출력 위치
		nIconPos=5;

		//================스크롤 바

		if(g_HelpDisPlay> HLEPDISPLAY_MAX)		
		{
			
			#ifdef USE_PROSTONTALE		//=========== 포리스턴소스인경우
			bx = 16;
			by = 16;
			#else
			bx = 16;
			by = 16;
			#endif

			menuPos[nIconPos].left = BackStartPos.x + SCROLLBAR_LEFT;//SCROLLBAR_LEFT;
			menuPos[nIconPos].top =  BackStartPos.y + SCROOBAR_TOP;//BackStartPos.y + SCROOBAR_TOP;
			menuPos[nIconPos].right = menuPos[nIconPos].left + 8;
			menuPos[nIconPos].bottom = menuPos[nIconPos].top + 160;

			//ktj 스크롤관련
			tscr.Init(1);
			tscr.Init(	menuPos[nIconPos].top,//BackStartPos.y + SCROOBAR_TOP, 
						menuPos[nIconPos].top + 160,//256,
					g_HelpDisPlay, HLEPDISPLAY_MAX);					
			scrIconPos.left  = BackStartPos.x + SCROLLBAR_LEFT-5;//SCROLLBAR_LEFT-4;
			//ktj : 바꿈.
			//scrIconPos.top =  BackStartPos.y  + 87 -8;//(BackStartPos.y + SCROOBAR_TOP)-8;
			scrIconPos.top =  menuPos[nIconPos].top -8;//(BackStartPos.y + SCROOBAR_TOP)-8;
		}

		
		nIconPos=7;
		nAdd = 0;
		for(i = 0; i<HLEPDISPLAY_MAX; i++)
		{			
			#ifdef USE_PROSTONTALE		//=========== 포리스턴소스인경우
			bx = 64;
			by = 16;
			#else
			bx = 64;
			by = 16;
			#endif

			menuPos[nIconPos].left = BackStartPos.x + HELPCONTENT_X;//BackStartPos.x +  HELPCONTENT_X;		
			menuPos[nIconPos].top =  BackStartPos.y +  HELPCONTENT_Y + nAdd;//BackStartPos.y +  HELPCONTENT_Y+nAdd;
			menuPos[nIconPos].right = 0;//menuPos[nIconPos].left + CLANHELPICON_WIDTH;
			menuPos[nIconPos].bottom = 0;//menuPos[nIconPos].top + CLANHELPICON_HEIGHT;
			nIconPos+=1;
			nAdd += HELPCONTENT_CAP;
		}

		break;
	case CLANUSER_RELEASEMESSAGE:
		menuSu = 3;
		#ifdef USE_PROSTONTALE		//=========== 포리스턴소스인경우
		bx = 64;
		by = 16;
		#else		
		bx = 64;
		by = 16;
		#endif
		menuPos[0].left = BackStartPos.x + BUTTON1_X;		//확인
		menuPos[0].top = BackStartPos.y + BUTTON1_Y;
		menuPos[0].right = menuPos[0].left + bx;
		menuPos[0].bottom = menuPos[0].top + by;

		//텍스트 출력 위치
		menuPos[1].left = BackStartPos.x + 55;
		menuPos[1].top = BackStartPos.y + 70;

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
	case CLANUSER_SUBCHIP_READMSG_FLAG:
	case ISPFLAG_USER:
	case CLAN_USER_ReadDATA:
	case CLAN_RECONF_SECESSION_LOADING:
		//텍스트 출력 위치
		menuPos[0].left = BackStartPos.x + clanLoading.pos.x;
		menuPos[0].top = BackStartPos.y + clanLoading.pos.y;
		menuPos[0].right = 0;
		menuPos[0].bottom = 0;
		break;
	}
}



void CLANEntity_user::Draw()
{
	int i =0,nIconCount=0;
	int nIconPos = 0;
	int nSaveTop = 0,nAdd = 0;
	int nStrLen = 0;
	char szDrawMenuMsg[2024];
	ZeroMemory(szDrawMenuMsg,sizeof(char)*2024);

#ifdef USE_PROSTONTALE		//=========== 포리스턴소스인경우
	char szHelpContent[HELPWIDTH];
#else
	HDC hdc;
	HDC sorDC;
	BITMAP bit;
	int bx,by;
	
	int j =0;
	char szHelpContent[HELPWIDTH];
//	HFONT font, oldfont;

	

#endif
	
#ifdef USE_PROSTONTALE		//=========== 포리스턴소스인경우
	switch(C_menuN){
		case CLANUSER_SUBCHIP_APPOINTMSG:  //클랜부침이 임명되다고 한번 보여줌
			DrawSprite(menuPos[2].left,menuPos[2].top, hButton_Box,  0, 0, 48, 23, 1);		
			if(menuMouseOver[0]>0) DrawSprite(menuPos[0].left,menuPos[0].top, hOk_Yellow,  0, 0, 32, 16, 1);
			else	DrawSprite(menuPos[0].left,menuPos[0].top, hOk_Gray,  0, 0, 32, 16, 1);
			
			
			if(cldata.SubChip[0] == 0)
			{
				wsprintf(szClanUserMsg,SubChipAppointEnd.szMsg,cldata.OldSubChip);
			}
			else
			{
				wsprintf(szClanUserMsg,SubChipAppointEnd.szMsg,cldata.SubChip);
			}
			Draw_C_Text(szClanUserMsg, menuPos[1].left, menuPos[1].top);		
			break;
		case CLANUSER_SUBCHIP_RELEASEMSG:	//클랜부침이 해임되다고 한번 보여줌
			DrawSprite(menuPos[2].left,menuPos[2].top, hButton_Box,  0, 0, 48, 23, 1);		
			if(menuMouseOver[0]>0) DrawSprite(menuPos[0].left,menuPos[0].top, hOk_Yellow,  0, 0, 32, 16, 1);
			else	DrawSprite(menuPos[0].left,menuPos[0].top, hOk_Gray,  0, 0, 32, 16, 1);
			
			wsprintf(szClanUserMsg,SubChipReleaseEnd.szMsg,cldata.OldSubChip);			
			Draw_C_Text(szClanUserMsg, menuPos[1].left, menuPos[1].top);		
			break;
		case CLANUSER_LEAVECLAN_MESSAGE:
			DrawSprite(menuPos[2].left,menuPos[2].top, hButton_Box,  0, 0, 48, 23, 1);		
			if(menuMouseOver[0]>0) DrawSprite(menuPos[0].left,menuPos[0].top, hOk_Yellow,  0, 0, 32, 16, 1);
			else	DrawSprite(menuPos[0].left,menuPos[0].top, hOk_Gray,  0, 0, 32, 16, 1);
			
			wsprintf(szClanUserMsg,LeaveClanMessage.szMsg,cldata.OldChipChaName,cldata.ClanZang_Name);
			Draw_C_Text(szClanUserMsg, menuPos[1].left, menuPos[1].top);		
			break;
		case CLANUSER_REMOVE ://클랜원이 추방당했을경우
			DrawSprite(menuPos[2].left,menuPos[2].top, hButton_Box,  0, 0, 48, 23, 1);		
			if(menuMouseOver[0]>0) DrawSprite(menuPos[0].left,menuPos[0].top, hOk_Yellow,  0, 0, 32, 16, 1);
			else	DrawSprite(menuPos[0].left,menuPos[0].top, hOk_Gray,  0, 0, 32, 16, 1);
			
			wsprintf(szClanUserMsg,clanClanUserRemove.szMsg,cldata.name,cldata.ChaName);
			Draw_C_Text(szClanUserMsg, menuPos[1].left, menuPos[1].top);		
			break;
		case	CLANUSER_AUTORELEASE://자동해체
			DrawSprite(menuPos[2].left,menuPos[2].top, hButton_Box,  0, 0, 48, 23, 1);		
			if(menuMouseOver[0]>0) DrawSprite(menuPos[0].left,menuPos[0].top, hOk_Yellow,  0, 0, 32, 16, 1);
			else	DrawSprite(menuPos[0].left,menuPos[0].top, hOk_Gray,  0, 0, 32, 16, 1);
			
			//wsprintf(szClanUserMsg,clanClanUserReleaseMsg.szMsg,cldata.name);
			Draw_C_Text(szClanUserMsg, menuPos[1].left, menuPos[1].top);		
			break;
		case CLAN_USER:
		DrawSprite(menuPos[0].left,menuPos[0].top, hClanName,  0, 0, CLANNAME_WIDTH, CLANNAME_HEIGHT, 1);
		

		//if((menuMouseOver[2] > 0) ||(menuMouseOver[8] > 0)||(menuMouseOver[9] > 0))
		if(menuMouseOver[2] > 0)
		{
			//DrawSprite(menuPos[2].left-31,menuPos[2].top, hClanRelease[1],  0, 0, 30, 30, 1);
			//DrawSprite(menuPos[2].right +1,menuPos[2].top, hClanRelease[1],  0, 0, 30, 30, 1);
			DrawSprite(menuPos[2].left,menuPos[2].top, hTxt_Secession[1],  0, 0, CLANTEXT_IMAGE_WIDTH, CLANTEXT_IMAGE_HEIGHT, 1);
		}
		else
		{
			//DrawSprite(menuPos[2].left-31,menuPos[2].top, hClanRelease[0],  0, 0, 30, 30, 1);
			//DrawSprite(menuPos[2].right +1,menuPos[2].top, hClanRelease[0],  0, 0, 30, 30, 1);
			DrawSprite(menuPos[2].left,menuPos[2].top, hTxt_Secession[0],  0, 0, CLANTEXT_IMAGE_WIDTH, CLANTEXT_IMAGE_HEIGHT, 1);
		}
		
		//if((menuMouseOver[3] > 0)||(menuMouseOver[10] > 0)||(menuMouseOver[11] > 0))
		if(menuMouseOver[3] > 0)
		{
			//DrawSprite(menuPos[3].left-31,menuPos[3].top, hHelp[1],  0, 0, 30, 30, 1);
			//DrawSprite(menuPos[3].right+1,menuPos[3].top, hHelp[1],  0, 0, 30, 30, 1);
			DrawSprite(menuPos[3].left,menuPos[3].top, hTxt_Help[1],  0, 0, CLANTEXT_IMAGE_WIDTH, CLANTEXT_IMAGE_HEIGHT, 1);

		}
		else
		{
			//DrawSprite(menuPos[3].left-31,menuPos[3].top, hHelp[0],  0, 0, 30, 30, 1);
			//DrawSprite(menuPos[3].right+1,menuPos[3].top, hHelp[0],  0, 0, 30, 30, 1);
			DrawSprite(menuPos[3].left,menuPos[3].top, hTxt_Help[0],  0, 0, CLANTEXT_IMAGE_WIDTH, CLANTEXT_IMAGE_HEIGHT, 1);
		}

		DrawSprite(menuPos[5].left,menuPos[5].top, hButton_Box,  0, 0, 48, 23, 1);
		if(menuMouseOver[4] > 0) DrawSprite(menuPos[4].left,menuPos[4].top, hClose[1],  0, 0, 32, 16, 1);
		else DrawSprite(menuPos[4].left,menuPos[4].top, hClose[0],  0, 0, 32, 16, 1);

		//DrawSprite_TJB(menuPos[1].left,menuPos[1].top, 16, 16,  hMark,  0, 0, 16, 16, 1);
		DrawSprite(menuPos[1].left,menuPos[1].top, hMark_box,  0, 0, 69, 59, 1);

		if(cldata.hClanMark)
		{
			//DrawSprite(menuPos[1].left,menuPos[1].top, hMark,  0, 0, 16, 16, 1);
			//DrawSprite(menuPos[6].left,menuPos[6].top, hMark,  0, 0, 16, 16, 1);

			void DrawSprite_TJB(int winX ,int winY, int winxlen, int winylen, LPDIRECTDRAWSURFACE4 pdds,int x, int y, int width, int height,int Bltflag);
			//DrawSprite_TJB(menuPos[1].left,menuPos[1].top, 16, 16,  hMark,  0, 0, 16, 16, 1);
			//DrawSprite(menuPos[6].left,menuPos[6].top, hMark,  0, 0, 32, 32, 1);
			DrawSprite(menuPos[6].left,menuPos[6].top, cldata.hClanMark,  0, 0, 32, 32, 1);

		}

		nStrLen = lstrlen(cldata.name)/2;		
		
		if(nStrLen%2)
		{
			nStrLen = nStrLen/2;
			nStrLen+=1;
			Draw_C_Text(cldata.name, menuPos[7].left- (nStrLen*11), menuPos[7].top, 2);	//클랜이름그림자 2임
		}
		else
		{
			nStrLen = nStrLen/2;	
			Draw_C_Text(cldata.name, menuPos[7].left- (nStrLen*11)-3, menuPos[7].top, 2);	//클랜이름그림자 2임
		}

		lpD3DDevice->BeginScene();
		sinDrawTexImage(hLine,(float)menuPos[12].left,(float)menuPos[12].top,
							(float)240, 10.0f,
							0.0f,0.0f,0.0f,0.0f,0.0f,0.0f,255);    
		lpD3DDevice->EndScene();
		

		
				
		break;
		case CLANUSER_MAKE_FIRSTMESSAGE:
			DrawSprite(menuPos[2].left,menuPos[2].top, hButton_Box,  0, 0, 48, 23, 1);
			if(menuMouseOver[0] > 0) DrawSprite(menuPos[0].left,menuPos[0].top, hOk_Yellow,  0, 0, 32, 16, 1);		
			else DrawSprite(menuPos[0].left,menuPos[0].top, hOk_Gray,  0, 0, 32, 16, 1);				
			wsprintf(szDrawMenuMsg,szMakeFinishClan,cldata.name);
			//wsprintf(string,szMakeFinishClanWon,cldata.name);
			Draw_C_Text(string, menuPos[1].left, menuPos[1].top);			
			break;
		case CLANUSER_DOWN6_CLANWON_MESSAGE:
		case CLANUSER_DOWN6_CLANWON:
			DrawSprite(menuPos[2].left,menuPos[2].top, hButton_Box,  0, 0, 48, 23, 1);
			if(menuMouseOver[0] > 0) DrawSprite(menuPos[0].left,menuPos[0].top, hOk_Yellow,  0, 0, 32, 16, 1);		
			else DrawSprite(menuPos[0].left,menuPos[0].top, hOk_Gray,  0, 0, 32, 16, 1);				
			Draw_C_Text(szClanWon6Down, menuPos[1].left, menuPos[1].top);			
			break;		
	case CLAN_USER_NOMAKE: //클랜을 아직 결성 못함(6인이하 10일동안)
		DrawSprite(menuPos[2].left,menuPos[2].top, hButton_Box,  0, 0, 48, 23, 1);
		if(menuMouseOver[0] > 0) DrawSprite(menuPos[0].left,menuPos[0].top, hOk_Yellow,  0, 0, 32, 16,1);		
		else DrawSprite(menuPos[0].left,menuPos[0].top, hOk_Gray,  0, 0, 32, 16, 1);				
		wsprintf(string,clanClanUserNoMakeMsg.szMsg,cldata.name);
		Draw_C_Text(string, menuPos[1].left, menuPos[1].top);	
		break;


	case CLAN_USER_WAREHOUSE:
		DrawSprite(menuPos[1].left,menuPos[1].top, hButton_Box,  0, 0, 48, 23, 1);
		if(menuMouseOver[0] > 0) DrawSprite(menuPos[0].left,menuPos[0].top, hOk_Yellow,  0, 0, 32, 16, 1);		
		else DrawSprite(menuPos[0].left,menuPos[0].top, hOk_Gray,  0, 0, 32, 16, 1);				
		break;

	case CLAN_SECESSION:
		DrawSprite(menuPos[0].left,menuPos[0].top, hClanName,  0, 0, CLANNAME_WIDTH, CLANNAME_HEIGHT, 1);	
		
		DrawSprite(menuPos[4].left,menuPos[4].top, hButton_Box,  0, 0, 48, 23, 1);
		DrawSprite(menuPos[5].left,menuPos[5].top, hButton_Box,  0, 0, 48, 23, 1);

		if(menuMouseOver[3] > 0) DrawSprite(menuPos[3].left,menuPos[3].top, hCancel_Yellow,  0, 0, 32, 16, 1);
		else DrawSprite(menuPos[3].left,menuPos[3].top, hCancel_Gray,  0, 0, 32, 16, 1);
		if(menuMouseOver[2]>0) DrawSprite(menuPos[2].left,menuPos[2].top, hOk_Yellow,  0, 0, 32, 16, 1);	
		else	DrawSprite(menuPos[2].left,menuPos[2].top, hOk_Gray,  0, 0, 32, 16, 1);			
		
		DrawSprite(menuPos[1].left,menuPos[1].top, hMark_box,  0, 0, 69, 59, 1);

		if(cldata.hClanMark)
		{
			//DrawSprite(menuPos[1].left,menuPos[1].top, hMark,  0, 0, 16, 16, 1);
			//DrawSprite(menuPos[7].left,menuPos[7].top, hMark,  0, 0, 16, 16, 1);

			void DrawSprite_TJB(int winX ,int winY, int winxlen, int winylen, LPDIRECTDRAWSURFACE4 pdds,int x, int y, int width, int height,int Bltflag);
			//DrawSprite_TJB(menuPos[1].left,menuPos[1].top, 16, 16,  hMark,  0, 0, 16, 16, 1);
			DrawSprite(menuPos[7].left,menuPos[7].top, cldata.hClanMark,  0, 0, 32, 32, 1);

		}
		//wsprintf(string,szSecessiong,cldata.name);
		//wsprintf(string,szSecession,cldata.name);
		wsprintf(string,clanSecession.szMsg,cldata.name);
		Draw_C_Text(string, menuPos[6].left, menuPos[6].top);	
		nStrLen = lstrlen(cldata.name)/2;		
		
		if(nStrLen%2)
		{
			nStrLen = nStrLen/2;
			nStrLen+=1;
			Draw_C_Text(cldata.name, menuPos[8].left- (nStrLen*11), menuPos[8].top, 2);
		}
		else
		{
			nStrLen = nStrLen/2;	
			Draw_C_Text(cldata.name, menuPos[8].left- (nStrLen*11)-3, menuPos[8].top, 2);
		}
		

		break;
	case CLAN_RECONF_SECESSION:
		DrawSprite(menuPos[0].left,menuPos[0].top, hClanName,  0, 0, CLANNAME_WIDTH, CLANNAME_HEIGHT, 1);	
		
		DrawSprite(menuPos[4].left,menuPos[4].top, hButton_Box,  0, 0, 48, 23, 1);
		DrawSprite(menuPos[5].left,menuPos[5].top, hButton_Box,  0, 0, 48, 23, 1);

		if(menuMouseOver[3] > 0) DrawSprite(menuPos[3].left,menuPos[3].top, hCancel_Yellow,  0, 0, 32, 16, 1);
		else DrawSprite(menuPos[3].left,menuPos[3].top, hCancel_Gray,  0, 0, 32, 16, 1);
		if(menuMouseOver[2]>0) DrawSprite(menuPos[2].left,menuPos[2].top, hOk_Yellow,  0, 0, 32, 16, 1);	
		else	DrawSprite(menuPos[2].left,menuPos[2].top, hOk_Gray,  0, 0, 32, 16, 1);			

		DrawSprite(menuPos[1].left,menuPos[1].top,   hMark_box,  0, 0, 69, 59, 1);

		if(cldata.hClanMark)
		{
			//DrawSprite(menuPos[1].left,menuPos[1].top, hMark,  0, 0, 16, 16, 1);
			//DrawSprite(menuPos[7].left,menuPos[7].top, hMark,  0, 0, 16, 16, 1);

			void DrawSprite_TJB(int winX ,int winY, int winxlen, int winylen, LPDIRECTDRAWSURFACE4 pdds,int x, int y, int width, int height,int Bltflag);
			//DrawSprite_TJB(menuPos[1].left,menuPos[1].top, 16, 16,  hMark,  0, 0, 16, 16, 1);
			DrawSprite(menuPos[7].left,menuPos[7].top,   cldata.hClanMark,  0, 0, 32, 32, 1);

		}
		Draw_C_Text(clanSecessionMsg.szMsg, menuPos[6].left, menuPos[6].top);	
		nStrLen = lstrlen(cldata.name)/2;		
		
		if(nStrLen%2)
		{
			nStrLen = nStrLen/2;
			nStrLen+=1;
			Draw_C_Text(cldata.name, menuPos[8].left- (nStrLen*11), menuPos[8].top, 2);
		}
		else
		{
			nStrLen = nStrLen/2;	
			Draw_C_Text(cldata.name, menuPos[8].left- (nStrLen*11)-3, menuPos[8].top, 2);
		}
		

		break;
	case CLANUSER_SUBCHIP_READMSG_FLAG:
	case ISPFLAG_USER:
	case CLAN_USER_ReadDATA:
	case CLAN_RECONF_SECESSION_LOADING:
		Draw_C_Text(clanLoading.szMsg, menuPos[0].left, menuPos[0].top);
		break;
	case CLAN_FINISH_SECESSION:
		DrawSprite(menuPos[0].left,menuPos[0].top, hClanName,  0, 0, CLANNAME_WIDTH, CLANNAME_HEIGHT, 1);	
		DrawSprite(menuPos[4].left,menuPos[4].top, hButton_Box,  0, 0, 48, 23, 1);
		if(menuMouseOver[3]>0) DrawSprite(menuPos[3].left,menuPos[3].top, hOk_Yellow,  0, 0, 32, 16, 1);	
		else	DrawSprite(menuPos[3].left,menuPos[3].top, hOk_Gray,  0, 0, 32, 16, 1);	

		DrawSprite(menuPos[1].left,menuPos[1].top,   hMark_box,  0, 0, 69, 59, 1);

		if(cldata.hClanMark)
		{
			//DrawSprite(menuPos[1].left,menuPos[1].top, hMark,  0, 0, 16, 16, 1);
			//DrawSprite(menuPos[7].left,menuPos[7].top, hMark,  0, 0, 16, 16, 1);

			void DrawSprite_TJB(int winX ,int winY, int winxlen, int winylen, LPDIRECTDRAWSURFACE4 pdds,int x, int y, int width, int height,int Bltflag);
			
			//DrawSprite_TJB(menuPos[2].left,menuPos[2].top, 16, 16,  hMark,  0, 0, 16, 16, 1);
			DrawSprite(menuPos[2].left,menuPos[2].top,   cldata.hClanMark,  0, 0, 32, 32, 1);

		}
		//wsprintf(szClanUserMsg,szFinishSecessionMsg,cldata.name,cldata.ChaName);
		Draw_C_Text(szClanUserMsg, menuPos[5].left, menuPos[5].top);	
		nStrLen = lstrlen(cldata.name)/2;		
		
		if(nStrLen%2)
		{
			nStrLen = nStrLen/2;
			nStrLen+=1;
			Draw_C_Text(cldata.name, menuPos[6].left- (nStrLen*11), menuPos[6].top, 2);
		}
		else
		{
			nStrLen = nStrLen/2;	
			Draw_C_Text(cldata.name, menuPos[6].left- (nStrLen*11)-3, menuPos[6].top, 2);
		}
		
		break;

	
	case CLAN_USER_HELP:
		DrawSprite(menuPos[1].left,menuPos[1].top, hButton_Box,  0, 0, 48, 23, 1);
		if(menuMouseOver[0] > 0) DrawSprite(menuPos[0].left,menuPos[0].top, hOk_Yellow,  0, 0, 32, 16, 1);		
		else DrawSprite(menuPos[0].left,menuPos[0].top, hOk_Gray,  0, 0, 32, 16, 1);		
		nIconPos = 2;
		for(i=0; i < CLANHELPICON_DISPLAY_MAX; i++)
		{
			if(i == g_nClanIconNum) break;
			if(menuMouseOver[nIconPos] > 0) 
			{
				//DrawSprite(menuPos[nIconPos].left - 31,menuPos[nIconPos].top, hHelp[1],  0, 0, 30, 30, 1);
				//DrawSprite(menuPos[nIconPos].right +1,menuPos[nIconPos].top, hClanHelpIcon[i+nScl_Bar][1],  0, 0, 30, 30, 1);
				DrawSprite(menuPos[nIconPos].left,menuPos[nIconPos].top, hClanHelpIcon[i+nScl_Bar][0],  0, 0, 109, 30, 1);
				
			}
			else
			{
				//DrawSprite(menuPos[nIconPos].left - 31,menuPos[nIconPos].top, hHelp[0],  0, 0, 30, 30, 1);
				//DrawSprite(menuPos[nIconPos].right +1,menuPos[nIconPos].top, hClanHelpIcon[i+nScl_Bar][0],  0, 0, 30, 30, 1);
				DrawSprite(menuPos[nIconPos].left,menuPos[nIconPos].top, hClanHelpIcon[i+nScl_Bar][1],  0, 0, 109, 30, 1);
			}
			nIconPos+=1;
		}
		/*nIconPos=1;
		for(i=0; i < CLANHELPICON_DISPLAY_MAX; i++)
		{
			if(i == g_nClanIconNum) break;
			nIconPos+=1;									
			strcpy(szClanWon,ClanHelpIcon[i+nScl_Bar].szName);
			Draw_C_Text(szClanWon, menuPos[nIconPos].left+ CLANHELPICON_WIDTH + 20, menuPos[nIconPos].top+CLANHELPTEXT_Y);	
		}*/
		nIconPos = 6;
		if(g_nClanIconNum>CLANHELPICON_DISPLAY_MAX)
		{			
			
			//DrawSprite(menuPos[nIconPos].left,menuPos[nIconPos].top, hScl_Bar,  0, 0, 8, 256, 1);			
			//ktj : 스크롤 아이콘 그리기
			//DrawSprite(scrIconPos.left, scrIconPos.top, hScl_icon,  0, 0, 16, 16, 1);			
			DrawSprite(menuPos[nIconPos].left, menuPos[nIconPos].top, hScl_Bar,   
				0, 0, 8, 160, 1);	
			lpD3DDevice->BeginScene();
			//sinDrawTexImage(hScl_Bar_tj,  (float)menuPos[nIconPos].left, (float)menuPos[nIconPos].top, 8, 256);
			sinDrawTexImage( hScl_icon_tj , (float)scrIconPos.left, (float)scrIconPos.top, 16, 16);
			lpD3DDevice->EndScene();
		}
		nIconPos +=1;
		
		DrawSprite(menuPos[nIconPos].left ,menuPos[nIconPos].top, hTxt_Help[0],  0, 0, CLANTEXT_IMAGE_WIDTH, CLANTEXT_IMAGE_HEIGHT, 1);
		//DrawSprite(menuPos[nIconPos].left - 31,menuPos[nIconPos].top, hHelp[1],  0, 0, 30, 30, 1);
		//DrawSprite(menuPos[nIconPos].right + 1,menuPos[nIconPos].top, hHelp[1],  0, 0, 30, 30, 1);
		break;
	case CLAN_USER_HELP_CONTENT:
		DrawSprite(menuPos[1].left,menuPos[1].top, hButton_Box,  0, 0, 48, 23, 1);
		if(menuMouseOver[0] > 0) DrawSprite(menuPos[0].left,menuPos[0].top, hOk_Yellow,  0, 0, 32, 16, 1);		
		else DrawSprite(menuPos[0].left,menuPos[0].top, hOk_Gray,  0, 0, 32, 16, 1);				
		
		DrawSprite(menuPos[2].left,menuPos[2].top, hTxt_Help[0],  0, 0, 109, 30, 1);
		DrawSprite(menuPos[3].left,menuPos[3].top, hBt_Help,  0, 0, 30, 30, 1);
		//DrawSprite(menuPos[2].left-31,menuPos[2].top, hHelp[1],  0, 0, 30, 30, 1);
		//DrawSprite(menuPos[2].right + 1,menuPos[2].top, hHelp[1],  0, 0, 30, 30, 1);

		//Draw_C_Text(ClanHelpIcon[g_HelpIconNumber].szName, menuPos[3].left, menuPos[3].top);
		lpD3DDevice->BeginScene();
		sinDrawTexImage(hLine,(float)menuPos[4].left,(float)menuPos[4].top,
							(float)240, 10.0f,
							0.0f,0.0f,0.0f,0.0f,0.0f,0.0f,255);    
		lpD3DDevice->EndScene();		

		nIconPos = 5;
		if(g_HelpDisPlay> HLEPDISPLAY_MAX)
		{
			/*
			DrawSprite(menuPos[nIconPos].left,menuPos[nIconPos].top, hScl_Bar,  0, 0, 8, 256, 1);			

			//ktj : 스크롤 아이콘 그리기
			DrawSprite(scrIconPos.left, scrIconPos.top, hScl_icon,  0, 0, 16, 16, 1);			
			*/

			DrawSprite(menuPos[nIconPos].left, menuPos[nIconPos].top, hScl_Bar,   
				0, 0, 8, 160, 1);	
			lpD3DDevice->BeginScene();
			//sinDrawTexImage(hScl_Bar_tj,  (float)menuPos[nIconPos].left, (float)menuPos[nIconPos].top, 8, 256);
			sinDrawTexImage( hScl_icon_tj , (float)scrIconPos.left, (float)scrIconPos.top, 16, 16);
			lpD3DDevice->EndScene();

		}
		nIconPos=7;
		for(i=0; i < HLEPDISPLAY_MAX; i++)
		{
			if(i == g_HelpDisPlay) break;
			
			ZeroMemory(szHelpContent,sizeof(char)*HELPWIDTH);		;
									
			strncpy(szHelpContent,HelpContents[i+nScl_Bar].szHelpContent,lstrlen(HelpContents[i+nScl_Bar].szHelpContent));			
			Draw_C_Text(szHelpContent, menuPos[nIconPos].left,menuPos[nIconPos].top);				
			nIconPos+=1;
		}		
		
		break;
	case CLANUSER_RELEASEMESSAGE:
		DrawSprite(menuPos[2].left,menuPos[2].top, hButton_Box,  0, 0, 48, 23, 1);		
		if(menuMouseOver[0]>0) DrawSprite(menuPos[0].left,menuPos[0].top, hOk_Yellow,  0, 0, 32, 16, 1);
		else	DrawSprite(menuPos[0].left,menuPos[0].top, hOk_Gray,  0, 0, 32, 16, 1);
		
		Draw_C_Text(clanClanUserRelease.szMsg, menuPos[1].left, menuPos[1].top);				
		break;
	
	}
#else
     HDC destdc;
	switch(C_menuN){
		case CLANUSER_SUBCHIP_APPOINTMSG://클랜부침이 임명되다고 한번 보여줌
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
			if(cldata.SubChip[0]==0)
			{
				wsprintf(szDrawMenuMsg,SubChipAppointEnd.szMsg,cldata.OldSubChip);
			}
			else{	wsprintf(szDrawMenuMsg,SubChipAppointEnd.szMsg,cldata.SubChip);}
			Text_Out(destdc,menuPos[1].left,menuPos[1].top,szDrawMenuMsg);

			
			
			DeleteDC(sorDC);
			DeleteDC(destdc);
			ReleaseDC(g_hwnd, hdc );
			break;
		case CLANUSER_SUBCHIP_RELEASEMSG://클랜부침이 해임되다고 한번 보여줌
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
			wsprintf(szDrawMenuMsg,SubChipReleaseEnd.szMsg,cldata.OldSubChip);
			Text_Out(destdc,menuPos[1].left,menuPos[1].top,szDrawMenuMsg);

			
			
			DeleteDC(sorDC);
			DeleteDC(destdc);
			ReleaseDC(g_hwnd, hdc );
			break;
		case CLANUSER_LEAVECLAN_MESSAGE:
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
			wsprintf(szClanUserMsg,LeaveClanMessage.szMsg,cldata.OldChipChaName,cldata.ClanZang_Name);
			Text_Out(destdc,menuPos[1].left,menuPos[1].top,szClanUserMsg);

			
			
			DeleteDC(sorDC);
			DeleteDC(destdc);
			ReleaseDC(g_hwnd, hdc );
			break;
		case CLANUSER_REMOVE ://클랜원이 추방당했을경우
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
			wsprintf(szClanUserMsg,clanClanUserRemove.szMsg,cldata.name,cldata.ChaName);
			Text_Out(destdc,menuPos[1].left,menuPos[1].top,szClanUserMsg);

			
			
			DeleteDC(sorDC);
			DeleteDC(destdc);
			ReleaseDC(g_hwnd, hdc );
			break;
		case	CLANUSER_AUTORELEASE://자동해체
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
			wsprintf(szClanUserMsg,szClanUserReleaseMsg,cldata.name);
			Text_Out(destdc,menuPos[1].left,menuPos[1].top,szClanUserMsg);

			
			
			DeleteDC(sorDC);
			DeleteDC(destdc);
			ReleaseDC(g_hwnd, hdc );
			break;
		case CLAN_USER:
		hdc = GetDC(g_hwnd);
		sorDC  = CreateCompatibleDC(hdc);
		destdc = CreateCompatibleDC(hdc);
		SelectObject(destdc, Dback.hb );		//off스크린설정.

		SelectObject(sorDC,hButton_Box);
		GetObject(hButton_Box,sizeof(BITMAP),&bit);
		bx = bit.bmWidth;
		by = bit.bmHeight;		
		BitBlt(destdc,menuPos[5].left,menuPos[5].top,menuPos[5].right,menuPos[5].bottom,sorDC,0,0,SRCCOPY);

		SelectObject(sorDC, hClanName);
		GetObject(hClanName,sizeof(BITMAP),&bit);
		bx = bit.bmWidth;
		by = bit.bmHeight;
		BitBlt(destdc,menuPos[0].left,menuPos[0].top,menuPos[0].right,menuPos[0].bottom,sorDC,0,0,SRCCOPY);
		/*font=PT_CreateFont(11,0,0,0,0,0,0,0,0,3,2,1,
			VARIABLE_PITCH | FF_ROMAN,"굴림체");
		oldfont=(HFONT)SelectObject(hdc,font);*/		
		nStrLen = lstrlen(cldata.name)/2;		
		
		if(nStrLen%2)
		{
			nStrLen = nStrLen/2;
			nStrLen+=1;
		}
		else nStrLen = nStrLen/2;
		
		//dsTextLineOut(hdc,menuPos[7].left - (nStrLen*11),menuPos[7].top,cldata.name,lstrlen(cldata.name));
		Text_Out(destdc,menuPos[7].left - (nStrLen*11),menuPos[7].top,cldata.name);
		//SelectObject(hdc,oldfont);
		//DeleteObject(font);

		//dsTextLineOut(hdc,menuPos[0].left+CLAN_NAME_POS_X,menuPos[0].top+CLAN_NAME_POS_Y,cldata.name,lstrlen(cldata.name));
		
         

		SelectObject(sorDC, hMark_box);
		GetObject(hMark_box,sizeof(BITMAP),&bit);
		bx = bit.bmWidth;
		by = bit.bmHeight;		
		BitBlt(destdc,menuPos[1].left,menuPos[1].top,menuPos[1].right,menuPos[1].bottom,sorDC,0,0,SRCCOPY);

		if(cldata.hClanMark)
		{		

			SelectObject(sorDC, cldata.hClanMark);
			GetObject(cldata.hClanMark,sizeof(BITMAP),&bit);
			bx = bit.bmWidth;
			by = bit.bmHeight;		
			BitBlt(destdc,menuPos[6].left,menuPos[6].top,menuPos[6].right,menuPos[6].bottom,sorDC,0,0,SRCCOPY);
		}
		


		//if((menuMouseOver[2] > 0) ||(menuMouseOver[8] > 0)||(menuMouseOver[9] > 0))
		if(menuMouseOver[2] > 0)
		{			
			SelectObject(sorDC,hTxt_Secession[1]);
			GetObject(hRemove[1],sizeof(BITMAP),&bit);
			bx = bit.bmWidth;
			by = bit.bmHeight;		
			BitBlt(destdc,menuPos[2].left,menuPos[2].top,menuPos[2].right,menuPos[2].bottom,sorDC,0,0,SRCCOPY);
		}
		else
		{			
			SelectObject(sorDC,hTxt_Secession[0]);
			GetObject(hTxt_Secession[0],sizeof(BITMAP),&bit);
			bx = bit.bmWidth;
			by = bit.bmHeight;		
			BitBlt(destdc,menuPos[2].left,menuPos[2].top,menuPos[2].right,menuPos[2].bottom,sorDC,0,0,SRCCOPY);
		}

		//if((menuMouseOver[3] > 0)||(menuMouseOver[10] > 0)||(menuMouseOver[11] > 0))
		if(menuMouseOver[3] > 0)
		{			
			SelectObject(sorDC,hTxt_Help[1]);
			GetObject(hTxt_Help[1],sizeof(BITMAP),&bit);
			bx = bit.bmWidth;
			by = bit.bmHeight;		
			BitBlt(destdc,menuPos[3].left,menuPos[3].top,menuPos[3].right,menuPos[3].bottom,sorDC,0,0,SRCCOPY);
		}
		else
		{			
			SelectObject(sorDC,hTxt_Help[0]);
			GetObject(hTxt_Help[0],sizeof(BITMAP),&bit);
			bx = bit.bmWidth;
			by = bit.bmHeight;		
			BitBlt(destdc,menuPos[3].left,menuPos[3].top,menuPos[3].right,menuPos[3].bottom,sorDC,0,0,SRCCOPY);
		}

		if(menuMouseOver[4] > 0)
		{
			SelectObject(sorDC,hClose[1]);
			GetObject(hClose[1],sizeof(BITMAP),&bit);
			bx = bit.bmWidth;
			by = bit.bmHeight;		
			BitBlt(destdc,menuPos[4].left,menuPos[4].top,menuPos[4].right,menuPos[4].bottom,sorDC,0,0,SRCCOPY);
		}
		else
		{
			SelectObject(sorDC,hClose[0]);
			GetObject(hClose[0],sizeof(BITMAP),&bit);
			bx = bit.bmWidth;
			by = bit.bmHeight;		
			BitBlt(destdc,menuPos[4].left,menuPos[4].top,menuPos[4].right,menuPos[4].bottom,sorDC,0,0,SRCCOPY);
		}

		SelectObject(sorDC, hLine);
		GetObject(hLine,sizeof(BITMAP),&bit);
		StretchBlt(destdc,menuPos[12].left,menuPos[12].top,240,16,sorDC,0,0,16,16,SRCCOPY);

		

		
		DeleteDC(sorDC);
		DeleteDC(destdc);
		ReleaseDC(g_hwnd, hdc );

		break;
	case CLAN_USER_NOMAKE: //클랜 경성하지 못함
		hdc = GetDC(g_hwnd);
		sorDC  = CreateCompatibleDC(hdc);
		destdc = CreateCompatibleDC(hdc);
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
		
			
		
		
		//dsTextLineOut(hdc,menuPos[1].left,menuPos[1].left,szMsg6,lstrlen(szMsg6));
		wsprintf(string,clanClanUserNoMakeMsg.szMsg,cldata.name);
		//Draw_Text(menuPos[1].left,menuPos[1].top,string);
		Text_Out(destdc,menuPos[1].left,menuPos[1].top,string);
		
		

		DeleteDC(sorDC);
		DeleteDC(destdc);
		ReleaseDC(g_hwnd, hdc );
		break;
	
	case CLAN_SECESSION:
		hdc = GetDC(g_hwnd);
		sorDC  = CreateCompatibleDC(hdc);
		destdc = CreateCompatibleDC(hdc);
		SelectObject(destdc, Dback.hb );		//off스크린설정.
			
		SelectObject(sorDC, hClanName);
		GetObject(hClanName,sizeof(BITMAP),&bit);
		bx = bit.bmWidth;
		by = bit.bmHeight;
		BitBlt(destdc,menuPos[0].left,menuPos[0].top,menuPos[0].right,menuPos[0].bottom,sorDC,0,0,SRCCOPY);

		SelectObject(sorDC, hMark_box);
		GetObject(hMark_box,sizeof(BITMAP),&bit);
		bx = bit.bmWidth;
		by = bit.bmHeight;		
		BitBlt(destdc,menuPos[1].left,menuPos[1].top,menuPos[1].right,menuPos[1].bottom,sorDC,0,0,SRCCOPY);

		if(cldata.hClanMark)
		{
			SelectObject(sorDC, cldata.hClanMark);
			GetObject(cldata.hClanMark,sizeof(BITMAP),&bit);
			bx = bit.bmWidth;
			by = bit.bmHeight;		
			BitBlt(destdc,menuPos[7].left,menuPos[7].top,menuPos[7].right,menuPos[7].bottom,sorDC,0,0,SRCCOPY);
		}
		
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

		if(menuMouseOver[3] > 0)
		{
			SelectObject(sorDC,hCancel_Yellow);
			GetObject(hCancel_Yellow,sizeof(BITMAP),&bit);
			
			bx = bit.bmWidth;
			by = bit.bmHeight;		
			BitBlt(destdc,menuPos[3].left,menuPos[3].top,menuPos[3].right,menuPos[3].bottom,sorDC,0,0,SRCCOPY);
		}
		else
		{
			SelectObject(sorDC,hCancel_Gray);
			GetObject(hCancel_Gray,sizeof(BITMAP),&bit);
			bx = bit.bmWidth;
			by = bit.bmHeight;		
			BitBlt(destdc,menuPos[3].left,menuPos[3].top,menuPos[3].right,menuPos[3].bottom,sorDC,0,0,SRCCOPY);
		}
		if(menuMouseOver[2] > 0)
		{
			SelectObject(sorDC,hOk_Yellow);
			GetObject(hOk_Yellow,sizeof(BITMAP),&bit);
			bx = bit.bmWidth;
			by = bit.bmHeight;		
			BitBlt(destdc,menuPos[2].left,menuPos[2].top,menuPos[2].right,menuPos[2].bottom,sorDC,0,0,SRCCOPY);
		}
		else
		{
			SelectObject(sorDC,hOk_Gray);
			GetObject(hOk_Gray,sizeof(BITMAP),&bit);			
			bx = bit.bmWidth;
			by = bit.bmHeight;		
			BitBlt(destdc,menuPos[2].left,menuPos[2].top,menuPos[2].right,menuPos[2].bottom,sorDC,0,0,SRCCOPY);
		}
		
		
		wsprintf(string,clanSecession.szMsg,cldata.name);
		//Draw_Text(menuPos[6].left, menuPos[6].top,string);	
		Text_Out(destdc,menuPos[6].left, menuPos[6].top,string);	

		nStrLen = lstrlen(cldata.name)/2;		
		
		if(nStrLen%2)
		{
			nStrLen = nStrLen/2;
			nStrLen+=1;
		}
		else nStrLen = nStrLen/2;
		
		//dsTextLineOut(hdc,menuPos[8].left - (nStrLen*11),menuPos[8].top,cldata.name,lstrlen(cldata.name));
		Text_Out(destdc,menuPos[8].left - (nStrLen*11),menuPos[8].top,cldata.name);
		

		
		DeleteDC(sorDC);
		DeleteDC(destdc);
		ReleaseDC(g_hwnd, hdc );
		break;
	case CLAN_RECONF_SECESSION:
		hdc = GetDC(g_hwnd);
		sorDC  = CreateCompatibleDC(hdc);
		destdc = CreateCompatibleDC(hdc);
		SelectObject(destdc, Dback.hb );		//off스크린설정.
			
		SelectObject(sorDC, hClanName);
		GetObject(hClanName,sizeof(BITMAP),&bit);
		bx = bit.bmWidth;
		by = bit.bmHeight;
		BitBlt(destdc,menuPos[0].left,menuPos[0].top,menuPos[0].right,menuPos[0].bottom,sorDC,0,0,SRCCOPY);


		SelectObject(sorDC, hMark_box);
			GetObject(hMark_box,sizeof(BITMAP),&bit);
			bx = bit.bmWidth;
			by = bit.bmHeight;		
			BitBlt(destdc,menuPos[1].left,menuPos[1].top,menuPos[1].right,menuPos[1].bottom,sorDC,0,0,SRCCOPY);

		if(cldata.hClanMark)
		{
			

			SelectObject(sorDC, cldata.hClanMark);
			GetObject(cldata.hClanMark,sizeof(BITMAP),&bit);
			bx = bit.bmWidth;
			by = bit.bmHeight;		
			BitBlt(destdc,menuPos[7].left,menuPos[7].top,menuPos[7].right,menuPos[7].bottom,sorDC,0,0,SRCCOPY);
		}
		

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


		if(menuMouseOver[3] > 0)
		{
			SelectObject(sorDC,hCancel_Yellow);
			GetObject(hCancel_Yellow,sizeof(BITMAP),&bit);
			
			bx = bit.bmWidth;
			by = bit.bmHeight;		
			BitBlt(destdc,menuPos[3].left,menuPos[3].top,menuPos[3].right,menuPos[3].bottom,sorDC,0,0,SRCCOPY);
		}
		else
		{
			SelectObject(sorDC,hCancel_Gray);
			GetObject(hCancel_Gray,sizeof(BITMAP),&bit);
			bx = bit.bmWidth;
			by = bit.bmHeight;		
			BitBlt(destdc,menuPos[3].left,menuPos[3].top,menuPos[3].right,menuPos[3].bottom,sorDC,0,0,SRCCOPY);
		}
		if(menuMouseOver[2] > 0)
		{
			SelectObject(sorDC,hOk_Yellow);
			GetObject(hOk_Yellow,sizeof(BITMAP),&bit);
			bx = bit.bmWidth;
			by = bit.bmHeight;		
			BitBlt(destdc,menuPos[2].left,menuPos[2].top,menuPos[2].right,menuPos[2].bottom,sorDC,0,0,SRCCOPY);
		}
		else
		{
			SelectObject(sorDC,hOk_Gray);
			GetObject(hOk_Gray,sizeof(BITMAP),&bit);			
			bx = bit.bmWidth;
			by = bit.bmHeight;		
			BitBlt(destdc,menuPos[2].left,menuPos[2].top,menuPos[2].right,menuPos[2].bottom,sorDC,0,0,SRCCOPY);
		}
		
		
		//Draw_Text(menuPos[6].left, menuPos[6].top,szSecessionMsg);	
		Text_Out(destdc,menuPos[6].left, menuPos[6].top,clanSecessionMsg.szMsg);	

		nStrLen = lstrlen(cldata.name)/2;		
		
		if(nStrLen%2)
		{
			nStrLen = nStrLen/2;
			nStrLen+=1;
		}
		else nStrLen = nStrLen/2;
		
		//dsTextLineOut(hdc,menuPos[8].left - (nStrLen*11),menuPos[8].top,cldata.name,lstrlen(cldata.name));
		Text_Out(destdc,menuPos[8].left - (nStrLen*11),menuPos[8].top,cldata.name);

		
		DeleteDC(sorDC);
		DeleteDC(destdc);
		ReleaseDC(g_hwnd, hdc );

		break;
     case CLANUSER_SUBCHIP_READMSG_FLAG:
	case ISPFLAG_USER:
	case CLAN_USER_ReadDATA:
	case CLAN_RECONF_SECESSION_LOADING:
		hdc = GetDC(g_hwnd);
		sorDC  = CreateCompatibleDC(hdc);

		destdc  = CreateCompatibleDC(hdc);
		SelectObject(destdc, Dback.hb );		//off스크린설정.				
		
		
		//dsTextLineOut(hdc,menuPos[1].left,menuPos[1].left,szMsg6,lstrlen(szMsg6));
		//Draw_Text(menuPos[1].left,menuPos[1].top,szMsg6);
		Text_Out(destdc,menuPos[0].left,menuPos[0].top,clanLoading.szMsg);

		DeleteDC(sorDC);
		DeleteDC(destdc);
		ReleaseDC(g_hwnd, hdc );
		break;
	case CLAN_FINISH_SECESSION:
		hdc = GetDC(g_hwnd);
		sorDC  = CreateCompatibleDC(hdc);
		destdc = CreateCompatibleDC(hdc);
		SelectObject(destdc, Dback.hb );		//off스크린설정.

		SelectObject(sorDC,hButton_Box);
		GetObject(hButton_Box,sizeof(BITMAP),&bit);
		bx = bit.bmWidth;
		by = bit.bmHeight;		
		BitBlt(destdc,menuPos[4].left,menuPos[4].top,menuPos[4].right,menuPos[4].bottom,sorDC,0,0,SRCCOPY);

		SelectObject(sorDC, hClanName);
		GetObject(hClanName,sizeof(BITMAP),&bit);
		bx = bit.bmWidth;
		by = bit.bmHeight;
		BitBlt(destdc,menuPos[0].left,menuPos[0].top,menuPos[0].right,menuPos[0].bottom,sorDC,0,0,SRCCOPY);

		SelectObject(sorDC, hMark_box);
		GetObject(hMark_box,sizeof(BITMAP),&bit);
		bx = bit.bmWidth;
		by = bit.bmHeight;		
		BitBlt(destdc,menuPos[1].left,menuPos[1].top,menuPos[1].right,menuPos[1].bottom,sorDC,0,0,SRCCOPY);

		if(cldata.hClanMark)
		{
			
			SelectObject(sorDC, cldata.hClanMark);
			GetObject(cldata.hClanMark,sizeof(BITMAP),&bit);
			bx = bit.bmWidth;
			by = bit.bmHeight;		
			BitBlt(destdc,menuPos[2].left,menuPos[2].top,menuPos[2].right,menuPos[2].bottom,sorDC,0,0,SRCCOPY);
		}
		

		if(menuMouseOver[3] > 0)
		{
			SelectObject(sorDC,hOk_Yellow);
			GetObject(hOk_Yellow,sizeof(BITMAP),&bit);
			
			bx = bit.bmWidth;
			by = bit.bmHeight;		
			BitBlt(destdc,menuPos[3].left,menuPos[3].top,menuPos[3].right,menuPos[3].bottom,sorDC,0,0,SRCCOPY);
		}
		else
		{
			SelectObject(sorDC,hOk_Gray);
			GetObject(hOk_Gray,sizeof(BITMAP),&bit);
			bx = bit.bmWidth;
			by = bit.bmHeight;		
			BitBlt(destdc,menuPos[3].left,menuPos[3].top,menuPos[3].right,menuPos[3].bottom,sorDC,0,0,SRCCOPY);
		}	
		

		/*wsprintf(string,"%s",szFinishReleaseMsg);
		rect.left		= menuPos[3].left;
		rect.top		= menuPos[3].top ;
		rect.right	= menuPos[3].right ;
		rect.bottom	= menuPos[3].bottom;
		DrawText(GetDC(g_hwnd),string,-1,&rect,DT_LEFT | DT_WORDBREAK);*/
		//dsTextLineOut(GetDC(g_hwnd),menuPos[5].left,menuPos[5].top,szErrorMsg,lstrlen(szErrorMsg) );
		//Draw_Text(menuPos[5].left,menuPos[5].top,szErrorMsg);
		wsprintf(szClanUserMsg,szFinishSecessionMsg,cldata.name,cldata.ChaName);
		Text_Out(destdc,menuPos[5].left,menuPos[5].top,szClanUserMsg);

		nStrLen = lstrlen(cldata.name)/2;		
		
		if(nStrLen%2)
		{
			nStrLen = nStrLen/2;
			nStrLen+=1;
		}
		else nStrLen = nStrLen/2;
		
		//dsTextLineOut(hdc,menuPos[6].left - (nStrLen*11),menuPos[6].top,cldata.name,lstrlen(cldata.name));
		Text_Out(destdc,menuPos[6].left - (nStrLen*11),menuPos[6].top,cldata.name);

		DeleteDC(sorDC);
		DeleteDC(destdc);
		ReleaseDC(g_hwnd, hdc );
		break;

		//클랜 유저 창고
	case CLAN_USER_WAREHOUSE:
		hdc = GetDC(g_hwnd);
		sorDC  = CreateCompatibleDC(hdc);		
		if(menuMouseOver[0] > 0)
		{
			SelectObject(sorDC, hOk_Yellow);
			GetObject(hOk_Yellow,sizeof(BITMAP),&bit);
			bx = bit.bmWidth;
			by = bit.bmHeight;	
			BitBlt(hdc,menuPos[0].left,menuPos[0].top,menuPos[0].right,menuPos[0].bottom,sorDC,0,0,SRCCOPY);
		}
		else
		{
			SelectObject(sorDC, hOk_Gray);
			GetObject(hOk_Gray,sizeof(BITMAP),&bit);
			bx = bit.bmWidth;
			by = bit.bmHeight;	
			BitBlt(hdc,menuPos[0].left,menuPos[0].top,menuPos[0].right,menuPos[0].bottom,sorDC,0,0,SRCCOPY);
		}
		SelectObject(sorDC, hButton_Box);
		GetObject(hButton_Box,sizeof(BITMAP),&bit);
		bx = bit.bmWidth;
		by = bit.bmHeight;	
		BitBlt(hdc,menuPos[1].left,menuPos[1].top,menuPos[1].right,menuPos[1].bottom,sorDC,0,0,SRCCOPY);

		DeleteDC(sorDC);
		ReleaseDC(g_hwnd, hdc );
		
		break;
	case CLAN_USER_HELP:
		hdc = GetDC(g_hwnd);
		sorDC  = CreateCompatibleDC(hdc);

		destdc = CreateCompatibleDC(hdc);
		SelectObject(destdc, Dback.hb );		//off스크린설정.

		SelectObject(sorDC, hButton_Box);
		GetObject(hButton_Box,sizeof(BITMAP),&bit);
		bx = bit.bmWidth;
		by = bit.bmHeight;

		BitBlt(destdc,menuPos[1].left,menuPos[1].top,menuPos[1].right,menuPos[1].bottom,sorDC,0,0,SRCCOPY);
	
		if(menuMouseOver[0] > 0)
		{
			SelectObject(sorDC, hOk_Yellow);
			GetObject(hOk_Yellow,sizeof(BITMAP),&bit);
			bx = bit.bmWidth;
			by = bit.bmHeight;

			BitBlt(hdc,menuPos[0].left,menuPos[0].top,menuPos[0].right,menuPos[0].bottom,sorDC,0,0,SRCCOPY);
		}
		else
		{
			SelectObject(sorDC, hOk_Gray);
			GetObject(hOk_Gray,sizeof(BITMAP),&bit);
			bx = bit.bmWidth;
			by = bit.bmHeight;

			BitBlt(hdc,menuPos[0].left,menuPos[0].top,menuPos[0].right,menuPos[0].bottom,sorDC,0,0,SRCCOPY);
		}
		

		
		nIconPos = 2;

		for(i=0; i < CLANHELPICON_DISPLAY_MAX; i++)
		{				
			if(i == g_nClanIconNum) break;			
			if(menuMouseOver[nIconPos] > 0)
			{
				SelectObject(sorDC, hClanHelpIcon[i+nScl_Bar][0]);
				GetObject(hClanHelpIcon[i+nScl_Bar][0],sizeof(BITMAP),&bit);
				bx = bit.bmWidth;
				by = bit.bmHeight;
				BitBlt(destdc,menuPos[nIconPos].left,menuPos[nIconPos].top,menuPos[nIconPos].right,menuPos[nIconPos].bottom,sorDC,0,0,SRCCOPY);

				
			}
			else
			{
				SelectObject(sorDC, hClanHelpIcon[i+nScl_Bar][1]);
				GetObject(hClanHelpIcon[i+nScl_Bar][1],sizeof(BITMAP),&bit);
				bx = bit.bmWidth;
				by = bit.bmHeight;
				BitBlt(destdc,menuPos[nIconPos].left,menuPos[nIconPos].top,menuPos[nIconPos].right,menuPos[nIconPos].bottom,sorDC,0,0,SRCCOPY);

				
			}
			nIconPos+=1;
		}
	/*	nIconPos=1;
		for(i=0; i < CLANHELPICON_DISPLAY_MAX; i++)
		{
			if(i == g_nClanIconNum) break;
			nIconPos+=1;
									
			strcpy(szClanWon,ClanHelpIcon[i+nScl_Bar].szName);
			//dsTextLineOut(hdc,0,30*i,szClanWon,lstrlen(szClanWon));
			dsTextLineOut(hdc,menuPos[nIconPos].left+ CLANHELPICON_WIDTH + 20,menuPos[nIconPos].top+CLANHELPTEXT_Y,szClanWon,lstrlen(szClanWon));
			
		}		*/
		//스크롤바 그리기
		nIconPos = 6;
		if(g_nClanIconNum>CLANHELPICON_DISPLAY_MAX)
		{
			
			SelectObject(sorDC,hScl_Bar);
			GetObject(hScl_Bar,sizeof(BITMAP),&bit);		
			BitBlt(destdc,menuPos[nIconPos].left,menuPos[nIconPos].top,menuPos[nIconPos].right,menuPos[nIconPos].bottom,sorDC,0,0,SRCCOPY);


			SelectObject(sorDC,hScl_icon);
			GetObject(hScl_icon,sizeof(BITMAP),&bit);
			BitBlt(destdc,scrIconPos.left,scrIconPos.top,16,16,sorDC,0,0,SRCCOPY);
			//nIconPos+=1;

			//SelectObject(sorDC,hScl_icon);
			//GetObject(hScl_icon,sizeof(BITMAP),&bit);			
			//BitBlt(hdc,menuPos[nIconPos].left,menuPos[nIconPos].top,menuPos[nIconPos].right,menuPos[nIconPos].bottom,sorDC,0,0,SRCCOPY);
			
		}
		nIconPos+=1;

		SelectObject(sorDC,hTxt_Help[0]);
		GetObject(hTxt_Help[0],sizeof(BITMAP),&bit);		
		BitBlt(destdc,menuPos[nIconPos].left,menuPos[nIconPos].top,menuPos[nIconPos].right,menuPos[nIconPos].bottom,sorDC,0,0,SRCCOPY);

		

		DeleteDC(destdc);
		DeleteDC(sorDC);
		ReleaseDC(g_hwnd, hdc );

		break;	
	case CLANUSER_MAKE_FIRSTMESSAGE:
		hdc = GetDC(g_hwnd);
			sorDC  = CreateCompatibleDC(hdc);

			destdc  = CreateCompatibleDC(hdc);
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
			
				
			
			
			//dsTextLineOut(hdc,menuPos[1].left,menuPos[1].left,szMsg6,lstrlen(szMsg6));
			//Draw_Text(menuPos[1].left,menuPos[1].top,szMsg6);
			//wsprintf(szDrawMenuMsg,szMakeFinishClanWon,cldata.name);
			Text_Out(destdc,menuPos[1].left,menuPos[1].top,string);

			DeleteDC(sorDC);
			DeleteDC(destdc);
			ReleaseDC(g_hwnd, hdc );
		break;
		case CLANUSER_DOWN6_CLANWON_MESSAGE:
	case CLANUSER_DOWN6_CLANWON:	
		hdc = GetDC(g_hwnd);
			sorDC  = CreateCompatibleDC(hdc);

			destdc  = CreateCompatibleDC(hdc);
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
			
				
			
			
			//dsTextLineOut(hdc,menuPos[1].left,menuPos[1].left,szMsg6,lstrlen(szMsg6));
			//Draw_Text(menuPos[1].left,menuPos[1].top,szMsg6);
			Text_Out(destdc,menuPos[1].left,menuPos[1].top,szClanWon6Down);

			DeleteDC(sorDC);
			DeleteDC(destdc);
			ReleaseDC(g_hwnd, hdc );
		break;
	case CLAN_USER_HELP_CONTENT:
		hdc = GetDC(g_hwnd);
		sorDC  = CreateCompatibleDC(hdc);
		destdc = CreateCompatibleDC(hdc);
		SelectObject(destdc, Dback.hb );		//off스크린설정.

		SelectObject(sorDC, hButton_Box);
		GetObject(hButton_Box,sizeof(BITMAP),&bit);
		bx = bit.bmWidth;
		by = bit.bmHeight;

		BitBlt(destdc,menuPos[1].left,menuPos[1].top,menuPos[1].right,menuPos[1].bottom,sorDC,0,0,SRCCOPY);

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
		
		
		SelectObject(sorDC, hTxt_Help[0]);
		GetObject(hTxt_Help[0],sizeof(BITMAP),&bit);
		bx = bit.bmWidth;
		by = bit.bmHeight;		
		BitBlt(destdc,menuPos[2].left,menuPos[2].top,menuPos[2].right,menuPos[2].bottom,sorDC,0,0,SRCCOPY);

		SelectObject(sorDC,hBt_Help);
		GetObject(hBt_Help,sizeof(BITMAP),&bit);		
		bx = bit.bmWidth;
		by = bit.bmHeight;		
		BitBlt(destdc,menuPos[3].left,menuPos[3].top,menuPos[3].right,menuPos[3].bottom,sorDC,0,0,SRCCOPY);

		//dsTextLineOut(hdc,menuPos[3].left,menuPos[3].top,ClanHelpIcon[g_HelpIconNumber].szName,lstrlen(ClanHelpIcon[g_HelpIconNumber].szName));
		//dsTextLineOut(hdc,0,0,ClanHelpIcon[g_HelpIconNumber].szName,lstrlen(ClanHelpIcon[g_HelpIconNumber].szName));

		SelectObject(sorDC, hLine);
		GetObject(hLine,sizeof(BITMAP),&bit);
		StretchBlt(destdc,menuPos[4].left,menuPos[4].top,240,16,sorDC,0,0,16,16,SRCCOPY);

		nIconPos = 5;
		if(g_HelpDisPlay> HLEPDISPLAY_MAX)
		{
			
			SelectObject(sorDC,hScl_Bar);
			GetObject(hScl_Bar,sizeof(BITMAP),&bit);		
			BitBlt(destdc,menuPos[nIconPos].left,menuPos[nIconPos].top,menuPos[nIconPos].right,menuPos[nIconPos].bottom,sorDC,0,0,SRCCOPY);
			nIconPos+=1;

			SelectObject(sorDC,hScl_icon);
			GetObject(hScl_icon,sizeof(BITMAP),&bit);
			BitBlt(destdc,scrIconPos.left,scrIconPos.top,16,16,sorDC,0,0,SRCCOPY);
			//sinDrawTexImage( hScl_icon_tj , (float)scrIconPos.left, (float)scrIconPos.top, 16, 16);
			//nIconPos+=1;

			//SelectObject(sorDC,hScl_icon);
			//GetObject(hScl_icon,sizeof(BITMAP),&bit);			
			//BitBlt(destdc,menuPos[nIconPos].left,menuPos[nIconPos].top,menuPos[nIconPos].right,menuPos[nIconPos].bottom,sorDC,0,0,SRCCOPY);
		}

		nIconPos=7;
		
		for(i=0; i < HLEPDISPLAY_MAX; i++)
		{
			if(i == g_HelpDisPlay) break;			
			ZeroMemory(szHelpContent,sizeof(char)*HELPWIDTH);		;
									
			strncpy(szHelpContent,HelpContents[i+nScl_Bar].szHelpContent,lstrlen(HelpContents[i+nScl_Bar].szHelpContent));			
			//dsTextLineOut(hdc,menuPos[nIconPos].left,menuPos[nIconPos].top,szHelpContent,lstrlen(szHelpContent));
			Text_Out(destdc,menuPos[nIconPos].left,menuPos[nIconPos].top,szHelpContent);
			nIconPos+=1;
		}		
		

		DeleteDC(sorDC);
		DeleteDC(destdc);
		ReleaseDC(g_hwnd, hdc );
		break;
	case CLANUSER_RELEASEMESSAGE:
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
		Text_Out(destdc,menuPos[1].left,menuPos[1].top,clanClanUserRelease.szMsg);

		
		
		DeleteDC(sorDC);
		DeleteDC(destdc);
		ReleaseDC(g_hwnd, hdc );
		break;
	}

	
#endif
}


