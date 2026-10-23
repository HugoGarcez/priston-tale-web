#define CE_CHIP_CPP
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
#include "cE_chip.h"
#include "tjscroll.h"
#include "cE_Report.h"
#include "cE_CViewClanInfo.h"
#include "..\\isaocheck\\auth.h"
#include "..\\isaocheck\\isaocheck.h"
#include "..\\ygy\\ygyheader.h"
//void CheckMsgPos(CLANMSG* clanmsg,int width, int height);
//*****************************************************************************
CLANEntity_chip::CLANEntity_chip()
{
	Money = 0;
	bClan6Down = FALSE;
	ZeroMemory(szTemp,sizeof(szTemp));
	bRemoveSecondCheck = FALSE;
}

CLANEntity_chip::~CLANEntity_chip()
{

}


//*****************************************************************************
int CLANEntity_chip::Init(int Life)
{
	life = Life;
	bPFlag = FALSE;
	bRelease = FALSE;
	bSubChip = FALSE;
	bisSubChip = FALSE;
	return 1;
}

int CLANEntity_chip::Clear()
{
	return 1;
}


//*****************************************************************************
void CLANEntity_chip::Main()
{
	int i;
	char szClanMark[32];
	if(life ==0) return;

	switch(C_menuN)
	{
#ifdef __CLANSUBCHIP__
	//===================부 클랜칩 임명및 해임  =================start
	case CLANCHIP_NOSUBCHIP:
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
				C_menuN = CLAN_CHIP;
				//C_menuN = CLAN_ERROR_ret;		//나가기 : 끝내는걸로간다.
				menuInit(C_menuN );

				C_menuN2 =-1;
				CLAN_ERROR_ret=-1;
				break;	
			default:
				i = i;
				break;

			}
		}
		
		break;
	case CLANCHIP_SUBCHIP_LOAD_DATA:
		switch(C_menuN2){
			case 0:
				ANSdata.ret_val= -1;
			
				WebDB.Resume() ;		//웹db를 재개한다.
				//클랜의 멤버데이타를 읽는다.
				WebDB.ReadClan(cldata.szID,cldata.gserver,cldata.ClanZang_Name);
#ifdef CLAN_DEBUG
			DebugMessage("CLAN_CHIP_ReadDATA\277\241\274\255 WebDB.ReadClan(\276\306\300\314\265\360 : %s,\260\324\300\323\274\255\271\366 : %s,\304\263\270\257\305\315\300\314\270\247 : %s)",cldata.szID,cldata.gserver,cldata.ClanZang_Name);
#endif
				C_menuN2=1;
				break;
			case 1:
				switch(ANSdata.ret_val) 
				{
				case 0:
					WebDB.Suspend();
					//wsprintf(szErrorMsg, "가입한 클랜이 없음");
					C_menuN = CLAN_ERROR;
					wsprintf(szErrorMsg,"%s (101)",clanszError_Msg.szMsg);
					cClanReport.menuInit(C_menuN,clanszError_Msg.pos.x,clanszError_Msg.pos.y);
					cClanReport.InitMsg(szErrorMsg);				
					CLAN_ERROR_ret=255;		//에러메뉴보여준후그냥끝낸다.
					C_menuN2=-1;
#ifdef CLAN_DEBUG
			DebugMessage("CLAN_CHIP_ReadDATA\277\241\274\255 \270\256\305\317\304\332\265\345 0 \260\241\300\324\307\321 \305\254\267\243\300\314 \276\370\300\275");
#endif
					break;
				case 1:
					WebDB.Suspend();
					ParsigClanWon(ANSdata.ret_Buf);	
					C_menuN = CLANCHIP_SUBCHIP_LIST;
					menuInit(C_menuN);
					break;
				case 100:
					WebDB.Suspend();
					//ygy : 살려야함
					C_menuN = CLAN_ERROR;
					wsprintf(szErrorMsg,"%s (102)",clanszError_Msg.szMsg);
					cClanReport.InitMsg(szErrorMsg);
					cClanReport.menuInit(C_menuN,clanszError_Msg.pos.x,clanszError_Msg.pos.y);
					CLAN_ERROR_ret=CLAN_AllEnd;		//에러메뉴보여준후그냥끝낸다.			
					C_menuN2=-1;
	#ifdef CLAN_DEBUG
				DebugMessage("CLAN_CHIP_ReadDATA\277\241\274\255 \270\256\305\317\304\332\265\345 100");
	#endif
					break;
				case ISAO_ERR_TCP_CONNECT:
					WebDB.Suspend();
					//ygy : 살려야함
					C_menuN = CLAN_ERROR;
					wsprintf(szErrorMsg,"%s (103)",clanszError_Msg.szMsg);
					cClanReport.InitMsg(szErrorMsg);
					cClanReport.menuInit(C_menuN,clanszError_Msg.pos.x,clanszError_Msg.pos.y);
	#ifdef CLAN_DEBUG
				DebugMessage("CLAN_CHIP_ReadDATA\277\241\274\255 \274\255\271\366\277\241 \301\242\274\323\307\322\274\366 \276\370\300\275");
	#endif
					break;
			
				default:
					if(ANSdata.ret_val != -1) {		//전혀엉뚱한 답이 와서 에러가 났다.
					WebDB.Suspend();
					C_menuN = CLAN_AllEnd;				//클랜결성못하니 돌아가라고 하는메뉴로 간다.
					menuInit(C_menuN );

					C_menuN2=-1;
#ifdef CLAN_DEBUG
			DebugMessage("CLAN_CHIP_ReadDATA\277\241\274\255 \265\360\306\372\306\256\277\241\267\257");
#endif
					}
					break;
	
				}
				break;
		}
		break;


	case CLANCHIP_SUBCHIP_READMSG_FLAG:	//클랜장이 부침이 관련메세지를 읽어다는 플래그세팅
		switch(C_menuN2){
			case 0:
				ANSdata.ret_val= -1;			

				WebDB.Resume() ;		//웹db를 재개한다.
				if(cldata.nKFlag == 4) //부칩임명만 들어 왔을때
				{
					WebDB.isKPFlag(cldata.szID,cldata.name,cldata.ChaName,cldata.gserver,"8","0");
				}
				else if(cldata.nKFlag == 16) //부칩해임만 들어 왔을때
				{
					WebDB.isKPFlag(cldata.szID,cldata.name,cldata.ChaName,cldata.gserver,"32","0");
				}
				else if(cldata.nKFlag == 5) //칩위임 부칩임명만 들어 왔을때(칩위임 확인 부칩임명확인플래그 세팅)
				{
					WebDB.isKPFlag(cldata.szID,cldata.name,cldata.ChaName,cldata.gserver,"8","0");
				}
				else if((cldata.nKFlag == 7)&&(cldata.nKFlag_Count)) //칩위임만 확인했고 부침임명확인 안했어 추가로 플래그 세팅
				{
					WebDB.isKPFlag(cldata.szID,cldata.name,cldata.ChaName,cldata.gserver,"8","0");
				}
				else if((cldata.nKFlag == 17)&&(cldata.nKFlag_Count == 1)) //칩위임 부치해임
				{
					WebDB.isKPFlag(cldata.szID,cldata.name,cldata.ChaName,cldata.gserver,"32","0");
				}
				else if((cldata.nKFlag == 19)&&(cldata.nKFlag_Count == 1)) //칩위임 부칩해임에서 칩위임만 확인했을 경우
				{
					WebDB.isKPFlag(cldata.szID,cldata.name,cldata.ChaName,cldata.gserver,"32","0");
				}
				else if((cldata.nKFlag == 20)&&(cldata.nKFlag_Count ==1)&&(bSubChip== TRUE)) //부칩임명 부칩해임만 들어 왔을때(부칩해임만 확인)
				{
					WebDB.isKPFlag(cldata.szID,cldata.name,cldata.ChaName,cldata.gserver,"32","0");
				}
				else if((cldata.nKFlag == 20) &&(cldata.nKFlag_Count == 2)&&(bSubChip== TRUE)) //부칩임명확인  플래그 세팅
				{
					WebDB.isKPFlag(cldata.szID,cldata.name,cldata.ChaName,cldata.gserver,"8","0");
				}
				else if((cldata.nKFlag == 20)&&(cldata.nKFlag_Count ==1)&&(bSubChip== FALSE))
				{
					WebDB.isKPFlag(cldata.szID,cldata.name,cldata.ChaName,cldata.gserver,"8","0");
				}
				else if((cldata.nKFlag == 20) &&(cldata.nKFlag_Count == 2)&&(bSubChip== FALSE)) //부칩임명확인  플래그 세팅
				{
					WebDB.isKPFlag(cldata.szID,cldata.name,cldata.ChaName,cldata.gserver,"32","0");
				}
				else if(cldata.nKFlag == 52) //부칩해임만 확인하여 추가로 부침임명확인 플래그 세팅
				{
					WebDB.isKPFlag(cldata.szID,cldata.name,cldata.ChaName,cldata.gserver,"8","0");
				}
				else if((cldata.nKFlag == 21) && (cldata.nKFlag_Count == 1)&&(bSubChip == TRUE)) //클랜칩 위임 부칩해임 부칩임명 들어 왔을때(칩이임확인,부칩해임 확인)
				{
					WebDB.isKPFlag(cldata.szID,cldata.name,cldata.ChaName,cldata.gserver,"32","0");
				}
				else if((cldata.nKFlag == 21)&& (cldata.nKFlag_Count == 2)&&(bSubChip == TRUE)) //클랜칩 위임 부칩해임 부칩임명 들어 왔을때(칩이임확인,부칩해임 확인,부칩임명 확인)
				{
					WebDB.isKPFlag(cldata.szID,cldata.name,cldata.ChaName,cldata.gserver,"8","0");
				}
				else if((cldata.nKFlag == 21) && (cldata.nKFlag_Count == 1)&&(bSubChip == FALSE)) //클랜칩 위임 부칩해임 부칩임명 들어 왔을때(칩이임확인,부칩해임 확인)
				{
					WebDB.isKPFlag(cldata.szID,cldata.name,cldata.ChaName,cldata.gserver,"8","0");
				}
				else if((cldata.nKFlag == 21)&& (cldata.nKFlag_Count == 2)&&(bSubChip == FALSE)) //클랜칩 위임 부칩해임 부칩임명 들어 왔을때(칩이임확인,부칩해임 확인,부칩임명 확인)
				{
					WebDB.isKPFlag(cldata.szID,cldata.name,cldata.ChaName,cldata.gserver,"32","0");
				}
				else if((cldata.nKFlag == 23)&&(cldata.nKFlag_Count == 1)) //클랜칩 위임 부치해임 부칩임명 에서 클랜칩 확인만 했을 경우
				{
					WebDB.isKPFlag(cldata.szID,cldata.name,cldata.ChaName,cldata.gserver,"32","0");
				}
				else if((cldata.nKFlag == 23)&&(cldata.nKFlag_Count == 2))
				{
					WebDB.isKPFlag(cldata.szID,cldata.name,cldata.ChaName,cldata.gserver,"8","0");
				}
				else if(cldata.nKFlag == 28) //부칩해임 확인 플래그세팅
				{
					WebDB.isKPFlag(cldata.szID,cldata.name,cldata.ChaName,cldata.gserver,"32","0");
				}
				else if(cldata.nKFlag == 55) // 클랜칩 이임 부침해임 부침임명에서(클랜칩이임 확인,부침해임 확인만 했을 경우)
				{
					WebDB.isKPFlag(cldata.szID,cldata.name,cldata.ChaName,cldata.gserver,"8","0");
				}
				else{}
				
				
				
				C_menuN2=1;
#ifdef CLAN_DEBUG
				DebugMessage("CLANCHIP_LEAVECLAN_MESSAGEEND\277\241\274\255 WebDB.isPFlag(\276\306\300\314\265\360 : %s,\305\254\267\243\300\314\270\247 : %s,\304\263\270\257\305\315\300\314\270\247 : %s,\260\324\300\323\274\255\271\366 : %s,\307\303\267\241\261\327 : %s,0)",cldata.szID,cldata.name,cldata.ChaName,cldata.gserver,cldata.PFlag);
#endif
				break;
			case 1:
				switch(ANSdata.ret_val){
					case 0://플래그 삽입 실패
						WebDB.Suspend();
						C_menuN = CLAN_ERROR;
						wsprintf(szErrorMsg,"%s (86)",clanszError_Msg.szMsg);
						cClanReport.menuInit(C_menuN,clanszError_Msg.pos.x,clanszError_Msg.pos.y);
						cClanReport.InitMsg(szErrorMsg);
						C_menuN2=-1;
#ifdef CLAN_DEBUG
				DebugMessage("CLANCHIP_LEAVECLAN_MESSAGEEND\277\241\274\255 \273\360\300\324\275\307\306\320");
#endif
						break;
					case 1: //플래그 삽입 성공							
						WebDB.Suspend();														
						C_menuN2=-1;
						if((cldata.nKFlag == 20)&&(cldata.nKFlag_Count ==1)) //부칩임명 부칩해임만 들어 왔을때(부칩해임만 확인)
						{
							cldata.nKFlag_Count = 2;
							if(bSubChip) C_menuN = CLANCHIP_SUBCHIP_APPOINTMSG;
							else C_menuN = CLANCHIP_SUBCHIP_RELEASEMSG;
							menuInit(C_menuN);
							break;
						}
						else if((cldata.nKFlag == 21) && (cldata.nKFlag_Count == 1)) //클랜칩 위임 부칩해임 부칩임명 들어 왔을때(칩이임확인,부칩해임 확인)
						{
							cldata.nKFlag_Count = 2;
							if(bSubChip==TRUE) C_menuN = CLANCHIP_SUBCHIP_APPOINTMSG;
							else	C_menuN = CLANCHIP_SUBCHIP_RELEASEMSG;
							menuInit(C_menuN);
							break;
						}
						else if((cldata.nKFlag == 23)&&(cldata.nKFlag_Count == 1)) //클랜칩 위임 부치해임 부칩임명 에서 클랜칩 확인만 했을 경우
						{
							cldata.nKFlag_Count = 2;
							C_menuN = CLANCHIP_SUBCHIP_APPOINTMSG;
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
						

						if((strcmp(cldata.stats,"0") == 0)&&(strcmp(cldata.PFlag,"0") == 0))
						{
						wsprintf(szDrawMenuMsg,clanMsg6.szMsg,cldata.nLastDay);
						strcpy(szTemp,clanMsg6.szMsg);
						strcpy(clanMsg6.szMsg,szDrawMenuMsg);					
						CheckMsgPos(&clanMsg6,BOX_MAX_WIDTH,0);
						strcpy(clanMsg6.szMsg,szTemp);

						if(cldata.bIsOneDay)
						{
							//strcpy(clanMsg6.szMsg,szDrawMenuMsg);
							wsprintf(szDrawMenuMsg,clanMsg7.szMsg,cldata.nHour,cldata.nMinute);
							strcpy(szTemp,clanMsg6.szMsg);
							strcpy(clanMsg6.szMsg,szDrawMenuMsg);
							//void CheckMsgPos(CLANMSG* clanmsg);
							CheckMsgPos(&clanMsg6,BOX_MAX_WIDTH,0);	
							strcpy(clanMsg6.szMsg,szTemp);
						}
						else
						{
							wsprintf(szDrawMenuMsg,clanMsg6.szMsg,cldata.nLastDay);

							strcpy(szTemp,clanMsg6.szMsg);

							strcpy(clanMsg6.szMsg,szDrawMenuMsg);
							//void CheckMsgPos(CLANMSG* clanmsg);
							CheckMsgPos(&clanMsg6,BOX_MAX_WIDTH,0);		

							strcpy(clanMsg6.szMsg,szTemp);
						}		

						C_menuN = CLAN_NOMAKE;	//stats가 0이면 6명이하이므로 더 모집하라는메시지보냄
						menuInit(C_menuN );
						bPFlag = FALSE;
						C_menuN2=-1;
						break;
					}
					else if((strcmp(cldata.stats,"1") == 0)&&(strcmp(cldata.PFlag,"0") == 0)) //클랜 결성되고 처음 보여주는 메세지
					{
						strcpy(cldata.PFlag,"2");
						strcpy(szTemp,clanMakeFinishClan.szMsg);
						wsprintf(szDrawMenuMsg,clanMakeFinishClan.szMsg,cldata.name);
						strcpy(clanMakeFinishClan.szMsg,szDrawMenuMsg);					
						//void CheckMsgPos(CLANMSG* clanmsg);
						CheckMsgPos(&clanMakeFinishClan,BOX_MAX_WIDTH,0);
						C_menuN = CLANCHIP_MAKE_FIRSTMESSAGE;	//stats가 0이면 6명이하이므로 더 모집하라는메시지보냄
						menuInit(C_menuN );
						strcpy(clanMakeFinishClan.szMsg,szTemp);
						C_menuN2=-1;
						bPFlag = TRUE;
						break;
					}				
					//else if((strcmp(cldata.stats,"0") == 0)&&(strcmp(cldata.PFlag,"2") == 0)) //클랜원이 6명이라로 떨어졌을 경우
					//{	
					//	strcpy(cldata.PFlag,"3");
					//	C_menuN = CLANCHIP_DOWN6_CLANWON;	
					//	menuInit(C_menuN );									
					//	C_menuN2=-1;
					//	break;
					//}				
					else if((strcmp(cldata.stats,"1") == 0)&&(strcmp(cldata.PFlag,"2") == 0)&&(nClanWon_Num <5))
					{					

						wsprintf(szDrawMenuMsg,clanMsg6.szMsg,cldata.nLastDay);
						strcpy(szTemp,clanMsg6.szMsg);
						strcpy(clanMsg6.szMsg,szDrawMenuMsg);					
						CheckMsgPos(&clanMsg6,BOX_MAX_WIDTH,0);
						strcpy(clanMsg6.szMsg,szTemp);

						if(cldata.bIsOneDay)
						{
							//strcpy(clanMsg6.szMsg,szDrawMenuMsg);
							wsprintf(szDrawMenuMsg,clanMsg7.szMsg,cldata.nHour,cldata.nMinute);
							strcpy(szTemp,clanMsg6.szMsg);
							strcpy(clanMsg6.szMsg,szDrawMenuMsg);
							//void CheckMsgPos(CLANMSG* clanmsg);
							CheckMsgPos(&clanMsg6,BOX_MAX_WIDTH,0);	
							strcpy(clanMsg6.szMsg,szTemp);
						}
						else
						{
							wsprintf(szDrawMenuMsg,clanMsg6.szMsg,cldata.nLastDay);

							strcpy(szTemp,clanMsg6.szMsg);

							strcpy(clanMsg6.szMsg,szDrawMenuMsg);
							//void CheckMsgPos(CLANMSG* clanmsg);
							CheckMsgPos(&clanMsg6,BOX_MAX_WIDTH,0);		

							strcpy(clanMsg6.szMsg,szTemp);
						}		

						C_menuN = CLAN_NOMAKE;	
						menuInit(C_menuN );									
						C_menuN2=-1;
						bPFlag = FALSE;
					}
					//else if((strcmp(cldata.stats,"1") == 0)&&(strcmp(cldata.PFlag,"3") == 0))
					//{
					//	strcpy(cldata.PFlag,"4");
					//	C_menuN = ISPFLAG_CHIP;		//도움말로 돌아감.
					//	menuInit(C_menuN);
					//	C_menuN2 = 0;					
					//}				
					//else if((strcmp(cldata.stats,"0") == 0)&&(strcmp(cldata.PFlag,"4") == 0))
					//{
					//	strcpy(cldata.PFlag,"3");
					//	C_menuN = CLANCHIP_DOWN6_CLANWON;	
					//	menuInit(C_menuN );									
					//	C_menuN2=-1;
					//}
					else // 1:1 or 1:3
					{
						C_menuN = CLAN_CHIP;	//바로 클랜칲메뉴로 간다.
						menuInit(C_menuN );									
						C_menuN2=-1;
						bPFlag = TRUE;
					}
#ifdef CLAN_DEBUG
			DebugMessage("CLANCHIP_LEAVECLAN_MESSAGE\277\241\274\255 \270\256\305\317\304\332\265\345 1 \274\272\260\370");
#endif
						
						
						C_menuN2 = -1;
#ifdef CLAN_DEBUG
				DebugMessage("CLANCHIP_LEAVECLAN_MESSAGEEND\277\241\274\255 \273\360\300\324\274\272\260\370");
#endif
						break;
					default:

						if(ANSdata.ret_val != -1) {		//전혀엉뚱한 답이 와서 에러가 났다.
							WebDB.Suspend();
							C_menuN = CLAN_ERROR;
							wsprintf(szErrorMsg,"%s (87)",clanszError_Msg.szMsg);
							cClanReport.menuInit(C_menuN,clanszError_Msg.pos.x,clanszError_Msg.pos.y);
							cClanReport.InitMsg(szErrorMsg);
							C_menuN2 = -1;
#ifdef CLAN_DEBUG
				DebugMessage("CLANCHIP_LEAVECLAN_MESSAGEEND\277\241\274\255 \265\360\306\372\306\256\277\241\267\257");
#endif
						}						
						break;
				}
		}
		break;	
	case CLANCHIP_SUBCHIP_APPOINT_FLAG:	//클랜부침이 임명되어다는 플래그세팅
		switch(C_menuN2){
			case 0:
				ANSdata.ret_val= -1;			

				WebDB.Resume() ;		//웹db를 재개한다.
				
				WebDB.isKPFlag(cldata.szID,cldata.name,cldata.ChaName,cldata.gserver,"200","1");
				
				C_menuN2=1;
#ifdef CLAN_DEBUG
				DebugMessage("CLANCHIP_LEAVECLAN_PFLAG\277\241\274\255 WebDB.isPFlag(\276\306\300\314\265\360 : %s,\305\254\267\243\300\314\270\247 : %s,\304\263\270\257\305\315\300\314\270\247 : %s,\260\324\300\323\274\255\271\366 : %s,\307\303\267\241\261\327 : %s,0)",cldata.szID,cldata.name,cldata.ChaName,cldata.gserver,cldata.PFlag);
#endif
				break;
			case 1:
				switch(ANSdata.ret_val){
					case 0://플래그 삽입 실패
						WebDB.Suspend();
						C_menuN = CLAN_ERROR;
						wsprintf(szErrorMsg,"%s (88)",clanszError_Msg.szMsg);
						cClanReport.menuInit(C_menuN,clanszError_Msg.pos.x,clanszError_Msg.pos.y);
						cClanReport.InitMsg(szErrorMsg);
						C_menuN2=-1;
#ifdef CLAN_DEBUG
				DebugMessage("CLANCHIP_LEAVECLAN_PFLAG\277\241\274\255 \273\360\300\324\275\307\306\320");
#endif
						break;
					case 1: //플래그 삽입 성공							
						WebDB.Suspend();														
						C_menuN2=-1;
						//C_menuN = CLAN_CHIP;						
						C_menuN = CLANCHIP_LEAVECLAN_SUCESS;
						menuInit(C_menuN);
						
						C_menuN2 = -1;
#ifdef CLAN_DEBUG
				DebugMessage("CLANCHIP_LEAVECLAN_PFLAG\277\241\274\255 \273\360\300\324\274\272\260\370");
#endif
						break;
					default:

						if(ANSdata.ret_val != -1) {		//전혀엉뚱한 답이 와서 에러가 났다.
							WebDB.Suspend();
							C_menuN = CLAN_ERROR;
							wsprintf(szErrorMsg,"%s (89)",clanszError_Msg.szMsg);
							cClanReport.menuInit(C_menuN,clanszError_Msg.pos.x,clanszError_Msg.pos.y);
							cClanReport.InitMsg(szErrorMsg);
							C_menuN2 = -1;
#ifdef CLAN_DEBUG
				DebugMessage("CLANCHIP_LEAVECLAN_PFLAG\277\241\274\255 \265\360\306\372\306\256\277\241\267\257");
#endif
						}						
						break;
				}
		}
		break;	
	case CLANCHIP_SUBCHIP_APPOINTMSG:		//클랜부침이 임명되다고 한번 보여줌
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
				
				C_menuN = CLANCHIP_SUBCHIP_READMSG_FLAG;
				//C_menuN = CLAN_ERROR_ret;		//나가기 : 끝내는걸로간다.
				menuInit(C_menuN );

				C_menuN2 =0;
				CLAN_ERROR_ret=-1;
				break;	
			default:
				i = i;
				break;

			}
		}
		break;
	case CLANCHIP_SUBCHIP_RELEASEMSG:		//클랜부침이 해임되다고 한번 보여줌	
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
				C_menuN = CLANCHIP_SUBCHIP_READMSG_FLAG;				
				menuInit(C_menuN );

				C_menuN2 =0;
				CLAN_ERROR_ret=-1;
				break;	
			default:
				i = i;
				break;

			}
		}
		break;
	case CLANCHIP_SUBCHIP_LIST:			//클랜원들 리스트 보여줌 부 클랜칩이 있으면 따로 보여줌
		i = chkeckMenuN();
		switch(i) {
		case 0:
			menuMouseOver[0] = 1;
			break;
		case 1:
			menuMouseOver[1] = 1;
			break;		
		default:
			memset(menuMouseOver,-1,sizeof(int)*30);
			break;
		}

		//ktj : 삭제
			//ygy : 휠관련한 변수 TJwheel살려야함
			if(TJwheel != 0) {			//ktj : wheel>0 휠 앞으로 굴리기  wheel<0 휠 뒤로 굴리기
				scrIconPos.top = tscr.ret_WHEELpoint(TJwheel , scrIconPos.top);
				nScl_Bar = tscr.topScrollLineNum(scrIconPos.top);
				TJwheel = 0;
			}


		//ygy : 살려야함
		if(MOUSE0())			
		{
			switch(i) 
			{
			case 12:
				nScl_Bar = tscr.topScrollLineNum(pCursorPos.y);
				//ktj : 스크롤 아이콘 위치재설정
				scrIconPos.top =  pCursorPos.y-8;//BackStartPos.y + SCROOBAR_TOP;
				break;

			case 4:
			case 5:
			case 6:
			case 7:
			case 8:		//추방할사람을 설정한다.
			case 9:
			case 10:
			case 11:				
				//if(nReleaseClanWon >= 5){	
					if(banisF[i-4+nScl_Bar]>0){
						banisF[i-4+nScl_Bar] = !banisF[i-4+nScl_Bar];
				//		nReleaseClanWon-=1;
						nSavePos = i-4+nScl_Bar;
					}
					else
					{
						ZeroMemory(banisF,sizeof(banisF));
						banisF[i-4+nScl_Bar] = !banisF[i-4+nScl_Bar];
						nSavePos = i-4+nScl_Bar;
					}
				//}
				//else{
					
				//	banisF[i-4+nScl_Bar] = !banisF[i-4+nScl_Bar];
				//	if(banisF[i-4+nScl_Bar]){
				//		nReleaseClanWon+=1;
				//	}
				//	else{
				//		nReleaseClanWon-=1;
				//	}
				//}

				//ygy :살려야함
				MOUSE0()=0;
				break;
			case 15:
			case 16:
			case 17:
			case 18:
			case 19:
			case 20:
			case 21:
			case 22:
				//if(nReleaseClanWon >= 5){	
					if(banisF[i-15+nScl_Bar]>0){
						banisF[i-15+nScl_Bar] = !banisF[i-15+nScl_Bar];
						nSavePos = i-14+nScl_Bar;
				//		nReleaseClanWon-=1;
					}
					else
					{
						ZeroMemory(banisF,sizeof(banisF));
						banisF[i-15+nScl_Bar] = !banisF[i-15+nScl_Bar];
						nSavePos = i-15+nScl_Bar;
					}
			//	}
			//	else{
					
			//		banisF[i-14+nScl_Bar] = !banisF[i-14+nScl_Bar];
			//		if(banisF[i-14+nScl_Bar]){
			//			nReleaseClanWon+=1;
			//		}
			//		else{
			//			nReleaseClanWon-=1;
			//		}
			//	}

				//ygy : 살려야함
				MOUSE0()=0;
				break;
		
			case 0: //이임 ======================== 프로그램 짤것....
				
				C_menuN2 = 0;		//ktj : php로 추방할 데이타를  집어넣어 실행하는 루틴으로 간다.
				if(nSavePos < 0)
				{
					C_menuN = CLAN_CHIP;
				}
				else
				{
					//해임인지 임명인지 구분.
					if(strcmp(cldata.clanWon[nSavePos],cldata.SubChip) == 0)
					{
						C_menuN = CLANCHIP_RELEASESUBCHIP;
						bisSubChip = FALSE;
					}
					else if((cldata.clanWon[nSavePos][0] !=0)&&(cldata.SubChip[0] !=0)&&(strcmp(cldata.clanWon[nSavePos],cldata.SubChip) != 0))
					{
						C_menuN = CLANCHIP_RELEASESUBCHIP;
						bisSubChip = TRUE;
					}
					else
					{						
						C_menuN = CLANCHIP_RESUBCHIP_APPOINT;						
					}
					
				}
				
				menuInit(C_menuN);
				//ygy : 살려야함
				MOUSE0()=0;
				break;
			case 1:				
				C_menuN = CLAN_CHIP;				//닫기 : 그냥 돌아감.
				menuInit(C_menuN);
				//ygy : 살려야함
				MOUSE0()=0;
				break;
			default:
				//ygy : 살려야함
				MOUSE0()=0;
				break;
			}
		}
		break;
	case CLANCHIP_RESUBCHIP_APPOINT:		//부 클랜칩 임명 재확인
		i = chkeckMenuN();				//================================================
		switch(i) {
		case 0:		//yes
			menuMouseOver[0] = 1;
			break;
		case 1:		//no
			menuMouseOver[1] = 1;
			break;
		default:
			memset(menuMouseOver,-1,sizeof(int)*30);
		}
		//ygy : 꼭 살릴것
		if(MOUSE0())
		{
			MOUSE0()=0;
			switch(i)
			{
			case 0:
				//ktj : 이부분 다시 열것:

				C_menuN2 =1;
				C_menuN = CLANCHIP_APPOINTSUBCHIP_DO;			//Y : 클랜결성하겟다.  대금지불의사로간다.
				menuInit(C_menuN );
				
				break;

			case 1: //클랜 만들기를 취소
				//ygy : 꼭 살릴것
				
				C_menuN = CLAN_CHIP;			//취소				
				menuInit(C_menuN);				
				break;

			default:
				break;
			}
		}
		break;
	case CLANCHIP_RESUBCHIP_APPOINT_END:	//부 클랜칩임명 마지막 메세지
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
				C_menuN = CLAN_CHIP;
				//C_menuN = CLAN_ERROR_ret;		//나가기 : 끝내는걸로간다.
				menuInit(C_menuN );
				C_menuN2 =-1;
				//CLAN_ERROR_ret=-1;
				break;	
			default:
				i = i;
				break;

			}
		}
		break;
	case CLANCHIP_RELEASESUBCHIP:			//부 클랜칩해임 재확인
		i = chkeckMenuN();				//================================================
		switch(i) {
		case 0:		//yes
			menuMouseOver[0] = 1;
			break;
		case 1:		//no
			menuMouseOver[1] = 1;
			break;
		default:
			memset(menuMouseOver,-1,sizeof(int)*30);
		}
		//ygy : 꼭 살릴것
		if(MOUSE0())
		{	
			MOUSE0()=0;
			switch(i) 
			{
			case 0:
				//ktj : 이부분 다시 열것:

				C_menuN2 =1;
				C_menuN = CLANCHIP_RELEASESUBCHIP_DO;			//Y : 클랜결성하겟다.  대금지불의사로간다.
				menuInit(C_menuN );
				
				break;

			case 1: //클랜 만들기를 취소
				//ygy : 꼭 살릴것
				
				C_menuN = CLAN_CHIP;			//취소				
				menuInit(C_menuN);				
				break;

			default:
				break;
			}
		}
		break;
	case CLANCHIP_RELEASESUBCHIP_END:		//클랜칩 해임 마지막 메세지
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
				ZeroMemory(cldata.SubChip,sizeof(cldata.SubChip));
				C_menuN = CLAN_CHIP;
				//C_menuN = CLAN_ERROR_ret;		//나가기 : 끝내는걸로간다.
				menuInit(C_menuN );

				C_menuN2 =-1;
				CLAN_ERROR_ret=-1;
				break;	
			default:
				i = i;
				break;

			}
		}
		break;
	case CLANCHIP_APPOINTSUBCHIP_DO:		//클랜부칩 임명 ASP호출
		switch(C_menuN2) {
		case 1:
			ANSdata.ret_val= -1;
			WebDB.Resume() ;		//웹db를 재개한다.
			WebDB.AppointSubChip(cldata.szID,cldata.ChaName,cldata.name,cldata.gserver,cldata.clanWon[nSavePos]);
			C_menuN2=2;
			
			break;
		case 2:
			switch(ANSdata.ret_val) 
			{
			case 0: //클랜원이 아님
				WebDB.Suspend(); 
				C_menuN2=0;
				
				C_menuN = CLAN_ERROR;
				wsprintf(szErrorMsg,"%s (90)", clanszError_Msg.szMsg);
				cClanReport.InitMsg(szErrorMsg);
				cClanReport.menuInit(C_menuN,clanszError_Msg.pos.x,clanszError_Msg.pos.y);
				CLAN_ERROR_ret=CLAN_AllEnd;		//에러메뉴보여준후 다시 돌아갈곳.
				break;
			case 3: //부클랜칩이 존재
				WebDB.Suspend(); 
				C_menuN2=0;
				
				C_menuN = CLAN_ERROR;
				wsprintf(szErrorMsg,"%s (100)", clanszError_Msg.szMsg);
				cClanReport.InitMsg(szErrorMsg);
				cClanReport.menuInit(C_menuN,clanszError_Msg.pos.x,clanszError_Msg.pos.y);
				CLAN_ERROR_ret=CLAN_AllEnd;		//에러메뉴보여준후 다시 돌아갈곳.
				break;
			case 2: //클랜칩니거나 부클랜칩
				WebDB.Suspend(); 
				C_menuN2=0;
				
				C_menuN = CLAN_ERROR;
				wsprintf(szErrorMsg,"%s (91)", clanszError_Msg.szMsg);
				cClanReport.InitMsg(szErrorMsg);
				cClanReport.menuInit(C_menuN,clanszError_Msg.pos.x,clanszError_Msg.pos.y);
				CLAN_ERROR_ret=CLAN_AllEnd;		//에러메뉴보여준후 다시 돌아갈곳.
				break;
			case 1: //부클랜칩 임명 성공				
				WebDB.Suspend(); 
				C_menuN2=0;
				strcpy(cldata.SubChip,cldata.clanWon[nSavePos]);
				//C_menuN = CLANCHIP_SUBCHIP_APPOINT_FLAG;
				C_menuN = CLANCHIP_RESUBCHIP_APPOINT_END; //부클랜칩 임명 마지막 메세지 출력
				menuInit(C_menuN);


				void SetClanWon(); //서버에게 업데이트할것을 알림
				SetClanWon();
				break;		


			case 100:								//클랜을 만든것이 성공(클랜마크설명메뉴로..)				
				WebDB.Suspend(); 
				C_menuN2=0;
				
				C_menuN = CLAN_ERROR;
				wsprintf(szErrorMsg,"%s (92)", clanszError_Msg.szMsg);
				cClanReport.InitMsg(szErrorMsg);
				cClanReport.menuInit(C_menuN,clanszError_Msg.pos.x,clanszError_Msg.pos.y);
				CLAN_ERROR_ret=CLAN_AllEnd;		//에러메뉴보여준후 다시 돌아갈곳.
				break;
			case ISAO_ERR_TCP_CONNECT:
				WebDB.Suspend();				
				C_menuN = CLAN_ERROR;
				wsprintf(szErrorMsg,"%s (93)", clanszError_Msg.szMsg);
				cClanReport.InitMsg(szErrorMsg);
				cClanReport.menuInit(C_menuN,clanszError_Msg.pos.x,clanszError_Msg.pos.y);
				
				break;
			default:
				if(ANSdata.ret_val != -1) {		//전혀엉뚱한 답이 와서 에러가 났다.
					WebDB.Suspend();
					C_menuN2=0;					
					C_menuN = CLAN_ERROR;
					wsprintf(szErrorMsg,"%s (94)", clanszError_Msg.szMsg);
					cClanReport.InitMsg(szErrorMsg);
					cClanReport.menuInit(C_menuN,clanszError_Msg.pos.x,clanszError_Msg.pos.y);
					CLAN_ERROR_ret=CLAN_AllEnd;		//에러메뉴보여준후 다시 돌아갈곳.
				}
				break;
			}
		}
		break;
	case CLANCHIP_SUBCHIP_LOAD:
		break;
	case CLANCHIP_RELEASESUBCHIP_DO:		//클랜부칩 해임 ASP호출
		switch(C_menuN2) {
		case 1:
			ANSdata.ret_val= -1;
			WebDB.Resume() ;		//웹db를 재개한다.
			
			WebDB.ReleaseSubChip(cldata.szID,cldata.ChaName,cldata.name,cldata.gserver,cldata.SubChip);//cldata.clanWon[nSavePos]);
			C_menuN2=2;
			
			break;
		case 2:
			switch(ANSdata.ret_val) 
			{
			case 0:
				WebDB.Suspend(); 
				C_menuN2=0;
				
				C_menuN = CLAN_ERROR;
				wsprintf(szErrorMsg,"%s (95)", clanszError_Msg.szMsg);
				cClanReport.InitMsg(szErrorMsg);
				cClanReport.menuInit(C_menuN,clanszError_Msg.pos.x,clanszError_Msg.pos.y);
				CLAN_ERROR_ret=CLAN_AllEnd;		//에러메뉴보여준후 다시 돌아갈곳.
				break;
			case 2: //클랜칩이거나 클랜원일 경우
				WebDB.Suspend(); 
				C_menuN2=0;
				
				C_menuN = CLAN_ERROR;
				wsprintf(szErrorMsg,"%s (96)", clanszError_Msg.szMsg);
				cClanReport.InitMsg(szErrorMsg);
				cClanReport.menuInit(C_menuN,clanszError_Msg.pos.x,clanszError_Msg.pos.y);
				CLAN_ERROR_ret=CLAN_AllEnd;		//에러메뉴보여준후 다시 돌아갈곳.
				break;
			case 1: //부클랜칩 해임 성공				
				WebDB.Suspend(); 
				C_menuN2=0;
				//C_menuN = CLANCHIP_SUBCHIP_RELEASE_FLAG;				
				C_menuN = CLANCHIP_RELEASESUBCHIP_END;
				menuInit(C_menuN);


				void SetClanWon(); //서버에게 업데이트할것을 알림
				SetClanWon();

				if(bisSubChip)
				{
					C_menuN = CLANCHIP_RESUBCHIP_APPOINT;
					menuInit(C_menuN);
				}
				break;
			
			case 100:								//클랜을 만든것이 성공(클랜마크설명메뉴로..)				
				WebDB.Suspend(); 
				C_menuN2=0;
				
				C_menuN = CLAN_ERROR;
				wsprintf(szErrorMsg,"%s (97)", clanszError_Msg.szMsg);
				cClanReport.InitMsg(szErrorMsg);
				cClanReport.menuInit(C_menuN,clanszError_Msg.pos.x,clanszError_Msg.pos.y);
				CLAN_ERROR_ret=CLAN_AllEnd;		//에러메뉴보여준후 다시 돌아갈곳.
				break;
			case ISAO_ERR_TCP_CONNECT:
				WebDB.Suspend();				
				C_menuN = CLAN_ERROR;
				wsprintf(szErrorMsg,"%s (98)", clanszError_Msg.szMsg);
				cClanReport.InitMsg(szErrorMsg);
				cClanReport.menuInit(C_menuN,clanszError_Msg.pos.x,clanszError_Msg.pos.y);
				
				break;
			default:
				if(ANSdata.ret_val != -1) {		//전혀엉뚱한 답이 와서 에러가 났다.
					WebDB.Suspend();
					C_menuN2=0;					
					C_menuN = CLAN_ERROR;
					wsprintf(szErrorMsg,"%s (99)", clanszError_Msg.szMsg);
					cClanReport.InitMsg(szErrorMsg);
					cClanReport.menuInit(C_menuN,clanszError_Msg.pos.x,clanszError_Msg.pos.y);
					CLAN_ERROR_ret=CLAN_AllEnd;		//에러메뉴보여준후 다시 돌아갈곳.
				}
				break;
			}
		}
		break;
//==========================부칩 임명 및 해임 ==============end
#endif
	
	case CLANCHIP_GO_RELEASEMESSAGE:
		C_menuN = CLANCHIP_RELEASEMESSAGE;
		menuInit(C_menuN);
		break;
	case CLANCHIP_RELEASEMESSAGE:
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
				
				C_menuN = CLANCHIP_RELEASE_FLAG;
				//C_menuN = CLAN_ERROR_ret;		//나가기 : 끝내는걸로간다.
				menuInit(C_menuN );
				C_menuN2 =0;

				break;	
			default:
				i = i;
				break;

			}
		}
		break;
//===============서버가 처리중일때 로드 메뉴 보여줌
	case CLAN_REMOVE_SHORTCLANWON_LOADING:
		if(ANSdata.ret_val !=-1) C_menuN = CLAN_REMOVE_SHORTCLANWON;
		break;
	case CLAN_REMOVE_LOADING:
		if(ANSdata.ret_val !=-1) C_menuN = CLAN_REMOVE;
		break;
	case CLAN_RELEASE_LOADING: 
		if(ANSdata.ret_val !=-1) C_menuN = CLAN_RECONF_RELEASE;
		break;	
	case CLAN_CHIP:
		i = chkeckMenuN();
		memset(menuMouseOver,-1,sizeof(menuMouseOver));
		switch(i) {
			
		case 2: //클랜 마크
			menuMouseOver[2] =1;
			break;
		case 3: //추방
#ifndef _LANGUAGE_BRAZIL	//브라질 클랜 //해외
			menuMouseOver[3] =1;
#endif
			break;
		case 4: //클랜 해체
#ifndef _LANGUAGE_BRAZIL	//브라질 클랜 //해외
			menuMouseOver[4] =1;
#endif
			break;

		case 5: //도움말
			menuMouseOver[5] =1;
			break;
		case 6: //나가기

			menuMouseOver[6] =1;
			break;
		case 7: //테두리			
			menuMouseOver[7] =1;
			break;		
		case 18: //이임
#ifndef _LANGUAGE_BRAZIL	//브라질 클랜 //해외
			if(cldata.stats[0] == '0') menuMouseOver[18] =0;
			else menuMouseOver[18] =1;
#endif
			break;
		//ktj : 클랜메뉴중 부클랜칲 임명메뉴를 일단 삭제

#ifdef __CLANSUBCHIP__
		case 19: //부칩임명및 해임
			if(cldata.stats[0] == '0') menuMouseOver[19] =0;
			else menuMouseOver[19] =1;			
			break;
#endif

		default:
			memset(menuMouseOver,-1,sizeof(menuMouseOver));
			break;
		}
		//ygy : 살려야함
		if(MOUSE0())			
		{	
			//ygy : 살려야함
			MOUSE0()=0;

			switch(i) 
			{		
			case 2: //클랜 마크 : 보류
				//ygy : 살려야함				
				wsprintf(szClanMark, "%s", cldata.ClanMark);
				nMark = chkMark(szClanMark);
				switch(nMark)
				{
				case 0:  //클라이언트,서버 두곳에 클랜 마크이미지가 없음
					C_menuN = CLAN_MARK1;
					menuInit(CLAN_MARK1);
					break;
				case 1: //클라이언트에만 이미지가 있음
					C_menuN = CLAN_MARK2;
					menuInit(CLAN_MARK2);
					break;
				case 2: //클라이언트 서버 두곳에 이미지가 있음
					C_menuN = CLAN_MARK3;
					menuInit(CLAN_MARK3);
					break;
				}
				break;
			case 3: //추방
#ifndef _LANGUAGE_BRAZIL	//브라질 클랜 //해외
				ZeroMemory(banisF, sizeof(banisF));
				ZeroMemory(banisBuf1, sizeof(banisBuf1));
				ZeroMemory(banisBuf2, sizeof(banisBuf2));
				banisCnt=1;

				C_menuN = CLAN_REMOVE;
				menuInit(CLAN_REMOVE);
				C_menuN2 = -1;
#endif
				break;
			case 4: //클랜 해체 
#ifndef _LANGUAGE_BRAZIL	//브라질 클랜 //해외
				C_menuN2 = -1;
				C_menuN = CLAN_RELEASE;
				menuInit(CLAN_RELEASE);
#endif
				break;
			case 5: //도움말
				//ygy : 살려야함
				//ReadClanHelpScr(CLANCHIPHELPINF);
				LoadHelpImage();
				C_menuN2 = -1;
				//C_menuN = CLAN_HELP;
				LoadHelp(HELPCONTENT_TXT);
				C_menuN = CLAN_HELP_CONTENT;
				menuInit(C_menuN);
				
				break;
			case 6: //나가기				
				C_menuN2 = -1;
				C_menuN = CLAN_AllEnd;		//나가기 : 끝내는걸로간다.
				break;
			case 18: //이임
#ifndef _LANGUAGE_BRAZIL	//브라질 클랜 //해외
				if(cldata.stats[0] == '0')
				{
					C_menuN = CLANCHIP_NOLEAVECLAN;
				}
				else
				{
					C_menuN = CLANCHIP_LEAVECLAN_CONFIRM;
				}
				
				menuInit(C_menuN);
#endif
				break;
			//ktj : 클랜메뉴중 부클랜칲 임명메뉴를 일단 삭제
			
#ifdef __CLANSUBCHIP__
			case 19:
				
				//C_menuN = CLANCHIP_SUBCHIP_LIST;
				if(cldata.stats[0] == '0')
				{
					C_menuN = CLANCHIP_NOSUBCHIP;
					menuInit(C_menuN);
					
				}
				else
				{
					C_menuN = CLANCHIP_SUBCHIP_LOAD_DATA;
					menuInit(C_menuN);
					C_menuN2 = 0;
				}
				
				break;
				
#endif
			default:
				break;
			}
		}
		break;
	case CLAN_WAREHOUSE:					//창고
		wsprintf(szErrorMsg, "\303\242\260\355\270\336\264\272\264\302 \301\330\272\361\301\337\300\324\264\317\264\331.");
		C_menuN = CLAN_ERROR;
		menuInit(C_menuN);
		CLAN_ERROR_ret=CLAN_CHIP;
		break;
	

	case CLAN_MARK1: //클라이언트,서버에 두 곳에 클랜 마크가 존재하지 않음
		i = chkeckMenuN();
		switch(i) {
		case 0:
			menuMouseOver[0] = 1;
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
				C_menuN = 4;		//전 단계로 돌아감
				menuInit(C_menuN);
				break;				
			default:
				break;
			}
		}		
		break;
	case CLAN_MARK2: //클라이언트에만 클랜 마크가 존재함
		i = chkeckMenuN();
		switch(i) {
		case 0:
			menuMouseOver[0] = 1;
			break;
		case 1:
			menuMouseOver[1] = 1;
			break;
		default:
			memset(menuMouseOver,-1,sizeof(int)*30);
			break;
		}
		//ygy : mouse0살려야함
		if(MOUSE0())			
		{	
			MOUSE0()=0;
			switch(i) 
			{
			case 0:	//확인 :  클랜 마크를 등록함			
				break;				
			case 1: //취소
				C_menuN = CLAN_CHIP;		//전 단계로 돌아감
				menuInit(C_menuN);
				break;
			default:
				break;
			}
		}		
		break;
	case CLAN_MARK3: //클라이언트,서버에 클랜 마크가 존재함
		i = chkeckMenuN();
		switch(i) {
		case 0:
			menuMouseOver[0] = 1;
			break;	
		case 1:
			menuMouseOver[1] = 1;
			break;
		default:
			memset(menuMouseOver,-1,sizeof(int)*30);
			break;
		}
		//ygy : mouse0살려야함
		if(MOUSE0())			
		{	
			MOUSE0()=0;
			switch(i) 
			{
			case 0:	//확인 :  클랜 마크를 교체
				break;				
			case 1: //취소
				C_menuN = CLAN_CHIP;		//전 단계로 돌아감
				menuInit(C_menuN);
				break;
			default:
				break;
			}
		}		
		break;




	//==========이임===========================start

	case CLANCHIP_LEAVECLAN_MESSAGE: //이임이 되고나서 npc를 만날때 클랜칩이 바뀌었다는 메세지를 보여줌
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
				C_menuN = CLANCHIP_LEAVECLAN_MESSAGEEND;
				menuInit(C_menuN);
				C_menuN2 = 0;
				break;
			default:
				i = i;
				break;

			}
		}
		break;
	case CLANCHIP_LEAVECLAN_NOLEVEL: //레벨이 50미만일 경우의 메세지 처리
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
				C_menuN = CLAN_CHIP;				
				menuInit(C_menuN );

				C_menuN2 =-1;
				CLAN_ERROR_ret=-1;
				break;	
			default:
				i = i;
				break;

			}
		}		
		break;
	case CLANCHIP_LEAVECLANDOWNLEVEL: //클랜장보다 수용할수 있는 클랜원수가 적을때 보여주는 메세지
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
				C_menuN = CLAN_CHIP;
				//C_menuN = CLAN_ERROR_ret;		//나가기 : 끝내는걸로간다.
				menuInit(C_menuN );

				C_menuN2 =-1;
				CLAN_ERROR_ret=-1;
				break;	
			default:
				i = i;
				break;

			}
		}
		break;

	case CLANCHIP_NOLEAVECLAN:  //클랜이 미등록 상태이기때문에 이임할수 없다는 메세지 처리
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
				C_menuN = CLAN_CHIP;
				//C_menuN = CLAN_ERROR_ret;		//나가기 : 끝내는걸로간다.
				menuInit(C_menuN );

				C_menuN2 =-1;
				CLAN_ERROR_ret=-1;
				break;	
			default:
				i = i;
				break;

			}
		}
		break;
	case CLANCHIP_LEAVECLAN_MESSAGEEND: //npc를 만나고나서 클랜장이 바뀌었다는 메세지를 읽어다고 플래그 값 201을 넣음
		switch(C_menuN2){
			case 0:
				ANSdata.ret_val= -1;			

				WebDB.Resume() ;		//웹db를 재개한다.
				
				if((cldata.nKFlag == 1)&&(cldata.nKFlag_Count == 1)) //클랜칩 위임
				{
					WebDB.isKPFlag(cldata.szID,cldata.name,cldata.ChaName,cldata.gserver,"2","0");
				}				
				else if((cldata.nKFlag == 5)&&(cldata.nKFlag_Count == 1)) //칩이임 부침임명
				{
					WebDB.isKPFlag(cldata.szID,cldata.name,cldata.ChaName,cldata.gserver,"2","0");
				}
				else if((cldata.nKFlag == 17)&&(cldata.nKFlag_Count == 1)) //칩이임 부침해임
				{
					WebDB.isKPFlag(cldata.szID,cldata.name,cldata.ChaName,cldata.gserver,"2","0");
				}
				else if((cldata.nKFlag == 21)&&(cldata.nKFlag_Count == 1)) //칩이임 부침해임
				{
					WebDB.isKPFlag(cldata.szID,cldata.name,cldata.ChaName,cldata.gserver,"2","0");
				}
				else{}
				
				C_menuN2=1;
#ifdef CLAN_DEBUG
				DebugMessage("CLANCHIP_LEAVECLAN_MESSAGEEND\277\241\274\255 WebDB.isPFlag(\276\306\300\314\265\360 : %s,\305\254\267\243\300\314\270\247 : %s,\304\263\270\257\305\315\300\314\270\247 : %s,\260\324\300\323\274\255\271\366 : %s,\307\303\267\241\261\327 : %s,0)",cldata.szID,cldata.name,cldata.ChaName,cldata.gserver,cldata.PFlag);
#endif
				break;
			case 1:
				switch(ANSdata.ret_val){
					case 0://플래그 삽입 실패
						WebDB.Suspend();
						C_menuN = CLAN_ERROR;
						wsprintf(szErrorMsg,"%s (86)",clanszError_Msg.szMsg);
						cClanReport.menuInit(C_menuN,clanszError_Msg.pos.x,clanszError_Msg.pos.y);
						cClanReport.InitMsg(szErrorMsg);
						C_menuN2=-1;
#ifdef CLAN_DEBUG
				DebugMessage("CLANCHIP_LEAVECLAN_MESSAGEEND\277\241\274\255 \273\360\300\324\275\307\306\320");
#endif
						break;
					case 1: //플래그 삽입 성공							
						WebDB.Suspend();														
						C_menuN2=-1;
						//cldata.nKFlag -=1;
						if((cldata.nKFlag == 1)&&(cldata.nKFlag_Count == 1)) //클랜칩 위임
						{
							cldata.nKFlag_Count = 0;
						}
						else if(cldata.nKFlag == 17) //부클랜칩 해임				
						{
							cldata.nKFlag_Count  = 1;
							
							C_menuN = CLANCHIP_SUBCHIP_RELEASEMSG;
							menuInit(C_menuN);					
							break;
						}
						else if(cldata.nKFlag == 5) //부클랜칩 임명
						{
							cldata.nKFlag_Count  = 1;
							C_menuN = CLANCHIP_SUBCHIP_APPOINTMSG;
							menuInit(C_menuN);				
							break;
						}
						else if(cldata.nKFlag == 21) //부클랜칩 해임,임명
						{
							cldata.nKFlag_Count  = 1;
							if(cldata.SubChip[0] == 0)
							{
								bSubChip = FALSE;
								C_menuN = CLANCHIP_SUBCHIP_APPOINTMSG;
							}
							else
							{
								bSubChip = TRUE;
								C_menuN = CLANCHIP_SUBCHIP_RELEASEMSG;
							}
							
							menuInit(C_menuN);					
							break;
						}
						else{}


						if((strcmp(cldata.stats,"0") == 0)&&(strcmp(cldata.PFlag,"0") == 0))
						{
							wsprintf(szDrawMenuMsg,clanMsg6.szMsg,cldata.nLastDay);
							strcpy(szTemp,clanMsg6.szMsg);
							strcpy(clanMsg6.szMsg,szDrawMenuMsg);					
							CheckMsgPos(&clanMsg6,BOX_MAX_WIDTH,0);
							strcpy(clanMsg6.szMsg,szTemp);

							if(cldata.bIsOneDay)
							{
								//strcpy(clanMsg6.szMsg,szDrawMenuMsg);
								wsprintf(szDrawMenuMsg,clanMsg7.szMsg,cldata.nHour,cldata.nMinute);
								strcpy(szTemp,clanMsg6.szMsg);
								strcpy(clanMsg6.szMsg,szDrawMenuMsg);
								//void CheckMsgPos(CLANMSG* clanmsg);
								CheckMsgPos(&clanMsg6,BOX_MAX_WIDTH,0);	
								strcpy(clanMsg6.szMsg,szTemp);
							}
							else
							{
								wsprintf(szDrawMenuMsg,clanMsg6.szMsg,cldata.nLastDay);

								strcpy(szTemp,clanMsg6.szMsg);

								strcpy(clanMsg6.szMsg,szDrawMenuMsg);
								//void CheckMsgPos(CLANMSG* clanmsg);
								CheckMsgPos(&clanMsg6,BOX_MAX_WIDTH,0);		

								strcpy(clanMsg6.szMsg,szTemp);
							}		

							C_menuN = CLAN_NOMAKE;	//stats가 0이면 6명이하이므로 더 모집하라는메시지보냄
							menuInit(C_menuN );
							bPFlag = FALSE;
							C_menuN2=-1;
							break;
					}
					else if((strcmp(cldata.stats,"1") == 0)&&(strcmp(cldata.PFlag,"0") == 0)) //클랜 결성되고 처음 보여주는 메세지
					{
						strcpy(cldata.PFlag,"2");
						strcpy(szTemp,clanMakeFinishClan.szMsg);
						wsprintf(szDrawMenuMsg,clanMakeFinishClan.szMsg,cldata.name);
						strcpy(clanMakeFinishClan.szMsg,szDrawMenuMsg);					
						//void CheckMsgPos(CLANMSG* clanmsg);
						CheckMsgPos(&clanMakeFinishClan,BOX_MAX_WIDTH,0);
						C_menuN = CLANCHIP_MAKE_FIRSTMESSAGE;	//stats가 0이면 6명이하이므로 더 모집하라는메시지보냄
						menuInit(C_menuN );
						strcpy(clanMakeFinishClan.szMsg,szTemp);
						C_menuN2=-1;
						bPFlag = TRUE;
						break;
					}				
					//else if((strcmp(cldata.stats,"0") == 0)&&(strcmp(cldata.PFlag,"2") == 0)) //클랜원이 6명이라로 떨어졌을 경우
					//{	
					//	strcpy(cldata.PFlag,"3");
					//	C_menuN = CLANCHIP_DOWN6_CLANWON;	
					//	menuInit(C_menuN );									
					//	C_menuN2=-1;
					//	break;
					//}				
					else if((strcmp(cldata.stats,"1") == 0)&&(strcmp(cldata.PFlag,"2") == 0)&&(nClanWon_Num <5))
					{					

						wsprintf(szDrawMenuMsg,clanMsg6.szMsg,cldata.nLastDay);
						strcpy(szTemp,clanMsg6.szMsg);
						strcpy(clanMsg6.szMsg,szDrawMenuMsg);					
						CheckMsgPos(&clanMsg6,BOX_MAX_WIDTH,0);
						strcpy(clanMsg6.szMsg,szTemp);

						if(cldata.bIsOneDay)
						{
							//strcpy(clanMsg6.szMsg,szDrawMenuMsg);
							wsprintf(szDrawMenuMsg,clanMsg7.szMsg,cldata.nHour,cldata.nMinute);
							strcpy(szTemp,clanMsg6.szMsg);
							strcpy(clanMsg6.szMsg,szDrawMenuMsg);
							//void CheckMsgPos(CLANMSG* clanmsg);
							CheckMsgPos(&clanMsg6,BOX_MAX_WIDTH,0);	
							strcpy(clanMsg6.szMsg,szTemp);
						}
						else
						{
							wsprintf(szDrawMenuMsg,clanMsg6.szMsg,cldata.nLastDay);

							strcpy(szTemp,clanMsg6.szMsg);

							strcpy(clanMsg6.szMsg,szDrawMenuMsg);
							//void CheckMsgPos(CLANMSG* clanmsg);
							CheckMsgPos(&clanMsg6,BOX_MAX_WIDTH,0);		

							strcpy(clanMsg6.szMsg,szTemp);
						}		

						C_menuN = CLAN_NOMAKE;	
						menuInit(C_menuN );									
						C_menuN2=-1;
						bPFlag = FALSE;
					}
					//else if((strcmp(cldata.stats,"1") == 0)&&(strcmp(cldata.PFlag,"3") == 0))
					//{
					//	strcpy(cldata.PFlag,"4");
					//	C_menuN = ISPFLAG_CHIP;		//도움말로 돌아감.
					//	menuInit(C_menuN);
					//	C_menuN2 = 0;					
					//}				
					//else if((strcmp(cldata.stats,"0") == 0)&&(strcmp(cldata.PFlag,"4") == 0))
					//{
					//	strcpy(cldata.PFlag,"3");
					//	C_menuN = CLANCHIP_DOWN6_CLANWON;	
					//	menuInit(C_menuN );									
					//	C_menuN2=-1;
					//}
	
					else // 1:1 or 1:3
					{
						C_menuN = CLAN_CHIP;	//바로 클랜칲메뉴로 간다.
						menuInit(C_menuN );									
						C_menuN2=-1;
						bPFlag = TRUE;
					}
#ifdef CLAN_DEBUG
			DebugMessage("CLANCHIP_LEAVECLAN_MESSAGE\277\241\274\255 \270\256\305\317\304\332\265\345 1 \274\272\260\370");
#endif
						
						
						C_menuN2 = -1;
#ifdef CLAN_DEBUG
				DebugMessage("CLANCHIP_LEAVECLAN_MESSAGEEND\277\241\274\255 \273\360\300\324\274\272\260\370");
#endif
						break;
					default:

						if(ANSdata.ret_val != -1) {		//전혀엉뚱한 답이 와서 에러가 났다.
							WebDB.Suspend();
							C_menuN = CLAN_ERROR;
							wsprintf(szErrorMsg,"%s (87)",clanszError_Msg.szMsg);
							cClanReport.menuInit(C_menuN,clanszError_Msg.pos.x,clanszError_Msg.pos.y);
							cClanReport.InitMsg(szErrorMsg);
							C_menuN2 = -1;
#ifdef CLAN_DEBUG
				DebugMessage("CLANCHIP_LEAVECLAN_MESSAGEEND\277\241\274\255 \265\360\306\372\306\256\277\241\267\257");
#endif
						}						
						break;
				}
		}
		break;	

	//이임을 하면서 이임플래그 값을 200을 넣음 npc를 만날때 클랜장이 바뀌었다는 메세지를 띄우기 위해서
	case CLANCHIP_LEAVECLAN_PFLAG: 
		switch(C_menuN2){
			case 0:
				ANSdata.ret_val= -1;			

				WebDB.Resume() ;		//웹db를 재개한다.
				
				WebDB.isKPFlag(cldata.szID,cldata.name,cldata.ChaName,cldata.gserver,"200","1");
				
				C_menuN2=1;
#ifdef CLAN_DEBUG
				DebugMessage("CLANCHIP_LEAVECLAN_PFLAG\277\241\274\255 WebDB.isPFlag(\276\306\300\314\265\360 : %s,\305\254\267\243\300\314\270\247 : %s,\304\263\270\257\305\315\300\314\270\247 : %s,\260\324\300\323\274\255\271\366 : %s,\307\303\267\241\261\327 : %s,0)",cldata.szID,cldata.name,cldata.ChaName,cldata.gserver,cldata.PFlag);
#endif
				break;
			case 1:
				switch(ANSdata.ret_val){
					case 0://플래그 삽입 실패
						WebDB.Suspend();
						C_menuN = CLAN_ERROR;
						wsprintf(szErrorMsg,"%s (88)",clanszError_Msg.szMsg);
						cClanReport.menuInit(C_menuN,clanszError_Msg.pos.x,clanszError_Msg.pos.y);
						cClanReport.InitMsg(szErrorMsg);
						C_menuN2=-1;
#ifdef CLAN_DEBUG
				DebugMessage("CLANCHIP_LEAVECLAN_PFLAG\277\241\274\255 \273\360\300\324\275\307\306\320");
#endif
						break;
					case 1: //플래그 삽입 성공							
						WebDB.Suspend();														
						C_menuN2=-1;
						//C_menuN = CLAN_CHIP;						
						C_menuN = CLANCHIP_LEAVECLAN_SUCESS;
						menuInit(C_menuN);
						
						C_menuN2 = -1;
#ifdef CLAN_DEBUG
				DebugMessage("CLANCHIP_LEAVECLAN_PFLAG\277\241\274\255 \273\360\300\324\274\272\260\370");
#endif
						break;
					default:

						if(ANSdata.ret_val != -1) {		//전혀엉뚱한 답이 와서 에러가 났다.
							WebDB.Suspend();
							C_menuN = CLAN_ERROR;
							wsprintf(szErrorMsg,"%s (89)",clanszError_Msg.szMsg);
							cClanReport.menuInit(C_menuN,clanszError_Msg.pos.x,clanszError_Msg.pos.y);
							cClanReport.InitMsg(szErrorMsg);
							C_menuN2 = -1;
#ifdef CLAN_DEBUG
				DebugMessage("CLANCHIP_LEAVECLAN_PFLAG\277\241\274\255 \265\360\306\372\306\256\277\241\267\257");
#endif
						}						
						break;
				}
		}
		break;	
	
		//이임을 하기 위한 asp를 호출
	case CLANCHIP_LEAVECLAN_DO:
		switch(C_menuN2) {
		case 1:
			ANSdata.ret_val= -1;
			WebDB.Resume() ;		//웹db를 재개한다.
			WebDB.LeaveClan(cldata.name,cldata.clanWon[nSavePos],cldata.gserver);
			C_menuN2=2;
			
			break;
		case 2:
			switch(ANSdata.ret_val) 
			{
			case 1: //변경 성공
				//C_menuN = CLANCHIP_LEAVECLAN_SUCESS;
				WebDB.Suspend(); 
				C_menuN2=0;
#ifdef USE_PROSTONTALE		//=========== 포리스턴소스인경우
				//int sinMinusMoney(int Money); //이임 비용처
				int sinMinusMoney(int Money,int Kind=0);
				sinMinusMoney(300000);
#endif
				C_menuN = CLANCHIP_LEAVECLAN_PFLAG;
				menuInit(C_menuN);
				break;
			case 2: //클랜원이 아님
				WebDB.Suspend(); 
				C_menuN2=0;
				
				C_menuN = CLAN_ERROR;
				wsprintf(szErrorMsg,"%s (83)", clanszError_Msg.szMsg);
				cClanReport.InitMsg(szErrorMsg);
				cClanReport.menuInit(C_menuN,clanszError_Msg.pos.x,clanszError_Msg.pos.y);
				CLAN_ERROR_ret=CLAN_AllEnd;		//에러메뉴보여준후 다시 돌아갈곳.
				break;
			case 3: //존재하지 않는 클랜
				WebDB.Suspend(); 
				C_menuN2=0;
				
				C_menuN = CLAN_ERROR;
				wsprintf(szErrorMsg,"%s (82)", clanszError_Msg.szMsg);
				cClanReport.InitMsg(szErrorMsg);
				cClanReport.menuInit(C_menuN,clanszError_Msg.pos.x,clanszError_Msg.pos.y);
				CLAN_ERROR_ret=CLAN_AllEnd;		//에러메뉴보여준후 다시 돌아갈곳.
				break;
			case 4: //레벨 50미만
				WebDB.Suspend(); 
				C_menuN2=0;
				C_menuN = CLANCHIP_LEAVECLAN_NOLEVEL;
				menuInit(C_menuN);
				//C_menuN = CLAN_ERROR;
				//wsprintf(szErrorMsg,"%s (81)", clanszError_Msg.szMsg);
				//cClanReport.InitMsg(szErrorMsg);
				//cClanReport.menuInit(C_menuN,clanszError_Msg.pos.x,clanszError_Msg.pos.y);
				//CLAN_ERROR_ret=CLAN_AllEnd;		//에러메뉴보여준후 다시 돌아갈곳.
				break;
			case 5: //권한 자격만 성공
				WebDB.Suspend(); 
				C_menuN2=0;
				C_menuN = CLAN_ERROR;
				wsprintf(szErrorMsg,"%s (80)", clanszError_Msg.szMsg);
				cClanReport.InitMsg(szErrorMsg);
				cClanReport.menuInit(C_menuN,clanszError_Msg.pos.x,clanszError_Msg.pos.y);
				CLAN_ERROR_ret=CLAN_AllEnd;		//에러메뉴보여준후 다시 돌아갈곳.
				break;
			case 6: //지정캐릭터의 포용최대인원수 보다 현재 인원수가 큼
				WebDB.Suspend(); 
				C_menuN2=0;
				C_menuN = CLANCHIP_LEAVECLAN_NOLEVEL;
				menuInit(C_menuN);
				
				/*C_menuN = CLAN_ERROR;
				wsprintf(szErrorMsg,"%s (79)", clanszError_Msg.szMsg);
				cClanReport.InitMsg(szErrorMsg);
				cClanReport.menuInit(C_menuN,clanszError_Msg.pos.x,clanszError_Msg.pos.y);
				CLAN_ERROR_ret=CLAN_AllEnd;		//에러메뉴보여준후 다시 돌아갈곳.
				*/
				break;
			case 100:								//클랜을 만든것이 성공(클랜마크설명메뉴로..)				
				WebDB.Suspend(); 
				C_menuN2=0;
				
				C_menuN = CLAN_ERROR;
				wsprintf(szErrorMsg,"%s (76)", clanszError_Msg.szMsg);
				cClanReport.InitMsg(szErrorMsg);
				cClanReport.menuInit(C_menuN,clanszError_Msg.pos.x,clanszError_Msg.pos.y);
				CLAN_ERROR_ret=CLAN_AllEnd;		//에러메뉴보여준후 다시 돌아갈곳.
				break;
			case ISAO_ERR_TCP_CONNECT:
				WebDB.Suspend();				
				C_menuN = CLAN_ERROR;
				wsprintf(szErrorMsg,"%s (77)", clanszError_Msg.szMsg);
				cClanReport.InitMsg(szErrorMsg);
				cClanReport.menuInit(C_menuN,clanszError_Msg.pos.x,clanszError_Msg.pos.y);
				
				break;
			default:
				if(ANSdata.ret_val != -1) {		//전혀엉뚱한 답이 와서 에러가 났다.
					WebDB.Suspend();
					C_menuN2=0;					
					C_menuN = CLAN_ERROR;
					wsprintf(szErrorMsg,"%s (78)", clanszError_Msg.szMsg);
					cClanReport.InitMsg(szErrorMsg);
					cClanReport.menuInit(C_menuN,clanszError_Msg.pos.x,clanszError_Msg.pos.y);
					CLAN_ERROR_ret=CLAN_AllEnd;		//에러메뉴보여준후 다시 돌아갈곳.
				}
				break;
			}
		}
		break;
	case CLANCHIP_LEAVECLAN_RECONFIRM:
		
		i = chkeckMenuN();				//================================================
		switch(i) {
		case 0:		//yes
			menuMouseOver[0] = 1;
			break;
		case 1:		//no
			menuMouseOver[1] = 1;
			break;
		default:
			memset(menuMouseOver,-1,sizeof(int)*30);
		}
		//ygy : 꼭 살릴것
		if(MOUSE0())
		{	
			MOUSE0()=0;
			switch(i) 
			{
			case 0:
				//ktj : 이부분 다시 열것:

				C_menuN2 =1;
				C_menuN = CLANCHIP_LEAVECLAN_DO;			//Y : 클랜결성하겟다.  대금지불의사로간다.
				menuInit(C_menuN );
				
				break;

			case 1: //클랜 만들기를 취소
				//ygy : 꼭 살릴것
				
				C_menuN = CLAN_CHIP;			//취소				
				menuInit(C_menuN);				
				break;

			default:
				break;
			}
		}
		break;
	case CLANCHIP_LEAVECLAN_CONFIRM:
		i = chkeckMenuN();				//================================================
		switch(i) {
		case 0:		//yes
			menuMouseOver[0] = 1;
			break;
		case 1:		//no
			menuMouseOver[1] = 1;
			break;
		default:
			memset(menuMouseOver,-1,sizeof(int)*30);
		}
		//ygy : 꼭 살릴것
		if(MOUSE0())
		{	
			MOUSE0()=0;
			switch(i) 
			{
			case 0: //이임
				//ktj : 이부분 다시 열것:
				if(cldata.money < CLANCHIP_LEAVECLAN_MONEY)
				{
					C_menuN = CLANCHIP_LEAVECLAN_NOMONEY;
				}
				else
				{
					C_menuN = CLANCHIP_LEAVECLAN;			
				}
				
				menuInit(C_menuN );
				
				break;

			case 1: //이임 취소			
				
				C_menuN = CLAN_CHIP;			//취소				
				menuInit(C_menuN);				
				break;

			default:
				break;
			}
		}
		break;
	case CLANCHIP_LEAVECLAN_NOMONEY:
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
				C_menuN = CLAN_CHIP;
				menuInit(C_menuN );
				//C_menuN = CLAN_AllEnd;
				//C_menuN = CLAN_ERROR_ret;		//나가기 : 끝내는걸로간다.
				

				C_menuN2 =-1;
				CLAN_ERROR_ret=-1;
				break;	
			default:
				i = i;
				break;

			}
		}
		break;
	case CLANCHIP_LEAVECLAN_SUCESS:		
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
	case CLANCHIP_LEAVECLAN://이임
		i = chkeckMenuN();
		switch(i) {
		case 0:
			menuMouseOver[0] = 1;
			break;
		case 1:
			menuMouseOver[1] = 1;
			break;		
		default:
			memset(menuMouseOver,-1,sizeof(int)*30);
			break;
		}

		//ktj : 삭제
			//ygy : 윌관련한 변수 TJwheel살려야함
			if(TJwheel != 0) {			//ktj : wheel>0 휠 앞으로 굴리기  wheel<0 휠 뒤로 굴리기
				scrIconPos.top = tscr.ret_WHEELpoint(TJwheel , scrIconPos.top);
				nScl_Bar = tscr.topScrollLineNum(scrIconPos.top);
				TJwheel = 0;
			}


		//ygy : 살려야함
		if(MOUSE0())			
		{
			switch(i) 
			{
			case 12:
				nScl_Bar = tscr.topScrollLineNum(pCursorPos.y);
				//ktj : 스크롤 아이콘 위치재설정
				scrIconPos.top =  pCursorPos.y-8;//BackStartPos.y + SCROOBAR_TOP;
				break;

			case 4:
			case 5:
			case 6:
			case 7:
			case 8:		//추방할사람을 설정한다.
			case 9:
			case 10:
			case 11:				
				//if(nReleaseClanWon >= 5){	
					if(banisF[i-4+nScl_Bar]>0){
						banisF[i-4+nScl_Bar] = !banisF[i-4+nScl_Bar];
				//		nReleaseClanWon-=1;
						nSavePos = i-4+nScl_Bar;
					}
					else
					{
						ZeroMemory(banisF,sizeof(banisF));
						banisF[i-4+nScl_Bar] = !banisF[i-4+nScl_Bar];
						nSavePos = i-4+nScl_Bar;
					}
				//}
				//else{
					
				//	banisF[i-4+nScl_Bar] = !banisF[i-4+nScl_Bar];
				//	if(banisF[i-4+nScl_Bar]){
				//		nReleaseClanWon+=1;
				//	}
				//	else{
				//		nReleaseClanWon-=1;
				//	}
				//}

				//ygy :살려야함
				MOUSE0()=0;
				break;
			case 15:
			case 16:
			case 17:
			case 18:
			case 19:
			case 20:
			case 21:
			case 22:
				//if(nReleaseClanWon >= 5){	
					if(banisF[i-15+nScl_Bar]>0){
						banisF[i-15+nScl_Bar] = !banisF[i-15+nScl_Bar];
						nSavePos = i-14+nScl_Bar;
				//		nReleaseClanWon-=1;
					}
					else
					{
						ZeroMemory(banisF,sizeof(banisF));
						banisF[i-15+nScl_Bar] = !banisF[i-15+nScl_Bar];
						nSavePos = i-15+nScl_Bar;
					}
			//	}
			//	else{
					
			//		banisF[i-14+nScl_Bar] = !banisF[i-14+nScl_Bar];
			//		if(banisF[i-14+nScl_Bar]){
			//			nReleaseClanWon+=1;
			//		}
			//		else{
			//			nReleaseClanWon-=1;
			//		}
			//	}

				//ygy : 살려야함
				MOUSE0()=0;
				break;
		
			case 0: //이임 ======================== 프로그램 짤것....
				
				C_menuN2 = 0;		//ktj : php로 추방할 데이타를  집어넣어 실행하는 루틴으로 간다.
				if(nSavePos < 0)
				{
					C_menuN = CLAN_CHIP;
				}
				else
				{
					C_menuN = CLANCHIP_LEAVECLAN_RECONFIRM;
				}
				
				menuInit(C_menuN);
				//ygy : 살려야함
				MOUSE0()=0;
				break;
			case 1:				
				C_menuN = CLAN_CHIP;				//닫기 : 그냥 돌아감.
				menuInit(C_menuN);
				//ygy : 살려야함
				MOUSE0()=0;
				break;
			default:
				//ygy : 살려야함
				MOUSE0()=0;
				break;
			}
		}
		break;
	//이임==========================================end

	//========추방

	case CLAN_REMOVE:
		switch(C_menuN2) {
		case 0:
			ANSdata.ret_val= -1;				
						
			WebDB.Resume() ;		//웹db를 재개한다.
			WebDB.Banishiment(cldata.szID,cldata.gserver,cldata.ClanZang_Name,cldata.name,szRemoveClanWon);
			C_menuN2=1;
#ifdef CLAN_DEBUG
			DebugMessage("CLAN_REMOVE\277\241\274\255 WebDB.Banishiment(\276\306\300\314\265\360 : %s,\260\324\300\323\274\255\271\366 : %s,\305\254\267\243\300\345 : %s,\305\254\267\243\300\314\270\247 : %s,\305\254\267\243\277\370%s)",cldata.szID,cldata.gserver,cldata.ClanZang_Name,cldata.name,szRemoveClanWon);
#endif
			break;
		case 1:		

			if(ANSdata.ret_val != -1) {
				ANSdata.ret_val = ANSdata.ret_val;
			}
			
			switch(ANSdata.ret_val) 
			{
			case -1:
				C_menuN = CLAN_REMOVE_LOADING;
				menuInit(C_menuN);
				break;
					//ygy
			case 0: //트랜잭션 오류
				WebDB.Suspend(); //웹db 중지
				//ygy : 에러 메세지 살려야함
				C_menuN = CLAN_ERROR;
				wsprintf(szErrorMsg,"%s (12)",clanszError_Msg.szMsg);
				cClanReport.InitMsg(szErrorMsg);
				cClanReport.menuInit(C_menuN,clanszError_Msg.pos.x,clanszError_Msg.pos.y);
				CLAN_ERROR_ret=255;		//에러메뉴보여준후그냥끝낸다.
				C_menuN2=-1;
#ifdef CLAN_DEBUG
			DebugMessage("CLAN_REMOVE\277\241\274\255 \306\256\267\243\300\350\274\307\277\300\267\371");
#endif
				break;
			case 1: //클랜원 탈퇴 성공
			case 3: //탈퇴는 성공했으나 클랜회원수가 6명이하로 떨어졌을 경우(LimitDate 설정)

				WebDB.Suspend(); //웹db 중지
				void SetClanWon();
				SetClanWon();
				C_menuN2=0;
				C_menuN=CLAN_REMOVE_RESULT;
				menuInit(C_menuN);				
#ifdef CLAN_DEBUG
			DebugMessage("CLAN_REMOVE\277\241\274\255 \303\337\271\346\274\272\260\370(%d)",ANSdata.ret_val);
#endif
				
				break;
			case 2: //클랜이 존재하지 않거나 클랜 장이 아닌경우 또는 클랜원이 아닌경우
				WebDB.Suspend(); //웹db 중지
				//wsprintf(szErrorMsg, "클랜이 존재하지 않거나 클랜 장이 아닌경우 또는 클랜원이 아닌경우.");
				C_menuN = CLAN_ERROR;
				wsprintf(szErrorMsg,"%s (13)",clanszError_Msg.szMsg);
				cClanReport.InitMsg(szErrorMsg);
				cClanReport.menuInit(C_menuN,clanszError_Msg.pos.x,clanszError_Msg.pos.y);
				//menuInit(C_menuN);
				CLAN_ERROR_ret=255;		//에러메뉴보여준후그냥끝낸다.
				
				C_menuN2=-1;
#ifdef CLAN_DEBUG
			DebugMessage("CLAN_REMOVE\277\241\274\255 \305\254\267\243\300\314 \301\270\300\347\307\317\301\366 \276\312\260\305\263\252 \305\254\267\243 \300\345\300\314 \276\306\264\321\260\346\277\354 \266\307\264\302 \305\254\267\243\277\370\300\314 \276\306\264\321\260\346\277\354(%d)",ANSdata.ret_val);
#endif
				break;				
			case 100: //엉둥한 값
				WebDB.Suspend(); //웹db 중지
				//ygy : 에러 메세지 살려야함
				//wsprintf(szErrorMsg, sz100Error);
				C_menuN = CLAN_ERROR;
				wsprintf(szErrorMsg,"%s (14)",clanszError_Msg.szMsg);
				cClanReport.InitMsg(szErrorMsg);
				cClanReport.menuInit(C_menuN,clanszError_Msg.pos.x,clanszError_Msg.pos.y);
				//menuInit(C_menuN);
				CLAN_ERROR_ret=255;		//에러메뉴보여준후그냥끝낸다.
				C_menuN2=-1;
#ifdef CLAN_DEBUG
			DebugMessage("CLAN_REMOVE\277\241\274\255 \270\256\305\317\304\332\265\345 100");
#endif
				break;
			case ISAO_ERR_TCP_CONNECT:
				WebDB.Suspend();
				//ygy : 에러 메세지 살려야함
				wsprintf(szErrorMsg,"%s (15)",clanszError_Msg.szMsg);
				C_menuN = CLAN_ERROR;
				cClanReport.InitMsg(szErrorMsg);
				cClanReport.menuInit(C_menuN,clanszError_Msg.pos.x,clanszError_Msg.pos.y);
#ifdef CLAN_DEBUG
			DebugMessage("CLAN_REMOVE\277\241\274\255 \274\255\271\366\277\241\301\242\274\323\307\322\274\366 \276\370\300\275");
#endif
				break;
				//end ygy
			
			default:
				if(ANSdata.ret_val != -1) {		//전혀엉뚱한 답이 와서 에러가 났다.
					WebDB.Suspend();

					//ygy : 살려야함
					C_menuN = CLAN_ERROR;
					wsprintf(szErrorMsg,"%s (16)",clanszError_Msg.szMsg);
					cClanReport.InitMsg(szErrorMsg);
					cClanReport.menuInit(C_menuN,clanszError_Msg.pos.x,clanszError_Msg.pos.y);
					CLAN_ERROR_ret=255;		//에러메뉴보여준후그냥끝낸다.
					C_menuN2=-1;
#ifdef CLAN_DEBUG
			DebugMessage("CLAN_REMOVE\277\241\274\255 \265\360\306\372\306\256\277\241\267\257");
#endif
				}
				break;
			}
			break;
		}


		i = chkeckMenuN();
		switch(i) {
		case 0:
			menuMouseOver[0] = 1;
			break;
		case 1:
			menuMouseOver[1] = 1;
			break;		
		default:
			memset(menuMouseOver,-1,sizeof(int)*30);
			break;
		}

		//ktj : 삭제
			//ygy : 윌관련한 변수 TJwheel살려야함
			if(TJwheel != 0) {			//ktj : wheel>0 휠 앞으로 굴리기  wheel<0 휠 뒤로 굴리기
				scrIconPos.top = tscr.ret_WHEELpoint(TJwheel , scrIconPos.top);
				nScl_Bar = tscr.topScrollLineNum(scrIconPos.top);
				TJwheel = 0;
			}


		//ygy : 살려야함
		if(MOUSE0())			
		{
			switch(i) 
			{
			case 12:
				nScl_Bar = tscr.topScrollLineNum(pCursorPos.y);
				//ktj : 스크롤 아이콘 위치재설정
				scrIconPos.top =  pCursorPos.y-8;//BackStartPos.y + SCROOBAR_TOP;
				break;

			case 4:
			case 5:
			case 6:
			case 7:
			case 8:		//추방할사람을 설정한다.
			case 9:
			case 10:
			case 11:				
				if(nReleaseClanWon >= 5){	
					if(banisF[i-4+nScl_Bar]>0){
						banisF[i-4+nScl_Bar] = !banisF[i-4+nScl_Bar];
						nReleaseClanWon-=1;
					}
				}
				else{
					
					banisF[i-4+nScl_Bar] = !banisF[i-4+nScl_Bar];
					if(banisF[i-4+nScl_Bar]){
						nReleaseClanWon+=1;
					}
					else{
						nReleaseClanWon-=1;
					}
				}

				//ygy :살려야함
				MOUSE0()=0;
				break;
			case 14:
			case 15:
			case 16:
			case 17:
			case 18:
			case 19:
			case 20:
			case 21:
				if(nReleaseClanWon >= 5){	
					if(banisF[i-14+nScl_Bar]>0){
						banisF[i-4+nScl_Bar] = !banisF[i-14+nScl_Bar];
						nReleaseClanWon-=1;
					}
				}
				else{
					
					banisF[i-14+nScl_Bar] = !banisF[i-14+nScl_Bar];
					if(banisF[i-14+nScl_Bar]){
						nReleaseClanWon+=1;
					}
					else{
						nReleaseClanWon-=1;
					}
				}

				//ygy : 살려야함
				MOUSE0()=0;
				break;
		
			case 0: //추방 ======================== 프로그램 짤것....
				ZeroMemory(szRemoveClanWon,sizeof(szRemoveClanWon));
				for(i=0; i<nClanWon_Num; i++) {
					if(banisF[i] ) {					
						wsprintf(banisBuf1, "clwon%d=%s", banisCnt, cldata.clanWon[i]);

						if(banisCnt==1) {
							wsprintf(banisBuf2, "%s", banisBuf1);
						}
						else {
							wsprintf(banisBuf2, "%s&%s", szRemoveClanWon, banisBuf1);
						}
						memcpy(szRemoveClanWon,banisBuf2, strlen(banisBuf2)+1);
						banisCnt++;
					}
				}
				if(szRemoveClanWon[0] == 0)
				{
					C_menuN = CLAN_CHIP;				//닫기 : 그냥 돌아감.
					menuInit(C_menuN);
					//ygy : 살려야함
					MOUSE0()=0;
					break;
				}
				//ygy : 살려야함
				ReMoveClanWon(szRemoveClanWon);
				if((nClanWon_Num - nReleaseClanWon)<5)
				{
					bRemoveSecondCheck = FALSE;
					C_menuN = CLAN_REMOVE_SHORTCLANWON;
					menuInit(C_menuN);
					//ygy : 살려야함
					MOUSE0()=0;
					break;
				}
				else
				{
					//6명 이상이여도 중간 메세지를 보여주기 위해서
					bRemoveSecondCheck = TRUE;
					C_menuN = CLAN_REMOVE_SHORTCLANWON;
					menuInit(C_menuN);
					//ygy : 살려야함
					MOUSE0()=0;					
					break;
				}
				C_menuN2 = 0;		//ktj : php로 추방할 데이타를  집어넣어 실행하는 루틴으로 간다.
				//ygy : 살려야함
				MOUSE0()=0;
				break;
			case 1:				
				C_menuN = CLAN_CHIP;				//닫기 : 그냥 돌아감.
				menuInit(C_menuN);
				//ygy : 살려야함
				MOUSE0()=0;
				break;
			default:
				//ygy : 살려야함
				MOUSE0()=0;
				break;
			}
		}
		break;
	case CLAN_REMOVE_SHORTCLANWON:
		switch(C_menuN2) {
		case 0:
			ANSdata.ret_val= -1;					
						
			WebDB.Resume() ;		//웹db를 재개한다.			
			WebDB.Banishiment(cldata.szID,cldata.gserver,cldata.ClanZang_Name,cldata.name,szRemoveClanWon);	
			
			C_menuN2=1;
#ifdef CLAN_DEBUG
			DebugMessage("CLAN_REMOVE_SHORTCLANWON\277\241\274\255 WebDB.Banishiment(\276\306\300\314\265\360 : %s,\260\324\300\323\274\255\271\366 : %s,\305\254\267\243\300\345 : %s,\305\254\267\243\300\314\270\247 : %s,\305\254\267\243\277\370%s)",cldata.szID,cldata.gserver,cldata.ClanZang_Name,cldata.name,szRemoveClanWon);
#endif
			break;
		case 1:
			
			switch(ANSdata.ret_val) 
			{
			case -1:
				C_menuN = CLAN_REMOVE_SHORTCLANWON_LOADING;
				menuInit(C_menuN);
				break;
				//ygy
			case 0: //트랜잭션 오류
				WebDB.Suspend(); //웹db 중지
				//ygy : 에러 메세지 살려야함
				C_menuN = CLAN_ERROR;
				wsprintf(szErrorMsg,"%s (17)",clanszError_Msg.szMsg);
				cClanReport.InitMsg(szErrorMsg);				
				cClanReport.menuInit(C_menuN,clanszError_Msg.pos.x,clanszError_Msg.pos.y);
				//menuInit(C_menuN);
				CLAN_ERROR_ret=255;		//에러메뉴보여준후그냥끝낸다.
				C_menuN2=-1;
#ifdef CLAN_DEBUG
			DebugMessage("CLAN_REMOVE_SHORTCLANWON\277\241\274\255 \306\256\267\243\300\350\274\307\277\300\267\371");
#endif
				break;
			case 1: //클랜원 탈퇴 성공					
			case 3: //탈퇴는 성공했으나 클랜회원수가 6명이하로 떨어졌을 경우(LimitDate 설정)
				//fd2(" 답받음. ret_val == 100 ");
				WebDB.Suspend(); //웹db 중지
				C_menuN2=0;
				
				void SetClanWon();
				SetClanWon();

				//ygy : 살려야함
				//wsprintf(szErrorMsg, szRemoveShowrtClanWonResultMsg);
				if(nReleaseClanWon > 0) cldata.nCurrentClanWon_Num -=nReleaseClanWon; //추방하고 나서 현재 클랜원수 세팅
				
				C_menuN = CLAN_REMOVE_SHORTCLANWON_RESULT;				
				menuInit(C_menuN);
				//
				
				
				
				break;
			case 2: //클랜이 존재하지 않거나 클랜 장이 아닌경우 또는 클랜원이 아닌경우
				WebDB.Suspend(); //웹db 중지
				//wsprintf(szErrorMsg, "클랜이 존재하지 않거나 클랜 장이 아닌경우 또는 클랜원이 아닌경우.");
				C_menuN = CLAN_ERROR;
				wsprintf(szErrorMsg,"%s (18)",clanszError_Msg.szMsg);
				cClanReport.InitMsg(szErrorMsg);
				cClanReport.menuInit(C_menuN,clanszError_Msg.pos.x,clanszError_Msg.pos.y);
				
				menuInit(C_menuN);
				CLAN_ERROR_ret=255;		//에러메뉴보여준후그냥끝낸다.
				
				C_menuN2=-1;
#ifdef CLAN_DEBUG
			DebugMessage("CLAN_REMOVE_SHORTCLANWON\277\241\274\255 \305\254\267\243\300\314 \301\270\300\347\307\317\301\366 \276\312\260\305\263\252 \305\254\267\243 \300\345\300\314 \276\306\264\321\260\346\277\354 \266\307\264\302 \305\254\267\243\277\370\300\314 \276\306\264\321\260\346\277\354(2)");
#endif
				break;
				
			case 100: //엉둥한 값
				WebDB.Suspend(); //웹db 중지
				//ygy : 살려야함
				C_menuN = CLAN_ERROR;
				wsprintf(szErrorMsg,"%s (19)",clanszError_Msg.szMsg);
				cClanReport.InitMsg(szErrorMsg);
				cClanReport.menuInit(C_menuN,clanszError_Msg.pos.x,clanszError_Msg.pos.y);
				CLAN_ERROR_ret=255;		//에러메뉴보여준후그냥끝낸다.
				C_menuN2=-1;
#ifdef CLAN_DEBUG
			DebugMessage("CLAN_REMOVE_SHORTCLANWON\277\241\274\255 \270\256\305\317\304\332\265\345 100");
#endif

				break;
			case ISAO_ERR_TCP_CONNECT:
				WebDB.Suspend();
				//ygy : 에러 메세지 살려야함
				C_menuN = CLAN_ERROR;
				wsprintf(szErrorMsg,"%s (20)",clanszError_Msg.szMsg);
				cClanReport.InitMsg(szErrorMsg);
				cClanReport.menuInit(C_menuN,clanszError_Msg.pos.x,clanszError_Msg.pos.y);
#ifdef CLAN_DEBUG
			DebugMessage("CLAN_REMOVE_SHORTCLANWON\277\241\274\255 \274\255\271\366\277\241 \301\242\274\323\307\322\274\366 \276\370\300\275");
#endif
				break;
				//end ygy
			
			default:
				if(ANSdata.ret_val != -1) {		//전혀엉뚱한 답이 와서 에러가 났다.
					WebDB.Suspend();
					//ygy : 살려야함
					C_menuN = CLAN_ERROR;
					wsprintf(szErrorMsg,"%s (21)",clanszError_Msg.szMsg);
					cClanReport.InitMsg(szErrorMsg);
					cClanReport.menuInit(C_menuN,clanszError_Msg.pos.x,clanszError_Msg.pos.y);
					CLAN_ERROR_ret=255;		//에러메뉴보여준후그냥끝낸다.
					C_menuN2=-1;
#ifdef CLAN_DEBUG
			DebugMessage("CLAN_REMOVE_SHORTCLANWON\277\241\274\255 \265\360\306\372\306\256\277\241\267\257");
#endif
				}
				break;
			}
			break;
		}


		i = chkeckMenuN();
		switch(i) {
		case 0:
			menuMouseOver[0] = 1;
			break;
		case 1:
			menuMouseOver[1] = 1;
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
				C_menuN2=0;
				break;
			case 1:				
				ZeroMemory(banisBuf1, sizeof(banisBuf1));
				ZeroMemory(banisBuf2, sizeof(banisBuf2));
				
				memset(banisF,0,sizeof(int)*100);
				ZeroMemory(szRemoveClanWon,sizeof(char)*500);	
				ZeroMemory(szRemoveClanWonResult,sizeof(szRemoveClanWonResult));  //클랜원을 저장할 변수 초기화
				banisCnt = 1;
				C_menuN = CLAN_REMOVE;
				menuInit(C_menuN);
				break;
			}
		}
		break;
	case CLAN_REMOVE_SHORTCLANWON_RESULT:
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
				bClan6Down = TRUE;
				C_menuN2=0;
				C_menuN=CLAN_CHIP_ReadDATA;
				//C_menuN = CLAN_CHIP;				//닫기 : 그냥 돌아감.
				menuInit(C_menuN);
				
				break;				
			default:				
				break;
			}
		}
		break;		
	case CLAN_REMOVE_RESULT:
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
				C_menuN2=0;
				C_menuN=CLAN_CHIP_ReadDATA;
				//C_menuN = CLAN_CHIP;				//닫기 : 그냥 돌아감.
				menuInit(C_menuN);
				break;				
			default:				
				break;
			}
		}
		break;
	
	case CLAN_RELEASE: //해체
		i = chkeckMenuN();
		switch(i) {
		case 0:
			menuMouseOver[0] =1;
			break;
		case 1:
			menuMouseOver[1] =1;
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
			case 0:			//해체하기 실행(위쪽에서 실행됨)				
				if(strcmp(cldata.stats,"0") == 0){
					//ygy : 살려야함
					wsprintf(szReconfReleaseMsg,clanNoRecordReleaseMsg.szMsg);
				}
				else{
					//ygy : 살려야함
					wsprintf(szReconfReleaseMsg,clanszReConfReleaseMsg.szMsg);
				
				}
				C_menuN = CLAN_RECONF_RELEASE;
				menuInit(C_menuN);
				break;	
			case 1:			//돌아간다.
				C_menuN = CLAN_CHIP;
				menuInit(C_menuN);
				break;
			default:
				break;
			}
		}
		break;
	case CLAN_RECONF_RELEASE:
		switch(C_menuN2) {
		case 0:
			ANSdata.ret_val= -1;
			
			WebDB.Resume() ;		//웹db를 재개한다.			
			WebDB.BreakUP_Clan(cldata.szID,cldata.gserver,cldata.ClanZang_Name,cldata.name);		//클랜헤체
#ifdef CLAN_DEBUG
			DebugMessage("CLAN_RECONF_RELEASE\277\241\274\255 WebDB.BreakUP_Clan(\276\306\300\314\265\360 : %s,\260\324\300\323\274\255\271\366 \300\314\270\247 : %s,\304\263\270\257\305\315\300\314\270\247 : %s,\305\254\267\243\300\314\270\247 : %s)",cldata.szID,cldata.gserver,cldata.ClanZang_Name,cldata.name);
#endif
			C_menuN2=1;
			break;
		case 1:			
			switch(ANSdata.ret_val) 
			{
			case -1:
				C_menuN = CLAN_RELEASE_LOADING;
				menuInit(C_menuN);
				break;
			case 0: //트랜잭션 오류
				WebDB.Suspend();
				//ygy : 에러 메세지 살려야함
				C_menuN = CLAN_ERROR;
				wsprintf(szErrorMsg,"%s (22)",clanszError_Msg.szMsg);
				cClanReport.InitMsg(szErrorMsg);
				cClanReport.menuInit(C_menuN,clanszError_Msg.pos.x,clanszError_Msg.pos.y);
				CLAN_ERROR_ret=255;	
#ifdef CLAN_DEBUG
			DebugMessage("CLAN_RECONF_RELEASE\277\241\274\255 \306\256\267\243\300\350\274\307 \277\300\267\371");
#endif
				break;			
			case 1: //삭제 성공				
				WebDB.Suspend(); //웹db 중지				
				cldata.myPosition = CLANNO_REGESTER;									
				C_menuN = CLAN_FINISH_RELEASE;		//끝낸다.
				menuInit(C_menuN );

				C_menuN2=0;

				void SetClanWon();
				SetClanWon();

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



#ifdef VOICE_CHAT
			   if(bCheckClan)
			   {
				   g_CtrlProcess.SetSelectCha(cldata.szID,cldata.ChaName,
					   NULL,0, cldata.myPosition);
				  // bCheckClan = FALSE;

			   }
#endif
				//=============================================================

#ifdef CLAN_DEBUG
			DebugMessage("CLAN_RECONF_RELEASE\277\241\274\255 \305\254\267\243\307\330\303\274 \274\272\260\370");
#endif				

				
				break; 
			case 2: //현재 캐릭명이 현재 클랜의 장이 아니거나 해당클랜이 없음
				WebDB.Suspend();
				//ygy : 살려야함
				//wsprintf(szErrorMsg, szReleseError);
				C_menuN = CLAN_ERROR;
				wsprintf(szErrorMsg,"%s (23)",clanszError_Msg.szMsg);
				cClanReport.InitMsg(szErrorMsg);
				cClanReport.menuInit(C_menuN,clanszError_Msg.pos.x,clanszError_Msg.pos.y);
				CLAN_ERROR_ret=255;	
#ifdef CLAN_DEBUG
			DebugMessage("CLAN_RECONF_RELEASE\277\241\274\255 \307\366\300\347 \304\263\270\257\270\355\300\314 \307\366\300\347 \305\254\267\243\300\307 \300\345\300\314 \276\306\264\317\260\305\263\252 \307\330\264\347\305\254\267\243\300\314 \276\370\300\275(2)");
#endif
				break;
			case 100: //알수 없는 오류
				WebDB.Suspend();
				//ygy : 살려야함
				C_menuN = CLAN_ERROR;
				wsprintf(szErrorMsg,"%s (24)",clanszError_Msg.szMsg);
				cClanReport.InitMsg(szErrorMsg);
				cClanReport.menuInit(C_menuN,clanszError_Msg.pos.x,clanszError_Msg.pos.y);
				CLAN_ERROR_ret=255;	
#ifdef CLAN_DEBUG
			DebugMessage("CLAN_RECONF_RELEASE\277\241\274\255 \270\256\305\317\304\332\265\345 100");
#endif
				break;
			case ISAO_ERR_TCP_CONNECT:
				WebDB.Suspend();
				//ygy : 살려야함
				C_menuN = CLAN_ERROR;
				wsprintf(szErrorMsg,"%s (25)",clanszError_Msg.szMsg);
				cClanReport.InitMsg(szErrorMsg);
				cClanReport.menuInit(C_menuN,clanszError_Msg.pos.x,clanszError_Msg.pos.y);
#ifdef CLAN_DEBUG
			DebugMessage("CLAN_RECONF_RELEASE\277\241\274\255 \274\255\271\366\277\241 \301\242\274\323\307\322\274\366 \276\370\300\275");
#endif
				break;
						
			default:
				if(ANSdata.ret_val != -1) {		//전혀엉뚱한 답이 와서 에러가 났다.
					WebDB.Suspend();
					//ygy : 살려야함
					C_menuN = CLAN_ERROR;
					wsprintf(szErrorMsg,"%s (26)",clanszError_Msg.szMsg);
					cClanReport.InitMsg(szErrorMsg);
					cClanReport.menuInit(C_menuN,clanszError_Msg.pos.x,clanszError_Msg.pos.y);
					CLAN_ERROR_ret=255;		//에러메뉴보여준후그냥끝낸다.
					C_menuN2=-1;
#ifdef CLAN_DEBUG
			DebugMessage("CLAN_RECONF_RELEASE\277\241\274\255 \265\360\306\372\306\256\277\241\267\257");
#endif
				}
				break;
			}
			break;
		}

		i = chkeckMenuN();
		switch(i) {
		case 0:
			menuMouseOver[0] =1;
			break;
		case 1:
			menuMouseOver[1] =1;
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
			case 0:			//해체하기 실행(위쪽에서 실행됨)
				C_menuN2 = 0;					
				bRelease = TRUE;
				break;	
			case 1:			//돌아간다.
				C_menuN = CLAN_CHIP;
				menuInit(C_menuN);
				break;
			default:
				break;
			}
		}
		break;
	case CLAN_FINISH_RELEASE:  //도움말
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
			case 0:			//해체하기 실행(위쪽에서 실행됨)
				
				C_menuN =CLAN_AllEnd;;
				menuInit(C_menuN);
				break;				
			default:
				break;
			}
		}
		break;
	case CLAN_HELP:  //도움말
		i = chkeckMenuN();
		memset(menuMouseOver,-1,sizeof(int)*30);

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
		
		//ygy : 살려야함
		if(MOUSE0())			
		{	
			switch(i) 
			{
			case 0:
				//ygy : 살려야함
				DestroyClanHelpIcon();
				C_menuN = CLAN_CHIP;	//전단계로 돌아간다
				menuInit(C_menuN );
				//ygy : 살려야함
				MOUSE0()=0;

				break;	
			case 2:				
			case 3:				
			case 4:				
			case 5:
				//ygy : 살려야함
			//	g_HelpIconNumber = 0;				
			//	g_HelpIconNumber = i+nScl_Bar-2;

				//ygy : 살려야함
				LoadHelp(ClanHelpIcon[i+nScl_Bar-2].TextFilePath);

				C_menuN = CLAN_HELP_CONTENT;		//도움말로 돌아감.
				menuInit(C_menuN);
				//ygy : 살려야함
				MOUSE0()=0;

				break;


			case 6:
				nScl_Bar = tscr.topScrollLineNum(pCursorPos.y);
				//ktj : 스크롤 아이콘 위치재설정
				scrIconPos.top =  pCursorPos.y-8;//BackStartPos.y + SCROOBAR_TOP;
				break;
			
			default:
				//ygy : 살려야함
				MOUSE0()=0;
				break;
			}
		}		
		break;
	case CLAN_HELP_CONTENT://도움말
		i = chkeckMenuN();
			switch(i) {
			case 0:
				menuMouseOver[0] =1;
				break;					
			default:
				memset(menuMouseOver,-1,sizeof(int)*30);
				break;
			}

			
			//ktj : 삭제
			//ygy : 윌관련한 변수 TJwheel살려야함
			if(TJwheel != 0) {			//ktj : wheel>0 휠 앞으로 굴리기  wheel<0 휠 뒤로 굴리기
				scrIconPos.top = tscr.ret_WHEELpoint(TJwheel , scrIconPos.top);
				nScl_Bar = tscr.topScrollLineNum(scrIconPos.top);
				TJwheel = 0;
			}
			

			//ygy : 살려야함
			if(MOUSE0())			
			{	
				switch(i) 
				{
				case 0:
					C_menuN = CLAN_CHIP;		//도움말로 돌아감.
					menuInit(C_menuN);
					//ygy : 살려야함
					MOUSE0()=0;

					//ktj 스크롤관련
					tscr.Init(0);
					break;					
				case 5:
					nScl_Bar = tscr.topScrollLineNum(pCursorPos.y);
					//ktj : 스크롤 아이콘 위치재설정
					scrIconPos.top =  pCursorPos.y-8;//BackStartPos.y + SCROOBAR_TOP;
					break;

				default:
					//ygy : 살려야함
					MOUSE0()=0;
					break;
				}
			}		
		
		break;	
	case CLANCHIP_RELEASE_FLAG:
		switch(C_menuN2){
			case 0:
				ANSdata.ret_val= -1;		

				WebDB.Resume() ;		//웹db를 재개한다.
				WebDB.isPFlag(cldata.szID,cldata.name,cldata.ChaName,cldata.gserver,"100","0");		
				C_menuN2=1;
#ifdef CLAN_DEBUG
				DebugMessage("CLANCHIP_RELEASE_FLAG\277\241\274\255 WebDB.isPFlag(\276\306\300\314\265\360 : %s,\305\254\267\243\300\314\270\247 : %s,\304\263\270\257\305\315\300\314\270\247 : %s,\260\324\300\323\274\255\271\366 : %s,100,0)",cldata.szID,cldata.name,cldata.ChaName,cldata.gserver);
#endif
				break;
			case 1:
				switch(ANSdata.ret_val){
					case 0://플래그 삽입 실패
						WebDB.Suspend();
						C_menuN = CLAN_ERROR;
						wsprintf(szErrorMsg,"%s (27)",clanszError_Msg);
						cClanReport.menuInit(C_menuN,clanszError_Msg.pos.x,clanszError_Msg.pos.y);
						cClanReport.InitMsg(szErrorMsg);
						C_menuN2=-1;
#ifdef CLAN_DEBUG
			DebugMessage("CLANCHIP_RELEASE_FLAG\277\241\274\255 \273\360\300\324\275\307\306\320");
#endif
						break;
					case 1: //플래그 삽입 성공
						WebDB.Suspend();
						C_menuN2=-1;
						C_menuN = CLAN_AllEnd;						

						//=============================================================
					/*	cldata.myPosition = CLANNO_REGESTER;
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
						#endif*/
#ifdef CLAN_DEBUG
			DebugMessage("CLANCHIP_RELEASE_FLAG\277\241\274\255 \273\360\300\324\274\272\260\370");
#endif
						
						break;
					default:
						if(ANSdata.ret_val != -1) {		//전혀엉뚱한 답이 와서 에러가 났다.
							WebDB.Suspend();
							C_menuN = CLAN_ERROR;
							wsprintf(szErrorMsg,"%s (28)",clanszError_Msg.szMsg);
							cClanReport.menuInit(C_menuN,clanszError_Msg.pos.x,clanszError_Msg.pos.y);
							cClanReport.InitMsg(szErrorMsg);
							C_menuN2=-1;
#ifdef CLAN_DEBUG
			DebugMessage("CLANCHIP_RELEASE_FLAG\277\241\274\255 \265\360\306\372\306\256\277\241\267\257");
#endif
						}
						break;
				}
		}
		break;
	case ISPFLAG_CHIP:
		switch(C_menuN2){
			case 0:
				ANSdata.ret_val= -1;			

				WebDB.Resume() ;		//웹db를 재개한다.
				
				WebDB.isPFlag(cldata.szID,cldata.name,cldata.ChaName,cldata.gserver,cldata.PFlag,"0");
				
				C_menuN2=1;
#ifdef CLAN_DEBUG
				DebugMessage("ISPFLAG_CHIP\277\241\274\255 WebDB.isPFlag(\276\306\300\314\265\360 : %s,\305\254\267\243\300\314\270\247 : %s,\304\263\270\257\305\315\300\314\270\247 : %s,\260\324\300\323\274\255\271\366 : %s,\307\303\267\241\261\327 : %s,0)",cldata.szID,cldata.name,cldata.ChaName,cldata.gserver,cldata.PFlag);
#endif
				break;
			case 1:
				switch(ANSdata.ret_val){
					case 0://플래그 삽입 실패
						WebDB.Suspend();
						C_menuN = CLAN_ERROR;
						wsprintf(szErrorMsg,"%s (29)",clanszError_Msg.szMsg);
						cClanReport.menuInit(C_menuN,clanszError_Msg.pos.x,clanszError_Msg.pos.y);
						cClanReport.InitMsg(szErrorMsg);
						C_menuN2=-1;
#ifdef CLAN_DEBUG
				DebugMessage("ISPFLAG_CHIP\277\241\274\255 \273\360\300\324\275\307\306\320");
#endif
						break;
					case 1: //플래그 삽입 성공							
						WebDB.Suspend();														
						C_menuN2=-1;
						C_menuN = CLAN_CHIP;						
						menuInit(C_menuN);
						
						C_menuN2 = -1;
#ifdef CLAN_DEBUG
				DebugMessage("ISPFLAG_CHIP\277\241\274\255 \273\360\300\324\274\272\260\370");
#endif
						break;
					default:

						if(ANSdata.ret_val != -1) {		//전혀엉뚱한 답이 와서 에러가 났다.
							WebDB.Suspend();
							C_menuN = CLAN_ERROR;
							wsprintf(szErrorMsg,"%s (30)",clanszError_Msg.szMsg);
							cClanReport.menuInit(C_menuN,clanszError_Msg.pos.x,clanszError_Msg.pos.y);
							cClanReport.InitMsg(szErrorMsg);
							C_menuN2 = -1;
#ifdef CLAN_DEBUG
				DebugMessage("ISPFLAG_CHIP\277\241\274\255 \265\360\306\372\306\256\277\241\267\257");
#endif
						}						
						break;
				}
		}
		break;	
		//클랜원이 6인 이하일 경우
	case CLANCHIP_MAKE_FIRSTMESSAGE:  //처음 클랜이 만들어졌을때 한번 보여줌
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
				C_menuN = ISPFLAG_CHIP;		//도움말로 돌아감.
				menuInit(C_menuN);
				C_menuN2 = 0;
				break;				
			default:
				break;
			}
		}
		break;
	case CLANCHIP_DOWN6_CLANWON_MESSAGE:
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
				C_menuN = CLAN_CHIP;		//도움말로 돌아감.
				menuInit(C_menuN);
				C_menuN2 = 0;
				break;				
			default:
				break;
			}
		}
		break;
		
	case CLANCHIP_DOWN6_CLANWON:     //클랜원이 6명 이하
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
				C_menuN = ISPFLAG_CHIP;		//도움말로 돌아감.
				menuInit(C_menuN);
				C_menuN2 = 0;
				break;				
			default:
				break;
			}
		}
		break;
	case CLAN_NOMAKE:               //아직 클랜이 결성되지 않았을때
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
				C_menuN = CLAN_CHIP;		//도움말로 돌아감.
				menuInit(C_menuN);
				break;				
			default:
				break;
			}
		}

		break;
	
	//===========클랜 정보 읽어오기
	case CLAN_CHIP_ReadDATA:

		switch(C_menuN2) {
		case 0:
			ANSdata.ret_val= -1;
			
			WebDB.Resume() ;		//웹db를 재개한다.
			//클랜의 멤버데이타를 읽는다.
			WebDB.ReadClan(cldata.szID,cldata.gserver,cldata.ClanZang_Name);
#ifdef CLAN_DEBUG
			DebugMessage("CLAN_CHIP_ReadDATA\277\241\274\255 WebDB.ReadClan(\276\306\300\314\265\360 : %s,\260\324\300\323\274\255\271\366 : %s,\304\263\270\257\305\315\300\314\270\247 : %s)",cldata.szID,cldata.gserver,cldata.ClanZang_Name);
#endif
			C_menuN2=1;
			break;
		case 1:			

			switch(ANSdata.ret_val) 
			{
			case -1:
				//C_menuN = CLAN_CHIP_ReadDATA_LOADING;
				//menuInit(C_menuN);
				break;
				//ygy
			case 0://가입한 클랜이 없음
				WebDB.Suspend();
				//wsprintf(szErrorMsg, "가입한 클랜이 없음");
				C_menuN = CLAN_ERROR;
				wsprintf(szErrorMsg,"%s (31)",clanszError_Msg.szMsg);
				cClanReport.menuInit(C_menuN,clanszError_Msg.pos.x,clanszError_Msg.pos.y);
				cClanReport.InitMsg(szErrorMsg);				
				CLAN_ERROR_ret=255;		//에러메뉴보여준후그냥끝낸다.
				C_menuN2=-1;
#ifdef CLAN_DEBUG
			DebugMessage("CLAN_CHIP_ReadDATA\277\241\274\255 \270\256\305\317\304\332\265\345 0 \260\241\300\324\307\321 \305\254\267\243\300\314 \276\370\300\275");
#endif
				break;	
			case 1://정상적으로 클랜멤보데이타받아 파싱
				WebDB.Suspend();
				ParsigClanWon(ANSdata.ret_Buf);				
				if(bClan6Down)
				{
					bClan6Down = FALSE;
					C_menuN = CLAN_CHIP;
					menuInit(C_menuN);					
					break;
				}
				//npc접근했을때 클랜칩 이임,부클랜칩 임명,해임 관련메세지 출력.......
				if(cldata.nKFlag == 1) //클랜칩 위임  확인 : 3
				{
					cldata.nKFlag_Count = 1;
					C_menuN = CLANCHIP_LEAVECLAN_MESSAGE;
					menuInit(C_menuN);
					break;
				}
				else if(cldata.nKFlag == 16) //부클랜장 해임  확인 32
				{
					cldata.nKFlag_Count = 1;
					C_menuN = CLANCHIP_SUBCHIP_RELEASEMSG;
					menuInit(C_menuN);
					break;
				}
				else if(cldata.nKFlag == 4) //부클랜장 임명 확인 8
				{
					cldata.nKFlag_Count = 1;
					C_menuN = CLANCHIP_SUBCHIP_APPOINTMSG;
					menuInit(C_menuN);
					break;
				}
				else if(cldata.nKFlag == 17)//칩이임,부침해임에서 
				{
					cldata.nKFlag_Count = 1;
					C_menuN = CLANCHIP_LEAVECLAN_MESSAGE;
					menuInit(C_menuN);
					break;
				}
				else if(cldata.nKFlag == 19) //칩이임,부침해임에서 칩이임만 했을경우
				{
					cldata.nKFlag_Count = 1;
					C_menuN = CLANCHIP_SUBCHIP_RELEASEMSG;
					menuInit(C_menuN);
					break;
				}
				else if(cldata.nKFlag == 5) //칩이임,부침임명
				{
					cldata.nKFlag_Count = 1;
					C_menuN = CLANCHIP_LEAVECLAN_MESSAGE;
					menuInit(C_menuN);
					break;
				}
				else if(cldata.nKFlag == 7) //칩이임과 부침임명에서 칩이임만 확앴으핸
				{
					cldata.nKFlag_Count = 1;
					C_menuN = CLANCHIP_SUBCHIP_APPOINTMSG;
					menuInit(C_menuN);
					break;
				}
				else if(cldata.nKFlag == 20) //부칩해임 부칩임명
				{
					if(cldata.SubChip[0] ==0)
					{
						bSubChip = FALSE;
						cldata.nKFlag_Count = 1;
						C_menuN = CLANCHIP_SUBCHIP_APPOINTMSG;
						menuInit(C_menuN);
					}
					else
					{
						bSubChip = TRUE;
						cldata.nKFlag_Count = 1;
						C_menuN = CLANCHIP_SUBCHIP_RELEASEMSG;
						menuInit(C_menuN);
					}
					break;
				}
				else if(cldata.nKFlag == 52)//부칩해임 부칩임명에서 부칩해임만 확인했을 경우
				{
					cldata.nKFlag_Count = 1;
					C_menuN = CLANCHIP_SUBCHIP_APPOINTMSG;
					menuInit(C_menuN);
					break;
				}
				else if(cldata.nKFlag == 21) //칩이임,부침임명,부칩해임
				{
					cldata.nKFlag_Count = 1;
					C_menuN = CLANCHIP_LEAVECLAN_MESSAGE;
					menuInit(C_menuN);
					break;
				}	
				else if(cldata.nKFlag == 23) //클랜칩이임 부클랜칩 해임 부클랜칩 임명에서 (클랜칩 이임만 확인)
				{
					cldata.nKFlag_Count = 1;
					C_menuN = CLANCHIP_SUBCHIP_RELEASEMSG;
					menuInit(C_menuN);
					break;
				}
				else if(cldata.nKFlag == 28)
				{
					cldata.nKFlag_Count = 1;
					C_menuN = CLANCHIP_SUBCHIP_RELEASEMSG;
					menuInit(C_menuN);
					break;
				}
				else if(cldata.nKFlag == 55) //클랜칩이임 부클랜칩 해임 부클랜칩 임명에서 (클랜칩 이임, 부클랜칩 해임만 확인했을 경우)
				{
					cldata.nKFlag_Count = 1;
					C_menuN = CLANCHIP_SUBCHIP_APPOINTMSG;
					menuInit(C_menuN);
					break;
				}
				else{}
				//if(strcmp(cldata.LeaveClanFlag,"200") == 0)
				//{
				//	C_menuN = CLANCHIP_LEAVECLAN_MESSAGE;
				//	menuInit(C_menuN);
				//	break;
				//}
				//ygy : 플래그 관련 메세지 살릴것
				if((strcmp(cldata.stats,"0") == 0)&&(strcmp(cldata.PFlag,"0") == 0))
				{
					wsprintf(szDrawMenuMsg,clanMsg6.szMsg,cldata.nLastDay);
					strcpy(szTemp,clanMsg6.szMsg);
					strcpy(clanMsg6.szMsg,szDrawMenuMsg);					
					CheckMsgPos(&clanMsg6,BOX_MAX_WIDTH,0);
					strcpy(clanMsg6.szMsg,szTemp);

					if(cldata.bIsOneDay)
					{
						//strcpy(clanMsg6.szMsg,szDrawMenuMsg);
						wsprintf(szDrawMenuMsg,clanMsg7.szMsg,cldata.nHour,cldata.nMinute);
						strcpy(szTemp,clanMsg6.szMsg);
						strcpy(clanMsg6.szMsg,szDrawMenuMsg);
						//void CheckMsgPos(CLANMSG* clanmsg);
						CheckMsgPos(&clanMsg6,BOX_MAX_WIDTH,0);	
						strcpy(clanMsg6.szMsg,szTemp);
					}
					else
					{
						wsprintf(szDrawMenuMsg,clanMsg6.szMsg,cldata.nLastDay);

						strcpy(szTemp,clanMsg6.szMsg);

						strcpy(clanMsg6.szMsg,szDrawMenuMsg);
						//void CheckMsgPos(CLANMSG* clanmsg);
						CheckMsgPos(&clanMsg6,BOX_MAX_WIDTH,0);		

						strcpy(clanMsg6.szMsg,szTemp);
					}		

					C_menuN = CLAN_NOMAKE;	//stats가 0이면 6명이하이므로 더 모집하라는메시지보냄
					menuInit(C_menuN );
					bPFlag = FALSE;
					C_menuN2=-1;
					break;
				}
				else if((strcmp(cldata.stats,"1") == 0)&&(strcmp(cldata.PFlag,"0") == 0)) //클랜 결성되고 처음 보여주는 메세지
				{
					strcpy(cldata.PFlag,"2");
					strcpy(szTemp,clanMakeFinishClan.szMsg);
					wsprintf(szDrawMenuMsg,clanMakeFinishClan.szMsg,cldata.name);
					strcpy(clanMakeFinishClan.szMsg,szDrawMenuMsg);					
					//void CheckMsgPos(CLANMSG* clanmsg);
					CheckMsgPos(&clanMakeFinishClan,BOX_MAX_WIDTH,0);
					C_menuN = CLANCHIP_MAKE_FIRSTMESSAGE;	//stats가 0이면 6명이하이므로 더 모집하라는메시지보냄
					menuInit(C_menuN );
					strcpy(clanMakeFinishClan.szMsg,szTemp);
					C_menuN2=-1;
					bPFlag = TRUE;
					break;
				}				
				//else if((strcmp(cldata.stats,"0") == 0)&&(strcmp(cldata.PFlag,"2") == 0)) //클랜원이 6명이라로 떨어졌을 경우
				//{	
				//	strcpy(cldata.PFlag,"3");
				//	C_menuN = CLANCHIP_DOWN6_CLANWON;	
				//	menuInit(C_menuN );									
				//	C_menuN2=-1;
				//	break;
				//}				
				else if((strcmp(cldata.stats,"1") == 0)&&(strcmp(cldata.PFlag,"2") == 0)&&(nClanWon_Num <5))
				{					

					wsprintf(szDrawMenuMsg,clanMsg6.szMsg,cldata.nLastDay);
					strcpy(szTemp,clanMsg6.szMsg);
					strcpy(clanMsg6.szMsg,szDrawMenuMsg);					
					CheckMsgPos(&clanMsg6,BOX_MAX_WIDTH,0);
					strcpy(clanMsg6.szMsg,szTemp);

					if(cldata.bIsOneDay)
					{
						//strcpy(clanMsg6.szMsg,szDrawMenuMsg);
						wsprintf(szDrawMenuMsg,clanMsg7.szMsg,cldata.nHour,cldata.nMinute);
						strcpy(szTemp,clanMsg6.szMsg);
						strcpy(clanMsg6.szMsg,szDrawMenuMsg);
						//void CheckMsgPos(CLANMSG* clanmsg);
						CheckMsgPos(&clanMsg6,BOX_MAX_WIDTH,0);	
						strcpy(clanMsg6.szMsg,szTemp);
					}
					else
					{
						wsprintf(szDrawMenuMsg,clanMsg6.szMsg,cldata.nLastDay);

						strcpy(szTemp,clanMsg6.szMsg);

						strcpy(clanMsg6.szMsg,szDrawMenuMsg);
						//void CheckMsgPos(CLANMSG* clanmsg);
						CheckMsgPos(&clanMsg6,BOX_MAX_WIDTH,0);		

						strcpy(clanMsg6.szMsg,szTemp);
					}		

					C_menuN = CLAN_NOMAKE;	
					menuInit(C_menuN );									
					C_menuN2=-1;
					bPFlag = FALSE;
				}
				//else if((strcmp(cldata.stats,"1") == 0)&&(strcmp(cldata.PFlag,"3") == 0))
				//{
				//	strcpy(cldata.PFlag,"4");
				//	C_menuN = ISPFLAG_CHIP;		//도움말로 돌아감.
				//	menuInit(C_menuN);
				//	C_menuN2 = 0;					
				//}				
				//else if((strcmp(cldata.stats,"0") == 0)&&(strcmp(cldata.PFlag,"4") == 0))
				//{
				//	strcpy(cldata.PFlag,"3");
				//	C_menuN = CLANCHIP_DOWN6_CLANWON;	
				//	menuInit(C_menuN );									
				//	C_menuN2=-1;
				//}
				else // 1:1 or 1:3
				{
					C_menuN = CLAN_CHIP;	//바로 클랜칲메뉴로 간다.
					menuInit(C_menuN );									
					C_menuN2=-1;
					bPFlag = TRUE;
				}
#ifdef CLAN_DEBUG
			DebugMessage("CLAN_CHIP_ReadDATA\277\241\274\255 \270\256\305\317\304\332\265\345 1 \274\272\260\370");
#endif
				
				break;
			case 100:
				WebDB.Suspend();
				//ygy : 살려야함
				C_menuN = CLAN_ERROR;
				wsprintf(szErrorMsg,"%s (32)",clanszError_Msg.szMsg);
				cClanReport.InitMsg(szErrorMsg);
				cClanReport.menuInit(C_menuN,clanszError_Msg.pos.x,clanszError_Msg.pos.y);
				CLAN_ERROR_ret=CLAN_AllEnd;		//에러메뉴보여준후그냥끝낸다.			
				C_menuN2=-1;
#ifdef CLAN_DEBUG
			DebugMessage("CLAN_CHIP_ReadDATA\277\241\274\255 \270\256\305\317\304\332\265\345 100");
#endif
				break;
			case ISAO_ERR_TCP_CONNECT:
				WebDB.Suspend();
				//ygy : 살려야함
				C_menuN = CLAN_ERROR;
				wsprintf(szErrorMsg,"%s (33)",clanszError_Msg.szMsg);
				cClanReport.InitMsg(szErrorMsg);
				cClanReport.menuInit(C_menuN,clanszError_Msg.pos.x,clanszError_Msg.pos.y);
#ifdef CLAN_DEBUG
			DebugMessage("CLAN_CHIP_ReadDATA\277\241\274\255 \274\255\271\366\277\241 \301\242\274\323\307\322\274\366 \276\370\300\275");
#endif
				break;
			
			default:
				if(ANSdata.ret_val != -1) {		//전혀엉뚱한 답이 와서 에러가 났다.
					WebDB.Suspend();
					C_menuN = CLAN_AllEnd;				//클랜결성못하니 돌아가라고 하는메뉴로 간다.
					menuInit(C_menuN );

					C_menuN2=-1;
#ifdef CLAN_DEBUG
			DebugMessage("CLAN_CHIP_ReadDATA\277\241\274\255 \265\360\306\372\306\256\277\241\267\257");
#endif
				}
				break;
			}
			break;
		}
		break;
	
	//========칩용 인터페이스 end

	}
	
}



//*****************************************************************************
void CLANEntity_chip::RenderMain()
{
	if(life ==0) return;

}




//***************************************************************************************
void CLANEntity_chip::menuInit(int num)
{
	int bx = 0, by = 0;
	int      nNext = 0,i,nIconCount = 0,nIconPos = 0,nAdd=0,nWidth = 0;
	int      nFirstIconSave = 0;
	switch(num){

		//===================부 클랜칩 임명및 해임
		case CLANCHIP_SUBCHIP_APPOINTMSG://클랜부침이 임명되다고 한번 보여줌
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
		case CLANCHIP_SUBCHIP_RELEASEMSG://클랜부침이 해임되다고 한번 보여줌
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
		case CLANCHIP_SUBCHIP_LIST:			//클랜원들 리스트 보여줌 부 클랜칩이 있으면 따로 보여줌
			menuSu = 6+DISPLAY_MAX + 9;	
			
			nScl_Bar = 0;	
			nSavePos = -1;
			nReleaseClanWon = 0;
			ZeroMemory(banisF,sizeof(banisF));
			memset(menuPos,0,sizeof(RECT)*30);	
		// memset(banisF,0,sizeof(int)*100);
		// ZeroMemory(szRemoveClanWon,sizeof(char)*500);	
		// ZeroMemory(szRemoveClanWonResult,sizeof(szRemoveClanWonResult));  //클랜원을 저장할 변수 초기화
			
		
			#ifdef USE_PROSTONTALE		//=========== 포리스턴소스인경우
			bx = 32;
			by = 16;
			#else
			//GetObject(hSecession,sizeof(BITMAP),&bit);
			bx = 32;
			by = 16;
			#endif

			menuPos[0].left = BackStartPos.x + BUTTON2_X;		//추방
			menuPos[0].top = BackStartPos.y + BUTTON2_Y;
			menuPos[0].right = menuPos[0].left + BUTTONTEXT_WIDTH;
			menuPos[0].bottom = menuPos[0].top + BUTTONTEXT_HEIGHT;

			#ifdef USE_PROSTONTALE		//=========== 포리스턴소스인경우
			bx = 32;
			by = 16;
			#else
			bx = 32;
			by = 16;
			#endif

			menuPos[1].left = BackStartPos.x + BUTTON3_X;		//닫기
			menuPos[1].top = BackStartPos.y + BUTTON3_Y;
			menuPos[1].right = menuPos[1].left + BUTTONTEXT_WIDTH;
			menuPos[1].bottom = menuPos[1].top + BUTTONTEXT_HEIGHT;

			#ifdef USE_PROSTONTALE		//=========== 포리스턴소스인경우
			bx = 68;
			by = 27;
			#else
			bx = 68;
			by = 27;
			#endif

			menuPos[2].left = BackStartPos.x + BUTTON2_BOX_X;		//추방 테두리 박스
			menuPos[2].top = BackStartPos.y + BUTTON2_BOX_Y;
			menuPos[2].right = menuPos[2].left + BUTTONBOX_WIDTH;
			menuPos[2].bottom = menuPos[2].top + BUTTONBOX_HEIGHT;

			#ifdef USE_PROSTONTALE		//=========== 포리스턴소스인경우
			bx = 32;
			by = 16;
			#else
			bx = 32;
			by = 16;
			#endif

			menuPos[3].left = BackStartPos.x + BUTTON3_BOX_X;		//닫기 테두리 박스
			menuPos[3].top = BackStartPos.y + BUTTON3_BOX_Y;
			menuPos[3].right = menuPos[3].left + BUTTONBOX_WIDTH;
			menuPos[3].bottom = menuPos[3].top + BUTTONBOX_HEIGHT;		
			
			//================아이콘 설정				
			nIconCount = 3;
			//int	nClanWon[40];
			memset(nClanWon,0,sizeof(int)*40);
			
			for(i=0; i<nClanWon_Num; i++)
			
			{	
				if(i >=DISPLAY_MAX) break;
				nIconCount += 1;
				
				#ifdef USE_PROSTONTALE		//=========== 포리스턴소스인경우
				bx = 13;
				by = 17;
				#else
				bx = 13;
				by = 17;
				#endif

				menuPos[nIconCount].left = BackStartPos.x +  CLANWONLIST_X;		
				menuPos[nIconCount].top =  BackStartPos.y +   CLANWONLIST_Y +nAdd;//ICON_START + nAdd + REMOVE_ICON;
				menuPos[nIconCount].right = menuPos[nIconCount].left + 13;
				menuPos[nIconCount].bottom = menuPos[nIconCount].top + 17;
				nNext+=1;
				nAdd+=ICON_ADD;
				nClanWon[nIconCount] = 1;			
			}
			nIconCount = 11;

			//================스크롤 바
			if(nClanWon_Num > DISPLAY_MAX)
			
			{
				nIconCount+=1;
				#ifdef USE_PROSTONTALE		//=========== 포리스턴소스인경우
				bx = 8;
				by = 160;
				#else
				bx = 8;
				by = 160;
				#endif

				menuPos[nIconCount].left = BackStartPos.x + SCROLLBAR_LEFT;//SCROLLBAR_LEFT;
				menuPos[nIconCount].top =  BackStartPos.y + SCROOBAR_TOP;
				menuPos[nIconCount].right = menuPos[nIconCount].left + 8;
				menuPos[nIconCount].bottom = menuPos[nIconCount].top + 160;
				nClanWon[nIconCount] = 1;


				//ktj 스크롤관련
				tscr.Init(1);
				tscr.Init(	menuPos[nIconCount].top,//BackStartPos.y + SCROOBAR_TOP, 
						menuPos[nIconCount].top + 160,//256,
						nClanWon_Num, DISPLAY_MAX);						
				
				scrIconPos.left  = BackStartPos.x + SCROLLBAR_LEFT-5;//SCROLLBAR_LEFT-4;
				scrIconPos.top =  menuPos[nIconCount].top -8;//(BackStartPos.y + SCROOBAR_TOP)-8;
			}

			nIconCount+=1;
			#ifdef USE_PROSTONTALE		//=========== 포리스턴소스인경우
			bx = CLANTEXT_IMAGE_WIDTH;
			by = CLANTEXT_IMAGE_HEIGHT;
			#else
			
			bx = CLANTEXT_IMAGE_WIDTH;
			by = CLANTEXT_IMAGE_HEIGHT;
			#endif

			menuPos[nIconCount].left = BackStartPos.x + CLANTITLE_X;//CLANTEXT_IMAGE_X;		//텍스트 이미지 출력
			menuPos[nIconCount].top =  BackStartPos.y +   CLANTITLE_Y;//CLANTEXT_IMAGE_Y;
			menuPos[nIconCount].right = menuPos[nIconCount].left + bx;
			menuPos[nIconCount].bottom = menuPos[nIconCount].top + by;		


			nIconCount+=1;
			#ifdef USE_PROSTONTALE		//=========== 포리스턴소스인경우
			bx = CLANTEXT_IMAGE_WIDTH;
			by = CLANTEXT_IMAGE_HEIGHT;
			#else
			
			bx = CLANTEXT_IMAGE_WIDTH;
			by = CLANTEXT_IMAGE_HEIGHT;
			#endif

			menuPos[nIconCount].left = BackStartPos.x + 9;//CLANTEXT_IMAGE_X;		//텍스트 이미지 출력
			menuPos[nIconCount].top =  BackStartPos.y +   109 - 70;//CLANTEXT_IMAGE_Y;
			menuPos[nIconCount].right = menuPos[nIconCount].left + bx;
			menuPos[nIconCount].bottom = menuPos[nIconCount].top + by;		
			break;
			
		case CLANCHIP_RESUBCHIP_APPOINT:		//부 클랜칩 임명 재확인
			menuSu = 2;			

			#ifdef USE_PROSTONTALE		//=========== 프리스턴소스인경우
			bx = 32;
			by = 16;
			#else			
			bx = 32;
			by = 16;
			#endif
			
			menuPos[0].left = BackStartPos.x + BUTTON2_X;//174;		//yes
			menuPos[0].top = BackStartPos.y + + BUTTON2_Y;//353;
			menuPos[0].right = menuPos[0].left + bx;
			menuPos[0].bottom = menuPos[0].top + by;



			#ifdef USE_PROSTONTALE		//=========== 프리스턴소스인경우
			bx = 32;
			by = 16;
			#else			
			bx = 32;
			by = 16;
			#endif

			menuPos[1].left = BackStartPos.x + BUTTON3_X;//254		//no
			menuPos[1].top = BackStartPos.y + BUTTON3_Y; //353
			menuPos[1].right = menuPos[1].left+ bx;
			menuPos[1].bottom = menuPos[1].top + by;

			//텍스트 출력 위치
			menuPos[2].left = BackStartPos.x + SubChipAppoint.pos.x;
			menuPos[2].top = BackStartPos.y + SubChipAppoint.pos.y+30;
			menuPos[2].right = menuPos[2].left+280;
			menuPos[2].bottom = menuPos[2].top+250;

			//yes, no 테두리 박스
			#ifdef USE_PROSTONTALE		//=========== 프리스턴소스인경우
			bx = 68;
			by = 27;
			#else			
			bx = 68;
			by = 27;
			#endif

			menuPos[3].left = BackStartPos.x + BUTTON2_BOX_X;//166;		//yes 테두리 박스
			menuPos[3].top = BackStartPos.y + BUTTON2_BOX_Y;//348
			menuPos[3].right = menuPos[3].left + bx;
			menuPos[3].bottom = menuPos[3].top + by;

			#ifdef USE_PROSTONTALE		//=========== 프리스턴소스인경우
			bx = 68;
			by = 27;
			#else			
			bx = 68;
			by = 27;
			#endif
			menuPos[4].left = BackStartPos.x + BUTTON3_BOX_X;	//246	//no 테두리 박스
			menuPos[4].top = BackStartPos.y + BUTTON3_BOX_Y;//348
			menuPos[4].right = menuPos[4].left + bx;
			menuPos[4].bottom = menuPos[4].top + by;

			#ifdef USE_PROSTONTALE		//=========== 프리스턴소스인경우
			bx = 68;
			by = 27;
			#else			
			bx = 68;
			by = 27;
			#endif
			menuPos[5].left = BackStartPos.x + 9;	//246	//라인
			menuPos[5].top = BackStartPos.y + 109-65;//348
			menuPos[5].right = menuPos[5].left + bx;
			menuPos[5].bottom = menuPos[5].top + by;

			#ifdef USE_PROSTONTALE		//=========== 프리스턴소스인경우
			bx = 68;
			by = 27;
			#else			
			bx = 68;
			by = 27;
			#endif
			menuPos[6].left = BackStartPos.x + CLANTITLE_X;	//246	//no 테두리 박스
			menuPos[6].top = BackStartPos.y + CLANTITLE_Y;//348
			menuPos[6].right = menuPos[6].left + bx;
			menuPos[6].bottom = menuPos[6].top + by;

			break;
		case CLANCHIP_RESUBCHIP_APPOINT_END:	//부 클랜칩임명 마지막 메세지
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
		case CLANCHIP_RELEASESUBCHIP:			//부 클랜칩해임 재확인
			menuSu = 2;			

			#ifdef USE_PROSTONTALE		//=========== 프리스턴소스인경우
			bx = 32;
			by = 16;
			#else			
			bx = 32;
			by = 16;
			#endif
			
			menuPos[0].left = BackStartPos.x + BUTTON2_X;//174;		//yes
			menuPos[0].top = BackStartPos.y + + BUTTON2_Y;//353;
			menuPos[0].right = menuPos[0].left + bx;
			menuPos[0].bottom = menuPos[0].top + by;



			#ifdef USE_PROSTONTALE		//=========== 프리스턴소스인경우
			bx = 32;
			by = 16;
			#else			
			bx = 32;
			by = 16;
			#endif

			menuPos[1].left = BackStartPos.x + BUTTON3_X;//254		//no
			menuPos[1].top = BackStartPos.y + BUTTON3_Y; //353
			menuPos[1].right = menuPos[1].left+ bx;
			menuPos[1].bottom = menuPos[1].top + by;

			//텍스트 출력 위치
			menuPos[2].left = BackStartPos.x + SubChipRelease.pos.x;
			menuPos[2].top = BackStartPos.y + SubChipRelease.pos.y+30;
			menuPos[2].right = menuPos[2].left+280;
			menuPos[2].bottom = menuPos[2].top+250;

			//yes, no 테두리 박스
			#ifdef USE_PROSTONTALE		//=========== 프리스턴소스인경우
			bx = 68;
			by = 27;
			#else			
			bx = 68;
			by = 27;
			#endif

			menuPos[3].left = BackStartPos.x + BUTTON2_BOX_X;//166;		//yes 테두리 박스
			menuPos[3].top = BackStartPos.y + BUTTON2_BOX_Y;//348
			menuPos[3].right = menuPos[3].left + bx;
			menuPos[3].bottom = menuPos[3].top + by;

			#ifdef USE_PROSTONTALE		//=========== 프리스턴소스인경우
			bx = 68;
			by = 27;
			#else			
			bx = 68;
			by = 27;
			#endif
			menuPos[4].left = BackStartPos.x + BUTTON3_BOX_X;	//246	//no 테두리 박스
			menuPos[4].top = BackStartPos.y + BUTTON3_BOX_Y;//348
			menuPos[4].right = menuPos[4].left + bx;
			menuPos[4].bottom = menuPos[4].top + by;

			#ifdef USE_PROSTONTALE		//=========== 프리스턴소스인경우
			bx = 68;
			by = 27;
			#else			
			bx = 68;
			by = 27;
			#endif
			menuPos[5].left = BackStartPos.x + 9;	//246	//라인
			menuPos[5].top = BackStartPos.y + 109-65;//348
			menuPos[5].right = menuPos[5].left + bx;
			menuPos[5].bottom = menuPos[5].top + by;

			#ifdef USE_PROSTONTALE		//=========== 프리스턴소스인경우
			bx = 68;
			by = 27;
			#else			
			bx = 68;
			by = 27;
			#endif
			menuPos[6].left = BackStartPos.x + CLANTITLE_X;	//246	//no 테두리 박스
			menuPos[6].top = BackStartPos.y + CLANTITLE_Y;//348
			menuPos[6].right = menuPos[6].left + bx;
			menuPos[6].bottom = menuPos[6].top + by;

			break;
		case CLANCHIP_RELEASESUBCHIP_END:		//클랜칩 해임 마지막 메세지
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
		case CLAN_CHIP:  //클랜 칩						
			menuSu = 20;			
#ifdef USE_PROSTONTALE		//=========== 포리스턴소스인경우
			if(hMark == NULL) hMark = GetClanMark();
			
#else
			if(hMark == NULL) hMark = GetClanMark();
#endif

			#ifdef USE_PROSTONTALE		//=========== 포리스턴소스인경우
			bx = 152;
			by = 28;
			#else			
			bx = 152;
			by = 28;
			#endif

			
			menuPos[0].left = BackStartPos.x +CLANNAME_X;		//클랜 이름
			menuPos[0].top =  BackStartPos.y +CLANNAME_Y + 4;
			menuPos[0].right = menuPos[0].left + bx;
			menuPos[0].bottom = menuPos[0].top + by;

			#ifdef USE_PROSTONTALE		//=========== 포리스턴소스인경우
			bx = 64;
			by = 16;
			#else			
			bx = 64;
			by = 16;
			#endif

			menuPos[10].left = BackStartPos.x + CLANNAMETEXT_X;		//클랜이름
			menuPos[10].top =  BackStartPos.y + CLANNAMETEXT_Y + 4;//BackStartPos.y +BUTTON3_BOX_Y;
			menuPos[10].right = 0;//menuPos[7].left + bx;
			menuPos[10].bottom = 0;//menuPos[7].top + by;

			#ifdef USE_PROSTONTALE		//=========== 포리스턴소스인경우
			bx = 16;
			by = 16;
			#else
			bx = 16;
			by = 16;
			#endif

			menuPos[1].left = BackStartPos.x +CLANMARKBOX_X - 68;		//마크 박스
			menuPos[1].top	 = BackStartPos.y +CLANMARKBOX_Y + 55;
			menuPos[1].right = menuPos[1].left + bx;
			menuPos[1].bottom = menuPos[1].top + by;

			#ifdef USE_PROSTONTALE		//=========== 포리스턴소스인경우
			bx = 16;
			by = 16;
			#else
			
			bx = 16;
			by = 16;
			#endif

			menuPos[9].left = BackStartPos.x +CLANMARK_X - 68;		//마크
			menuPos[9].top	 = BackStartPos.y +CLANMARK_Y + 55;
			menuPos[9].right = menuPos[9].left + bx;
			menuPos[9].bottom = menuPos[9].top + by;

			#ifdef USE_PROSTONTALE		//=========== 포리스턴소스인경우
			bx = 48;
			by = 23;
			#else			
			bx = 48;
			by = 23;
			#endif

			menuPos[8].left = BackStartPos.x + BUTTON1_BOX_X;		// 나가기 테두리 박스
			menuPos[8].top =  BackStartPos.y + BUTTON1_BOX_Y;
			menuPos[8].right = menuPos[8].left + bx;
			menuPos[8].bottom = menuPos[8].top + by;
	        

			#ifdef USE_PROSTONTALE		//=========== 포리스턴소스인경우
			bx = 50;
			by = 44;
			#else			
			bx = 50;
			by = 44;
			#endif

			menuPos[2].left = -1;//BackStartPos.x + 185+ CHIP_BUTTON_X_MOVE;		//클랜마크
			menuPos[2].top = -1;//BackStartPos.y +  110-CHIP_BUTTON_Y_MOVE;
			menuPos[2].right = -1;//menuPos[2].left + bx;
			menuPos[2].bottom = -1;//menuPos[2].top + by;


			#ifdef USE_PROSTONTALE		//=========== 포리스턴소스인경우
			bx = 120;
			by = 30;
			#else			
			bx = 120;
			by = 30;
			#endif

			menuPos[3].left = BackStartPos.x + CLANCHIPMENU_REMOVE_X;		//추방
			menuPos[3].top = BackStartPos.y +  CLANCHIPMENU_REMOVE_Y;
			menuPos[3].right = menuPos[3].left + bx;
			menuPos[3].bottom = menuPos[3].top + by;

			#ifdef USE_PROSTONTALE		//=========== 포리스턴소스인경우
			bx = 120;
			by = 30;
			#else
			
			bx = 120;
			by = 30;
			#endif

			menuPos[4].left = BackStartPos.x + CLANCHIPMENU_RELEASE_X; 	//클랜 해제
			menuPos[4].top =  BackStartPos.y + CLANCHIPMENU_RELEASE_y;
			menuPos[4].right = menuPos[4].left + bx;
			menuPos[4].bottom = menuPos[4].top + by;

			#ifdef USE_PROSTONTALE		//=========== 포리스턴소스인경우
			bx = 120;
			by = 30;
			#else
			
			bx = 120;
			by = 30;
			#endif

			menuPos[5].left = BackStartPos.x +  CLANCHIPMENU_HELP_X;		//도움말
			menuPos[5].top =  BackStartPos.y + CLANCHIPMENU_HELP_Y;
			menuPos[5].right = menuPos[5].left + bx;
			menuPos[5].bottom = menuPos[5].top + by;

			#ifdef USE_PROSTONTALE		//=========== 포리스턴소스인경우
			bx = 32;
			by = 16;
			#else			
			bx = 32;
			by = 16;
			#endif

			menuPos[6].left = BackStartPos.x + BUTTON1_X;		//나가기
			menuPos[6].top =  BackStartPos.y + BUTTON1_Y;
			menuPos[6].right = menuPos[6].left + bx;
			menuPos[6].bottom = menuPos[6].top + by;

			#ifdef USE_PROSTONTALE		//=========== 포리스턴소스인경우
			bx = 48;
			by = 27;
			#else			
			bx = 48;
			by = 27;
			#endif

			menuPos[7].left = BackStartPos.x + BUTTON1_BOX_X;		//나가기 테두리 박스
			menuPos[7].top =  BackStartPos.y + BUTTON1_BOX_Y;
			menuPos[7].right = menuPos[7].left + bx;
			menuPos[7].bottom = menuPos[7].top + by;

			//해당하는 작은버턴 영역
			#ifdef USE_PROSTONTALE		//=========== 포리스턴소스인경우
			bx = 30;
			by = 30;
			#else			
			bx = 30;
			by = 30;
			#endif

			menuPos[11].left = BackStartPos.x + 75;		//해체 왼쪽
			menuPos[11].top =  BackStartPos.y + 105;
			menuPos[11].right = menuPos[11].left + bx;
			menuPos[11].bottom = menuPos[11].top + by;

			#ifdef USE_PROSTONTALE		//=========== 포리스턴소스인경우
			bx = 30;
			by = 30;
			#else			
			bx = 30;
			by = 30;
			#endif

			menuPos[12].left = BackStartPos.x + 216;		//해체 오른쪽
			menuPos[12].top =  BackStartPos.y + 105;
			menuPos[12].right = menuPos[12].left + bx;
			menuPos[12].bottom = menuPos[12].top + by;

			#ifdef USE_PROSTONTALE		//=========== 포리스턴소스인경우
			bx = 30;
			by = 30;
			#else			
			bx = 30;
			by = 30;
			#endif

			menuPos[13].left = BackStartPos.x + 75;		//추방 왼쪽
			menuPos[13].top =  BackStartPos.y + 155;
			menuPos[13].right = menuPos[13].left + bx;
			menuPos[13].bottom = menuPos[13].top + by;

			#ifdef USE_PROSTONTALE		//=========== 포리스턴소스인경우
			bx = 30;
			by = 30;
			#else			
			bx = 30;
			by = 30;
			#endif

			menuPos[14].left = BackStartPos.x + 216;		//추방 오른쪽
			menuPos[14].top =  BackStartPos.y + 155;
			menuPos[14].right = menuPos[14].left + bx;
			menuPos[14].bottom = menuPos[14].top + by;

			#ifdef USE_PROSTONTALE		//=========== 포리스턴소스인경우
			bx = 30;
			by = 30;
			#else			
			bx = 30;
			by = 30;
			#endif

			menuPos[15].left = BackStartPos.x + 75;		//도움말 왼쪽
			menuPos[15].top =  BackStartPos.y + 205;
			menuPos[15].right = menuPos[15].left + bx;
			menuPos[15].bottom = menuPos[15].top + by;

			#ifdef USE_PROSTONTALE		//=========== 포리스턴소스인경우
			bx = 30;
			by = 30;
			#else			
			bx = 30;
			by = 30;
			#endif
			menuPos[16].left = BackStartPos.x + 216;		//도움말 오른쪽
			menuPos[16].top =  BackStartPos.y + 205;
			menuPos[16].right = menuPos[16].left + bx;
			menuPos[16].bottom = menuPos[16].top + by;		

			#ifdef USE_PROSTONTALE		//=========== 포리스턴소스인경우
			bx = 30;
			by = 30;
			#else			
			bx = 30;
			by = 30;
			#endif
			menuPos[17].left = BackStartPos.x + 9;		//라인
			menuPos[17].top =  BackStartPos.y + 109 - 60;
			menuPos[17].right = menuPos[17].left + bx;
			menuPos[17].bottom = menuPos[17].top + by;		
			//이임
			#ifdef USE_PROSTONTALE		//=========== 포리스턴소스인경우
			bx = 120;
			by = 30;
			#else			
			bx = 120;
			by = 30;
			#endif
			menuPos[18].left = BackStartPos.x + CLANCHIPMENU_LC_X;		//이임
			menuPos[18].top =  BackStartPos.y + CLANCHIPMENU_LC_Y;
			menuPos[18].right = menuPos[18].left + bx;
			menuPos[18].bottom = menuPos[18].top + by;		

			#ifdef USE_PROSTONTALE		//=========== 포리스턴소스인경우
			bx = 120;
			by = 30;
			#else			
			bx = 120;
			by = 30;
			#endif
			menuPos[19].left = BackStartPos.x + CLANCHIPMENU_SUBCHIP_X;		//부침임명 및 해임
			menuPos[19].top =  BackStartPos.y + CLANCHIPMENU_SUBCHIP_Y;
			menuPos[19].right = menuPos[19].left + bx;
			menuPos[19].bottom = menuPos[19].top + by;		




			break;
		case CLANCHIP_LEAVECLAN_MESSAGE:
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
		case CLANCHIP_LEAVECLAN_RECONFIRM:
			menuSu = 2;			

			#ifdef USE_PROSTONTALE		//=========== 프리스턴소스인경우
			bx = 32;
			by = 16;
			#else			
			bx = 32;
			by = 16;
			#endif
			
			menuPos[0].left = BackStartPos.x + BUTTON2_X;//174;		//yes
			menuPos[0].top = BackStartPos.y + + BUTTON2_Y;//353;
			menuPos[0].right = menuPos[0].left + bx;
			menuPos[0].bottom = menuPos[0].top + by;



			#ifdef USE_PROSTONTALE		//=========== 프리스턴소스인경우
			bx = 32;
			by = 16;
			#else			
			bx = 32;
			by = 16;
			#endif

			menuPos[1].left = BackStartPos.x + BUTTON3_X;//254		//no
			menuPos[1].top = BackStartPos.y + BUTTON3_Y; //353
			menuPos[1].right = menuPos[1].left+ bx;
			menuPos[1].bottom = menuPos[1].top + by;

			//텍스트 출력 위치
			menuPos[2].left = BackStartPos.x + LeaveClanReConfirm.pos.x;
			menuPos[2].top = BackStartPos.y + LeaveClanReConfirm.pos.y;
			menuPos[2].right = menuPos[2].left+280;
			menuPos[2].bottom = menuPos[2].top+250;

			//yes, no 테두리 박스
			#ifdef USE_PROSTONTALE		//=========== 프리스턴소스인경우
			bx = 68;
			by = 27;
			#else			
			bx = 68;
			by = 27;
			#endif

			menuPos[3].left = BackStartPos.x + BUTTON2_BOX_X;//166;		//yes 테두리 박스
			menuPos[3].top = BackStartPos.y + BUTTON2_BOX_Y;//348
			menuPos[3].right = menuPos[3].left + bx;
			menuPos[3].bottom = menuPos[3].top + by;

			#ifdef USE_PROSTONTALE		//=========== 프리스턴소스인경우
			bx = 68;
			by = 27;
			#else			
			bx = 68;
			by = 27;
			#endif
			menuPos[4].left = BackStartPos.x + BUTTON3_BOX_X;	//246	//no 테두리 박스
			menuPos[4].top = BackStartPos.y + BUTTON3_BOX_Y;//348
			menuPos[4].right = menuPos[4].left + bx;
			menuPos[4].bottom = menuPos[4].top + by;
			break;
		case CLANCHIP_LEAVECLANDOWNLEVEL:
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
			menuPos[1].left = BackStartPos.x + LeaveClanDownLevel.pos.x;
			menuPos[1].top = BackStartPos.y + LeaveClanDownLevel.pos.y;

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
		case CLANCHIP_NOSUBCHIP:
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
			menuPos[1].left = BackStartPos.x + NoSubChip.pos.x;
			menuPos[1].top = BackStartPos.y + NoSubChip.pos.y;

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

		case CLANCHIP_NOLEAVECLAN:
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
			menuPos[1].left = BackStartPos.x + NoLeaveClan.pos.x;
			menuPos[1].top = BackStartPos.y + NoLeaveClan.pos.y;

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
		case CLANCHIP_LEAVECLAN_NOLEVEL:
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
			menuPos[1].left = BackStartPos.x + LeaveClanNoLevel.pos.x;
			menuPos[1].top = BackStartPos.y + LeaveClanNoLevel.pos.y;

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
		case CLANCHIP_LEAVECLAN_CONFIRM:
			menuSu = 2;			

			#ifdef USE_PROSTONTALE		//=========== 프리스턴소스인경우
			bx = 32;
			by = 16;
			#else			
			bx = 32;
			by = 16;
			#endif
			
			menuPos[0].left = BackStartPos.x + BUTTON2_X;//174;		//yes
			menuPos[0].top = BackStartPos.y + + BUTTON2_Y;//353;
			menuPos[0].right = menuPos[0].left + bx;
			menuPos[0].bottom = menuPos[0].top + by;



			#ifdef USE_PROSTONTALE		//=========== 프리스턴소스인경우
			bx = 32;
			by = 16;
			#else			
			bx = 32;
			by = 16;
			#endif

			menuPos[1].left = BackStartPos.x + BUTTON3_X;//254		//no
			menuPos[1].top = BackStartPos.y + BUTTON3_Y; //353
			menuPos[1].right = menuPos[1].left+ bx;
			menuPos[1].bottom = menuPos[1].top + by;

			//텍스트 출력 위치
			menuPos[2].left = BackStartPos.x + LeaveClanConfirm.pos.x;
			menuPos[2].top = BackStartPos.y + LeaveClanConfirm.pos.y+30;
			menuPos[2].right = menuPos[2].left+280;
			menuPos[2].bottom = menuPos[2].top+250;

			//yes, no 테두리 박스
			#ifdef USE_PROSTONTALE		//=========== 프리스턴소스인경우
			bx = 68;
			by = 27;
			#else			
			bx = 68;
			by = 27;
			#endif

			menuPos[3].left = BackStartPos.x + BUTTON2_BOX_X;//166;		//yes 테두리 박스
			menuPos[3].top = BackStartPos.y + BUTTON2_BOX_Y;//348
			menuPos[3].right = menuPos[3].left + bx;
			menuPos[3].bottom = menuPos[3].top + by;

			#ifdef USE_PROSTONTALE		//=========== 프리스턴소스인경우
			bx = 68;
			by = 27;
			#else			
			bx = 68;
			by = 27;
			#endif
			menuPos[4].left = BackStartPos.x + BUTTON3_BOX_X;	//246	//no 테두리 박스
			menuPos[4].top = BackStartPos.y + BUTTON3_BOX_Y;//348
			menuPos[4].right = menuPos[4].left + bx;
			menuPos[4].bottom = menuPos[4].top + by;

			#ifdef USE_PROSTONTALE		//=========== 프리스턴소스인경우
			bx = 68;
			by = 27;
			#else			
			bx = 68;
			by = 27;
			#endif
			menuPos[5].left = BackStartPos.x + 9;	//246	//라인
			menuPos[5].top = BackStartPos.y + 109-65;//348
			menuPos[5].right = menuPos[5].left + bx;
			menuPos[5].bottom = menuPos[5].top + by;

			#ifdef USE_PROSTONTALE		//=========== 프리스턴소스인경우
			bx = 68;
			by = 27;
			#else			
			bx = 68;
			by = 27;
			#endif
			menuPos[6].left = BackStartPos.x + CLANTITLE_X;	//246	//no 테두리 박스
			menuPos[6].top = BackStartPos.y + CLANTITLE_Y;//348
			menuPos[6].right = menuPos[6].left + bx;
			menuPos[6].bottom = menuPos[6].top + by;

			break;
		case CLANCHIP_LEAVECLAN_NOMONEY:
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
			menuPos[1].left = BackStartPos.x + LeaveClanNoMoney.pos.x;
			menuPos[1].top = BackStartPos.y + LeaveClanNoMoney.pos.y;

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
		case CLANCHIP_LEAVECLAN_SUCESS:
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
			menuPos[1].left = BackStartPos.x + LeaveClanSuccess.pos.x;
			menuPos[1].top = BackStartPos.y + LeaveClanSuccess.pos.y;

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
		case CLANCHIP_LEAVECLAN:
			menuSu = 6+DISPLAY_MAX + 9;	
			
			nScl_Bar = 0;	
			nSavePos = -1;
			nReleaseClanWon = 0;
			ZeroMemory(banisF,sizeof(banisF));
			memset(menuPos,0,sizeof(RECT)*30);	
		// memset(banisF,0,sizeof(int)*100);
		// ZeroMemory(szRemoveClanWon,sizeof(char)*500);	
		// ZeroMemory(szRemoveClanWonResult,sizeof(szRemoveClanWonResult));  //클랜원을 저장할 변수 초기화
			
		
			#ifdef USE_PROSTONTALE		//=========== 포리스턴소스인경우
			bx = 32;
			by = 16;
			#else
			//GetObject(hSecession,sizeof(BITMAP),&bit);
			bx = 32;
			by = 16;
			#endif

			menuPos[0].left = BackStartPos.x + BUTTON2_X;		//추방
			menuPos[0].top = BackStartPos.y + BUTTON2_Y;
			menuPos[0].right = menuPos[0].left + BUTTONTEXT_WIDTH;
			menuPos[0].bottom = menuPos[0].top + BUTTONTEXT_HEIGHT;

			#ifdef USE_PROSTONTALE		//=========== 포리스턴소스인경우
			bx = 32;
			by = 16;
			#else
			bx = 32;
			by = 16;
			#endif

			menuPos[1].left = BackStartPos.x + BUTTON3_X;		//닫기
			menuPos[1].top = BackStartPos.y + BUTTON3_Y;
			menuPos[1].right = menuPos[1].left + BUTTONTEXT_WIDTH;
			menuPos[1].bottom = menuPos[1].top + BUTTONTEXT_HEIGHT;

			#ifdef USE_PROSTONTALE		//=========== 포리스턴소스인경우
			bx = 68;
			by = 27;
			#else
			bx = 68;
			by = 27;
			#endif

			menuPos[2].left = BackStartPos.x + BUTTON2_BOX_X;		//추방 테두리 박스
			menuPos[2].top = BackStartPos.y + BUTTON2_BOX_Y;
			menuPos[2].right = menuPos[2].left + BUTTONBOX_WIDTH;
			menuPos[2].bottom = menuPos[2].top + BUTTONBOX_HEIGHT;

			#ifdef USE_PROSTONTALE		//=========== 포리스턴소스인경우
			bx = 32;
			by = 16;
			#else
			bx = 32;
			by = 16;
			#endif

			menuPos[3].left = BackStartPos.x + BUTTON3_BOX_X;		//닫기 테두리 박스
			menuPos[3].top = BackStartPos.y + BUTTON3_BOX_Y;
			menuPos[3].right = menuPos[3].left + BUTTONBOX_WIDTH;
			menuPos[3].bottom = menuPos[3].top + BUTTONBOX_HEIGHT;		
			
			//================아이콘 설정				
			nIconCount = 3;
			//int	nClanWon[40];
			memset(nClanWon,0,sizeof(int)*40);
			
			for(i=0; i<nClanWon_Num; i++)
			
			{	
				if(i >=DISPLAY_MAX) break;
				nIconCount += 1;
				
				#ifdef USE_PROSTONTALE		//=========== 포리스턴소스인경우
				bx = 13;
				by = 17;
				#else
				bx = 13;
				by = 17;
				#endif

				menuPos[nIconCount].left = BackStartPos.x +  CLANWONLIST_X;		
				menuPos[nIconCount].top =  BackStartPos.y +   CLANWONLIST_Y +nAdd;//ICON_START + nAdd + REMOVE_ICON;
				menuPos[nIconCount].right = menuPos[nIconCount].left + 13;
				menuPos[nIconCount].bottom = menuPos[nIconCount].top + 17;
				nNext+=1;
				nAdd+=ICON_ADD;
				nClanWon[nIconCount] = 1;			
			}
			nIconCount = 11;

			//================스크롤 바
			if(nClanWon_Num > DISPLAY_MAX)
			
			{
				nIconCount+=1;
				#ifdef USE_PROSTONTALE		//=========== 포리스턴소스인경우
				bx = 8;
				by = 160;
				#else
				bx = 8;
				by = 160;
				#endif

				menuPos[nIconCount].left = BackStartPos.x + SCROLLBAR_LEFT;//SCROLLBAR_LEFT;
				menuPos[nIconCount].top =  BackStartPos.y + SCROOBAR_TOP;
				menuPos[nIconCount].right = menuPos[nIconCount].left + 8;
				menuPos[nIconCount].bottom = menuPos[nIconCount].top + 160;
				nClanWon[nIconCount] = 1;


				//ktj 스크롤관련
				tscr.Init(1);
				tscr.Init(	menuPos[nIconCount].top,//BackStartPos.y + SCROOBAR_TOP, 
						menuPos[nIconCount].top + 160,//256,
						nClanWon_Num, DISPLAY_MAX);						
				
				scrIconPos.left  = BackStartPos.x + SCROLLBAR_LEFT-5;//SCROLLBAR_LEFT-4;
				scrIconPos.top =  menuPos[nIconCount].top -8;//(BackStartPos.y + SCROOBAR_TOP)-8;
			}

			nIconCount+=1;
			#ifdef USE_PROSTONTALE		//=========== 포리스턴소스인경우
			bx = CLANTEXT_IMAGE_WIDTH;
			by = CLANTEXT_IMAGE_HEIGHT;
			#else
			
			bx = CLANTEXT_IMAGE_WIDTH;
			by = CLANTEXT_IMAGE_HEIGHT;
			#endif

			menuPos[nIconCount].left = BackStartPos.x + CLANTITLE_X;//CLANTEXT_IMAGE_X;		//텍스트 이미지 출력
			menuPos[nIconCount].top =  BackStartPos.y +   CLANTITLE_Y;//CLANTEXT_IMAGE_Y;
			menuPos[nIconCount].right = menuPos[nIconCount].left + bx;
			menuPos[nIconCount].bottom = menuPos[nIconCount].top + by;		


			nIconCount+=1;
			#ifdef USE_PROSTONTALE		//=========== 포리스턴소스인경우
			bx = CLANTEXT_IMAGE_WIDTH;
			by = CLANTEXT_IMAGE_HEIGHT;
			#else
			
			bx = CLANTEXT_IMAGE_WIDTH;
			by = CLANTEXT_IMAGE_HEIGHT;
			#endif

			menuPos[nIconCount].left = BackStartPos.x + 9;//CLANTEXT_IMAGE_X;		//텍스트 이미지 출력
			menuPos[nIconCount].top =  BackStartPos.y +   109 - 70;//CLANTEXT_IMAGE_Y;
			menuPos[nIconCount].right = menuPos[nIconCount].left + bx;
			menuPos[nIconCount].bottom = menuPos[nIconCount].top + by;		
			break;
		case CLAN_REMOVE://추방
			menuSu = 6+DISPLAY_MAX + 8;	
			
			nScl_Bar = 0;	
			nReleaseClanWon = 0;
			memset(menuPos,0,sizeof(RECT)*30);	
		// memset(banisF,0,sizeof(int)*100);
		// ZeroMemory(szRemoveClanWon,sizeof(char)*500);	
		// ZeroMemory(szRemoveClanWonResult,sizeof(szRemoveClanWonResult));  //클랜원을 저장할 변수 초기화
			
		
			#ifdef USE_PROSTONTALE		//=========== 포리스턴소스인경우
			bx = 32;
			by = 16;
			#else
			//GetObject(hSecession,sizeof(BITMAP),&bit);
			bx = 32;
			by = 16;
			#endif

			menuPos[0].left = BackStartPos.x + BUTTON2_X;		//추방
			menuPos[0].top = BackStartPos.y + BUTTON2_Y;
			menuPos[0].right = menuPos[0].left + BUTTONTEXT_WIDTH;
			menuPos[0].bottom = menuPos[0].top + BUTTONTEXT_HEIGHT;

			#ifdef USE_PROSTONTALE		//=========== 포리스턴소스인경우
			bx = 32;
			by = 16;
			#else
			bx = 32;
			by = 16;
			#endif

			menuPos[1].left = BackStartPos.x + BUTTON3_X;		//닫기
			menuPos[1].top = BackStartPos.y + BUTTON3_Y;
			menuPos[1].right = menuPos[1].left + BUTTONTEXT_WIDTH;
			menuPos[1].bottom = menuPos[1].top + BUTTONTEXT_HEIGHT;

			#ifdef USE_PROSTONTALE		//=========== 포리스턴소스인경우
			bx = 68;
			by = 27;
			#else
			bx = 68;
			by = 27;
			#endif

			menuPos[2].left = BackStartPos.x + BUTTON2_BOX_X;		//추방 테두리 박스
			menuPos[2].top = BackStartPos.y + BUTTON2_BOX_Y;
			menuPos[2].right = menuPos[2].left + BUTTONBOX_WIDTH;
			menuPos[2].bottom = menuPos[2].top + BUTTONBOX_HEIGHT;

			#ifdef USE_PROSTONTALE		//=========== 포리스턴소스인경우
			bx = 32;
			by = 16;
			#else
			bx = 32;
			by = 16;
			#endif

			menuPos[3].left = BackStartPos.x + BUTTON3_BOX_X;		//닫기 테두리 박스
			menuPos[3].top = BackStartPos.y + BUTTON3_BOX_Y;
			menuPos[3].right = menuPos[3].left + BUTTONBOX_WIDTH;
			menuPos[3].bottom = menuPos[3].top + BUTTONBOX_HEIGHT;		
			
			//================아이콘 설정				
			nIconCount = 3;
			//int	nClanWon[40];
			memset(nClanWon,0,sizeof(int)*40);
			
			for(i=0; i<nClanWon_Num; i++)
			
			{	
				if(i >=DISPLAY_MAX) break;
				nIconCount += 1;
				
				#ifdef USE_PROSTONTALE		//=========== 포리스턴소스인경우
				bx = 13;
				by = 17;
				#else
				bx = 13;
				by = 17;
				#endif

				menuPos[nIconCount].left = BackStartPos.x +  CLANWONLIST_X;		
				menuPos[nIconCount].top =  BackStartPos.y +   CLANWONLIST_Y +nAdd;//ICON_START + nAdd + REMOVE_ICON;
				menuPos[nIconCount].right = menuPos[nIconCount].left + 13;
				menuPos[nIconCount].bottom = menuPos[nIconCount].top + 17;
				nNext+=1;
				nAdd+=ICON_ADD;
				nClanWon[nIconCount] = 1;			
			}
			nIconCount = 11;

			//================스크롤 바
			if(nClanWon_Num > DISPLAY_MAX)
			
			{
				nIconCount+=1;
				#ifdef USE_PROSTONTALE		//=========== 포리스턴소스인경우
				bx = 8;
				by = 160;
				#else
				bx = 8;
				by = 160;
				#endif

				menuPos[nIconCount].left = BackStartPos.x + SCROLLBAR_LEFT;//SCROLLBAR_LEFT;
				menuPos[nIconCount].top =  BackStartPos.y + SCROOBAR_TOP;
				menuPos[nIconCount].right = menuPos[nIconCount].left + 8;
				menuPos[nIconCount].bottom = menuPos[nIconCount].top + 160;
				nClanWon[nIconCount] = 1;


				//ktj 스크롤관련
				tscr.Init(1);
				tscr.Init(	menuPos[nIconCount].top,//BackStartPos.y + SCROOBAR_TOP, 
						menuPos[nIconCount].top + 160,//256,
						nClanWon_Num, DISPLAY_MAX);						
				
				scrIconPos.left  = BackStartPos.x + SCROLLBAR_LEFT-5;//SCROLLBAR_LEFT-4;
				scrIconPos.top =  menuPos[nIconCount].top -8;//(BackStartPos.y + SCROOBAR_TOP)-8;
			}

			nIconCount+=1;
			#ifdef USE_PROSTONTALE		//=========== 포리스턴소스인경우
			bx = CLANTEXT_IMAGE_WIDTH;
			by = CLANTEXT_IMAGE_HEIGHT;
			#else
			
			bx = CLANTEXT_IMAGE_WIDTH;
			by = CLANTEXT_IMAGE_HEIGHT;
			#endif

			menuPos[nIconCount].left = BackStartPos.x + CLANTITLE_X;//CLANTEXT_IMAGE_X;		//텍스트 이미지 출력
			menuPos[nIconCount].top =  BackStartPos.y +   CLANTITLE_Y;//CLANTEXT_IMAGE_Y;
			menuPos[nIconCount].right = menuPos[nIconCount].left + bx;
			menuPos[nIconCount].bottom = menuPos[nIconCount].top + by;		
			

			break;
		case CLAN_REMOVE_SHORTCLANWON: //이 클랜원을 추방할경우 클랜원이 6명이하로 떨어진다고 메세지 출력
			menuSu = 5;
			
			#ifdef USE_PROSTONTALE		//=========== 포리스턴소스인경우
			bx = 32;
			by = 16;
			#else
			bx = 32;
			by = 16;
			#endif

			menuPos[0].left = BackStartPos.x +BUTTON2_X;		//확인
			menuPos[0].top = BackStartPos.y +BUTTON2_Y;
			menuPos[0].right = menuPos[0].left + bx;
			menuPos[0].bottom = menuPos[0].top + by;


		#ifdef USE_PROSTONTALE		//=========== 포리스턴소스인경우
			bx = 32;
			by = 16;
			#else
			
			bx = 32;
			by = 16;
			#endif

			menuPos[1].left = BackStartPos.x +BUTTON3_X;		//취소
			menuPos[1].top = BackStartPos.y +BUTTON3_Y;
			menuPos[1].right = menuPos[1].left+ bx;
			menuPos[1].bottom = menuPos[1].top + by;
			

			//텍스트 출력 위치
			if(bRemoveSecondCheck) //6명 이상일 경우
			{
				menuPos[2].left	= BackStartPos.x + clanRemoveSecond.pos.x;
				menuPos[2].top		= BackStartPos.y + clanRemoveSecond.pos.y - 20;
			}
			else
			{
				menuPos[2].left	= BackStartPos.x + clanszRemoveShortClanWonMsg.pos.x;
				menuPos[2].top		= BackStartPos.y + clanszRemoveShortClanWonMsg.pos.y;
			}
			menuPos[2].right	=menuPos[2].left + 190;
			menuPos[2].bottom	=menuPos[2].top +	180;

			//확인, 취소 테두리 박스
			#ifdef USE_PROSTONTALE		//=========== 포리스턴소스인경우
			bx = 68;
			by = 27;
			#else
			bx = 68;
			by = 27;
			#endif

			menuPos[3].left = BackStartPos.x + BUTTON2_BOX_X;		//확인 테두리 박스
			menuPos[3].top = BackStartPos.y + BUTTON2_BOX_Y;
			menuPos[3].right = menuPos[3].left + bx;
			menuPos[3].bottom = menuPos[3].top + by;

			#ifdef USE_PROSTONTALE		//=========== 포리스턴소스인경우
			bx = 68;
			by = 27;
			#else
			bx = 68;
			by = 27;
			#endif

			menuPos[4].left = BackStartPos.x + BUTTON3_BOX_X;		//취소 테두리 박스
			menuPos[4].top = BackStartPos.y + BUTTON3_BOX_Y;
			menuPos[4].right = menuPos[4].left + bx;
			menuPos[4].bottom = menuPos[4].top + by;
			
			break;
		case CLAN_REMOVE_SHORTCLANWON_RESULT:
			menuSu = 3;
			

			#ifdef USE_PROSTONTALE		//=========== 포리스턴소스인경우
			bx = 64;
			by = 64;
			#else
			bx = 64;
			by = 64;
			#endif
			menuPos[0].left = BackStartPos.x +BUTTON1_X;		//확인
			menuPos[0].top = BackStartPos.y +BUTTON1_Y;
			menuPos[0].right = menuPos[0].left + bx;
			menuPos[0].bottom = menuPos[0].top + by;

			//텍스트 출력 위치
			if(bRemoveSecondCheck) //6명 이상일 경우
			{
				menuPos[1].left	= BackStartPos.x + clanRemoveSecondResult.pos.x;
				menuPos[1].top		= BackStartPos.y + clanRemoveSecondResult.pos.y;
			}
			else
			{
				menuPos[1].left	= BackStartPos.x + clanRemoveShowrtClanWonResultMsg.pos.x;
				menuPos[1].top		= BackStartPos.y + clanRemoveShowrtClanWonResultMsg.pos.y;
			}
			
			menuPos[1].right	=menuPos[1].left + 190;
			menuPos[1].bottom	=menuPos[1].top +180;

			//확인 테두리
			#ifdef USE_PROSTONTALE		//=========== 포리스턴소스인경우
			bx = 68;
			by = 27;
			#else
			bx = 68;
			by = 27;
			#endif

			menuPos[2].left = BackStartPos.x + BUTTON1_BOX_X;		//확인 테두리 박스
			menuPos[2].top = BackStartPos.y + BUTTON1_BOX_Y;
			menuPos[2].right = menuPos[2].left + bx;
			menuPos[2].bottom = menuPos[2].top + by;
			break;
		case CLAN_REMOVE_RESULT: //클랜원 추방 결과 보여주기
			menuSu = 3;
			

			#ifdef USE_PROSTONTALE		//=========== 포리스턴소스인경우
			bx = 64;
			by = 64;
			#else
			bx = 64;
			by = 64;
			#endif
			menuPos[0].left = BackStartPos.x +BUTTON1_X;		//확인
			menuPos[0].top = BackStartPos.y +BUTTON1_Y;
			menuPos[0].right = menuPos[0].left + bx;
			menuPos[0].bottom = menuPos[0].top + by;

			//텍스트 출력 위치
			menuPos[1].left	= BackStartPos.x + clanReMoveHeaderMsg.pos.x;
			menuPos[1].top		= BackStartPos.y + clanReMoveHeaderMsg.pos.y - 20;
			menuPos[1].right	=menuPos[1].left + 190;
			menuPos[1].bottom	=menuPos[1].top +180;

			//확인 테두리
			#ifdef USE_PROSTONTALE		//=========== 포리스턴소스인경우
			bx = 68;
			by = 27;
			#else
			bx = 68;
			by = 27;
			#endif

			menuPos[2].left = BackStartPos.x + BUTTON1_BOX_X;		//확인 테두리 박스
			menuPos[2].top = BackStartPos.y + BUTTON1_BOX_Y;
			menuPos[2].right = menuPos[2].left + bx;
			menuPos[2].bottom = menuPos[2].top + by;
			break;
		case CLANCHIP_RELEASEMESSAGE:
			menuSu = 3;
			

			#ifdef USE_PROSTONTALE		//=========== 포리스턴소스인경우
			bx = 64;
			by = 64;
			#else
			bx = 64;
			by = 64;
			#endif
			menuPos[0].left = BackStartPos.x +BUTTON1_X;		//확인
			menuPos[0].top = BackStartPos.y +BUTTON1_Y;
			menuPos[0].right = menuPos[0].left + bx;
			menuPos[0].bottom = menuPos[0].top + by;

			//텍스트 출력 위치
			menuPos[1].left	= BackStartPos.x + clanClanUserRelease.pos.x;
			menuPos[1].top		= BackStartPos.y + clanClanUserRelease.pos.y;
			menuPos[1].right	=menuPos[1].left + 190;
			menuPos[1].bottom	=menuPos[1].top +180;

			//확인 테두리
			#ifdef USE_PROSTONTALE		//=========== 포리스턴소스인경우
			bx = 68;
			by = 27;
			#else
			bx = 68;
			by = 27;
			#endif

			menuPos[2].left = BackStartPos.x + BUTTON1_BOX_X;		//확인 테두리 박스
			menuPos[2].top = BackStartPos.y + BUTTON1_BOX_Y;
			menuPos[2].right = menuPos[2].left + bx;
			menuPos[2].bottom = menuPos[2].top + by;
			break;
		case CLAN_WAREHOUSE_RIGHT:
			menuSu = 6;
			
			#ifdef USE_PROSTONTALE		//=========== 포리스턴소스인경우
			
			bx = 13;
			by = 17;
			#else
			bx = 13;
			by = 17;
			#endif

			menuPos[0].left = BackStartPos.x +  70;		// 아이콘
			menuPos[0].top =  BackStartPos.y +  50 +WHR_ICON;
			menuPos[0].right = menuPos[0].left + bx;
			menuPos[0].bottom = menuPos[0].top + by;


			#ifdef USE_PROSTONTALE		//=========== 포리스턴소스인경우
			bx = 13;
			by = 17;
			#else
			bx = 13;
			by = 17;
			#endif

			menuPos[1].left = BackStartPos.x +  70;		//아이콘
			menuPos[1].top = BackStartPos.y +  80+WHR_ICON;
			menuPos[1].right = menuPos[1].left + bx;
			menuPos[1].bottom = menuPos[1].top + by;

			#ifdef USE_PROSTONTALE		//=========== 포리스턴소스인경우
			bx = 13;
			by = 17;
			#else
			bx = 13;
			by = 17;
			#endif

			menuPos[2].left = BackStartPos.x + 70;		//아이콘
			menuPos[2].top =  BackStartPos.y + 110+WHR_ICON;
			menuPos[2].right = menuPos[2].left + bx;
			menuPos[2].bottom = menuPos[2].top + by;

			#ifdef USE_PROSTONTALE		//=========== 포리스턴소스인경우
			bx = 13;
			by = 17;
			#else
			bx = 13;
			by = 17;
			#endif

			menuPos[3].left = BackStartPos.x +   70;		//아이콘
			menuPos[3].top = BackStartPos.y +   140+WHR_ICON;
			menuPos[3].right = menuPos[3].left + bx;
			menuPos[3].bottom = menuPos[3].top + by;


			#ifdef USE_PROSTONTALE		//=========== 포리스턴소스인경우
			bx = 13;
			by = 17;
			#else
			bx = 13;
			by = 17;
			#endif

			menuPos[4].left = BackStartPos.x +  70;		//아이콘
			menuPos[4].top = BackStartPos.y +   170+WHR_ICON;
			menuPos[4].right = menuPos[4].left + bx;
			menuPos[4].bottom = menuPos[4].top + by;


			#ifdef USE_PROSTONTALE		//=========== 포리스턴소스인경우
			bx = 32;
			by = 16;
			#else
			bx = 32;
			by = 16;
			#endif

			menuPos[5].left = BackStartPos.x + BUTTON3_X;		//확인
			menuPos[5].top = BackStartPos.y +  BUTTON3_Y;
			menuPos[5].right = menuPos[5].left + bx;
			menuPos[5].bottom = menuPos[5].top + by;

			//텍스트 출력 위치
			menuPos[6].left	= menuPos[0].left+ bx + XCAP;
			menuPos[6].top		= menuPos[0].top+YDOWN;

			menuPos[7].left	= menuPos[1].left+ bx + XCAP;
			menuPos[7].top		= menuPos[1].top+YDOWN;

			menuPos[8].left	= menuPos[2].left+ bx + XCAP;
			menuPos[8].top		= menuPos[2].top+YDOWN;

			menuPos[9].left	= menuPos[3].left+ bx + XCAP;
			menuPos[9].top		= menuPos[3].top+YDOWN;

			menuPos[10].left	= menuPos[4].left+ bx + XCAP;
			menuPos[10].top	= menuPos[4].top+YDOWN;

			#ifdef USE_PROSTONTALE		//=========== 포리스턴소스인경우
			bx = 64;
			by = 16;
			#else
			
			bx = 64;
			by = 16;
			#endif

			menuPos[11].left = BackStartPos.x + BUTTON3_BOX_X;		//확인 테두리 박스
			menuPos[11].top = BackStartPos.y +  BUTTON3_BOX_Y;
			menuPos[11].right = menuPos[11].left + bx;
			menuPos[11].bottom = menuPos[11].top + by;

			break;





		case CLAN_MARK1: // 클라이언트, 서버에 클랜 마크가 업음
			menuSu = 1;
			
			#ifdef USE_PROSTONTALE		//=========== 포리스턴소스인경우
			bx = 32;
			by = 16;
			#else
			bx = 32;
			by = 16;
			#endif
			menuPos[0].left = BackStartPos.x +BUTTON3_X;		//확인
			menuPos[0].top = BackStartPos.y +BUTTON3_Y;
			menuPos[0].right = menuPos[0].left + bx;
			menuPos[0].bottom = menuPos[0].top + by;

			//텍스트 출력 위치
			menuPos[1].left	= BackStartPos.x + 60;
			menuPos[1].top		= BackStartPos.y + 100;
			menuPos[1].right	=menuPos[1].left + 260;
			menuPos[1].bottom	=menuPos[1].top + 180;

			#ifdef USE_PROSTONTALE		//=========== 포리스턴소스인경우
			bx = 64;
			by = 64;
			#else
			bx = 64;
			by = 64;
			#endif
			menuPos[2].left = BackStartPos.x +BUTTON3_BOX_X;		//확인 테두리 박스
			menuPos[2].top = BackStartPos.y +BUTTON3_BOX_Y;
			menuPos[2].right = menuPos[2].left + bx;
			menuPos[2].bottom = menuPos[2].top + by;


			#ifdef USE_PROSTONTALE		//=========== 포리스턴소스인경우
			bx = CLANTEXT_IMAGE_WIDTH;
			by = CLANTEXT_IMAGE_HEIGHT;
			#else
			
			bx = CLANTEXT_IMAGE_WIDTH;
			by = CLANTEXT_IMAGE_HEIGHT;
			#endif

			menuPos[3].left = BackStartPos.x +  CLANTEXT_IMAGE_X;		//텍스트 이미지 출력
			menuPos[3].top =  BackStartPos.y +   CLANTEXT_IMAGE_Y;
			menuPos[3].right = menuPos[3].left+ bx;
			menuPos[3].bottom = menuPos[3].top + by;
			break;
		case CLAN_MARK2: //클랜마크가 클라이언트에만 있음
			menuSu = 2;		
			
			#ifdef USE_PROSTONTALE		//=========== 포리스턴소스인경우
			bx = 32;
			by = 16;
			#else
			bx = 32;
			by = 16;
			#endif

			menuPos[0].left = BackStartPos.x + BUTTON1_X;		//yes
			menuPos[0].top = BackStartPos.y + BUTTON1_Y;
			menuPos[0].right = menuPos[0].left + bx;
			menuPos[0].bottom = menuPos[0].top + by;


			#ifdef USE_PROSTONTALE		//=========== 포리스턴소스인경우
			bx = 32;
			by = 16;
			#else
			bx = 32;
			by = 16;
			#endif

			menuPos[1].left = BackStartPos.x +BUTTON2_X;		//no
			menuPos[1].top = BackStartPos.y +BUTTON2_Y;
			menuPos[1].right = menuPos[1].left+ bx;
			menuPos[1].bottom = menuPos[1].top + by;

			#ifdef USE_PROSTONTALE		//=========== 포리스턴소스인경우
			bx = 64;
			by = 64;
			#else
			bx = 64;
			by = 64;
			#endif

			menuPos[2].left = BackStartPos.x +150;		//이미지 디스플레이
			menuPos[2].top = BackStartPos.y +150;
			menuPos[2].right = menuPos[2].left+ 15;
			menuPos[2].bottom = menuPos[2].top + 15;



			//텍스트 출력 위치
			menuPos[3].left	= BackStartPos.x + 60;
			menuPos[3].top		= BackStartPos.y + 100;
			menuPos[3].right	=menuPos[3].left + 260;
			menuPos[3].bottom	=menuPos[3].top + 180;

			//yes,no 테두리 박스
			#ifdef USE_PROSTONTALE		//=========== 포리스턴소스인경우
			bx = 64;
			by = 64;
			#else
			bx = 64;
			by = 64;
			#endif

			menuPos[4].left = BackStartPos.x +BUTTON1_BOX_X;	//YES 테두리 박스
			menuPos[4].top = BackStartPos.y +BUTTON1_BOX_Y;
			menuPos[4].right = menuPos[4].left+ 15;
			menuPos[4].bottom = menuPos[4].top + 15;

			#ifdef USE_PROSTONTALE		//=========== 포리스턴소스인경우
			bx = 64;
			by = 64;
			#else
			bx = 64;
			by = 64;
			#endif

			menuPos[5].left = BackStartPos.x +BUTTON2_BOX_X;		//NO 테두리 박스
			menuPos[5].top = BackStartPos.y +BUTTON2_BOX_Y;
			menuPos[5].right = menuPos[5].left+ 15;
			menuPos[5].bottom = menuPos[5].top + 15;

			#ifdef USE_PROSTONTALE		//=========== 포리스턴소스인경우
			bx = CLANTEXT_IMAGE_WIDTH;
			by = CLANTEXT_IMAGE_HEIGHT;
			#else

			bx = CLANTEXT_IMAGE_WIDTH;
			by = CLANTEXT_IMAGE_HEIGHT;
			#endif

			menuPos[6].left = BackStartPos.x +  CLANTEXT_IMAGE_X;		//텍스트 이미지 출력
			menuPos[6].top =  BackStartPos.y +   CLANTEXT_IMAGE_Y;
			menuPos[6].right = menuPos[6].left+ bx;
			menuPos[6].bottom = menuPos[6].top + by;

			break;
		case CLAN_MARK3: //클랜 마크가 클라이언트,서버에 다 존재함
			menuSu = 2;		


			#ifdef USE_PROSTONTALE		//=========== 포리스턴소스인경우
			bx = 64;
			by = 64;
			#else
			bx = 64;
			by = 64;
			#endif

			menuPos[0].left = BackStartPos.x + BUTTON1_X;		//yes
			menuPos[0].top = BackStartPos.y + BUTTON1_Y;
			menuPos[0].right = menuPos[0].left + bx;
			menuPos[0].bottom = menuPos[0].top + by;


			#ifdef USE_PROSTONTALE		//=========== 포리스턴소스인경우
			bx = 64;
			by = 64;
			#else
			bx = 64;
			by = 64;
			#endif

			menuPos[1].left = BackStartPos.x +BUTTON2_X;		//no
			menuPos[1].top = BackStartPos.y +BUTTON2_Y;
			menuPos[1].right = menuPos[1].left+ bx;
			menuPos[1].bottom = menuPos[1].top + by;

			#ifdef USE_PROSTONTALE		//=========== 포리스턴소스인경우
			bx = 64;
			by = 64;
			#else
			bx = 64;
			by = 64;
			#endif

			menuPos[2].left = BackStartPos.x +100;		//이미지 디스플레이
			menuPos[2].top = BackStartPos.y +150;
			menuPos[2].right = menuPos[2].left+ 15;
			menuPos[2].bottom = menuPos[2].top + 15;

			#ifdef USE_PROSTONTALE		//=========== 포리스턴소스인경우
			bx = 64;
			by = 64;
			#else
			bx = 64;
			by = 64;
			#endif

			menuPos[3].left = BackStartPos.x +200;		//이미지 디스플레이
			menuPos[3].top = BackStartPos.y +150;
			menuPos[3].right = menuPos[3].left+ 15;
			menuPos[3].bottom = menuPos[3].top + 15;

			//텍스트 출력 위치
			menuPos[4].left	= BackStartPos.x + 60;
			menuPos[4].top		= BackStartPos.y + 100;
			menuPos[4].right	=menuPos[4].left + 260;
			menuPos[4].bottom	=menuPos[4].top + 180;

			//yes, no 테두리 박스
			#ifdef USE_PROSTONTALE		//=========== 포리스턴소스인경우
			bx = 64;
			by = 64;
			#else
			bx = 64;
			by = 64;
			#endif

			menuPos[5].left = BackStartPos.x +BUTTON1_BOX_X;	//YES 테두리 박스
			menuPos[5].top = BackStartPos.y +BUTTON1_BOX_Y;
			menuPos[5].right = menuPos[5].left+ 15;
			menuPos[5].bottom = menuPos[5].top + 15;

			#ifdef USE_PROSTONTALE		//=========== 포리스턴소스인경우
			bx = 64;
			by = 64;
			#else
			bx = 64;
			by = 64;
			#endif

			menuPos[6].left = BackStartPos.x +BUTTON2_BOX_X;		//NO 테두리 박스
			menuPos[6].top = BackStartPos.y +BUTTON2_BOX_Y;
			menuPos[6].right = menuPos[6].left+ 15;
			menuPos[6].bottom = menuPos[6].top + 15;

			#ifdef USE_PROSTONTALE		//=========== 포리스턴소스인경우
			bx = CLANTEXT_IMAGE_WIDTH;
			by = CLANTEXT_IMAGE_HEIGHT;
			#else
			
			bx = CLANTEXT_IMAGE_WIDTH;
			by = CLANTEXT_IMAGE_HEIGHT;
			#endif

			menuPos[7].left = BackStartPos.x +  CLANTEXT_IMAGE_X;		//텍스트 이미지 출력
			menuPos[7].top =  BackStartPos.y +   CLANTEXT_IMAGE_Y;
			menuPos[7].right = menuPos[7].left+ bx;
			menuPos[7].bottom = menuPos[7].top + by;
			break;


		case CLAN_RELEASE:
			menuSu = 2;
			
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

			//텍스트 출력 위치
			menuPos[2].left = BackStartPos.x + clanMsg15.pos.x;
			menuPos[2].top = BackStartPos.y + clanMsg15.pos.y;
			menuPos[2].right = menuPos[2].left + 280;
			menuPos[2].bottom = menuPos[2].top + 250;

			//yes,no 테두리 박스
			#ifdef USE_PROSTONTALE		//=========== 포리스턴소스인경우
			bx = 64;
			by = 16;
			#else
			bx = 64;
			by = 16;
			#endif

			menuPos[3].left = BackStartPos.x +  BUTTON2_BOX_X;		//YES 테두리 박스
			menuPos[3].top =  BackStartPos.y +   BUTTON2_BOX_Y;
			menuPos[3].right = menuPos[3].left+ bx;
			menuPos[3].bottom = menuPos[3].top + by;

			#ifdef USE_PROSTONTALE		//=========== 포리스턴소스인경우
			bx = 64;
			by = 16;
			#else
			bx = 64;
			by = 16;
			#endif

			menuPos[4].left = BackStartPos.x +  BUTTON3_BOX_X;		//NO 테두리 박스
			menuPos[4].top =  BackStartPos.y +   BUTTON3_BOX_Y;
			menuPos[4].right = menuPos[4].left+ bx;
			menuPos[4].bottom = menuPos[4].top + by;

			#ifdef USE_PROSTONTALE		//=========== 포리스턴소스인경우
			bx = CLANTEXT_IMAGE_WIDTH;
			by = CLANTEXT_IMAGE_HEIGHT;
			#else
			
			bx = CLANTEXT_IMAGE_WIDTH;
			by = CLANTEXT_IMAGE_HEIGHT;
			#endif

			menuPos[5].left = BackStartPos.x +  CLANTITLE_X;		//텍스트 이미지 출력
			menuPos[5].top =  BackStartPos.y +   CLANTITLE_Y;
			menuPos[5].right = menuPos[5].left+ bx;
			menuPos[5].bottom = menuPos[5].top + by;

			break;
		case CLAN_RECONF_RELEASE:
			menuSu = 6;
			
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

			//텍스트 출력 위치
			menuPos[2].left = BackStartPos.x + CLAN_RECONF_RELEASE_X;
			menuPos[2].top = BackStartPos.y + CLAN_RECONF_RELEASE_Y;
			menuPos[2].right = 0;//menuPos[2].left + 280;
			menuPos[2].bottom = 0;//menuPos[2].top + 250;

			//yes,no 테두리 박스
			#ifdef USE_PROSTONTALE		//=========== 포리스턴소스인경우
			bx = 64;
			by = 16;
			#else
			bx = 64;
			by = 16;
			#endif

			menuPos[3].left = BackStartPos.x +  BUTTON2_BOX_X;		//YES 테두리 박스
			menuPos[3].top =  BackStartPos.y +   BUTTON2_BOX_Y;
			menuPos[3].right = menuPos[3].left+ bx;
			menuPos[3].bottom = menuPos[3].top + by;

			#ifdef USE_PROSTONTALE		//=========== 포리스턴소스인경우
			bx = 64;
			by = 16;
			#else
			bx = 64;
			by = 16;
			#endif

			menuPos[4].left = BackStartPos.x +  BUTTON3_BOX_X;		//NO 테두리 박스
			menuPos[4].top =  BackStartPos.y +   BUTTON3_BOX_Y;
			menuPos[4].right = menuPos[4].left+ bx;
			menuPos[4].bottom = menuPos[4].top + by;

			#ifdef USE_PROSTONTALE		//=========== 포리스턴소스인경우
			bx = CLANTEXT_IMAGE_WIDTH;
			by = CLANTEXT_IMAGE_HEIGHT;
			#else
			
			bx = CLANTEXT_IMAGE_WIDTH;
			by = CLANTEXT_IMAGE_HEIGHT;
			#endif

			menuPos[5].left = BackStartPos.x +  CLANTITLE_X;		//텍스트 이미지 출력
			menuPos[5].top =  BackStartPos.y +   CLANTITLE_Y;
			menuPos[5].right = menuPos[5].left+ bx;
			menuPos[5].bottom = menuPos[5].top + by;

			

			break;
		case CLAN_FINISH_RELEASE:
			menuSu = 4;
			
			#ifdef USE_PROSTONTALE		//=========== 포리스턴소스인경우
			bx = 32;
			by = 16;
			#else
			bx = 32;
			by = 16;
			#endif

			menuPos[0].left = BackStartPos.x +  BUTTON1_X;		//확인
			menuPos[0].top =  BackStartPos.y +   BUTTON1_Y;
			menuPos[0].right = menuPos[0].left + bx;
			menuPos[0].bottom = menuPos[0].top + by;

			#ifdef USE_PROSTONTALE		//=========== 포리스턴소스인경우
			bx = 64;
			by = 16;
			#else
			bx = 64;
			by = 16;
			#endif

			menuPos[1].left = BackStartPos.x +  BUTTON1_BOX_X;		//YES 테두리 박스
			menuPos[1].top =  BackStartPos.y +   BUTTON1_BOX_Y;
			menuPos[1].right = menuPos[1].left+ bx;
			menuPos[1].bottom = menuPos[1].top + by;

			#ifdef USE_PROSTONTALE		//=========== 포리스턴소스인경우
			bx = CLANTEXT_IMAGE_WIDTH;
			by = CLANTEXT_IMAGE_HEIGHT;
			#else

			bx = CLANTEXT_IMAGE_WIDTH;
			by = CLANTEXT_IMAGE_HEIGHT;
			#endif

			menuPos[2].left = BackStartPos.x +  CLANTITLE_X;		//텍스트 이미지 출력
			menuPos[2].top =  BackStartPos.y +   CLANTITLE_Y;
			menuPos[2].right = menuPos[2].left+ bx;
			menuPos[2].bottom = menuPos[2].top + by;

			//텍스트 출력 위치
			menuPos[3].left = BackStartPos.x + clanFinishReleaseMsg.pos.x;
			menuPos[3].top = BackStartPos.y + clanFinishReleaseMsg.pos.y;
			menuPos[3].right = 0;
			menuPos[3].bottom = 0;

			break;	
		case CLAN_HELP:
			menuSu = 2 + CLANHELPICON_DISPLAY_MAX+3;
			
			
			nScl_Bar = 0;
			//g_HelpIconNumber = 0;
			//clanBase->SetClanHelpIconNum(0);
			

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
				menuPos[nIconPos].top =  BackStartPos.y + CLANHELPLIST_Y + nAdd;//BackStartPos.y +  CLANHELPICON_START_Y+nAdd+nWidth;
				menuPos[nIconPos].right = menuPos[nIconPos].left + CLANHELPICON_WIDTH;
				menuPos[nIconPos].bottom = menuPos[nIconPos].top + CLANHELPICON_HEIGHT;
				nIconPos+=1;
				nAdd += CLANHELPICON_CAP;
				//nWidth += CLANHELPICON_HEIGHT;
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
				
				scrIconPos.left  = BackStartPos.x + SCROLLBAR_LEFT-5;//SCROLLBAR_LEFT-4;
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
			menuPos[nIconPos].left = BackStartPos.x +  CLANTITLE_X;	
			menuPos[nIconPos].top =	 BackStartPos.y +   CLANTITLE_Y;
			menuPos[nIconPos].right = menuPos[nIconPos].left + bx;
			menuPos[nIconPos].bottom = menuPos[nIconPos].top + by;

			
			break;
		case CLAN_HELP_CONTENT: //헬프에서 첫번째 지문
			menuSu = 1+HLEPDISPLAY_MAX+5;
			
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
			menuPos[2].top =  BackStartPos.y + CLANTITLE_Y;
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

			#ifdef USE_PROSTONTALE		//=========== 포리스턴소스인경우
			bx = 30;
			by = 30;
			#else

			bx = 30;
			by = 30;
			#endif
			
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

				menuPos[nIconPos].left = BackStartPos.x + SCROLLBAR_LEFT;
				menuPos[nIconPos].top =  BackStartPos.y + SCROOBAR_TOP;
				menuPos[nIconPos].right = menuPos[nIconPos].left + 8;
				menuPos[nIconPos].bottom = menuPos[nIconPos].top + 160;

				//ktj 스크롤관련
				tscr.Init(1);
				tscr.Init(	menuPos[nIconPos].top,//BackStartPos.y + SCROOBAR_TOP, 
						menuPos[nIconPos].top + 160,//256,
						g_HelpDisPlay, HLEPDISPLAY_MAX);
						
				scrIconPos.left  = BackStartPos.x + SCROLLBAR_LEFT-5;//SCROLLBAR_LEFT-4;
				scrIconPos.top =  menuPos[nIconPos].top  - 8;
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

				menuPos[nIconPos].left = BackStartPos.x +  HELPCONTENT_X;		
				menuPos[nIconPos].top =  BackStartPos.y +  HELPCONTENT_Y + nAdd;//BackStartPos.y +  HELPCONTENT_Y+nAdd;
				menuPos[nIconPos].right = 0;//menuPos[nIconPos].left + CLANHELPICON_WIDTH;
				menuPos[nIconPos].bottom = 0;//menuPos[nIconPos].top + CLANHELPICON_HEIGHT;
				nIconPos+=1;
				nAdd += HELPCONTENT_CAP;
			}
		
			

			break;		
		case CLANCHIP_MAKE_FIRSTMESSAGE: 			
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
			menuPos[1].left = BackStartPos.x + clanMakeFinishClan.pos.x;
			menuPos[1].top = BackStartPos.y + clanMakeFinishClan.pos.y;

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
		case CLANCHIP_DOWN6_CLANWON:    
		case CLANCHIP_DOWN6_CLANWON_MESSAGE:
		case CLAN_NOMAKE:
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
			menuPos[1].left = BackStartPos.x + clanMsg6.pos.x;
			menuPos[1].top = BackStartPos.y + clanMsg6.pos.y;

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
		case CLANCHIP_SUBCHIP_LOAD_DATA:
		case CLANCHIP_SUBCHIP_READMSG_FLAG:
		case	CLANCHIP_APPOINTSUBCHIP_DO:	//클랜부칩 임명 ASP호출
		case CLANCHIP_RELEASESUBCHIP_DO:	//클랜부칩 해임 ASP호출
		case CLANCHIP_LEAVECLAN_PFLAG:
		case CLANCHIP_LEAVECLAN_DO:
		case ISPFLAG_CHIP:
		case CLAN_REMOVE_SHORTCLANWON_LOADING:
		case CLAN_REMOVE_LOADING:
		case CLAN_RELEASE_LOADING:	
		case CLAN_CHIP_ReadDATA:

			//텍스트 출력 위치
			menuPos[0].left = BackStartPos.x + clanLoading.pos.x;
			menuPos[0].top = BackStartPos.y + clanLoading.pos.y;
			menuPos[0].right = 0;
			menuPos[0].bottom = 0;
			
			break;
	}
}



//=============클랜마크관련 함수 ===============================================================start


int CLANEntity_chip::chkMark(char *clanName)
{
	nChkMark = -1;
	if(!SearchMarkClient(clanName))
	{
		if(!SearchMarkServer(clanName))
		{
			nChkMark = 0;
		}
	}
	else if(SearchMarkClient(clanName))
	{
		if(!SearchMarkServer(clanName))
		{
			nChkMark = 1;
		}
		else
		{
			nChkMark = 2;
		}
	}

	return nChkMark;
}

BOOL CLANEntity_chip::CheckMark(char* filename)
{
	WIN32_FIND_DATA wfd;	
	HANDLE hFind;
	char szTemp[50];
	memset(szTemp,0,sizeof(char)*50);
		
	wsprintf(szTemp,"image\\clanDATA\\%s.bmp",filename);
	hFind = FindFirstFile(szTemp,&wfd);
	
	if(hFind == INVALID_HANDLE_VALUE )
	{
		return FALSE;
	}
	return TRUE;
}

BOOL CLANEntity_chip::SearchMarkClient(char *clanName)
{
	WIN32_FIND_DATA wfd;	
	HANDLE hFind;
	char szTemp[50];
	memset(szTemp,0,sizeof(char)*50);	
	
	wsprintf(szTemp,"image\\clanDATA\\%s.bmp",clanName);
	hFind = FindFirstFile(szTemp,&wfd);
	
	if(hFind == INVALID_HANDLE_VALUE )
	{
		return FALSE;
	}

	#ifdef USE_PROSTONTALE
	hClinetMark =  LoadDibSurfaceOffscreen(szTemp);
	#else
	hClinetMark =  (HBITMAP)LoadImage(NULL,szTemp,
						IMAGE_BITMAP,
						0,0,
						LR_LOADFROMFILE);

	#endif

	FindClose(hFind);
	return TRUE;
}

BOOL CLANEntity_chip::SearchMarkServer(char *clanName)
{
	WIN32_FIND_DATA wfd;	
	HANDLE hFind;	
	char szTemp[50];
	memset(szTemp,0,sizeof(char)*50);	
	
	wsprintf(szTemp,"image\\clanDATA\\%s.bmp",clanName);
	hFind = FindFirstFile(szTemp,&wfd);
	
	if(hFind == INVALID_HANDLE_VALUE )
	{
		return FALSE;
	}		
	#ifdef USE_PROSTONTALE
	hServerMark =  LoadDibSurfaceOffscreen(szTemp);
	#else
	hServerMark =  (HBITMAP)LoadImage(NULL,szTemp,
						IMAGE_BITMAP,
						0,0,
						LR_LOADFROMFILE);
	#endif

	FindClose(hFind);
	return TRUE;
}


//=============클랜마크관련 함수 =============================================================== end

void CLANEntity_chip::Draw()
{
	int i =0,nIconCount=0;
	int nIconPos = 0;
	char szClanWon[30];
	int nSaveTop = 0,nAdd = 0;
	int nStrLen = 0;
	
	int bx,by;
	char szTemp[256];
	//ZeroMemory(szDrawMenuMsg,sizeof(char)*2024);

#ifdef USE_PROSTONTALE		//=========== 포리스턴소스인경우
	char szHelpContent[HELPWIDTH];
#else
	HDC hdc;
	HDC sorDC;
	BITMAP bit;
	
	
	RECT rect;	
	int j =0;
	char szHelpContent[HELPWIDTH];
//	HFONT font, oldfont;

	

#endif
	char string[500];
#ifdef USE_PROSTONTALE		//=========== 포리스턴소스인경우
	//void DrawSprite(int winX ,int winY,LPDIRECTDRAWSURFACE4 pdds,int x, int y, int width , int height,int Bltflag=1); //컬러키는 현재 무조건 빼준다
	
	switch(C_menuN) {			

	//===================부 클랜칩 임명및 해임
	case CLANCHIP_SUBCHIP_APPOINTMSG://클랜부침이 임명되다고 한번 보여줌
		DrawSprite(menuPos[2].left,menuPos[2].top, hButton_Box,  0, 0, 48, 23, 1);		
		if(menuMouseOver[0]>0) DrawSprite(menuPos[0].left,menuPos[0].top, hOk_Yellow,  0, 0, 32, 16, 1);
		else	DrawSprite(menuPos[0].left,menuPos[0].top, hOk_Gray,  0, 0, 32, 16, 1);
		
		if(cldata.SubChip[0] ==0)
		{
			wsprintf(szDrawMenuMsg,SubChipAppointEnd.szMsg,cldata.OldSubChip);
		}
		else
		{
			wsprintf(szDrawMenuMsg,SubChipAppointEnd.szMsg,cldata.SubChip);
		}
		Draw_C_Text(szDrawMenuMsg, menuPos[1].left, menuPos[1].top);				
		break;
	case CLANCHIP_SUBCHIP_RELEASEMSG://클랜부침이 해임되다고 한번 보여줌
		DrawSprite(menuPos[2].left,menuPos[2].top, hButton_Box,  0, 0, 48, 23, 1);		
		if(menuMouseOver[0]>0) DrawSprite(menuPos[0].left,menuPos[0].top, hOk_Yellow,  0, 0, 32, 16, 1);
		else	DrawSprite(menuPos[0].left,menuPos[0].top, hOk_Gray,  0, 0, 32, 16, 1);
		
		wsprintf(szDrawMenuMsg,SubChipReleaseEnd.szMsg,cldata.OldSubChip);
		Draw_C_Text(szDrawMenuMsg, menuPos[1].left, menuPos[1].top);		
		break;
		
	case CLANCHIP_SUBCHIP_LIST:			//클랜원들 리스트 보여줌 부 클랜칩이 있으면 따로 보여줌
		nIconCount = 11;
		if(nClanWon_Num>DISPLAY_MAX)
		{
			nIconCount+=1;
			/*
			DrawSprite(menuPos[nIconCount].left,menuPos[nIconCount].top, hScl_Bar,  0, 0, 8, 256, 1);
			//ktj : 스크롤 아이콘 그리기
			DrawSprite(scrIconPos.left, scrIconPos.top, hScl_icon,  0, 0, 16, 16, 1);			
			*/


			//스크롤바 bmp그림
			DrawSprite(menuPos[nIconCount].left, menuPos[nIconCount].top, hScl_Bar,   
				0, 0, 8, 160, 1);	

			lpD3DDevice->BeginScene();
			//sinDrawTexImage(hScl_Bar_tj,  (float)menuPos[nIconCount].left, (float)menuPos[nIconCount].top, 8, 256);


			//스크롤 위치표시 아이콘
			sinDrawTexImage( hScl_icon_tj , (float)scrIconPos.left, (float)scrIconPos.top, 16, 16);
			lpD3DDevice->EndScene();

		}
		nIconCount+=1;
		DrawSprite(menuPos[nIconCount].left,menuPos[nIconCount].top, hTxt_SubChip[0],  0, 0, CLANTEXT_IMAGE_WIDTH, CLANTEXT_IMAGE_HEIGHT, 1);
		nIconCount+=1;

		lpD3DDevice->BeginScene();
		sinDrawTexImage(hLine,(float)menuPos[nIconCount].left,(float)menuPos[nIconCount].top,
							(float)240, 10.0f,
							0.0f,0.0f,0.0f,0.0f,0.0f,0.0f,255);    
		lpD3DDevice->EndScene();	

		//DrawSprite(menuPos[nIconCount].left - 31,menuPos[nIconCount].top, hRemove[1],   0, 0, 30, 30, 1);	
		//DrawSprite(menuPos[nIconCount].right + 1,menuPos[nIconCount].top, hRemove[1],   0, 0, 30, 30, 1);	

		//extern int banisF[100];
		nIconCount = 3;
		for(i=0; i<DISPLAY_MAX; i++ )
		{
			if(i == nClanWon_Num) break;
			nIconCount+=1;
			if(banisF[nScl_Bar + i] ) {
				DrawSprite(menuPos[nIconCount].left,menuPos[nIconCount].top, hIcon_Click,  0, 0, 13, 17, 1);					
			}
			else {
				DrawSprite(menuPos[nIconCount].left,menuPos[nIconCount].top, hIcon,  0, 0, 13, 17, 1);									
			}
		}
		
				
		DrawSprite(menuPos[2].left,menuPos[2].top, hButton_Box,  0, 0, 48, 23, 1);
		DrawSprite(menuPos[3].left,menuPos[3].top, hButton_Box,  0, 0, 48, 23, 1);
		if(menuMouseOver[0] >0) DrawSprite(menuPos[0].left,menuPos[0].top, hOk_Yellow,  0, 0, 32, 16, 1);	
		else DrawSprite(menuPos[0].left,menuPos[0].top, hOk_Gray,  0, 0, 32, 16, 1);	
		if(menuMouseOver[1] >0) DrawSprite(menuPos[1].left,menuPos[1].top, hCancel_Yellow,  0, 0, 32, 16, 1);	
		else DrawSprite(menuPos[1].left,menuPos[1].top, hCancel_Gray,  0, 0, 32, 16, 1);
		//nIconCount=3;

		nIconCount=15; //클랜원 글자 위치 시작지점
		nAdd = 0;
		for(i=0; i<nClanWon_Num; i++)
		{	
			if(i >=DISPLAY_MAX) break;			
			
			#ifdef USE_PROSTONTALE		//=========== 포리스턴소스인경우
			bx = 13;
			by = 17;
			#else
			GetObject(hIcon,sizeof(BITMAP),&bit);
			bx = bit.bmWidth;
			by = bit.bmHeight;
			#endif

			menuPos[nIconCount].left = BackStartPos.x +  CLANWONLIST_X+30;		
			menuPos[nIconCount].top =  BackStartPos.y +   CLANWONLIST_Y +nAdd;//ICON_START + nAdd + REMOVE_ICON;
			menuPos[nIconCount].right =  (BackStartPos.x +  120) + cldata.clanWonStrLen[i+nScl_Bar];
			menuPos[nIconCount].bottom = menuPos[nIconCount].top + 17;
		//	nNext+=1;
			nAdd+=ICON_ADD;
			nClanWon[nIconCount] = 1;
			nIconCount += 1;
		}

		nIconCount=15;
		for(i=0; i < DISPLAY_MAX; i++)
		{
			
			if(nClanWon[nIconCount])
			{	
				if(strcmp(cldata.clanWon[i+nScl_Bar],cldata.SubChip) == 0)
				{
					strcpy(szClanWon,cldata.clanWon[i+nScl_Bar]);
					Draw_C_Text2(szClanWon, menuPos[nIconCount].left+30,menuPos[nIconCount].top);
				}
				else
				{
					ZeroMemory(szClanWon,sizeof(szClanWon) );
					strcpy(szClanWon,cldata.clanWon[i+nScl_Bar]);
					Draw_C_Text(szClanWon, menuPos[nIconCount].left+30, menuPos[nIconCount].top);
				}
				nIconCount+=1;
			}
		}		
		
		break;
	case CLANCHIP_RESUBCHIP_APPOINT:		//부 클랜칩 임명 재확인
		DrawSprite(menuPos[3].left,menuPos[3].top, hButton_Box,  0, 0, 48, 23, 1);
		DrawSprite(menuPos[4].left,menuPos[4].top, hButton_Box,  0, 0, 48, 23, 1);
		if(menuMouseOver[0]>0)	DrawSprite(menuPos[0].left,menuPos[0].top, hYes_Yellow,  0, 0, 32, 16, 1);
		else					DrawSprite(menuPos[0].left,menuPos[0].top, hYes_Gray,  0, 0, 32, 16, 1);
		if(menuMouseOver[1]>0)	DrawSprite(menuPos[1].left,menuPos[1].top, hNo_Yellow,  0, 0, 32, 16, 1);
		else					DrawSprite(menuPos[1].left,menuPos[1].top, hNo_Gray,  0, 0, 32, 16, 1);

		DrawSprite(menuPos[6].left,menuPos[6].top, hTxt_LeaveClan[0],  0, 0, CLANTEXT_IMAGE_WIDTH, CLANTEXT_IMAGE_HEIGHT, 1);
		lpD3DDevice->BeginScene();
		sinDrawTexImage(hLine,(float)menuPos[5].left,(float)menuPos[5].top,
							(float)240, 10.0f,
							0.0f,0.0f,0.0f,0.0f,0.0f,0.0f,255);    
		lpD3DDevice->EndScene();	
		wsprintf(szDrawMenuMsg,SubChipAppoint.szMsg,cldata.clanWon[nSavePos]);		
		Draw_C_Text(szDrawMenuMsg, menuPos[2].left, menuPos[2].top);
		
		break;
		
	case CLANCHIP_RESUBCHIP_APPOINT_END:	//부 클랜칩임명 마지막 메세지
		DrawSprite(menuPos[2].left,menuPos[2].top, hButton_Box,  0, 0, 48, 23, 1);		
		if(menuMouseOver[0]>0) DrawSprite(menuPos[0].left,menuPos[0].top, hOk_Yellow,  0, 0, 32, 16, 1);
		else	DrawSprite(menuPos[0].left,menuPos[0].top, hOk_Gray,  0, 0, 32, 16, 1);
		
		//wsprintf(szDrawMenuMsg,LeaveClanMessage.szMsg,cldata.OldChipChaName,cldata.ClanZang_Name);
		wsprintf(szDrawMenuMsg,SubChipAppointEnd.szMsg,cldata.clanWon[nSavePos]);
		Draw_C_Text(szDrawMenuMsg, menuPos[1].left, menuPos[1].top);		
		break;
	case CLANCHIP_RELEASESUBCHIP:			//부 클랜칩해임 재확인
		DrawSprite(menuPos[3].left,menuPos[3].top, hButton_Box,  0, 0, 48, 23, 1);
		DrawSprite(menuPos[4].left,menuPos[4].top, hButton_Box,  0, 0, 48, 23, 1);
		if(menuMouseOver[0]>0)	DrawSprite(menuPos[0].left,menuPos[0].top, hYes_Yellow,  0, 0, 32, 16, 1);
		else					DrawSprite(menuPos[0].left,menuPos[0].top, hYes_Gray,  0, 0, 32, 16, 1);
		if(menuMouseOver[1]>0)	DrawSprite(menuPos[1].left,menuPos[1].top, hNo_Yellow,  0, 0, 32, 16, 1);
		else					DrawSprite(menuPos[1].left,menuPos[1].top, hNo_Gray,  0, 0, 32, 16, 1);

		DrawSprite(menuPos[6].left,menuPos[6].top, hTxt_LeaveClan[0],  0, 0, CLANTEXT_IMAGE_WIDTH, CLANTEXT_IMAGE_HEIGHT, 1);
		lpD3DDevice->BeginScene();
		sinDrawTexImage(hLine,(float)menuPos[5].left,(float)menuPos[5].top,
							(float)240, 10.0f,
							0.0f,0.0f,0.0f,0.0f,0.0f,0.0f,255);    
		lpD3DDevice->EndScene();	
		wsprintf(szDrawMenuMsg,SubChipRelease.szMsg,cldata.SubChip);//clanWon[nSavePos]);
		Draw_C_Text(szDrawMenuMsg, menuPos[2].left, menuPos[2].top);
		
		break;
	case CLANCHIP_RELEASESUBCHIP_END:		//클랜칩 해임 마지막 메세지
		DrawSprite(menuPos[2].left,menuPos[2].top, hButton_Box,  0, 0, 48, 23, 1);		
		if(menuMouseOver[0]>0) DrawSprite(menuPos[0].left,menuPos[0].top, hOk_Yellow,  0, 0, 32, 16, 1);
		else	DrawSprite(menuPos[0].left,menuPos[0].top, hOk_Gray,  0, 0, 32, 16, 1);
		
		
		wsprintf(szDrawMenuMsg,SubChipReleaseEnd.szMsg,cldata.SubChip);//cldata.clanWon[nSavePos]);
		Draw_C_Text(szDrawMenuMsg, menuPos[1].left, menuPos[1].top);		
		break;

	case CLAN_CHIP: //클랜 칩 인터페이스
		

		DrawSprite(menuPos[0].left,menuPos[0].top, hClanName,  0, 0, CLANNAME_WIDTH, CLANNAME_HEIGHT		, 1);	
		
		
		/*if(menuMouseOver[2] >0)
		{
			DrawSprite(menuPos[2].left,menuPos[2].top, hClanMark[1],  0, 0, 50, 44, 1);	
			DrawSprite(menuPos[2].left-CLANMARK_TXT_X,menuPos[2].bottom + CLANMARK_TXT_Y, hTxt_Mark[1],  0, 0, 80, 25, 1);	
		}
		else
		{
			DrawSprite(menuPos[2].left,menuPos[2].top, hClanMark[0],  0, 0, 50, 44, 1);	
			DrawSprite(menuPos[2].left-CLANMARK_TXT_X,menuPos[2].bottom + CLANMARK_TXT_Y, hTxt_Mark[0],  0, 0, 80, 25, 1);
		}*/
		//if((menuMouseOver[3] >0)||(menuMouseOver[13] >0)||(menuMouseOver[14] >0)) //추방
#ifndef _LANGUAGE_BRAZIL	//브라질 클랜 //해외
		if(menuMouseOver[3] >0)
		{
			//DrawSprite(menuPos[3].left - 31,menuPos[3].top, hRemove[1],   0, 0, 30, 30, 1);	
			//DrawSprite(menuPos[3].right + 1,menuPos[3].top, hRemove[1],   0, 0, 30, 30, 1);	
			DrawSprite(menuPos[3].left,menuPos[3].top, hTxt_Remove[1], 0, 0, CLANTEXT_IMAGE_WIDTH, CLANTEXT_IMAGE_HEIGHT, 1);;	
		}
		else
		{
			//DrawSprite(menuPos[3].left-31,menuPos[3].top, hRemove[0],  0, 0, 30, 30, 1);
			//DrawSprite(menuPos[3].right + 1,menuPos[3].top, hRemove[0],  0, 0, 30, 30, 1);
			DrawSprite(menuPos[3].left,menuPos[3].top, hTxt_Remove[0],  0, 0, CLANTEXT_IMAGE_WIDTH, CLANTEXT_IMAGE_HEIGHT, 1);
		}
		//if((menuMouseOver[4] >0)||(menuMouseOver[11] >0)||(menuMouseOver[12] >0))  //해체
		if(menuMouseOver[4] >0)
		{
			//DrawSprite(menuPos[4].left-31,menuPos[4].top, hClanRelease[1],  0, 0, 30, 30, 1);
			//DrawSprite(menuPos[4].right + 1,menuPos[4].top, hClanRelease[1],  0, 0, 30, 30, 1);
			DrawSprite(menuPos[4].left,menuPos[4].top, hTxt_Release[1],  0, 0, CLANTEXT_IMAGE_WIDTH, CLANTEXT_IMAGE_HEIGHT, 1);	
		}
		else
		{
			//DrawSprite(menuPos[4].left-31,menuPos[4].top, hClanRelease[0],  0, 0, 30, 30, 1);
			//DrawSprite(menuPos[4].right+1,menuPos[4].top, hClanRelease[0],  0, 0, 30, 30, 1);
			DrawSprite(menuPos[4].left,menuPos[4].top, hTxt_Release[0],  0, 0, CLANTEXT_IMAGE_WIDTH, CLANTEXT_IMAGE_HEIGHT, 1);	
		}
#endif
		//if((menuMouseOver[5] >0)||(menuMouseOver[15] >0)||(menuMouseOver[16] >0))
		if(menuMouseOver[5] >0)
		{
			//DrawSprite(menuPos[5].left-31,menuPos[5].top, hHelp[1],  0, 0, 30, 30, 1);
			//DrawSprite(menuPos[5].right + 1,menuPos[5].top, hHelp[1],  0, 0, 30, 30, 1);
			DrawSprite(menuPos[5].left,menuPos[5].top, hTxt_Help[1],  0, 0, CLANTEXT_IMAGE_WIDTH, CLANTEXT_IMAGE_HEIGHT, 1);	
		}
		else
		{
			//DrawSprite(menuPos[5].left - 31,menuPos[5].top, hHelp[0],  0, 0, 30, 30, 1);
			//DrawSprite(menuPos[5].right + 1,menuPos[5].top, hHelp[0],  0, 0, 30, 30, 1);
			DrawSprite(menuPos[5].left,menuPos[5].top, hTxt_Help[0],  0, 0, CLANTEXT_IMAGE_WIDTH, CLANTEXT_IMAGE_HEIGHT, 1);	
		}

#ifndef _LANGUAGE_BRAZIL	//브라질 클랜 //해외
		if(menuMouseOver[18] >0)
		{
			//DrawSprite(menuPos[5].left-31,menuPos[5].top, hHelp[1],  0, 0, 30, 30, 1);
			//DrawSprite(menuPos[5].right + 1,menuPos[5].top, hHelp[1],  0, 0, 30, 30, 1);
			DrawSprite(menuPos[18].left,menuPos[18].top, hTxt_LeaveClan[1],  0, 0, CLANTEXT_IMAGE_WIDTH, CLANTEXT_IMAGE_HEIGHT, 1);	
		}
		else
		{
			//DrawSprite(menuPos[5].left - 31,menuPos[5].top, hHelp[0],  0, 0, 30, 30, 1);
			//DrawSprite(menuPos[5].right + 1,menuPos[5].top, hHelp[0],  0, 0, 30, 30, 1);
			DrawSprite(menuPos[18].left,menuPos[18].top, hTxt_LeaveClan[0],  0, 0, CLANTEXT_IMAGE_WIDTH, CLANTEXT_IMAGE_HEIGHT, 1);	
		}
#endif

		
		//ktj : 클랜메뉴중 부클랜칲 임명메뉴를 일단 삭제
#ifdef __CLANSUBCHIP__
		if(menuMouseOver[19] >0)
		{
			//DrawSprite(menuPos[5].left-31,menuPos[5].top, hHelp[1],  0, 0, 30, 30, 1);
			//DrawSprite(menuPos[5].right + 1,menuPos[5].top, hHelp[1],  0, 0, 30, 30, 1);
			DrawSprite(menuPos[19].left,menuPos[19].top, hTxt_SubChip[1],  0, 0, CLANTEXT_IMAGE_WIDTH, CLANTEXT_IMAGE_HEIGHT, 1);	
		}
		else
		{
		
			//DrawSprite(menuPos[5].left - 31,menuPos[5].top, hHelp[0],  0, 0, 30, 30, 1);
			//DrawSprite(menuPos[5].right + 1,menuPos[5].top, hHelp[0],  0, 0, 30, 30, 1);
			DrawSprite(menuPos[19].left,menuPos[19].top, hTxt_SubChip[0],  0, 0, CLANTEXT_IMAGE_WIDTH, CLANTEXT_IMAGE_HEIGHT, 1);	
			
		}
#endif


		

		DrawSprite(menuPos[1].left,menuPos[1].top, hMark_box,  0, 0, 69, 59, 1);	

		if(cldata.hClanMark)
		{
			//DrawSprite(menuPos[1].left,menuPos[1].top, hMark,  0, 0, 16, 16, 1);
			//DrawSprite(menuPos[9].left,menuPos[9].top, hMark,  0, 0, 16, 16, 1);

			//DrawSprite(menuPos[1].left,menuPos[1].top, hMark,  0, 0, 32, 32, 1);
			//DrawSprite(menuPos[9].left,menuPos[9].top, hMark,  0, 0, 32, 32, 1);

			//void DrawSprite_TJB(int winX ,int winY, int winxlen, int winylen, LPDIRECTDRAWSURFACE4 pdds,int x, int y, int width, int height,int Bltflag);
			//DrawSprite_TJB(menuPos[1].left,menuPos[1].top, 16, 16,  hMark,  0, 0, 16, 16, 1);
			//DrawSprite_TJB(menuPos[9].left,menuPos[9].top, 16, 16,  hMark,  0, 0, 16, 16, 1);

			DrawSprite(menuPos[9].left,menuPos[9].top,  cldata.hClanMark,  0, 0, 32, 32, 1);
		}
		
		
		DrawSprite(menuPos[7].left,menuPos[7].top, hButton_Box,  0, 0, 48, 23, 1);

		if(menuMouseOver[6] >0) DrawSprite(menuPos[6].left,menuPos[6].top, hClose[1],  0, 0, 32, 16, 1);		
		else DrawSprite(menuPos[6].left,menuPos[6].top, hClose[0],  0, 0, 32, 16, 1);

		nStrLen = lstrlen(cldata.name)/2;		
		
		if(nStrLen%2)
		{
			nStrLen = nStrLen/2;
			nStrLen+=1;
			Draw_C_Text(cldata.name, menuPos[10].left- (nStrLen*11), menuPos[10].top, 2);		//2:그림자  
		}
		else 
		{
			nStrLen = nStrLen/2;		
			Draw_C_Text(cldata.name, menuPos[10].left- (nStrLen*11) -3, menuPos[10].top, 2);		//2:그림자  
		}

		lpD3DDevice->BeginScene();
		sinDrawTexImage(hLine,(float)menuPos[17].left,(float)menuPos[17].top,
							(float)240, 10.0f,
							0.0f,0.0f,0.0f,0.0f,0.0f,0.0f,255);    
		lpD3DDevice->EndScene();		
		
		
		break;
	case CLANCHIP_LEAVECLAN_NOLEVEL:
		DrawSprite(menuPos[2].left,menuPos[2].top, hButton_Box,  0, 0, 48, 23, 1);		
		if(menuMouseOver[0]>0) DrawSprite(menuPos[0].left,menuPos[0].top, hOk_Yellow,  0, 0, 32, 16, 1);
		else	DrawSprite(menuPos[0].left,menuPos[0].top, hOk_Gray,  0, 0, 32, 16, 1);
		
		Draw_C_Text(LeaveClanNoLevel.szMsg, menuPos[1].left, menuPos[1].top);				
		
		break;
	case CLANCHIP_LEAVECLAN_RECONFIRM:
		DrawSprite(menuPos[3].left,menuPos[3].top, hButton_Box,  0, 0, 48, 23, 1);
		DrawSprite(menuPos[4].left,menuPos[4].top, hButton_Box,  0, 0, 48, 23, 1);
		if(menuMouseOver[0]>0)	DrawSprite(menuPos[0].left,menuPos[0].top, hYes_Yellow,  0, 0, 32, 16, 1);
		else					DrawSprite(menuPos[0].left,menuPos[0].top, hYes_Gray,  0, 0, 32, 16, 1);
		if(menuMouseOver[1]>0)	DrawSprite(menuPos[1].left,menuPos[1].top, hNo_Yellow,  0, 0, 32, 16, 1);
		else					DrawSprite(menuPos[1].left,menuPos[1].top, hNo_Gray,  0, 0, 32, 16, 1);

		wsprintf(szDrawMenuMsg,LeaveClanReConfirm.szMsg,cldata.clanWon[nSavePos]);
		Draw_C_Text(szDrawMenuMsg, menuPos[2].left, menuPos[2].top);
		
		break;
	case CLANCHIP_LEAVECLAN_CONFIRM:
		DrawSprite(menuPos[3].left,menuPos[3].top, hButton_Box,  0, 0, 48, 23, 1);
		DrawSprite(menuPos[4].left,menuPos[4].top, hButton_Box,  0, 0, 48, 23, 1);
		if(menuMouseOver[0]>0)	DrawSprite(menuPos[0].left,menuPos[0].top, hYes_Yellow,  0, 0, 32, 16, 1);
		else					DrawSprite(menuPos[0].left,menuPos[0].top, hYes_Gray,  0, 0, 32, 16, 1);
		if(menuMouseOver[1]>0)	DrawSprite(menuPos[1].left,menuPos[1].top, hNo_Yellow,  0, 0, 32, 16, 1);
		else					DrawSprite(menuPos[1].left,menuPos[1].top, hNo_Gray,  0, 0, 32, 16, 1);

		DrawSprite(menuPos[6].left,menuPos[6].top, hTxt_LeaveClan[0],  0, 0, CLANTEXT_IMAGE_WIDTH, CLANTEXT_IMAGE_HEIGHT, 1);
		lpD3DDevice->BeginScene();
		sinDrawTexImage(hLine,(float)menuPos[5].left,(float)menuPos[5].top,
							(float)240, 10.0f,
							0.0f,0.0f,0.0f,0.0f,0.0f,0.0f,255);    
		lpD3DDevice->EndScene();	
		Draw_C_Text(LeaveClanConfirm.szMsg, menuPos[2].left, menuPos[2].top);
		
		break;
	case CLANCHIP_LEAVECLAN_MESSAGE:
		DrawSprite(menuPos[2].left,menuPos[2].top, hButton_Box,  0, 0, 48, 23, 1);		
		if(menuMouseOver[0]>0) DrawSprite(menuPos[0].left,menuPos[0].top, hOk_Yellow,  0, 0, 32, 16, 1);
		else	DrawSprite(menuPos[0].left,menuPos[0].top, hOk_Gray,  0, 0, 32, 16, 1);
		
		wsprintf(szDrawMenuMsg,LeaveClanMessage.szMsg,cldata.OldChipChaName,cldata.ClanZang_Name);
		Draw_C_Text(szDrawMenuMsg, menuPos[1].left, menuPos[1].top);		
		break;
	case CLANCHIP_LEAVECLANDOWNLEVEL:
		DrawSprite(menuPos[2].left,menuPos[2].top, hButton_Box,  0, 0, 48, 23, 1);		
		if(menuMouseOver[0]>0) DrawSprite(menuPos[0].left,menuPos[0].top, hOk_Yellow,  0, 0, 32, 16, 1);
		else	DrawSprite(menuPos[0].left,menuPos[0].top, hOk_Gray,  0, 0, 32, 16, 1);
		
		Draw_C_Text(LeaveClanDownLevel.szMsg, menuPos[1].left, menuPos[1].top);		
		break;
	case CLANCHIP_NOSUBCHIP:
		DrawSprite(menuPos[2].left,menuPos[2].top, hButton_Box,  0, 0, 48, 23, 1);		
		if(menuMouseOver[0]>0) DrawSprite(menuPos[0].left,menuPos[0].top, hOk_Yellow,  0, 0, 32, 16, 1);
		else	DrawSprite(menuPos[0].left,menuPos[0].top, hOk_Gray,  0, 0, 32, 16, 1);
		
		Draw_C_Text(NoSubChip.szMsg, menuPos[1].left, menuPos[1].top);		
		break;
	case CLANCHIP_NOLEAVECLAN:
		DrawSprite(menuPos[2].left,menuPos[2].top, hButton_Box,  0, 0, 48, 23, 1);		
		if(menuMouseOver[0]>0) DrawSprite(menuPos[0].left,menuPos[0].top, hOk_Yellow,  0, 0, 32, 16, 1);
		else	DrawSprite(menuPos[0].left,menuPos[0].top, hOk_Gray,  0, 0, 32, 16, 1);
		
		Draw_C_Text(NoLeaveClan.szMsg, menuPos[1].left, menuPos[1].top);		
		break;
	case CLANCHIP_LEAVECLAN_NOMONEY:
		DrawSprite(menuPos[2].left,menuPos[2].top, hButton_Box,  0, 0, 48, 23, 1);		
		if(menuMouseOver[0]>0) DrawSprite(menuPos[0].left,menuPos[0].top, hOk_Yellow,  0, 0, 32, 16, 1);
		else	DrawSprite(menuPos[0].left,menuPos[0].top, hOk_Gray,  0, 0, 32, 16, 1);
		
		Draw_C_Text(LeaveClanNoMoney.szMsg, menuPos[1].left, menuPos[1].top);		
		break;
	case CLANCHIP_LEAVECLAN_SUCESS:
		DrawSprite(menuPos[2].left,menuPos[2].top, hButton_Box,  0, 0, 48, 23, 1);		
		if(menuMouseOver[0]>0) DrawSprite(menuPos[0].left,menuPos[0].top, hOk_Yellow,  0, 0, 32, 16, 1);
		else	DrawSprite(menuPos[0].left,menuPos[0].top, hOk_Gray,  0, 0, 32, 16, 1);
		
		
		wsprintf(szDrawMenuMsg,LeaveClanSuccess.szMsg,cldata.clanWon[nSavePos]);
		Draw_C_Text(szDrawMenuMsg, menuPos[1].left, menuPos[1].top);		
		break;


	case CLANCHIP_LEAVECLAN://이임
#ifndef _LANGUAGE_BRAZIL	//브라질 클랜 //해외
		nIconCount = 11;
		if(nClanWon_Num>DISPLAY_MAX)
		{
			nIconCount+=1;
			/*
			DrawSprite(menuPos[nIconCount].left,menuPos[nIconCount].top, hScl_Bar,  0, 0, 8, 256, 1);
			//ktj : 스크롤 아이콘 그리기
			DrawSprite(scrIconPos.left, scrIconPos.top, hScl_icon,  0, 0, 16, 16, 1);			
			*/


			//스크롤바 bmp그림
			DrawSprite(menuPos[nIconCount].left, menuPos[nIconCount].top, hScl_Bar,   
				0, 0, 8, 160, 1);	

			lpD3DDevice->BeginScene();
			//sinDrawTexImage(hScl_Bar_tj,  (float)menuPos[nIconCount].left, (float)menuPos[nIconCount].top, 8, 256);


			//스크롤 위치표시 아이콘
			sinDrawTexImage( hScl_icon_tj , (float)scrIconPos.left, (float)scrIconPos.top, 16, 16);
			lpD3DDevice->EndScene();

		}
		nIconCount+=1;
		DrawSprite(menuPos[nIconCount].left,menuPos[nIconCount].top, hTxt_LeaveClan[0],  0, 0, CLANTEXT_IMAGE_WIDTH, CLANTEXT_IMAGE_HEIGHT, 1);
		nIconCount+=1;

		lpD3DDevice->BeginScene();
		sinDrawTexImage(hLine,(float)menuPos[nIconCount].left,(float)menuPos[nIconCount].top,
							(float)240, 10.0f,
							0.0f,0.0f,0.0f,0.0f,0.0f,0.0f,255);    
		lpD3DDevice->EndScene();	

		//DrawSprite(menuPos[nIconCount].left - 31,menuPos[nIconCount].top, hRemove[1],   0, 0, 30, 30, 1);	
		//DrawSprite(menuPos[nIconCount].right + 1,menuPos[nIconCount].top, hRemove[1],   0, 0, 30, 30, 1);	

		//extern int banisF[100];
		nIconCount = 3;
		for(i=0; i<DISPLAY_MAX; i++ )
		{
			if(i == nClanWon_Num) break;
			nIconCount+=1;
			if(banisF[nScl_Bar + i] ) {
				DrawSprite(menuPos[nIconCount].left,menuPos[nIconCount].top, hIcon_Click,  0, 0, 13, 17, 1);					
			}
			else {
				DrawSprite(menuPos[nIconCount].left,menuPos[nIconCount].top, hIcon,  0, 0, 13, 17, 1);									
			}
		}
		
				
		DrawSprite(menuPos[2].left,menuPos[2].top, hButton_Box,  0, 0, 48, 23, 1);
		DrawSprite(menuPos[3].left,menuPos[3].top, hButton_Box,  0, 0, 48, 23, 1);
		if(menuMouseOver[0] >0) DrawSprite(menuPos[0].left,menuPos[0].top, hOk_Yellow,  0, 0, 32, 16, 1);	
		else DrawSprite(menuPos[0].left,menuPos[0].top, hOk_Gray,  0, 0, 32, 16, 1);	
		if(menuMouseOver[1] >0) DrawSprite(menuPos[1].left,menuPos[1].top, hCancel_Yellow,  0, 0, 32, 16, 1);	
		else DrawSprite(menuPos[1].left,menuPos[1].top, hCancel_Gray,  0, 0, 32, 16, 1);
		//nIconCount=3;

		nIconCount=15; //클랜원 글자 위치 시작지점
		nAdd = 0;
		for(i=0; i<nClanWon_Num; i++)
		{	
			if(i >=DISPLAY_MAX) break;			
			
			#ifdef USE_PROSTONTALE		//=========== 포리스턴소스인경우
			bx = 13;
			by = 17;
			#else
			GetObject(hIcon,sizeof(BITMAP),&bit);
			bx = bit.bmWidth;
			by = bit.bmHeight;
			#endif

			menuPos[nIconCount].left = BackStartPos.x +  CLANWONLIST_X+30;		
			menuPos[nIconCount].top =  BackStartPos.y +   CLANWONLIST_Y +nAdd;//ICON_START + nAdd + REMOVE_ICON;
			menuPos[nIconCount].right =  (BackStartPos.x +  120) + cldata.clanWonStrLen[i+nScl_Bar];
			menuPos[nIconCount].bottom = menuPos[nIconCount].top + 17;
		//	nNext+=1;
			nAdd+=ICON_ADD;
			nClanWon[nIconCount] = 1;
			nIconCount += 1;
		}

		nIconCount=15;
		for(i=0; i < DISPLAY_MAX; i++)
		{
			
			if(nClanWon[nIconCount])
			{	
				ZeroMemory(szClanWon,sizeof(szClanWon) );
				strcpy(szClanWon,cldata.clanWon[i+nScl_Bar]);
				Draw_C_Text(szClanWon, menuPos[nIconCount].left+30, menuPos[nIconCount].top);
				nIconCount+=1;
			}
		}		
#endif
		break;


	case CLAN_REMOVE:	//추방
#ifndef _LANGUAGE_BRAZIL	//브라질 클랜 //해외
		nIconCount = 11;
		if(nClanWon_Num>DISPLAY_MAX)
		{
			nIconCount+=1;
			/*
			DrawSprite(menuPos[nIconCount].left,menuPos[nIconCount].top, hScl_Bar,  0, 0, 8, 256, 1);
			//ktj : 스크롤 아이콘 그리기
			DrawSprite(scrIconPos.left, scrIconPos.top, hScl_icon,  0, 0, 16, 16, 1);			
			*/


			//스크롤바 bmp그림
			DrawSprite(menuPos[nIconCount].left, menuPos[nIconCount].top, hScl_Bar,   
				0, 0, 8, 160, 1);	

			lpD3DDevice->BeginScene();
			//sinDrawTexImage(hScl_Bar_tj,  (float)menuPos[nIconCount].left, (float)menuPos[nIconCount].top, 8, 256);


			//스크롤 위치표시 아이콘
			sinDrawTexImage( hScl_icon_tj , (float)scrIconPos.left, (float)scrIconPos.top, 16, 16);
			lpD3DDevice->EndScene();

		}
		nIconCount+=1;
		DrawSprite(menuPos[nIconCount].left,menuPos[nIconCount].top, hTxt_Remove[0],  0, 0, CLANTEXT_IMAGE_WIDTH, CLANTEXT_IMAGE_HEIGHT, 1);
		//DrawSprite(menuPos[nIconCount].left - 31,menuPos[nIconCount].top, hRemove[1],   0, 0, 30, 30, 1);	
		//DrawSprite(menuPos[nIconCount].right + 1,menuPos[nIconCount].top, hRemove[1],   0, 0, 30, 30, 1);	

		//extern int banisF[100];
		nIconCount = 3;
		for(i=0; i<DISPLAY_MAX; i++ )
		{
			if(i == nClanWon_Num) break;
			nIconCount+=1;
			if(banisF[nScl_Bar + i] ) {
				DrawSprite(menuPos[nIconCount].left,menuPos[nIconCount].top, hIcon_Click,  0, 0, 13, 17, 1);					
			}
			else {
				DrawSprite(menuPos[nIconCount].left,menuPos[nIconCount].top, hIcon,  0, 0, 13, 17, 1);									
			}
		}
		
				
		DrawSprite(menuPos[2].left,menuPos[2].top, hButton_Box,  0, 0, 48, 23, 1);
		DrawSprite(menuPos[3].left,menuPos[3].top, hButton_Box,  0, 0, 48, 23, 1);
		if(menuMouseOver[0] >0) DrawSprite(menuPos[0].left,menuPos[0].top, hRemoveToMenu[1],  0, 0, 32, 16, 1);	
		else DrawSprite(menuPos[0].left,menuPos[0].top, hRemoveToMenu[0],  0, 0, 32, 16, 1);	
		if(menuMouseOver[1] >0) DrawSprite(menuPos[1].left,menuPos[1].top, hClose[1],  0, 0, 32, 16, 1);	
		else DrawSprite(menuPos[1].left,menuPos[1].top, hClose[0],  0, 0, 32, 16, 1);
		//nIconCount=3;

		nIconCount=14; //클랜원 글자 위치 시작지점
		nAdd = 0;
		for(i=0; i<nClanWon_Num; i++)
		{	
			if(i >=DISPLAY_MAX) break;			
			
			#ifdef USE_PROSTONTALE		//=========== 포리스턴소스인경우
			bx = 13;
			by = 17;
			#else
			GetObject(hIcon,sizeof(BITMAP),&bit);
			bx = bit.bmWidth;
			by = bit.bmHeight;
			#endif

			menuPos[nIconCount].left = BackStartPos.x +  CLANWONLIST_X+30;		
			menuPos[nIconCount].top =  BackStartPos.y +   CLANWONLIST_Y +nAdd;//ICON_START + nAdd + REMOVE_ICON;
			menuPos[nIconCount].right =  (BackStartPos.x +  120) + cldata.clanWonStrLen[i+nScl_Bar];
			menuPos[nIconCount].bottom = menuPos[nIconCount].top + 17;
		//	nNext+=1;
			nAdd+=ICON_ADD;
			nClanWon[nIconCount] = 1;
			nIconCount += 1;
		}

		nIconCount=14;
		for(i=0; i < DISPLAY_MAX; i++)
		{
			
			if(nClanWon[nIconCount])
			{	
				ZeroMemory(szClanWon,sizeof(szClanWon) );
				strcpy(szClanWon,cldata.clanWon[i+nScl_Bar]);
				Draw_C_Text(szClanWon, menuPos[nIconCount].left+30, menuPos[nIconCount].top);
				nIconCount+=1;
			}
		}		
		
	
#endif
		break;

	case CLAN_REMOVE_SHORTCLANWON:
		DrawSprite(menuPos[3].left,menuPos[3].top, hButton_Box,  0, 0, 48, 23, 1);
		DrawSprite(menuPos[4].left,menuPos[4].top, hButton_Box,  0, 0, 48, 23, 1);
		if(menuMouseOver[0]>0)	DrawSprite(menuPos[0].left,menuPos[0].top, hOk_Yellow,  0, 0, 32, 16, 1);
		else					DrawSprite(menuPos[0].left,menuPos[0].top, hOk_Gray,  0, 0, 32, 16, 1);
		if(menuMouseOver[1]>0)	DrawSprite(menuPos[1].left,menuPos[1].top, hCancel_Yellow,  0, 0, 32, 16, 1);
		else					DrawSprite(menuPos[1].left,menuPos[1].top, hCancel_Gray,  0, 0, 32, 16, 1);

		for(i=0; i< 5; i++)
		{
			if(szRemoveClanWonResult[i][0])
			{
				Draw_C_Text(szRemoveClanWonResult[i], menuPos[2].left, menuPos[2].top+nAdd);
				nSaveTop = menuPos[2].top+nAdd;
				nAdd+=20;
			}
		}		

		if(bRemoveSecondCheck) //6명 이상일 경우
		{
			Draw_C_Text(clanRemoveSecond.szMsg, menuPos[2].left, nSaveTop + 20);
		}
		else
		{
			Draw_C_Text(clanszRemoveShortClanWonMsg.szMsg, menuPos[2].left, nSaveTop + 20);
		}

		
		
		//Draw_C_Text(szMsg3 , menuPos[2].left, menuPos[2].top);
		break;
	case CLAN_REMOVE_SHORTCLANWON_RESULT:
		DrawSprite(menuPos[2].left,menuPos[2].top, hButton_Box,  0, 0, 48, 23, 1);		
		if(menuMouseOver[0]>0) DrawSprite(menuPos[0].left,menuPos[0].top, hOk_Yellow,  0, 0, 32, 16, 1);
		else	DrawSprite(menuPos[0].left,menuPos[0].top, hOk_Gray,  0, 0, 32, 16, 1);


		if(bRemoveSecondCheck)
		{
			Draw_C_Text(clanRemoveSecondResult.szMsg, menuPos[1].left, menuPos[1].top);
		}
		else
		{
			Draw_C_Text(clanRemoveShowrtClanWonResultMsg.szMsg, menuPos[1].left, menuPos[1].top);
		}
		//Draw_C_Text(clanRemoveShowrtClanWonResultMsg.szMsg, menuPos[1].left, menuPos[1].top);
		break;
	case CLANCHIP_RELEASEMESSAGE:
		DrawSprite(menuPos[2].left,menuPos[2].top, hButton_Box,  0, 0, 48, 23, 1);		
		if(menuMouseOver[0]>0) DrawSprite(menuPos[0].left,menuPos[0].top, hOk_Yellow,  0, 0, 32, 16, 1);
		else	DrawSprite(menuPos[0].left,menuPos[0].top, hOk_Gray,  0, 0, 32, 16, 1);
		Draw_C_Text(clanClanUserRelease.szMsg, menuPos[1].left, menuPos[1].top);
		break;
	case CLAN_REMOVE_RESULT:
		DrawSprite(menuPos[2].left,menuPos[2].top, hButton_Box,  0, 0, 48, 23, 1);		
		if(menuMouseOver[0]>0) DrawSprite(menuPos[0].left,menuPos[0].top, hOk_Yellow,  0, 0, 32, 16, 1);
		else	DrawSprite(menuPos[0].left,menuPos[0].top, hOk_Gray,  0, 0, 32, 16, 1);

		/*for(i=0; i< 5; i++)
		{
			if(i == 0) {
				if(szRemoveClanWonResult[i][0] !=0) {
				wsprintf(szRemoveClanWonResultMsg, "%s", szRemoveClanWonResult[i]);
				}
			}
			else {
				if(szRemoveClanWonResult[i][0] !=0) {
					wsprintf(szRemoveClanWonResultMsg, "%s, %s", szRemoveClanWonResultMsg, szRemoveClanWonResult[i]);
				}
				
			}
		}
		wsprintf(szRemoveClanWonResultMsg,"%s %s",szRemoveClanWonResultMsg,szReMoveClanWonMsg);
          */
		Draw_C_Text(clanReMoveHeaderMsg.szMsg, menuPos[1].left, menuPos[1].top);

		for(i=0; i< 5; i++)
		{
			if(szRemoveClanWonResult[i][0])
			{
				Draw_C_Text(szRemoveClanWonResult[i], menuPos[1].left, menuPos[1].top+20+nAdd);
				nSaveTop = menuPos[1].top+40+nAdd;
				nAdd+=20;
			}
		}		
		//Draw_C_Text(clanReMoveHeaderMsg.szMsg, menuPos[1].left, nSaveTop + 40);
		//Draw_C_Text(clanszReMoveClanWonMsg.szMsg, menuPos[1].left, nSaveTop + 40);
		break;


	/*case CLAN_WAREHOUSE_RIGHT:

		if(buttonclick[0].bIsClick) DrawSprite(menuPos[0].left,menuPos[0].top, hIcon_Click,  0, 0, 13, 17, 1);				
		else DrawSprite(menuPos[0].left,menuPos[0].top, hIcon,  0, 0, 13, 17, 1);				

		if(buttonclick[1].bIsClick) DrawSprite(menuPos[1].left,menuPos[1].top, hIcon_Click,  0, 0, 13, 17, 1);
		else DrawSprite(menuPos[1].left,menuPos[1].top, hIcon,  0, 0, 13, 17, 1);				
		if(buttonclick[2].bIsClick) DrawSprite(menuPos[2].left,menuPos[2].top, hIcon_Click,  0, 0, 54, 36, 1);
		else DrawSprite(menuPos[2].left,menuPos[2].top, hIcon,  0, 0, 13, 17, 1);				
		if(buttonclick[3].bIsClick) DrawSprite(menuPos[3].left,menuPos[3].top, hIcon_Click,  0, 0, 54, 36, 1);
		else DrawSprite(menuPos[3].left,menuPos[3].top, hIcon,  0, 0, 13, 17, 1);				
		if(buttonclick[4].bIsClick) DrawSprite(menuPos[4].left,menuPos[4].top, hIcon_Click,  0, 0, 54, 36, 1);
		else DrawSprite(menuPos[4].left,menuPos[4].top, hIcon,  0, 0, 13, 17, 1);	

		DrawSprite(menuPos[11].left,menuPos[11].top, hButton_Box,  0, 0, 68, 27, 1);

		if(menuMouseOver[5] > 0) DrawSprite(menuPos[5].left,menuPos[5].top, hOk_Yellow,  0, 0, 32, 16, 1);	
		else DrawSprite(menuPos[5].left,menuPos[5].top, hOk_Gray,  0, 0, 32, 16, 1);	
		
		Draw_C_Text(cldata.clanWon1, menuPos[6].left, menuPos[6].top);
		Draw_C_Text(cldata.clanWon2, menuPos[7].left, menuPos[7].top);
		Draw_C_Text(cldata.clanWon3, menuPos[8].left, menuPos[8].top);
		Draw_C_Text(cldata.clanWon4, menuPos[9].left, menuPos[9].top);
		Draw_C_Text(cldata.clanWon5, menuPos[10].left, menuPos[10].top);		

		break;*/



	case CLAN_MARK1:
		DrawSprite(menuPos[2].left,menuPos[2].top, hButton_Box,  0, 0, 8, 27, 1);
		if(menuMouseOver[0] > 0) DrawSprite(menuPos[0].left,menuPos[0].top, hYes_Yellow,  0, 0, 32, 16, 1);		
		else DrawSprite(menuPos[0].left,menuPos[0].top, hYes_Gray,  0, 0, 32, 16, 1);		
		DrawSprite(menuPos[3].left,menuPos[3].top, hTxt_Mark[1],  0, 0, CLANTEXT_IMAGE_WIDTH, CLANTEXT_IMAGE_HEIGHT, 1);
		
		wsprintf(string,"%s",szMark1 );
		Draw_C_Text(string, menuPos[1].left, menuPos[1].top);				
		break;
	case CLAN_MARK2:
		DrawSprite(menuPos[4].left,menuPos[4].top, hButton_Box,  0, 0, 48, 27, 1);
		DrawSprite(menuPos[5].left,menuPos[5].top, hButton_Box,  0, 0, 48, 27, 1);
		if(menuMouseOver[0] > 0) DrawSprite(menuPos[0].left,menuPos[0].top, hOk_Yellow,  0, 0, 32, 16, 1);		
		else DrawSprite(menuPos[0].left,menuPos[0].top, hOk_Gray,  0, 0, 32, 16, 1);		
		if(menuMouseOver[1] > 0) DrawSprite(menuPos[1].left,menuPos[1].top, hCancel_Yellow,  0, 0, 32, 16, 1);		
		else DrawSprite(menuPos[1].left,menuPos[1].top, hCancel_Gray,  0, 0, 32, 16, 1);		
			
		DrawSprite(menuPos[6].left,menuPos[6].top, hTxt_Mark[1],  0, 0, CLANTEXT_IMAGE_WIDTH, CLANTEXT_IMAGE_HEIGHT, 1);
		wsprintf(string,"%s",szMark2 );
		Draw_C_Text(string, menuPos[2].left, menuPos[2].top);						

		break;
	case CLAN_MARK3:
		DrawSprite(menuPos[5].left,menuPos[5].top, hButton_Box,  0, 0, 48, 23, 1);
		DrawSprite(menuPos[6].left,menuPos[6].top, hButton_Box,  0, 0, 48, 23, 1);
		DrawSprite(menuPos[7].left,menuPos[7].top, hTxt_Mark[1],  0, 0, CLANTEXT_IMAGE_WIDTH, CLANTEXT_IMAGE_HEIGHT, 1);
		if(menuMouseOver[0] > 0) DrawSprite(menuPos[0].left,menuPos[0].top, hOk_Yellow,  0, 0, 32, 16, 1);		
		else DrawSprite(menuPos[0].left,menuPos[0].top, hOk_Gray,  0, 0, 32, 16, 1);		
		if(menuMouseOver[1] > 0) DrawSprite(menuPos[1].left,menuPos[1].top, hCancel_Yellow,  0, 0, 32, 16, 1);		
		else DrawSprite(menuPos[1].left,menuPos[1].top, hCancel_Gray,  0, 0, 32, 16, 1);		
			
		//DrawSprite(menuPos[2].left,menuPos[2].top, hClinetMark,  0, 0, 15, 15, 1);		
		//DrawSprite(menuPos[3].left,menuPos[3].top, hServerMark,  0, 0, 15, 15, 1);	

		DrawSprite(menuPos[2].left,menuPos[2].top, hClinetMark,  0, 0, 32, 32, 1);		
		DrawSprite(menuPos[3].left,menuPos[3].top, hServerMark,  0, 0, 32, 32, 1);	

		wsprintf(string,"%s",szMark3 );
		Draw_C_Text(string, menuPos[4].left, menuPos[4].top);		
		
		break;



#ifndef _LANGUAGE_BRAZIL	//브라질 클랜 //해외
	case CLAN_RELEASE:
		
		DrawSprite(menuPos[3].left,menuPos[3].top, hButton_Box,  0, 0, 48, 23, 1);
		DrawSprite(menuPos[4].left,menuPos[4].top, hButton_Box,  0, 0, 48, 23, 1);
		if(menuMouseOver[0] > 0) DrawSprite(menuPos[0].left,menuPos[0].top, hYes_Yellow,  0, 0, 32, 16, 1);		
		else DrawSprite(menuPos[0].left,menuPos[0].top, hYes_Gray,  0, 0, 32, 16, 1);		
		if(menuMouseOver[1] > 0) DrawSprite(menuPos[1].left,menuPos[1].top, hNo_Yellow,  0, 0, 32, 16, 1);		
		else DrawSprite(menuPos[1].left,menuPos[1].top, hNo_Gray,  0, 0, 32, 16, 1);		

		DrawSprite(menuPos[5].left,menuPos[5].top, hTxt_Release[0],  0, 0, CLANTEXT_IMAGE_WIDTH, CLANTEXT_IMAGE_HEIGHT, 1);
		//DrawSprite(menuPos[5].left-31,menuPos[5].top, hClanRelease[1],  0, 0, 30, 30, 1);
		//DrawSprite(menuPos[5].right + 1,menuPos[5].top, hClanRelease[1],  0, 0, 30, 30, 1);
				
		//Draw_C_Text(szMsg15, menuPos[2].left, menuPos[2].top);		clanMsg15.szMsg
		Draw_C_Text(clanMsg15.szMsg, menuPos[2].left, menuPos[2].top);		

		break;
	case CLAN_RECONF_RELEASE:
		DrawSprite(menuPos[3].left,menuPos[3].top, hButton_Box,  0, 0, 48, 23, 1);
		DrawSprite(menuPos[4].left,menuPos[4].top, hButton_Box,  0, 0, 48, 23, 1);
		if(menuMouseOver[0] > 0) DrawSprite(menuPos[0].left,menuPos[0].top, hYes_Yellow,  0, 0, 32, 16, 1);		
		else DrawSprite(menuPos[0].left,menuPos[0].top, hYes_Gray,  0, 0, 32, 16, 1);		
		if(menuMouseOver[1] > 0) DrawSprite(menuPos[1].left,menuPos[1].top, hNo_Yellow,  0, 0, 32, 16, 1);		
		else DrawSprite(menuPos[1].left,menuPos[1].top, hNo_Gray,  0, 0, 32, 16, 1);		

		DrawSprite(menuPos[5].left,menuPos[5].top, hTxt_Release[0],  0, 0, CLANTEXT_IMAGE_WIDTH, CLANTEXT_IMAGE_HEIGHT, 1);
		//DrawSprite(menuPos[5].left-31,menuPos[5].top, hClanRelease[1],  0, 0, 30, 30, 1);
		//DrawSprite(menuPos[5].right + 1,menuPos[5].top, hClanRelease[1],  0, 0, 30, 30, 1);
				
		Draw_C_Text(szReconfReleaseMsg, menuPos[2].left, menuPos[2].top);	
		break;
	case CLAN_FINISH_RELEASE:
		DrawSprite(menuPos[1].left,menuPos[1].top, hButton_Box,  0, 0, 48, 23, 1);
		if(menuMouseOver[0] > 0) DrawSprite(menuPos[0].left,menuPos[0].top, hOk_Yellow,  0, 0, 32, 16, 1);		
		else DrawSprite(menuPos[0].left,menuPos[0].top, hOk_Gray,  0, 0, 32, 16, 1);		

		DrawSprite(menuPos[2].left,menuPos[2].top, hTxt_Release[0],  0, 0, CLANTEXT_IMAGE_WIDTH, CLANTEXT_IMAGE_HEIGHT, 1);
		//DrawSprite(menuPos[2].left-31,menuPos[2].top, hClanRelease[1],  0, 0, 30, 30, 1);
		//DrawSprite(menuPos[2].right + 1,menuPos[2].top, hClanRelease[1],  0, 0, 30, 30, 1);

		Draw_C_Text(clanFinishReleaseMsg.szMsg, menuPos[3].left, menuPos[3].top);	

		break;
#endif

	case CLAN_HELP:

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
				DrawSprite(menuPos[nIconPos].left,menuPos[nIconPos].top, hClanHelpIcon[i+nScl_Bar][0],  0, 0, CLANTEXT_IMAGE_WIDTH, CLANTEXT_IMAGE_HEIGHT, 1);
				
			}
			else
			{
				//DrawSprite(menuPos[nIconPos].left - 31,menuPos[nIconPos].top, hHelp[0],  0, 0, 30, 30, 1);
				//DrawSprite(menuPos[nIconPos].right +1,menuPos[nIconPos].top, hClanHelpIcon[i+nScl_Bar][0],  0, 0, 30, 30, 1);
				DrawSprite(menuPos[nIconPos].left,menuPos[nIconPos].top, hClanHelpIcon[i+nScl_Bar][1],  0, 0, CLANTEXT_IMAGE_WIDTH, CLANTEXT_IMAGE_HEIGHT, 1);
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
	case CLAN_HELP_CONTENT:
		
		
		DrawSprite(menuPos[1].left,menuPos[1].top, hButton_Box,  0, 0, 48, 23, 1);
		if(menuMouseOver[0] > 0) DrawSprite(menuPos[0].left,menuPos[0].top, hOk_Yellow,  0, 0, 32, 16, 1);		
		else DrawSprite(menuPos[0].left,menuPos[0].top, hOk_Gray,  0, 0, 32, 16, 1);				
		
		//DrawSprite(menuPos[2].left,menuPos[2].top, hClanHelpIcon[g_HelpIconNumber][1],  0, 0, CLANTEXT_IMAGE_WIDTH, CLANTEXT_IMAGE_HEIGHT, 1);
		DrawSprite(menuPos[2].left,menuPos[2].top, hTxt_Help[0],  0, 0, CLANTEXT_IMAGE_WIDTH, CLANTEXT_IMAGE_HEIGHT, 1);
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
			
			ZeroMemory(szHelpContent,sizeof(char)*HELPWIDTH);		
									
			strncpy(szHelpContent,HelpContents[i+nScl_Bar].szHelpContent,lstrlen(HelpContents[i+nScl_Bar].szHelpContent));			
			Draw_C_Text(szHelpContent, menuPos[nIconPos].left,menuPos[nIconPos].top);				
			nIconPos+=1;
		}		
		
		break;
	
	case CLANCHIP_MAKE_FIRSTMESSAGE: 
		DrawSprite(menuPos[2].left,menuPos[2].top, hButton_Box,  0, 0, 48, 23, 1);
		if(menuMouseOver[0] > 0) DrawSprite(menuPos[0].left,menuPos[0].top, hOk_Yellow,  0, 0, 32, 16, 1);		
		else DrawSprite(menuPos[0].left,menuPos[0].top, hOk_Gray,  0, 0, 32, 16, 1);				
		//wsprintf(szDrawMenuMsg,szMakeFinishClan,cldata.name);
		Draw_C_Text(szDrawMenuMsg, menuPos[1].left, menuPos[1].top);			
		break;
	case CLANCHIP_DOWN6_CLANWON_MESSAGE:
	case CLANCHIP_DOWN6_CLANWON:    
		DrawSprite(menuPos[2].left,menuPos[2].top, hButton_Box,  0, 0, 48, 23, 1);
		if(menuMouseOver[0] > 0) DrawSprite(menuPos[0].left,menuPos[0].top, hOk_Yellow,  0, 0, 32, 16, 1);		
		else DrawSprite(menuPos[0].left,menuPos[0].top, hOk_Gray,  0, 0, 32, 16, 1);				

		if(cldata. bIsOneDay){			
			wsprintf(szTemp,"\275\303\260\243 : %d : %d \263\262\276\322\275\300\264\317\264\331",cldata.nMinute,cldata.nHour);
			wsprintf(szDrawMenuMsg,szClanWon6Down,szTemp);
			Draw_C_Text(szDrawMenuMsg, menuPos[1].left, menuPos[1].top);			
		}
		else{
			wsprintf(szTemp,"\263\257\302\245 : %d\300\317 \263\262\276\322\275\300\264\317\264\331",cldata.nLastDay);
			wsprintf(szDrawMenuMsg,szClanWon6Down,szTemp);
			Draw_C_Text(szDrawMenuMsg, menuPos[1].left, menuPos[1].top);			
			
		}

		//Draw_C_Text(szClanWon6Down, menuPos[1].left, menuPos[1].top);			
		break;

	case CLAN_NOMAKE:
		DrawSprite(menuPos[2].left,menuPos[2].top, hButton_Box,  0, 0, 48, 23, 1);
		if(menuMouseOver[0] > 0) DrawSprite(menuPos[0].left,menuPos[0].top, hOk_Yellow,  0, 0, 32, 16, 1);		
		else DrawSprite(menuPos[0].left,menuPos[0].top, hOk_Gray,  0, 0, 32, 16, 1);
		//wsprintf(szDrawMenuMsg,szMsg6,cldata.nLastDay);

		if(cldata.bIsOneDay)
		{
			//wsprintf(szDrawMenuMsg,szMsg7,cldata.nHour,cldata.nMinute);
			Draw_C_Text(szDrawMenuMsg, menuPos[1].left, menuPos[1].top);			
			
		}
		else
		{
			//wsprintf(szDrawMenuMsg,szMsg6,cldata.nLastDay);
			Draw_C_Text(szDrawMenuMsg, menuPos[1].left, menuPos[1].top);
		}		
		
		
		break;
	
	case CLANCHIP_SUBCHIP_APPOINT_FLAG:		//클랜부침이 임명되어다는 플래그세팅
	case CLANCHIP_SUBCHIP_RELEASE_FLAG:		//클랜부침이 해임되어다는 플래그세팅	
	case CLANUSER_SUBCHIP_READMSG_FLAG:		//클랜유저가 부침관련 메세지를 읽어다는 플래그 세팅

	case	CLANCHIP_APPOINTSUBCHIP_DO:		//클랜부칩 임명 ASP호출
	case CLANCHIP_RELEASESUBCHIP_DO:		//클랜부칩 해임 ASP호출
	case CLANCHIP_LEAVECLAN_PFLAG:
	case CLANCHIP_LEAVECLAN_DO:
	case ISPFLAG_CHIP:
	case CLAN_REMOVE_SHORTCLANWON_LOADING:
	case CLAN_REMOVE_LOADING:
	case CLAN_RELEASE_LOADING:
	case CLAN_CHIP_ReadDATA:
		Draw_C_Text(clanLoading.szMsg, menuPos[0].left, menuPos[0].top);
		break;
	}
#else
         HDC destdc;
		switch(C_menuN){
		//===================부 클랜칩 임명및 해임
		case CLANCHIP_SUBCHIP_APPOINTMSG://클랜부침이 임명되다고 한번 보여줌
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
			if(cldata.SubChip[0] ==0)
			{
				wsprintf(szDrawMenuMsg,SubChipAppointEnd.szMsg,cldata.OldSubChip);
			}
			else{	wsprintf(szDrawMenuMsg,SubChipAppointEnd.szMsg,cldata.SubChip);}
			Text_Out(destdc,menuPos[1].left,menuPos[1].top,szDrawMenuMsg);

			
			
			DeleteDC(sorDC);
			DeleteDC(destdc);
			ReleaseDC(g_hwnd, hdc );
			break;
		case CLANCHIP_SUBCHIP_RELEASEMSG://클랜부침이 해임되다고 한번 보여줌
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
		case CLANCHIP_SUBCHIP_LIST:			//클랜원들 리스트 보여줌 부 클랜칩이 있으면 따로 보여줌
			hdc = GetDC(g_hwnd);
			sorDC  = CreateCompatibleDC(hdc);
			destdc=CreateCompatibleDC(hdc);

			SelectObject(destdc, Dback.hb );		//off스크린설정.
			
			nIconCount = 3;
			for(i=0; i<DISPLAY_MAX; i++ )
			{
				if(i == nClanWon_Num) break;
				nIconCount+=1;
				if(nClanWon[nIconCount])
				{
					if(banisF[nScl_Bar + i])
					{
						SelectObject(sorDC,hIcon_Click);
						GetObject(hIcon_Click,sizeof(BITMAP),&bit);	;			
						BitBlt(destdc,menuPos[nIconCount].left,menuPos[nIconCount].top,menuPos[nIconCount].right,menuPos[nIconCount].bottom,sorDC,0,0,SRCCOPY);
					}
					else
					{
						SelectObject(sorDC, hIcon);
						GetObject(hIcon,sizeof(BITMAP),&bit);			
						BitBlt(destdc,menuPos[nIconCount].left,menuPos[nIconCount].top,menuPos[nIconCount].right,menuPos[nIconCount].bottom,sorDC,0,0,SRCCOPY);
					}
				}			
			}
			nIconCount = 11;
			if(nClanWon_Num>DISPLAY_MAX)
			{
				nIconCount+=1;
				SelectObject(sorDC,hScl_Bar);
				GetObject(hScl_Bar,sizeof(BITMAP),&bit);		
				BitBlt(destdc,menuPos[nIconCount].left,menuPos[nIconCount].top,menuPos[nIconCount].right,menuPos[nIconCount].bottom,sorDC,0,0,SRCCOPY);
				

				SelectObject(sorDC,hScl_icon);
				GetObject(hScl_icon,sizeof(BITMAP),&bit);
				BitBlt(hdc,scrIconPos.left,scrIconPos.top,16,16,sorDC,0,0,SRCCOPY);
			
			}
			nIconCount+=1;
			SelectObject(sorDC,hTxt_SubChip[0]);
			GetObject(hTxt_SubChip[1],sizeof(BITMAP),&bit);		
			BitBlt(destdc,menuPos[nIconCount].left,menuPos[nIconCount].top,menuPos[nIconCount].right,menuPos[nIconCount].bottom,sorDC,0,0,SRCCOPY);

			nIconCount+=1;
			SelectObject(sorDC, hLine);
			GetObject(hLine,sizeof(BITMAP),&bit);
			StretchBlt(destdc,menuPos[nIconCount].left,menuPos[nIconCount].top,240,16,sorDC,0,0,16,16,SRCCOPY);
			
			


			nIconCount=15; //클랜원 글자 위치 시작지점
			nAdd = 0;
			for(i=0; i<nClanWon_Num; i++)
			{	
				if(i >=DISPLAY_MAX) break;			
				
				#ifdef USE_PROSTONTALE		//=========== 포리스턴소스인경우
				bx = 13;
				by = 17;
				#else
				GetObject(hIcon,sizeof(BITMAP),&bit);
				bx = bit.bmWidth;
				by = bit.bmHeight;
				#endif

				menuPos[nIconCount].left = BackStartPos.x +  CLANWONLIST_X+30;		
				menuPos[nIconCount].top =  BackStartPos.y +   CLANWONLIST_Y +nAdd;//ICON_START + nAdd + REMOVE_ICON;
				menuPos[nIconCount].right =  (BackStartPos.x +  120) + cldata.clanWonStrLen[i+nScl_Bar];
				menuPos[nIconCount].bottom = menuPos[nIconCount].top + 17;
			
				nAdd+=ICON_ADD;
				nClanWon[nIconCount] = 1;
				nIconCount += 1;
			}
			nIconCount=15;
			for(i=0; i < DISPLAY_MAX; i++)
			{
				
				if(nClanWon[nIconCount])
				{	
					if(strcmp(cldata.clanWon[i+nScl_Bar],cldata.SubChip) == 0)
					{
						strcpy(szClanWon,cldata.clanWon[i+nScl_Bar]);
						SetTextColor(destdc,RGB(0,255,0));
						dsTextLineOut(destdc,menuPos[nIconCount].left,menuPos[nIconCount].top,szClanWon,lstrlen(szClanWon));
					}
					else
					{
						SetTextColor(destdc,RGB(0,0,0));
						strcpy(szClanWon,cldata.clanWon[i+nScl_Bar]);
						//dsTextLineOut(destdc,0,30*i,szClanWon,lstrlen(szClanWon));					
						dsTextLineOut(destdc,menuPos[nIconCount].left,menuPos[nIconCount].top,szClanWon,lstrlen(szClanWon));
					}
					
					nIconCount+=1;
				}
			}		
			
			SelectObject(sorDC,hButton_Box);
			GetObject(hButton_Box,sizeof(BITMAP),&bit);
			bx = bit.bmWidth;
			by = bit.bmHeight;		
			BitBlt(destdc,menuPos[2].left,menuPos[2].top,menuPos[2].right,menuPos[2].bottom,sorDC,0,0,SRCCOPY);

			SelectObject(sorDC,hButton_Box);
			GetObject(hButton_Box,sizeof(BITMAP),&bit);
			bx = bit.bmWidth;
			by = bit.bmHeight;		
			BitBlt(destdc,menuPos[3].left,menuPos[3].top,menuPos[3].right,menuPos[3].bottom,sorDC,0,0,SRCCOPY);
			if(menuMouseOver[0] > 0)
			{
				SelectObject(sorDC,hOk_Yellow);
				GetObject(hOk_Yellow,sizeof(BITMAP),&bit);
				bx = bit.bmWidth;
				by = bit.bmHeight;		
				BitBlt(destdc,menuPos[0].left,menuPos[0].top,menuPos[0].right,menuPos[0].bottom,sorDC,0,0,SRCCOPY);
			}
			else
			{
				SelectObject(sorDC,hOk_Gray);
				GetObject(hOk_Gray,sizeof(BITMAP),&bit);
				bx = bit.bmWidth;
				by = bit.bmHeight;		
				BitBlt(destdc,menuPos[0].left,menuPos[0].top,menuPos[0].right,menuPos[0].bottom,sorDC,0,0,SRCCOPY);
			}
			if(menuMouseOver[1] > 0)
			{
				SelectObject(sorDC,hCancel_Yellow);
				GetObject(hCancel_Yellow,sizeof(BITMAP),&bit);
				bx = bit.bmWidth;
				by = bit.bmHeight;		
				BitBlt(destdc,menuPos[1].left,menuPos[1].top,menuPos[1].right,menuPos[1].bottom,sorDC,0,0,SRCCOPY);
			}
			else
			{
				SelectObject(sorDC,hCancel_Gray);
				GetObject(hCancel_Gray,sizeof(BITMAP),&bit);
				bx = bit.bmWidth;
				by = bit.bmHeight;		
				BitBlt(destdc,menuPos[1].left,menuPos[1].top,menuPos[1].right,menuPos[1].bottom,sorDC,0,0,SRCCOPY);
			}

			
			
			DeleteDC(sorDC);
			DeleteDC(destdc);
			ReleaseDC(g_hwnd, hdc );
			break;
		case CLANCHIP_RESUBCHIP_APPOINT:		//부 클랜칩 임명 재확인
			hdc = GetDC(g_hwnd);
			sorDC  = CreateCompatibleDC(hdc);
			destdc=CreateCompatibleDC(hdc);

			SelectObject(destdc, Dback.hb );		//off스크린설정.

			SelectObject(sorDC,hButton_Box);
			GetObject(hButton_Box,sizeof(BITMAP),&bit);
			bx = bit.bmWidth;
			by = bit.bmHeight;		
			BitBlt(destdc,menuPos[3].left,menuPos[3].top,menuPos[3].right,menuPos[3].bottom,sorDC,0,0,SRCCOPY);

			SelectObject(sorDC,hButton_Box);
			GetObject(hButton_Box,sizeof(BITMAP),&bit);
			bx = bit.bmWidth;
			by = bit.bmHeight;		
			BitBlt(destdc,menuPos[4].left,menuPos[4].top,menuPos[4].right,menuPos[4].bottom,sorDC,0,0,SRCCOPY);
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
				SelectObject(sorDC, hNo_Yellow);

				GetObject(hNo_Yellow,sizeof(BITMAP),&bit);
				bx = bit.bmWidth;
				by = bit.bmHeight;
				
				BitBlt(destdc,menuPos[1].left,menuPos[1].top,menuPos[1].right,menuPos[1].bottom,sorDC,0,0,SRCCOPY);
			}
			else
			{
				SelectObject(sorDC, hNo_Gray);

				GetObject(hNo_Gray,sizeof(BITMAP),&bit);
				bx = bit.bmWidth;
				by = bit.bmHeight;
				
				BitBlt(destdc,menuPos[1].left,menuPos[1].top,menuPos[1].right,menuPos[1].bottom,sorDC,0,0,SRCCOPY);
			}

			SelectObject(sorDC, hLine);
			GetObject(hLine,sizeof(BITMAP),&bit);
			StretchBlt(destdc,menuPos[5].left,menuPos[5].top,240,16,sorDC,0,0,16,16,SRCCOPY);

			SelectObject(sorDC,hTxt_LeaveClan[0]);
			GetObject(hTxt_LeaveClan[1],sizeof(BITMAP),&bit);		
			BitBlt(destdc,menuPos[6].left,menuPos[6].top,menuPos[6].right,menuPos[6].bottom,sorDC,0,0,SRCCOPY);



			wsprintf(szDrawMenuMsg,SubChipAppoint.szMsg,cldata.clanWon[nSavePos]);		
			
			
			Text_Out(destdc,menuPos[2].left,menuPos[2].top,szDrawMenuMsg);

			
			DeleteDC(sorDC);
			DeleteDC(destdc);
			ReleaseDC(g_hwnd, hdc );
			

			break;
		case CLANCHIP_RESUBCHIP_APPOINT_END:	//부 클랜칩임명 마지막 메세지
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
			wsprintf(szDrawMenuMsg,SubChipAppointEnd.szMsg,cldata.clanWon[nSavePos]);		
			Text_Out(destdc,menuPos[1].left,menuPos[1].top,szDrawMenuMsg);

			
			
			DeleteDC(sorDC);
			DeleteDC(destdc);
			ReleaseDC(g_hwnd, hdc );
			break;
		case CLANCHIP_RELEASESUBCHIP:			//부 클랜칩해임 재확인
			hdc = GetDC(g_hwnd);
			sorDC  = CreateCompatibleDC(hdc);
			destdc=CreateCompatibleDC(hdc);

			SelectObject(destdc, Dback.hb );		//off스크린설정.

			SelectObject(sorDC,hButton_Box);
			GetObject(hButton_Box,sizeof(BITMAP),&bit);
			bx = bit.bmWidth;
			by = bit.bmHeight;		
			BitBlt(destdc,menuPos[3].left,menuPos[3].top,menuPos[3].right,menuPos[3].bottom,sorDC,0,0,SRCCOPY);

			SelectObject(sorDC,hButton_Box);
			GetObject(hButton_Box,sizeof(BITMAP),&bit);
			bx = bit.bmWidth;
			by = bit.bmHeight;		
			BitBlt(destdc,menuPos[4].left,menuPos[4].top,menuPos[4].right,menuPos[4].bottom,sorDC,0,0,SRCCOPY);
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
				SelectObject(sorDC, hNo_Yellow);

				GetObject(hNo_Yellow,sizeof(BITMAP),&bit);
				bx = bit.bmWidth;
				by = bit.bmHeight;
				
				BitBlt(destdc,menuPos[1].left,menuPos[1].top,menuPos[1].right,menuPos[1].bottom,sorDC,0,0,SRCCOPY);
			}
			else
			{
				SelectObject(sorDC, hNo_Gray);

				GetObject(hNo_Gray,sizeof(BITMAP),&bit);
				bx = bit.bmWidth;
				by = bit.bmHeight;
				
				BitBlt(destdc,menuPos[1].left,menuPos[1].top,menuPos[1].right,menuPos[1].bottom,sorDC,0,0,SRCCOPY);
			}

			SelectObject(sorDC, hLine);
			GetObject(hLine,sizeof(BITMAP),&bit);
			StretchBlt(destdc,menuPos[5].left,menuPos[5].top,240,16,sorDC,0,0,16,16,SRCCOPY);

			SelectObject(sorDC,hTxt_LeaveClan[0]);
			GetObject(hTxt_LeaveClan[1],sizeof(BITMAP),&bit);		
			BitBlt(destdc,menuPos[6].left,menuPos[6].top,menuPos[6].right,menuPos[6].bottom,sorDC,0,0,SRCCOPY);



			wsprintf(szDrawMenuMsg,SubChipRelease.szMsg,cldata.SubChip);//cldata.clanWon[nSavePos]);		
			
			Text_Out(destdc,menuPos[2].left,menuPos[2].top,szDrawMenuMsg);

			
			DeleteDC(sorDC);
			DeleteDC(destdc);
			ReleaseDC(g_hwnd, hdc );
			

			break;
		case CLANCHIP_RELEASESUBCHIP_END:		//클랜칩 해임 마지막 메세지
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
			wsprintf(szDrawMenuMsg,SubChipReleaseEnd.szMsg,cldata.SubChip);//cldata.clanWon[nSavePos]);		
			Text_Out(destdc,menuPos[1].left,menuPos[1].top,szDrawMenuMsg);

			
			
			DeleteDC(sorDC);
			DeleteDC(destdc);
			ReleaseDC(g_hwnd, hdc );
			break;



		case CLAN_CHIP: //클랜 칩 인터페이스
			hdc = GetDC(g_hwnd);
			sorDC  = CreateCompatibleDC(hdc);
			destdc=CreateCompatibleDC(hdc);

			SelectObject(destdc, Dback.hb );		//off스크린설정.
				
			SelectObject(sorDC, hClanName);
			GetObject(hClanName,sizeof(BITMAP),&bit);
			bx = bit.bmWidth;
			by = bit.bmHeight;
			BitBlt(destdc,menuPos[0].left,menuPos[0].top,menuPos[0].right,menuPos[0].bottom,sorDC,0,0,SRCCOPY);
			//nX = 160 - (lstrlen(cldata.name)/2);
			nStrLen = lstrlen(cldata.name)/2;		
		
		if(nStrLen%2)
		{
			nStrLen = nStrLen/2;
			nStrLen+=1;
		}
		else nStrLen = nStrLen/2;
		
		
		Text_Out(destdc,menuPos[10].left - (nStrLen*11),menuPos[10].top,cldata.name);
			//dsTextLineOut(hdc,menuPos[0].left +CLAN_NAME_POS_X ,menuPos[0].top+CLAN_NAME_POS_Y,cldata.name,lstrlen(cldata.name));
		
		
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
			BitBlt(destdc,menuPos[9].left,menuPos[9].top,menuPos[9].right,menuPos[9].bottom,sorDC,0,0,SRCCOPY);
		}
	

			

			if(menuMouseOver[2] >0)
			{
				SelectObject(sorDC,hClanMark[1]);
				GetObject(hClanMark[1],sizeof(BITMAP),&bit);
				bx = bit.bmWidth;
				by = bit.bmHeight;		
				BitBlt(destdc,menuPos[2].left,menuPos[2].top,menuPos[2].right,menuPos[2].bottom,sorDC,0,0,SRCCOPY);

				SelectObject(sorDC,hTxt_Mark[1]);
				GetObject(hTxt_Mark[1],sizeof(BITMAP),&bit);
				bx = bit.bmWidth;
				by = bit.bmHeight;		
				BitBlt(destdc,menuPos[2].left-CLANMARK_TXT_X,menuPos[2].bottom+CLANMARK_TXT_Y,menuPos[2].right,menuPos[2].bottom,sorDC,0,0,SRCCOPY);
			}
			else
			{
				SelectObject(sorDC,hClanMark[0]);
				GetObject(hClanMark[0],sizeof(BITMAP),&bit);
				bx = bit.bmWidth;
				by = bit.bmHeight;		
				BitBlt(destdc,menuPos[2].left,menuPos[2].top,menuPos[2].right,menuPos[2].bottom,sorDC,0,0,SRCCOPY);

				SelectObject(sorDC,hTxt_Mark[0]);
				GetObject(hTxt_Mark[0],sizeof(BITMAP),&bit);
				bx = bit.bmWidth;
				by = bit.bmHeight;		
				BitBlt(destdc,menuPos[2].left-CLANMARK_TXT_X,menuPos[2].bottom+CLANMARK_TXT_Y ,menuPos[2].right,menuPos[2].bottom,sorDC,0,0,SRCCOPY);
			}

			//if((menuMouseOver[3] >0)||(menuMouseOver[13] >0)||(menuMouseOver[14] >0))

#ifndef _LANGUAGE_BRAZIL	//브라질 클랜 //해외
			if(menuMouseOver[3] >0)
			{
				//SelectObject(sorDC,hRemove[1]);
				//GetObject(hRemove[1],sizeof(BITMAP),&bit);
				//bx = bit.bmWidth;
				//by = bit.bmHeight;		
				//BitBlt(destdc,menuPos[3].left-31,menuPos[3].top,menuPos[3].right,menuPos[3].bottom,sorDC,0,0,SRCCOPY);

				//SelectObject(sorDC,hRemove[1]);
				//GetObject(hRemove[1],sizeof(BITMAP),&bit);
				//bx = bit.bmWidth;
				//by = bit.bmHeight;		
				//BitBlt(destdc,menuPos[3].right + 1,menuPos[3].top,menuPos[3].right,menuPos[3].bottom,sorDC,0,0,SRCCOPY);
				
				SelectObject(sorDC,hTxt_Remove[1]);
				GetObject(hTxt_Remove[1],sizeof(BITMAP),&bit);
				bx = bit.bmWidth;
				by = bit.bmHeight;		
				BitBlt(destdc,menuPos[3].left,menuPos[3].top,menuPos[3].right,menuPos[3].bottom,sorDC,0,0,SRCCOPY);


			}
			else
			{
				//SelectObject(sorDC,hRemove[0]);
				//GetObject(hRemove[0],sizeof(BITMAP),&bit);
				//bx = bit.bmWidth;
				//by = bit.bmHeight;		
				//BitBlt(destdc,menuPos[3].left - 31,menuPos[3].top,menuPos[3].right,menuPos[3].bottom,sorDC,0,0,SRCCOPY);

				//SelectObject(sorDC,hRemove[0]);
				//GetObject(hRemove[0],sizeof(BITMAP),&bit);
				//bx = bit.bmWidth;
				//by = bit.bmHeight;		
				//BitBlt(destdc,menuPos[3].right + 1,menuPos[3].top,menuPos[3].right,menuPos[3].bottom,sorDC,0,0,SRCCOPY);

				SelectObject(sorDC,hTxt_Remove[0]);
				GetObject(hTxt_Remove[0],sizeof(BITMAP),&bit);
				bx = bit.bmWidth;
				by = bit.bmHeight;		
				BitBlt(destdc,menuPos[3].left,menuPos[3].top,menuPos[3].right,menuPos[3].bottom,sorDC,0,0,SRCCOPY);
			}

			//클랜 해체
			//if((menuMouseOver[4] >0)||(menuMouseOver[11] >0)||(menuMouseOver[12] >0)) 
			if(menuMouseOver[4] >0)
			{
				//SelectObject(sorDC,hClanRelease[1]);
				//GetObject(hClanRelease[1],sizeof(BITMAP),&bit);
				//bx = bit.bmWidth;
				//by = bit.bmHeight;		
				//BitBlt(destdc,menuPos[11].left,menuPos[11].top,menuPos[11].right,menuPos[11].bottom,sorDC,0,0,SRCCOPY);

				//SelectObject(sorDC,hClanRelease[1]);
				//GetObject(hClanRelease[1],sizeof(BITMAP),&bit);
				//bx = bit.bmWidth;
				//by = bit.bmHeight;		
				//BitBlt(destdc,menuPos[4].right+1,menuPos[4].top,menuPos[4].right,menuPos[4].bottom,sorDC,0,0,SRCCOPY);

				SelectObject(sorDC,hTxt_Release[1]);
				GetObject(hTxt_Release[1],sizeof(BITMAP),&bit);
				bx = bit.bmWidth;
				by = bit.bmHeight;		
				BitBlt(destdc,menuPos[4].left,menuPos[4].top ,menuPos[4].right,menuPos[4].bottom,sorDC,0,0,SRCCOPY);
			}
			else
			{
				//SelectObject(sorDC,hClanRelease[0]);
				//GetObject(hClanRelease[0],sizeof(BITMAP),&bit);
				//bx = bit.bmWidth;
				//by = bit.bmHeight;		
				//BitBlt(destdc,menuPos[11].left,menuPos[11].top,menuPos[11].right,menuPos[11].bottom,sorDC,0,0,SRCCOPY);

				//SelectObject(sorDC,hClanRelease[0]);
				//GetObject(hClanRelease[0],sizeof(BITMAP),&bit);
				//bx = bit.bmWidth;
				//by = bit.bmHeight;		
				//BitBlt(destdc,menuPos[4].right + 1,menuPos[4].top,menuPos[4].right,menuPos[4].bottom,sorDC,0,0,SRCCOPY);

				SelectObject(sorDC,hTxt_Release[0]);
				GetObject(hTxt_Release[0],sizeof(BITMAP),&bit);
				bx = bit.bmWidth;
				by = bit.bmHeight;		
				BitBlt(destdc,menuPos[4].left,menuPos[4].top,menuPos[4].right,menuPos[4].bottom,sorDC,0,0,SRCCOPY);
			}
#endif
			
			//if((menuMouseOver[5] >0)||(menuMouseOver[15] >0)||(menuMouseOver[16] >0))
			if(menuMouseOver[5] >0)
			{
				
				SelectObject(sorDC,hTxt_Help[1]);
				GetObject(hTxt_Help[1],sizeof(BITMAP),&bit);
				bx = bit.bmWidth;
				by = bit.bmHeight;		
				BitBlt(destdc,menuPos[5].left,menuPos[5].top,menuPos[5].right,menuPos[5].bottom,sorDC,0,0,SRCCOPY);
			}
			else
			{				

				SelectObject(sorDC,hTxt_Help[0]);
				GetObject(hTxt_Help[0],sizeof(BITMAP),&bit);
				bx = bit.bmWidth;
				by = bit.bmHeight;		
				BitBlt(destdc,menuPos[5].left,menuPos[5].top,menuPos[5].right,menuPos[5].bottom,sorDC,0,0,SRCCOPY);
			}
			
			SelectObject(sorDC,hButton_Box);
			GetObject(hButton_Box,sizeof(BITMAP),&bit);
			bx = bit.bmWidth;
			by = bit.bmHeight;		
			BitBlt(destdc,menuPos[7].left,menuPos[7].top,menuPos[7].right,menuPos[7].bottom,sorDC,0,0,SRCCOPY);
			if(menuMouseOver[6] >0)
			{
				SelectObject(sorDC,hClose[1]);
				GetObject(hClose[1],sizeof(BITMAP),&bit);
				bx = bit.bmWidth;
				by = bit.bmHeight;		
				BitBlt(destdc,menuPos[6].left,menuPos[6].top,menuPos[6].right,menuPos[6].bottom,sorDC,0,0,SRCCOPY);
			}
			else
			{
				SelectObject(sorDC,hClose[0]);
				GetObject(hClose[0],sizeof(BITMAP),&bit);
				bx = bit.bmWidth;
				by = bit.bmHeight;		
				BitBlt(destdc,menuPos[6].left,menuPos[6].top,menuPos[6].right,menuPos[6].bottom,sorDC,0,0,SRCCOPY);
			}



			SelectObject(sorDC, hLine);
			GetObject(hLine,sizeof(BITMAP),&bit);
			StretchBlt(destdc,menuPos[17].left,menuPos[17].top,240,16,sorDC,0,0,16,16,SRCCOPY);

			if(menuMouseOver[18] >0)
			{
				
				SelectObject(sorDC,hTxt_LeaveClan[1]);
				GetObject(hTxt_LeaveClan[1],sizeof(BITMAP),&bit);
				bx = bit.bmWidth;
				by = bit.bmHeight;		
				BitBlt(destdc,menuPos[18].left,menuPos[18].top,menuPos[18].right,menuPos[18].bottom,sorDC,0,0,SRCCOPY);
			}
			else
			{				

				SelectObject(sorDC,hTxt_LeaveClan[0]);
				GetObject(hTxt_LeaveClan[0],sizeof(BITMAP),&bit);
				bx = bit.bmWidth;
				by = bit.bmHeight;		
				BitBlt(destdc,menuPos[18].left,menuPos[18].top,menuPos[18].right,menuPos[18].bottom,sorDC,0,0,SRCCOPY);
			}
#ifdef __CLANSUBCHIP__
			if(menuMouseOver[19] >0)
			{
				
				SelectObject(sorDC,hTxt_SubChip[1]);
				GetObject(hTxt_SubChip[1],sizeof(BITMAP),&bit);
				bx = bit.bmWidth;
				by = bit.bmHeight;		
				BitBlt(destdc,menuPos[19].left,menuPos[19].top,menuPos[19].right,menuPos[19].bottom,sorDC,0,0,SRCCOPY);
			}
			else
			{				

				SelectObject(sorDC,hTxt_SubChip[0]);
				GetObject(hTxt_SubChip[0],sizeof(BITMAP),&bit);
				bx = bit.bmWidth;
				by = bit.bmHeight;		
				BitBlt(destdc,menuPos[19].left,menuPos[19].top,menuPos[19].right,menuPos[19].bottom,sorDC,0,0,SRCCOPY);
			}
#endif


			DeleteDC(sorDC);
			DeleteDC(destdc);
			ReleaseDC(g_hwnd, hdc );
			break;
		case CLANCHIP_LEAVECLANDOWNLEVEL:
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
			Text_Out(destdc,menuPos[1].left,menuPos[1].top,LeaveClanDownLevel.szMsg);

			
			
			DeleteDC(sorDC);
			DeleteDC(destdc);
			ReleaseDC(g_hwnd, hdc );
			break;
		case CLANCHIP_NOSUBCHIP:
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
			Text_Out(destdc,menuPos[1].left,menuPos[1].top,NoSubChip.szMsg);

			
			
			DeleteDC(sorDC);
			DeleteDC(destdc);
			ReleaseDC(g_hwnd, hdc );
			break;
		case CLANCHIP_NOLEAVECLAN:
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
			Text_Out(destdc,menuPos[1].left,menuPos[1].top,NoLeaveClan.szMsg);

			
			
			DeleteDC(sorDC);
			DeleteDC(destdc);
			ReleaseDC(g_hwnd, hdc );
			break;

		case CLANCHIP_LEAVECLAN_CONFIRM:
			hdc = GetDC(g_hwnd);
			sorDC  = CreateCompatibleDC(hdc);
			destdc=CreateCompatibleDC(hdc);

			SelectObject(destdc, Dback.hb );		//off스크린설정.

			SelectObject(sorDC,hButton_Box);
			GetObject(hButton_Box,sizeof(BITMAP),&bit);
			bx = bit.bmWidth;
			by = bit.bmHeight;		
			BitBlt(destdc,menuPos[3].left,menuPos[3].top,menuPos[3].right,menuPos[3].bottom,sorDC,0,0,SRCCOPY);

			SelectObject(sorDC,hButton_Box);
			GetObject(hButton_Box,sizeof(BITMAP),&bit);
			bx = bit.bmWidth;
			by = bit.bmHeight;		
			BitBlt(destdc,menuPos[4].left,menuPos[4].top,menuPos[4].right,menuPos[4].bottom,sorDC,0,0,SRCCOPY);
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
				SelectObject(sorDC, hNo_Yellow);

				GetObject(hNo_Yellow,sizeof(BITMAP),&bit);
				bx = bit.bmWidth;
				by = bit.bmHeight;
				
				BitBlt(destdc,menuPos[1].left,menuPos[1].top,menuPos[1].right,menuPos[1].bottom,sorDC,0,0,SRCCOPY);
			}
			else
			{
				SelectObject(sorDC, hNo_Gray);

				GetObject(hNo_Gray,sizeof(BITMAP),&bit);
				bx = bit.bmWidth;
				by = bit.bmHeight;
				
				BitBlt(destdc,menuPos[1].left,menuPos[1].top,menuPos[1].right,menuPos[1].bottom,sorDC,0,0,SRCCOPY);
			}

			SelectObject(sorDC, hLine);
			GetObject(hLine,sizeof(BITMAP),&bit);
			StretchBlt(destdc,menuPos[5].left,menuPos[5].top,240,16,sorDC,0,0,16,16,SRCCOPY);

			SelectObject(sorDC,hTxt_LeaveClan[0]);
			GetObject(hTxt_LeaveClan[1],sizeof(BITMAP),&bit);		
			BitBlt(destdc,menuPos[6].left,menuPos[6].top,menuPos[6].right,menuPos[6].bottom,sorDC,0,0,SRCCOPY);



			
			Text_Out(destdc,menuPos[2].left,menuPos[2].top,LeaveClanConfirm.szMsg);

			
			DeleteDC(sorDC);
			DeleteDC(destdc);
			ReleaseDC(g_hwnd, hdc );
			

			break;
		case CLANCHIP_LEAVECLAN_NOLEVEL:
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
			Text_Out(destdc,menuPos[1].left,menuPos[1].top,LeaveClanNoLevel.szMsg);

			
			
			DeleteDC(sorDC);
			DeleteDC(destdc);
			ReleaseDC(g_hwnd, hdc );
			break;
		case CLANCHIP_LEAVECLAN_RECONFIRM:
			hdc = GetDC(g_hwnd);
			sorDC  = CreateCompatibleDC(hdc);
			destdc=CreateCompatibleDC(hdc);

			SelectObject(destdc, Dback.hb );		//off스크린설정.

			SelectObject(sorDC,hButton_Box);
			GetObject(hButton_Box,sizeof(BITMAP),&bit);
			bx = bit.bmWidth;
			by = bit.bmHeight;		
			BitBlt(destdc,menuPos[3].left,menuPos[3].top,menuPos[3].right,menuPos[3].bottom,sorDC,0,0,SRCCOPY);

			SelectObject(sorDC,hButton_Box);
			GetObject(hButton_Box,sizeof(BITMAP),&bit);
			bx = bit.bmWidth;
			by = bit.bmHeight;		
			BitBlt(destdc,menuPos[4].left,menuPos[4].top,menuPos[4].right,menuPos[4].bottom,sorDC,0,0,SRCCOPY);
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
				SelectObject(sorDC, hNo_Yellow);

				GetObject(hNo_Yellow,sizeof(BITMAP),&bit);
				bx = bit.bmWidth;
				by = bit.bmHeight;
				
				BitBlt(destdc,menuPos[1].left,menuPos[1].top,menuPos[1].right,menuPos[1].bottom,sorDC,0,0,SRCCOPY);
			}
			else
			{
				SelectObject(sorDC, hNo_Gray);

				GetObject(hNo_Gray,sizeof(BITMAP),&bit);
				bx = bit.bmWidth;
				by = bit.bmHeight;
				
				BitBlt(destdc,menuPos[1].left,menuPos[1].top,menuPos[1].right,menuPos[1].bottom,sorDC,0,0,SRCCOPY);
			}

			
			
			wsprintf(szDrawMenuMsg,LeaveClanReConfirm.szMsg,cldata.clanWon[nSavePos]);
			Text_Out(destdc,menuPos[2].left,menuPos[2].top,szDrawMenuMsg);

			
			DeleteDC(sorDC);
			DeleteDC(destdc);
			ReleaseDC(g_hwnd, hdc );
		
			break;
		case CLANCHIP_LEAVECLAN_MESSAGE:
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
			wsprintf(szDrawMenuMsg,LeaveClanMessage.szMsg,cldata.OldChipChaName,cldata.ClanZang_Name);
			Text_Out(destdc,menuPos[1].left,menuPos[1].top,szDrawMenuMsg);

			
			
			DeleteDC(sorDC);
			DeleteDC(destdc);
			ReleaseDC(g_hwnd, hdc );
			break;

		case CLANCHIP_LEAVECLAN_NOMONEY:
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
			Text_Out(destdc,menuPos[1].left,menuPos[1].top,LeaveClanNoMoney.szMsg);

			
			
			DeleteDC(sorDC);
			DeleteDC(destdc);
			ReleaseDC(g_hwnd, hdc );

			break;
		case CLANCHIP_LEAVECLAN_SUCESS:
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
			wsprintf(szDrawMenuMsg,LeaveClanSuccess.szMsg,cldata.clanWon[nSavePos]);
			Text_Out(destdc,menuPos[1].left,menuPos[1].top,szDrawMenuMsg);

			
			
			DeleteDC(sorDC);
			DeleteDC(destdc);
			ReleaseDC(g_hwnd, hdc );

			break;
		case CLANCHIP_LEAVECLAN://이임
			hdc = GetDC(g_hwnd);
			sorDC  = CreateCompatibleDC(hdc);
			destdc=CreateCompatibleDC(hdc);

			SelectObject(destdc, Dback.hb );		//off스크린설정.
			
			nIconCount = 3;
			for(i=0; i<DISPLAY_MAX; i++ )
			{
				if(i == nClanWon_Num) break;
				nIconCount+=1;
				if(nClanWon[nIconCount])
				{
					if(banisF[nScl_Bar + i])
					{
						SelectObject(sorDC,hIcon_Click);
						GetObject(hIcon_Click,sizeof(BITMAP),&bit);	;			
						BitBlt(destdc,menuPos[nIconCount].left,menuPos[nIconCount].top,menuPos[nIconCount].right,menuPos[nIconCount].bottom,sorDC,0,0,SRCCOPY);
					}
					else
					{
						SelectObject(sorDC, hIcon);
						GetObject(hIcon,sizeof(BITMAP),&bit);			
						BitBlt(destdc,menuPos[nIconCount].left,menuPos[nIconCount].top,menuPos[nIconCount].right,menuPos[nIconCount].bottom,sorDC,0,0,SRCCOPY);
					}
				}			
			}
			nIconCount = 11;
			if(nClanWon_Num>DISPLAY_MAX)
			{
				nIconCount+=1;
				SelectObject(sorDC,hScl_Bar);
				GetObject(hScl_Bar,sizeof(BITMAP),&bit);		
				BitBlt(destdc,menuPos[nIconCount].left,menuPos[nIconCount].top,menuPos[nIconCount].right,menuPos[nIconCount].bottom,sorDC,0,0,SRCCOPY);
				

				SelectObject(sorDC,hScl_icon);
				GetObject(hScl_icon,sizeof(BITMAP),&bit);
				BitBlt(hdc,scrIconPos.left,scrIconPos.top,16,16,sorDC,0,0,SRCCOPY);
			
			}
			nIconCount+=1;
			SelectObject(sorDC,hTxt_LeaveClan[0]);
			GetObject(hTxt_LeaveClan[1],sizeof(BITMAP),&bit);		
			BitBlt(destdc,menuPos[nIconCount].left,menuPos[nIconCount].top,menuPos[nIconCount].right,menuPos[nIconCount].bottom,sorDC,0,0,SRCCOPY);

			nIconCount+=1;
			SelectObject(sorDC, hLine);
			GetObject(hLine,sizeof(BITMAP),&bit);
			StretchBlt(destdc,menuPos[nIconCount].left,menuPos[nIconCount].top,240,16,sorDC,0,0,16,16,SRCCOPY);
			
			


			nIconCount=15; //클랜원 글자 위치 시작지점
			nAdd = 0;
			for(i=0; i<nClanWon_Num; i++)
			{	
				if(i >=DISPLAY_MAX) break;			
				
				#ifdef USE_PROSTONTALE		//=========== 포리스턴소스인경우
				bx = 13;
				by = 17;
				#else
				GetObject(hIcon,sizeof(BITMAP),&bit);
				bx = bit.bmWidth;
				by = bit.bmHeight;
				#endif

				menuPos[nIconCount].left = BackStartPos.x +  CLANWONLIST_X+30;		
				menuPos[nIconCount].top =  BackStartPos.y +   CLANWONLIST_Y +nAdd;//ICON_START + nAdd + REMOVE_ICON;
				menuPos[nIconCount].right =  (BackStartPos.x +  120) + cldata.clanWonStrLen[i+nScl_Bar];
				menuPos[nIconCount].bottom = menuPos[nIconCount].top + 17;
			
				nAdd+=ICON_ADD;
				nClanWon[nIconCount] = 1;
				nIconCount += 1;
			}
			nIconCount=15;
			for(i=0; i < DISPLAY_MAX; i++)
			{
				
				if(nClanWon[nIconCount])
				{					
					strcpy(szClanWon,cldata.clanWon[i+nScl_Bar]);
					dsTextLineOut(destdc,0,30*i,szClanWon,lstrlen(szClanWon));					
					dsTextLineOut(destdc,menuPos[nIconCount].left,menuPos[nIconCount].top,szClanWon,lstrlen(szClanWon));
					nIconCount+=1;
				}
			}		
			
			SelectObject(sorDC,hButton_Box);
			GetObject(hButton_Box,sizeof(BITMAP),&bit);
			bx = bit.bmWidth;
			by = bit.bmHeight;		
			BitBlt(destdc,menuPos[2].left,menuPos[2].top,menuPos[2].right,menuPos[2].bottom,sorDC,0,0,SRCCOPY);

			SelectObject(sorDC,hButton_Box);
			GetObject(hButton_Box,sizeof(BITMAP),&bit);
			bx = bit.bmWidth;
			by = bit.bmHeight;		
			BitBlt(destdc,menuPos[3].left,menuPos[3].top,menuPos[3].right,menuPos[3].bottom,sorDC,0,0,SRCCOPY);
			if(menuMouseOver[0] > 0)
			{
				SelectObject(sorDC,hOk_Yellow);
				GetObject(hOk_Yellow,sizeof(BITMAP),&bit);
				bx = bit.bmWidth;
				by = bit.bmHeight;		
				BitBlt(destdc,menuPos[0].left,menuPos[0].top,menuPos[0].right,menuPos[0].bottom,sorDC,0,0,SRCCOPY);
			}
			else
			{
				SelectObject(sorDC,hOk_Gray);
				GetObject(hOk_Gray,sizeof(BITMAP),&bit);
				bx = bit.bmWidth;
				by = bit.bmHeight;		
				BitBlt(destdc,menuPos[0].left,menuPos[0].top,menuPos[0].right,menuPos[0].bottom,sorDC,0,0,SRCCOPY);
			}
			if(menuMouseOver[1] > 0)
			{
				SelectObject(sorDC,hCancel_Yellow);
				GetObject(hCancel_Yellow,sizeof(BITMAP),&bit);
				bx = bit.bmWidth;
				by = bit.bmHeight;		
				BitBlt(destdc,menuPos[1].left,menuPos[1].top,menuPos[1].right,menuPos[1].bottom,sorDC,0,0,SRCCOPY);
			}
			else
			{
				SelectObject(sorDC,hCancel_Gray);
				GetObject(hCancel_Gray,sizeof(BITMAP),&bit);
				bx = bit.bmWidth;
				by = bit.bmHeight;		
				BitBlt(destdc,menuPos[1].left,menuPos[1].top,menuPos[1].right,menuPos[1].bottom,sorDC,0,0,SRCCOPY);
			}

			
			
			DeleteDC(sorDC);
			DeleteDC(destdc);
			ReleaseDC(g_hwnd, hdc );
			break;

#ifndef _LANGUAGE_BRAZIL	//브라질 클랜 //해외
		case CLAN_REMOVE: //추방
			
			hdc = GetDC(g_hwnd);
			sorDC  = CreateCompatibleDC(hdc);
			destdc=CreateCompatibleDC(hdc);

			SelectObject(destdc, Dback.hb );		//off스크린설정.
			/*nIconCount = 3;
			for(i=0; i<DISPLAY_MAX; i++ )
			{
				nIconCount+=1;
				if(banisF[nScl_Bar + i] ) {
					DrawSprite(menuPos[nIconCount].left,menuPos[nIconCount].top, hIcon_Click,  0, 0, 13, 17, 1);					
				}
				else {
					DrawSprite(menuPos[nIconCount].left,menuPos[nIconCount].top, hIcon,  0, 0, 13, 17, 1);									
				}
			}*/
			nIconCount = 3;
			for(i=0; i<DISPLAY_MAX; i++ )
			{
				if(i == nClanWon_Num) break;
				nIconCount+=1;
				if(nClanWon[nIconCount])
				{
					if(banisF[nScl_Bar + i])
					{
						SelectObject(sorDC,hIcon_Click);
						GetObject(hIcon_Click,sizeof(BITMAP),&bit);	;			
						BitBlt(destdc,menuPos[nIconCount].left,menuPos[nIconCount].top,menuPos[nIconCount].right,menuPos[nIconCount].bottom,sorDC,0,0,SRCCOPY);
					}
					else
					{
						SelectObject(sorDC, hIcon);
						GetObject(hIcon,sizeof(BITMAP),&bit);			
						BitBlt(destdc,menuPos[nIconCount].left,menuPos[nIconCount].top,menuPos[nIconCount].right,menuPos[nIconCount].bottom,sorDC,0,0,SRCCOPY);
					}
				}			
			}
			nIconCount = 11;
			if(nClanWon_Num>DISPLAY_MAX)
			{
				nIconCount+=1;
				SelectObject(sorDC,hScl_Bar);
				GetObject(hScl_Bar,sizeof(BITMAP),&bit);		
				BitBlt(destdc,menuPos[nIconCount].left,menuPos[nIconCount].top,menuPos[nIconCount].right,menuPos[nIconCount].bottom,sorDC,0,0,SRCCOPY);
				

				SelectObject(sorDC,hScl_icon);
				GetObject(hScl_icon,sizeof(BITMAP),&bit);
				BitBlt(hdc,scrIconPos.left,scrIconPos.top,16,16,sorDC,0,0,SRCCOPY);
				//nIconCount+=1;

				//SelectObject(sorDC,hScl_icon);
				//GetObject(hScl_icon,sizeof(BITMAP),&bit);			
				//BitBlt(hdc,menuPos[nIconCount].left,menuPos[nIconCount].top,menuPos[nIconCount].right,menuPos[nIconCount].bottom,sorDC,0,0,SRCCOPY);
			}
			nIconCount+=1;
			SelectObject(sorDC,hTxt_Remove[0]);
			GetObject(hTxt_Remove[1],sizeof(BITMAP),&bit);		
			BitBlt(destdc,menuPos[nIconCount].left,menuPos[nIconCount].top,menuPos[nIconCount].right,menuPos[nIconCount].bottom,sorDC,0,0,SRCCOPY);
			
			//SelectObject(sorDC,hRemove[1]);
			//GetObject(hRemove[1],sizeof(BITMAP),&bit);
			//bx = bit.bmWidth;
			//by = bit.bmHeight;		
			//BitBlt(destdc,menuPos[nIconCount].left-31,menuPos[nIconCount].top,menuPos[nIconCount].right,menuPos[nIconCount].bottom,sorDC,0,0,SRCCOPY);

			//SelectObject(sorDC,hRemove[1]);
			//GetObject(hRemove[1],sizeof(BITMAP),&bit);
			//bx = bit.bmWidth;
			//by = bit.bmHeight;		
			//BitBlt(destdc,menuPos[nIconCount].right + 1,menuPos[nIconCount].top,menuPos[nIconCount].right,menuPos[nIconCount].bottom,sorDC,0,0,SRCCOPY);


			nIconCount=14; //클랜원 글자 위치 시작지점
			nAdd = 0;
			for(i=0; i<nClanWon_Num; i++)
			{	
				if(i >=DISPLAY_MAX) break;			
				
				#ifdef USE_PROSTONTALE		//=========== 포리스턴소스인경우
				bx = 13;
				by = 17;
				#else
				GetObject(hIcon,sizeof(BITMAP),&bit);
				bx = bit.bmWidth;
				by = bit.bmHeight;
				#endif

				menuPos[nIconCount].left = BackStartPos.x +  CLANWONLIST_X+30;		
				menuPos[nIconCount].top =  BackStartPos.y +   CLANWONLIST_Y +nAdd;//ICON_START + nAdd + REMOVE_ICON;
				menuPos[nIconCount].right =  (BackStartPos.x +  120) + cldata.clanWonStrLen[i+nScl_Bar];
				menuPos[nIconCount].bottom = menuPos[nIconCount].top + 17;
			//	nNext+=1;
				nAdd+=ICON_ADD;
				nClanWon[nIconCount] = 1;
				nIconCount += 1;
			}
			nIconCount=14;
			for(i=0; i < DISPLAY_MAX; i++)
			{
				
				if(nClanWon[nIconCount])
				{						
					//strcpy(szClanWon,clanWons[i+nScl_Bar].clanWon);
					strcpy(szClanWon,cldata.clanWon[i+nScl_Bar]);
					dsTextLineOut(destdc,0,30*i,szClanWon,lstrlen(szClanWon));
					//dsTextLineOut(hdc,menuPos[nIconCount].left+ 30,menuPos[nIconCount].top,szClanWon,lstrlen(szClanWon));
					dsTextLineOut(destdc,menuPos[nIconCount].left,menuPos[nIconCount].top,szClanWon,lstrlen(szClanWon));
					nIconCount+=1;
				}
			}		
			
			SelectObject(sorDC,hButton_Box);
			GetObject(hButton_Box,sizeof(BITMAP),&bit);
			bx = bit.bmWidth;
			by = bit.bmHeight;		
			BitBlt(destdc,menuPos[2].left,menuPos[2].top,menuPos[2].right,menuPos[2].bottom,sorDC,0,0,SRCCOPY);

			SelectObject(sorDC,hButton_Box);
			GetObject(hButton_Box,sizeof(BITMAP),&bit);
			bx = bit.bmWidth;
			by = bit.bmHeight;		
			BitBlt(destdc,menuPos[3].left,menuPos[3].top,menuPos[3].right,menuPos[3].bottom,sorDC,0,0,SRCCOPY);
			if(menuMouseOver[0] > 0)
			{
				SelectObject(sorDC,hRemoveToMenu[1]);
				GetObject(hRemoveToMenu[1],sizeof(BITMAP),&bit);
				bx = bit.bmWidth;
				by = bit.bmHeight;		
				BitBlt(destdc,menuPos[0].left,menuPos[0].top,menuPos[0].right,menuPos[0].bottom,sorDC,0,0,SRCCOPY);
			}
			else
			{
				SelectObject(sorDC,hRemoveToMenu[0]);
				GetObject(hRemoveToMenu[0],sizeof(BITMAP),&bit);
				bx = bit.bmWidth;
				by = bit.bmHeight;		
				BitBlt(destdc,menuPos[0].left,menuPos[0].top,menuPos[0].right,menuPos[0].bottom,sorDC,0,0,SRCCOPY);
			}
			if(menuMouseOver[1] > 0)
			{
				SelectObject(sorDC,hClose[1]);
				GetObject(hClose[1],sizeof(BITMAP),&bit);
				bx = bit.bmWidth;
				by = bit.bmHeight;		
				BitBlt(destdc,menuPos[1].left,menuPos[1].top,menuPos[1].right,menuPos[1].bottom,sorDC,0,0,SRCCOPY);
			}
			else
			{
				SelectObject(sorDC,hClose[0]);
				GetObject(hClose[0],sizeof(BITMAP),&bit);
				bx = bit.bmWidth;
				by = bit.bmHeight;		
				BitBlt(destdc,menuPos[1].left,menuPos[1].top,menuPos[1].right,menuPos[1].bottom,sorDC,0,0,SRCCOPY);
			}

			
			
			DeleteDC(sorDC);
			DeleteDC(destdc);
			ReleaseDC(g_hwnd, hdc );
			break;
#endif
		case CLAN_REMOVE_SHORTCLANWON:
			hdc = GetDC(g_hwnd);
			sorDC  = CreateCompatibleDC(hdc);
			destdc=CreateCompatibleDC(hdc);

			SelectObject(destdc, Dback.hb );		//off스크린설정.

			SelectObject(sorDC,hButton_Box);
			GetObject(hButton_Box,sizeof(BITMAP),&bit);
			bx = bit.bmWidth;
			by = bit.bmHeight;		
			BitBlt(destdc,menuPos[3].left,menuPos[3].top,menuPos[3].right,menuPos[3].bottom,sorDC,0,0,SRCCOPY);
			

			SelectObject(sorDC,hButton_Box);
			GetObject(hButton_Box,sizeof(BITMAP),&bit);
			bx = bit.bmWidth;
			by = bit.bmHeight;		
			BitBlt(destdc,menuPos[4].left,menuPos[4].top,menuPos[4].right,menuPos[4].bottom,sorDC,0,0,SRCCOPY);
			if(menuMouseOver[0] >0)
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
			if(menuMouseOver[1] >0)
			{
				SelectObject(sorDC, hCancel_Yellow);
				GetObject(hCancel_Yellow,sizeof(BITMAP),&bit);
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
			

			for(i=0; i< 5; i++)
			{
				if(szRemoveClanWonResult[i][0])
				{
					//dsTextLineOut(GetDC(g_hwnd),menuPos[2].left,menuPos[2].top+nAdd,szRemoveClanWonResult[i],lstrlen(szRemoveClanWonResult[i]));
					//Draw_Text(menuPos[2].left,menuPos[2].top+nAdd,szRemoveClanWonResult[i]);
					Text_Out(destdc,menuPos[2].left,menuPos[2].top+nAdd,szRemoveClanWonResult[i]);
					
					nSaveTop = menuPos[2].top+nAdd;
					nAdd+=20;
				}
			}		
			//dsTextLineOut(GetDC(g_hwnd),menuPos[2].left,nSaveTop+20,szRemoveShortClanWonMsg,lstrlen(szReMoveClanWonMsg ));
			if(bRemoveSecondCheck) //6명 이상일 경우
			{
				Text_Out(destdc,menuPos[2].left,nSaveTop+20,clanRemoveSecond.szMsg);
				
			}
			else
			{
				Text_Out(destdc,menuPos[2].left,nSaveTop+20,clanszRemoveShortClanWonMsg.szMsg);
			}
			
			

			
			
			DeleteDC(sorDC);
			DeleteDC(destdc);
			ReleaseDC(g_hwnd, hdc );
			
			break;
		case CLANCHIP_RELEASEMESSAGE:
			hdc = GetDC(g_hwnd);
			sorDC  = CreateCompatibleDC(hdc);	
			destdc=CreateCompatibleDC(hdc);

			SelectObject(destdc, Dback.hb );		//off스크린설정.

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

			Text_Out(destdc,menuPos[1].left,menuPos[1].top,clanClanUserRelease.szMsg);
			DeleteDC(sorDC);
			DeleteDC(destdc);
			ReleaseDC(g_hwnd, hdc );
			break;

		case CLAN_REMOVE_SHORTCLANWON_RESULT:
			hdc = GetDC(g_hwnd);
			sorDC  = CreateCompatibleDC(hdc);	
			destdc=CreateCompatibleDC(hdc);

			SelectObject(destdc, Dback.hb );		//off스크린설정.

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
			if(bRemoveSecondCheck)
			{
				Text_Out(destdc,menuPos[1].left,menuPos[1].top,clanRemoveSecondResult.szMsg);
			}
			else
			{
				Text_Out(destdc,menuPos[1].left,menuPos[1].top,clanRemoveShowrtClanWonResultMsg.szMsg);
			}

			
			DeleteDC(sorDC);
			DeleteDC(destdc);
			ReleaseDC(g_hwnd, hdc );
			break;
		
		case CLAN_REMOVE_RESULT:
			hdc = GetDC(g_hwnd);
			sorDC  = CreateCompatibleDC(hdc);	
			destdc=CreateCompatibleDC(hdc);

			SelectObject(destdc, Dback.hb );		//off스크린설정.

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
			
			rect.left = 70;
			rect.top  = 70;
			rect.right = 280;
			rect.bottom =250;
			/*for(i=0; i< 5; i++)
			{
				if(i == 0) {
					if(szRemoveClanWonResult[i][0] !=0) {
					wsprintf(szRemoveClanWonResultMsg, "%s", szRemoveClanWonResult[i]);
					}
				}
				else {
					if(szRemoveClanWonResult[i][0] !=0) {
						wsprintf(szRemoveClanWonResultMsg, "%s, %s", szRemoveClanWonResultMsg, szRemoveClanWonResult[i]);
					}
					
				}
			}
			wsprintf(szRemoveClanWonResultMsg,"%s %s",szRemoveClanWonResultMsg,szReMoveClanWonMsg);*/
			//DrawText(GetDC(g_hwnd),szReMoveHeaderMsg,-1,&rect,DT_LEFT | DT_WORDBREAK);
			//dsTextLineOut(GetDC(g_hwnd),menuPos[1].left,menuPos[1].top,szReMoveHeaderMsg,lstrlen(szReMoveHeaderMsg));
			//Draw_Text(menuPos[1].left,menuPos[1].top,szReMoveHeaderMsg);
			//Text_Out(destdc,menuPos[1].left,nSaveTop+20,clanReMoveHeaderMsg.szMsg);
	/*		for(i=0; i< 5; i++)
			{
				if(szRemoveClanWonResult[i][0])
				{
					Draw_C_Text(szRemoveClanWonResult[i], menuPos[1].left, menuPos[1].top+40+nAdd);
					nSaveTop = menuPos[1].top+40+nAdd;
					nAdd+=20;
				}
			}		*/
			for(i=0; i< 5; i++)
			{
				if(szRemoveClanWonResult[i][0])
				{
					//dsTextLineOut(GetDC(g_hwnd),menuPos[1].left,menuPos[1].top+40+nAdd,szRemoveClanWonResult[i],lstrlen(szRemoveClanWonResult[i]));
					//Draw_Text(menuPos[1].left,menuPos[1].top+40+nAdd,szRemoveClanWonResult[i]);
					Text_Out(destdc,menuPos[1].left,menuPos[1].top+40+nAdd,szRemoveClanWonResult[i]);
					nSaveTop = menuPos[1].top+40+nAdd;
					nAdd+=20;
				}
			}
			Text_Out(destdc,menuPos[1].left,menuPos[1].top,clanReMoveHeaderMsg.szMsg);
			//dsTextLineOut(GetDC(g_hwnd),menuPos[1].left,nSaveTop+20,szReMoveClanWonMsg ,lstrlen(szReMoveClanWonMsg ));

			//Draw_Text(menuPos[1].left,nSaveTop+20,szReMoveClanWonMsg);
			//Text_Out(destdc,menuPos[1].left,nSaveTop+20,clanszReMoveClanWonMsg.szMsg);
			SelectObject(sorDC, hButton_Box);
			
			

			DeleteDC(sorDC);
			DeleteDC(destdc);
			ReleaseDC(g_hwnd, hdc );
			break;
		
		/*case CLAN_WAREHOUSE_RIGHT:
			hdc = GetDC(g_hwnd);
			sorDC  = CreateCompatibleDC(hdc);
				
			if(buttonclick[0].bIsClick)
			{
				SelectObject(sorDC,hIcon_Click);
				GetObject(hIcon_Click,sizeof(BITMAP),&bit);	;			
				BitBlt(hdc,menuPos[0].left,menuPos[0].top,menuPos[0].right,menuPos[0].bottom,sorDC,0,0,SRCCOPY);
			}
			else
			{
				SelectObject(sorDC, hIcon);
				GetObject(hIcon,sizeof(BITMAP),&bit);			
				BitBlt(hdc,menuPos[0].left,menuPos[0].top,menuPos[0].right,menuPos[0].bottom,sorDC,0,0,SRCCOPY);
			}
			
			if(buttonclick[1].bIsClick)
			{
				SelectObject(sorDC,hIcon_Click);
				GetObject(hIcon_Click,sizeof(BITMAP),&bit);			
				BitBlt(hdc,menuPos[1].left,menuPos[1].top,menuPos[1].right,menuPos[1].bottom,sorDC,0,0,SRCCOPY);
			}
			else
			{
				SelectObject(sorDC, hIcon);
				GetObject(hIcon,sizeof(BITMAP),&bit);
				bx = bit.bmWidth;
				by = bit.bmHeight;
				BitBlt(hdc,menuPos[1].left,menuPos[1].top,menuPos[1].right,menuPos[1].bottom,sorDC,0,0,SRCCOPY);
			}

			if(buttonclick[2].bIsClick)
			{
				SelectObject(sorDC,hIcon_Click);
				GetObject(hIcon_Click,sizeof(BITMAP),&bit);			
				BitBlt(hdc,menuPos[2].left,menuPos[2].top,menuPos[2].right,menuPos[2].bottom,sorDC,0,0,SRCCOPY);
			}
			else
			{
				SelectObject(sorDC, hIcon);
				GetObject(hIcon,sizeof(BITMAP),&bit);
				bx = bit.bmWidth;
				by = bit.bmHeight;
				BitBlt(hdc,menuPos[2].left,menuPos[2].top,menuPos[2].right,menuPos[2].bottom,sorDC,0,0,SRCCOPY);
			}

			if(buttonclick[3].bIsClick)
			{
				SelectObject(sorDC,hIcon_Click);
				GetObject(hIcon_Click,sizeof(BITMAP),&bit);			
				BitBlt(hdc,menuPos[3].left,menuPos[3].top,menuPos[3].right,menuPos[3].bottom,sorDC,0,0,SRCCOPY);
			}
			else
			{
				SelectObject(sorDC, hIcon);
				GetObject(hIcon,sizeof(BITMAP),&bit);
				bx = bit.bmWidth;
				by = bit.bmHeight;
				BitBlt(hdc,menuPos[3].left,menuPos[3].top,menuPos[3].right,menuPos[3].bottom,sorDC,0,0,SRCCOPY);
			}

			if(buttonclick[4].bIsClick)
			{
				SelectObject(sorDC,hIcon_Click);
				GetObject(hIcon_Click,sizeof(BITMAP),&bit);			
				BitBlt(hdc,menuPos[4].left,menuPos[4].top,menuPos[4].right,menuPos[4].bottom,sorDC,0,0,SRCCOPY);
			}
			else
			{
				SelectObject(sorDC, hIcon);
				GetObject(hIcon,sizeof(BITMAP),&bit);
				bx = bit.bmWidth;
				by = bit.bmHeight;
				BitBlt(hdc,menuPos[4].left,menuPos[4].top,menuPos[4].right,menuPos[4].bottom,sorDC,0,0,SRCCOPY);
			}
			
			wsprintf(szClanUser1,"%s","별들의전쟁");
			dsTextLineOut(hdc,menuPos[6].left,menuPos[6].top,szClanUser1,lstrlen(szClanUser1));		
			wsprintf(szClanUser2,"%s","아처의나라");
			dsTextLineOut(hdc,menuPos[7].left,menuPos[7].top,szClanUser2,lstrlen(szClanUser2));		
			wsprintf(szClanUser3,"%s","절대신");
			dsTextLineOut(hdc,menuPos[8].left,menuPos[8].top,szClanUser3,lstrlen(szClanUser3));		
			wsprintf(szClanUser4,"%s","강토");
			dsTextLineOut(hdc,menuPos[9].left,menuPos[9].top,szClanUser4,lstrlen(szClanUser4));
			wsprintf(szClanUser5,"%s","러쉬");
			dsTextLineOut(hdc,menuPos[10].left,menuPos[10].top,szClanUser5,lstrlen(szClanUser5));

			if(menuMouseOver[5] >0)
			{
				SelectObject(sorDC,hOk_Yellow);
				GetObject(hOk_Yellow,sizeof(BITMAP),&bit);
				bx = bit.bmWidth;
				by = bit.bmHeight;		
				BitBlt(hdc,menuPos[5].left,menuPos[5].top,menuPos[5].right,menuPos[5].bottom,sorDC,0,0,SRCCOPY);
			}
			else
			{
				SelectObject(sorDC,hOk_Gray);
				GetObject(hOk_Gray,sizeof(BITMAP),&bit);
				bx = bit.bmWidth;
				by = bit.bmHeight;		
				BitBlt(hdc,menuPos[5].left,menuPos[5].top,menuPos[5].right,menuPos[5].bottom,sorDC,0,0,SRCCOPY);
			}

			SelectObject(sorDC,hButton_Box);
			GetObject(hButton_Box,sizeof(BITMAP),&bit);
			bx = bit.bmWidth;
			by = bit.bmHeight;		
			BitBlt(hdc,menuPos[11].left,menuPos[11].top,menuPos[11].right,menuPos[11].bottom,sorDC,0,0,SRCCOPY);

			
			DeleteDC(sorDC);
			ReleaseDC(g_hwnd, hdc );
			break;
	*/

		case CLAN_MARK1:
			if(m_hClanName)
			{
				ShowWindow(m_hClanName,SW_HIDE);
			}
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

			wsprintf(string,"%s",szMark1 );			
			
			Text_Out(destdc,menuPos[1].left,menuPos[1].top,string);

			
			SelectObject(sorDC, hTxt_Mark[1]);

			GetObject(hTxt_Mark[1],sizeof(BITMAP),&bit);
			bx = bit.bmWidth;
			by = bit.bmHeight;
					
			BitBlt(destdc,menuPos[3].left,menuPos[3].top,menuPos[3].right,menuPos[3].bottom,sorDC,0,0,SRCCOPY);

			DeleteDC(sorDC);
			DeleteDC(destdc);
			ReleaseDC(g_hwnd, hdc );
			break;
		case CLAN_MARK2:
			hdc = GetDC(g_hwnd);
			sorDC  = CreateCompatibleDC(hdc);

			destdc=CreateCompatibleDC(hdc);

			SelectObject(destdc, Dback.hb );		//off스크린설정.

			SelectObject(sorDC, hButton_Box);
			GetObject(hButton_Box,sizeof(BITMAP),&bit);
			bx = bit.bmWidth;
			by = bit.bmHeight;
			BitBlt(destdc,menuPos[4].left,menuPos[4].top,menuPos[4].right,menuPos[4].bottom,sorDC,0,0,SRCCOPY);

			SelectObject(sorDC, hButton_Box);
			GetObject(hButton_Box,sizeof(BITMAP),&bit);
			bx = bit.bmWidth;
			by = bit.bmHeight;
			BitBlt(destdc,menuPos[5].left,menuPos[5].top,menuPos[5].right,menuPos[5].bottom,sorDC,0,0,SRCCOPY);


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
			
			if(menuMouseOver[1] > 0)
			{
				SelectObject(sorDC, hCancel_Yellow);
				GetObject(hCancel_Yellow,sizeof(BITMAP),&bit);
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
			

			
			SelectObject(sorDC, hClinetMark);
			GetObject(hClinetMark,sizeof(BITMAP),&bit);
			bx = bit.bmWidth;
			by = bit.bmHeight;

			BitBlt(destdc,menuPos[2].left,menuPos[2].top,menuPos[2].right,menuPos[2].bottom,sorDC,0,0,SRCCOPY);
			
			wsprintf(string,"%s",szMark2 );			
			Text_Out(destdc,menuPos[3].left,menuPos[3].top, string);

			

			SelectObject(sorDC, hTxt_Mark[1]);

			GetObject(hTxt_Mark[1],sizeof(BITMAP),&bit);
			bx = bit.bmWidth;
			by = bit.bmHeight;
					
			BitBlt(destdc,menuPos[6].left,menuPos[6].top,menuPos[6].right,menuPos[6].bottom,sorDC,0,0,SRCCOPY);

			DeleteDC(sorDC);
			DeleteDC(destdc);
			ReleaseDC(g_hwnd, hdc );

			break;
		case CLAN_MARK3:
			hdc = GetDC(g_hwnd);
			sorDC  = CreateCompatibleDC(hdc);
			destdc=CreateCompatibleDC(hdc);

			SelectObject(destdc, Dback.hb );		//off스크린설정.

			SelectObject(sorDC, hButton_Box);
			GetObject(hButton_Box,sizeof(BITMAP),&bit);
			bx = bit.bmWidth;
			by = bit.bmHeight;
			BitBlt(destdc,menuPos[5].left,menuPos[5].top,menuPos[5].right,menuPos[5].bottom,sorDC,0,0,SRCCOPY);

			SelectObject(sorDC, hButton_Box);
			GetObject(hButton_Box,sizeof(BITMAP),&bit);
			bx = bit.bmWidth;
			by = bit.bmHeight;
			BitBlt(destdc,menuPos[6].left,menuPos[6].top,menuPos[6].right,menuPos[6].bottom,sorDC,0,0,SRCCOPY);
				
			SelectObject(sorDC, hOk_Gray);

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
			
			if(menuMouseOver[1] > 0)
			{
				SelectObject(sorDC, hCancel_Yellow);
				GetObject(hCancel_Yellow,sizeof(BITMAP),&bit);
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
			
			SelectObject(sorDC, hClinetMark);
			GetObject(hClinetMark,sizeof(BITMAP),&bit);
			bx = bit.bmWidth;
			by = bit.bmHeight;

			BitBlt(destdc,menuPos[2].left,menuPos[2].top,menuPos[2].right,menuPos[2].bottom,sorDC,0,0,SRCCOPY);

			SelectObject(sorDC, hServerMark);
			GetObject(hServerMark,sizeof(BITMAP),&bit);
			bx = bit.bmWidth;
			by = bit.bmHeight;

			BitBlt(destdc,menuPos[3].left,menuPos[3].top,menuPos[3].right,menuPos[3].bottom,sorDC,0,0,SRCCOPY);
			
			wsprintf(string,"%s",szMark2 );
			//DrawText(GetDC(g_hwnd),string,-1,&rect,DT_LEFT | DT_WORDBREAK);
			Text_Out(destdc,menuPos[4].left,menuPos[4].top,string);
			
			SelectObject(sorDC, hTxt_Mark[1]);

			GetObject(hTxt_Mark[1],sizeof(BITMAP),&bit);
			bx = bit.bmWidth;
			by = bit.bmHeight;					
			BitBlt(destdc,menuPos[7].left,menuPos[7].top,menuPos[7].right,menuPos[7].bottom,sorDC,0,0,SRCCOPY);

			DeleteDC(sorDC);
			DeleteDC(destdc);
			ReleaseDC(g_hwnd, hdc );
			break;

#ifndef _LANGUAGE_BRAZIL	//브라질 클랜 //해외
		case CLAN_RELEASE:
			hdc = GetDC(g_hwnd);
			sorDC  = CreateCompatibleDC(hdc);
			destdc  = CreateCompatibleDC(hdc);
			SelectObject(destdc, Dback.hb );		//off스크린설정.

			SelectObject(sorDC, hButton_Box);
			GetObject(hButton_Box,sizeof(BITMAP),&bit);
			bx = bit.bmWidth;
			by = bit.bmHeight;
			BitBlt(destdc,menuPos[3].left,menuPos[3].top,menuPos[3].right,menuPos[3].bottom,sorDC,0,0,SRCCOPY);

			SelectObject(sorDC, hButton_Box); 
			GetObject(hButton_Box,sizeof(BITMAP),&bit);
			bx = bit.bmWidth;
			by = bit.bmHeight;
			BitBlt(destdc,menuPos[4].left,menuPos[4].top,menuPos[4].right,menuPos[4].bottom,sorDC,0,0,SRCCOPY);

			if(menuMouseOver[0] > 0)
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
			if(menuMouseOver[1] > 0)
			{
				SelectObject(sorDC, hNo_Yellow);
				GetObject(hNo_Yellow,sizeof(BITMAP),&bit);
				bx = bit.bmWidth;
				by = bit.bmHeight;
				BitBlt(destdc,menuPos[1].left,menuPos[1].top,menuPos[1].right,menuPos[1].bottom,sorDC,0,0,SRCCOPY);
			}
			else
			{
				SelectObject(sorDC, hNo_Gray);
				GetObject(hNo_Gray,sizeof(BITMAP),&bit);
				bx = bit.bmWidth;
				by = bit.bmHeight;
				BitBlt(destdc,menuPos[1].left,menuPos[1].top,menuPos[1].right,menuPos[1].bottom,sorDC,0,0,SRCCOPY);
			}		

			
			wsprintf(string,"%s",szMsg15 );			
			//DrawText(GetDC(g_hwnd),string,-1,&rect,DT_LEFT | DT_WORDBREAK);
			//Draw_Text(menuPos[2].left,menuPos[2].top ,string);
			Text_Out(destdc,menuPos[2].left,menuPos[2].top ,clanMsg15.szMsg);
			
			
			SelectObject(sorDC, hTxt_Release[0]); 
			GetObject(hTxt_Release[1],sizeof(BITMAP),&bit);
			bx = bit.bmWidth;
			by = bit.bmHeight;
			BitBlt(destdc,menuPos[5].left,menuPos[5].top,menuPos[5].right,menuPos[5].bottom,sorDC,0,0,SRCCOPY);

			
			DeleteDC(sorDC);
			DeleteDC(destdc);
			ReleaseDC(g_hwnd, hdc );

			break;
		case CLAN_RECONF_RELEASE:
			hdc = GetDC(g_hwnd);
			sorDC  = CreateCompatibleDC(hdc);
			destdc  = CreateCompatibleDC(hdc);
			SelectObject(destdc, Dback.hb );		//off스크린설정.

			SelectObject(sorDC, hButton_Box);
			GetObject(hButton_Box,sizeof(BITMAP),&bit);
			bx = bit.bmWidth;
			by = bit.bmHeight;
			BitBlt(destdc,menuPos[3].left,menuPos[3].top,menuPos[3].right,menuPos[3].bottom,sorDC,0,0,SRCCOPY);

			SelectObject(sorDC, hButton_Box); 
			GetObject(hButton_Box,sizeof(BITMAP),&bit);
			bx = bit.bmWidth;
			by = bit.bmHeight;
			BitBlt(destdc,menuPos[4].left,menuPos[4].top,menuPos[4].right,menuPos[4].bottom,sorDC,0,0,SRCCOPY);

			if(menuMouseOver[0] > 0)
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
			if(menuMouseOver[1] > 0)
			{
				SelectObject(sorDC, hNo_Yellow);
				GetObject(hNo_Yellow,sizeof(BITMAP),&bit);
				bx = bit.bmWidth;
				by = bit.bmHeight;
				BitBlt(destdc,menuPos[1].left,menuPos[1].top,menuPos[1].right,menuPos[1].bottom,sorDC,0,0,SRCCOPY);
			}
			else
			{
				SelectObject(sorDC, hNo_Gray);
				GetObject(hNo_Gray,sizeof(BITMAP),&bit);
				bx = bit.bmWidth;
				by = bit.bmHeight;
				BitBlt(destdc,menuPos[1].left,menuPos[1].top,menuPos[1].right,menuPos[1].bottom,sorDC,0,0,SRCCOPY);
			}		

			
			//wsprintf(string,"%s",szReConfReleaseMsg );
			
			//DrawText(GetDC(g_hwnd),szReconfReleaseMsg,-1,&rect,DT_LEFT | DT_WORDBREAK);
			//Draw_Text(menuPos[2].left,menuPos[2].top,szReconfReleaseMsg);
			Text_Out(destdc,menuPos[2].left,menuPos[2].top,szReconfReleaseMsg);
			
			
			SelectObject(sorDC, hTxt_Release[0]); 
			GetObject(hTxt_Release[1],sizeof(BITMAP),&bit);
			bx = bit.bmWidth;
			by = bit.bmHeight;
			BitBlt(destdc,menuPos[5].left,menuPos[5].top,menuPos[5].right,menuPos[5].bottom,sorDC,0,0,SRCCOPY);

			
			DeleteDC(sorDC);
			DeleteDC(destdc);
			ReleaseDC(g_hwnd, hdc );

			break;
		case CLAN_FINISH_RELEASE:
			hdc = GetDC(g_hwnd);
			sorDC  = CreateCompatibleDC(hdc);
			destdc  = CreateCompatibleDC(hdc);
			SelectObject(destdc, Dback.hb );		//off스크린설정.

			SelectObject(sorDC, hButton_Box);
			GetObject(hButton_Box,sizeof(BITMAP),&bit);
			bx = bit.bmWidth;
			by = bit.bmHeight;
			BitBlt(destdc,menuPos[1].left,menuPos[1].top,menuPos[1].right,menuPos[1].bottom,sorDC,0,0,SRCCOPY);

			//SelectObject(sorDC,hClanRelease[1]);
			//GetObject(hClanRelease[1],sizeof(BITMAP),&bit);
			//bx = bit.bmWidth;
			//by = bit.bmHeight;		
			//BitBlt(destdc,menuPos[2].left-31,menuPos[2].top,menuPos[2].right,menuPos[2].bottom,sorDC,0,0,SRCCOPY);


			if(menuMouseOver[0] > 0)
			{
				SelectObject(sorDC, hOk_Yellow);

				GetObject(hYes_Yellow,sizeof(BITMAP),&bit);
				bx = bit.bmWidth;
				by = bit.bmHeight;
						
				BitBlt(destdc,menuPos[0].left,menuPos[0].top,menuPos[0].right,menuPos[0].bottom,sorDC,0,0,SRCCOPY);
			}
			else
			{
				SelectObject(sorDC, hOk_Gray);

				GetObject(hYes_Gray,sizeof(BITMAP),&bit);
				bx = bit.bmWidth;
				by = bit.bmHeight;		
				
				BitBlt(destdc,menuPos[0].left,menuPos[0].top,menuPos[0].right,menuPos[0].bottom,sorDC,0,0,SRCCOPY);
			}

			
			SelectObject(sorDC, hTxt_Release[0]); 
			GetObject(hTxt_Release[1],sizeof(BITMAP),&bit);
			bx = bit.bmWidth;
			by = bit.bmHeight;
			BitBlt(destdc,menuPos[2].left,menuPos[2].top,menuPos[2].right,menuPos[2].bottom,sorDC,0,0,SRCCOPY);
			
			//Draw_Text(menuPos[3].left,menuPos[3].top,szFinishReleaseMsg);
			Text_Out(destdc,menuPos[3].left,menuPos[3].top,clanFinishReleaseMsg.szMsg);

			DeleteDC(sorDC);
			DeleteDC(destdc);
			ReleaseDC(g_hwnd, hdc );
			break;
#endif

		case CLAN_HELP:
			hdc = GetDC(g_hwnd);
			sorDC  = CreateCompatibleDC(hdc);
			destdc  = CreateCompatibleDC(hdc);
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

					
					//SelectObject(sorDC, hHelp[1]);
					//GetObject(hHelp[1],sizeof(BITMAP),&bit);
					//bx = bit.bmWidth;
					//by = bit.bmHeight;
					//BitBlt(destdc,menuPos[nIconPos].left-31,menuPos[nIconPos].top,menuPos[nIconPos].right,menuPos[nIconPos].bottom,sorDC,0,0,SRCCOPY);

					//SelectObject(sorDC, hClanHelpIcon[i+nScl_Bar][1]);
					//GetObject(hClanHelpIcon[i+nScl_Bar][1],sizeof(BITMAP),&bit);
					//bx = bit.bmWidth;
					//by = bit.bmHeight;
					//BitBlt(destdc,menuPos[nIconPos].right+1,menuPos[nIconPos].top,menuPos[nIconPos].right,menuPos[nIconPos].bottom,sorDC,0,0,SRCCOPY);
				}
				else
				{
					//SelectObject(sorDC, hHelp[0]);
					//GetObject(hHelp[0],sizeof(BITMAP),&bit);
					//bx = bit.bmWidth;
					//by = bit.bmHeight;
					//BitBlt(destdc,menuPos[nIconPos].left-31,menuPos[nIconPos].top,menuPos[nIconPos].right,menuPos[nIconPos].bottom,sorDC,0,0,SRCCOPY);

					SelectObject(sorDC, hClanHelpIcon[i+nScl_Bar][1]);
					GetObject(hClanHelpIcon[i+nScl_Bar][1],sizeof(BITMAP),&bit);
					bx = bit.bmWidth;
					by = bit.bmHeight;
					BitBlt(destdc,menuPos[nIconPos].left,menuPos[nIconPos].top,menuPos[nIconPos].right,menuPos[nIconPos].bottom,sorDC,0,0,SRCCOPY);

					//SelectObject(sorDC, hClanHelpIcon[i+nScl_Bar][2]);
					//GetObject(hClanHelpIcon[i+nScl_Bar][0],sizeof(BITMAP),&bit);
					//bx = bit.bmWidth;
					//by = bit.bmHeight;				
					//BitBlt(destdc,menuPos[nIconPos].left,menuPos[nIconPos].top,menuPos[nIconPos].right,menuPos[nIconPos].bottom,sorDC,0,0,SRCCOPY);
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


				/*SelectObject(sorDC,hScl_icon);
				GetObject(hScl_icon,sizeof(BITMAP),&bit);
				BitBlt(hdc,menuPos[nIconPos].left,menuPos[nIconPos].top,menuPos[nIconPos].right,menuPos[nIconPos].bottom,sorDC,0,0,SRCCOPY);
				nIconPos+=1;

				SelectObject(sorDC,hScl_icon);
				GetObject(hScl_icon,sizeof(BITMAP),&bit);			
				BitBlt(hdc,menuPos[nIconPos].left,menuPos[nIconPos].top,menuPos[nIconPos].right,menuPos[nIconPos].bottom,sorDC,0,0,SRCCOPY);
				*/
			}
			nIconPos+=1;
			SelectObject(sorDC,hTxt_Help[0]);
			GetObject(hTxt_Help[0],sizeof(BITMAP),&bit);		
			BitBlt(destdc,menuPos[nIconPos].left,menuPos[nIconPos].top,menuPos[nIconPos].right,menuPos[nIconPos].bottom,sorDC,0,0,SRCCOPY);

			

			DeleteDC(destdc);
			DeleteDC(sorDC);
			ReleaseDC(g_hwnd, hdc );

			break;	
		case CLAN_HELP_CONTENT: //도움말 내용 디스플레이
			hdc = GetDC(g_hwnd);
			sorDC  = CreateCompatibleDC(hdc);
			destdc  = CreateCompatibleDC(hdc);
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
			
			
			//SelectObject(sorDC, hClanHelpIcon[g_HelpIconNumber][2]);
			//GetObject(hClanHelpIcon[g_HelpIconNumber][2],sizeof(BITMAP),&bit);
			//bx = bit.bmWidth;
			//by = bit.bmHeight;		
			//BitBlt(destdc,menuPos[2].left,menuPos[2].top,menuPos[2].right,menuPos[2].bottom,sorDC,0,0,SRCCOPY);

			

			SelectObject(sorDC,hTxt_Help[0]);
			GetObject(hTxt_Help[0],sizeof(BITMAP),&bit);		
			bx = bit.bmWidth;
			by = bit.bmHeight;		
			BitBlt(destdc,menuPos[2].left,menuPos[2].top,menuPos[2].right,menuPos[2].bottom,sorDC,0,0,SRCCOPY);

			SelectObject(sorDC,hBt_Help);
			GetObject(hBt_Help,sizeof(BITMAP),&bit);		
			bx = bit.bmWidth;
			by = bit.bmHeight;		
			BitBlt(destdc,menuPos[3].left,menuPos[3].top,menuPos[3].right,menuPos[3].bottom,sorDC,0,0,SRCCOPY);
			
			//SelectObject(sorDC,hHelp[1]);
			//GetObject(hTxt_Help[1],sizeof(BITMAP),&bit);		
			//bx = bit.bmWidth;
			//by = bit.bmHeight;		
			//BitBlt(destdc,menuPos[2].right+1,menuPos[2].top,menuPos[2].right,menuPos[2].bottom,sorDC,0,0,SRCCOPY);

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
				nIconPos+=1;

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
		case CLANCHIP_MAKE_FIRSTMESSAGE: 
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
			//wsprintf(szDrawMenuMsg,szMakeFinishClan,cldata.name);
			Text_Out(destdc,menuPos[1].left,menuPos[1].top,szDrawMenuMsg);

			DeleteDC(sorDC);
			DeleteDC(destdc);
			ReleaseDC(g_hwnd, hdc );
			break;
		case CLANCHIP_DOWN6_CLANWON_MESSAGE:
		case CLANCHIP_DOWN6_CLANWON:
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
			if(cldata. bIsOneDay){			
				wsprintf(szTemp,"\275\303\260\243 : %d : %d \263\262\276\322\275\300\264\317\264\331",cldata.nMinute,cldata.nHour);
				wsprintf(szDrawMenuMsg,szClanWon6Down,szTemp);
				Text_Out(destdc,menuPos[1].left,menuPos[1].top,szDrawMenuMsg);
			}
			else{
				wsprintf(szTemp,"\263\257\302\245 : %d\300\317 \263\262\276\322\275\300\264\317\264\331",cldata.nLastDay);
				wsprintf(szDrawMenuMsg,szClanWon6Down,szTemp);
				Text_Out(destdc,menuPos[1].left,menuPos[1].top,szDrawMenuMsg);
			}
			//Text_Out(destdc,menuPos[1].left,menuPos[1].top,szClanWon6Down);

			DeleteDC(sorDC);
			DeleteDC(destdc);
			ReleaseDC(g_hwnd, hdc );
			break;
		case CLAN_NOMAKE: //클랜이 만들어 지지 않았다
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
			//wsprintf(szDrawMenuMsg,szMsg6,cldata.nLastDay);
			Text_Out(destdc,menuPos[1].left,menuPos[1].top,szDrawMenuMsg);

			DeleteDC(sorDC);
			DeleteDC(destdc);
			ReleaseDC(g_hwnd, hdc );
			break;

		case CLANCHIP_SUBCHIP_LOAD_DATA:
		case CLANCHIP_SUBCHIP_READMSG_FLAG:		//클랜장이 부침관련메세지를 읽어다는 플래그세팅
		//case CLANUSER_SUBCHIP_READMSG_FLAG:		//클랜유저가 부침관련 메세지를 읽어다는 플래그 세팅
		case CLANCHIP_SUBCHIP_APPOINT_FLAG:    //클랜부침으로 임명관련 플래그 세팅 
		case CLANCHIP_SUBCHIP_RELEASE_FLAG:	//클랜부침이 해임되어다는 플래그세팅
		case	CLANCHIP_APPOINTSUBCHIP_DO:	    //클랜부칩 임명 ASP호출
		case CLANCHIP_RELEASESUBCHIP_DO:	    //클랜부칩 해임 ASP호출
		case CLANCHIP_LEAVECLAN_PFLAG:
		case CLANCHIP_LEAVECLAN_DO:
		case CLAN_REMOVE_SHORTCLANWON_LOADING:
		case CLAN_REMOVE_LOADING:
		case CLAN_RELEASE_LOADING:
		case ISPFLAG_CHIP:
		case CLAN_CHIP_ReadDATA:
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
    }
#endif
}


