
#define SIN_SKILL_USE_LEFT		0x02000000		//Available only for the left 
#define SIN_SKILL_USE_ALL		0x03000000		//Both available 
#define SIN_SKILL_USE_NOT		0x04000000		//Passive Skill

//Mechanician

{"Extreme Shield",		"T\343ng kha\322 n\343ng \360\365\336 \360o\314n khi s\375\322 du\362ng khi\352n",  // Tăng khả năng đỡ đòn khi sử dụng khiên
10,		34,2,		110,4,
{0,0,0},			{sinDS1,0,0,0,0,0,0,0},
F_Extreme_Shield,SKILL_EXTREME_SHIELD,SIN_SKILL_USE_RIGHT,E_Shield_UseMana},

{"Mechanic Bomb",		"Ne\354m m\364\362t tra\354i bom g\342y sa\354t   th\375\365ng",  // Ném một trái bom gây sát   thương
12,		28,2,		73,3,
{0,0,0},			{0,0,0,0,0,0,0,0},
F_Mechanic_Bomb,SKILL_MECHANIC_BOMB,SIN_SKILL_USE_RIGHT,Mechanic_Bomb_UseMana},

{"Poison Attribute",		"T\343ng kha\322 n\343ng ch\364\354ng la\362i     ch\342\354t \360\364\362c",  // Tăng khả năng chống lại     chất độc
14,		0,0,		0,0,
{0,0,0},			{0,0,0,0,0,0,0,0},
F_Poison_Attribute,SKILL_POISON_ATTRIBUTE,SIN_SKILL_USE_NOT,0},

{"Physical Absorption",		"T\343ng kha\322 n\343ng h\342\354p thu\362 s\375\354c  sa\354t th\375\365ng v\365\354i c\365 th\352\322",  // Tăng khả năng hấp thụ sức  sát thương với cơ thể
17,		30,2,		110,5,
{0,0,0},			{0,0,0,0,0,0,0,0},
F_Physical_Absorption,SKILL_PHYSICAL_ABSORB,SIN_SKILL_USE_RIGHT,P_Absor_bUseMana},

{"Great Smash",			"T\342\362p trung s\375\354c ma\362nh va\314o m\364\362t cu\354 \360a\354nh",  // Tập trung sức mạnh vào một cú đánh
20,		38,2,		73,3,
{0,0,0},			{sinWA1,sinWC1,sinWH1,sinWP1,sinWS2,0,0,0},
F_Great_Smash,SKILL_GREAT_SMASH,SIN_SKILL_USE_ALL,G_Smash_UseMana},

{"Maximize",			"S\375\322 du\362ng vu\336 khi\354 v\365\354i m\375\354c   sa\354t th\375\365ng cao nh\342\354t",  // Sử dụng vũ khí với mức   sát thương cao nhất
23,		41,2,		110,4,
{0,0,0},			{sinWA1,sinWC1,sinWH1,sinWP1,sinWS2,sinWS1,sinWT1,0},
F_Maximize,SKILL_MAXIMIZE,SIN_SKILL_USE_RIGHT,Maximize_UseMana},

{"Automation",			"T\375\362 \360\364\362ng hoa\354 ca\354c vu\336 khi\354   t\342\314m xa \360\352\322 t\343ng s\375\362 chi\354nh    xa\354c",  // Tự động hoá các vũ khí   tầm xa để tăng sự chính    xác
26,		42,2,		110,4,
{0,0,0},			{sinWS1,sinWT1,0,0,0,0,0,0},
F_Automation,SKILL_AUTOMATION,SIN_SKILL_USE_RIGHT,Automation_UseMana},

{"Spark",			"T\342\354n c\364ng b\343\314ng m\364\362t qua\322 c\342\314u \360i\352\362n c\375\362c ma\362nh",  // Tấn công bằng một quả cầu điện cực mạnh
30,		40,2,		82,2,
{0,0,0},			{0,0,0,0,0,0,0,0},
F_Spark,SKILL_SPARK,SIN_SKILL_USE_RIGHT,Spark_UseMana},

{"Metal Armor",		"T\343ng cao kha\322 n\343ng pho\314ng thu\322 cu\322a a\354o gia\354p",  // Tăng cao khả năng phòng thủ của áo giáp
40,		48,2,		110,5,
{0,0,0},			{0,0,0,0,0,0,0,0},
F_Metal_Armor,SKILL_METAL_ARMOR,SIN_SKILL_USE_RIGHT,Metal_Armor_UseMana},

{"Grand Smash",	"T\342\354n c\364ng li\352n tu\362c. T\343ng th\352m s\375\354c ma\362nh cu\322a \360o\314n t\342\354n c\364ng",  // Tấn công liên tục. Tăng thêm sức mạnh của đòn tấn công
43,		44,2,		82,2,
{0,0,0},			{sinWA1,sinWC1,sinWP1,sinWS2,0,0,0,0},
F_Grand_Smash,SKILL_GRAND_SMASH,SIN_SKILL_USE_RIGHT,Grand_Smash_UseMana},

{"Mechanic Weapon Mastery",	"S\375\322 du\362ng hoa\314n ha\322o ca\354c vu\336  khi\354 \360\343\362c bi\352\362t chuy\352n du\362ng",  // Sử dụng hoàn hảo các vũ  khí đặc biệt chuyên dụng
46,		0,0,		0,0,
{0,0,0},			{0,0,0,0,0,0,0,0},
F_M_W_Mastery,SKILL_MECHANIC_WEAPON,SIN_SKILL_USE_NOT,0},

{"Spark Shield",	    	"Du\314ng \360i\352\362n n\343ng \360\352\322 ba\322o v\352\362",  // Dùng điện năng để bảo vệ
50,		52,2,		110,2,
{0,0,0},			{sinDS1,0,0,0,0,0,0,0},
F_Spark_Shield,SKILL_SPARK_SHIELD,SIN_SKILL_USE_RIGHT,Spark_Shield_UseMana},

{"Impulsion","T\342\354n c\364ng \360\364\354i thu\322 b\343\314ng ca\354ch pha\354t ra tia l\375\322a \360i\352\362n",  // Tấn công đối thủ bằng cách phát ra tia lửa điện
60,     	55,2,		130,0,
{1,0,0},            {sinWC1,sinWH1,sinWP1,sinWS2,0,0,0,0},
F_Impulsion,SKILL_IMPULSION,SIN_SKILL_USE_RIGHT,Impulsion_UseMana},

{"Completion","Thu hu\354t qua\354i v\342\362t xung quanh b\343\314ng so\354ng \360i\352\362n t\375\314",  // Thu hút quái vật xung quanh bằng sóng điện từ
63,     	65,2,		440,0,
{1,0,0},            {0,0,0,0,0,0,0,0},
F_Compulsion,SKILL_COMPULSION,SIN_SKILL_USE_RIGHT,Compulsion_UseMana},

{"Magnetic Sphere","Ta\362o ca\354c vi\352n ngo\362c \360i\352\362n t\375\314 t\342\354n c\364ng b\342\354t ky\314 mu\362c ti\352u   na\314o \360\375\354ng g\342\314n",  // Tạo các viên ngọc điện từ tấn công bất kỳ mục tiêu   nào đứng gần
66,     	71,2,		600,0,
{1,0,0},            {0,0,0,0,0,0,0,0},
F_Magnetic_Sphere,SKILL_MAGNETIC_SPHERE,SIN_SKILL_USE_RIGHT,Magnetic_Sphere_UseMana},

{"Metal Glolem","Tri\352\362u h\364\314i ng\375\365\314i ma\354y b\343\314ng  s\343\354t the\354p",  // Triệu hồi người máy bằng  sắt thép
70,     	71,2,		1000,0,
{1,0,0},            {0,0,0,0,0,0,0,0},
F_Metal_Golem,SKILL_METAL_GOLEM,SIN_SKILL_USE_RIGHT,Metal_Golem_UseMana},

//Fighter 
{"Melee Mastery",		"Gia t\343ng s\375\354c t\342\354n c\364ng cho vu\336 khi\354 gia\354p la\354 ca\314 ",  // Gia tăng sức tấn công cho vũ khí giáp lá cà 
10,		0,0,		0,0,
{0,0,0},			{sinWA1,sinWC1,sinWH1,sinWS2,0,0,0,0},
F_Melee_Mastery,SKILL_MELEE_MASTERY,SIN_SKILL_USE_NOT,0},

{"Fire Attribute",		"Gia t\343ng s\375\354c chi\362u \360\375\362ng v\365\354i l\375\322a",  // Gia tăng sức chịu đựng với lửa
12,		0,0,		0,0,
{0,0,0},			{0,0,0,0,0,0,0,0},
F_Fire_Attribute,SKILL_FIRE_ATTRIBUTE,SIN_SKILL_USE_NOT,0},

{"Raving",			"T\343ng m\375\354c sa\354t th\375\365ng nh\375ng bi\362 m\342\354t HP trong m\364\336i cu\354 \360a\354nh",  // Tăng mức sát thương nhưng bị mất HP trong mỗi cú đánh
14,		35,2,		93,3,
{0,0,0},			{sinWA1,sinWC1,sinWH1,sinWP1,sinWS2,0,0,0},
F_Raving,SKILL_RAVING,SIN_SKILL_USE_ALL,Raving_UseMana},

{"Impact",			"T\342\354n c\364ng 2 chi\352u li\352n tu\362c    m\364\362t ca\354ch ma\362nh me\336 ",  // Tấn công 2 chiêu liên tục    một cách mạnh mẽ 
17,		37,2,		86,4,
{0,0,0},			{sinWA1,sinWC1,sinWP1,sinWS2,0,0,0,0},
F_Impact,SKILL_IMPACT,SIN_SKILL_USE_ALL,Impact_UseMana},

{"Triple Impact",		"M\364\362t ky\336 thu\342\362t ra \360o\314n ma\362nh  h\365n Impact",  // Một kỹ thuật ra đòn mạnh  hơn Impact
20,		39,2,		80,4,
{0,0,0},			{sinWA1,sinWC1,sinWP1,sinWS2,0,0,0,0},
F_Triple_Impact,SKILL_TRIPLE_IMPACT,SIN_SKILL_USE_ALL,T_Impact_UseMana},

{"Brutal Swing",		"Gia\354ng \360o\314n chi\354 ma\362ng",  // Giáng đòn chí mạng
23,		41,2,		82,2,
{0,0,0},			{sinWA1,sinWS2,0,0,0,0,0,0},
F_Brutal_Swing,SKILL_BRUTAL_SWING,SIN_SKILL_USE_ALL,B_Swing_UseMana},

{"Roar",			"G\342\314m l\352n la\314m cho qua\354i v\342\362t   choa\354ng va\354ng, s\365\362 ha\336i",  // Gầm lên làm cho quái vật   choáng váng, sợ hãi
26,		39,2,		112,4,
{0,0,0},			{0,0,0,0,0,0,0,0},
F_Roar,SKILL_ROAR,SIN_SKILL_USE_RIGHT,Roar_UseMana},

{"Rage of Zechram",		"M\364\362t ky\336 n\343ng ho\362c t\375\314 ng\375\365\314i  hu\314ng Zechram huy\352\314n thoa\362i",  // Một kỹ năng học từ người  hùng Zechram huyền thoại
30,		45,2,		84,4,
{0,0,0},			{sinWA1,sinWH1,sinWS2,0,0,0,0,0},
F_RageOfZecram,SKILL_RAGE_OF_ZECRAM,SIN_SKILL_USE_RIGHT,R_Zecram_UseMana},

{"Concentration",	"S\375\362 chi\354nh xa\354c \360\375\365\362c gia t\343ng trong m\364\362t khoa\322ng th\365\314i gian",  // Sự chính xác được gia tăng trong một khoảng thời gian
40,		53,2,		110,3,
{0,0,0},			{sinWA1,sinWS2,0,0,0,0,0,0},
F_Concentration,SKILL_CONCENTRATION,SIN_SKILL_USE_RIGHT,Concentration_UseMana},

{"Avenging Crash",	"\320o\314n t\342\354n c\364ng \360\364\354i thu\322 c\375\362c  ma\362nh",  // Đòn tấn công đối thủ cực  mạnh
43,		48,2,		73,3,
{0,0,0},			{sinWA1,sinWP1,sinWS2,0,0,0,0,0},
F_Avanging_Crash,SKILL_AVANGING_CRASH,SIN_SKILL_USE_RIGHT,A_Crash_UseMana},

{"Swift Axe",	"T\343ng t\364\354c \360\364\362 \360a\354nh cho Ri\314u   trong m\364\362t khoa\322ng th\365\314i gian",  // Tăng tốc độ đánh cho Rìu   trong một khoảng thời gian
46,		53,2,		200,3,
{0,0,0},			{sinWA1,0,0,0,0,0,0,0},
F_Swift_Axe,SKILL_SWIFT_AXE,SIN_SKILL_USE_RIGHT,Swift_Axe_UseMana},

{"Bone Crash",		"\320o\314n t\342\354n c\364ng ma\362nh du\314ng \360\352\322 ti\352u di\352\362t Demon",  // Đòn tấn công mạnh dùng để tiêu diệt Demon
50,		54,2,		73,3,
{0,0,0},			{sinWA1,sinWP1,sinWS2,0,0,0,0,0},
F_Bone_Crash,SKILL_BONE_CRASH,SIN_SKILL_USE_RIGHT,B_Crash_UseMana},

{"Destroyer",		"T\342\354n c\364ng li\352n hoa\314n ta\362o n\352n  sa\354t th\375\365ng c\375\362c l\365\354n \360\364\354i v\365\354i Demon",  // Tấn công liên hoàn tạo nên  sát thương cực lớn đối với Demon
60,		45,2,		150,0,
{1,0,0},			{sinWA1,sinWH1,0,0,0,0,0,0},
F_Destoryer,SKILL_DETORYER,SIN_SKILL_USE_ALL,Destoryer_UseMana},

{"Berserker","T\343ng kha\322 n\343ng sa\354t th\375\365ng     nh\375ng la\362i gia\322m kha\322 n\343ng hoa\354 gia\322i \360o\314n t\342\354n c\364ng",  // Tăng khả năng sát thương     nhưng lại giảm khả năng hoá giải đòn tấn công
63,		60,2,		700,0,
{1,0,0},			{0,0,0,0,0,0,0,0},
F_Berserker,SKILL_BERSERKER,SIN_SKILL_USE_RIGHT,Berserker_UseMana},

{"Cyclone Strike","\320o\314n t\342\354n c\364ng nhi\352\314u \360\364\354i thu\322 l\365\362i du\362ng l\375\362c ly t\342m",  // Đòn tấn công nhiều đối thủ lợi dụng lực ly tâm
66,		55,2,		135,0,
{1,0,0},			{sinWA1,sinWH1,sinWP1,sinWS2,0,0,0,0},
F_Cyclone_Strike,SKILL_CYCLONE_STRIKE,SIN_SKILL_USE_RIGHT,Cyclone_Strike_UseMana},

{"Boost Health","T\343ng chi\322 s\364\354 HP",  // Tăng chỉ số HP
70,		0,0,		0,0,
{1,0,0},			{0,0,0,0,0,0,0,0},
F_Boost_Health,SKILL_BOOST_HEALTH,SIN_SKILL_USE_NOT,},

//Pikeman
{"Pike Wind",			"Ta\362o gio\354 xoa\354y \360\342\322y ng\375\365\362c ra va\314 g\342y sa\354t th\375\365ng",  // Tạo gió xoáy đẩy ngược ra và gây sát thương
10,		27,2,		80,4,
{0,0,0},			{sinWP1,0,0,0,0,0,0,0},
F_Pike_Wind,SKILL_PIKE_WIND,SIN_SKILL_USE_RIGHT,Pike_Wind_UseMana},

{"Ice Attribute",		"T\343ng s\375\354c ch\364\354ng \360\365\336 v\365\354i b\343ng gia\354 ",  // Tăng sức chống đỡ với băng giá 
12,		0,0,		0,0,
{0,0,0},			{0,0,0,0,0,0,0,0},
F_Ice_Attribute,SKILL_ICE_ATTRIBUTE,SIN_SKILL_USE_NOT,0},

{"Critical Hit",		"Ra \360o\314n c\375\362c ma\362nh va\314o \360i\352\322m  y\352\354u",  // Ra đòn cực mạnh vào điểm  yếu
14,		31,2,		84,3,
{0,0,0},			{sinWP1,sinWC1,sinWH1,sinWS2,0,0,0,0},
F_Critical_Hit,SKILL_CRITICAL_HIT,SIN_SKILL_USE_ALL,Critical_Hit_UseMana},

{"Jumping Crash",		"Nha\322y l\352n va\314 ra \360o\314n t\375\314 tr\352n cao c\375\362c ma\362nh",  // Nhảy lên và ra đòn từ trên cao cực mạnh
17,		34,2,		76,4,
{0,0,0},			{sinWA1,sinWP1,sinWS2,0,0,0,0,0},
F_Jumping_Crash,SKILL_JUMPING_CRASH,SIN_SKILL_USE_ALL,Jumping_Crash_UseMana},

{"Ground Pike",			"La\314m \360o\354ng b\343ng \360\364\354i thu\322 ",  // Làm đóng băng đối thủ 
20,		36,2,		84,4,
{0,0,0},			{sinWP1,0,0,0,0,0,0,0},
F_Ground_Pike,SKILL_GROUND_PIKE,SIN_SKILL_USE_RIGHT,G_Pike_UseMana},

{"Tornado",			"T\342\354n c\364ng ca\354c mu\362c ti\352u xung  quanh b\343\314ng nh\375\336ng c\365n l\364\354c    xoa\354y",  // Tấn công các mục tiêu xung  quanh bằng những cơn lốc    xoáy
23,		38,2,		83,4,
{0,0,0},			{sinWP1,0,0,0,0,0,0,0},
F_Tornado_Pike,SKILL_TORNADO,SIN_SKILL_USE_RIGHT,Tornado_UseMana},

{"Weapon DEF Mastery",		"T\343ng kha\322 n\343ng chi\362u \360o\314n khi  c\342\314m gia\354o",  // Tăng khả năng chịu đòn khi  cầm giáo
26,		0,0,		0,0,
{0,0,0},			{sinWA1,sinWH1,sinWP1,sinWS2,0,0,0,0},
F_Weapone_Defence_Mastery,SKILL_WEAPONE_DEFENCE_MASTERY,SIN_SKILL_USE_NOT,0},


{"Expansion",			"Truy\352\314n s\375\354c ma\362nh va\314o vu\336    khi\354 \360\352\322 t\342\354n c\364ng",  // Truyền sức mạnh vào vũ    khí để tấn công
30,		42,2,		82,4,
{0,0,0},			{sinWA1,sinWH1,sinWP1,0,0,0,0,0},
F_Expansion,SKILL_EXPANSION,SIN_SKILL_USE_ALL,Expansion_UseMana},

{"Venom Spear",		"\320\342m th\375\365ng xu\364\354ng \360\342\354t \360\352\322 t\342\354n c\364ng",  // Đâm thương xuống đất để tấn công
40,		50,2,		110,2,
{0,0,0},			{sinWP1,0,0,0,0,0,0,0},
F_Venom_Spear,SKILL_VENOM_SPEAR,SIN_SKILL_USE_RIGHT,VenomSpear_UseMana},

{"Vanish",			"Ta\314ng hi\314nh trong m\364\362t khoa\322ng th\365\314i gian nh\342\354t \360i\362nh",  // Tàng hình trong một khoảng thời gian nhất định
43,		53,2,		140,2,
{0,0,0},			{0,0,0,0,0,0,0,0},
F_Vanish,SKILL_VANISH,SIN_SKILL_USE_RIGHT,Vanish_UseMana},

{"Critical Mastery",		"T\343ng ty\322 l\352\362 ra \360o\314n c\375\362c ma\362nh",  // Tăng tỷ lệ ra đòn cực mạnh
46,		0,0,		0,0,
{0,0,0},			{sinWP1,0,0,0,0,0,0,0},
F_Critical_Mastery,SKILL_CRITICAL_MASTERY,SIN_SKILL_USE_NOT,0},

{"Chain Lancer",	  	"Li\352n hoa\314n tam chi\352u",  // Liên hoàn tam chiêu
50,		51,2,		88,3,
{0,0,0},			{sinWC1,sinWP1,sinWS2,0,0,0,0,0},
F_Chain_Lance,SKILL_CHAIN_LANCE,SIN_SKILL_USE_RIGHT,Chain_Lance_UseMana},

{"Assassin Eye",	 "Ti\314m \360i\352\322m y\352\354u cu\322a \360\364\354i thu\322, t\343ng kha\322 n\343ng ra \360o\314n chi\354   ma\362ng",  // Tìm điểm yếu của đối thủ, tăng khả năng ra đòn chí   mạng
60,		61,2,		520,0,
{1,0,0},			{sinWP1,0,0,0,0,0,0,0},
F_Assassin_Eye,SKILL_ASSASSIN_EYE,SIN_SKILL_USE_RIGHT,Assassin_Eye_UseMana},

{"Charging Strike",	 "D\364\314n s\375\354c t\342\354n c\364ng li\352n tu\362c  b\343\314ng chi\352u \360\342m",  // Dồn sức tấn công liên tục  bằng chiêu đâm
63,		75,2,		150,0,
{1,0,0},			{sinWP1,sinWS2,0,0,0,0,0,0},
F_Charging_Strike,SKILL_CHARGING_STRIKE,SIN_SKILL_USE_RIGHT,Charging_Strike_UseMana},

{"Vague",	"T\343ng kha\322 n\343ng tra\354nh \360o\314n t\342\354n c\364ng t\375\314 qua\354i v\342\362t",  // Tăng khả năng tránh đòn tấn công từ quái vật
66,		71,2,		650,0,
{1,0,0},			{sinWP1,0,0,0,0,0,0,0},
F_Vague,SKILL_VAGUE,SIN_SKILL_USE_RIGHT,Vague_UseMana},

{"Shadow Master",	"T\342\354n c\364ng li\352n hoa\314n v\365\354i t\364\354c \360\364\362 c\375\362c l\365\354n",  // Tấn công liên hoàn với tốc độ cực lớn
70,		51,2,		160,0,
{1,0,0},			{sinWP1,sinWS2,0,0,0,0,0,0},
F_Shadow_Master,SKILL_SHADOW_MASTER,SIN_SKILL_USE_RIGHT,Shadow_Master_UseMana},

//Archer
{"Scout Hawk",			"Tri\352\362u h\364\314i chim \375ng gia t\343ng  s\375\362 chi\354nh xa\354c",  // Triệu hồi chim ưng gia tăng  sự chính xác
10,		25,2,		115,5,
{0,0,0},			{0,0,0,0,0,0,0,0},
F_Scout_Hawk ,SKILL_SCOUT_HAWK,SIN_SKILL_USE_RIGHT,Scout_Hawk_UseMana },

{"Shooting Mastery",		"T\343ng s\375\354c sa\354t th\375\365ng khi du\314ng cung ho\343\362c No\322 ",  // Tăng sức sát thương khi dùng cung hoặc Nỏ 
12,		0,0,		0,0,
{0,0,0},			{sinWS1,0,0,0,0,0,0,0},
F_Shooting_Mastery,SKILL_SHOOTING_MASTERY,SIN_SKILL_USE_NOT,0},

{"Wind Arrow",			"B\343\354n t\352n xuy\352n gio\354 v\365\354i s\375\354c  t\342\354n c\364ng ma\362nh",  // Bắn tên xuyên gió với sức  tấn công mạnh
14,		25,2,		82,3,
{0,0,0},			{sinWS1,0,0,0,0,0,0,0},
F_Wind_Arrow,SKILL_WIND_ARROW,SIN_SKILL_USE_ALL,Wind_Arrow_UseMana},

{"Perfect Aim",			"B\343\354n mu\336i t\352n \360\375\365\362c gia t\343ng   s\375\354c t\342\354n c\364ng va\314 s\375\362 chi\354nh  xa\354c",  // Bắn mũi tên được gia tăng   sức tấn công và sự chính  xác
17,		29,2,		81,3,
{0,0,0},			{sinWS1,0,0,0,0,0,0,0},
F_Perfect_Aim,SKILL_PERFECT_AIM,SIN_SKILL_USE_ALL,Perfect_Aim_UseMana},

{"Dion's Eye",			"Luy\352\362n t\342\314m nhi\314n nh\375 xa\362 thu\322 Dion huy\352\314n thoa\362i t\343ng s\375\362    chi\354nh xa\354c",  // Luyện tầm nhìn như xạ thủ Dion huyền thoại tăng sự    chính xác
20,		0,0,		0,0,
{0,0,0},			{sinWS1,0,0,0,0,0,0,0},
F_Dions_Eye,SKILL_DIONS_EYE,SIN_SKILL_USE_NOT,0},

{"Falcon",			"Tri\352\362u h\364\314i a\354c \360i\352\322u t\342\354n c\364ng ca\354c mu\362c ti\352u g\342\314n",  // Triệu hồi ác điểu tấn công các mục tiêu gần
23,		36,2,		115,4,
{0,0,0},			{0,0,0,0,0,0,0,0},
F_Falcon ,SKILL_FALCON,SIN_SKILL_USE_RIGHT,Falcon_UseMana },

{"Arrow of Rage",		"B\343\354n nh\375\336ng mu\336i t\352n l\375\322a \360\364\322  xu\364\354ng",  // Bắn những mũi tên lửa đổ  xuống
26,		38,2,		82,4,
{0,0,0},			{sinWS1,0,0,0,0,0,0,0},
F_Arrow_of_Rage,SKILL_ARROW_OF_RAGE,SIN_SKILL_USE_RIGHT,A_Rage_UseMana},

{"Avalanche",			"B\343\354n nh\375\336ng mu\336i t\352n be\354n nho\362n",  // Bắn những mũi tên bén nhọn
30,		41,2,		103,2,
{0,0,0},			{sinWS1,0,0,0,0,0,0,0},
F_Avalanche,SKILL_AVALANCHE,SIN_SKILL_USE_RIGHT,Avalanche_UseMana},

{"Elemental Shot",		"T\342\354n c\364ng li\352n tu\362c d\375\362a tr\352n  hai thu\364\362c ti\354nh L\375\322a va\314 Se\354t",  // Tấn công liên tục dựa trên  hai thuộc tính Lửa và Sét
40,		43,2,		82,2,
{0,0,0},			{sinWS1,0,0,0,0,0,0,0},
F_E_Shot,SKILL_ELEMENTAL_SHOT,SIN_SKILL_USE_RIGHT,Elemental_Shot_UseMana},

{"Golden Falcon",		"Tri\352\362u h\364\314i Chim \335ng t\342\354n c\364ng va\314 phu\362c h\364\314i HP nhanh",  // Triệu hồi Chim Ưng tấn công và phục hồi HP nhanh
43,		52,2,		120,4,
{0,0,0},			{0,0,0,0,0,0,0,0},
F_G_Falcon,SKILL_GOLDEN_FALCON,SIN_SKILL_USE_RIGHT,Golden_Falcon_UseMana},

{"Bomb shot",		    "B\343\354n hai mu\336i t\352n g\342y n\364\322",  // Bắn hai mũi tên gây nổ
46,		46,2,		87,3,
{0,0,0},			{sinWS1,0,0,0,0,0,0,0},
F_B_Shot,SKILL_BOMB_SHOT,SIN_SKILL_USE_RIGHT,BombShot_UseMana},

{"Perforation",		"Xuy\352n t\342m, t\342\354n c\364ng nhi\352\314u    mu\362c ti\352u theo \360\375\365\314ng th\343\314ng",  // Xuyên tâm, tấn công nhiều    mục tiêu theo đường thằng
50,		48,2,		82,2,
{0,0,0},			{sinWS1,0,0,0,0,0,0,0},
F_Perforation,SKILL_PERFORATION,SIN_SKILL_USE_RIGHT,Perforation_UseMana},

{"Recall Wolverine",		"Tri\352\362u t\342\362p so\354i th\342\314n",  // Triệu tập sói thần
60,		58,2,		900,0,
{1,0,0},			{0,0,0,0,0,0,0,0},
F_Recall_Wolverin,SKILL_RECALL_WOLVERIN,SIN_SKILL_USE_RIGHT,Recall_Wolverin_UseMana},

{"Evasion Mastery",	"T\343ng kha\322 n\343ng tra\354nh \360o\314n t\342\354n c\364ng t\375\314 \360\364\354i thu\322 ",  // Tăng khả năng tránh đòn tấn công từ đối thủ 
63,		0,0,		0,0,
{1,0,0},			{0,0,0,0,0,0,0,0},
F_Evasion_Mastery,SKILL_EVASION_MASTERY,SIN_SKILL_USE_NOT,0},

{"Phoenix Shot",	"T\342\362p trung s\375\354c ma\362nh ph\375\365\362ng  hoa\314ng b\342\354t t\375\322 g\342y sa\354t th\375\365ng ha\314ng loa\362t",  // Tập trung sức mạnh phượng  hoàng bất tử gây sát thương hàng loạt
66,		55,2,		100,0,
{1,0,0},			{sinWS1,0,0,0,0,0,0,0},
F_Phoenix_Shot,SKILL_PHOENIX_SHOT,SIN_SKILL_USE_RIGHT,Phoenix_Shot_UseMana},

{"Force of Natural","H\364\362i tu\362 s\375\354c ma\362nh thi\352n nhi\352n t\343ng kha\322 n\343ng chi\352\354n \360\342\354u cho ca\354c tha\314nh vi\352n nho\354m",  // Hội tụ sức mạnh thiên nhiên tăng khả năng chiến đấu cho các thành viên nhóm
70,		56,2,		900,0,
{1,0,0},			{sinWS1,0,0,0,0,0,0,0},
F_Force_Of_Nature,SKILL_FORCE_OF_NATURE,SIN_SKILL_USE_RIGHT,Force_Of_Nature_UseMana},


/*----------------------------------------------------------------------------*
*							Morion
*-----------------------------------------------------------------------------*/	

/////// Knight
{"Sword Blast",			"S\375\322 du\362ng n\343ng l\375\365\362ng cu\322a     ki\352\354m",  // Sử dụng năng lượng của     kiếm
10,		22,2,		82,2,
{0,0,0},			{sinWS2,0,0,0,0,0,0,0},
F_Sword_Blast,SKILL_SWORD_BLAST,SIN_SKILL_USE_ALL,Sword_Blast_UseMana},


{"Holy Body",			"Gia\322m sa\354t th\375\365ng khi bi\362      Undead t\342\354n c\364ng",  // Giảm sát thương khi bị      Undead tấn công
12,		28,2,		110,3,
{0,0,0},			{0,0,0,0,0,0,0,0},
F_HolyBody ,SKILL_HOLY_BODY,SIN_SKILL_USE_RIGHT,HolyBody_UseMana },

{"Physical Training",		"T\343ng th\352m Stamina",  // Tăng thêm Stamina
14,		0,0,		0,0,
{0,0,0},			{0,0,0,0,0,0,0,0},
F_Physical_Traning,SKILL_PHYSICAL_TRANING,SIN_SKILL_USE_NOT,0},

{"Double Crash",		"T\342\354n c\364ng hai chi\352u li\352n tu\362c",  // Tấn công hai chiêu liên tục
17,		35,2,		82,2,
{0,0,0},			{sinWA1,sinWC1,sinWS2,0,0,0,0,0},
F_Double_Crash,SKILL_DOUBLE_CRASH,SIN_SKILL_USE_RIGHT,D_Crash_UseMana},

{"Holy Valor",			"T\343ng s\375\354c g\342y sa\354t th\375\365ng l\352n  Undead cho Party",  // Tăng sức gây sát thương lên  Undead cho Party
20,		36,2,		100,3,
{0,0,0},			{0,0,0,0,0,0,0,0},
F_Holy_Valor,SKILL_HOLY_VALOR,SIN_SKILL_USE_RIGHT,Holy_Valor_Mana},

{"Brandish",			"Ra \360o\314n t\375\314 tr\352n cao g\342y sa\354t th\375\365ng di\352\362n r\364\362ng",  // Ra đòn từ trên cao gây sát thương diện rộng
23,		38,2,		82,2,
{0,0,0},			{sinWP1,sinWS2,0,0,0,0,0,0},
F_Brandish,SKILL_BRANDISH,SIN_SKILL_USE_ALL,Brandish_Mana},

{"Piercing",			"Du\314ng mu\336i ki\352\354m g\342y sa\354t      th\375\365ng cao",  // Dùng mũi kiếm gây sát      thương cao
26,		41,2,		82,2,
{0,0,0},			{sinWS2,0,0,0,0,0,0,0},
F_Piercing,SKILL_PIERCING,SIN_SKILL_USE_ALL,Piercing_Mana},

{"Drastic Spirit",		"T\343ng th\352m s\375\354c pho\314ng thu\322 ",  // Tăng thêm sức phòng thủ 
30,		44,2,		100,2,
{0,0,0},			{0,0,0,0,0,0,0,0},
F_Drastic_Spirit,SKILL_DRASTIC_SPIRIT,SIN_SKILL_USE_RIGHT,Drastic_Spirit_Mana},

{"Sword Mastery",	"T\343ng s\375\354c \360a\354nh khi du\314ng ki\352\354m",  // Tăng sức đánh khi dùng kiếm
40,		0,0,		0,0,
{0,0,0},			{sinWS2,0,0,0,0,0,0,0},
F_S_Mastery,SKILL_SWORD_MASTERY,SIN_SKILL_USE_NOT,0},

{"Divine Shield",		"H\342\354p thu\362 \360o\314n t\342\354n c\364ng cu\322a  Undead va\314 chuy\352\322n m\364\362t ph\342\314n  tha\314nh HP",  // Hấp thụ đòn tấn công của  Undead và chuyển một phần  thành HP
43,		46,2,		130,1,
{0,0,0},			{sinDS1,0,0,0,0,0,0,0},
F_D_Inhalation,SKILL_DIVINE_INHALATION,SIN_SKILL_USE_RIGHT,D_Inhalation_UseMana},

{"Holy Incantation",	"Thu phu\362c Undead",  // Thu phục Undead
46,		50,2,		120,1,
{0,0,0},			{0,0,0,0,0,0,0,0},
F_Holy_Incantation,SKILL_HOLY_INCANTATION,SIN_SKILL_USE_RIGHT,H_Incantation_UseMana},

{"Grand Cross",	"T\342\354n c\364ng hai chi\352u li\352n tu\362c  g\342y sa\354t th\375\365ng cao",  // Tấn công hai chiêu liên tục  gây sát thương cao
50,		45,2,		82,2,
{0,0,0},			{sinWH1,sinWS2,0,0,0,0,0,0},
F_Grand_Cross,SKILL_GRAND_CROSS,SIN_SKILL_USE_RIGHT,Grand_Cross_UseMana},

{"Devine piercing",	"T\343ng kha\322 n\343ng t\342\354n c\364ng b\343\314ng s\375\354c ma\362nh t\342m linh",  // Tăng khả năng tấn công bằng sức mạnh tâm linh
70,		51,2,		160,0,
{1,0,0},			{sinWS2,0,0,0,0,0,0,0},
F_Divine_Piercing,SKILL_DIVINE_PIERCING,SIN_SKILL_USE_RIGHT,Divine_Piercing_UseMana},

{"Godly Shield",	"T\343ng kha\322 n\343ng ch\364\354ng \360\365\336 khi  s\375\322 du\362ng khi\352n",  // Tăng khả năng chống đỡ khi  sử dụng khiên
63,		50,2,		400,0,
{1,0,0},			{sinDS1,0,0,0,0,0,0,0},
F_Godly_Shield,SKILL_GODLY_SHIELD,SIN_SKILL_USE_RIGHT,Godly_Shield_UseMana},

{"God Bless",	"T\343ng kha\322 n\343ng t\342\354n c\364ng b\343\314ng s\375\354c ma\362nh t\342m linh",  // Tăng khả năng tấn công bằng sức mạnh tâm linh
66,		51,2,		330,0,
{1,0,0},			{sinWS2,0,0,0,0,0,0,0},
F_God_Bless,SKILL_GOD_BLESS,SIN_SKILL_USE_RIGHT,God_Bless_UseMana},

{"Sword of Justice","Che\354m ma\362nh t\375\314 tr\352n xu\364\354ng g\342y sa\354t th\375\365ng ha\314ng loa\362t",  // Chém mạnh từ trên xuống gây sát thương hàng loạt
60,		53,2,		250,0,
{1,0,0},			{sinWS2,0,0,0,0,0,0,0},
F_Sword_Of_Justice,SKILL_SWORD_OF_JUSTICE,SIN_SKILL_USE_RIGHT,Sword_Of_Justice_UseMana},

/////////// Atalanta
{"Shield Strike",		"G\342y choa\354ng \360\364\354i thu\322 b\343\314ng    khi\352n",  // Gây choáng đối thủ bằng    khiên
10,		23,2,		82,2,
{0,0,0},			{sinDS1,0,0,0,0,0,0,0},
F_S_Strike ,SKILL_SHIELD_STRIKE,SIN_SKILL_USE_RIGHT,S_Strike_UseMana },

{"Farina",			"Pho\354ng lao ta\362o a\322o a\322nh",  // Phóng lao tạo ảo ảnh
12,		26,2,		82,2,
{0,0,0},			{sinWT1,0,0,0,0,0,0,0},
F_Farina,SKILL_FARINA,SIN_SKILL_USE_ALL,Farina_UseMana},

{"Throwing Mastery",		"T\343ng s\375\354c sa\354t th\375\365ng cu\322a lao",  // Tăng sức sát thương của lao
14,		0,0,		0,0,
{0,0,0},			{sinWT1,0,0,0,0,0,0,0},
F_D_Mastery,SKILL_THROWING_MASTERY,SIN_SKILL_USE_NOT,0},

{"Bigger Spear",		"T\342\354n c\364ng b\343\314ng ngo\362n gia\354o    a\354nh sa\354ng",  // Tấn công bằng ngọn giáo    ánh sáng
17,		32,2,		82,2,
{0,0,0},			{sinWP1,sinWT1,0,0,0,0,0,0},
F_V_Spear,SKILL_VIGOR_SPEAR,SIN_SKILL_USE_ALL,V_Spear_UseMana},

{"Windy",			"T\343ng khoa\322ng ca\354ch t\342\354n c\364ng   va\314 s\375\362 chi\354nh xa\354c cu\322a lao",  // Tăng khoảng cách tấn công   và sự chính xác của lao
20,		34,2,		100,2,
{0,0,0},			{sinWT1,0,0,0,0,0,0,0},
F_Windy,SKILL_WINDY,SIN_SKILL_USE_RIGHT,Windy_Mana},

{"Twist Javelin",		"Pho\354ng lao xoa\354y t\343ng s\375\354c     \360a\354nh va\314 \360\364\362 chi\354nh xa\354c",  // Phóng lao xoáy tăng sức     đánh và độ chính xác
23,		36,2,		82,2,
{0,0,0},			{sinWT1,0,0,0,0,0,0,0},
F_Twist_Javelin,SKILL_TWIST_JAVELIN,SIN_SKILL_USE_RIGHT,Twist_Javelin_Mana},

{"Soul Sucker",			"Hu\354t ma\354u qua\354i v\342\362t",  // Hút máu quái vật
26,		42,2,		110,2,
{0,0,0},			{sinDS1,0,0,0,0,0,0,0},
F_Soul_Sucker,SKILL_SOUL_SUCKER,SIN_SKILL_USE_RIGHT,Soul_Sucker_Mana},

{"Fire Javelin",		"T\342\354n c\364ng b\343\314ng gia\354o l\375\322a",  // Tấn công bằng giáo lửa
30,		40,2,		82,2,
{0,0,0},			{sinWT1,0,0,0,0,0,0,0},
F_Fire_Javelin,SKILL_FIRE_JAVELIN,SIN_SKILL_USE_ALL,Fire_Javelin_Mana},

{"Split javelin",	"T\342\354n c\364ng li\352n tu\362c v\365\354i t\364\354c  \360\364\362 nhanh",  // Tấn công liên tục với tốc  độ nhanh
40,		43,2,		115,1,
{0,0,0},			{sinWT1,0,0,0,0,0,0,0},
F_Split_Javelin,SKILL_SPLIT_JAVELIN,SIN_SKILL_USE_RIGHT,Split_Javelin_UseMana},

{"Triumph of Valhalla",	"T\343ng s\375\354c g\342y sa\354t th\375\365ng cho  Party",  // Tăng sức gây sát thương cho  Party
43,		50,2,		120,1,
{0,0,0},			{sinWT1,sinWS2,sinWS1,sinWP1,sinWH1,sinWA1,sinWC1,sinWM1},
F_T_Of_Valhalla,SKILL_TRIUMPH_OF_VALHALLA,SIN_SKILL_USE_RIGHT,T_Of_Valhalla_UseMana},

{"Lightning javelin",	"T\342\354n c\364ng b\343\314ng gia\354o \360i\352\362n",  // Tấn công bằng giáo điện
46,		46,2,		82,2,
{0,0,0},			{sinWT1,0,0,0,0,0,0,0},
F_Lightning_Javelin,SKILL_LIGHTNING_JAVELIN,SIN_SKILL_USE_ALL,Lightning_Javelin_UseMana},

{"Storm javelin",	    "T\342\354n c\364ng b\343\314ng cu\364\314ng phong",  // Tấn công bằng cuồng phong
50,		48,2,		82,2,
{0,0,0},			{sinWT1,0,0,0,0,0,0,0},
F_Storm_Javelin,SKILL_STORM_JAVELIN,SIN_SKILL_USE_RIGHT,Storm_Javelin_UseMana},

{"Hell of Vahalla",	"T\343ng kha\322 n\343ng t\342\354n c\364ng va\314   tra\354nh \360o\314n",  // Tăng khả năng tấn công và   tránh đòn
60,		52,2,		750,0,
{1,0,0},			{sinWA1,sinWC1,sinWH1,sinWP1,sinWS1,sinWT1,0,0},
F_Hall_Of_Valhalla,SKILL_HALL_OF_VALHALLA,SIN_SKILL_USE_RIGHT,Hall_Of_Valhalla_UseMana},

{"X Rage",	"T\342\354n c\364ng li\352n tu\362c \360\364\354i thu\322  t\375\314 tr\352n cao",  // Tấn công liên tục đối thủ  từ trên cao
63,		48,2,		210,0,
{1,0,0},			{sinWT1,0,0,0,0,0,0,0},
F_X_Rage,SKILL_X_RAGE,SIN_SKILL_USE_RIGHT,X_Rage_UseMana},

{"Frost Javelin",	"T\343ng s\375\354c t\342\354n c\364ng vu\336 khi\354   b\343\314ng s\375\354c ma\362nh b\343ng gia\354 ",  // Tăng sức tấn công vũ khí   bằng sức mạnh băng giá 
66,		50,2,		720,0,
{1,0,0},			{sinWT1,0,0,0,0,0,0,0},
F_Frost_Javelin,SKILL_FROST_JAVELIN,SIN_SKILL_USE_RIGHT,Frost_Javelin_UseMana},

{"Venchence",	"\320o\314n t\342\354n c\364ng li\352n tu\362c nhanh va\314 ma\362nh",  // Đòn tấn công liên tục nhanh và mạnh
70,		55,2,		100,0,
{1,0,0},			{sinWT1,0,0,0,0,0,0,0},
F_Vengeance,SKILL_VENGEANCE,SIN_SKILL_USE_RIGHT,Vengeance_UseMana},

///////////Priestess
{"Healing",			 "Phu\362c h\364\314i ma\354u b\343\314ng phe\354p    thu\342\362t",  // Phục hồi máu bằng phép    thuật
10,		15,1,		82,2,
{0,0,0},			{sinWM1,0,0,0,0,0,0,0},
F_Healing,SKILL_HEALING,SIN_SKILL_USE_RIGHT,Healing_UseMana},

{"Holy Bolt",			"Tri\352\362u h\364\314i c\342\314u se\354t g\342y sa\354t th\375\365ng",  // Triệu hồi cầu sét gây sát thương
12,		11,1,		82,2,
{0,0,0},			{sinWM1,0,0,0,0,0,0,0},
F_HolyBolt,SKILL_HOLY_BOLT,SIN_SKILL_USE_ALL,HolyBolt_UseMana},

{"Multi Spark",			"Go\362i tia l\375\322a ma thu\342\362t t\342\354n   c\364ng mu\362c ti\352u",  // Gọi tia lửa ma thuật tấn   công mục tiêu
14,		14,1,		82,2,
{0,0,0},			{sinWM1,0,0,0,0,0,0,0},
F_M_Spark,SKILL_MULTISPARK,SIN_SKILL_USE_RIGHT,M_Spark_UseMana},

{"Holy Mind",			"La\314m mu\362c ti\352u y\352\354u \360i",  // Làm mục tiêu yếu đi
17,		19,1,		100,2,
{0,0,0},			{sinWM1,0,0,0,0,0,0,0},
F_HolyMind,SKILL_HOLY_MIND,SIN_SKILL_USE_RIGHT,HolyMind_UseMana},

{"Meditation",			"T\343ng kha\322 n\343ng phu\362c h\364\314i Mana",  // Tăng khả năng phục hồi Mana
20,		0,0,		0,0,
{0,0,0},			{0,0,0,0,0,0,0,0},
F_Meditation,SKILL_MEDITATION,SIN_SKILL_USE_NOT,0},

{"Divine Lightning",		"T\342\354n c\364ng b\343\314ng nh\375\336ng tia se\354t thi\352ng",  // Tấn công bằng những tia sét thiêng
23,		21,1,		82,2,
{0,0,0},			{sinWM1,0,0,0,0,0,0,0},
F_Divine_Lightning,SKILL_DIVINE_LIGHTNING,SIN_SKILL_USE_RIGHT,Divine_Lightning_Mana},

{"Holy Reflection",		"Pha\322n h\364\314i \360o\314n t\342\354n c\364ng cu\322a Undead b\343\314ng vo\314ng ma thu\342\362t",  // Phản hồi đòn tấn công của Undead bằng vòng ma thuật
26,		23,1,		120,1,
{0,0,0},			{sinWM1,0,0,0,0,0,0,0},
F_Holy_Reflection,SKILL_HOLY_REFLECTION,SIN_SKILL_USE_RIGHT,Holy_Reflection_Mana},

{"Grand Healing",		"H\364\314i s\375\354c cho ca\322 Party",  // Hồi sức cho cả Party
30,		27,1,		82,2,
{0,0,0},			{sinWM1,0,0,0,0,0,0,0},
F_Grand_Healing,SKILL_GRAND_HEALING,SIN_SKILL_USE_RIGHT,Grand_Healing_Mana},

{"Vigor Ball",			"Ne\354m hai qua\322 c\342\314u Vigor \360\352\354n  mu\362c ti\352u",  // Ném hai quả cầu Vigor đến  mục tiêu
40,		24,1,		82,2,
{0,0,0},			{sinWM1,0,0,0,0,0,0,0},
F_Vigor_Ball,SKILL_VIGOR_BALL,SIN_SKILL_USE_ALL,Vigor_Ball_UseMana},

{"Resurrection",		"H\364\314i sinh m\364\362t tha\314nh vi\352n     Party",  // Hồi sinh một thành viên     Party
43,		31,1,		130,1,
{0,0,0},			{sinWM1,0,0,0,0,0,0,0},
F_Resurrection,SKILL_RESURRECTION,SIN_SKILL_USE_RIGHT,Resurrection_UseMana},

{"Extinction",		"Th\342\314n chu\354 g\342y ra sa\354t th\375\365ng  cho Undead",  // Thần chú gây ra sát thương  cho Undead
46,		28,1,		120,1,
{0,0,0},			{sinWM1,0,0,0,0,0,0,0},
F_Extinction,SKILL_EXTINCTION,SIN_SKILL_USE_RIGHT,Extinction_UseMana},

{"Virtual life",	"T\343ng ma\354u cho tha\314nh vi\352n Party trong m\364\362t khoa\322ng th\365\314i gian nh\342\354t \360i\362nh",  // Tăng máu cho thành viên Party trong một khoảng thời gian nhất định
50,		31,1,		134,1,
{0,0,0},			{sinWM1,0,0,0,0,0,0,0},
F_Virtual_Life,SKILL_VIRTUAL_LIFE,SIN_SKILL_USE_RIGHT,Virtual_Life_UseMana},

{"Glacial Spike",	"Tri\352\362u h\364\314i b\343ng gia\354 t\342\354n c\364ng \360\364\354i thu\322 ",  // Triệu hồi băng giá tấn công đối thủ 
60,		35,1,		120,0,
{1,0,0},			{sinWM1,0,0,0,0,0,0,0},
F_Glacial_Spike,SKILL_GLACIAL_SPIKE,SIN_SKILL_USE_RIGHT,Glacial_Spike_UseMana},

{"Regeneration Field",	"T\343ng kha\322 n\343ng h\364\314i phu\362c HP   va\314 MP cho \360\364\314ng \360\364\362i",  // Tăng khả năng hồi phục HP   và MP cho đồng đội
63,		41,1,		950,0,
{1,0,0},			{sinWM1,0,0,0,0,0,0,0},
F_Regeneration_Field,SKILL_REGENERATION_FIELD,SIN_SKILL_USE_RIGHT,Regeneration_Field_UseMana},

{"Chain Lightening",	"S\375\322 du\362ng s\375\354c ma\362nh cu\322a l\364i  th\342\314n t\342\354n c\364ng nhi\352\314u mu\362c    ti\352u",  // Sử dụng sức mạnh của lôi  thần tấn công nhiều mục    tiêu
66,		41,1,		120,0,
{1,0,0},			{sinWM1,0,0,0,0,0,0,0},
F_Chain_Lightning,SKILL_CHAIN_LIGHTNING,SIN_SKILL_USE_RIGHT,Chain_Lightning_UseMana},

{"Summon Muspell",	"K\352u go\362i s\375\362 giu\354p \360\365\336 cu\322a    ca\354c thi\352n th\342\314n",  // Kêu gọi sự giúp đỡ của    các thiên thần
70,		47,1,		800,0,
{1,0,0},			{sinWM1,0,0,0,0,0,0,0},
F_Summon_Muspell,SKILL_SUMMON_MUSPELL,SIN_SKILL_USE_RIGHT,Summon_Muspell_UseMana},

///////////Magician
{"Agony",			"Chuy\352\322n HP tha\314nh Mana theo ti\322 l\352\362",  // Chuyển HP thành Mana theo tỉ lệ
10,		12,1,		82,2,
{0,0,0},			{sinWM1,0,0,0,0,0,0,0},
F_Agony,SKILL_AGONY,SIN_SKILL_USE_RIGHT,Agony_UseMana},

{"Fire Bolt",			"T\342\354n c\364ng b\343\314ng phe\354p l\375\322a c\365  ba\322n",  // Tấn công bằng phép lửa cơ  bản
12,		11,1,		84,2,
{0,0,0},			{sinWM1,0,0,0,0,0,0,0},
F_FireBolt,SKILL_FIRE_BOLT,SIN_SKILL_USE_ALL,FireBolt_UseMana},

{"Zenith",			"T\343ng c\375\365\314ng kha\322 n\343ng ch\364\354ng   \360\365\336 b\343\314ng phe\354p thu\342\362t",  // Tăng cường khả năng chống   đỡ bằng phép thuật
14,		15,1,		100,3,
{0,0,0},			{sinWM1,0,0,0,0,0,0,0},
F_Zenith,SKILL_ZENITH,SIN_SKILL_USE_RIGHT,Zenith_UseMana},

{"Fire Ball",			"Phe\354p t\342\354n c\364ng thu\364\362c ti\354nh   l\375\322a co\354 t\342\314m sa\354t th\375\365ng r\364\362ng",  // Phép tấn công thuộc tính   lửa có tầm sát thương rộng
17,		19,1,		82,2,
{0,0,0},			{sinWM1,0,0,0,0,0,0,0},
F_FireBall,SKILL_FIRE_BALL,SIN_SKILL_USE_RIGHT,FireBall_UseMana},

{"Mental Mastery",		"T\343ng t\364\322ng s\364\354 mana hi\352\362n co\354 ",  // Tăng tổng số mana hiện có 
20,		0,0,		0,0,
{0,0,0},			{0,0,0,0,0,0,0,0},
F_Mental_Mastery,SKILL_MENTAL_MASTERY,SIN_SKILL_USE_NOT,0},

{"Watorado",			"Phe\354p thu\342\362t t\342\354n c\364ng b\343\314ng   c\364\362t n\375\365\354c xoa\354y",  // Phép thuật tấn công bằng   cột nước xoáy
23,		21,1,		82,2,
{0,0,0},			{sinWM1,0,0,0,0,0,0,0},
F_Watornado,SKILL_WATORNADO,SIN_SKILL_USE_RIGHT,Watornado_Mana},

{"Enchant Weapon",		"\312\354m bu\314a cho vu\336 khi\354 cu\322a     tha\314nh vi\352n party",  // Ếm bùa cho vũ khí của     thành viên party
26,		28,1,		104,2,
{0,0,0},			{sinWM1,0,0,0,0,0,0,0},
F_Enchant_Weapon,SKILL_ENCHANT_WEAPON,SIN_SKILL_USE_RIGHT,Enchant_Weapon_Mana},

{"Death Ray",			"T\342\354n c\364ng b\343\314ng m\364\362t lu\364\314ng    sa\354ng",  // Tấn công bằng một luồng    sáng
30,		26,1,		82,2,
{0,0,0},			{sinWM1,0,0,0,0,0,0,0},
F_Dead_Ray,SKILL_DEAD_RAY,SIN_SKILL_USE_ALL,Dead_Ray_Mana},

{"Energy shield",		"Ta\362o vo\314ng ma thu\342\362t ch\364\354ng \360\365\336 xung quanh c\365 th\352\322",  // Tạo vòng ma thuật chống đỡ xung quanh cơ thể
40,		40,1,		150,1,
{0,0,0},			{sinWM1,0,0,0,0,0,0,0},
F_Energy_Shield,SKILL_ENERGY_SHIELD,SIN_SKILL_USE_RIGHT,Energy_Shield_UseMana},

{"Diastrophism",	"Phe\354p thu\342\362t t\342\354n c\364ng ca\354c    mu\362c ti\352u xung quanh",  // Phép thuật tấn công các    mục tiêu xung quanh
43,		33,1,		106,1,
{0,0,0},			{sinWM1,0,0,0,0,0,0,0},
F_Diastrophism,SKILL_DIASTROPHISM,SIN_SKILL_USE_RIGHT,Diastrophism_UseMana},

{"Spirit elemental",	"Tri\352\362u h\364\314i tinh linh giu\354p gia t\343ng phe\354p thu\342\362t",  // Triệu hồi tinh linh giúp gia tăng phép thuật
46,		35,1,		150,1,
{0,0,0},			{sinWM1,0,0,0,0,0,0,0},
F_Spirit_Elemental,SKILL_SPIRIT_ELEMENTAL,SIN_SKILL_USE_RIGHT,Spirit_Elemental_UseMana},

{"Dancing sword",	"Tri\352\362u t\342\362p ma ki\352\354m t\342\354n c\364ng",  // Triệu tập ma kiếm tấn công
50,		30,1,		150,1,
{0,0,0},			{sinWM1,0,0,0,0,0,0,0},
F_D_Sword,SKILL_DANCING_SWORD,SIN_SKILL_USE_RIGHT,Dancing_Sword_UseMana},

{"Mana Recharging",	"Phu\362c h\364\314i Mana cho ca\354c tha\314nh vi\352n",  // Phục hồi Mana cho các thành viên
60,		120,2,		900,0,
{1,0,0},			{sinWM1,0,0,0,0,0,0,0},
F_Fire_Elemental,SKILL_FIRE_ELEMENTAL,SIN_SKILL_USE_RIGHT,Fire_Elemental_UseMana},

{"Frame Wave",	"T\342\354n c\364ng di\352\362n r\364\362ng b\343\314ng    phe\354p thu\342\362t l\375\322a",  // Tấn công diện rộng bằng    phép thuật lửa
63,		32,1,		124,0,
{1,0,0},			{sinWM1,0,0,0,0,0,0,0},
F_Flame_Wave,SKILL_FLAME_WAVE,SIN_SKILL_USE_RIGHT,Flame_Wave_UseMana},

{"Destruction",	"S\375\322 du\362ng a\354m thu\342\362t la\314m y\352\354u \360\364\354i thu\322 ",  // Sử dụng ám thuật làm yếu đối thủ 
66,		43,1,		460,0,
{1,0,0},			{sinWM1,0,0,0,0,0,0,0},
F_Distortion,SKILL_DISTORTION,SIN_SKILL_USE_RIGHT,Distortion_UseMana},

{"Meteor",	"Tri\352\362u h\364\314i thi\352n tha\362ch t\342\354n  c\364ng nhi\352\314u \360\364\354i thu\322 ",  // Triệu hồi thiên thạch tấn  công nhiều đối thủ 
70,		39,1,		190,0,
{1,0,0},			{sinWM1,0,0,0,0,0,0,0},
F_Meteo,SKILL_M_METEO,SIN_SKILL_USE_RIGHT,Meteo_UseMana},

{"Orb",		"T\343ng kha\322 n\343ng t\342\354n c\364ng b\343\314ng s\375\354c ma\362nh cu\322a Orb",  // Tăng khả năng tấn công bằng sức mạnh của Orb
0,		0,0,		0,0,
{0,0,0},			{0,0,0,0,0,0,0,0},
NULL,SKILL_FORCE_ORB,0,0},

//퀘스트 타임 테이블
{"Quest",		"Ti\314m va\314 di\352\362t qua\354i thu\354!",  // Tìm và diệt quái thú!
0,		0,0,		0,0,
{0,0,0},			{0,0,0,0,0,0,0,0},
NULL,CHANGE_JOB3_QUEST,0,0},

//퀘스트 타임 테이블
{"Quest",		"Ti\314m va\314 di\352\362t qua\354i thu\354!",  // Tìm và diệt quái thú!
0,		0,0,		0,0,
{0,0,0},			{0,0,0,0,0,0,0,0},
NULL,CHANGE_ELEMENTARY_QUEST,0,0},

//공성전 클랜스킬 테이블
{"Absorption of Glory","S\375\354c chi\362u \360\375\362ng +10 trong vo\314ng 1 tu\342\314n",  // Sức chịu đựng +10 trong vòng 1 tuần
0,		0,0,		0,0,
{0,0,0},			{0,0,0,0,0,0,0,0},
NULL,CLANSKILL_ABSORB,0,0},

{"Power Siege","S\375\354c \360a\354nh t\343ng th\352m 10% trong vo\314ng 1 tu\342\314n",  // Sức đánh tăng thêm 10% trong vòng 1 tuần
0,		0,0,		0,0,
{0,0,0},			{0,0,0,0,0,0,0,0},
NULL,CLANSKILL_ATTACK,0,0},

{"Evasion Adaption","Kha\322 n\343ng tra\354nh \360o\314n t\343ng th\352m 10% trong 1 tu\342\314n",  // Khả năng tránh đòn tăng thêm 10% trong 1 tuần
0,		0,0,		0,0,
{0,0,0},			{0,0,0,0,0,0,0,0},
NULL,CLANSKILL_EVASION,0,0},

//공성전 아이템 사용 스킬
{"Atanasia","S\375\354c \360a\354nh gia\322m \360i 1/2 va\314 b\342\354t t\375\322 trong m\364\362t khoa\322ng th\365\314i gian",  // Sức đánh giảm đi 1/2 và bất tử trong một khoảng thời gian
0,		0,0,		0,0,
{0,0,0},			{0,0,0,0,0,0,0,0},
NULL,SCROLL_INVULNERABILITY,0,0},

{"Deadly Edge","T\343ng 5% kha\322 n\343ng ra \360o\314n chi\354 ma\362ng",  // Tăng 5% khả năng ra đòn chí mạng
0,		0,0,		0,0,
{0,0,0},			{0,0,0,0,0,0,0,0},
NULL,SCROLL_CRITICAL,0,0},

{"Avail of Evasion","T\343ng 5% kha\322 n\343ng tra\354nh \360o\314n",  // Tăng 5% khả năng tránh đòn
0,		0,0,		0,0,
{0,0,0},			{0,0,0,0,0,0,0,0},
NULL,SCROLL_EVASION,0,0},

{"Defiance Stone","Gia\322m 50% s\375\354c sa\354t th\375\365ng cu\322a Tha\354p L\375\322a v\365\354i c\365 th\352\322",  // Giảm 50% sức sát thương của Tháp Lửa với cơ thể
0,		0,0,		0,0,
{0,0,0},			{0,0,0,0,0,0,0,0},
NULL,STONE_R_FIRECRYTAL,0,0},

{"Defiance Stone","Gia\322m 50% s\375\354c sa\354t th\375\365ng cu\322a Tha\354p B\343ng v\365\354i c\365 th\352\322",  // Giảm 50% sức sát thương của Tháp Băng với cơ thể
0,		0,0,		0,0,
{0,0,0},			{0,0,0,0,0,0,0,0},
NULL,STONE_R_ICECRYTAL,0,0},

{"Defiance Stone","Gia\322m 50% s\375\354c sa\354t th\375\365ng cu\322a Tha\354p A\354nh Sa\354ng v\365\354i c\365 th\352\322",  // Giảm 50% sức sát thương của Tháp Ánh Sáng với cơ thể
0,		0,0,		0,0,
{0,0,0},			{0,0,0,0,0,0,0,0},
NULL,STONE_R_LINGHTINGCRYTAL,0,0},

{"Might Stone","Gia t\343ng th\352m 20% s\375\354c ma\362nh v\365\354i \360\364\354i thu\322 la\314 Fighter",  // Gia tăng thêm 20% sức mạnh với đối thủ là Fighter
0,		0,0,		0,0,
{0,0,0},			{0,0,0,0,0,0,0,0},
NULL,STONE_A_FIGHTER,0,0},

{"Might Stone","Gia t\343ng th\352m 20% s\375\354c ma\362nh v\365\354i \360\364\354i thu\322 la\314 Mechanician",  // Gia tăng thêm 20% sức mạnh với đối thủ là Mechanician
0,		0,0,		0,0,
{0,0,0},			{0,0,0,0,0,0,0,0},
NULL,STONE_A_MECHANICIAN,0,0},

{"Might Stone","Gia t\343ng th\352m 20% s\375\354c ma\362nh v\365\354i \360\364\354i thu\322 la\314 Pikeman",  // Gia tăng thêm 20% sức mạnh với đối thủ là Pikeman
0,		0,0,		0,0,
{0,0,0},			{0,0,0,0,0,0,0,0},
NULL,STONE_A_PIKEMAN,0,0},

{"Might Stone","Gia t\343ng th\352m 20% s\375\354c ma\362nh v\365\354i \360\364\354i thu\322 la\314 Archer",  // Gia tăng thêm 20% sức mạnh với đối thủ là Archer
0,		0,0,		0,0,
{0,0,0},			{0,0,0,0,0,0,0,0},
NULL,STONE_A_ARCHER,0,0},

{"Might Stone","Gia t\343ng th\352m 20% s\375\354c ma\362nh v\365\354i \360\364\354i thu\322 la\314 Knight",  // Gia tăng thêm 20% sức mạnh với đối thủ là Knight
0,		0,0,		0,0,
{0,0,0},			{0,0,0,0,0,0,0,0},
NULL,STONE_A_KNIGHT,0,0},

{"Might Stone","Gia t\343ng th\352m 20% s\375\354c ma\362nh v\365\354i \360\364\354i thu\322 la\314 Atalanta",  // Gia tăng thêm 20% sức mạnh với đối thủ là Atalanta
0,		0,0,		0,0,
{0,0,0},			{0,0,0,0,0,0,0,0},
NULL,STONE_A_ATALANTA,0,0},

{"Might Stone","Gia t\343ng th\352m 20% s\375\354c ma\362nh v\365\354i \360\364\354i thu\322 la\314 Magician",  // Gia tăng thêm 20% sức mạnh với đối thủ là Magician
0,		0,0,		0,0,
{0,0,0},			{0,0,0,0,0,0,0,0},
NULL,STONE_A_MAGICIAN,0,0},

{"Might Stone","Gia t\343ng th\352m 20% s\375\354c ma\362nh v\365\354i \360\364\354i thu\322 la\314 Priestess",  // Gia tăng thêm 20% sức mạnh với đối thủ là Priestess
0,		0,0,		0,0,
{0,0,0},			{0,0,0,0,0,0,0,0},
NULL,STONE_A_PRIESTESS,0,0},
//프리미엄 부분 유료화 아이템
{"Eternal Life","Tr\365\322 n\352n b\342\354t t\375\322 trong m\364\362t khoa\322ng th\365\314i gian. Nh\375ng s\375\354c \360a\354nh se\336 gia\322m \360i 1/2",  // Trở nên bất tử trong một khoảng thời gian. Nhưng sức đánh sẽ giảm đi 1/2
0,		0,0,		0,0,
{0,0,0},			{0,0,0,0,0,0,0,0},
NULL,SCROLL_P_INVULNERABILITY,0,0},

{"T\375\322 \360ao","T\343ng th\352m 15% kha\322 n\343ng ra \360o\314n chi\354 ma\362ng",  // Tử đao | Tăng thêm 15% khả năng ra đòn chí mạng
0,		0,0,		0,0,
{0,0,0},			{0,0,0,0,0,0,0,0},
NULL,SCROLL_P_CRITICAL,0,0},

{"Bu\314a H\364\362 th\342n","T\343ng th\352m 15% kha\322 n\343ng ne\354 \360o\314n",  // Bùa Hộ thân | Tăng thêm 15% khả năng né đòn
0,		0,0,		0,0,
{0,0,0},			{0,0,0,0,0,0,0,0},
NULL,SCROLL_P_EVASION,0,0},
