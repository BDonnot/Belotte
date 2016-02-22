#ifndef PLAYER_BID_GRAPHIC_H
#define PLAYER_BID_GRAPHIC_H

/**
** Store an display the information relative to the bets for the players.
** The old bets are displayed for the player, if the mouse is over the "bubble"
** of its current bet.
**/

#include<list>
#include<algorithm>
#include<string>
#include<ostream>
#include<sstream>

#if COMPLETE_GAME > 0
#if defined(__GNUC__)
	#include "SDL/SDL.h"
	#include "SDL/SDL_image.h"
#elif defined(_MSC_VER)
	#include <SDL.h>
	#include <SDL_image.h>
#endif //SDL include
#endif //#if COMPLETE_GAME > 0

#include"Definitions.h"

#include "Player_Bid.h"

#include "Images_SpriteEvent.h"
#include "Images_Sprite.h"
#include "Images_Text.h"
#include "DisplayOldBets.h"


class Player_Bid_Graphic : public Player_Bid
{
    protected :
        Images_SpriteEvent<4> _bubble; //the bubble around the text
		Images_Text _text; //the text "passe" or "110", or "coinche"
		Images_Sprite<4> _color; //logo of the color (if any)
        bool _displayed_logo_color;
		Images_Sprite<4> _oldColor;
		Images_Text _oldText;
        std::list<Player_Bid> _listOldBet;
		DisplayOldBets _dsplOldBets;
    public:
        Player_Bid_Graphic(){}
#if COMPLETE_GAME > 0
        Player_Bid_Graphic(const Player_ID& player_number,SDL_Event* pevent,SDL_Surface* screen,Uint windows_width,Uint windows_height);
		void Display(SDL_Surface* screen, GAME_PHASES currentPhase);
#else
		void Display(GAME_PHASES currentPhase) {}
#endif //#if COMPLETE_GAME > 0
		Player_Bid_Graphic(const Player_ID& player_number, Uint windows_width, Uint windows_height) :
			Player_Bid()
			, _bubble("images/sprite_bulles.png")
			, _text("fonts/MLSGY.ttf", 20, 255, 255, 255, "Passe")
			, _color("images/sprite_take_color.png")
			, _displayed_logo_color(true)
			, _oldColor("images/sprite_take_color.png")
			, _oldText("fonts/MLSGY.ttf", 20, 255, 255, 255, "Passe")
			, _dsplOldBets(windows_width, windows_height, player_number, &_oldText, &_oldColor) {}

		virtual ~Player_Bid_Graphic(){}
        void Coinche();

#if COMPLETE_GAME > 0
        void UpdateEvent();
#else 
		void UpdateEvent() {}
#endif //#if COMPLETE_GAME > 0

        bool AddBid(const Card_Color& color,Uint bid);
        std::string GetString(Uint biddingTurn) const;
    protected:
        virtual void handleGraphicsIfAny();
        void HandleColor();
        void HandleText();
    private:
};
#endif // PLAYER_BID_GRAPHIC_H
