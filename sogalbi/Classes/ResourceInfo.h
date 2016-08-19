#pragma once
#include <string>

namespace FOLDERNAME
{
	const std::string SPRITE = "Sprites/";
	const std::string FONT = "fonts/";
	const std::string AUDIO = "Sounds/";
}

namespace FILENAME
{
#define DEF_FILENAME(kind,name,filename) const std::string name = FOLDERNAME::kind + filename
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

		DEF_FILENAME(SPRITE, POKE_1, "poke/�źϿ�.png");
		DEF_FILENAME(SPRITE, POKE_2, "poke/������.png");
		DEF_FILENAME(SPRITE, POKE_3, "poke/������.png");
		DEF_FILENAME(SPRITE, POKE_4, "poke/���׶�.png");
		DEF_FILENAME(SPRITE, POKE_5, "poke/��ī��.png");
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