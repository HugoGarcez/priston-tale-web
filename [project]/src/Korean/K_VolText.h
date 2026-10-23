
#include "..\\Language.h"

#ifdef	_LANGUAGE_KOREAN

char *parkTextVO = "\300\275\274\272 \303\244\306\303 \277\311\274\307";
char *parkTextMIC = "\270\266\300\314\305\251";
char *parkTextMicON = "On";
char *parkTextMicOFF = "Off";

char *parkTextVol = "\272\274\267\375 : ";
char *parkTextVolLow = "\300\372";
char *parkTextVolHigh = "\260\355";

char *parkTextVolErr1 = "\300\251\265\265\277\354 \263\273\277\241\274\255 \270\266\300\314\305\251";
char *parkTextVolErr2 = "\272\274\267\375 \301\266\300\375 \307\330\301\326\274\274\277\344.";


#endif



#ifdef	_LANGUAGE_CHINESE
#include "..\\chinese\\C_VolText.h"
#endif

#ifdef	_LANGUAGE_JAPANESE
#include "..\\Japanese\\J_VolText.h"
#endif

#ifdef	_LANGUAGE_TAIWAN
#include "..\\Taiwan\\T_VolText.h"
#endif

#ifdef	_LANGUAGE_ENGLISH
#include "..\\English\\E_VolText.h"
#endif

#ifdef	_LANGUAGE_THAI
#include "..\\Thai\\Th_VolText.h"
#endif

#ifdef	_LANGUAGE_VEITNAM
#include "..\\veitnam\\v_VolText.h"
#endif

#ifdef	_LANGUAGE_BRAZIL
#include "..\\BRAZIL\\b_VolText.h"
#endif

#ifdef	_LANGUAGE_ARGENTINA
#include "..\\ARGENTINA\\a_VolText.h"
#endif

