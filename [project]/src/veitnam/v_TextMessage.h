#include "TextMessage.h"

char *szAppName="Priston Tale";


char *mgRequestTrade = "G\375\322i y\352u c\342\314u trao \360\364\322i t\365\354i %s";  // Gửi yêu cầu trao đổi tới %s
char *mgRequestTrade2 = "%s \360a\336 \360i qua\354 xa \360\352\322 co\354 th\352\322 trao \360\364\322i";  // %s đã đi quá xa để có thể trao đổi

char *mgDiconnect = "K\352\354t n\364\354i v\365\354i Server bi\362 ng\343\354t";  // Kết nối với Server bị ngắt
char *mgDiconnect1 = "K\352\354t n\364\354i v\365\354i Server bi\362 ng\343\354t (1)";  // Kết nối với Server bị ngắt (1)
char *mgDiconnect2 = "K\352\354t n\364\354i v\365\354i Server bi\362 ng\343\354t (2)";  // Kết nối với Server bị ngắt (2)
char *mgDiconnect3 = "K\352\354t n\364\354i v\365\354i Server bi\362 ng\343\354t (3)";  // Kết nối với Server bị ngắt (3)
char *mgDiconnect4 = "K\352\354t n\364\354i v\365\354i Server bi\362 ng\343\354t (4)";  // Kết nối với Server bị ngắt (4)

char *mgCloseGame = "\320ang thoa\354t kho\322i tro\314 ch\365i";  // Đang thoát khỏi trò chơi
char *mgCloseWindow = "Vui lo\314ng \360o\354ng c\375\322a s\364\322 na\314y la\362i  tr\375\365\354c khi thoa\354t.";  // Vui lòng đóng cửa sổ này lại  trước khi thoát.
char *mgCloseBattle = "Kh\364ng th\352\322 thoa\354t kho\322i tro\314 ch\365i   khi \360ang chi\352\354n \360\342\354u";  // Không thể thoát khỏi trò chơi   khi đang chiến đấu
/*
char *mgQuestName[][4] = {
	{	"Rank up quest"	,	"Rank up quest(Completed)" , "ChangeJob1a.Msg" , "ChangeJob1b.Msg" },
	{	"Rank up quest"	,	"Rank-up Exam (Done)" , "ChangeJob2a.Msg" , "ChangeJob1b.Msg" },
	{	"Rank up quest"	,	"Rank up quest (Completed)" , "ChangeJob3a.Msg" , "ChangeJob1b.Msg" },

	{	"Rank up quest [Second]"		,	"Rank up quest [Second]" , "ChangeJob3-1.msg" , "ChangeJob3-2.msg" },
	{	"Rank up quest [Second]"		,	"Rank up quest (Completed)" , "ChangeJob3-3.msg" , "ChangeJob3-4.msg" },

	//Fighter Pikeman Mecanicion Knight
	{	"Rank up quest second[1]"		,	"Rank up quest second[2]"		, "Quest3\\ChangeJob3-1.msg" , "Quest3\\ChangeJob3-W1-2.msg" },
	{	"Rank up quest second[3]"		,	"Rank up quest second[4]"		, "Quest3\\ChangeJob3-W2-2.msg" , "Quest3\\ChangeJob3-W3-2.msg" },
	{	"Rank up quest second[5]"		,	"Rank up quest second[Completed]"	, "Quest3\\ChangeJob3-3.msg" , "Quest3\\ChangeJob3-4.msg" },

	//Archer Atalanta
	{	"Rank up quest second[1]"		,	"Rank up quest second[2]"		, "Quest3\\ChangeJob3-1.msg" , "Quest3\\ChangeJob3-R1-2.msg" },
	{	"Rank up quest second[3]"		,	"Rank up quest second[4]"		, "Quest3\\ChangeJob3-R2-2.msg" , "Quest3\\ChangeJob3-R3-2.msg" },
	{	"Rank up quest second[5]"		,	"Rank up quest second[Completed]"	, "Quest3\\ChangeJob3-3.msg" , "Quest3\\ChangeJob3-4.msg" },

	//Priestess Magicion
	{	"Rank up quest second[1]"		,	"Rank up quest second[2]"		, "Quest3\\ChangeJob3-1.msg" , "Quest3\\ChangeJob3-M1-2.msg" },
	{	"Rank up quest second[3]"		,	"Rank up quest second[4]"		, "Quest3\\ChangeJob3-M2-2.msg" , "Quest3\\ChangeJob3-M3-2.msg" },
	{	"Rank up quest second[5]"		,	"Rank up quest second[Completed]"	, "Quest3\\ChangeJob3-3.msg" , "Quest3\\ChangeJob3-4.msg" },
	
	//·¹º§º° Äù½ºÆ®
	{	"For her"	,	"For her(completed)" ,	"LevelQuest\\Quest30.Msg" , "LevelQuest\\Quest30a.Msg" },	//14
	{	"The Cave"	,	"The Cave(completed)" ,					"LevelQuest\\Quest55.Msg" , "LevelQuest\\Quest55a.Msg" },	//15
	{	"The Cave"	,	"The Cave(completed)" ,					"LevelQuest\\Quest55_2.Msg" , "LevelQuest\\Quest55_2a.Msg" }, //16
	{	"Friendship with Michelle"	,	"Friendship with Michelle(completed)" ,	"LevelQuest\\Quest70.Msg" , "LevelQuest\\Quest70a.Msg" },	//17
	{	"Enclosed Fury",	"Enclosed Fury(completed)" ,				"LevelQuest\\Quest80.Msg" , "LevelQuest\\Quest80a.Msg" },	//18
	{	"Tears of Kaliha","Tears of Kaliha(completed)" ,				"LevelQuest\\Quest85.Msg" , "LevelQuest\\Quest85a.Msg" },	//19
	{	"Village of eura","Village of eura(completed)" ,					"LevelQuest\\Quest90.Msg" , "LevelQuest\\Quest90a.Msg" },	//20

	//보너스 스텟 퀘스트
	{	"Test of Kingdom",	"Test of Kingdom(Complete)" ,	"LevelQuest\\Quest_7State_1.msg" , "LevelQuest\\Quest_7State_end.msg" },	//21
	{	"Test of Kingdom",	"Test of Kingdom(Complete)",	"LevelQuest\\Quest_7State_2.msg" , "LevelQuest\\Quest_7State_end.msg" },	//22
	{	"Test of Kingdom",	"Test of Kingdom(Complete)",	"LevelQuest\\Quest_7State_3.msg" , "LevelQuest\\Quest_7State_end.msg" },	//23

	{	"Bitter ordeal"	,	"Bitter ordeal(Complete)" ,	"LevelQuest\\Quest_10State_1.msg" , "LevelQuest\\Quest_10State_end.msg" },	//24
	{	"Bitter ordeal"	,	"Bitter ordeal(Complete)" ,	"LevelQuest\\Quest_10State_2_1.msg" , "LevelQuest\\Quest_10State_end.msg" },//25
	{	"Bitter ordeal"	,	"Bitter ordeal(Complete)" ,	"LevelQuest\\Quest_10State_2_2.msg" , "LevelQuest\\Quest_10State_end.msg" },//26
	{	"Bitter ordeal"	,	"Bitter ordeal(Complete)" ,	"LevelQuest\\Quest_10State_2_3.msg" , "LevelQuest\\Quest_10State_end.msg" },//27

	//3차 전업 퀘스트
	{	"Job Quest 3th[1]"	,	"Job Quest 3th(Complete)" ,	"Quest4\\ChangeJob4_1.msg" ,	"Quest4\\ChangeJob4_5.msg" },	//28
	{	"Job Quest 3th[2]"	,	"Job Quest 3th(Complete)" ,	"Quest4\\ChangeJob4_2.msg" ,	"Quest4\\ChangeJob4_5.msg" },	//29
	{	"Job Quest 3th[3]"	,	"Job Quest 3th(Complete)" ,	"Quest4\\ChangeJob4_3_1.msg" ,	"Quest4\\ChangeJob4_3_2.msg" },	//30
	{	"Job Quest 3th[3]"	,	"Job Quest 3th(Complete)" ,	"Quest4\\ChangeJob4_3_3.msg" ,	"Quest4\\ChangeJob4_3_4.msg" },	//31
	{	"Job Quest 3th[3]"	,	"Job Quest 3th(Complete)" ,	"Quest4\\ChangeJob4_3_5.msg" ,	"Quest4\\ChangeJob4_3_6.msg" },	//32
	{	"Job Quest 3th[4]"	,	"Job Quest 3th(Complete)" ,	"Quest4\\ChangeJob4_4.msg" ,	"Quest4\\ChangeJob4_5.msg" }	//33

};
*/
char *mgRefuseWhisper = "Tin nh\343\354n ri\352ng \360a\336 bi\362 t\375\314 ch\364\354i";  // Tin nhắn riêng đã bị từ chối
char *mgWeightOver = "Kh\364ng th\352\322 mang th\352m \360\375\365\362c n\375\336a ";  // Không thể mang thêm được nữa 

char *mgYahoo = "Yes!";
char *mgContinueChat = "%s : T\364i no\354i nhi\352\314u \360\352\354n m\375\354c c\375\354ng ca\322 l\375\365\336i la\362i r\364\314i! WOW q^_^p";  // %s : Tôi nói nhiều đến mức cứng cả lưỡi lại rồi! WOW q^_^p
char *mgRecvItem = "  Ba\362n \360a\336 nh\342\362n \360\375\365\362c mo\354n \360\364\314 ( %s )  ";  //   Bạn đã nhận được món đồ ( %s )  

char *mgItemTimeOut = "Mo\354n \360\364\314 \360a\336 bi\362 xoa\354 ( %s ) v\375\365\362t qua\354 th\365\314i ha\362n s\375\322 du\362ng cho phe\354p ";  // Món đồ đã bị xoá ( %s ) vượt quá thời hạn sử dụng cho phép 
char *mgSOD_Clear = " Chu\354c m\375\314ng! Ba\362n \360a\336 nh\342\362n \360\375\365\362c gia\322i th\375\365\322ng( %d Va\314ng) ";  //  Chúc mừng! Bạn đã nhận được giải thưởng( %d Vàng) 

char *mgBlessCrystal_01 = "  Gi\365\354i ha\362n cho vi\352\362c tri\352\362u t\342\362p cu\322a ba\362n \360a\336 h\352\354t  ";  //   Giới hạn cho việc triệu tập của bạn đã hết  
char *mgBlessCrystal_02 = "   Ba\362n chi\322 co\354 th\352\322 tri\352\362u t\342\362p m\364\336i l\342\314n m\364\362t qua\354i v\342\362t   ";  //    Bạn chỉ có thể triệu tập mỗi lần một quái vật   
char *mgBlessCrystal_03 = "   Ba\362n kh\364ng th\352\322 s\375\322 du\362ng vi\352n ngo\362c na\314y  ";  //    Bạn không thể sử dụng viên ngọc này  


// Bellatra
char *g_lpBellatraTeamNameText[] = 
{
	"Th\375\322 tha\354ch cu\322a N\375\365\354c",  // Thử thách của Nước
	"Th\375\322 tha\354ch cu\322a \320\342\354t",  // Thử thách của Đất
	"Th\375\322 tha\354ch cu\322a Gio\354 ",  // Thử thách của Gió 
	"Th\375\322 tha\354ch cu\322a L\375\322a",  // Thử thách của Lửa
	"T\342\314ng Nh\342\362t Quan",  // Tầng Nhật Quan
	"T\342\314ng Nguy\352\362t Quang",  // Tầng Nguyệt Quang
};

char *g_lpBellatraResult_Text1 = "%s of";
char *g_lpBellatraResult_Text2 = "%s and %s of";
char *g_lpBellatraResult_Text3 = "Join team";
char *g_lpBellatraResult_Text4 = "Chuy\352\322n l\352n t\342\314ng cao h\365n.";  // Chuyển lên tầng cao hơn.
