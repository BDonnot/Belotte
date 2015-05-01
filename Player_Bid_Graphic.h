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

#if defined(__GNUC__)
	#include "SDL/SDL.h"
	#include "SDL/SDL_image.h"
#elif defined(_MSC_VER)
	#include <SDL.h>
	#include <SDL_image.h>
#endif //SDL include

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
        Player_Bid_Graphic(const Player_ID& player_number,SDL_Event* pevent,SDL_Surface* screen,Uint windows_width,Uint windows_height);
        virtual ~Player_Bid_Graphic(){}
        void Display(SDL_Surface* screen,GAME_PHASES currentPhase);
        void Coinche();
        void UpdateEvent();
        bool AddBid(const Card_Color& color,Uint bid);

        std::string GetString(Uint biddingTurn) const;
    protected:
        virtual void handleGraphicsIfAny();
        void HandleColor();
        void HandleText();
    private:
};
#endif // PLAYER_BID_GRAPHIC_H
