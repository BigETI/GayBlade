/****************************************                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                          /***********************************        DragonBlade   Ver 1.0.0      ****      � Copyright by Ryan Best 1992  ****          All Rights Reserved.       *******************************************/#include "GB.h"extern	WindowPtr	gPictureWindow, gGameWindow, gCastleWindow;extern	Boolean		gDone, gWNEImplemented;extern	EventRecord	gTheEvent;extern	MenuHandle	gAppleMenu,gFileMenu,gOptionsMenu,gSpecialMenu;extern	Rect		gIntroRect, gTavernRect, gBankRect, gRollerRect, gStoreRect, gCharRect0,					gCharRect1, gCharRect2, gCharRect3,gItemsRect, gMapRect, gSpellsRect,gSrcRect,					gGuildRect,gHealerRect,gdungeonRect, gStatsRect, gPartyInfoRect,					gUseRect,gPanicRect, gMoveRect, gDungDispRect, gActionRect,gConsoleRect, 					gMScreenRect,leftRect,centerRect,rightRect,cLeftRect,cCenterRect,cRightRect,					doorRect,clwallRect,crwallRect,cRect, gChestRect,gItemRect,gListStatsRect,					gListItemsRect, gCompassRect,gTreasureRect;extern	PicHandle	gThePicture, gConsolePicture, gDungeon1, gLeftWall, gCenterWall, gRightWall,					gLeftDoor,gRightDoor,gFlatWall,gFlatDoor,gOpenSpace,gFarWall,gFarDoor,gCLWall,gCRWall,					gCOpenSpace,gCLDoor,gCRDoor,gCFWall,gCFDoor, gMScreenPicture;extern	CIconHandle	gVialIcon,gShieldIcon, gRingIcon, gSphereIcon,gFlailIcon,gCloakIcon,gArrowIcon,gDaggerIcon,					gSwordIcon,gHelmetIcon,gTorchIcon,gBookIcon,gGlovesIcon,gBurnedIcon,gArmorIcon,					gChestIcon, gCompassIcon,gCNorthIcon,gCSouthIcon,gCEastIcon,					gCWestIcon;extern int			gSndOn,gPanicOn, gInCastle, gPartyNum,gNewChar, gSndNum, gLastChar, gLight,					gLightSpell,gItemUseScreen, gCurChar, gInPrison,gDialogNum,gDisplay,gDungLevel,gXLoc,gYLoc,					gView,gResistFire,gResistIce,gPartyInvis,gInEncounter,gCureWho,gFoundItems[3],					gCreatType, gCreatureCounter, gCreatHits[10],gLevelCount,dmgType,gTreasure,					gDisarmSpellCast,gCompass,ASave,gMagicItems[4],gSpeed,gCharShields[4],gEvade;extern	long		gStats[6], gDungParty[4],gWhichChar,glastWhen,gPanic;extern	Point		glastWhere;extern	DialogPtr	gCastleDialog;extern	RGBColor	gNameBack, gStatsBack, gSpecName,gNormName,gMessBack,gBlack,gStatsFore;extern	CCrsrHandle	gSpellCursor;extern	SFReply		fileName;extern	struct	CharInfo {	Str255	name;	int		type;	int		race;	int		level;	int		exp;	int		status;	int		hits;	int		dmg;	int		age;	int		str;	int		wis;	int		itl;	int		con;	int		dex;	int		items[7];		int		itemsInUse[7];			/* what items are being used */	int		gold;	int	 	bank;					/* released wizard? */	int		mspells[4],cspells[4];	int		dngLevel, xLoc, yLoc,Pos;	};extern	struct	CharInfo	GameChars[8];						/*  Can store up to 8 chars per game  */extern	struct	ItemInfo {	Str255			name;	unsigned char	type;	unsigned char	status;		/* whether item is in use or not */	unsigned char	instore;	unsigned char	level;	Byte			attrib;	unsigned char	dmg;	unsigned char	def;	unsigned char	con;};extern	struct	ItemInfo	GameItems[101];extern	struct	DungInfo 				{	unsigned int	XY[10][10];		/*  x, y  rooms location */	unsigned int	N[100];			/* 100 rooms w/north walls */	unsigned int	S[100];			/* 100 rooms w/south walls */	unsigned int	E[100];			/* 100 rooms w/east walls */	unsigned int	W[100];			/* 100 rooms w/west walls */	unsigned int	SA[100];};extern	struct	DungInfo	Dungeon[1];	extern struct	CreatureInfo 				/* access database each encounter */{	Str255			name;	unsigned char	type;	unsigned char	status;		/* For use in editor only */	unsigned char	instore;	/* Unused variable */	unsigned char	level;	Byte			attrib;	Byte			dmg;	Byte			def;	Byte			hits;};extern struct	CreatureInfo	GameCreatures[1];/************************ Guild ****************************/TheGuild(){int				itemType, guild_id, itemHit,  dialogDone = FALSE,z; Handle			itemHandle;Rect			itemRect;Str255			text,lmess,theGold;Str255			type="\p"; gSndNum=19402;	PlaySound();  if (gDungParty[0]==99)			/* no char, then go away! */{	ClearMessage();		DrawString("\pThe Guild does not allow your types in!");	return;}z=DeadCharMsg();	if (z == -1)				/* dead char */		return;					gDialogNum=408;	DoDialog();					ClearMessage();			DrawString(GameChars[gCurChar].name);DrawString("\p has entered the guild.");	RGBBackColor(&gStatsBack);			NumToString(GameChars[gCurChar].level,text);			NumToString(GameChars[gCurChar].level*275,lmess);			    NumToString(GameChars[gCurChar].level*50,theGold);   	  	 	if (GameChars[gCurChar].type==0)						ParamText("\pQueer",text,lmess,theGold);		if (GameChars[gCurChar].type==1)			ParamText("\pDrag Queen",text,lmess,theGold);		if (GameChars[gCurChar].type==2)			ParamText("\pMuse",text,lmess,theGold);		if (GameChars[gCurChar].type==3)			ParamText("\pGuppie",text,lmess,theGold);		if (GameChars[gCurChar].type==4)			ParamText("\pLesbian",text,lmess,theGold);		if (GameChars[gCurChar].type==5)			ParamText("\pMaster",text,lmess,theGold);		while (dialogDone == FALSE)	{		ModalDialog(NIL_POINTER, &itemHit);			switch (itemHit)			{				case CANCEL_ITEM:					DisposeDialog(gCastleDialog);					dialogDone = TRUE;																break;				case OKAY_ITEM:					DisposeDialog(gCastleDialog);					dialogDone = TRUE;					MakeLevel();					break;			}			}	SetPort(gGameWindow);		}/************************ MakeLevel ****************************/MakeLevel(){int			x,y,z;unsigned	min,max;Str255		theText;if (GameChars[gCurChar].exp >= GameChars[gCurChar].level*275 &&	GameChars[gCurChar].gold>=GameChars[gCurChar].level*50)	{	if (GameChars[gCurChar].level<=50)		/** PROBLEM CODE!!!!!! FIX ME! **/		GameChars[gCurChar].gold=GameChars[gCurChar].gold-			(GameChars[gCurChar].level*50);			GameChars[gCurChar].level++;		ClearMessage();		DrawString("\pYou made level ");		gSndNum=10305;			PlaySound();		NumToString(GameChars[gCurChar].level,theText);		DrawString(theText);		DrawString("\p!");			x=GetRandomNum(min=2,max=13);					if (x<=84)				/* 16% chance of not increasing stat */				{					if (GameChars[gCurChar].hits+x <=255)	/* not more than 255 */						GameChars[gCurChar].hits=GameChars[gCurChar].hits+x;				}					x=GetRandomNum(min=0,max=100);					if (x<=84)				/* 16% chance of not increasing stat */				{					if (GameChars[gCurChar].str+1 <=35)		/* not more than 35 */						GameChars[gCurChar].str++;				}			x=GetRandomNum(min=0,max=100);					if (x<=84)				/* 16% chance of not increasing stat */				{					if (GameChars[gCurChar].wis+1 <=35)						GameChars[gCurChar].wis++;				}							x=GetRandomNum(min=0,max=100);					if (x<=84)				/* 16% chance of not increasing stat */				{					if (GameChars[gCurChar].itl+1 <=35)						GameChars[gCurChar].itl++;				}			x=GetRandomNum(min=0,max=100);					if (x<=84)				/* 16% chance of not increasing stat */				{					if (GameChars[gCurChar].con+1 <=35)						GameChars[gCurChar].con++;				}			x=GetRandomNum(min=0,max=100);					if (x<=84)				/* 16% chance of not increasing stat */				{					if (GameChars[gCurChar].dex+1 <=35)						GameChars[gCurChar].dex++;				}			x=GetRandomNum(min=0,max=100);					if (x<=64)				/* 36% chance of not increasing stat */				{					if (GameChars[gCurChar].age+1 <=99)						GameChars[gCurChar].age++;				}							GameChars[gCurChar].dmg=0;		for (z=0;z<=gPartyNum;z++)			GenerateSpells(z);		/* gWhichChar = char to generate spells for */				}else	{	ClearMessage();	DrawString("\pYou're not ready yet! BE GONE!");	GameChars[gCurChar].gold=GameChars[gCurChar].gold/2; /* penalty */	return;	}}