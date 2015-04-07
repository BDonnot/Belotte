#ifndef PLAYER_BID_GRAPHIC_H
#define PLAYER_BID_GRAPHIC_H

#include<list>
#include<algorithm>
#include<string>
#include<ostream>
#include<sstream>

#include"Definitions.h"

#include "Player_Bid.h"
#include "Images_SpriteEvent.h"
#include "Images_Sprite.h"
#include "Images_Text.h"

class DisplayOldBets //handle the positioning of the card in the hand
{
    private :
        Uint _nb_bets;
        SDL_Surface* _screen;
        Images_Text* _pText;
        Images_Sprite<4>* _pLogo;

        PositionGraphic _pos_real;
        PositionGraphic _pos_th;
        std::array<Uint,2> _shift; //the shift of the card (all the card must be visible)
        Uint colorToInt(CARDS_COLOR color);
    public :
        DisplayOldBets():_pText(nullptr),_pLogo(nullptr){};
        DisplayOldBets(SDL_Surface* screen
                       ,Uint windows_width
                       ,Uint windows_height
                       ,Uint player_number
                       ,Images_Text* text
                       ,Images_Sprite<4>* logo);
        void Reset();
        void operator()(const Player_Bid& bid);
};

class Player_Bid_Graphic : public Player_Bid
{
    protected :
        Images_SpriteEvent<4> _bubble; //the bubble around the text
        Images_Text _text; //the text "passe" or "110", or "coinche"
        Images_Sprite<4> _color; //logo of the color (if any)
        //bool _displayed;
        bool _displayed_logo_color;
        Images_Sprite<4> _oldColor;
        Images_Text _oldText;
        std::list<Player_Bid> _listOldBet;
        DisplayOldBets _dsplOldBets;
    public:
        Player_Bid_Graphic();
        Player_Bid_Graphic(Uint player_number,SDL_Event* pevent,SDL_Surface* screen,Uint windows_width,Uint windows_height);
        virtual ~Player_Bid_Graphic();
        void Display(SDL_Surface* screen,GAME_PHASES currentPhase);
        void Coinche();
        void UpdateEvent();
        bool AddBid(const Card_Color& color,Uint bid);
        static std::string IntToString(const Card_Color& color,const Uint& number);

        std::string GetString(Uint biddingTurn) const;
    protected:
        virtual void handleGraphicsIfAny();
        void HandleColor();
        void HandleText();
    private:
};
#endif // PLAYER_BID_GRAPHIC_H
