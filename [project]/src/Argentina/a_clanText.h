//ycy
//클랜 결성 의사 메세지
char *szMsg1 = " Los Guerreros de todo el continente\n\
se unen y forman un grupo con el mismo \n\
objetivo. Se llama 'clan'.    \n\
Estos clanes protegerán a los suyos.\n\
Para formar un clan, se debe pagar  \n\
impuestos para registrarse        \n\
oficialmente. Deseas formar   \n\
un clan?                    \n\
Hey, Te gustaría formar un clan también? ";

//클랜 지불 의사 메세지
char *szMsg2 = " Para formar un clan,\n necesitas pagar 500.000 Oro.\n Deseas continuar?\n";


//클랜 지불 의사!서 돈이 부족한 메세지
char *szMsg21 = " Necesitas 500.000 Oro\n\
para pagar los impuestos de formar un clan.\n\
Por favor regresa con suficiente dinero. ";

//클랜 이름 설정 메세지
char *szMsg3 = " Por favor tipea el nombre de tu clan.\n ";

//클랜 클랜 마크! 대한 설명 메세지
#ifdef		_LANGUAGE_PHILIPIN
char *szMsg4 = " Cuando se forma un clan, \n\
los jugadores podrán usar         \n\
su marca y frases.                \n\
Para mayor información por favor \n\
visita www.axeso5.com. Después de \n\
loguearte, podrás registrar/editar \n\
la página del clan, pero debes estar\n\
registrado oficialmente.            \n ";
#else
char *szMsg4 = " Cuando se forma un clan, \n\
los jugadores podrán usar su marca y frases. .\n\
Para mayor información por favor  visita nuestra página web.\n\
Después de loguearte, podrás registrar/editar\n\
la página del clan, pero debes estar\n\
Gracias.\n ";
#endif

//클랜 심시 기간! 대한 설명

char *szMsg5 = " Este es lo que deber\355as hacer\n\"  //  Este es lo que deberías hacer\n\
para estar oficialmente registrado.\n\
*Debes tener más de 6 miembros\n\
(incluyendo al jefe del clan )\n\
*Tienes 10 días para reclutar miembros.\n\
Si no puedes satisfacer estas condiciones,\n\
el registro del clan será cancelado\n\
automaticamente.\n ";

char *szClanMakeFinishMsg = " El registro del clan se ha completado.\n\
Por favor cumple con las condiciones de registro\n\
y registralo como clan oficial.\n ";

//클랜 미 충족 캐릭터! 대한 메세지
char *szMsg254 =" !Tonto! Ni pienses \n\
formar un clan con tus habilidades lastimosas.\n\
Te apuesto que ni siquiera puedes cuidar de ti mismo.\n\

//클랜 해체 메세지
char *szMsg15 = " Deseas desbandar el clan? ";
char *szMark1 = " No tienes una marca de clan. Por favor,lee el manual con cuidado. ";
char *szMark2 = " Deseas registrar una marca de clan? ";
char *szMark3 = " Deseas cambiar la marca de clan? ";


//Å¬·£ °!ÀÔ ¸Þ¼¼Áö
char *szJoin = " Nombre del Clan: %s\nMaestro del Clan : %s\n\n ha enviado un mensaje de invitaci\363n.\n\"  //  Nombre del Clan: %s\nMaestro del Clan : %s\n\n ha enviado un mensaje de invitación.\n\
Te gustaría unirte a este clan clan?\n ";
char *szSecession = " Nombre del Clan: %s\n Deseas abandonar el clan? ";
//Å¬·£øÀÌ 6¸í ÀÌÇÏÀÏ°æì ¸Þ¼¼Áö
char *szMsg6 = " Para registrar un clan, necesitas un m\355nimo de 6 miembros.\n Te quedan %d d\355as. ";  //  Para registrar un clan, necesitas un mínimo de 6 miembros.\n Te quedan %d días. 
//end ycy


char *szServerConnectErrorMsg = " Imposible conectar al servidor.\n\
Por favor intenta más tarde.\n ";

char *szReadClanMarkErrorMsg = " Fall\363 la lectura de la marca del clan\n\"  //  Falló la lectura de la marca del clan\n\
del servidor.\n ";

char *szCancelMakeClan = " Has cancelado la formaci\363n del clan.\n ";  //  Has cancelado la formación del clan.\n 

char *szTranzationError = " Ocurri\363 un error de DB. El men\372 finalizar\341 ahora.\n ";  //  Ocurrió un error de DB. El menú finalizará ahora.\n 

char *szInsertClanWonError1 = " Este personaje ya es parte de otro clan.\n ";

char *szClanNameError = " El nombre que deseas registrar ya existe.\n ";

char *sz100Error = " Ha ocurrido un error desconocido.\n\
El menú finalizará ahora.\n ";

char *szDefaultError = " Ha ocurrido un erro fatal.\n\
El menú finalizará ahora.\n ";

char *szReleseError = " Este personaje no es maestro de clan\n\
ni miembro de clan.\n ";


char *szSecessionSuccess = " Has abandonado el clan.\n ";

char *szSecession6DownMsg = " Quitado exitosamente.\n\
Tienes menos de 6 miembros en el clan.\n ";


char *szSecessionChipMsg = " El miembro que desea abandonar\n\
es el maetro del clan. \n ";

char *szSecession2ErrorMsg = " Ese clan, maestro de clan, o miembro\n\
no existe.\n ";





char *szClanJoinMsg = " Nombre de Clan: %s\n Maestro de Clan: %s\nmember id: %s\n\n desea registrarse.\n ";

char *szClanJoinError2Msg = " Este Guerrero ya es miembro de otro clan.\n ";

char *szClanJoinError3Msg = " Ese clan no existe o no es un maestro de clan .\n ";

char *szClanJoinError4Msg = " Todos los miembros del clan son de nivel 32 o superior.\n ";

char *szClanJoinCancelMsg = " %s rehus\363 unirse al clan.\n ";  //  %s rehusó unirse al clan.\n 

char *szClanJoinsucesssMsg = " %s se ha unido al clan.\n ";





char *szReMoveClanWonMsg = " Expulsado del clan.\n ";

char *szReMoveHeaderMsg = " Aqu\355 est\341 la lista de expulsados\n ";  //  Aquí está la lista de expulsados\n 

//클랜 해체 관련 메세지
char *szReConfReleaseMsg = " Si desbandas el clan, toda la informaci\363n\n\"  //  Si desbandas el clan, toda la información\n\
acerca de los miembros será borrada.\n\
Deseas desbandar de tods maneras?\n ";
char *szNoRecordReleaseMsg = " Est\341s intentando desbandar el clan\n\"  //  Estás intentando desbandar el clan\n\
antes de registrarlo.\n\
Deseas continuar?\n ";
char *szFinishReleaseMsg = " El clan ha sido desbandado.\n ";

char *szSecessionMsg = " Si dejas el clan, todas tus funciones de clan\n desaparecer\341n.\n Deseas continuar?\n ";  //  Si dejas el clan, todas tus funciones de clan\n desaparecerán.\n Deseas continuar?\n 
char *szFinishSecessionMsg = " Clan: %s\nMiembro : %s\n ha dejado el clan.\n ";

char *szisCheckClanJangErrorMsg = " No puedes hacer otro clan\n\
porque ya tienes un personaje \n que es maestro de clan.\n ";

char *szMoneyErrorMsg = " No hay suficiente dinero\n para hacer un clan.\n "
" Imposible hacer un clan. ";

char *szAbilityErrorMsg = " No tienes suficiente EXP.\n para hacer un clan.\n ";

char *szRemoveShortClanWonMsg = " Si borras a esta miembro, \n tu clan no cumplir\341 con el n\372mero de miembros requeridos,\n\"  //  Si borras a esta miembro, \n tu clan no cumplirá con el número de miembros requeridos,\n\
Deseas borrarlo de todas maneras? ";
char *szRemoveShowrtClanWonResultMsg = " Tu clan tiene menos de 6 miembros.\n Por favor, recluta m\341s Guerreros.\n ";  //  Tu clan tiene menos de 6 miembros.\n Por favor, recluta más Guerreros.\n 

char *szNoName = " Por favor, escribe el nombre de tu clan.\n ";

char *szLoading = " Conectando al servidor.\n ";


/////////캐릭터 삭제 메세지
char *szDeleteChaReleaseBefore = " Este personaje es un maestro de clan.\n\
Si lo borras, el clan también será borrado.\n\
Deseas continuar?\n ";

char *szDeleteChaRelease = " El clan ha sido borrado.\n ";

char *szDeleteChaSecessionBefore = " Este personaje est\341 en un clan.\n\"  //  Este personaje está en un clan.\n\
Si lo borras, será desbandado de él.\n\
Deseas borrar?\n ";
char *szDeleteChaSecession = " te has separado del clan.\n ";

char *szDeleteChaError = " Falla al borrar el clan.\n Imposible borrar el personaje.\n ";
char *szMakeFinishClan = " !Felicitacones!!!\n[%s]\nTe has convertido en miembro de un clan.\n ";
char *szClanWon6Down = " Tu clan tiene menos de 6 miembros.\n Por favor, recluta m\341s\n Guerreros.\n\"  //  Tu clan tiene menos de 6 miembros.\n Por favor, recluta más\n Guerreros.\n\
%s\n";


char *szClanUserNoMakeMsg = " Clan : %s\nRegistrando clan. ";
char *szMakeFinishClanWon = " Clan : %s\nTu clan ha sido oficialmente registrado.\n ";
char *ClanServerInspect = " Mantenimiento del Servidor.\n ";
char *szClanUserRelease = " Tu clan ha sido desbandado. ";
char *Replay = "    Por favor, intenta nuevamente ";
char *szClanMakedowninfo = " Registrando clan. ";

//ÀÚµÇØÃ¼, Å¬·£ÀåÀÌ Å¬·£ÀÌ ¹Ìµî·ÏµÈ »óÅÂ!¼­ ÇØÃ¼ÇÏ´À» °æì
char *szClanUserReleaseMsg = " Clan : %s\n el registro ha sido cancelado.\n ";
char *szClanUserRemove = " Clan : %s\nMiembro : %s\nha sido expulsado del clan.\n ";

