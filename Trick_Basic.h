#ifndef TRICK_BASIC_H
#define TRICK_BASIC_H

#include <string>
#include <array>

#include "Basic_Game_Info.h"
#include "TrickBasic_Memory.h"
//#include "Cards_Basic.h"
#include "Cards.h" // TO DO : graphics here : to remove
#include "Player.h" // TO DO : graphics here : to remove
#include "Score_Team.h" // TO DO : graphics here : to remove
#include "Cards_Deck.h" // TO DO : graphics here : to remove

class Trick_Basic : public TrickBasic_Memory
{
    protected :
        TrickBasic_Memory _trickMemory;
        std::array<POSITION_TRICK,4> _posTrick;

        std::array<Player*,4> _players;
        std::array<Cards*,4> _previousTrick;
        Uint _cardsPlayedThisTurn;
        PLAYER_ID _oldFirstPlayer;
        Cards_Deck* _pDeck;
        Uint _number_cards_play;

    public:
        Trick_Basic(){}
        Trick_Basic(Cards_Deck* pDeck,const std::array<Player*,4>& players);
        virtual ~Trick_Basic();

        void SetPlayers(const std::array<Player*,4>& players);
        bool Play();

        void WinnerTakeCards();

        Uint TrickNumber() const {return _trickMemory.TrickNumber(); }
        std::string GetString() const;
    protected:
        void terminatecurrentTrick(Uint first); //compute the score
        POSITION_TRICK whoWinTrick();
        virtual bool trickFinished(){ return true;}
        //virtual void animateIfGraphic(Cards_Basic* pcard, Uint i){}
        virtual void animateIfGraphic(Cards* pcard, Uint i){}
    private:
};

#endif // TRICK_BASIC_H
