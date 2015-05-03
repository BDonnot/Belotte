#ifndef PLAYER_H
#define PLAYER_H

/**
** This class defines what is a player in a generic way.
** The different kinds of players are derived from this class.
** There are two main kinds of players : the AI and the human.
** For historical reason, this is not templated.
** One improvement would be to delete the classes "Player_AI" and "Player_Human"
** and regroup everyone in this class, which would be templated.
**/
#include <list>
#include <array>
#include <algorithm>

#include "Definitions.h"
#include "Basic_Game_Info.h"
#include "Player_Bid.h"
#include "TrickStatus.h"
#include "TrickBasic_Memory.h"
#include "Foncteurs_Cards.h"
#include "Foncteurs_CardsBasic.h"
#include "Foncteurs_Players.h"

#include "Cards.h"
#include "Player_Name.h"
#include "Player_Bid_Graphic.h"
#include "BetsMemory.h" //for the bets to be remembered

//Define the variable for debugging purpose
#include "DebugwithPrint.h"
#define PLAYER_DEBUG 0

///TO DO : template to remove Player_AI and Player_Human...
///->static polymorphism.

class Player
{
    protected:
        std::list<Cards*> _hand; //the hand of the player
        std::list<Cards* > _playable_cards; //the card a player can play in a trick
        const Player_ID _number; //the number
        Basic_Game_Info _basic_info;
        Cards* _cardPlayed;
        TrickStatus _currentTrickStatus;

        PositionGraphic _pos;
        Player_Name _name; //his name
        DisplayCardPlayer _DisplayCardPlayer;//to display the cards the proper way
        SDL_Surface* _screen;
        SDL_Event* _event;
        Player_Bid _currentBid;
        Player_Bid_Graphic _oldBid;

        CanPlayCards<Cards*> _fCanPlayCard;
        WrapperPrint<PLAYER_DEBUG> _printf;


    public:
        Player():_number(PLAYER0),_currentTrickStatus(_number){}
        Player(const Player_ID& number,Uint windows_width, Uint windows_height,SDL_Event* pevent,Basic_Images* fond,SDL_Surface* screen):
            _playable_cards()
            ,_number(number)
            ,_cardPlayed(nullptr)
            ,_currentTrickStatus(_number)
            ,_pos(setPosition(number,windows_width,windows_height))
            ,_name(fond
                   ,setName(number)
                   ,setPathImage(number)
                   ,pevent
                   ,windows_width
                   ,windows_height
                   ,_pos)
            ,_DisplayCardPlayer(screen,windows_width,windows_height,number)
            ,_oldBid(number,pevent,screen,windows_width,windows_height)
        {
            _screen = screen;
        }

        virtual ~Player()
        {
            for (Cards * pcards : _hand)
            {
                delete(pcards);
            }
        }

        void ReceivedCard(Cards* pcard,unsigned int card_number); //received a card, during the phase of giving
        virtual void Display(GAME_PHASES currentPhase); //to display the right thing : the name, the image, and the cards in the hand.

        virtual void Update_Mouse(GAME_PHASES currentPhase); //to update the position of the mouse, the click etc.

        Cards* PlayCard(const TrickBasic_Memory& trick); //choose the right card to play
        void UpdateEndTrick(const TrickBasic_Memory& trick,const Position_Trick& myPos); //do whatever you have to do at the end of each trick
        Card_Color Take(bool first_round,const Card_Color& color_proposed,const Card_Height& height_proposed); //choose if the player take or not (classic game)
        const Player_Bid& Take(bool previousPlayerChoose,const BetsMemory& bets); //choose if the player take or not (coinche)
        const Player_Bid& Take(const BetsMemory& bets); //choose if the player take or not (coinche)
        const bool Coinche();

        const unsigned int GetHandSize() const { return _hand.size(); }
        const std::string& Getname() const { return _name.Name(); }
        virtual void ResetBid(bool betFinished);

        void GiveCardsBack(std::list<Cards*>& deck);
        const Card_Color& CurrentColorBid();

        void InitMemory(); //and sort the hand

        std::string GetString(const std::string& embraced) const; //return <Player : (#)> (embraced) <\player>
        std::string GetString() const; //return <Player : (#)> (hands) <\player>
        std::string GetString(Uint biddingCount) const; //return <Player : (#)> (bet) <\player>
        std::string GetString(const Player_Bid& bid) const; //return <Player : (#)> (bet) <\player>

    protected:
        virtual void updateMemoryTrick(const TrickBasic_Memory& trick,const Position_Trick& myPos){}
        virtual void initMemoryTrick() {}
        virtual void updateBid(const BetsMemory& bets){}
        virtual bool do_I_coinche();

        virtual Card_Color do_i_take(bool first_round,const Card_Color& color_proposed,const Card_Height& height_proposed);
        virtual Cards* what_card_do_i_play(const TrickBasic_Memory& trick) {return _hand.front();}

        int how_many_colour(const Card_Color& colour);

        virtual void resetTake(){} //for the AI to be able to forget the previous bets
    private:
        PositionGraphic setPosition(const Player_ID& number,Uint16 windows_width, Uint16 widows_height); //to properly set the right position
        std::string setName(const Player_ID&  number); //to set the default name
        std::string setPathImage(const Player_ID&  number); //to set the path of the image of the player

        void updatePlayebleCards(const TrickBasic_Memory& trick_in_progress);
        bool can_play_card(Cards* PmyCard,const TrickBasic_Memory& trick);

        bool has_colour(const Card_Color& color);
};

#endif // PLAYER_H
