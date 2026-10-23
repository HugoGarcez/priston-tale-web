#include "sinLinkHeader.h"

sMESSAGEBOX	sMessageBox[100] = {
	{"Kh\364ng \360u\322 khoa\322ng tr\364\354ng trong ng\343n ch\375\354a \360\364\314  "},  // Không đủ khoảng trống trong ngăn chứa đồ  
	{"V\375\365\362t qua\354 c\342n n\343\362ng cho phe\354p "},     // Vượt quá cân nặng cho phép 
	{"Mo\354n \360\364\314 na\314y kh\364ng th\352\322 s\375\322 du\362ng \360\375\365\362c "},	     // Món đồ này không thể sử dụng được 
	{"Kh\364ng \360u\322 va\314ng "},  // Không đủ vàng 
	{"Mo\354n \360\364\314 na\314y kh\364ng th\352\322 Mix \360\375\365\362c "},  // Món đồ này không thể Mix được 
	{"V\375\365\362t qua\354 gi\365\354i ha\362n cu\322a s\364\354 l\375\365\362ng \360\364\314 \360\375\365\362c trao \360\364\322i "},	// short 64 (done)  // Vượt quá giới hạn của số lượng đồ được trao đổi 
	{"Giao di\362ch bi\362 huy\322 bo\322  "},  // Giao dịch bị huỷ bỏ  
	{"Kh\364ng th\352\322 l\375u tr\375\336 Potions "},  // Không thể lưu trữ Potions 
	{"Mix mo\354n \360\364\314 na\314y th\342\354t ba\362i "},  // Mix món đồ này thất bại 
	{"Ca\322m \365n s\375\362 \360o\354ng go\354p quy\354 ba\354u cu\322a ba\362n "},  // Cảm ơn sự đóng góp quý báu của bạn 
	{"Co\354 qua\354 nhi\352\314u mo\354n \360\364\314 trong nha\314 kho "},  // Có quá nhiều món đồ trong nhà kho 
	{"Mo\354n \360\364\314 na\314y kh\364ng th\352\322 Age \360\375\365\362c "},  // Món đồ này không thể Age được 
	{"M\364\362t va\314i \360\364\314 v\342\362t \360a\336 bi\362 thay \360\364\322i. Vui lo\314ng ki\352\322m tra la\362i "},  // Một vài đồ vật đã bị thay đổi. Vui lòng kiểm tra lại 
	{"V\375\365\362t qua\354 gi\365\354i ha\362n va\314ng \360\375\365\362c phe\354p s\365\322 h\375\336u "},   // Vượt quá giới hạn vàng được phép sở hữu 
	{"Age mo\354n \360\364\314 na\314y th\342\354t ba\362i "},  // Age món đồ này thất bại 
	{"\320i\352\322m Statistics \360a\336 \360\375\365\362c thi\352\354t l\342\362p la\362i. "},  // Điểm Statistics đã được thiết lập lại. 
	{"Item image isn't prepared "},
	{"Ba\362n chi\322 co\354 th\352\322 la\314m \360i\352\314u na\314y m\364\362t l\342\314n th\364i "},  // Bạn chỉ có thể làm điều này một lần thôi 
	{"Chi\322 da\314nh cho tha\314nh vi\352n cu\322a Clan "},		// short 64 (done)  // Chỉ dành cho thành viên của Clan 
	{"Ba\362n pha\322i gia nh\342\362p m\364\362t Clan m\365\354i co\354 th\352\322 s\375\322 du\362ng "},		// short 64 (done)  // Bạn phải gia nhập một Clan mới có thể sử dụng 
	{"Kh\364ng th\352\322 di\362ch chuy\352\322n vi\314 gi\365\354i ha\362n Level "},  // Không thể dịch chuyển vì giới hạn Level 
	{"C\342\354p \360\364\362 Age \360a\336 bi\362 tr\375\314 \360i -1 "},  // Cấp độ Age đã bị trừ đi -1 
	{"C\342\354p \360\364\362 Age \360a\336 bi\362 tr\375\314 \360i -2 "},  // Cấp độ Age đã bị trừ đi -2 
	{"C\342\354p \360\364\362 Age \360a\336 \360\375\365\362c t\343ng l\352n +1 "},  // Cấp độ Age đã được tăng lên +1 
	{"C\342\354p \360\364\362 Age \360a\336 \360\375\365\362c t\343ng l\352n +2 "},  // Cấp độ Age đã được tăng lên +2 
	{"T\364\354c \360\364\362 ATK cu\322a mo\354n \360\364\314 nhi\352\362m vu\362  "},  // Tốc độ ATK của món đồ nhiệm vụ  
	{"   N\342ng level mo\354n \360\364\314 nhi\352\362m vu\362 "},  //    Nâng level món đồ nhiệm vụ 
	{"   HP \360\375\365\362c t\343ng th\352m +15 "},  //    HP được tăng thêm +15 
	{"   Puzzle \360a\336 hoa\314n t\342\354t  "},  //    Puzzle đã hoàn tất  
	{"   Puzzle s\343\354p x\352\354p kh\364ng \360u\354ng "},  //    Puzzle sắp xếp không đúng 
	{"   Kh\364ng co\354 gi\314 \360\352\322 ba\354n ca\322  "},  //    Không có gì để bán cả  
	{"Mo\354n \360\364\314 na\314y \360a\336 \360\375\365\362c ba\354n r\364\314i "},  // Món đồ này đã được bán rồi 
	{"Gian ha\314ng ca\354 nh\342n \360a\336 bi\362 \360o\354ng la\362i "},  // Gian hàng cá nhân đã bị đóng lại 
	{"   Kh\364ng ti\314m th\342\354y ng\375\365\314i ba\354n "},  //    Không tìm thấy người bán 
	{"    S\364\354 l\375\365\362ng mo\354n \360\364\314 kh\364ng \360u\354ng  "},  //     Số lượng món đồ không đúng  
	{"    Nh\342n v\342\362t nam kh\364ng th\352\322 s\375\322 du\362ng "},  //     Nhân vật nam không thể sử dụng 
	{"    Nh\342n v\342\362t n\375\336 kh\364ng th\352\322 s\375\322 du\362ng "},  //     Nhân vật nữ không thể sử dụng 
	{" Nh\342n v\342\362t n\375\336 kh\364ng th\352\322 s\375\322 du\362ng \360\375\365\362c mo\354n \360\364\314 na\314y "},  //  Nhân vật nữ không thể sử dụng được món đồ này 
	{" Nh\342n v\342\362t nam kh\364ng th\352\322 s\375\322 du\362ng \360\375\365\362c mo\354n \360\364\314 na\314y "},  //  Nhân vật nam không thể sử dụng được món đồ này 
	{"    Gia\354 cu\322a mo\354n \360\364\314 \360a\336 thay \360\364\322i "},  //     Giá của món đồ đã thay đổi 
	{"    Kh\364ng co\354 ti\352\314n th\375\365\322ng cho Clan "},  //     Không có tiền thưởng cho Clan 
	{"  Level cu\322a ba\362n ch\375a th\352\322 du\314ng \360\375\365\362c loa\362i force na\314y   "},  //   Level của bạn chưa thể dùng được loại force này   
	{"     Force Orb \360ang \360\375\365\362c s\375\322 du\362ng  "},  //      Force Orb đang được sử dụng  
	{"     Ch\375\354c n\343ng ATK POW \360ang \360\375\365\362c th\375\322 nghi\352\362m    "},  //      Chức năng ATK POW đang được thử nghiệm    
	{"    Qua\354 tri\314nh Force Orb \360a\336 hoa\314n t\342\354t     "},  //     Quá trình Force Orb đã hoàn tất     
	{"    \320i\352\322m Stats \360\375\365\362c t\343ng th\352m 5  "},  //     Điểm Stats được tăng thêm 5  
	{"    \320i\352\322m Skill \360\375\365\362c t\343ng th\352m 1  "},  //     Điểm Skill được tăng thêm 1  
	{" \320i\352\322m Stats & Skill \360\375\365\362c t\343ng th\352m 5 va\314 1 "},  //  Điểm Stats & Skill được tăng thêm 5 và 1 
	{" Stat & skill point \360\375\365\362c t\343ng th\352m 5 va\314 2 "},  //  Stat & skill point được tăng thêm 5 và 2 
	{"   Ph\342\314n tr\343m XP bi\362 m\342\354t khi ch\352\354t gia\322m \360i 1   "},  //    Phần trăm XP bị mất khi chết giảm đi 1   
	{"   Vitality t\343ng th\352m 40   " },  //    Vitality tăng thêm 40   
	{"    Ba\362n \360ang th\375\362c hi\352\362n nhi\352\362m vu\362     " },  //     Bạn đang thực hiện nhiệm vụ     
	{"T\375\314 level 80, ba\362n se\336 \360\375\365\362c 7 \360i\352\322m khi \360a\362t th\352m 1 level." },  // Từ level 80, bạn sẽ được 7 điểm khi đạt thêm 1 level.
	{"T\375\314 level 90, ba\362n se\336 \360\375\365\362c 10 \360i\352\322m khi \360a\362t th\352m 1 level." },  // Từ level 90, bạn sẽ được 10 điểm khi đạt thêm 1 level.
	{"Resetted your getting-number for tranning." },
	{"Nhi\352\362m vu\362 kh\364ng th\352\322 hoa\314n tha\314nh do h\352\354t th\365\314i gian cho phe\354p" },  // Nhiệm vụ không thể hoàn thành do hết thời gian cho phép
	{"Co\354 qua\354 nhi\352\314u li\354nh \360a\354nh thu\352. "},  // Có quá nhiều lính đánh thuê. 
	{"M\364\362t mo\354n \360\364\314 cu\314ng loa\362i \360ang \360\375\365\362c s\375\322 du\362ng. "},  // Một món đồ cùng loại đang được sử dụng. 
	{"Kh\364ng th\352\322 di\362ch chuy\352\322n t\365\354i Bless Castle "},	  // Không thể dịch chuyển tới Bless Castle 
	{"Mo\354n \360\364\314 na\314y kh\364ng th\352\322 ba\354n trong gian ha\314ng ca\354 nh\342n "},  // Món đồ này không thể bán trong gian hàng cá nhân 
	{"Kh\364ng co\354 \360u\322 s\364\354 ti\352\314n thu\352\354 \360\352\322 ru\354t ra. "},  // Không có đủ số tiền thuế để rút ra. 
	{"Ba\362n \360a\336 tra\322 l\365\314i c\342u ho\322i na\314y r\364\314i. "},  // Bạn đã trả lời câu hỏi này rồi. 
	{"Ca\322m \365n ba\362n \360a\336 tra\322 l\365\314i c\342u ho\322i. "},   // Cảm ơn bạn đã trả lời câu hỏi. 
	{"Level cu\322a mo\354n \360\364\314 kh\364ng phu\314 h\365\362p. "},  // Level của món đồ không phù hợp. 
	{"Mo\354n \360\364\314 \360a\336 \360\375\365\362c bi\352\354n \360\364\322i hoa\314n t\342\354t. "},  // Món đồ đã được biến đổi hoàn tất. 
	{"Chi\352\354n th\343\354ng trong tr\342\362n \360\342\354u v\365\354i Fury. "},  // Chiến thắng trong trận đấu với Fury. 
	{"Ba\362n \360a\336 thua trong tr\342\362n \360\342\354u v\365\354i Fury "},  // Bạn đã thua trong trận đấu với Fury 
	{"Acquierd items in compensation. "},
	{"Ca\354c mo\354n \360\364\314 na\314y kh\364ng th\352\322 s\375\322 du\362ng \360\375\365\362c cu\314ng m\364\362t lu\354c. "},  // Các món đồ này không thể sử dụng được cùng một lúc. 
	{"Able to use later "},
	{"Kh\364ng th\352\322 huy\322 bo\322 khi ca\354c Sheltom \360ang \360\375\365\362c Mix/Age. "},  // Không thể huỷ bỏ khi các Sheltom đang được Mix/Age. 
	{"Kh\364ng th\352\322 s\375\322 du\362ng khi ca\354c Sheltom \360ang \360\375\365\362c Mix/Age. "},  // Không thể sử dụng khi các Sheltom đang được Mix/Age. 
	{"Cu\314ng m\364\362t ki\352\322u to\354c \360ang \360\375\365\362c s\375\322 du\362ng. "},  // Cùng một kiểu tóc đang được sử dụng. 
	{"Qua\354 tri\314nh luy\352\362n kim th\342\354t ba\362i. "}, // pluto 제련 실패 메세지  // Quá trình luyện kim thất bại. 
	{"Qua\354 tri\314nh ch\352\354 ta\362o th\342\354t ba\362i. "}, // pluto 제작 실패 메세지  // Quá trình chế tạo thất bại. 
	{"Trong khi tr\364\362n Sheltoms, kh\364ng th\352\322 s\375\322 du\362ng Ore/Crystal. "}, // pluto  // Trong khi trộn Sheltoms, không thể sử dụng Ore/Crystal. 
	{"Trong khi tr\364\362n Runes, kh\364ng th\352\322 s\375\322 du\362ng Recipe. "}, // pluto  // Trong khi trộn Runes, không thể sử dụng Recipe. 
	{"Ore/Crystal kh\364ng cu\314ng loa\362i "}, // pluto  // Ore/Crystal không cùng loại 
	{"C\364ng th\375\354c ch\352\354 ta\362o kh\364ng \360u\354ng "}, // pluto MESSAGE_NOT_NEED_RUNE  // Công thức chế tạo không đúng 
};



sMESSAGEBOX2 sMessageBox2[20] = {
	{"Bao nhi\352u mo\354n \360\364\314 ","ba\362n mu\364\354n mua? "},  // Bao nhiêu món đồ  | bạn muốn mua? 
	{"Bao nhi\352u mo\354n \360\364\314 ","ba\362n mu\364\354n ba\354n? "},  // Bao nhiêu món đồ  | bạn muốn bán? 
	{"Bao nhi\352u mo\354n \360\364\314 ","ba\362n mu\364\354n qu\343\322ng \360i? "},  // Bao nhiêu món đồ  | bạn muốn quẳng đi? 
	{"Bao nhi\352u va\314ng","ba\362n mu\364\354n qu\343\322ng \360i? "},  // Bao nhiêu vàng | bạn muốn quẳng đi? 
	{""," "}, 
	{"Bao nhi\352u va\314ng"," ba\362n mu\364\354n trao \360\364\322i? "},  // Bao nhiêu vàng |  bạn muốn trao đổi? 
	{"Bao nhi\352u va\314ng","ba\362n mu\364\354n c\342\354t \360i? "},  // Bao nhiêu vàng | bạn muốn cất đi? 
	{"Bao nhi\352u va\314ng","ba\362n mu\364\354n l\342\354y la\362i? "},  // Bao nhiêu vàng | bạn muốn lấy lại? 
	{" Bao nhi\352u ng\364i sao ","ba\362n mu\364\354n mua? "},  //  Bao nhiêu ngôi sao  | bạn muốn mua? 
	{"  Bao nhi\352u ti\352\314n  ","ba\362n mu\364\354n quy\352n go\354p? "},   //   Bao nhiêu tiền   | bạn muốn quyên góp? 
	{"Do you wanna register","gia\354 ti\352\314n cu\322a mo\354n \360\364\314?  "},   // giá tiền của món đồ?  
	{"Bao nhi\352u mo\354n \360\364\314 ","ba\362n mu\364\354n mua? "},    // Bao nhiêu món đồ  | bạn muốn mua? 
	{"Ba\362n mu\364\354n \360\343\362t gia\354 ","ba\362n mu\364\354n l\342\354y la\362i? "},   // Bạn muốn đặt giá  | bạn muốn lấy lại? 
	{"  Ba\362n co\354 mu\364\354n ","mua mo\354n \360\364\314 na\314y kh\364ng? "},   //   Bạn có muốn  | mua món đồ này không? 
	{"  Ba\362n co\354 mu\364\354n  ","mua mo\354n \360\364\314 na\314y kh\364ng? "},  //   Bạn có muốn   | mua món đồ này không? 
	{"Bao nhi\352u ti\352\314n","ba\362n mu\364\354n ru\354t ra? "},//공성전 돈찾기  // Bao nhiêu tiền | bạn muốn rút ra? 
	{"Bao nhi\352u li\354nh \360a\354nh thu\352 ","ba\362n mu\364\354n mua? "} //용병 구입  // Bao nhiêu lính đánh thuê  | bạn muốn mua? 
};



sMESSAGEBOX3 sMessageBox3[MESSAGE3_MAX] = {
	{"Ba\362n co\354 mu\364\354n","ho\362c Skill na\314y kh\364ng? "},  // Bạn có muốn | học Skill này không? 
	{"Ba\362n co\354 mu\364\354n", "phong ch\375\354c kh\364ng? "},  // Bạn có muốn | phong chức không? 
	{"Ba\362n co\354 mu\364\354n", "th\375\362c hi\352\362n nhi\352\362m vu\362 kh\364ng? "},  // Bạn có muốn | thực hiện nhiệm vụ không? 
	{"Ba\362n co\354 mu\364\354n","ho\362c la\362i ca\354c Skill kh\364ng? "},		// short 32 (done)  // Bạn có muốn | học lại các Skill không? 
	{"Se\336 t\364\354n va\314ng"," cho s\375\362 ki\352\362n na\314y "},  // Sẽ tốn vàng |  cho sự kiện này 
	{"Ba\362n co\354 mu\364\354n","\360i\352\314u chi\322nh la\362i STATS kh\364ng? "},  // Bạn có muốn | điều chỉnh lại STATS không? 
	{"Ba\362n co\354 mu\364\354n", "th\375\362c hi\352\362n nhi\352\362m vu\362 kh\364ng? "},  // Bạn có muốn | thực hiện nhiệm vụ không? 
	{"Ba\362n co\354 mu\364\354n", "nh\342\362n \360\364\314 kh\364ng? "}, //PostBox  // Bạn có muốn | nhận đồ không? 
	{"Ba\362n co\354 mu\364\354n", "ky\354 va\314o Union Core kh\364ng? "}, //LinkCore  // Bạn có muốn | ký vào Union Core không? 
	{"Ba\362n co\354 mu\364\354n", "nh\342\362n \360\364\314 kh\364ng? "}, //RecvOthers  // Bạn có muốn | nhận đồ không? 
	{"Ba\362n co\354 mu\364\354n", "chuy\352\322n t\365\354i ? "}, //RecvOthers  // Bạn có muốn | chuyển tới ? 
	{"","Vui lo\314ng nh\342\362p va\314o ch\375\336 s\364\354 "}, //RecvOthers		// short 32 (done)  // Vui lòng nhập vào chữ số 
	{"Ba\362n co\354 mu\364\354n","purchase star item? "}, //RecvOthers  // Bạn có muốn
	{"Ba\362n co\354 mu\364\354n", "ba\354n v\342\362t na\314y kh\364ng? "},   // Bạn có muốn | bán vật này không? 
	{"Ba\362n co\354 mu\364\354n", "nhi\352\362m vu\362? "},   // Bạn có muốn | nhiệm vụ? 
	{" Ba\362n co\354 mu\364\354n", "quy\352n go\354p kh\364ng? "},   //  Bạn có muốn | quyên góp không? 
	{" Private Store ", "fffffffffff "}, 
	{"Ba\362n co\354 mu\364\354n", "\360\364\322i mo\354n \360\364\314 kh\364ng?  "},   // Bạn có muốn | đổi món đồ không?  
	{"Ba\362n co\354 mu\364\354n", "nh\375\336ng mo\354n \360\364\314? "},   // Bạn có muốn | những món đồ? 
	{"Ba\362n co\354 mu\364\354n ", "huy\322 bo\322 \360\343ng ky\354? "},   // Bạn có muốn  | huỷ bỏ đăng ký? 
	{"Ba\362n co\354 mu\364\354n", "\360\364\322i mo\354n \360\364\314?  "},   // Bạn có muốn | đổi món đồ?  
	{"Ba\362n co\354 mu\364\354n", "\360\364\322i mo\354n \360\364\314?  "},   // Bạn có muốn | đổi món đồ?  
	{"Ba\362n co\354 mu\364\354n", "th\375\362c hi\352\362n nhi\352\362m vu\362 kh\364ng? "}, //Level30  // Bạn có muốn | thực hiện nhiệm vụ không? 
	{"Ba\362n co\354 mu\364\354n", "th\375\362c hi\352\362n nhi\352\362m vu\362 kh\364ng? "}, //Level55  // Bạn có muốn | thực hiện nhiệm vụ không? 
	{"Ba\362n co\354 mu\364\354n", "th\375\362c hi\352\362n nhi\352\362m vu\362 kh\364ng? "}, //Level55_2  // Bạn có muốn | thực hiện nhiệm vụ không? 
	{"Ba\362n co\354 mu\364\354n", "th\375\362c hi\352\362n nhi\352\362m vu\362 kh\364ng? "}, //Level70  // Bạn có muốn | thực hiện nhiệm vụ không? 
	{"Ba\362n co\354 mu\364\354n", "th\375\362c hi\352\362n nhi\352\362m vu\362 kh\364ng? "}, //Level80  // Bạn có muốn | thực hiện nhiệm vụ không? 
	{"Ba\362n co\354 mu\364\354n", "th\375\362c hi\352\362n nhi\352\362m vu\362 kh\364ng? "}, //Level85  // Bạn có muốn | thực hiện nhiệm vụ không? 
	{"Ba\362n co\354 mu\364\354n", "th\375\362c hi\352\362n nhi\352\362m vu\362 kh\364ng? "}, //Level90  // Bạn có muốn | thực hiện nhiệm vụ không? 
	{"N\365i na\314o", "ba\362n mu\364\354n di chuy\352\322n t\365\354i "}, //텔레포트  // Nơi nào | bạn muốn di chuyển tới 
	{"Ba\362n co\354 mu\364\354n", "th\375\362c hi\352\362n nhi\352\362m vu\362 kh\364ng? "}, //Level80  // Bạn có muốn | thực hiện nhiệm vụ không? 
	{"Ba\362n co\354 mu\364\354n", "th\375\362c hi\352\362n nhi\352\362m vu\362 kh\364ng? "}, //Level90_2  // Bạn có muốn | thực hiện nhiệm vụ không? 
	{"Ba\362n co\354 mu\364\354n", "th\375\362c hi\352\362n nhi\352\362m vu\362 kh\364ng? "}, //4Â÷ÀüÁ÷  // Bạn có muốn | thực hiện nhiệm vụ không? 
	{"Ba\362n co\354 mu\364\354n", "th\343ng ha\362ng kh\364ng? "}, //4Â÷ ÀüÁ÷  // Bạn có muốn | thăng hạng không? 
	{"Ba\362n co\354 mu\364\354n", "trao \360\364\322i mo\354n \360\364\314?  "},//일곱개의 크리스탈을 아이템으로 교환하기  // Bạn có muốn | trao đổi món đồ?  
	{"Ba\362n mu\364\354n di\362ch chuy\352\322n", "t\365\354i \360\342y kh\364ng? "},    //Teleport  // Bạn muốn dịch chuyển | tới đây không? 
	{"Ba\362n co\354 mu\364\354n mua","m\364\362t toa\314 tha\354p kh\364ng? "},      //크리스탈 타워를 구입하다.  // Bạn có muốn mua | một toà tháp không? 
	{"Ba\362n co\354 mu\364\354n", "th\375\362c hi\352\362n nhi\352\362m vu\362 kh\364ng? "}, //A  // Bạn có muốn | thực hiện nhiệm vụ không? 
	{"Ba\362n co\354 mu\364\354n", "th\375\362c hi\352\362n nhi\352\362m vu\362 kh\364ng? "}, //B  // Bạn có muốn | thực hiện nhiệm vụ không? 
	{"Ba\362n co\354 mu\364\354n", "th\375\362c hi\352\362n nhi\352\362m vu\362 kh\364ng? "}, //C  // Bạn có muốn | thực hiện nhiệm vụ không? 
	{"Ba\362n co\354 mu\364\354n", "th\375\362c hi\352\362n nhi\352\362m vu\362 kh\364ng? "}, //D  // Bạn có muốn | thực hiện nhiệm vụ không? 
	{"Ba\362n co\354 mu\364\354n", "th\375\362c hi\352\362n nhi\352\362m vu\362 kh\364ng? "}, //E  // Bạn có muốn | thực hiện nhiệm vụ không? 
	{"Ba\362n co\354 mu\364\354n", "th\375\362c hi\352\362n nhi\352\362m vu\362 kh\364ng? "}, //F  // Bạn có muốn | thực hiện nhiệm vụ không? 
	{"Ba\362n co\354 mu\364\354n", "th\375\362c hi\352\362n nhi\352\362m vu\362 kh\364ng? "}, //G  // Bạn có muốn | thực hiện nhiệm vụ không? 
	{"Ba\362n co\354 mu\364\354n", "s\375\322 du\362ng mo\354n \360\364\314 na\314y? "},       //구매 아이템 사용  // Bạn có muốn | sử dụng món đồ này? 
	{"Ba\362n co\354 mu\364\354n ", "th\375\362c hi\352\362n nhi\352\362m vu\362 kh\364ng? "},//100LV퀘스트 수해여부   // Bạn có muốn  | thực hiện nhiệm vụ không? 
	{"Ba\362n co\354 mu\364\354n ", "nh\342\362n mo\354n \360\364\314 kh\364ng? "},   //100LV아이템   // Bạn có muốn  | nhận món đồ không? 
	{"Ba\362n co\354 mu\364\354n","di chuy\352\322n t\365\354i Zone kh\364ng? "},   //100LV퀘스트 대련장 이동여부  // Bạn có muốn | di chuyển tới Zone không? 
	{"Ba\362n co\354 mu\364\354n", "di\362ch chuy\352\322n kh\364ng? "},  // Bạn có muốn | dịch chuyển không? 
	{"Di\362ch chuy\352\322n", "Ba\362n ch\343\354c ch\343\354n ch\375\354? "},    //횡스크롤 필드  // Dịch chuyển | Bạn chắc chắn chứ? 
	{"Item", "se\336 \360\375a va\314o lo\314 luy\352\362n kim? "}, // pluto 제작 메세지  // sẽ đưa vào lò luyện kim? 
	{"Ch\352\354 ta\362o", "Ba\362n ch\343\354c ch\343\354n ch\375\354?? "},    // pluto 추가 던전 끝없는 탑 MESSAGE_TELEPORT_DUNGEON  // Chế tạo | Bạn chắc chắn chứ?? 
};


char *SkillMaster[4] = {
	"Ho\362c Skill",  // Học Skill
	"Change Class",
	"Thoa\354t Game"  // Thoát Game

};
char *SkillMaster2[4] = {
	"",
	"",
	""

};

char *ReStartMsg[4] = {
	" B\343\354t \360\342\314u la\362i t\375\314 ch\364\336 na\314y",  //  Bắt đầu lại từ chỗ này
	" Quay tr\365\322 v\352\314 tha\314nh",  //  Quay trở về thành
	" Thoa\354t Game",  //  Thoát Game
	" Kh\364ng \360u\322 XP"  //  Không đủ XP

};
char *ReStartMsg2[4] = {
	" Pha\362t th\352m (Gold/XP) 1%",  //  Phạt thêm (Gold/XP) 1%
	"",
	"",
	" \360\352\322 b\343\354t \360\342\314u la\362i t\375\314 ch\364\336 na\314y",  //  để bắt đầu lại từ chỗ này

};

char *SkillUseInfoText[10]= { "    Ho\362c Skill/Magic     ","    L\375\362a cho\362n m\364\362t Skill/Magic     ","      Ho\362c ho\343\362c luy\352\362n t\342\362p       ","  Kh\364ng co\354 \360u\322 \360i\352\322m Skill ","   \360\352\322 ho\362c hay luy\352\362n t\342\362p" };  //     Học Skill/Magic      |     Lựa chọn một Skill/Magic      |       Học hoặc luyện tập        |   Không có đủ điểm Skill  |    để học hay luyện tập
char *sinMsgSkillPoint = "    Skill Points : %d";
char *sinMsgEliteSkillPoint = "Elite Skill Points : %d " ;
char *sinStateName[5]={"H P : %d/%d  ","M P : %d/%d  ","STM : %d/%d  ","X P : %d/%d  ","X P : %dM/%dM   "};


char *TownName[3] = {"Quay v\352\314 Ricarten\r","Quay v\352\314 Navisko\r","Quay v\352\314 Pillai\r "};  // Quay về Ricarten\r | Quay về Navisko\r | Quay về Pillai\r 

char *sinAbilityName[50] = {"S\375\354c \360a\354nh:  \r","T\364\354c \360\364\362:  \r","T\342\314m xa:  \r","Critical:  \r","S\375\354c ch\364\354ng \360\365\336:  \r","Ty\322 l\352\362 \360a\354nh:  \r",  // Sức đánh:  \r | Tốc độ:  \r | Tầm xa:  \r | Sức chống đỡ:  \r | Tỷ lệ đánh:  \r
					"S\375\354c chi\362u \360\375\362ng:  \r","Kha\322 n\343ng \360\365\336:  \r","T\364\354c \360\364\362:  \r","S\375\362 toa\314n ve\362n:  \r","Phu\362c h\364\314i MP:  \r","Phu\362c h\364\314i HP :  \r",  // Sức chịu đựng:  \r | Khả năng đỡ:  \r | Tốc độ:  \r | Sự toàn vẹn:  \r | Phục hồi MP:  \r | Phục hồi HP :  \r
					"Phu\362c h\364\314i STM:  \r","Ch\364\354ng v\342\362t ly\354:  \r","Nature Type:  \r","Ch\364\354ng l\375\322a:  \r","Ch\364\354ng b\343ng:  \r","Ch\364\354ng se\354t:  \r","Ch\364\354ng \360\364\362c: \r",  // Phục hồi STM:  \r | Chống vật lý:  \r | Chống lửa:  \r | Chống băng:  \r | Chống sét:  \r | Chống độc: \r
					"Water Type:  \r","Wind Type:  \r","Phu\362c h\364\314i HP:  \r","Phu\362c h\364\314i MP: \r","Phu\362c h\364\314i STM: \r","C\364\362ng th\352m HP: \r",  // Phục hồi HP:  \r | Phục hồi MP: \r | Phục hồi STM: \r | Cộng thêm HP: \r
					"C\364\362ng th\352m MP:  \r","C\364\362ng th\352m STM:  \r","Ch\375\354a Potion:  \r","C\342\314n Level:  \r","C\342\314n Strength:  \r","C\342\314n Spirit:  \r",  // Cộng thêm MP:  \r | Cộng thêm STM:  \r | Chứa Potion:  \r | Cần Level:  \r | Cần Strength:  \r | Cần Spirit:  \r
					"C\342\314n Talent:  \r","C\342\314n Agility:  \r","C\342\314n Health:  \r "};  // Cần Talent:  \r | Cần Agility:  \r | Cần Health:  \r 


char *sinSpecialName[50] = {"Spec. ATK SPD:  \r","Spec. CRIT:  \r","Spec. DEF RTG:  \r","Spec. ABS RTG:  \r",
						"Spec. BLK RTG:  \r","Magic APT:  \r","Spec. SPD:  \r","Spec. Organic:  \r","Spec. Nature:  \r",
						"Spec. Flame:  \r","Spec. Frost:  \r","Spec. Lightning:  \r","Spec. Poison:  \r","Spec. Water:  \r","Spec. Wind:  \r",
						"Spec. ATK POW:  \r","Spec. ATK RTG:  \r","Spec. RNG:  \r","Spec. Organic ATK:  \r","Spec. Nature ATK:  \r","Spec. Flame ATK:  \r","Spec. Frost ATK:  \r",
						"Spec. Lightning ATK:  \r","Spec. Poison ATK:  \r","Spec. Water ATK:  \r","Spec. Wind ATK:  \r","Max HP Boost:  \r","Max MP Boost:  \r",
						"Phu\362c h\364\314i HP:  \r","Phu\362c h\364\314i MP:  \r","Phu\362c h\364\314i STM:  \r","Gia\354 ba\354n  \r","Gia\354:  \r "};  // Phục hồi HP:  \r | Phục hồi MP:  \r | Phục hồi STM:  \r | Giá bán  \r | Giá:  \r 


char *SpecialName3 = "%s Spec. \r";
char *MixResultTitle = "K\352\354t qua\322 Mix";  // Kết quả Mix
char *NowLevelText = "Level hi\352\362n ta\362i\r";  // Level hiện tại\r
char *NextLevelText = "Level k\352\354 ti\352\354p\r";  // Level kế tiếp\r

char *NormalAttckName ="Default Attack";
char *RequirLevel = "%s (Req. LVL:%d)\r";
char *UseItemGroupName = "Chi\322 s\375\322 du\362ng \360\375\365\362c v\365\354i:\r";  // Chỉ sử dụng được với:\r

char *PoisonName = "Poison Type:\r";
char *FireName = "Flame Type:\r";
char *AttackAreaName = "T\342\314m a\322nh h\375\365\322ng:\r";  // Tầm ảnh hưởng:\r
char *Attack_RateName = "Ty\322 L\352\362 \320a\354nh:\r";  // Tỷ Lệ Đánh:\r
char *Max_DamageIncre = "T\343ng Max Damage:\r";  // Tăng Max Damage:\r
char *DamageAddName = "T\343ng s\375\354c sa\354t th\375\365ng:\r";  // Tăng sức sát thương:\r
char *Attack_SpeedAdd = "T\343ng t\364\354c \360\364\362 \360a\354nh:\r";  // Tăng tốc độ đánh:\r
char *IncreBlock = "T\343ng kha\322 n\343ng \360\365\336:\r";  // Tăng khả năng đỡ:\r
char *CountinueTime = "Th\365\314i gian:\r";  // Thời gian:\r
char *UseManaName = "MP s\375\322 du\362ng:\r";  // MP sử dụng:\r
char *DamageName = "Damage:\r";
char *ShootingRangeName = "t\342\314m xa:\r";  // tầm xa:\r
char *IncreAsorb = "T\343ng chi\322 s\364\354 chi\362u \360\375\362ng:\r";  // Tăng chỉ số chịu đựng:\r
char *SecName = "gi\342y";  // giây
char *PlusDamageName = "C\364\362ng s\375\354c \360a\354nh:\r";  // Cộng sức đánh:\r
char *HitNumName = "S\364\354 l\342\314n \360a\354nh:\r";  // Số lần đánh:\r
char *DecreLifeName = "Gia\322m HP:\r";  // Giảm HP:\r
char *Attck_RateAdd = "C\364\362ng ty\322 l\352\362 \360a\354nh:\r";  // Cộng tỷ lệ đánh:\r
char *Add_CriticalName = "C\364\362ng ty\322 l\352\362 Critical:\r";  // Cộng tỷ lệ Critical:\r
char *Push_AreaName = "T\342\314m \360\342\322y lu\314i:\r";  // Tầm đẩy lùi:\r
char *FireAttackDamageAdd ="C\364\362ng phe\354p l\375\322a:\r";  // Cộng phép lửa:\r
char *IceName = "Thu\364\362c ti\354nh B\343ng:\r";  // Thuộc tính Băng:\r
char *IceAttackDamageAdd = "C\364\362ng phe\354p b\343ng:\r";  // Cộng phép băng:\r
char *StuneRate = "Ty\322 l\352\362 g\342y choa\354ng:\r";  // Tỷ lệ gây choáng:\r
char *DefenseRateIncre = "T\343ng s\375\354c ch\364\354ng \360\365\336:\r";  // Tăng sức chống đỡ:\r
char *WeaponSizeName ="Ki\354ch c\365\336 vu\336 khi\354:\r";  // Kích cỡ vũ khí:\r
char *HwakRotationNum = "S\364\354 l\342\314n t\342\354n c\364ng:\r";  // Số lần tấn công:\r
char *WeaponSpeedAddName  = "C\364\362ng t\364\354c \360\364\362 \360a\354nh:\r";  // Cộng tốc độ đánh:\r
char *IncreAttack_RateName = "T\343ng ty\322 l\352\362 \360a\354nh:\r";  // Tăng tỷ lệ đánh:\r
char *ShootingNumName = "S\364\354 l\342\314n b\343\354n:\r";  // Số lần bắn:\r
char *SeriesShootingCount = "S\364\354 l\342\314n t\342\354n c\364ng:\r";  // Số lần tấn công:\r
char *UseStaminaName = "Stamina s\375\322 du\362ng:\r";  // Stamina sử dụng:\r
char *MasterSkillMoneyName = "Gia\354 ti\352\314n:\r";  // Giá tiền:\r

char *ItemAgingResult;		// << Add Point

char *sinGold ="%d Gold";
char *sinGiveItem5 = "Gift Item\r";
char *sinCopyItem5 = "Copied Item\r";
char *sinNum7 = "RND";
char *sinSkillPointName = "Skill Point";	// << Add Point
char *sinG_Pike_Time3 = "Frozen Time:\r";	// << Add Point
char *sinItemLimitTimeOverMsg = "Th\365\314i gian cho phe\354p \360a\336 h\352\354t\r";  // Thời gian cho phép đã hết\r
char *sinDeadSongPyeunEat = "Darn it! It's rotten rice cake!";
char *SmeltingResultTitle = "K\352\354t qua\322 luy\352\362n kim";		// pluto 제련  // Kết quả luyện kim
char *ManufactureResultTitle = "K\352\354t qua\322 ch\352\354 ta\362o";	// pluto 제작  // Kết quả chế tạo
char *ManufacturingTitle = "Mo\354n \360\364\314 ng\342\336u nhi\352n";	//fuck pluto  // Món đồ ngẫu nhiên


char *sinMaxDamageAdd = "Increase in Maximum damage+\r";
char *sinDamagePiercing = "(Piercing attack)\r";
char *sinAddStamina = "Added stamina:\r";
char *sinAddLife = "Life recovery:\r";
//char *sinPartyHealDoc = " If in party, experience points will be added:\ as much as 1/3 of life recovered:\r";
char *sinDamageUndead50 = "50% more damages to Undead units:\r";
char *sinNumCount2 = "number:\r";
char *sinNumCount3 = "quantity\r";
char *sinDecreDamage = "Decrease in damage:\r";
char *sinConvert4 = "conversion rate:\r";
char *sinIncreElement = "Increase in element feature:\r";
char *sinPartyIncreElement = "Party member gather 50% increase of element\r";
char *sinFireBallDamage2 = "(100% damage to the targeted enemy)\r";
char *sinFireBallDamage3 = "(targeted area 60 (1/3 damage))\r";
char *sinAddDamage7 = "added damage:\r";
char *PlusDamageWeapon = "weapon";
char *SparkDamage = "damage per unit:\r";
char *SparkNum7 = "# of maximum spark:\r";
char *StunRage = "Stun Range:\r";
char *RecvPotion7 = "Potion";


char *PartyArea7 ="Party Area:\r";
char *Area17 ="Area:\r";
char *AddAttack_Rate9 ="Attack Rating:\r";
char *PiercingRange ="Piercing Range:\r";
char *IncreDefense = "Increase Defense:\r";
char *AddShootingRange = "Increased SHT range:\r";
char *LifeAbsorb = "Life absorb.:\r";
char *FireDamage = "Fire Damage:\r";
char *IceDamage = "Ice Damage:\r";
char *LightningDamage = "Lightning Damage:\r";
char *AddManaRegen = "Add Mana Regen:\r";
char *LightNum = "# of Lightning:\r";
char *ReturnDamage = "Return Damage:\r";
char *Area18 = "Available Range:\r";
char *IncreMana5 = "Increase Mana:\r";
char *Area19 = "Available Area:\r";
char *HelpTitle8 = " T R \325\362  G I U\354 P ";  //  T R Ợ  G I Ú P 
char *QuestTitle8 = " N H I \312\362 M  V U\362 ";  //  N H I Ệ M  V Ụ 
char *ResearchTitle8 = "RESEARCH";
char *TeleportTitle8 = "TELEPORT";

char *IncreWeaponAttack_RateName = "Increased ATK rating:\r";
char *AddMaxDamage = "Increase maximum damage:\r";
char *LinkCoreName = "Di\362ch chuy\352\322n t\375\354c th\365\314i t\365\354i tha\314nh vi\352n cu\322a Clan\r";  // Dịch chuyển tức thời tới thành viên của Clan\r
char *DesLinkCore = "Name:";
char *Itemul = "item";
char *ItemRecvOk = "Ba\362n co\354 mu\364\354n v\342\362t na\314y kh\364ng?";  // Bạn có muốn vật này không?
char *Money5 = "gold";
char *Exp5 = "EXP";
char *WingItemName[] = {"Metal wing","Silver wing","Gold wing","Diamond wing", "Chaos wing", "Extreme wing "};
char *WarpGateName[] = {"Warp gate","Ricarten","Pillai","R\375\314ng c\364\322","R\375\314ng tru\354c","La\314ng Ruinen","Thi\362 tr\342\354n Navisko","Vu\314ng \360\342\354t c\342\354m", "La\314ng Eura", "Lost Island "};  // Rừng cổ | Rừng trúc | Làng Ruinen | Thị trấn Navisko | Vùng đất cấm | Làng Eura
char *sinWarningName5 = "Chu\354 y\354 ";  // Chú ý 
char *ClanCristalName = "Da\314nh ri\352ng cho tha\314nh vi\352n Clan\r";  // Dành riêng cho thành viên Clan\r
char *NowMyShopSell = "\320ang ba\354n\r";  // Đang bán\r
char *SecretNumName = "Nh\342\362p va\314o ma\336 s\364\354 ki\352\322m soa\354t";  // Nhập vào mã số kiểm soát
char *MyShopExpDoc7 = "Th\364ng tin v\352\314 c\375\322a ha\314ng";  // Thông tin về cửa hàng
char *NotAgingItem2 = "'Kh\364ng th\352\322 Age mo\354n \360\364\314 na\314y";  // 'Không thể Age món đồ này
char *ExpPercent2 = "EXP : %d.%d%s";
char *ExpPercent3 = "EXP : %d.0%d%s";
char *Mutant7   =   "Mutant";
char *Mechanic7 = "Mechanic";
char *Demon7    = "Demon";
char *Nomal7    = "Normal";
char *Undead7    = "Undead";
char *MonsterAddDamage2 = "add extra damamge)\r";
char *MonsterAddDamageFire = "add fire element damage)\r";
char *HaWarpGateName[] = {"Bless Castle", 0};
char *FallGameName = "Twisted forest";
char *EndlessGameName	= "Endless Tower";
char *LookHelp = ">>more..";


char *HoldStarNumDoc = "The number of your stars";
char *StarItemBuy7   = "Purchase the stars";
char *ChangeMoney7   = "(The price of one star: 100000)"; 



char *LightningDamage2 = "LightningDamage:\r";
char *SheildDefense = "Increasing Defense:\r";

char *DemonDamage4  = "DemonVSATKpower:\r";
char *PoisonDamage3  = "PoisonDamage:\r";
char *PikeNum4       = "Number of spear:\r";
char *poisoningTime  = "poisoningTime:\r";
char *PlusCriticalName = "Increasing Critical%:\r";
char *SpiritFalconDamage2 = "SpritFalconDamage:\r";
char *LifeGegenPlus = "Increaing HP:\r";
char *PlusDamage4 = "PlusDamage:\r";
char *PiercingPercent = "PiercingPercent:\r";
char *DamageToLife = "DamageToLife:\r";
char *MyMonster7 = "MyMonster:\r";
char *LifeIncre4 = "LifeIncre:\r";
char *UndeadDamage3 = "UndeadDamage:\r";

char *AttackNum3     = "NumberofATK:\r"; 
char *AttackRateMinus = "DecresingATKRATING:\r"; 
char *MaxDamagePlus2 = "MaxDamagePlus:\r"; 
char *LightningAddDamage3 = "LightningAddDamage:\r"; 
char *ReLifePercent4    = "ReLifePercent:\r"; 
char *GetExp3    = "GetExp:\r"; 
char *ResurrectionChar4    = "ResurrectionCharLV"; 
char *ExtinctionPercent2    = "ExtinctionPercen:\r"; 
char *ExtinctionAmount2    = "ExtinctionAmount:\r"; 
char *IncreLifePercent2   = "IncreLifePercent:\r"; 

char *ReduceDamage3 = "ReduceDamage:\r"; 
char *IncreMagicDamage = "IncreMagicDamage:\r"; 
char *AttackDelay3   = "AttackDelay:\r"; 

char *FireDamage2 = "FireDamage:\r";
char *IceDamage2 = "IceDamage:\r";
char *AddDefense8 = "AddDefense:\r";
char *SkillDamage5 = "SkillDamage:\r";

char *FireDamage3 = "FireDamage:\r";
char *AddSpeed7 = "AddSpeed:\r";
char *AfterDamage7 = "AfterDamage:\r";
char *MonsterSight7 = "MonsterSight:\r";


char *LinghtingAddDamage4="Add attack rate on lightening:\r";
char *MaxBoltNum4="Max voltage:\r";
char *AddAbsorb4="Add rate on absorb:\r";
char *IncreArea4="Increasing range:\r";
char *IncreAttack4="More attacking power:\r";
char *GolemLife4="Life+STR:\r";


char *SubAbsorb4="Decreasing absorption:\r";
char *AreaDamage4="Attacking rate for range:\r";
char *MaxAttackNum4="Max hits:\r";
char *LifeUp4="Add vitality:\r";

char *CriticalAddDamage4="Increase of critical:\r";
char *MonstervsSubCritical4="Weaken critical of monster:\r";
char *ChargingAddPercentDamage4="Add attacking rate on charging:\r";
//char *ChargingTime4="Holding time of charging:\r";
char *IncreEvasionPercent4="Additinal percentage of evasion:\r";
char *AddShadowNum4="Increasing combination of ocular:\r";

char *WolverinLife4="Vitality:\r";
char *WolverinRate4="Accuracy:\r";
char *WolverinDefense4="Defence rate:\r";
char *AddEvasion4="Increasing ability of evasion:\r";
char *AddDamage4="Increasing attacking rate(physic):\r";
char *FalconAddDamage4="Increasing attacking rate of falcon:\r";


char *IncreAttackAbsorb4="Increasing absorption rate:\r";

char *RectAngleArea4="Attacking at sharp angled range:\r"; 

char *MonsterSubSpeed4="Decreasing speed of attacked monster:\r";


char *AddLifeRegen4="Add life-regeneration:\r";
char *AddManaRegen4="Add mana-regeneration:\r";
char *MagicArea4="Magic affected range:\r";
char *ChainNum4="A number of affected monster:\r";
char *ChainRange4="The rage of affected monster:\r";
char *UndeadDamageAbsorb4="Undead attacking absorption:\r";
char *UserBlockPercent4="Blocking rate for character:\r";

char *SecondIncreMana4="Increasing mana rate per second:\r";
char *FireDamage4="Fire attacking rate:\r";
char *DecreSpeed4="Decreasing Speed:\r";
char *DecreAttack4="Decreasing attacking rate:\r";
char *AddPercentDamage4="Adding attacking rate:\r";



char *ChainDamage3    = "ChainDamage)\r";
char *BrandishDamage3 = "BrandishDamage";
char *PhysicalAbsorb3 = "PhysicalAbsorb";
char *SparkDamage3    = "SparkDamage";
char *BrutalSwingCritical3   = "BrutalSwingCritical";
char *Attck_RateAdd5 = "Attck_RateAdd:\r";
char *Triumph4="Triumph";
char *Use4="you";
char *DivineShield4="Blocking rate of divine shiled";


char *QuestMonsterName[] = {"Tha\362ch nh\342n ","Ta\314n binh c\364\354t ","Tra\322m thu\322 qua\354i ","Bo\362 gia\354p ","C\364\354t xa\362 thu\322 ","Tha\362ch tinh " ,  // Thạch nhân  | Tàn binh cốt  | Trảm thủ quái  | Bọ giáp  | Cốt xạ thủ  | Thạch tinh 
                         "Qua\354i v\342\362t ta\362i Vu\314ng \360\342\354t bi\362 la\336ng qu\352n ","Qua\354i v\342\362t ta\362i \324\354c \360a\322o ","Qua\354i v\342\362t ta\362i Nha\314 ngu\362c c\364\322, t\342\314ng 1  "};  // Quái vật tại Vùng đất bị lãng quên  | Quái vật tại Ốc đảo  | Quái vật tại Nhà ngục cổ, tầng 1  




char *VampOption[10] ={"IncreaseAging%: \r","Vampire: \r","RefDamage: \r","DecreaseHP: \r","Lucky: \r "};


char *Quest3ItemName[]={"Demon Hunter","Ancient Revenge","Mist","Devil Side","Anaconda","Plated Sword","Ghost "};

char *Quset3ItemDoc15 = "Mo\354n \360\364\314 nhi\352\362m vu\362 ";  // Món đồ nhiệm vụ 
char *Quset3ItemDoc16 = " \360a\336 bi\352\354n m\342\354t";  //  đã biến mất

char *SparkDamage10 = "SparkDamage:\r";

char *RequirLevel3 = "(RequirLevel:%d)\r";

char *RecvItemTT = "Nh\342\362n \360\375\365\362c mo\354n \360\364\314 ";  // Nhận được món đồ 
char *PuzzleEvent5 = "Completed Puzzle";
char *BabelHorn = "The horn of Babel";
char *NineTailFoxItem = "Ha\354i l\364\362c \360\342\314u xu\342n";  // Hái lộc đầu xuân
char *MyShopItemSell5 = "The Price: \r";
char *CristalItem     ="the 7 Crystals";
char *StarDust	= "B\364\362t tinh tu\354 ";  // Bột tinh tú 
char *ChristMas = "Merry ChristMas";

char *sinDeadCandyEat = "\303\362c! Ca\354i ke\362o na\314y bi\362 ho\322ng r\364\314i -_-";  // Ặc! Cái kẹo này bị hỏng rồi -_-

char *PotionCntDoc2 = "Ba\354n Potions";  // Bán Potions
char *ItemPrice7 = "Ba\354n mo\354n \360\364\314 ";  // Bán món đồ 

char *BuyMyShopItem7 = "%s \360a\336 mua %d %s b\343\314ng %d va\314ng";  // %s đã mua %d %s bằng %d vàng
char *BuyMyShopItem8 = "%s \360a\336 ba\354n %d %s v\365\354i gia\354 %d va\314ng";  // %s đã bán %d %s với giá %d vàng
char *ItemName87 = "item";

char *sinEvent87 = "Request of the danger collector";
char *sinAddDamageItem = "AddedATKPOWER: \r";
char *sinContinueTimeItem = "Duration: \r";
char *BuyPotionMoney = "Gold:";
char *BuyPotionKind = "S\364\354 l\375\365\362ng:";  // Số lượng:
char *SheltomName2[] = {"Lucidy","Sereno"," Fadeo","Sparky","Raident","Transparo","Murky","Devine","Celesto ","Mirage","Inferna","Enigma"};

char *AddPercentDamage3= "AddATKPOWER%: \r";

char *sinLuckyBox = "Lucky Bag";
char *SodMessage_Etc[]= {
			"Thu nh\342\362p cu\322a Clan qua\322n ly\354 Bellatra la\314 ",  // Thu nhập của Clan quản lý Bellatra là 
			"Chi\322 co\354 chu\322 Clan m\365\354i \360\375\365\362c nh\342\362n.",  // Chỉ có chủ Clan mới được nhận.
			"Ti\322 l\352\362 thu\352\354 hi\352\362n ha\314nh la\314 %d%s.",  // Tỉ lệ thuế hiện hành là %d%s.
			};
			
char *SodMessage_Clan[] = {
			"T\364\322ng thu nh\342\362p ",  // Tổng thu nhập 
			"Bellatra cung c\342\354p ti\352\336n la\336i nga\314y h\364m qua.",  // Bellatra cung cấp tiễn lãi ngày hôm qua.
			};
			
char *SodMessage_Master[] = {
			"T\364\322ng thu nh\342\362p",  // Tổng thu nhập
			"Bellatra cung c\342\354p ti\352\314n la\336i nga\314y h\364m qua.",  // Bellatra cung cấp tiền lãi ngày hôm qua.
			"Ba\362n co\354 mu\364\354n l\342\354y ra kh\364ng?",  // Bạn có muốn lấy ra không?
			};
			
char *OtherClanMaster[] = {
			"Clan v\342\336n co\314n co\354 th\352\322 nh\342\362n ti\352\314n la\336i",  // Clan vẫn còn có thể nhận tiền lãi
			"nh\342\362p va\314o s\364\354 ti\352\314n c\342\314n l\342\354y ra.",  // nhập vào số tiền cần lấy ra.
			};
/*----------------------------------------------------------------------------*
*			테스트			공성전 메뉴메세지 
*-----------------------------------------------------------------------------*/	
char *SiegeMessage_Taxrates[] = {
	"Ti\322 l\352\362 thu\352\354 \360a\336 thay \360\364\322i %d%s.",  // Tỉ lệ thuế đã thay đổi %d%s.
	"T\364\322ng s\364\354 thu\352\354 \360a\336 ti\354ch luy\336 ",  // Tổng số thuế đã tích luỹ 
	"Gold.",
	"Ba\362n co\354 mu\364\354n ru\354t ti\352\314n thu\352\354 ra kh\364ng?.",  // Bạn có muốn rút tiền thuế ra không?.
	"Ti\322 l\352\362 thu\352\354 hi\352\362n ta\362i la\314 %d%s.",  // Tỉ lệ thuế hiện tại là %d%s.
};
char *SiegeMessage_MercenrayA[] = {
	"D\342n qu\342n",  // Dân quân
	"La\314 l\375\362c l\375\365\362ng d\342n qu\342n",  // Là lực lượng dân quân
	"S\375\354c chi\352\354n \360\342\354u kh\364ng cao nh\375ng gia\354 tha\314nh re\322.",  // Sức chiến đấu không cao nhưng giá thành rẻ.
	"T\364\322ng s\364\354 d\342n qu\342n: 20 / %d",  // Tổng số dân quân: 20 / %d
};
char *SiegeMessage_MercenrayB[] = {
	"V\352\362 binh",  // Vệ binh
	"Gia\354 ca\322 phu\314 h\365\362p v\365\354i kha\322 n\343ng chi\352\354n \360\342\354u.",  // Giá cả phù hợp với khả năng chiến đấu.
	"Co\354 s\375\354c chi\352\354n \360\342\354u trung bi\314nh.",  // Có sức chiến đấu trung bình.
	"T\364\322ng s\364\354 V\352\362 binh: 20 / %d",  // Tổng số Vệ binh: 20 / %d
};
char *SiegeMessage_MercenrayC[] = {
	"C\342\354m v\352\362 qu\342n",  // Cấm vệ quân
	"Co\354 kha\322 n\343ng chi\352\354n \360\342\354u cao nh\342\354t",  // Có khả năng chiến đấu cao nhất
	"S\375\354c chi\352\354n \360\342\354u va\314 pho\314ng v\352\362 r\342\354t du\336ng ma\336nh.",  // Sức chiến đấu và phòng vệ rất dũng mãnh.
	"T\364\322ng s\364\354 C\342\354m v\352\362 qu\342n: 20 / %d",  // Tổng số Cấm vệ quân: 20 / %d
};
char *SiegeMessage_TowerIce[] = {
	"Toa\314 tha\354p B\343ng",  // Toà tháp Băng
	"Ti\354nh b\343ng la\314m gia\322m t\364\354c \360\364\362 di chuy\352\322n va\314 ",  // Tính băng làm giảm tốc độ di chuyển và 
	"co\354 kha\322 n\343ng \360\343\362c bi\352\362t la\314m gia\322m t\364\354c \360\364\362 \360a\354nh.",  // có khả năng đặc biệt làm giảm tốc độ đánh.
	"Thi\352\354t l\342\362p Toa\314 tha\354p B\343ng \360a\336 hoa\314n t\342\354t",  // Thiết lập Toà tháp Băng đã hoàn tất
};
char *SiegeMessage_TowerFire[] = {
	"Toa\314 tha\354p L\375\322a",  // Toà tháp Lửa
	"Co\354 kha\322 n\343ng t\342\354n c\364ng b\343\314ng l\375\322a r\342\354t ma\362nh",  // Có khả năng tấn công bằng lửa rất mạnh
	"Ngoa\314i ra kh\364ng co\354 kha\322 n\343ng \360\343\362c bi\352\362t.",  // Ngoài ra không có khả năng đặc biệt.
	"Thi\352\354t l\342\362p Toa\314 tha\354p L\375\322a \360a\336 hoa\314n t\342\354t",  // Thiết lập Toà tháp Lửa đã hoàn tất
};
char *SiegeMessage_TowerLighting[] = {
	"Toa\314 tha\354p \320i\352\362n",  // Toà tháp Điện
	"La\314m gia\322m t\364\354c \360\364\362 di chuy\352\322n va\314 ",  // Làm giảm tốc độ di chuyển và 
	"co\354 kha\322 n\343ng \360\343\362c bi\352\362t la\314m gia\322m t\364\354c \360\364\362 \360a\354nh.",  // có khả năng đặc biệt làm giảm tốc độ đánh.
	"Thi\352\354t l\342\362p Toa\314 tha\354p \320i\352\362n \360a\336 hoa\314n t\342\354t",  // Thiết lập Toà tháp Điện đã hoàn tất
};
char *SiegeMessage_MerMoney   = "Gia\354 cu\322a li\354nh \360a\354nh thu\352: ";  // Giá của lính đánh thuê: 
char *SiegeMessage_TowerMoney = "Gia\354 cu\322a Toa\314 tha\354p: ";  // Giá của Toà tháp: 

char *sinClanMaster7 = "Chu\322 Clan";  // Chủ Clan
char *sinPrize7 = "Ph\342\314n th\375\365\322ng : ";  // Phần thưởng : 


char *sinLevelQuestMonster[]={"Tha\362ch nh\342n ","N\342\354m quy\322 ","Ong \360\364\362c ","\320a\362i tinh tinh ","Xi\354ch ma v\375\365ng ","Thi\352\354t ma v\375\365ng ","Thi\352\354t thu\322 v\375\365ng  "};  // Thạch nhân  | Nấm quỷ  | Ong độc  | Đại tinh tinh  | Xích ma vương  | Thiết ma vương  | Thiết thủ vương  
char *sinLevelQusetDoc="Nhi\352\362m vu\362> \320a\336 ti\352u di\352\362t \360\375\365\362c %s %d";  // Nhiệm vụ> Đã tiêu diệt được %s %d

char *sinTeleportDoc[] = {"Hang N\342\354m ","Hang Tha\362ch Nhu\336 ","Vu\314ng \360\342\354t bi\362 nguy\352\314n ru\322a ","Vu\314ng \360\342\354t c\342\354m  "};  // Hang Nấm  | Hang Thạch Nhũ  | Vùng đất bị nguyền rủa  | Vùng đất cấm  
char *sinLevelQuest90_2Mon[] = {"\320i\362a \342\354n v\375\365ng ","Thi\352\354t khu\322ng long ","Song chuy\314 quy\322  "};  // Địa ấn vương  | Thiết khủng long  | Song chuỳ quỷ  
char *sinLevelQusetDoc2="<Nhi\352\362m vu\362> Ha\336y ti\352u di\352\362t %s %d";  // <Nhiệm vụ> Hãy tiêu diệt %s %d
char *sinLevelQusetDoc3="<Nhi\352\362m vu\362> Ti\314m mo\354n \360\364\314 t\375\314 %s";  // <Nhiệm vụ> Tìm món đồ từ %s
char *sinLimitTiem2 = "Th\365\314i gian gi\365\354i ha\362n: \r";  // Thời gian giới hạn: \r
char *sinMinute2 = "phu\354t";  // phút
char *sinDay4 = "day";
char *sinHour4 = "hour";
char *sinLevelQusetDoc4="<Nhi\352\362m vu\362> \320a\336 ti\352\362u di\352\362t \360\375\365\362c %s %d";  // <Nhiệm vụ> Đã tiệu diệt được %s %d

sinSTRING sinTestMsg7("Hurray, Neh Neh Neh Boo Boo, You're losers!");

char *sinMonCodeName[]={
	0,
	"Tha\362ch nh\342n ",  // Thạch nhân 
	"Ta\314n binh c\364\354t ",  // Tàn binh cốt 
	"Tra\322m thu\322 qua\354i ",  // Trảm thủ quái 
	"Bo\362 gia\354p ",  // Bọ giáp 
	"C\364\354t xa\362 thu\322 ",     //5  // Cốt xạ thủ 
	"Tha\362ch tinh ",  // Thạch tinh 
	"N\342\354m quy\322 ",  // Nấm quỷ 
	"Ong \360\364\362c ",  // Ong độc 
	"\320a\362i tinh tinh ",  // Đại tinh tinh 
	"Xi\354ch ma v\375\365ng ",                //10  // Xích ma vương 
	"Thi\352\354t ma v\375\365ng ",  // Thiết ma vương 
	"Thi\352\354t thu\322 v\375\365ng ",  // Thiết thủ vương 
	"\320i\362a \342\354n v\375\365ng ",  // Địa ấn vương 
	"Thi\352\354t khu\322ng long ",  // Thiết khủng long 
	"Song chuy\314 quy\322 ",              //15  // Song chuỳ quỷ 
	"Thi\352\354t binh c\364\354t ",  // Thiết binh cốt 
	"S\352n h\364\322 ",  // Sên hổ 
	"Thi\352\354t ca\314ng ",  // Thiết càng 
	"Xa\354c \375\365\354p ",  // Xác ướp 
	"Qua\322n ngu\362c ",             //20  // Quản ngục 
	"Hoa\322 nh\342n",              // Hoả nhân
	"Tha\362ch v\375\365ng ",  // Thạch vương 
	"Tha\362ch th\342\314n ",  // Thạch thần 
	"V\352\362 gia\354p binh ",  // Vệ giáp binh 
	"Nh\342n ma\336 v\375\365ng ",       //25  // Nhân mã vương 
	"Nh\342n ma\336 th\375\365ng ",  // Nhân mã thương 
	"Ong chu\354a ",  // Ong chúa 
	"Ky\362 si\336 a\322o ",  // Kỵ sĩ ảo 
	"A\354c ma ",  // Ác ma 
	"Phu\314 thu\322y ",                //30  // Phù thủy 
	"\320a\362i qua\354i ",  // Đại quái 
	"Song \360\342\314u qua\354i ",  // Song đầu quái 
	"T\375\322 th\342\314n ",  // Tử thần 
	"Xu\354 di\352\362n ma ",  // Xú diện ma 
	"Xi\354ch quy\322 ",           //35  // Xích quỷ 
	"H\343\354c ma v\375\365ng ",  // Hắc ma vương 
	"Thi\352\354t h\342\314u ",  // Thiết hầu 
	"Nh\342n ma\336 chu\314y ",  // Nhân mã chùy 
	"H\364\314ng sa\354t thu\322 ",  // Hồng sát thủ 
	"S\342\314u quy\322 ",            //40  // Sầu quỷ 
	0,
};

char *ChargingTime4="Raise speed for charging:\r";
char *AddIceDamage4="Added Damage of Frost Elemental:\r";
char *IceTime4="Freezed Time:\r";
char *sinAttackType = "Attack Type:\r";
char *sinAttackType2[2] ={"Manual\r","Auto\r "};

char *Won="Gold";

//공성전 아이템 설명
char *CastlItemInfo[] ={
	{"B\342\354t t\375\322 trong m\364\362t khoa\322ng th\365\314i gian\r "},    // Bất tử trong một khoảng thời gian\r 
	{"Ti\322 l\352\362 \360a\354nh Critical\r "},  // Tỉ lệ đánh Critical\r 
	{"T\343ng ti\322 l\352\362 ne\354 \360o\314n\r "},  // Tăng tỉ lệ né đòn\r 
	{"STM,HP,MP cho tha\314nh vi\352n party\r "},  // STM,HP,MP cho thành viên party\r 
	{"H\364\314i sinh t\375\354c thi\314.\r "},  // Hồi sinh tức thì.\r 
	{"S\375\354c t\342\354n c\364ng cu\322a toa\314 tha\354p\r "},     //  // Sức tấn công của toà tháp\r 
	{"Khi t\342\354n c\364ng mu\362c ti\352u, s\375\354c \360a\354nh t\343ng th\352m 20%\r "},  // Khi tấn công mục tiêu, sức đánh tăng thêm 20%\r 
	0,
};
char *CastlItemInfo2[] ={
	{"S\375\354c \360a\354nh gia\322m \360i 1/2.\r "},  // Sức đánh giảm đi 1/2.\r 
	{"T\343ng th\352m 5%.\r "},  // Tăng thêm 5%.\r 
	{"t\343ng th\352m.\r "},  // tăng thêm.\r 
	{"phu\362c h\364\314i \360\342\314y \360u\322.\r "},  // phục hồi đầy đủ.\r 
	{"\r "},
	{"gia\322m \360i 50%\r "},  // giảm đi 50%\r 
	{"C\364\362ng th\352m.\r "},  // Cộng thêm.\r 
	0,
};


char *CharClassTarget[8]={
	"Mu\362c ti\352u: Fighter\r",  // Mục tiêu: Fighter\r
	"Mu\362c ti\352u: Mechanician\r",  // Mục tiêu: Mechanician\r
	"Mu\362c ti\352u: Pikeman\r",  // Mục tiêu: Pikeman\r
	"Mu\362c ti\352u: Archer\r",  // Mục tiêu: Archer\r
	"Mu\362c ti\352u: Knight\r",  // Mục tiêu: Knight\r
	"Mu\362c ti\352u: Atalanta\r",  // Mục tiêu: Atalanta\r
	"Mu\362c ti\352u: Magician\r",  // Mục tiêu: Magician\r
	"Mu\362c ti\352u: Priestess\r",  // Mục tiêu: Priestess\r
};
char *AttributeTower[4]={
	"Thu\364\362c ti\354nh L\375\322a\r",  // Thuộc tính Lửa\r
	"Thu\364\362c ti\354nh B\343ng\r",  // Thuộc tính Băng\r
	"Thu\364\362c ti\354nh \320i\352\362n\r",  // Thuộc tính Điện\r
	"Th\365\314i gian:%d gi\342y\r",  // Thời gian:%d giây\r
};

char *SiegeMessage_MerComplete ="Thi\352\354t l\342\362p hoa\314n t\342\354t";  // Thiết lập hoàn tất

//초보 퀘스트 
char *haQuestMonsterName[]={
	"S\375\314ng gai",  // Sừng gai
	"Tho\322 r\375\314ng",  // Thỏ rừng
	"\320\364\362c tinh",  // Độc tinh
	"H\343\354c tinh",  // Hắc tinh
	"Kh\364 c\364\354t",  // Khô cốt
	"Hu\322 c\364\354t",  // Hủ cốt
	"\320\364\362c nha\336n ",  // Độc nhãn 
	"Tha\362ch nh\342n ",  // Thạch nhân 
	0,
};
char *ha100LVQuestItemName[]={
	"Minotaur Axe",
	"Extreme Talon",
	"Dragon Bone Hammer ",
	"Hellfire Scythe",
	"Revenge Bow",
	"Immortal Sword",
	"Salamander Javelin",
	"Gothic Staff",
	0,
};
//100LV퀘스트 (퓨리의 환영)
char *ha100LVQuestMonterName[]={
	"Qua\354i v\342\362t ta\362i Nha\314 ma\354y Perum",  // Quái vật tại Nhà máy Perum
	"Qua\354i v\342\362t ta\362i Thung lu\336ng Gallubia",  // Quái vật tại Thung lũng Gallubia
	0,
};
char *ha100LVQuestName = "A\322o t\375\365\362ng Fury";  // Ảo tượng Fury

//프리미엄 유료화 아이템
char *PremiumItemDoc[][2]={
	{"\320i\352\322m Stats\r"     ,"se\336 \360\375\365\362c thi\352\354t l\342\362p la\362i.\r "},     //스탯 초기화 아이템  // Điểm Stats\r | sẽ được thiết lập lại.\r 
	{"\320i\352\322m Skill\r"     ,"se\336 \360\375\365\362c thi\352\354t l\342\362p la\362i.\r "},    //스킬 초기화 아이템  // Điểm Skill\r | sẽ được thiết lập lại.\r 
	{"\320i\352\322m Stats/Skill\r","se\336 \360\375\365\362c thi\352\354t l\342\362p la\362i.\r "},     //스탯/스킬 초기화 아이템  // Điểm Stats/Skill\r | sẽ được thiết lập lại.\r 
	{"H\364\314i sinh t\375\354c thi\314 \r"    ,"khi bi\362 ch\352\354t\r "},        //부활 아이템  // Hồi sinh tức thì \r | khi bị chết\r 
	{"Tr\365\322 n\352n b\342\354t t\375\322.\r","S\375\354c t\342\354n c\364ng gia\322m \360i 1/2\r "},    //이터널 라이프  // Trở nên bất tử.\r | Sức tấn công giảm đi 1/2\r 
	{"T\343ng th\352m 15% \r"    ,"ti\322 l\352\362 ra \360o\314n chi\354 ma\362ng.\r "},		//페이틀 에지  // Tăng thêm 15% \r | tỉ lệ ra đòn chí mạng.\r 
	{"T\343ng th\352m 15% \r"         ,"kha\322 n\343ng ne\354 \360o\314n.\r "},		//에버트 스크롤  // Tăng thêm 15% \r | khả năng né đòn.\r 
};

char *UpKeepItemDoc[] = {
	"Co\314n la\362i : %d phu\354t",  // Còn lại : %d phút
	"Ti\322 l\352\362 r\365i \360\364\314 t\343ng th\352m %d%s",  // Tỉ lệ rơi đồ tăng thêm %d%s
	"Ti\322 l\352\362 XP t\343ng th\352m %d%s",  // Tỉ lệ XP tăng thêm %d%s
	"T\343ng th\352m s\375\354c t\342\354n c\364ng %d%s",  // Tăng thêm sức tấn công %d%s
	"H\342\354p thu ma\354u cu\322a \360\364\354i thu\322 ",  // Hấp thu máu của đối thủ 
	"H\342\354p thu Mana cu\322a \360\364\354i thu\322 ",  // Hấp thu Mana của đối thủ 
	"T\343ng c\342n n\343\362ng th\352m d%",  // Tăng cân nặng thêm d%
	"Gia\322m MP khi du\314ng ky\336 n\343ng %d%s",  // Giảm MP khi dùng kỹ năng %d%s
	0,
};
char *UpKeepItemName[] = {
	"Th\342\314n nha\336n",  // Thần nhãn
	"T\343ng XP",  // Tăng XP
	"Nanh h\342\354p huy\352\354t ",  // Nanh hấp huyết 
	"Bi\314nh h\342\354p l\375\362c ",  // Bình hấp lực 
	"Bi\314nh t\343ng l\375\362c",  // Bình tăng lực
	"Bi\314nh tr\365\362 l\375\362c",  // Bình trợ lực
	"Red Pixie",
	0,
};
