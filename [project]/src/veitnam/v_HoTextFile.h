#include "..\\HoBaram\\HoTextFile.h"

char *HoTextLoginMessage[18] =  {"K\352\354t n\364\354i \360ang \360\375\365\362c thi\352\354t l\342\362p....",  // Kết nối đang được thiết lập....
				"Kh\364ng th\352\322 k\352\354t n\364\354i v\365\354i ma\354y chu\322 ",  // Không thể kết nối với máy chủ 
				"     Vui lo\314ng nh\342\362p la\362i ID    ",  //      Vui lòng nhập lại ID    
				" Vui lo\314ng nh\342\362p la\362i m\342\362t kh\342\322u  ",  //  Vui lòng nhập lại mật khẩu  
				"ID na\314y \360a\336 bi\362 kho\354a, ki\352\322m tra \365\322 Website",  // ID này đã bị khóa, kiểm tra ở Website
				"  ID na\314y hi\352\362n \360ang bi\362 s\375\322 du\362ng  ",  //   ID này hiện đang bị sử dụng  
				"  Phi\352n ba\322n kh\364ng phu\314 h\365\362p   ",  //   Phiên bản không phù hợp   
				"      Ma\354y chu\322 bi\362 \360\342\314y        ",  //       Máy chủ bị đầy        
				"    M\342\354t k\352\354t n\364\354i v\365\354i ma\354y chu\322     ",  //     Mất kết nối với máy chủ     
				"   Th\365\314i gian cho phe\354p \360a\336 h\352\354t  ",  //    Thời gian cho phép đã hết  
				"   Th\365\314i gian cho phe\354p \360a\336 h\352\354t  ",  //    Thời gian cho phép đã hết  
				" \320ang xo\354a ta\314i khoa\322n tr\352n ma\354y chu\322 ",  //  Đang xóa tài khoản trên máy chủ 
				 "Ma\354y chu\322 qua\354 ta\322i!!!, vui lo\314ng th\375\322 la\362i sau.",  // Máy chủ quá tải!!!, vui lòng thử lại sau.
				" vui lo\314ng th\375\322 la\362i sau... ",  //  vui lòng thử lại sau... 
				"Incorrect password over 3 times, regarding PT webpage.",
				"You can't use the characters in password.",
				};

char *HoTextSelectMessage[12] = {"Ba\362n co\354 mu\364\354n xo\354a?",  // Bạn có muốn xóa?
				  "Ti\352\354p tu\362c v\365\354i nh\342n v\342\362t na\314y?",  // Tiếp tục với nhân vật này?
				  "V\375\365\362t qua\354 gi\365\354i ha\362n s\364\354 nh\342n v\342\362t cho phe\354p.",  // Vượt quá giới hạn số nhân vật cho phép.
				  "\320\364\314ng y\354 ta\362o nh\342n v\342\362t na\314y?", //3  // Đồng ý tạo nhân vật này?
				  "Ha\336y \360i\352\314n t\352n nh\342n v\342\362t", //4  // Hãy điền tên nhân vật
				  "T\352n nh\342n v\342\362t \360a\336 t\364\314n ta\362i", //5  // Tên nhân vật đã tồn tại
				  "  Cho\362n m\364\362t nh\342n v\342\362t   ", //6  //   Chọn một nhân vật   
				  "    Cho\362n m\364\362t b\364\362 la\362c     ", //7  //     Chọn một bộ lạc     
				  "    Cho\362n m\364\362t class     ", //8  //     Chọn một class     
				  "    Cho\362n khu\364n m\343\362t     ", //9  //     Chọn khuôn mặt     
				  "Kh\364ng th\352\322 xo\354a nh\342n v\342\362t la\314 chu\322 m\364\362t clan", //10  // Không thể xóa nhân vật là chủ một clan
				  "Kh\364ng th\352\322 xa\354c \360i\362nh th\364ng tin clan" //11  // Không thể xác định thông tin clan
				};

char *HoTextTempscronInfo[5] = {" Tempskron la\314 m\364\362t b\364\362 la\362c ma\314 ta\362o ho\354a \360a\336 ban cho ho\362 nh\375\336ng kha\322 n\343ng ky\336 thu\342\362t tinh xa\322o \360\375\365\362c cho la\314 b\342\362c nh\342\354t \360a\362i lu\362c Priston, va\314 cu\336ng chi\354nh b\364\362 la\362c na\314y \360a\336 v\342\362n du\362ng nh\375\336ng tinh hoa cu\322a mi\314nh cho vi\352\362c ch\352\354 ta\362o nh\375\336ng sa\322n ph\342\322m ky\336 thu\342\362t cao n\364\322i ti\352\354ng thi\352n ha\362.",  //  Tempskron là một bộ lạc mà tạo hóa đã ban cho họ những khả năng kỹ thuật tinh xảo được cho là bậc nhất đại lục Priston, và cũng chính bộ lạc này đã vận dụng những tinh hoa của mình cho việc chế tạo những sản phẩm kỹ thuật cao nổi tiếng thiên hạ.
								/* Maybe " The Tempskron tribe is greatly renowned for their physical skills and efficient techniques..."*/
                                " R\342\354t n\364\322i ti\352\354ng v\365\354i nga\314nh c\364ng nghi\352\362p ch\352\354 ta\362o ma\354y, ho\362 co\354 nh\375\336ng t\364\362c ng\375\365\314i v\365\354i nh\375\336ng kha\322 n\343ng kha\354c nhau.",  //  Rất nổi tiếng với ngành công nghiệp chế tạo máy, họ có những tộc người với những khả năng khác nhau.
								" Fighter \360\375\365\362c sinh ra \360\352\322 quy\352\354t chi\352\354n quy\352\354t th\343\354ng, va\314 tr\364ng ho\362 th\342\362t du\336ng ma\336nh v\365\354i c\342y ri\314u tr\352n tay.",  //  Fighter được sinh ra để quyết chiến quyết thắng, và trông họ thật dũng mãnh với cây rìu trên tay.
								" Pikeman, ho\362 s\375\322 du\362ng t\342\314m nhi\314n r\364\362ng l\365\354n va\314 nh\375\336ng vu\336 khi\354 co\354 \360\364\362 s\343\354c xa\322o \360\352\354n ch\352\354t ng\375\365\314i.",  //  Pikeman, họ sử dụng tầm nhìn rộng lớn và những vũ khí có độ sắc xảo đến chết người.
								" B\343\354n xa va\314 xe\354 gio\354, ho\362 la\314 nh\375\336ng xa\362 thu\322 chuy\352n nghi\352\362p trong ca\354c loa\362i cung no\322."};  //  Bắn xa và xé gió, họ là những xạ thủ chuyên nghiệp trong các loại cung nỏ.



char *HoTextMoryon = "Morion";
char *HoTextMoryonInfo[5] = {
	"Morions la\314 gi\364\354ng ng\375\365\314i s\364\354ng \365\322 phi\354a B\343\354c lu\362c \360i\362a Priston. Ng\375\365\314i Morions t\364i luy\352\362n ca\322 ki\352\354m thu\342\362t va\314 phe\354p thu\342\362t nh\375ng s\365\322 tr\375\365\314ng chi\354nh v\342\336n la\314 nh\375\336ng cu\354 luy\352\362n ch\375\365\322ng xu\342\354t quy\322 nh\342\362p th\342\314n ma\314 chu\354ng ta go\362i \360o\354 la\314 ma thu\342\362t. V\365\354i s\375\362 k\352\354t h\365\362p ha\314i ho\314a cu\322a t\364n gia\354o va\314 quy\352\314n n\343ng ho\362 \360a\336 x\342y d\375\362ng s\375\362 ph\364\314n thi\362nh va\314 s\375\354c ma\362nh t\342m linh \360a\354ng s\365\362.",  // Morions là giống người sống ở phía Bắc lục địa Priston. Người Morions tôi luyện cả kiếm thuật và phép thuật nhưng sở trường chính vẫn là những cú luyện chưởng xuất quỷ nhập thần mà chúng ta gọi đó là ma thuật. Với sự kết hợp hài hòa của tôn giáo và quyền năng họ đã xây dựng sự phồn thịnh và sức mạnh tâm linh đáng sợ.

    "S\365\322 h\375\336u b\364\362 gia\354p oai phong va\314 thanh g\375\365m \360\364\314 s\364\362, Knight la\314 t\364\362c ng\375\365\314i ha\314ng \360\342\314u trong ca\354c chi\352\354n binh \365\322 Morion. Ho\362 s\375\322 du\362ng s\375\354c ma\362nh t\342m linh \360\352\322 chi\352\354n th\343\354ng ke\322 thu\314.",  // Sở hữu bộ giáp oai phong và thanh gươm đồ sộ, Knight là tộc người hàng đầu trong các chiến binh ở Morion. Họ sử dụng sức mạnh tâm linh để chiến thắng kẻ thù.
	"Atlantas la\314 m\364\362t ki\352\322u chi\352\354n binh kha\354c, ho\362 th\375\365\314ng du\314ng gia\354o ma\354c la\314m vu\336 khi\354. S\375\354c ma\362nh cu\322a ho\362 chi\322 th\375\362c s\375\362 n\364\322 tr\364\362i khi ho\362 du\314ng nh\375\336ng th\375\354 vu\336 khi\354 na\314y. V\365\354i kha\322 n\343ng t\342\354n c\364ng t\375\314 xa, s\375\362 k\352\354t h\365\362p gi\375\336a pha\354p thu\342\362t va\314 s\375\354c ma\362nh trong t\342\354n c\364ng la\314 n\364\322i b\342\362t cu\322a ho\362.",  // Atlantas là một kiểu chiến binh khác, họ thường dùng giáo mác làm vũ khí. Sức mạnh của họ chỉ thực sự nổ trội khi họ dùng những thứ vũ khí này. Với khả năng tấn công từ xa, sự kết hợp giữa pháp thuật và sức mạnh trong tấn công là nổi bật của họ.
	"Priestesses la\314 nh\375\336ng ng\375\365\314i tu ha\314nh va\314 ho\362 du\314ng n\343ng l\375\362c cu\322a th\342\314n linh \360\352\322 ch\364\354ng la\362i ke\322 thu\314 va\314 ch\375\336a th\375\365ng cho \360\364\314ng \360\364\362i. Kha\322 n\343ng ch\375\336a b\352\362nh c\375\354u ng\375\365\314i cu\322a ho\362 th\342\362t h\375\336u i\354ch bu\314 la\362i cho s\375\354c kho\322e va\314 s\375\354c t\342\354n c\364ng ha\362n ch\352\354. ",  // Priestesses là những người tu hành và họ dùng năng lực của thần linh để chống lại kẻ thù và chữa thương cho đồng đội. Khả năng chữa bệnh cứu người của họ thật hữu ích bù lại cho sức khỏe và sức tấn công hạn chế. 
	"Magicians h\364\362i tu\362 t\342\354t ca\322 s\375\354c ma\362nh cu\322a ma l\375\362c \360\352\322 la\314m t\343\362ng t\364\354i \360a kha\322 n\343ng chi\352\354n \360\342\354u. V\365\354i s\375\362 th\364ng minh cu\322a thi\352n phu\354, Magician la\314 t\364\362c ng\375\365\314i co\354 kha\322 n\343ng ho\362c r\342\354t nhi\352\314u phe\354p thu\342\362t.",  // Magicians hội tụ tất cả sức mạnh của ma lực để làm tặng tối đa khả năng chiến đấu. Với sự thông minh của thiên phú, Magician là tộc người có khả năng học rất nhiều phép thuật.
};


char *HoText_PartyMessage = "y\352u c\342\314u Party \360a\336 \360\375\365\362c g\375\322i";  // yêu cầu Party đã được gửi

char *HoTextTempskron = "Tempskron";

char *HoTextFriendNear = "[M\365\354i \360\342y]";  // [Mới đây]
char *HoTextFriendList = "[Ba\362n be\314] %d/%d";  // [Bạn bè] %d/%d
char *HoTextFriendRefuse = "[Bo\322 qua] %d/%d";  // [Bỏ qua] %d/%d