#ifndef TRICK_BASIC_H
#define TRICK_BASIC_H

#include <string>
#include <array>

#include "Basic_Game_Info.h"
#include "TrickBasic_Memory.h"

#include "Cards.h"
#include "Player.h"
#include "Score_Team.h"
#include "Cards_Deck.h"

#include "DebugwithPrint.h"
#define TRICKBASIC_DEBUG 1

class Trick_Basic : public TrickBasic_Memory
{
    protected :
        //TrickBasic_Memory _trickMemory;
        std::array<POSITION_TRICK,4> _posTrick;

        std::array<Player*,4> _players;
        std::array<Cards*,4> _previousTrick;
        Uint _cardsPlayedThisTurn;
        Cards_Deck* _pDeck;
        WrapperPrint<TRICKBASIC_DEBUG> _printf;

    public:
        Trick_Basic(){}
        Trick_Basic(Cards_Deck* pDeck,const std::array<Player*,4>& players);
        virtual ~Trick_Basic();

        void SetPlayers(const std::array<Player*,4>& players);
        bool Play();

        void WinnerTakeCards();

        std::string GetString() const;
    protected:
        void terminatecurrentTrick(Uint first); //compute also the scores
        Position_Trick whoWinTrick();

        virtual void animateIfGraphic(Cards* pcard, Uint i){}
        virtual void updateBasicGameInfo(){_infos.SetScores(_to_play,_scores);}
    private:
};

#endif // TRICK_BASIC_H
