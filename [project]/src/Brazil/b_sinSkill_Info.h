

#define SIN_SKILL_USE_RIGHT		0x01000000		//Available only for the right 
#define SIN_SKILL_USE_LEFT		0x02000000		//Available only for the left 
#define SIN_SKILL_USE_ALL		0x03000000		//Both available 
#define SIN_SKILL_USE_NOT		0x04000000		//Passive Skill

//Mechanician
{"Escudo Extremo",		"Aumenta o Bloqueio de Escudos, temporariamente",
10,		34,2,		110,4,
{0,0,0},			{sinDS1,0,0,0,0,0,0,0},
F_Extreme_Shield,SKILL_EXTREME_SHIELD,SIN_SKILL_USE_RIGHT,E_Shield_UseMana},

{"Bomba Mec\342nica",		"Arremessa uma granada para ferir os inimigos em seu alcance",  // Bomba Mecânica
12,		28,2,		73,3,
{0,0,0},			{0,0,0,0,0,0,0,0},
F_Mechanic_Bomb,SKILL_MECHANIC_BOMB,SIN_SKILL_USE_RIGHT,Mechanic_Bomb_UseMana},

{"Resist\352ncia a Veneno",		"Aumenta a Resist\352ncia a Veneno, permanentemente",  // Resistência a Veneno | Aumenta a Resistência a Veneno, permanentemente
14,		0,0,		0,0,
{0,0,0},			{0,0,0,0,0,0,0,0},
F_Poison_Attribute,SKILL_POISON_ATTRIBUTE,SIN_SKILL_USE_NOT,0},

{"Absor\347\343o F\355sica",		"Aumenta, temporariamente, a capacidade de Absor\347\343o",  // Absorção Física | Aumenta, temporariamente, a capacidade de Absorção
17,		30,2,		110,5,
{0,0,0},			{0,0,0,0,0,0,0,0},
F_Physical_Absorption,SKILL_PHYSICAL_ABSORB,SIN_SKILL_USE_RIGHT,P_Absor_bUseMana},

{"Grande Golpe",			"Carrega e desfere um golpe poderoso nos inimigos",
20,		38,2,		73,3,
{0,0,0},			{sinWA1,sinWC1,sinWH1,sinWP1,sinWS2,0,0,0},
F_Great_Smash,SKILL_GREAT_SMASH,SIN_SKILL_USE_ALL,G_Smash_UseMana},

{"Maximizar",			"Aumenta tempor\341riamente o Poder de ATQ M\341ximo da Arma equipada",  // Aumenta temporáriamente o Poder de ATQ Máximo da Arma equipada
23,		41,2,		110,4,
{0,0,0},			{sinWA1,sinWC1,sinWH1,sinWP1,sinWS2,sinWS1,sinWT1,0},
F_Maximize,SKILL_MAXIMIZE,SIN_SKILL_USE_RIGHT,Maximize_UseMana},

{"Automa\347\343o",			"Aperfei\347oa o uso de Armas de longo alcance, aumentando Vel. e Poder de ATQ",  // Automação | Aperfeiçoa o uso de Armas de longo alcance, aumentando Vel. e Poder de ATQ
26,		42,2,		110,4,
{0,0,0},			{sinWS1,sinWT1,0,0,0,0,0,0},
F_Automation,SKILL_AUTOMATION,SIN_SKILL_USE_RIGHT,Automation_UseMana},

{"Spark",			"Ataca os inimigos, utilizando esferas de energia concentrada",
30,		40,2,		82,2,
{0,0,0},			{0,0,0,0,0,0,0,0},
F_Spark,SKILL_SPARK,SIN_SKILL_USE_RIGHT,Spark_UseMana},

{"Armadura Met\341lica",		"Aumenta temporariamente a Def. de Armaduras especializadas para Mec\342nicos",  // Armadura Metálica | Aumenta temporariamente a Def. de Armaduras especializadas para Mecânicos
40,		48,2,		110,5,
{0,0,0},			{0,0,0,0,0,0,0,0},
F_Metal_Armor,SKILL_METAL_ARMOR,SIN_SKILL_USE_RIGHT,Metal_Armor_UseMana},

{"Golpe Grandioso",	"Desfere dois poderosos golpes com Poder e Taxa de ATQ adicionais",
43,		44,2,		82,2,
{0,0,0},			{sinWA1,sinWC1,sinWP1,sinWS2,0,0,0,0},
F_Grand_Smash,SKILL_GRAND_SMASH,SIN_SKILL_USE_RIGHT,Grand_Smash_UseMana},

{"Mestre dos Mec\342nicos",	"Aperfei\347oa o uso de Armas especializadas para Mec\342nicos",  // Mestre dos Mecânicos | Aperfeiçoa o uso de Armas especializadas para Mecânicos
46,		0,0,		0,0,
{0,0,0},			{0,0,0,0,0,0,0,0},
F_M_W_Mastery,SKILL_MECHANIC_WEAPON,SIN_SKILL_USE_NOT,0},

{"Escudo Espinhoso",	    "Escudo de energia que aumenta a Def. e fere inimigos ao atacarem. Dano baseado na habilidade Spark",
50,		52,2,		110,2,
{0,0,0},			{sinDS1,0,0,0,0,0,0,0},
F_Spark_Shield,SKILL_SPARK_SHIELD,SIN_SKILL_USE_RIGHT,Spark_Shield_UseMana},

//[메카니션]4차 스킬
{"Impulso","Ataque duplo que libera uma forte descarga el\351trica",  // Ataque duplo que libera uma forte descarga elétrica
60,     	55,2,		130,0,
{1,0,0},            {sinWC1,sinWH1,sinWP1,sinWS2,0,0,0,0},
F_Impulsion,SKILL_IMPULSION,SIN_SKILL_USE_RIGHT,Impulsion_UseMana},

{"Implos\343o","Implos\343o que atrai inimigos e aumenta tempor\341riamente a Abs",  // Implosão | Implosão que atrai inimigos e aumenta temporáriamente a Abs
63,     	65,2,		440,0,
{1,0,0},            {0,0,0,0,0,0,0,0},
F_Compulsion,SKILL_COMPULSION,SIN_SKILL_USE_RIGHT,Compulsion_UseMana},

{"Esfera Magn\351tica","Cria esferas magn\351ticas, que ferem os inimigos mais pr\363ximos",  // Esfera Magnética | Cria esferas magnéticas, que ferem os inimigos mais próximos
66,     	71,2,		600,0,
{1,0,0},            {0,0,0,0,0,0,0,0},
F_Magnetic_Sphere,SKILL_MAGNETIC_SPHERE,SIN_SKILL_USE_RIGHT,Magnetic_Sphere_UseMana},

{"Golem de Metal","Invoca um Monstro de metal, para auxiliar nas batalhas",
70,     	71,2,		1000,0,
{1,0,0},            {0,0,0,0,0,0,0,0},
F_Metal_Golem,SKILL_METAL_GOLEM,SIN_SKILL_USE_RIGHT,Metal_Golem_UseMana},


//Fighter 
{"Mestre das Armas",		"Aumenta a efici\352ncia de Machados, Garras, Martelos e Espadas, permanentemente",  // Aumenta a eficiência de Machados, Garras, Martelos e Espadas, permanentemente
10,		0,0,		0,0,
{0,0,0},			{sinWA1,sinWC1,sinWH1,sinWS2,0,0,0,0},
F_Melee_Mastery,SKILL_MELEE_MASTERY,SIN_SKILL_USE_NOT,0},

{"Resist\352ncia a Fogo",		"Aumenta a Resist\352ncia a Fogo, permanentemente",  // Resistência a Fogo | Aumenta a Resistência a Fogo, permanentemente
12,		0,0,		0,0,
{0,0,0},			{0,0,0,0,0,0,0,0},
F_Fire_Attribute,SKILL_FIRE_ATTRIBUTE,SIN_SKILL_USE_NOT,0},

{"Raiva",			"Fica nervoso e mais letal, mas perde HP com o tempo",
14,		35,2,		93,3,
{0,0,0},			{sinWA1,sinWC1,sinWH1,sinWP1,sinWS2,0,0,0},
F_Raving,SKILL_RAVING,SIN_SKILL_USE_ALL,Raving_UseMana},

{"Impacto",			"Desfere golpes com Taxa e Poder de ATQ adicionais",
17,		37,2,		86,4,
{0,0,0},			{sinWA1,sinWC1,sinWP1,sinWS2,0,0,0,0},
F_Impact,SKILL_IMPACT,SIN_SKILL_USE_ALL,Impact_UseMana},

{"Impacto Triplo",		"Vers\343o aprimorada de 'Impacto'",  // Versão aprimorada de 'Impacto'
20,		39,2,		80,4,
{0,0,0},			{sinWA1,sinWC1,sinWP1,sinWS2,0,0,0,0},
F_Triple_Impact,SKILL_TRIPLE_IMPACT,SIN_SKILL_USE_ALL,T_Impact_UseMana},

{"Girada Brutal",		"Desfere um poderoso golpe com Cr\355tico adicional",  // Desfere um poderoso golpe com Crítico adicional
23,		41,2,		82,2,
{0,0,0},			{sinWA1,sinWS2,0,0,0,0,0,0},
F_Brutal_Swing,SKILL_BRUTAL_SWING,SIN_SKILL_USE_ALL,B_Swing_UseMana},

{"Rugido",			"Atordoa, tempor\341riamente, os inimigos.",  // Atordoa, temporáriamente, os inimigos.
26,		39,2,		112,4,
{0,0,0},			{0,0,0,0,0,0,0,0},
F_Roar,SKILL_ROAR,SIN_SKILL_USE_RIGHT,Roar_UseMana},

{"F\372ria de Zecram",		"Salta enquanto golpeia o ch\343o com a Arma, causando dano por Fogo",   // Fúria de Zecram | Salta enquanto golpeia o chão com a Arma, causando dano por Fogo
30,		45,2,		84,4,
{0,0,0},			{sinWA1,sinWH1,sinWS2,0,0,0,0,0},
F_RageOfZecram,SKILL_RAGE_OF_ZECRAM,SIN_SKILL_USE_RIGHT,R_Zecram_UseMana},

{"Concentra\347\343o",	"Aumenta, tempor\341riamente, a Taxa de ATQ.",  // Concentração | Aumenta, temporáriamente, a Taxa de ATQ.
40,		53,2,		110,3,
{0,0,0},			{sinWA1,sinWS2,0,0,0,0,0,0},
F_Concentration,SKILL_CONCENTRATION,SIN_SKILL_USE_RIGHT,Concentration_UseMana},

{"Golpe da Vingan\347a",	"Ataque poderoso, com Poder e Taxa de ATQ adicionais.",  // Golpe da Vingança
43,		48,2,		73,3,
{0,0,0},			{sinWA1,sinWP1,sinWS2,0,0,0,0,0},
F_Avanging_Crash,SKILL_AVANGING_CRASH,SIN_SKILL_USE_RIGHT,A_Crash_UseMana},

{"Machado Veloz",	"Aumenta, temporariamente, a Vel. de ATQ com Machados.",
46,		53,2,		200,3,
{0,0,0},			{sinWA1,0,0,0,0,0,0,0},
F_Swift_Axe,SKILL_SWIFT_AXE,SIN_SKILL_USE_RIGHT,Swift_Axe_UseMana},

{"Quebra Ossos",		"Ataque m\372ltiplo, com dano adicional contra Dem\364nios.",  // Ataque múltiplo, com dano adicional contra Demônios.
50,		54,2,		73,3,
{0,0,0},			{sinWA1,sinWP1,sinWS2,0,0,0,0,0},
F_Bone_Crash,SKILL_BONE_CRASH,SIN_SKILL_USE_RIGHT,B_Crash_UseMana},

//[파이터]4차 스킬
{"Destroyer",		"Ataque m\372ltiplo fatal.",  // Ataque múltiplo fatal.
60,		45,2,		150,0,
{1,0,0},			{sinWA1,sinWH1,0,0,0,0,0,0},
F_Destoryer,SKILL_DETORYER,SIN_SKILL_USE_ALL,Destoryer_UseMana},

{"F\372ria","Aumenta a for\347a do Lutador, mas diminui, consider\341velmente, a Abs.",  // Fúria | Aumenta a força do Lutador, mas diminui, considerávelmente, a Abs.
63,		60,2,		700,0,
{1,0,0},			{0,0,0,0,0,0,0,0},
F_Berserker,SKILL_BERSERKER,SIN_SKILL_USE_RIGHT,Berserker_UseMana},

{"Golpe Girat\363rio","Golpe m\372ltiplo e girat\363rio letal a grupos de inimigos.",  // Golpe Giratório | Golpe múltiplo e giratório letal a grupos de inimigos.
66,		55,2,		135,0,
{1,0,0},			{sinWA1,sinWH1,sinWP1,sinWS2,0,0,0,0},
F_Cyclone_Strike,SKILL_CYCLONE_STRIKE,SIN_SKILL_USE_RIGHT,Cyclone_Strike_UseMana},

{"B\364nus de Vitalidade","Aumenta, permanentemente, o HP total.",  // Bônus de Vitalidade
70,		0,0,		0,0,
{1,0,0},			{0,0,0,0,0,0,0,0},
F_Boost_Health,SKILL_BOOST_HEALTH,SIN_SKILL_USE_NOT,},

//Pikeman
{"Sopro Afiado",			"Lan\347a a foice para o alto, fazendo os inimigos recuar",  // Lança a foice para o alto, fazendo os inimigos recuar
10,		27,2,		80,4,
{0,0,0},			{sinWP1,0,0,0,0,0,0,0},
F_Pike_Wind,SKILL_PIKE_WIND,SIN_SKILL_USE_RIGHT,Pike_Wind_UseMana},

{"Resist\352ncia a Gelo",		"Aumenta a Resist\352ncia a Gelo, permanentemente",  // Resistência a Gelo | Aumenta a Resistência a Gelo, permanentemente
12,		0,0,		0,0,
{0,0,0},			{0,0,0,0,0,0,0,0},
F_Ice_Attribute,SKILL_ICE_ATTRIBUTE,SIN_SKILL_USE_NOT,0},

{"Ataque Cr\355tico",		"Busca o ponto fraco do inimigo, atacando com Cr\355tico aperfei\347oado",  // Ataque Crítico | Busca o ponto fraco do inimigo, atacando com Crítico aperfeiçoado
14,		31,2,		84,3,
{0,0,0},			{sinWP1,sinWC1,sinWH1,sinWS2,0,0,0,0},
F_Critical_Hit,SKILL_CRITICAL_HIT,SIN_SKILL_USE_ALL,Critical_Hit_UseMana},

{"Pulo Fatal",		"Explode no ar, com um giro fatal, que fere o inimigo",
17,		34,2,		76,4,
{0,0,0},			{sinWA1,sinWP1,sinWS2,0,0,0,0,0},
F_Jumping_Crash,SKILL_JUMPING_CRASH,SIN_SKILL_USE_ALL,Jumping_Crash_UseMana},

{"Espinhos de Gelo",		"Espinhos de gelo surgem e paralisam os inimigos por um tempo",
20,		36,2,		84,4,
{0,0,0},			{sinWP1,0,0,0,0,0,0,0},
F_Ground_Pike,SKILL_GROUND_PIKE,SIN_SKILL_USE_RIGHT,G_Pike_UseMana},

{"Tornado",			"Ataque girat\363rio letal para os inimigos",  // Ataque giratório letal para os inimigos
23,		38,2,		83,4,
{0,0,0},			{sinWP1,0,0,0,0,0,0,0},
F_Tornado_Pike,SKILL_TORNADO,SIN_SKILL_USE_RIGHT,Tornado_UseMana},

{"Mestre da Def.",		"Aumenta permanentemente o Bloqueio com Foices de duas m\343os",  // Aumenta permanentemente o Bloqueio com Foices de duas mãos
26,		0,0,		0,0,
{0,0,0},			{sinWA1,sinWH1,sinWP1,sinWS2,0,0,0,0},
F_Weapone_Defence_Mastery,SKILL_WEAPONE_DEFENCE_MASTERY,SIN_SKILL_USE_NOT,0},


{"Expans\343o",			"Desfere um golpe letal",  // Expansão
30,		42,2,		82,4,
{0,0,0},			{sinWA1,sinWH1,sinWP1,0,0,0,0,0},
F_Expansion,SKILL_EXPANSION,SIN_SKILL_USE_ALL,Expansion_UseMana},

{"Lan\347as Venenosas",	"Ataque r\341pido, que envenena os inimigos.",  // Lanças Venenosas | Ataque rápido, que envenena os inimigos.
40,		50,2,		110,2,
{0,0,0},			{sinWP1,0,0,0,0,0,0,0},
F_Venom_Spear,SKILL_VENOM_SPEAR,SIN_SKILL_USE_RIGHT,VenomSpear_UseMana},

{"Desaparecer",			"Torna-o invis\355vel, temporariamente",  // Torna-o invisível, temporariamente
43,		53,2,		140,2,
{0,0,0},			{0,0,0,0,0,0,0,0},
F_Vanish,SKILL_VANISH,SIN_SKILL_USE_RIGHT,Vanish_UseMana},

{"Mestre do ATQ Cr\355tico",		"Treina o uso de Foices, aumentando a Taxa de Cr\355tico permanentemente",  // Mestre do ATQ Crítico | Treina o uso de Foices, aumentando a Taxa de Crítico permanentemente
46,		0,0,		0,0,
{0,0,0},			{sinWP1,0,0,0,0,0,0,0},
F_Critical_Mastery,SKILL_CRITICAL_MASTERY,SIN_SKILL_USE_NOT,0},

{"Foice de A\347o",	  		  "Ataque girat\363rio triplo",  // Foice de Aço | Ataque giratório triplo
50,		51,2,		88,3,
{0,0,0},			{sinWC1,sinWP1,sinWS2,0,0,0,0,0},
F_Chain_Lance,SKILL_CHAIN_LANCE,SIN_SKILL_USE_RIGHT,Chain_Lance_UseMana},

//[파이크맨]4차 스킬
{"Olho Assassino",	 "Encontra o ponto fraco dos monstros, aumentando a Taxa de Cr\355tico",  // Encontra o ponto fraco dos monstros, aumentando a Taxa de Crítico
60,		61,2,		520,0,
{1,0,0},			{sinWP1,0,0,0,0,0,0,0},
F_Assassin_Eye,SKILL_ASSASSIN_EYE,SIN_SKILL_USE_RIGHT,Assassin_Eye_UseMana},

{"Golpe Carregado",	 "Desfere golpes letais, ap\363s se concentrar",  // Desfere golpes letais, após se concentrar
63,		75,2,		150,0,
{1,0,0},			{sinWP1,sinWS2,0,0,0,0,0,0},
F_Charging_Strike,SKILL_CHARGING_STRIKE,SIN_SKILL_USE_RIGHT,Charging_Strike_UseMana},

{"Furtivo",	"Aumenta a furtividade do personagem",
66,		71,2,		650,0,
{1,0,0},			{sinWP1,0,0,0,0,0,0,0},
F_Vague,SKILL_VAGUE,SIN_SKILL_USE_RIGHT,Vague_UseMana},

{"Mestre das Sombras",	"Ataque m\372ltiplo de sombras misteriosas, para destruir o inimigo.",  // Ataque múltiplo de sombras misteriosas, para destruir o inimigo.
70,		51,2,		160,0,
{1,0,0},			{sinWP1,sinWS2,0,0,0,0,0,0},
F_Shadow_Master,SKILL_SHADOW_MASTER,SIN_SKILL_USE_RIGHT,Shadow_Master_UseMana},


//Archer
{"Falc\343o Vigia",			"Invoca um falc\343o para vigiar a \341rea, aumentando a Taxa de ATQ",  // Falcão Vigia | Invoca um falcão para vigiar a área, aumentando a Taxa de ATQ
10,		25,2,		115,5,
{0,0,0},			{0,0,0,0,0,0,0,0},
F_Scout_Hawk ,SKILL_SCOUT_HAWK,SIN_SKILL_USE_RIGHT,Scout_Hawk_UseMana },

{"Mestra do Tiro",		"Aumenta, permanentemente, o Poder de ATQ com Arcos",
12,		0,0,		0,0,
{0,0,0},			{sinWS1,0,0,0,0,0,0,0},
F_Shooting_Mastery,SKILL_SHOOTING_MASTERY,SIN_SKILL_USE_NOT,0},

{"Flecha de Vento",			"Dispara uma flecha de Vento, com Vel. adicional",
14,		25,2,		82,3,
{0,0,0},			{sinWS1,0,0,0,0,0,0,0},
F_Wind_Arrow,SKILL_WIND_ARROW,SIN_SKILL_USE_ALL,Wind_Arrow_UseMana},

{"Mira Perfeita",			"Desfere um tiro certeiro, com Taxa e Poder de ATQ adicionais",
17,		29,2,		81,3,
{0,0,0},			{sinWS1,0,0,0,0,0,0,0},
F_Perfect_Aim,SKILL_PERFECT_AIM,SIN_SKILL_USE_ALL,Perfect_Aim_UseMana},

{"Olho de Dion",			"Treina a pontaria, aumentando a Taxa de ATQ, permanentemente",
20,		0,0,		0,0,
{0,0,0},			{sinWS1,0,0,0,0,0,0,0},
F_Dions_Eye,SKILL_DIONS_EYE,SIN_SKILL_USE_NOT,0},

{"Falc\343o",			"Invoca um falc\343o, para abater os inimigos",  // Falcão | Invoca um falcão, para abater os inimigos
23,		36,2,		115,4,
{0,0,0},			{0,0,0,0,0,0,0,0},
F_Falcon ,SKILL_FALCON,SIN_SKILL_USE_RIGHT,Falcon_UseMana },

{"Flecha da F\372ria",		"Rajada fatal, que explode e fere um grupo de inimigos",  // Flecha da Fúria
26,		38,2,		82,4,
{0,0,0},			{sinWS1,0,0,0,0,0,0,0},
F_Arrow_of_Rage,SKILL_ARROW_OF_RAGE,SIN_SKILL_USE_RIGHT,A_Rage_UseMana},

{"Avalanche",			"Lan\347a uma seq\374\352ncia de flechas perfuradoras, para destruir os inimigos",  // Lança uma seqüência de flechas perfuradoras, para destruir os inimigos
30,		41,2,		103,2,
{0,0,0},			{sinWS1,0,0,0,0,0,0,0},
F_Avalanche,SKILL_AVALANCHE,SIN_SKILL_USE_RIGHT,Avalanche_UseMana},

{"Tiro Elemental",		"Disparos de fogo ou gelo, para abater os inimigos",
40,		43,2,		82,2,
{0,0,0},			{sinWS1,0,0,0,0,0,0,0},
F_E_Shot,SKILL_ELEMENTAL_SHOT,SIN_SKILL_USE_RIGHT,Elemental_Shot_UseMana},

{"Falc\343o Dourado",		"Invoca um falc\343o, com dano adicional e que aumenta a regenera\347\343o de HP",  // Falcão Dourado | Invoca um falcão, com dano adicional e que aumenta a regeneração de HP
43,		52,2,		120,4,
{0,0,0},			{0,0,0,0,0,0,0,0},
F_G_Falcon,SKILL_GOLDEN_FALCON,SIN_SKILL_USE_RIGHT,Golden_Falcon_UseMana},

{"Tiro Explosivo",		    "Dispara duas flechas explosivas, causando dano por \341rea.",  // Dispara duas flechas explosivas, causando dano por área.
46,		46,2,		87,3,
{0,0,0},			{sinWS1,0,0,0,0,0,0,0},
F_B_Shot,SKILL_BOMB_SHOT,SIN_SKILL_USE_RIGHT,BombShot_UseMana},

{"Perfura\347\343o",		"Ataque preciso e cont\355nuo, que atravessa os inimigos.",  // Perfuração | Ataque preciso e contínuo, que atravessa os inimigos.
50,		48,2,		82,2,
{0,0,0},			{sinWS1,0,0,0,0,0,0,0},
F_Perforation,SKILL_PERFORATION,SIN_SKILL_USE_RIGHT,Perforation_UseMana},

//[아쳐]4차 스킬
{"Invocar Wolverine",		"Invoca um Wolverine.",
60,		58,2,		900,0,
{1,0,0},			{0,0,0,0,0,0,0,0},
F_Recall_Wolverin,SKILL_RECALL_WOLVERIN,SIN_SKILL_USE_RIGHT,Recall_Wolverin_UseMana},

{"Mestra da Esquiva",	"Aumenta a Esquiva, para desviar melhor dos ataques inimigos.",
63,		0,0,		0,0,
{1,0,0},			{0,0,0,0,0,0,0,0},
F_Evasion_Mastery,SKILL_EVASION_MASTERY,SIN_SKILL_USE_NOT,0},

{"Tiro F\352nix",	"Tiro explosivo, com a for\347a da F\352nix.",  // Tiro Fênix | Tiro explosivo, com a força da Fênix.
66,		55,2,		100,0,
{1,0,0},			{sinWS1,0,0,0,0,0,0,0},
F_Phoenix_Shot,SKILL_PHOENIX_SHOT,SIN_SKILL_USE_RIGHT,Phoenix_Shot_UseMana},

{"For\347a da Natureza","Aumenta seus poderes, pela for\347a da natureza.",  // Força da Natureza | Aumenta seus poderes, pela força da natureza.
70,		56,2,		900,0,
{1,0,0},			{sinWS1,0,0,0,0,0,0,0},
F_Force_Of_Nature,SKILL_FORCE_OF_NATURE,SIN_SKILL_USE_RIGHT,Force_Of_Nature_UseMana},


/*----------------------------------------------------------------------------*
*							Morion
*-----------------------------------------------------------------------------*/	
/////// Knight
{"Rajada Sagrada",		"Rajada divina que fere os inimigos",
10,		22,2,		82,2,
{0,0,0},			{sinWS2,0,0,0,0,0,0,0},
F_Sword_Blast,SKILL_SWORD_BLAST,SIN_SKILL_USE_ALL,Sword_Blast_UseMana},

{"Corpo Sagrado",			"Recebe a b\352n\347\343o divina, para aumentar a Abs. contra Mortos Vivos",  // Recebe a bênção divina, para aumentar a Abs. contra Mortos Vivos
12,		28,2,		110,3,
{0,0,0},			{0,0,0,0,0,0,0,0},
F_HolyBody ,SKILL_HOLY_BODY,SIN_SKILL_USE_RIGHT,HolyBody_UseMana },

{"Treinamento F\355sico",		"Aumenta a RES, permanentemente",  // Treinamento Físico
14,		0,0,		0,0,
{0,0,0},			{0,0,0,0,0,0,0,0},
F_Physical_Traning,SKILL_PHYSICAL_TRANING,SIN_SKILL_USE_NOT,0},

{"Golpe Duplo",		"Ataque duplo, veloz e letal.",
17,		35,2,		82,2,
{0,0,0},			{sinWA1,sinWC1,sinWS2,0,0,0,0,0},
F_Double_Crash,SKILL_DOUBLE_CRASH,SIN_SKILL_USE_RIGHT,D_Crash_UseMana},

{"Valor Sagrado",			"Aumenta, temporariamente, o Poder de ATQ contra mortos vivos",
20,		36,2,		100,3,
{0,0,0},			{0,0,0,0,0,0,0,0},
F_Holy_Valor,SKILL_HOLY_VALOR,SIN_SKILL_USE_RIGHT,Holy_Valor_Mana},

{"Brandish",			"Ataca os inimigos, causando dano por \341rea",  // Ataca os inimigos, causando dano por área
23,		38,2,		82,2,
{0,0,0},			{sinWP1,sinWS2,0,0,0,0,0,0},
F_Brandish,SKILL_BRANDISH,SIN_SKILL_USE_ALL,Brandish_Mana},

{"Piercing",			"Carrega a espada com um ataque que atravessa seus alvos",
26,		41,2,		82,2,
{0,0,0},			{sinWS2,0,0,0,0,0,0,0},
F_Piercing,SKILL_PIERCING,SIN_SKILL_USE_ALL,Piercing_Mana},

{"Esp\355rito Dr\341stico",		"Aumenta a Def., por um per\355odo determinado.",  // Espírito Drástico | Aumenta a Def., por um período determinado.
30,		44,2,		100,2,
{0,0,0},			{0,0,0,0,0,0,0,0},
F_Drastic_Spirit,SKILL_DRASTIC_SPIRIT,SIN_SKILL_USE_RIGHT,Drastic_Spirit_Mana},

{"Mestre da Espada",	"Treina o uso de espadas, aumentando o Poder de ATQ, permanentemente.",
40,		0,0,		0,0,
{0,0,0},			{sinWS2,0,0,0,0,0,0,0},
F_S_Mastery,SKILL_SWORD_MASTERY,SIN_SKILL_USE_NOT,0},

{"Escudo Divino",		"Absorve o ATQ de Mortos Vivos, convertendo-o para HP.",
43,		46,2,		130,1,
{0,0,0},			{sinDS1,0,0,0,0,0,0,0},
F_D_Inhalation,SKILL_DIVINE_INHALATION,SIN_SKILL_USE_RIGHT,D_Inhalation_UseMana},

{"Encantamento Sagrado",	"Encanto sagrado que converte mortos-vivos em aliados",
46,		50,2,		120,1,
{0,0,0},			{0,0,0,0,0,0,0,0},
F_Holy_Incantation,SKILL_HOLY_INCANTATION,SIN_SKILL_USE_RIGHT,H_Incantation_UseMana},

{"Grande Cruzada",	"Golpe duplo e veloz, para derrubar os inimigos.",
50,		45,2,		82,2,
{0,0,0},			{sinWH1,sinWS2,0,0,0,0,0,0},
F_Grand_Cross,SKILL_GRAND_CROSS,SIN_SKILL_USE_RIGHT,Grand_Cross_UseMana},

//[나이트]4차 스킬
{"Piercing Divino",	"Desfere uma seq\374\352ncia letal de golpes, com a b\352n\347\343o sagrada ao final",  // Desfere uma seqüência letal de golpes, com a bênção sagrada ao final
70,		51,2,		160,0,
{1,0,0},			{sinWS2,0,0,0,0,0,0,0},
F_Divine_Piercing,SKILL_DIVINE_PIERCING,SIN_SKILL_USE_RIGHT,Divine_Piercing_UseMana},

{"Escudo dos deuses",	"Cria um escudo divino, dando prote\347\343o extrema ao Cavaleiro",  // Cria um escudo divino, dando proteção extrema ao Cavaleiro
63,		50,2,		400,0,
{1,0,0},			{sinDS1,0,0,0,0,0,0,0},
F_Godly_Shield,SKILL_GODLY_SHIELD,SIN_SKILL_USE_RIGHT,Godly_Shield_UseMana},

{"B\352n\347\343o Divina",	"Invoca a for\347a divina, para aumentar o poder de ATQ, temporariamente",  // Bênção Divina | Invoca a força divina, para aumentar o poder de ATQ, temporariamente
66,		51,2,		330,0,
{1,0,0},			{sinWS2,0,0,0,0,0,0,0},
F_God_Bless,SKILL_GOD_BLESS,SIN_SKILL_USE_RIGHT,God_Bless_UseMana},

{"Espada da Justi\347a","Poderoso ataque em \341rea com dano adicional",  // Espada da Justiça | Poderoso ataque em área com dano adicional
60,		53,2,		250,0,
{1,0,0},			{sinWS2,0,0,0,0,0,0,0},
F_Sword_Of_Justice,SKILL_SWORD_OF_JUSTICE,SIN_SKILL_USE_RIGHT,Sword_Of_Justice_UseMana},

/////////// Atalanta
{"Golpe do Escudo",		"Atira o escudo ferindo inimigos e paralizando-os temporariamente",
10,		23,2,		82,2,
{0,0,0},			{sinDS1,0,0,0,0,0,0,0},
F_S_Strike ,SKILL_SHIELD_STRIKE,SIN_SKILL_USE_RIGHT,S_Strike_UseMana },

{"Farina",			"Arremessa uma lan\347a Fantasma no alvo com grande precis\343o",  // Arremessa uma lança Fantasma no alvo com grande precisão
12,		26,2,		82,2,
{0,0,0},			{sinWT1,0,0,0,0,0,0,0},
F_Farina,SKILL_FARINA,SIN_SKILL_USE_ALL,Farina_UseMana},

{"Mestra do Arremesso",		"Treina a perfei\347\343o dos arremessos e aumenta o Poder de ATQ, permanentemente",  // Treina a perfeição dos arremessos e aumenta o Poder de ATQ, permanentemente
14,		0,0,		0,0,
{0,0,0},			{sinWT1,0,0,0,0,0,0,0},
F_D_Mastery,SKILL_THROWING_MASTERY,SIN_SKILL_USE_NOT,0},

{"Lan\347a Gigante",		"Ataca os inimigos, com a for\347a divina",  // Lança Gigante | Ataca os inimigos, com a força divina
17,		32,2,		82,2,
{0,0,0},			{sinWP1,sinWT1,0,0,0,0,0,0},
F_V_Spear,SKILL_VIGOR_SPEAR,SIN_SKILL_USE_ALL,V_Spear_UseMana},

{"Windy",			"Aumenta o Alcance e a Taxa de ATQ de suas Lan\347as",  // Aumenta o Alcance e a Taxa de ATQ de suas Lanças
20,		34,2,		100,2,
{0,0,0},			{sinWT1,0,0,0,0,0,0,0},
F_Windy,SKILL_WINDY,SIN_SKILL_USE_RIGHT,Windy_Mana},

{"Lan\347a Girat\363ria",		"Aumenta a Taxa e o Poder de ATQ, a cada ataque",  // Lança Giratória
23,		36,2,		82,2,
{0,0,0},			{sinWT1,0,0,0,0,0,0,0},
F_Twist_Javelin,SKILL_TWIST_JAVELIN,SIN_SKILL_USE_RIGHT,Twist_Javelin_Mana},

{"Ladr\343o de Almas",			"Absorve o HP dos inimigos",  // Ladrão de Almas
26,		42,2,		110,2,
{0,0,0},			{sinDS1,0,0,0,0,0,0,0},
F_Soul_Sucker,SKILL_SOUL_SUCKER,SIN_SKILL_USE_RIGHT,Soul_Sucker_Mana},

{"Lan\347a de Fogo",		"Ataca os inimigos, com o poder do Fogo",  // Lança de Fogo
30,		40,2,		82,2,
{0,0,0},			{sinWT1,0,0,0,0,0,0,0},
F_Fire_Javelin,SKILL_FIRE_JAVELIN,SIN_SKILL_USE_ALL,Fire_Javelin_Mana},

{"Lan\347a Partida",	"Ataque m\372ltiplo, veloz e letal aos inimigos",  // Lança Partida | Ataque múltiplo, veloz e letal aos inimigos
40,		43,2,		115,1,
{0,0,0},			{sinWT1,0,0,0,0,0,0,0},
F_Split_Javelin,SKILL_SPLIT_JAVELIN,SIN_SKILL_USE_RIGHT,Split_Javelin_UseMana},

{"Triunfo de Valhalla",	"Concede ao alvo o poder de Valhalla, aumentando o dano m\341ximo",  // Concede ao alvo o poder de Valhalla, aumentando o dano máximo
43,		50,2,		120,1,
{0,0,0},			{sinWT1,sinWS2,sinWS1,sinWP1,sinWH1,sinWA1,sinWC1,sinWM1},
F_T_Of_Valhalla,SKILL_TRIUMPH_OF_VALHALLA,SIN_SKILL_USE_RIGHT,T_Of_Valhalla_UseMana},

{"Lan\347a de Raio",	"Adiciona dano elemental \340s Lan\347as.",  // Lança de Raio | Adiciona dano elemental às Lanças.
46,		46,2,		82,2,
{0,0,0},			{sinWT1,0,0,0,0,0,0,0},
F_Lightning_Javelin,SKILL_LIGHTNING_JAVELIN,SIN_SKILL_USE_ALL,Lightning_Javelin_UseMana},

{"Chuva de Lan\347as",	    "Arremessa Lan\347as como devastadores espirais que destroem tudo em seu caminho",  // Chuva de Lanças | Arremessa Lanças como devastadores espirais que destroem tudo em seu caminho
50,		48,2,		82,2,
{0,0,0},			{sinWT1,0,0,0,0,0,0,0},
F_Storm_Javelin,SKILL_STORM_JAVELIN,SIN_SKILL_USE_RIGHT,Storm_Javelin_UseMana},

//[아틀란타]4차 스킬
{"Inferno de Valhalla",	"Aumenta temporariamente a Evas\343o, usando o poder m\341ximo de Valhalla",  // Aumenta temporariamente a Evasão, usando o poder máximo de Valhalla
60,		52,2,		750,0,
{1,0,0},			{sinWA1,sinWC1,sinWH1,sinWP1,sinWS1,sinWT1,0,0},
F_Hall_Of_Valhalla,SKILL_HALL_OF_VALHALLA,SIN_SKILL_USE_RIGHT,Hall_Of_Valhalla_UseMana},

{"F\372ria-X",	"Pula ao ar e atira 3 lan\347as explosivas ferindo todo inimigos na \341rea",  // Fúria-X | Pula ao ar e atira 3 lanças explosivas ferindo todo inimigos na área
63,		48,2,		210,0,
{1,0,0},			{sinWT1,0,0,0,0,0,0,0},
F_X_Rage,SKILL_X_RAGE,SIN_SKILL_USE_RIGHT,X_Rage_UseMana},

{"Lan\347a de Gelo",	"Temporariamente encanta a Lan\347a com elemento gelo, reduzindo a velocidade dos inimigos quando atingidos",  // Lança de Gelo | Temporariamente encanta a Lança com elemento gelo, reduzindo a velocidade dos inimigos quando atingidos
66,		50,2,		720,0,
{1,0,0},			{sinWT1,0,0,0,0,0,0,0},
F_Frost_Javelin,SKILL_FROST_JAVELIN,SIN_SKILL_USE_RIGHT,Frost_Javelin_UseMana},

{"Vingan\347a",	"Ataque r\341pido, m\372ltiplo e letal",  // Vingança | Ataque rápido, múltiplo e letal
70,		55,2,		100,0,
{1,0,0},			{sinWT1,0,0,0,0,0,0,0},
F_Vengeance,SKILL_VENGEANCE,SIN_SKILL_USE_RIGHT,Vengeance_UseMana},

///////////Priestess
{"Cura",			 "Cura os personagens escolhidos com a b\352n\347\343o divina",  // Cura os personagens escolhidos com a bênção divina
10,		15,1,		82,2,
{0,0,0},			{sinWM1,0,0,0,0,0,0,0},
F_Healing,SKILL_HEALING,SIN_SKILL_USE_RIGHT,Healing_UseMana},

{"Fagulha Sagrada",			"Fa\355sca m\341gica para ferir os inimigos",  // Faísca mágica para ferir os inimigos
12,		11,1,		82,2,
{0,0,0},			{sinWM1,0,0,0,0,0,0,0},
F_HolyBolt,SKILL_HOLY_BOLT,SIN_SKILL_USE_ALL,HolyBolt_UseMana},

{"Espinhos M\372ltiplos",			"Dispara fagulhas m\341gicas, para abater os inimigos",  // Espinhos Múltiplos | Dispara fagulhas mágicas, para abater os inimigos
14,		14,1,		82,2,
{0,0,0},			{sinWM1,0,0,0,0,0,0,0},
F_M_Spark,SKILL_MULTISPARK,SIN_SKILL_USE_RIGHT,M_Spark_UseMana},

{"Feiti\347o Divino",			"Encanta e reduz o Poder de ATQ dos inimigos",  // Feitiço Divino
17,		19,1,		100,2,
{0,0,0},			{sinWM1,0,0,0,0,0,0,0},
F_HolyMind,SKILL_HOLY_MIND,SIN_SKILL_USE_RIGHT,HolyMind_UseMana},

{"Medita\347\343o",			"Aumenta o Regen de Mana, permanentemente",  // Meditação
20,		0,0,		0,0,
{0,0,0},			{0,0,0,0,0,0,0,0},
F_Meditation,SKILL_MEDITATION,SIN_SKILL_USE_NOT,0},

{"Raios Divinos",		"Rel\342mpagos divinos caem das nuvens atingindo os inimigos",  // Relâmpagos divinos caem das nuvens atingindo os inimigos
23,		21,1,		82,2,
{0,0,0},			{sinWM1,0,0,0,0,0,0,0},
F_Divine_Lightning,SKILL_DIVINE_LIGHTNING,SIN_SKILL_USE_RIGHT,Divine_Lightning_Mana},

{"Reflex\343o Divina",		"Reflete o ataque de Mortos Vivos, ao criar um escudo m\341gico.",  // Reflexão Divina | Reflete o ataque de Mortos Vivos, ao criar um escudo mágico.
26,		23,1,		120,1,
{0,0,0},			{sinWM1,0,0,0,0,0,0,0},
F_Holy_Reflection,SKILL_HOLY_REFLECTION,SIN_SKILL_USE_RIGHT,Holy_Reflection_Mana},

{"Cura M\341xima",		"Cura os membros do Grupo.",  // Cura Máxima
30,		27,1,		82,2,
{0,0,0},			{sinWM1,0,0,0,0,0,0,0},
F_Grand_Healing,SKILL_GRAND_HEALING,SIN_SKILL_USE_RIGHT,Grand_Healing_Mana},

{"Esferas de Vigor",			"Dispara duas esferas luminosas letais contra os inimigos.",
40,		24,1,		82,2,
{0,0,0},			{sinWM1,0,0,0,0,0,0,0},
F_Vigor_Ball,SKILL_VIGOR_BALL,SIN_SKILL_USE_ALL,Vigor_Ball_UseMana},

{"Ressurrei\347\343o",		"Permite trazer um membro do grupo de volta a vida",  // Ressurreição
43,		31,1,		130,1,
{0,0,0},			{sinWM1,0,0,0,0,0,0,0},
F_Resurrection,SKILL_RESURRECTION,SIN_SKILL_USE_RIGHT,Resurrection_UseMana},

{"Extin\347\343o",		"Lan\347a um feiti\347o que destr\363i parte do HP dos Mortos Vivos",   // Extinção | Lança um feitiço que destrói parte do HP dos Mortos Vivos
46,		28,1,		120,1,
{0,0,0},			{sinWM1,0,0,0,0,0,0,0},
F_Extinction,SKILL_EXTINCTION,SIN_SKILL_USE_RIGHT,Extinction_UseMana},

{"HP Virtual",	"Aumenta o HP do alvo temporariamente",
50,		31,1,		134,1,
{0,0,0},			{sinWM1,0,0,0,0,0,0,0},
F_Virtual_Life,SKILL_VIRTUAL_LIFE,SIN_SKILL_USE_RIGHT,Virtual_Life_UseMana},

//[프리스티스]4차 스킬
{"Espinhos Glaciais",	"L\342minas afiadas de gelo projetam do ch\343o contra os inimigos",   // Lâminas afiadas de gelo projetam do chão contra os inimigos
60,		35,1,		120,0,
{1,0,0},			{sinWM1,0,0,0,0,0,0,0},
F_Glacial_Spike,SKILL_GLACIAL_SPIKE,SIN_SKILL_USE_RIGHT,Glacial_Spike_UseMana},

{"Campo de Regen",	"Gera um campo que acelera o regen de HP e MP do grupo, em troca da sua imobilidade", 
63,		41,1,		950,0,
{1,0,0},			{sinWM1,0,0,0,0,0,0,0},
F_Regeneration_Field,SKILL_REGENERATION_FIELD,SIN_SKILL_USE_RIGHT,Regeneration_Field_UseMana},

{"Raios Cont\355nuos",	"Gera uma corrente de ataques el\351tricos que atinge alvos em proximidade",  // Raios Contínuos | Gera uma corrente de ataques elétricos que atinge alvos em proximidade
66,		41,1,		120,0,
{1,0,0},			{sinWM1,0,0,0,0,0,0,0},
F_Chain_Lightning,SKILL_CHAIN_LIGHTNING,SIN_SKILL_USE_RIGHT,Chain_Lightning_UseMana},
//신성계의 Giant를 소환한다.
{"Invocar Muspell",	"Anjo que batalha e protege a Celestial, aumentando seu Bloqueio e sua Abs. contra MortosVivos",
70,		47,1,		800,0,
{1,0,0},			{sinWM1,0,0,0,0,0,0,0},
F_Summon_Muspell,SKILL_SUMMON_MUSPELL,SIN_SKILL_USE_RIGHT,Summon_Muspell_UseMana},

///////////Magician
{"Agonia",			"Sacrifica o HP, para restaurar o MP",
10,		12,1,		82,2,
{0,0,0},			{sinWM1,0,0,0,0,0,0,0},
F_Agony,SKILL_AGONY,SIN_SKILL_USE_RIGHT,Agony_UseMana},

{"Fagulha",			"Dispara pequenas fagulhas, para ferir os inimigos",
12,		11,1,		84,2,
{0,0,0},			{sinWM1,0,0,0,0,0,0,0},
F_FireBolt,SKILL_FIRE_BOLT,SIN_SKILL_USE_ALL,FireBolt_UseMana},

{"Zenith",			"Aumenta a resist\352ncia elemental, durante determinado per\355odo",  // Aumenta a resistência elemental, durante determinado período
14,		15,1,		100,3,
{0,0,0},			{sinWM1,0,0,0,0,0,0,0},
F_Zenith,SKILL_ZENITH,SIN_SKILL_USE_RIGHT,Zenith_UseMana},

{"Bola de Fogo",			"Dispara grandes bolas de fogo, para abater os inimigos",
17,		19,1,		82,2,
{0,0,0},			{sinWM1,0,0,0,0,0,0,0},
F_FireBall,SKILL_FIRE_BALL,SIN_SKILL_USE_RIGHT,FireBall_UseMana},

{"Mestre da Mente",		"Aumenta a Mana total",
20,		0,0,		0,0,
{0,0,0},			{0,0,0,0,0,0,0,0},
F_Mental_Mastery,SKILL_MENTAL_MASTERY,SIN_SKILL_USE_NOT,0},

{"Watornado",			"Um poderoso tornado d'agua \351 arremessado, explodindo contra os inimigos",  // Um poderoso tornado d'agua é arremessado, explodindo contra os inimigos
23,		21,1,		82,2,
{0,0,0},			{sinWM1,0,0,0,0,0,0,0},
F_Watornado,SKILL_WATORNADO,SIN_SKILL_USE_RIGHT,Watornado_Mana},

{"Encanto",		"Encanta as Armas do grupo com elementais, aumentando o Poder de ATQ", 
26,		28,1,		104,2,
{0,0,0},			{sinWM1,0,0,0,0,0,0,0},
F_Enchant_Weapon,SKILL_ENCHANT_WEAPON,SIN_SKILL_USE_RIGHT,Enchant_Weapon_Mana},

{"Raio da Morte",			"Um raio poderoso sai das m\343os do Feiticeiro, pronto pra desintegrar um alvo",  // Um raio poderoso sai das mãos do Feiticeiro, pronto pra desintegrar um alvo
30,		26,1,		82,2,
{0,0,0},			{sinWM1,0,0,0,0,0,0,0},
F_Dead_Ray,SKILL_DEAD_RAY,SIN_SKILL_USE_ALL,Dead_Ray_Mana},

{"Escudo Energ\351tico",		"Forma um escudo m\341gico, absorvendo parte dos ataques sofridos ao custo de mana",  // Escudo Energético | Forma um escudo mágico, absorvendo parte dos ataques sofridos ao custo de mana
40,		40,1,		150,1,
{0,0,0},			{sinWM1,0,0,0,0,0,0,0},
F_Energy_Shield,SKILL_ENERGY_SHIELD,SIN_SKILL_USE_RIGHT,Energy_Shield_UseMana},

{"Terremoto",	"Cria uma explos\343o devastadora, baseada no elemento 'terra'",   // Cria uma explosão devastadora, baseada no elemento 'terra'
43,		33,1,		106,1,
{0,0,0},			{sinWM1,0,0,0,0,0,0,0},
F_Diastrophism,SKILL_DIASTROPHISM,SIN_SKILL_USE_RIGHT,Diastrophism_UseMana},

{"Esp\355rito Elemental",	"Invoca um pequeno fantasma, para aumentar o dano m\341gico",  // Espírito Elemental | Invoca um pequeno fantasma, para aumentar o dano mágico
46,		35,1,		150,1,
{0,0,0},			{sinWM1,0,0,0,0,0,0,0},
F_Spirit_Elemental,SKILL_SPIRIT_ELEMENTAL,SIN_SKILL_USE_RIGHT,Spirit_Elemental_UseMana},

{"Espada Dan\347ante",	"Uma espada fantasma aparece e ajuda a combater os inimigos com danos elementais",  // Espada Dançante
50,		30,1,		150,1,
{0,0,0},			{sinWM1,0,0,0,0,0,0,0},
F_D_Sword,SKILL_DANCING_SWORD,SIN_SKILL_USE_RIGHT,Dancing_Sword_UseMana},

//[메지션]4차 스킬
{"Chama Elemental",	"Invoca uma criatura de Fogo, para derrotar os inimigos",
60,		120,2,		900,0,
{1,0,0},			{sinWM1,0,0,0,0,0,0,0},
F_Fire_Elemental,SKILL_FIRE_ELEMENTAL,SIN_SKILL_USE_RIGHT,Fire_Elemental_UseMana},

{"Ondas de Fogo",	"Ataca os monstros, com poderosas ondas incandescentes",
63,		32,1,		124,0,
{1,0,0},			{sinWM1,0,0,0,0,0,0,0},
F_Flame_Wave,SKILL_FLAME_WAVE,SIN_SKILL_USE_RIGHT,Flame_Wave_UseMana},

{"Distor\347\343o",	"Gera um pulso de energia, temporariamente reduzindo os atributos dos inimigos na \341rea",  // Distorção | Gera um pulso de energia, temporariamente reduzindo os atributos dos inimigos na área
66,		43,1,		460,0,
{1,0,0},			{sinWM1,0,0,0,0,0,0,0},
F_Distortion,SKILL_DISTORTION,SIN_SKILL_USE_RIGHT,Distortion_UseMana},

{"Mete\363ro",	"Invoca uma chuva letal de Meteoros.",  // Meteóro
70,		39,1,		190,0,
{1,0,0},			{sinWM1,0,0,0,0,0,0,0},
F_Meteo,SKILL_M_METEO,SIN_SKILL_USE_RIGHT,Meteo_UseMana},

{"For\347a Orb",		"Aumenta o ATQ, de acordo com a Orb",  // Força Orb
0,		0,0,		0,0,
{0,0,0},			{0,0,0,0,0,0,0,0},
NULL,SKILL_FORCE_ORB,0,0},

//퀘스트 타임 테이블
{"Desafio",		"Ca\347ar os monstros!",  // Caçar os monstros!
0,		0,0,		0,0,
{0,0,0},			{0,0,0,0,0,0,0,0},
NULL,CHANGE_JOB3_QUEST,0,0},

//퀘스트 타임 테이블
{"Desafio",		"Ca\347ar os monstros!",  // Caçar os monstros!
0,		0,0,		0,0,
{0,0,0},			{0,0,0,0,0,0,0,0},
NULL,CHANGE_ELEMENTARY_QUEST,0,0},

//공성전 클랜스킬 테이블
{"Absor\347\343o Extrema","+10 de Absor\347\343o, por uma semana",  // Absorção Extrema | +10 de Absorção, por uma semana
0,		0,0,		0,0,
{0,0,0},			{0,0,0,0,0,0,0,0},
NULL,CLANSKILL_ABSORB,0,0},

{"For\347a Oculta","+10% de Poder de ATQ, por uma semana",  // Força Oculta
0,		0,0,		0,0,
{0,0,0},			{0,0,0,0,0,0,0,0},
NULL,CLANSKILL_ATTACK,0,0},

{"Instinto","+10% de Esquiva, por uma semana.",
0,		0,0,		0,0,
{0,0,0},			{0,0,0,0,0,0,0,0},
NULL,CLANSKILL_EVASION,0,0},

//공성전 아이템 사용 스킬
{"Sacrif\355cio de Pritos","Invencibilidade tempor\341ria. Poder de ATQ reduzido pela metade",  // Sacrifício de Pritos | Invencibilidade temporária. Poder de ATQ reduzido pela metade
0,		0,0,		0,0,
{0,0,0},			{0,0,0,0,0,0,0,0},
NULL,SCROLL_INVULNERABILITY,0,0},

{"Precis\343o de Skronipe","Aumento de 5% no Ataque Cr\355tico",  // Precisão de Skronipe | Aumento de 5% no Ataque Crítico
0,		0,0,		0,0,
{0,0,0},			{0,0,0,0,0,0,0,0},
NULL,SCROLL_CRITICAL,0,0},

{"Reflexo Selvagem","Aumento de 5% em Esquiva",
0,		0,0,		0,0,
{0,0,0},			{0,0,0,0,0,0,0,0},
NULL,SCROLL_EVASION,0,0},

{"Pedra da Revolta","Reduz o dano contra os Cristais de Fogo, em 50%",
0,		0,0,		0,0,
{0,0,0},			{0,0,0,0,0,0,0,0},
NULL,STONE_R_FIRECRYTAL,0,0},

{"Pedra da Revolta","Reduz o dano contra os Cristais de Gelo, em 50%.",
0,		0,0,		0,0,
{0,0,0},			{0,0,0,0,0,0,0,0},
NULL,STONE_R_ICECRYTAL,0,0},

{"Pedra da Revolta","Reduz o dano contra os Cristais de Raio, em 50%.",
0,		0,0,		0,0,
{0,0,0},			{0,0,0,0,0,0,0,0},
NULL,STONE_R_LINGHTINGCRYTAL,0,0},

{"Senten\347a de Morte ","Turbina o Poder de ATQ contra Lutadores, em 20%.",  // Sentença de Morte 
0,		0,0,		0,0,
{0,0,0},			{0,0,0,0,0,0,0,0},
NULL,STONE_A_FIGHTER,0,0},

{"Senten\347a de Morte ","Turbina o Poder de ATQ contra Mec\342nicos, em 20%.",  // Sentença de Morte  | Turbina o Poder de ATQ contra Mecânicos, em 20%.
0,		0,0,		0,0,
{0,0,0},			{0,0,0,0,0,0,0,0},
NULL,STONE_A_MECHANICIAN,0,0},

{"Senten\347a de Morte ","Turbina o Poder de ATQ contra Pikes, em 20%.",  // Sentença de Morte 
0,		0,0,		0,0,
{0,0,0},			{0,0,0,0,0,0,0,0},
NULL,STONE_A_PIKEMAN,0,0},

{"Senten\347a de Morte ","Turbina o Poder de ATQ contra Arqueiras, em 20%.",  // Sentença de Morte 
0,		0,0,		0,0,
{0,0,0},			{0,0,0,0,0,0,0,0},
NULL,STONE_A_ARCHER,0,0},

{"Senten\347a de Morte ","Turbina o Poder de ATQ contra Cavaleiros, em 20%.",  // Sentença de Morte 
0,		0,0,		0,0,
{0,0,0},			{0,0,0,0,0,0,0,0},
NULL,STONE_A_KNIGHT,0,0},

{"Senten\347a de Morte ","Turbina o Poder de ATQ contra Atalantas, em 20%.",  // Sentença de Morte 
0,		0,0,		0,0,
{0,0,0},			{0,0,0,0,0,0,0,0},
NULL,STONE_A_ATALANTA,0,0},

{"Senten\347a de Morte ","Turbina o Poder de ATQ contra Magos, em 20%.",  // Sentença de Morte 
0,		0,0,		0,0,
{0,0,0},			{0,0,0,0,0,0,0,0},
NULL,STONE_A_MAGICIAN,0,0},

{"Senten\347a de Morte ","Turbina o Poder de ATQ contra Sacerdotisas, em 20%.",  // Sentença de Morte 
0,		0,0,		0,0,
{0,0,0},			{0,0,0,0,0,0,0,0},
NULL,STONE_A_PRIESTESS,0,0},
//프리미엄 부분 유료화 아이템
{"Vida Eterna","Invencibilidade tempor\341ria. O ataque \351 reduzido pela 1/2",  // Invencibilidade temporária. O ataque é reduzido pela 1/2
0,		0,0,		0,0,
{0,0,0},			{0,0,0,0,0,0,0,0},
NULL,SCROLL_P_INVULNERABILITY,0,0},

{"Ponta Fatal","5% a mais de ataque cr\355tico",  // 5% a mais de ataque crítico
0,		0,0,		0,0,
{0,0,0},			{0,0,0,0,0,0,0,0},
NULL,SCROLL_P_CRITICAL,0,0},

{"Amuleto","5% a mais de esquiva",
0,		0,0,		0,0,
{0,0,0},			{0,0,0,0,0,0,0,0},
NULL,SCROLL_P_EVASION,0,0},
