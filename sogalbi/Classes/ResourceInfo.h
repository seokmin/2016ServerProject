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
	}
	namespace FONT
	{
		DEF_FILENAME(FONT, PIXEL, "pixel.ttf");
		DEF_FILENAME(FONT, SOYANON, "default.ttf");
	}
}