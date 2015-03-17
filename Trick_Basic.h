#ifndef TRICK_BASIC_H
#define TRICK_BASIC_H

#include <string>
#include <array>

#include "Basic_Game_Info.h"
#include "Cards_Basic.h"
#include "Cards.h" // TO DO : graphics here : to remove
#include "Player.h" // TO DO : graphics here : to remove
#include "Score_Team.h" // TO DO : graphics here : to remove
#include "Cards_Deck.h" // TO DO : graphics here : to remove

class Trick_Basic
{
    protected :
        Basic_Game_Info _info;

        std::array<POSITION_TRICK,4> _posTrick;

        std::array<Player*,4> _players;
        //std::array<Cards_Basic*,4> _currentTrick;
        //std::array<Cards_Basic*,4> _previousTrick;
        std::array<Cards*,4> _currentTrick;
        std::array<Cards*,4> _previousTrick;
        Uint _cardsPlayedThisTurn;
        PLAYER_ID _oldFirstPlayer;
        Score_Team _scoreTeam;
        Cards_Deck* _pDeck;

    public:
        Trick_Basic();
        Trick_Basic(Cards_Deck* pDeck,const std::array<Player*,4>& players,SDL_Surface* pScreen);
        virtual ~Trick_Basic();

        void SetPlayers(const std::array<Player*,4>& players);
        bool Play();

        void WinnerTakeCards();

        std::string GetString() const;
    protected:
        POSITION_TRICK whoWinTrick();
        virtual bool trickFinished()
        {
            return true;
        }
        virtual void animateIfGraphic(Cards* pcard, Uint i)
        {

        }
    private:
};

#endif // TRICK_BASIC_H
