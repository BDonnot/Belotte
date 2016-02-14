#ifndef DISPLAYOLDBETS_H
#define DISPLAYOLDBETS_H

/**
** This class handles the displaying of the old bets.
** When the mouse is over a player, during the Bidding phases
** we can see which bets (color and value) have been done by this player.
**/
#include<string>

#if COMPLETE_GAME > 0
#if defined(__GNUC__)
	#include "SDL/SDL.h"
	#include "SDL/SDL_image.h"
#elif defined(_MSC_VER)
	#include <SDL.h>
	#include <SDL_image.h>
#endif //SDL include
#endif // COMPLETE_GAME > 0

#include"Definitions.h"

#include "Player_Bid.h"
#include "Images_Sprite.h"
#include "Images_Text.h"

namespace SDL
{
#if COMPLETE_GAME > 0
	class DisplayOldBets //handle the positioning of the card in the hand
	{
	private:
		Uint _nb_bets;
		SDL_Surface* _screen;
		Images_Text* _pText;
		Images_Sprite<4>* _pLogo;

		PositionGraphic _pos_real;
		PositionGraphic _pos_th;
		std::array<Uint, 2> _shift; //the shift of the card (all the card must be visible)
		Uint colorToInt(CARDS_COLOR color);
	public:
		DisplayOldBets() :_pText(nullptr), _pLogo(nullptr) {}
		DisplayOldBets(SDL_Surface* screen
			, Uint windows_width
			, Uint windows_height
			, const Player_ID& player_number
			, Images_Text* text
			, Images_Sprite<4>* logo);
		static std::string IntToString(const Card_Color& color, const Uint& number);
		void Reset();
		void operator()(const Player_Bid& bid);
	};
#else
	class DisplayOldBets //handle the positioning of the card in the hand
	{
	private:
		Uint _nb_bets;
		Images_Text* _pText;
		Images_Sprite<4>* _pLogo;

		PositionGraphic _pos_real;
		PositionGraphic _pos_th;
		std::array<Uint, 2> _shift; //the shift of the card (all the card must be visible)
		Uint colorToInt(CARDS_COLOR color);
	public:
		DisplayOldBets() :_pText(nullptr), _pLogo(nullptr) {}
		DisplayOldBets(Uint windows_width
			, Uint windows_height
			, const Player_ID& player_number
			, Images_Text* text
			, Images_Sprite<4>* logo) {}
		static std::string IntToString(const Card_Color& color, const Uint& number) {}
		void Reset(){}
		void operator()(const Player_Bid& bid) {}
	};
#endif //COMPLETE_GAME > 0
};
#endif // DISPLAYOLDBETS_H
