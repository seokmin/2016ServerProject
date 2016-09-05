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
#define MAX_NUMBER_OF_POKEMON_AVAILABLE 101
	namespace SPRITE
	{
		DEF_FILENAME(SPRITE, TABLE_BG, "tableBg.png");
		DEF_FILENAME(SPRITE, DECK, "deckRed.png");
		DEF_FILENAME(SPRITE, NAMETAG, "nameTag.png");
		DEF_FILENAME(SPRITE, NAMETAG_BACK, "nameTagBack.png");

		DEF_FILENAME(SPRITE, BANNER_BLACKJACK, "bannerBlackJack.png");
		DEF_FILENAME(SPRITE, BANNER_STAND, "bannerStand.png");
		DEF_FILENAME(SPRITE, BANNER_WAITING, "bannerWaiting.png");
		DEF_FILENAME(SPRITE, BANNER_BURST, "bannerBurst.png");

		DEF_FILENAME(SPRITE, HIT, "hit.png");
		DEF_FILENAME(SPRITE, STAND, "stand.png");
		DEF_FILENAME(SPRITE, SPLIT, "split.png");
		DEF_FILENAME(SPRITE, DOUBLE_DOWN, "doubleDown.png");

		DEF_FILENAME(SPRITE, LOGIN_BG, "loginBg.png");

		DEF_FILENAME(SPRITE, CHANNEL_LIST_BG, "channelListBg.png");

		DEF_FILENAME(SPRITE, SLIDER_TRACK, "barTrack.png");
		DEF_FILENAME(SPRITE, SLIDER_PROGRESS, "barProgress.png");
		DEF_FILENAME(SPRITE, SLIDER_THUMB, "thumb.png");

		// 1번부터 시작
		static std::string POKE_ARRAY[MAX_NUMBER_OF_POKEMON_AVAILABLE + 1] =
		{
			std::string("error"),

			DEF_FILENAME_POKEMON(POKE, u8"가디.png"),
			DEF_FILENAME_POKEMON(POKE, u8"강챙이.png"),
			DEF_FILENAME_POKEMON(POKE, u8"갸라도스.png"),
			DEF_FILENAME_POKEMON(POKE, u8"거북왕.png"),
			DEF_FILENAME_POKEMON(POKE, u8"고라파덕.png"),
			DEF_FILENAME_POKEMON(POKE, u8"고오스.png"),
			DEF_FILENAME_POKEMON(POKE, u8"고우스트.png"),
			DEF_FILENAME_POKEMON(POKE, u8"고지.png"),
			DEF_FILENAME_POKEMON(POKE, u8"근육몬.png"),
			DEF_FILENAME_POKEMON(POKE, u8"꼬렛.png"),
			DEF_FILENAME_POKEMON(POKE, u8"꼬마돌.png"),
			DEF_FILENAME_POKEMON(POKE, u8"꼬부기.png"),
			DEF_FILENAME_POKEMON(POKE, u8"꼬지모.png"),
			DEF_FILENAME_POKEMON(POKE, u8"나시.png"),
			DEF_FILENAME_POKEMON(POKE, u8"나옹이.png"),
			DEF_FILENAME_POKEMON(POKE, u8"날쌩마.png"),
			DEF_FILENAME_POKEMON(POKE, u8"내루미.png"),
			DEF_FILENAME_POKEMON(POKE, u8"냄새꼬.png"),
			DEF_FILENAME_POKEMON(POKE, u8"닥트리오.png"),
			DEF_FILENAME_POKEMON(POKE, u8"단데기.png"),
			DEF_FILENAME_POKEMON(POKE, u8"독침붕.png"),
			DEF_FILENAME_POKEMON(POKE, u8"디그다.png"),
			DEF_FILENAME_POKEMON(POKE, u8"딱충이.png"),
			DEF_FILENAME_POKEMON(POKE, u8"또가스.png"),
			DEF_FILENAME_POKEMON(POKE, u8"또도가스.png"),
			DEF_FILENAME_POKEMON(POKE, u8"뚜벅쵸.png"),
			DEF_FILENAME_POKEMON(POKE, u8"라이츄.png"),
			DEF_FILENAME_POKEMON(POKE, u8"라프라스.png"),
			DEF_FILENAME_POKEMON(POKE, u8"라플레시아.png"),
			DEF_FILENAME_POKEMON(POKE, u8"레어코일.png"),
			DEF_FILENAME_POKEMON(POKE, u8"레트라.png"),
			DEF_FILENAME_POKEMON(POKE, u8"루기아.png"),
			DEF_FILENAME_POKEMON(POKE, u8"루주라.png"),
			DEF_FILENAME_POKEMON(POKE, u8"리아코.png"),
			DEF_FILENAME_POKEMON(POKE, u8"리자몽.png"),
			DEF_FILENAME_POKEMON(POKE, u8"마그마.png"),
			DEF_FILENAME_POKEMON(POKE, u8"마릴.png"),
			DEF_FILENAME_POKEMON(POKE, u8"마임맨.png"),
			DEF_FILENAME_POKEMON(POKE, u8"마자용.png"),
			DEF_FILENAME_POKEMON(POKE, u8"망나뇽.png"),
			DEF_FILENAME_POKEMON(POKE, u8"메타몽.png"),
			DEF_FILENAME_POKEMON(POKE, u8"모다피.png"),
			DEF_FILENAME_POKEMON(POKE, u8"모래두지.png"),
			DEF_FILENAME_POKEMON(POKE, u8"뮤.png"),
			DEF_FILENAME_POKEMON(POKE, u8"뮤츠.png"),
			DEF_FILENAME_POKEMON(POKE, u8"미뇽.png"),
			DEF_FILENAME_POKEMON(POKE, u8"밀탱크.png"),
			DEF_FILENAME_POKEMON(POKE, u8"발챙이.png"),
			DEF_FILENAME_POKEMON(POKE, u8"버터플.png"),
			DEF_FILENAME_POKEMON(POKE, u8"별가사리.png"),
			DEF_FILENAME_POKEMON(POKE, u8"부스터.png"),
			DEF_FILENAME_POKEMON(POKE, u8"부우부.png"),
			DEF_FILENAME_POKEMON(POKE, u8"붐볼.png"),
			DEF_FILENAME_POKEMON(POKE, u8"브케인.png"),
			DEF_FILENAME_POKEMON(POKE, u8"뿔충이.png"),
			DEF_FILENAME_POKEMON(POKE, u8"삐삐.png"),
			DEF_FILENAME_POKEMON(POKE, u8"샤미드.png"),
			DEF_FILENAME_POKEMON(POKE, u8"수륙챙이.png"),
			DEF_FILENAME_POKEMON(POKE, u8"스라크.png"),
			DEF_FILENAME_POKEMON(POKE, u8"슬리퍼.png"),
			DEF_FILENAME_POKEMON(POKE, u8"슬리프.png"),
			DEF_FILENAME_POKEMON(POKE, u8"식스테일.png"),
			DEF_FILENAME_POKEMON(POKE, u8"신뇽.png"),
			DEF_FILENAME_POKEMON(POKE, u8"썬더.png"),
			DEF_FILENAME_POKEMON(POKE, u8"아보.png"),
			DEF_FILENAME_POKEMON(POKE, u8"아보크.png"),
			DEF_FILENAME_POKEMON(POKE, u8"아쿠스타.png"),
			DEF_FILENAME_POKEMON(POKE, u8"야도란.png"),
			DEF_FILENAME_POKEMON(POKE, u8"왕콘치.png"),
			DEF_FILENAME_POKEMON(POKE, u8"우츠동.png"),
			DEF_FILENAME_POKEMON(POKE, u8"윈디.png"),
			DEF_FILENAME_POKEMON(POKE, u8"윤겔라.png"),
			DEF_FILENAME_POKEMON(POKE, u8"이브이.png"),
			DEF_FILENAME_POKEMON(POKE, u8"이상해씨.png"),
			DEF_FILENAME_POKEMON(POKE, u8"이상해풀.png"),
			DEF_FILENAME_POKEMON(POKE, u8"잉어킹.png"),
			DEF_FILENAME_POKEMON(POKE, u8"잠만보.png"),
			DEF_FILENAME_POKEMON(POKE, u8"쥬레곤.png"),
			DEF_FILENAME_POKEMON(POKE, u8"쥬쥬.png"),
			DEF_FILENAME_POKEMON(POKE, u8"쥬피썬더.png"),
			DEF_FILENAME_POKEMON(POKE, u8"질뻐기.png"),
			DEF_FILENAME_POKEMON(POKE, u8"질퍽이.png"),
			DEF_FILENAME_POKEMON(POKE, u8"찌리리공.png"),
			DEF_FILENAME_POKEMON(POKE, u8"치코리타.png"),
			DEF_FILENAME_POKEMON(POKE, u8"칠색조.png"),
			DEF_FILENAME_POKEMON(POKE, u8"캐터피.png"),
			DEF_FILENAME_POKEMON(POKE, u8"코일.png"),
			DEF_FILENAME_POKEMON(POKE, u8"콘치.png"),
			DEF_FILENAME_POKEMON(POKE, u8"크랩.png"),
			DEF_FILENAME_POKEMON(POKE, u8"탕구리.png"),
			DEF_FILENAME_POKEMON(POKE, u8"토게피.png"),
			DEF_FILENAME_POKEMON(POKE, u8"파이리.png"),
			DEF_FILENAME_POKEMON(POKE, u8"파이어.png"),
			DEF_FILENAME_POKEMON(POKE, u8"팬텀.png"),
			DEF_FILENAME_POKEMON(POKE, u8"포니타.png"),
			DEF_FILENAME_POKEMON(POKE, u8"푸린.png"),
			DEF_FILENAME_POKEMON(POKE, u8"프리져.png"),
			DEF_FILENAME_POKEMON(POKE, u8"프테라.png"),
			DEF_FILENAME_POKEMON(POKE, u8"피죤투.png"),
			DEF_FILENAME_POKEMON(POKE, u8"피카츄.png"),
			DEF_FILENAME_POKEMON(POKE, u8"홍수몬.png"),
		};

		static std::string CARD_ARRAY[4 + 1][13 + 1] =
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


		DEF_FILENAME(AUDIO, BLACK_JACK, "blackJack.mp3");
		DEF_FILENAME(AUDIO, BURST, "burst.wav");
		DEF_FILENAME(AUDIO, STAND, "stand.wav");
		DEF_FILENAME(AUDIO, HIT, "hit.wav");
		DEF_FILENAME(AUDIO, DOUBLE_DOWN, "doubleDown.wav");
		DEF_FILENAME(AUDIO, RESULT_WIN, "earnMoney.wav");
		DEF_FILENAME(AUDIO, RESULT_LOSE, "loseMoney.wav");
	}
}