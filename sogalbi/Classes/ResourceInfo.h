#pragma once

#include <string>

namespace FOLDERNAME
{
	const std::string SPRITE = "Sprites/";
	const std::string FONT = "fonts/";
	const std::string AUDIO = "Sounds/";
	const std::string POKE = "Sprites/poke/";
}

namespace FILENAME
{
#define DEF_FILENAME(kind,name,filename) const std::string name = FOLDERNAME::kind + filename
#define DEF_FILENAME_POKEMON(kind,filename) FOLDERNAME::kind + filename
#define MAX_NUMBER_OF_POKEMON_AVAILABLE 5
	namespace SPRITE
	{
		DEF_FILENAME(SPRITE, TABLE_BG, "tableBg.png");
		DEF_FILENAME(SPRITE, DECK, "deckRed.png");
		DEF_FILENAME(SPRITE, NAMETAG, "nameTag.png");
		DEF_FILENAME(SPRITE, NAMETAG_BACK, "nameTagBack.png");
		DEF_FILENAME(SPRITE, BANNER_BLACKJACK, "bannerBlackJack.png");
		DEF_FILENAME(SPRITE, BANNER_STAND, "bannerStand.png");
		DEF_FILENAME(SPRITE, BANNER_WAITING, "bannerWaiting.png");
		DEF_FILENAME(SPRITE, HIT, "hit.png");

		DEF_FILENAME(SPRITE, LOGIN_BG, "loginBg.png");

		DEF_FILENAME(SPRITE, CHANNEL_LIST_BG, "channelListBg.png");
				
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
	}

	namespace FONT
	{
		DEF_FILENAME(FONT, PIXEL, "pixel.ttf");
		DEF_FILENAME(FONT, SOYANON, "default.ttf");
	}
	namespace AUDIO
	{
		DEF_FILENAME(AUDIO, LOGIN_BGM, "login.mp3");
	}
}