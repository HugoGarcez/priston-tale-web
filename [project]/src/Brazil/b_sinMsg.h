/*----------------------------------------------------------------------------*
*	파일툈E:  sinMsg.cpp
*	하는일 :  다른언엉洑 컨버젼을 위해 메세지를 컖E?磯?
*	작성일 :  최종업데이트 4퓖E
*	적성자 :  박상열
*-----------------------------------------------------------------------------*/

#include "sinLinkHeader.h"

sMESSAGEBOX	sMessageBox[100] = {
	{"Espa\347o insuficiente"},  // Espaço insuficiente
	{"Excede o Peso limite"},
	{"Este item n\343o pode ser usado"},  // Este item não pode ser usado
	{"Ouro insuficiente"},
	{"Este item n\343o pode ser Mixado"},  // Este item não pode ser Mixado
	{"Excede o n\372mero M\341x. de itens"},  // Excede o número Máx. de itens
	{"Troca cancelada"},
	{"Po\347\365es n\343o podem ser armazenadas"},  // Poções não podem ser armazenadas
	{"A Mixagem do Item falhou"},
	{"Obrigado pela sua contribui\347\343o"},  // Obrigado pela sua contribuição
	{"Muitos Itens no Armaz\351m"},  // Muitos Itens no Armazém
	{"Este item n\343o pode ser usado no Aging"},  // Este item não pode ser usado no Aging
	{"A troca foi alterada, confira"},
	{"Excede o limite de Ouro"},
	{"O Aging falhou"},
	{"Status reiniciado"},
	{"A imagem do item n\343o est\341 pronta"},  // A imagem do item não está pronta
	{"Voc\352 s\363 pode reiniciar uma vez"},  // Você só pode reiniciar uma vez
	{"Somente para  membros de Clan"},
	{"Somente para  membros de Clan"},
	{"N\355vel insuficiente"},  // Nível insuficiente
	{"O N\355vel do Aging caiu -1"},  // O Nível do Aging caiu -1
	{"O N\355vel do Aging caiu -2"},  // O Nível do Aging caiu -2
	{"O N\355vel do Aging subiu +1"},  // O Nível do Aging subiu +1
	{"O N\355vel do Aging subiu +2"},  // O Nível do Aging subiu +2
	{"A Vel. de ATQ do Item foi reduzida"},
	{"   Mature o Item do Desafio    "},
	{"   HP +15"},
	{"   Quebra-cabe\347as completo    "},  //    Quebra-cabeças completo    
	{"   Quebra-cabe\347as incorreto    "},  //    Quebra-cabeças incorreto    
	{"   Nenhum item para vender  "},
	{"O item j\341 foi vendido"},  // O item já foi vendido
	{"A loja pessoal do jogador j\341 fechou"},  // A loja pessoal do jogador já fechou
	{"   Vendedor n\343o encontrado   "},  //    Vendedor não encontrado   
	{"    N\372mero de itens incorreto     "},  //     Número de itens incorreto     
	{"    Guerreiros n\343o podem usar este item    "},  //     Guerreiros não podem usar este item    
	{"    Guerreiras n\343o podem usar este item    "},  //     Guerreiras não podem usar este item    
	{" Guerreiras n\343o podem apanhar este item "},  //  Guerreiras não podem apanhar este item 
	{" Guerreiros n\343o podem apanhar este item "},  //  Guerreiros não podem apanhar este item 
	{"    O pre\347o do item foi alterado  "},  //     O preço do item foi alterado  
	{"    Nenhuma recompensa para o Clan    "},
	{"  Voc\352 n\343o est\341 acima do N\355vel limite  "},  //   Você não está acima do Nível limite  
	{"     A For\347a Orb est\341 sendo usada agora    "},  //      A Força Orb está sendo usada agora    
	{"     A fun\347\343o do Poder de Ataque est\341 em teste    "},  //      A função do Poder de Ataque está em teste    
	{"    A matura\347\343o da For\347a Orb est\341 completa    "},  //     A maturação da Força Orb está completa    
	{"    +5 Pontos de Status    "},
	{"    +1 Ponto de Habilidade    "},
	{" +1 Ponto de Habilidade, +5 Pontos de Status"},
	{" +2 Pontos de Habilidade, +5 Pontos de Status"},
	{"   Penalidade pela morte reduzida em 1%  "},
	{"   Vitalidade +40   " },
	{"    Voc\352 j\341 est\341 sob Desafio     " },  //     Você já está sob Desafio     
	{"Ap\363s o N\355vel 80, voc\352 ganhar\341 7 pontos por N\355vel." },  // Após o Nível 80, você ganhará 7 pontos por Nível.
	{"Ap\363s o N\355vel 90, voc\352 ganhar\341 10 pontos por N\355vel." },  // Após o Nível 90, você ganhará 10 pontos por Nível.
	{"Status reiniciado" },
	{"Miss\343o incompleta pelo tempo" },  // Missão incompleta pelo tempo
	{"Muitos soldados mercen\341rios"},  // Muitos soldados mercenários
	{"Est\341 sendo usada nas Habilidades"},  // Está sendo usada nas Habilidades
	{"Transporte imposs\355vel. Por favor, chame um GM"},  // Transporte impossível. Por favor, chame um GM
	{"Este Item n\343o pode ser vendido"},  // Este Item não pode ser vendido
	{"N\343o h\341 taxas suficientes"},  // Não há taxas suficientes
	{"Voc\352 j\341 respondeu a estas perguntas"},  // Você já respondeu a estas perguntas
	{"Obrigado por responder \340s perguntas"},  // Obrigado por responder às perguntas
	{"O item \351 incompat\355vel com seu N\355vel"},  // O item é incompatível com seu Nível
	{"O item foi reconstru\355do"},  // O item foi reconstruído
	{"Encontrado no confronto contra o F\372ria"},  // Encontrado no confronto contra o Fúria
	{"Perdido no confronto contra o F\372ria"},  // Perdido no confronto contra o Fúria
	{"Itens de compensa\347\343o"},  // Itens de compensação
	{"Itens aliados n\343o podem ser combinados"},  // Itens aliados não podem ser combinados
	{"Poder\341 ser usado futuramente"},  // Poderá ser usado futuramente
	{"Imposs\355vel cancelar enquanto sheltoms s\343o usados"},  // Impossível cancelar enquanto sheltoms são usados
	{"Imposs\355vel carregar arma enquanto sheltoms s\343o usados"},  // Impossível carregar arma enquanto sheltoms são usados
	{"Tipo de po\347\343o repetido"},  // Tipo de poção repetido
};


sMESSAGEBOX2 sMessageBox2[20] = {
	{"Quantos itens","voc\352 deseja comprar?"},  // você deseja comprar?
	{"Quantos itens","voc\352 deseja vender?"},  // você deseja vender?
	{"Quantos itens","voc\352 deseja abandonar?"},  // você deseja abandonar?
	{"Quanto dinheiro","voc\352 deseja abandonar?"},  // você deseja abandonar?
	{"",""},
	{"Quanto dinheiro"," voc\352 deseja trocar?"},  //  você deseja trocar?
	{"Quanto dinheiro","voc\352 deseja transferir?"},  // você deseja transferir?
	{"Quanto dinheiro","voc\352 deseja retirar?"},  // você deseja retirar?
	{" Quantas estrelas ","voc\352 deseja comprar?"},  // você deseja comprar?
	{"  Quanto dinheiro  ","voc\352 deseja doar?"},  // você deseja doar?
	{"Voc\352 deseja registrar","o pre\347o de seu item? "},  // Você deseja registrar | o preço de seu item? 
	{"Quantos itens","voc\352 deseja comprar?"},  //개인상점 포션  // você deseja comprar?
	{"Quanto dinheiro","voc\352 deseja retirar?"},  // você deseja retirar?
	{"  Voc\352 deseja ","comprar o item?"},  //   Você deseja 
	{"  Voc\352 deseja ","comprar o item?"}, //14번째 요넘만 진짜  //   Você deseja 
	{"Quanto dinheiro","voc\352 deseja investir?"},//공성전 돈찾기  // você deseja investir?
	{"Quantos mercen\341rios ","voc\352 deseja contratar?"} //용병 구입  // Quantos mercenários  | você deseja contratar?

};


sMESSAGEBOX3 sMessageBox3[MESSAGE3_MAX] = {
	{"Voc\352 deseja","adquirir as Habilidades?"},  // Você deseja
	{"Deseja", "acabar o Desafio de classe?"}, //요건 템스크론
	{"Voc\352 aceita", "o Desafio?"},  // Você aceita
	{"Voc\352 deseja","redistribuir?"},  // Você deseja
	{"O evento"," custa"},
	{"Deseja","reiniciar seus STATUS?"},
	{"Voc\352 aceita", "o Desafio?"}, //요건 모라이온  // Você aceita
	{"Voc\352 aceita", "o item?"}, //PostBox  // Você aceita
	{"Nomear o", "Core de Uni\343o?"}, //LinkCore  // Core de União?
	{"Voc\352 aceita", "o item?"}, //RecvOthers  // Você aceita
	{"Mover-se ao lugar", "indicado ?"}, //RecvOthers
	{"","Por favor, digite o n\372mero"}, //RecvOthers  // Por favor, digite o número
	{"Deseja","comprar as Estrelas?"}, //RecvOthers
	{"Deseja", "vender o item?"}, //아이템 팔기
	{"Voc\352 aceita", "o Desafio?"}, //요건 3차전업  // Você aceita
	{" Deseja", "mesmo doar?"}, //아이템 팔기
	{" Loja pessoal ", "fffffffffff"}, //아이템 팔기
	{"Deseja", "trocar os itens? "}, //퍼즐로 아이템교환
	{"Deseja", "comprar o item?"}, //개인상점 아이템사기
	{"Deseja ", "cancelar o registro?"}, //개인상점 아이템 등록 취소
	{"Deseja", "trocar o item? "}, //바벨의뿔 아이템으로 교환하기
	{"Deseja", "trocar o item? "}, //구미호 아이템으로 교환하기
	{"Voc\352 aceita", "o Desafio?"}, //Level30  // Você aceita
	{"Voc\352 aceita", "o Desafio?"}, //Level55  // Você aceita
	{"Voc\352 aceita", "o Desafio?"}, //Level55_2  // Você aceita
	{"Voc\352 aceita", "o Desafio?"}, //Level70  // Você aceita
	{"Voc\352 aceita", "o Desafio?"}, //Level80  // Você aceita
	{"Voc\352 aceita", "o Desafio?"}, //Level85  // Você aceita
	{"Voc\352 aceita", "o Desafio?"}, //Level90  // Você aceita
	{"Para onde", "voc\352 deseja ir?"}, //텔레포트  // você deseja ir?
	{"Voc\352 aceita", "o Desafio?"}, //Level80  // Você aceita
	{"Voc\352 aceita", "o Desafio?"}, //Level90_2  // Você aceita
	{"Voc\352 aceita", "o Desafio?"}, //4Â÷ÀüÁ÷  // Você aceita
	{"Voc\352 aceita", "o Desafio de Classe?"}, //4Â÷ ÀüÁ÷  // Você aceita
	{"Deseja", "trocar o item? "},//일곱개의 크리스탈을 아이템으로 교환하기
	{"Deseja ser transportado", "a algum lugar?"},    //Teleport
	{"Gostaria de comprar","uma Torre de Cristais?"},      //크리스탈 타워를구입하다.
	{"Voc\352 aceita", "o Desafio?"}, //A  // Você aceita
	{"Voc\352 aceita", "o Desafio?"}, //B  // Você aceita
	{"Voc\352 aceita", "o Desafio?"}, //C  // Você aceita
	{"Voc\352 aceita", "o Desafio?"}, //D  // Você aceita
	{"Voc\352 aceita", "o Desafio?"}, //E  // Você aceita
	{"Voc\352 aceita", "o Desafio?"}, //F  // Você aceita
	{"Voc\352 aceita", "o Desafio?"}, //G  // Você aceita
	{"Deseja", "usar este item"},       //구매 아이템 사용
	{"Voc\352 aceita", "o Desafio?"},//100LV퀘스트 수해여부  // Você aceita
	{"Voc\352 aceita", "receber o item?"},   //100LV아이템  // Você aceita
	{"Deseja","ir para o Campo de Batalha?"},   //100LV퀘스트 대련장 이동여부
	{"Deseja", "Teletransportar-se?"},
};


char *SkillMaster[4] = {
	"Aprender Habilidade",
	"Mudar de Classe",
	"Sair do jogo"

};
char *SkillMaster2[4] = {
	"",
	"",
	""

};

char *ReStartMsg[4] = {
	" Recome\347ar no Campo de Batalha",  //  Recomeçar no Campo de Batalha
	" Recome\347ar na Cidade",  //  Recomeçar na Cidade
	" Sair do jogo",
	" EXP. insuficiente"

};
char *ReStartMsg2[4] = {
	" Penalidade(Ouro/EXP.) 1%",
	"",
	"",
	" para recome\347ar no Campo de Batalha",  //  para recomeçar no Campo de Batalha

};

char *SkillUseInfoText[10]= { "    Treinar Habilidade/Magia     "," Selecione um Habilidade     ","      Para trein\341-la ou aprend\352-la       ",  //       Para treiná-la ou aprendê-la       
				"Pontos de Habilidade  ","   insuficientes " };
char *sinMsgSkillPoint = "    Pontos de Habilidade : %d";
char *sinMsgEliteSkillPoint = "Pontos Especiais : %d " ;
char *sinStateName[5]={"H P : %d/%d  ","M P : %d/%d  ","R E S : %d/%d  ","E X P : %d/%d  ","E X P : %dM/%dM  "};


char *TownName[3] = {"Voltar para Ricarten\r","Voltar paraNavisko\r","Voltar para Pillai\r"};

char *sinAbilityName[50] = {"Poder de Ataque:  \r","Vel. da Arma:\r","Alcance:  \r","Cr\355tico:  \r","Taxa de Defesa:  \r","Taxa de Ataque:\r",  // Crítico:  \r
					"Absor\347\343o:  \r","Bloqueio:  \r","Velocidade:  \r","Integridade:\r","Regen de MP:  \r","Regen de HP :  \r",  // Absorção:  \r
					"Regen de RES:  \r","Res. Org\342nica:  \r","Nature Type:  \r","Res. a Fogo:  \r","Res. a Gelo:  \r","Res. a Raio:  \r","Res. a Veneno: \r",  // Res. Orgânica:  \r
					"Water Type:  \r","Wind Type:  \r","HP Regen:  \r","MP Regen: \r","STM Regen: \r","Add. HP: \r",
					"MP Adicional:  \r","RES Adicional:  \r","Limite de Po\347\365es:  \r","N\355vel Necess\341rio:  \r","For\347a Necess\341ria:  \r","Intelig\352ncia Necess\341ria:  \r",  // Limite de Poções:  \r | Nível Necessário:  \r | Força Necessária:  \r | Inteligência Necessária:  \r
					"Talento Necess\341rio:  \r","Agilidade Necess\341ria:  \r","Vitalidade Necess\341ria:  \r"};  // Talento Necessário:  \r | Agilidade Necessária:  \r | Vitalidade Necessária:  \r


char *sinSpecialName[50] = {"Spec. ATK SPD:  \r","Cr\355tico Adicional: \r","Def. Adicional:  \r","Abs Adicional:  \r",  // Crítico Adicional: \r
						"Bloqueio Adicional  \r","B\364nus M\341gico:  \r","Vel. Adicional:  \r","R.Org. Adicional:  \r","Spec. Nature:  \r",  // Bônus Mágico:  \r
						"R. Fogo Adicional:  \r","R. Gelo Adicional:  \r","R. Raio Adicional:\r","R. Veneno Adicional:  \r","R. \301\147ua Adicional:  \r","R. Vento Adicional:  \r",
						"P. de ATQ Adicional:  \r","Taxa de ATQ ad.:  \r","Spec. RNG:\r","ATQ Org. Adicional:  \r","Spec. Nature ATK:  \r","ATQ Fogo Adicional\r","ATQ Gelo Adicional:  \r",
						"ATQ Raio Adicional:  \r","ATQ Veneno Adicional:  \r","ATQ \301gua Adicional:  \r","ATQ Vento Adicional:  \r","B\364nus HP Max.:  \r","B\364nus MP Max.:  \r",  // ATQ Água Adicional:  \r | Bônus HP Max.:  \r | Bônus MP Max.:  \r
						"Regen de HP:  \r","Regen de MP:  \r","Regen de RES:  \r","Pre\347o de venda:  \r","Pre\347o:  \r"};  // Preço de venda:  \r | Preço:  \r


char *SpecialName3 = "%s +. \r";
char *MixResultTitle = "Resultado da Mixagem";
char *NowLevelText = "N\355vel atual\r";  // Nível atual\r
char *NextLevelText = "Pr\363ximo N\355vel\r";  // Próximo Nível\r

char *NormalAttckName ="Ataque Padr\343o";  // Ataque Padrão
char *RequirLevel = "%s (Nv. Necess.:%d)\r";
char *UseItemGroupName = "Item compat\355vel\r";  // Item compatível\r

char *PoisonName = "Res. a Veneno:\r";
char *FireName = "Elemento Fogo:\r";
char *AttackAreaName = "Alcance:\r";
char *Attack_RateName = "Taxa de Ataque:\r";
char *Max_DamageIncre = "B\364nus no dano Max.:\r";  // Bônus no dano Max.:\r
char *DamageAddName = "B\364nus no dano:\r";  // Bônus no dano:\r
char *Attack_SpeedAdd = "B\364nus na Vel. de ATQ:\r";  // Bônus na Vel. de ATQ:\r
char *IncreBlock = "B\364nus no Bloqueio:\r";  // Bônus no Bloqueio:\r
char *CountinueTime = "Dura\347\343o:\r";  // Duração:\r
char *UseManaName = "Custo de MP:\r";
char *DamageName = "Poder de Ataque:\r";
char *ShootingRangeName = "Alcance:\r";
char *IncreAsorb = "B\364nus na Absor\347\343o:\r";  // Bônus na Absorção:\r
char *SecName = "seg.";
char *PlusDamageName = "Dano Adicional:\r";
char *HitNumName = "No. de Acertos:\r";
char *DecreLifeName = "HP diminui:\r";
char *Attck_RateAdd = "Poder de ATQ Ad.:\r";
char *Add_CriticalName = "Cr\355tico Adicional:\r";  // Crítico Adicional:\r
char *Push_AreaName = "Dura\347\343o do recuo:\r";  // Duração do recuo:\r
char *FireAttackDamageAdd ="ATQ por Fogo:\r";
char *IceName = "Elemento Gelo:\r";
char *IceAttackDamageAdd = "ATQ por Gelo:\r";
char *StuneRate = "Probabilidade de Recuo:\r";
char *DefenseRateIncre = "Defesa aumenta:\r";
char *WeaponSizeName ="Tamanho da Arma:\r";
char *HwakRotationNum = "N\372mero de voltas:\r";  // Número de voltas:\r
char *WeaponSpeedAddName  = "Vel. de ATQ Ad:\r";
char *IncreAttack_RateName = "Poder de ATQ aumenta:\r";
char *ShootingNumName = "No. de disparos:\r";
char *SeriesShootingCount = "Fogo cont\355nuo:\r";  // Fogo contínuo:\r
char *UseStaminaName = "RES utilizada:\r";
char *MasterSkillMoneyName = "Pre\347o da Habilidade:\r";  // Preço da Habilidade:\r

char *ItemAgingResult;		// << Add Point

char *sinGold ="%dOuro";
char *sinGiveItem5 = "Recompensa\r";
char *sinCopyItem5 = "Item duplicado\r";
char *sinNum7 = "RND";
char *sinSkillPointName = "Ponto de Habilidade";	// << Add Point
char *sinG_Pike_Time3 = "Tempo de Efeito:\r";	// << Add Point
char *sinItemLimitTimeOverMsg = "O tempo limite acabou\r";
char *sinDeadSongPyeunEat = "Mas, que meleca! Isso n\343o presta!";  // Mas, que meleca! Isso não presta!
char *SmeltingResultTitle = "Resultado da Fundi\347\343o";		// pluto 제련
char *ManufactureResultTitle = "Resultado da Fabrica\347\343o";	// pluto 제작  // Resultado da Fabricação
char *ManufacturingTitle = "Aleat\363rio";	//fuck pluto  // Aleatório

/////////////////// 모라이온 스킬 Text추가
char *sinMaxDamageAdd = "Dano M\341x. adicional\r";  // Dano Máx. adicional\r
char *sinDamagePiercing = "(ATQ Piercing)\r";
char *sinAddStamina = "RES adicional:\r";
char *sinAddLife = "Regen de HP:\r";
//char *sinPartyHealDoc = " Em grupos, a exp. será recuperada:\ tanto quanto 1/3 do HP recuperado:\r";
char *sinDamageUndead50 = "50% a mais de dano contra mortos-vivos:\r";
char *sinNumCount2 = "n\372mero:\r";  // número:\r
char *sinNumCount3 = "qtde.\r";
char *sinDecreDamage = "Preju\355zo ao dano:\r";  // Prejuízo ao dano:\r
char *sinConvert4 = "Taxa de convers\343o:\r";  // Taxa de conversão:\r
char *sinIncreElement = "Adi\347\343o a res. elemental:\r";  // Adição a res. elemental:\r
char *sinPartyIncreElement = "Membros do grupo ganham 50% de res. elemental\r";
char *sinFireBallDamage2 = "(100% de dano ao inimigo atacado)\r";
char *sinFireBallDamage3 = "(\341rea de alcance 60 (1/3 dano))\r";  // (área de alcance 60 (1/3 dano))\r
char *sinAddDamage7 = "dano adicional:\r";
char *PlusDamageWeapon = "arma";
char *SparkDamage = "dano por unidade:\r";
char *SparkNum7 = "# m\341x. de fagulhas:\r";  // # máx. de fagulhas:\r
char *StunRage = "Alcance do recuo:\r";
char *RecvPotion7 = "Po\347\343o";  // Poção

//모라이온 2차스킬
char *PartyArea7 ="Alcance no grupo:\r";
char *Area17 ="\301\162ea:\r";
char *AddAttack_Rate9 ="Taxa de ATQ:\r";
char *PiercingRange ="Alcance do Piercing:\r";
char *IncreDefense = "Aumento na Def.:\r";
char *AddShootingRange = "Aumento no alcance:\r";
char *LifeAbsorb = "Absor\347\343o do HP.:\r";  // Absorção do HP.:\r
char *FireDamage = "Dano por Fogo:\r";
char *IceDamage = "Dano por Gelo:\r";
char *LightningDamage = "Dano por Raio:\r";
char *AddManaRegen = "Regen de Mana ad.:\r";
char *LightNum = "# de Raios:\r";
char *ReturnDamage = "Dano refletido:\r";
char *Area18 = "Alcance:\r";
char *IncreMana5 = "Aumento na Mana:\r";
char *Area19 = "Alcance:\r";
char *HelpTitle8 = " A J U D A ";
char *QuestTitle8 = " D E S A F I O ";
char *ResearchTitle8 = "B U S C A";
char *TeleportTitle8 = "TELEPORT";

char *IncreWeaponAttack_RateName = "Aumento na Taxa de ATQ:\r";
char *AddMaxDamage = "Aumento no dano M\341x.:\r";  // Aumento no dano Máx.:\r
char *LinkCoreName = "Transportar-se ao membro do Clan\r";
char *DesLinkCore = "Nome:";
char *Itemul = "item";
char *ItemRecvOk = "Voc\352 aceita isto?";  // Você aceita isto?
char *Money5 = "ouro";
char *Exp5 = "EXP.";
char *WingItemName[] = {"Asa de Metal","Asa de Prata","Asa de Ouro","Asa de Diamante", "Asa do Caos"};
char *WarpGateName[] = {"Portal","Ricarten","Pillai","Terra do Crep\372sculo","Floresta Bamboo","Vila Ruinen","Cidade de Navisko","Terra Proibida", "Vila Eura"};  // Terra do Crepúsculo
char *sinWarningName5 = "A  V  I  S  O";
char *ClanCristalName = "Somente para membros de Clan\r";
char *NowMyShopSell = "A venda\r";
char *SecretNumName = "N\372mero de autentica\347\343o";  // Número de autenticação
char *MyShopExpDoc7 = "Detalhes da Loja";
char *NotAgingItem2 = "'Imposs\355vel aplicar o Aging";  // 'Impossível aplicar o Aging
char *ExpPercent2 = "EXP : %d.%d%s";
char *ExpPercent3 = "EXP : %d.0%d%s";
char *Mutant7   =   "Mutante";
char *Mechanic7 = "Mec\342nico";  // Mecânico
char *Demon7    = "Dem\364nio";  // Demônio
char *Nomal7    = "Normal";
char *Undead7    = "Morto Vivo";
char *MonsterAddDamage2 = "dano adicional)\r";
char *MonsterAddDamageFire = "dano por fogo adicional)\r";
char *HaWarpGateName[] = {"Castelo Bless",};
char *FallGameName = "Twisted forest";
char *EndlessGameName = "Torre sem Fim";
char *LookHelp = ">>more...";

//별아이템
char *HoldStarNumDoc = "O n\372mero das suas estrelas";  // O número das suas estrelas
char *StarItemBuy7   = "Comprar as estrelas";
char *ChangeMoney7   = "(Pre\347o de uma estrela : 100000)";  // (Preço de uma estrela : 100000)



//3차 스킬
char *LightningDamage2 = "Dano por Raio:\r";
char *SheildDefense = "Aumento na Def.:\r";

char *DemonDamage4  = "ATQ Ad. em Dem\364nios:\r";   // ATQ Ad. em Demônios:\r
char *PoisonDamage3  = "Dano por Veneno ad.:\r";
char *PikeNum4       = "N\372mero de Foices:\r";  // Número de Foices:\r
char *poisoningTime  = "Dura\347\343o do veneno:\r";  // Duração do veneno:\r
char *PlusCriticalName = "Cr\355tico adicional:\r";  // Crítico adicional:\r
char *SpiritFalconDamage2 = "Dano adicional:\r";
char *LifeGegenPlus = "Aumento no HP:\r";
char *PlusDamage4 = "Aumento no Dano:\r";
char *PiercingPercent = "Taxa de perfura\347\343o:\r";  // Taxa de perfuração:\r
char *DamageToLife = "Dano convertido:\r";
char *MyMonster7 = "Domina\347\343o:\r";  // Dominação:\r
char *LifeIncre4 = "Aumento no HP:\r";
char *UndeadDamage3 = "Dano ad. em Mortos vivos:\r";

char *AttackNum3     = "No. de ATQs:\r";
char *AttackRateMinus = "Deficit de Taxa de ATQ:\r";
char *MaxDamagePlus2 = "MaxDamagePlus:\r";
char *LightningAddDamage3 = "Dano ad. por Raio:\r";
char *ReLifePercent4    = "Taxa de Rec. de HP:\r";
char *GetExp3    = "Exp. adquirida:\r";
char *ResurrectionChar4    = "Taxa de restaura\347\343o";  // Taxa de restauração
char *ExtinctionPercent2    = "% de extin\347\343o:\r";  // % de extinção:\r
char *ExtinctionAmount2    = "Taxa de extin\347\343o:\r";  // Taxa de extinção:\r
char *IncreLifePercent2   = "% ad. de HP:\r";

char *ReduceDamage3 = "Redu\347\343o no dano:\r";  // Redução no dano:\r
char *IncreMagicDamage = "Aumento no dano M\341gico:\r";  // Aumento no dano Mágico:\r
char *AttackDelay3   = "Atraso no ATQ:\r";

char *FireDamage2 = "Dano por Fogo:\r";
char *IceDamage2 = "Dano por Gelo:\r";
char *AddDefense8 = "Def. Adicional:\r";
char *SkillDamage5 = "Dano da Habilidade:\r";

char *FireDamage3 = "Dano por Fogo:\r";
char *AddSpeed7 = "Vel. adicional:\r";
char *AfterDamage7 = "p\363s dano:\r";  // pós dano:\r
char *MonsterSight7 = "percep\347\343o inimiga:\r";  // percepção inimiga:\r

//4차스킬
//템스크론
char *LinghtingAddDamage4="Dano ad. por Raio:\r";
char *MaxBoltNum4="Voltagem Max.:\r";
char *AddAbsorb4="Abs. adicional:\r";
char *IncreArea4="Alcance adicional:\r";
char *IncreAttack4="Poder de ATQ Ad.:\r";
char *GolemLife4="HP+For\347a:\r";  // HP+Força:\r


char *SubAbsorb4="Redu\347\343o da Abs.:\r";  // Redução da Abs.:\r
char *AreaDamage4="Alcance do ATQ:\r";
char *MaxAttackNum4="Acertos max.:\r";
char *LifeUp4="Vitalidade adicional:\r";

char *CriticalAddDamage4="Cr\355tico adicional:\r";  // Crítico adicional:\r
char *MonstervsSubCritical4="Redu\347\343o do ATQ:\r";  // Redução do ATQ:\r
char *ChargingAddPercentDamage4="Taxa de ATQ ad. por Carga:\r";

char *IncreEvasionPercent4="Esquiva adicional:\r";
char *AddShadowNum4="Furtividade ad.:\r";

char *WolverinLife4="Vitalidade:\r";
char *WolverinRate4="Precis\343o:\r";  // Precisão:\r
char *WolverinDefense4="Defesa:\r";
char *AddEvasion4="Esquiva adicional:\r";
char *AddDamage4="Taxa de ATQ ad.:\r";
char *FalconAddDamage4="Taxa de ATQ ad. do Falc\343o:\r";  // Taxa de ATQ ad. do Falcão:\r

//모라이온
char *IncreAttackAbsorb4="Abs adicional:\r";

char *RectAngleArea4="Desfere ataques precisos e fatais:\r";

char *MonsterSubSpeed4="Redu\347\343o da vel. do ATQ:\r";  // Redução da vel. do ATQ:\r


char *AddLifeRegen4="Regen de HP ad.:\r";
char *AddManaRegen4="Regen de MP ad.:\r";
char *MagicArea4="Alcance m\341gico:\r";  // Alcance mágico:\r
char *ChainNum4="No. de inimigos afetados:\r";
char *ChainRange4="Efeito nos inimigos feridos:\r";
char *UndeadDamageAbsorb4="Abs ad. contra mortos vivos:\r";
char *UserBlockPercent4="Bloqueio adicional:\r";

char *SecondIncreMana4="Regen de Mana por seg.:\r";
char *FireDamage4="Taxa de ATQ por Fogo:\r";
char *DecreSpeed4="Redu\347\343o na Vel.:\r";  // Redução na Vel.:\r
char *DecreAttack4="Redu\347\343o na Taxa de ATQ:\r";  // Redução na Taxa de ATQ:\r
char *AddPercentDamage4="Taxa de ATQ ad.:\r";


//스킬 인계용
char *ChainDamage3    = "Dano por fa\355scas)\r";  // Dano por faíscas)\r
char *BrandishDamage3 = "Dano pelo Brandish";
char *PhysicalAbsorb3 = "Absor\347\343o f\355sica";  // Absorção física
char *SparkDamage3    = "Dano pelas fagulhas";
char *BrutalSwingCritical3   = "Cr\355tico brutal";  // Crítico brutal
char *Attck_RateAdd5 = "Attck_RateAdd:\r";
char *Triumph4="Triunfo";
char *Use4="Voc\352 ";  // Você 
char *DivineShield4="Bloqueio do escudo divino";

//퀘스트 몬스터
char *QuestMonsterName[] = {"Bargon","Guerreiro do Mal","Decapitador","Besouro Blindado ","Esqueleto Patrulheiro","Titan",
                         "Monstros da Terra Esquecida","Monstros de O\341sis","Monstros do 1\260 Andar do Calabou\347o Antigo"};  // Monstros de Oásis | Monstros do 1° Andar do Calabouço Antigo


//요건 뱀프용 +_+
char *VampOption[10] ={"Increaseagaing%: \r","Vampiro: \r","Danorelativo:\r","Redu\347\343odeHP: \r","Sorte: \r"};  // ReduçãodeHP: \r


char *Quest3ItemName[]={"Ca\347a dem\364nios","Vingador antigo","Mito","Lado demon\355aco","Anaconda","Espada de Platina","Fantasma"};  // Caça demônios | Lado demoníaco

char *Quset3ItemDoc15 = "Os itens do Desafio";
char *Quset3ItemDoc16 = " se foram ";

char *SparkDamage10 = "Dano das fa\355scas:\r";  // Dano das faíscas:\r

char *RequirLevel3 = "(Nv.Necess\341rio:%d)\r";  // (Nv.Necessário:%d)\r

char *RecvItemTT = "Recebo o Item";
char *PuzzleEvent5 = "Cart\343o completo";  // Cartão completo
char *BabelHorn = "Chifre de Babel";
char *NineTailFoxItem = "Amuleto das Nove Caudas";
char *ChristMas = "Chiny Powder";		//크리스마스이벤트
char *MyShopItemSell5 = "O Pre\347o: \r";  // O Preço: \r
char *CristalItem     ="os 7 Cristais";//일곱가지 크리스탈 보은


char *sinDeadCandyEat = "Que meleca! O gosto \351 horr\355vel!";  // Que meleca! O gosto é horrível!

char *PotionCntDoc2 = "Po\347\365es";  // Poções
char *ItemPrice7 = "Pre\347o do Item";  // Preço do Item

char *BuyMyShopItem7 = "%s comprou %d do vendedor %s por %d";
char *BuyMyShopItem8 = "%s comprou %d do vendedor %s por %d";
char *ItemName87 = "item";

char *sinEvent87 = "Desafio do Colecionador";
char *sinAddDamageItem = "P. de ATQ ad.: \r";
char *sinContinueTimeItem = "Dura\347\343o: \r";  // Duração: \r
char *BuyPotionMoney = "Ouro:";
char *BuyPotionKind = "N\372mero:";  // Número:
char *SheltomName2[] = {"Lucidy","Sereno","Fadeo","Sparky","Raident","Transparo","Murky","Devine","Celesto", "Mirage", "Inferna", "Enigma"};

char *AddPercentDamage3= "P. de ATQ ad.%: \r";

char *sinLuckyBox = "Saco da Sorte";
char *SodMessage_Etc[]= {
			"A tarifa do torneio de Bellatra ir\341 para",  // A tarifa do torneio de Bellatra irá para
			"Somente os L\355deres de Clan recebem",  // Somente os Líderes de Clan recebem
			"Agora, a taxa \351 %d%s.",  // Agora, a taxa é %d%s.
			};

char *SodMessage_Clan[] = {
			"O lucro total \351 de ",  // O lucro total é de 
			"Bellatra distribuiu a tarifa ontem.",
			};

char *SodMessage_Master[] = {
			"O total do pr\352mio \351 de",  // O total do prêmio é de
			"Bellatra distribuiu a tarifa ontem.",
			"Deseja retirar?",
			};

char *OtherClanMaster[] = {
			"O Clan ainda possui a Tarifa",
			"ajuste o desconto.",
			};
/*----------------------------------------------------------------------------*
*			테스트			공성전 메뉴메세지
*-----------------------------------------------------------------------------*/
char *SiegeMessage_Taxrates[] = {
	"A tarifa foi mudada para %d%s.",
	"O total das tarifas acumulou",
	"Ouro.",
	"Gostaria de arrecadar a tarifa?",
	"A tarifa agora \351 de %d%s.",  // A tarifa agora é de %d%s.
};
char *SiegeMessage_MercenrayA[] = {
	"Mil\355cia de Ricarten",  // Milícia de Ricarten
	"Esta \351 a Mil\355cia",  // Esta é a Milícia
	"S\343o fracos, por\351m, eficientes.",  // São fracos, porém, eficientes.
	"Qtde. m\341x. de Mercen\341rios: 20 / %d",  // Qtde. máx. de Mercenários: 20 / %d
};
char *SiegeMessage_MercenrayB[] = {
	"Guarda de Ricarten",
	"O pre\347o e as habilidades s\343o equilibrados",  // O preço e as habilidades são equilibrados
	"S\343o soldados melhor treinados para combate.",  // São soldados melhor treinados para combate.
	"Qtde. m\341x. de Mercen\341rios: 20 / %d",  // Qtde. máx. de Mercenários: 20 / %d
};
char *SiegeMessage_MercenrayC[] = {
	"Membro da Guarda Imperial",
	"Mercen\341rios especializados em grandes Guerras.",  // Mercenários especializados em grandes Guerras.
	"S\343o os melhores soldados que voc\352 encontrar\341.",  // São os melhores soldados que você encontrará.
	"Qtde. m\341x. de Mercen\341rios:20 / %d",  // Qtde. máx. de Mercenários:20 / %d
};
char *SiegeMessage_TowerIce[] = {
	"Torre de Cristais de Gelo",
	"O Gelo reduz a movimenta\347\343o",  // O Gelo reduz a movimentação
	"mas aumenta a velocidade de ATQ.",
	"Posicionamento dos Cristais de Gelo completo",
};
char *SiegeMessage_TowerFire[] = {
	"Torre de Cristais de Fogo",
	"O Fogo causa danos terr\355veis",  // O Fogo causa danos terríveis
	"mas n\343o possui nenhum efeito extra.",  // mas não possui nenhum efeito extra.
	"Posicionamento dos Cristais de Fogo completo",
};
char *SiegeMessage_TowerLighting[] = {
	"Torre de Cristais de Raio",
	"O Raio diminui a velocidade de ATQ e",
	"e de movimenta\347\343o dos advers\341rios.",  // e de movimentação dos adversários.
	"Posicionamento dos Cristais de Raio completo",
};
char *SiegeMessage_MerMoney   = "Custo por Mercen\341rio: ";  // Custo por Mercenário: 
char *SiegeMessage_TowerMoney = "Custo por Torre: ";

char *sinClanMaster7 = "L\355der do Clan";  // Líder do Clan
char *sinPrize7 = "Recompensa:  ";

char *sinLevelQuestMonster[]={"Bargon","Muffin","Abelha Selvagem","Typhoon","Ratoo","Grotesco","Punhos de A\347o"};  // Punhos de Aço
char *sinLevelQusetDoc="Quest> %s %d monstros adicionados";

char *sinTeleportDoc[] = {"Entrada da Caverna dos Cogumelos","Entrada da Caverna das Abelhas","Terra Maldita","Terra Proibida"};
char *sinLevelQuest90_2Mon[] = {"Omega","M\341quina-D","Montanha"};  // Máquina-D
char *sinLevelQusetDoc2="<Quest> Cacem %d dos %s";
char *sinLevelQusetDoc3="<Quest> Apanhem os itens dos %s";
char *sinLimitTiem2 = "Tempo: \r";
char *sinMinute2 = "minutos";
char *sinDay4    = "dia";
char *sinHour4   = "hora";
char *sinLevelQusetDoc4="<Quest> Conclua a ca\347a de %d dos %s";  // <Quest> Conclua a caça de %d dos %s

sinSTRING sinTestMsg7("Ha ha ha, seus ot\341rios... voc\352s n\343o valem nada!");  // Ha ha ha, seus otários... vocês não valem nada!

char *sinMonCodeName[]={
	0,
	"Bargon",
	"Esqueleto Guerreiro",
	"Decapitador",
	"Besouro Blindado",
	"Esqueleto Patrulheiro",     //5
	"Titan",
	"Muffin",
	"Abelha Selvagem",
	"Typhoon",
	"Ratoo",                //10
	"Grotesco",
	"Punhos de A\347o",  // Punhos de Aço
	"Omega",
	"Maquina-D",
	"Montanha",              //15
	"Cavaleiro do Mal",
	"Caramujo Rijo",
	"Caramujo Espinhoso",
	"M\372mia",  // Múmia
	"Guardi\343o Maldito",             //20  // Guardião Maldito
	"Figon",
	"Gigante de Pedra",
	"Golem de Pedra",
	"Guardi\343o de A\347o",  // Guardião de Aço
	"Centauro Arqueira",       //25
	"Centauro-L", 
	"Abelha do Mal",
	"Cavaleiro Fantasma",
	"Pesadelo",
	"Bruxa",                //30
	"Duende Guerreiro",
	"Dawlin",
	"Stygian",
	"Incubus",
	"Golem de A\347o",           //35  // Golem de Aço
	"Espectro Negro",
	"Metron",
	"Lorde Minotauro",
	"Dusk",
	"Ogro Mec\342nico",            //40  // Ogro Mecânico
	0,
};

char *ChargingTime4="Vel. ad. por carga:\r";
char *AddIceDamage4="Dano ad. por Gelo:\r";
char *IceTime4="Tempo de recuo:\r";
char *sinAttackType = "Tipo de ATQ:\r";
char *sinAttackType2[2] ={"Manual\r","Autom\341tico\r"};  // Automático\r

char *Won="Ouro";

//공성전 아이템 설명
char *CastlItemInfo[] ={
	{"Invencibilidade tempor\341ria\r"},  // Invencibilidade temporária\r
	{"Porcentagem cr\355tica\r"},  // Porcentagem crítica\r
	{"Esquiva adicional\r"},
	{"Restaura RES,HP,MP\r"},
	{"Ressurge imediatamente.\r"},
	{"ATQ da Torre de Cristais\r"},     //
	{"B\364nus de 20% no ATQ\r"},  // Bônus de 20% no ATQ\r
	0,
};
char *CastlItemInfo2[] ={
	{"reduz o ATQ pela metade\r"},
	{"aumento de 5%.\r"},
	{"aumento de 5%.\r"},
	{"dos membros do grupo.\r"},
	{"\r"},
	{"reduzido em 50%.\r"},
	{"contra o alvo.\r"},
	0,
};


char *CharClassTarget[8]={
	"Alvo(Lutador)\r",
	"Alvo(Mec\342nico)\r",  // Alvo(Mecânico)\r
	"Alvo(Pike)\r",
	"Alvo(Arqueira)\r",
	"Alvo(Cavaleiro)\r",
	"Alvo(Atalanta)\r",
	"Alvo(Mago)\r",
	"Alvo(Sacerdotisa)\r",
};
char *AttributeTower[4]={
	"Attribute(Fogo)\r",
	"Attribute(Gelo)\r",
	"Attribute(Raio)\r",
	"Dura\347\343o:%dSeg\r",  // Duração:%dSeg\r
};

char *SiegeMessage_MerComplete ="Posicionamento dos Mercen\341rios Completo";  // Posicionamento dos Mercenários Completo

//초보 퀘스트
char *haQuestMonsterName[]={
	"Hopi",
	"Coelho",
	"Duende Macabro",
	"Duende Selvagem",
	"Esqueleto",
	"Esqueleto do Mal",
	"Ciclope",
	"Bargon",
	0,
};
char *ha100LVQuestItemName[]={
	"Machado Minotauro",
	"Garra Extrema",
	"Martelo Drag\343o",  // Martelo Dragão
	"Foice Infernal",
	"Arco da Vingan\347a",  // Arco da Vingança
	"Espada Imortal",
	"Lan\347a Salamandra",  // Lança Salamandra
	"Cajado G\363tico",  // Cajado Gótico
	0,
};
//100LV퀘스트 (퓨리의 환영)
char *ha100LVQuestMonterName[]={
	"Monstros de Perum",
	"Monstros de Gallubia", 
	0,
};
char *ha100LVQuestName = "Fantasma do F\372ria";  // Fantasma do Fúria

//프리미엄 유료화 아이템
char *PremiumItemDoc[][2]={
	{"Redistribui\r"     ,"pontos de Status.\r"},     //스탯 초기화 아이템
	{"Redistribui\r"     ,"pontos de Habilidade.\r"},    //스킬 초기화 아이템
	{"Redistribui\r"," pontos de St e Hab.\r"},     //스탯/스킬 초기화 아이템
	{"Ressuscita \r"    ,"personagens mortos.\r"},        //부활 아이템
	{"Invencibilidade tempor\341ria\r","reduz o ATQ em 50%\r"},  // Invencibilidade temporária\r
//이터널 라이프
	{"Aumento de 5% \r"    ,"no cr\355tico.\r"},		//페이틀 에지  // no crítico.\r
	{"Aumento de 5% \r"         ,"na esquiva.\r"},		//에버트 스크롤
};

char *UpKeepItemDoc[] = {
	"Tempo restante : %dMin.",
	"Tx. de Drop %d%s adicional",
	"Exp. %d%s adicional",
	"Ataque %d%s adicional",
	"Converte % do dano em HP",
	"Converte % do dano em MP",
	"Aumenta a capacidade em d%",
	"Reduz o uso de MP em %d%s",
	0,
};
char *UpKeepItemName[] = {
	"Olho M\341gico",  // Olho Mágico
	"Po\347\343o da Experi\352ncia",  // Poção da Experiência
	"Presa de Vampiro",
	"Drena Almas.",
	"Poder de Awell",
	"Po\347\343o de redu\347\343o de Mana",  // Poção de redução de Mana
	"F\352nix",  // Fênix
	0,
};



