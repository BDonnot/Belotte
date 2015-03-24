#ifndef TRICKBASIC_MEMORY_H
#define TRICKBASIC_MEMORY_H

#include <array>

#include "Definitions.h"
#include "Cards.h"

/**Memorize all informations related to the trick memory :
-the current winner
-the last player who have played
-the number of trick played
-etc.
**/

class TrickBasic_Memory
{
    protected:
        Basic_Game_Info _infos;
        Uint _trickNumber; //the current trick number : 1 to 8
        Uint _cardsPlayed; //the number of cards played : 0 to 4
        PLAYER_ID _to_play;
        POSITION_TRICK _winner;
        IntIntPair _scores;

        CARDS_COLOR _colorAsked;
        CARDS_COLOR _colorMaster;
        CARDS_HEIGHT _heightMaster;

        std::array<Cards*,4> _currentTrick;
    public:
        TrickBasic_Memory();
        virtual ~TrickBasic_Memory();

        const Uint TrickNumber() const { return _trickNumber;}
        const Uint NumberCardsPlayed() const {return _cardsPlayed;}
        const PLAYER_ID NextPlaying() const {return _to_play;}
        const POSITION_TRICK CurrentWinner() const {return _winner;}
        const IntIntPair& CurrentScores() const {return _scores;}

        void PlayerPlay()
        {
            updateWinner();
            _to_play = _infos.Next(_to_play);
            _cardsPlayed++;
            if(_cardsPlayed == 4) //The trick is over
            {
                _cardsPlayed = 0;
                _trickNumber++;
                finishTrick();
                _winner = UNKNOWN;
            }

        }
        void Reset(); //at the end of the game (8 trick)
    protected:
        void updateWinner(); //update the player who currently win the trick

        void finishTrick();
        void updateToPlay(); //at the end of the trick, and store the scores
        void updateScores(Uint pointsInTheTrick);
    private:
};

#endif // TRICKBASIC_MEMORY_H
