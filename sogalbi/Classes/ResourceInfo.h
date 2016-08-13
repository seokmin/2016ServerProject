#pragma once
#include <string>

namespace FOLDERNAME
{
	const std::string SPRITE = "Sprites/";
	const std::string FONT = "fonts/";
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

		DEF_FILENAME(SPRITE, POKE_1, "poke/°ÅºÏ¿Õ.png");
		DEF_FILENAME(SPRITE, POKE_2, "poke/³ª¿ËÀÌ.png");
		DEF_FILENAME(SPRITE, POKE_3, "poke/¸Á³ª´¨.png");
		DEF_FILENAME(SPRITE, POKE_4, "poke/ÇÁÅ×¶ó.png");
		DEF_FILENAME(SPRITE, POKE_5, "poke/ÇÇÄ«Ãò.png");
	}
	namespace FONT
	{
		DEF_FILENAME(FONT, PIXEL, "pixel.ttf");
		DEF_FILENAME(FONT, SOYANON, "default.ttf");
	}
}