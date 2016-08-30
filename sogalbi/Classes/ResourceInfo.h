#pragma once

#include <string>

#include "..\Common\Packet.h"

namespace FOLDERNAME
{
	const std::string SPRITE = "Sprites/";
	const std::string FONT = "fonts/";
	const std::string AUDIO = "Sounds/";
	const std::string POKE = "Sprites/poke/";
	const std::string CARD = "Sprites/cards/";
}

namespace FILENAME
{
#define DEF_FILENAME(kind,name,filename) const std::string name = FOLDERNAME::kind + filename
#define DEF_FILENAME_POKEMON(kind,filename) FOLDERNAME::kind + filename
#define DEF_FILENAME_CARD(kind,filename) FOLDERNAME::kind + filename
#define MAX_NUMBER_OF_POKEMON_AVAILABLE 5
	namespace SPRITE
	{
		DEF_FILENAME(SPRITE, TABLE_BG, "tableBg.png");
		DEF_FILENAME(SPRITE, DECK, "deckRed.png");
		DEF_FILENAME(SPRITE, NAMETAG, "nameTag.png");
		DEF_FILENAME(SPRITE, NAMETAG_BACK, "nameTagBack.png");

		DEF_FILENAME(SPRITE, BANNER_BLACKJACK,	"bannerBlackJack.png");
		DEF_FILENAME(SPRITE, BANNER_STAND,		"bannerStand.png");
		DEF_FILENAME(SPRITE, BANNER_WAITING,	"bannerWaiting.png");
		DEF_FILENAME(SPRITE, BANNER_BURST,		"bannerBurst.png");

		DEF_FILENAME(SPRITE, HIT, "hit.png");
		DEF_FILENAME(SPRITE, STAND, "stand.png");
		DEF_FILENAME(SPRITE, SPLIT, "split.png");
		DEF_FILENAME(SPRITE, DOUBLE_DOWN, "doubleDown.png");

		DEF_FILENAME(SPRITE, LOGIN_BG, "loginBg.png");

		DEF_FILENAME(SPRITE, CHANNEL_LIST_BG, "channelListBg.png");

		DEF_FILENAME(SPRITE, SLIDER_TRACK, "barTrack.png");
		DEF_FILENAME(SPRITE, SLIDER_PROGRESS, "barProgress.png");
		DEF_FILENAME(SPRITE, SLIDER_THUMB, "thumb.png");

		// 1¹øºÎÅÍ ½ÃÀÛ
		static std::string POKE_ARRAY[MAX_NUMBER_OF_POKEMON_AVAILABLE + 1] =
		{
			std::string("error"),
			DEF_FILENAME_POKEMON(POKE, u8"°ÅºÏ¿Õ.png"),
			DEF_FILENAME_POKEMON(POKE, u8"³ª¿ËÀÌ.png"),
			DEF_FILENAME_POKEMON(POKE, u8"¸Á³ª´¨.png"),
			DEF_FILENAME_POKEMON(POKE, u8"ÇÁÅ×¶ó.png"),
			DEF_FILENAME_POKEMON(POKE, u8"ÇÇÄ«Ãò.png"),
		};

		static std::string CARD_ARRAY[4+1][13+1] = 
		{
			{},
			{
				std::string("error"),
				DEF_FILENAME_CARD(CARD,"SA.png"),
				DEF_FILENAME_CARD(CARD,"S2.png"),
				DEF_FILENAME_CARD(CARD,"S3.png"),
				DEF_FILENAME_CARD(CARD,"S4.png"),
				DEF_FILENAME_CARD(CARD,"S5.png"),
				DEF_FILENAME_CARD(CARD,"S6.png"),
				DEF_FILENAME_CARD(CARD,"S7.png"),
				DEF_FILENAME_CARD(CARD,"S8.png"),
				DEF_FILENAME_CARD(CARD,"S9.png"),
				DEF_FILENAME_CARD(CARD,"S10.png"),
				DEF_FILENAME_CARD(CARD,"SJ.png"),
				DEF_FILENAME_CARD(CARD,"SQ.png"),
				DEF_FILENAME_CARD(CARD,"SK.png"),
			},
			{
				std::string("error"),
				DEF_FILENAME_CARD(CARD,"DA.png"),
				DEF_FILENAME_CARD(CARD,"D2.png"),
				DEF_FILENAME_CARD(CARD,"D3.png"),
				DEF_FILENAME_CARD(CARD,"D4.png"),
				DEF_FILENAME_CARD(CARD,"D5.png"),
				DEF_FILENAME_CARD(CARD,"D6.png"),
				DEF_FILENAME_CARD(CARD,"D7.png"),
				DEF_FILENAME_CARD(CARD,"D8.png"),
				DEF_FILENAME_CARD(CARD,"D9.png"),
				DEF_FILENAME_CARD(CARD,"D10.png"),
				DEF_FILENAME_CARD(CARD,"DJ.png"),
				DEF_FILENAME_CARD(CARD,"DQ.png"),
				DEF_FILENAME_CARD(CARD,"DK.png"),
			},
			{
				std::string("error"),
				DEF_FILENAME_CARD(CARD,"HA.png"),
				DEF_FILENAME_CARD(CARD,"H2.png"),
				DEF_FILENAME_CARD(CARD,"H3.png"),
				DEF_FILENAME_CARD(CARD,"H4.png"),
				DEF_FILENAME_CARD(CARD,"H5.png"),
				DEF_FILENAME_CARD(CARD,"H6.png"),
				DEF_FILENAME_CARD(CARD,"H7.png"),
				DEF_FILENAME_CARD(CARD,"H8.png"),
				DEF_FILENAME_CARD(CARD,"H9.png"),
				DEF_FILENAME_CARD(CARD,"H10.png"),
				DEF_FILENAME_CARD(CARD,"HJ.png"),
				DEF_FILENAME_CARD(CARD,"HQ.png"),
				DEF_FILENAME_CARD(CARD,"HK.png"),
			},
			{
				std::string("error"),
				DEF_FILENAME_CARD(CARD,"CA.png"),
				DEF_FILENAME_CARD(CARD,"C2.png"),
				DEF_FILENAME_CARD(CARD,"C3.png"),
				DEF_FILENAME_CARD(CARD,"C4.png"),
				DEF_FILENAME_CARD(CARD,"C5.png"),
				DEF_FILENAME_CARD(CARD,"C6.png"),
				DEF_FILENAME_CARD(CARD,"C7.png"),
				DEF_FILENAME_CARD(CARD,"C8.png"),
				DEF_FILENAME_CARD(CARD,"C9.png"),
				DEF_FILENAME_CARD(CARD,"C10.png"),
				DEF_FILENAME_CARD(CARD,"CJ.png"),
				DEF_FILENAME_CARD(CARD,"CQ.png"),
				DEF_FILENAME_CARD(CARD,"CK.png"),
			}
		};
	}

	namespace FONT
	{
		DEF_FILENAME(FONT, PIXEL, "pixel.ttf");
		DEF_FILENAME(FONT, SOYANON, "default.ttf");
	}
	namespace AUDIO
	{
		DEF_FILENAME(AUDIO, LOGIN_BGM, "loginBgm.mp3");
		DEF_FILENAME(AUDIO, LOGIN_BUTTON, "login.mp3");
		DEF_FILENAME(AUDIO, LOGOUT_BUTTON, "logout.wav");
		DEF_FILENAME(AUDIO, CHANNEL_SELECT_BGM, "channel.mp3");
		DEF_FILENAME(AUDIO, GAME_READY_BGM, "gameReadyBgm.mp3");
		DEF_FILENAME(AUDIO, GAME_BATTLE_BGM, "battle.mp3");

		DEF_FILENAME(AUDIO, BURST, "burst.wav");
		DEF_FILENAME(AUDIO, STAND, "stand.wav");
		DEF_FILENAME(AUDIO, HIT, "hit.wav");
		DEF_FILENAME(AUDIO, DOUBLE_DOWN, "doubleDown.wav");
		DEF_FILENAME(AUDIO, RESULT_WIN, "earnMoney.wav");
		DEF_FILENAME(AUDIO, RESULT_LOSE, "loseMoney.wav");
	}
}