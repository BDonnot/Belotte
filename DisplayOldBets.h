#ifndef DISPLAYOLDBETS_H
#define DISPLAYOLDBETS_H

#include<string>

/*
#include<list>
#include<algorithm>
#include<ostream>
#include<sstream>
*/

#if defined(__GNUC__)
	#include "SDL/SDL.h"
	#include "SDL/SDL_image.h"
#elif defined(_MSC_VER)
	#include <SDL.h>
	#include <SDL_image.h>
#endif //SDL include

#include"Definitions.h"

#include "Player_Bid.h"
#include "Images_Sprite.h"
#include "Images_Text.h"

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
		DisplayOldBets() :_pText(nullptr), _pLogo(nullptr){}
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

#endif // DISPLAYOLDBETS_H
