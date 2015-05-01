#ifndef TRICKBASIC_MEMORY_H
#define TRICKBASIC_MEMORY_H

/**
** This class is the basic one handling the trick.
** It memorizes (and computes) all informations related to the trick memory
** -the current winner
** -the last player who have played
** -the number of trick played
** -etc.
** Copy safe, and does not influence _infos
**/

#include <array>

#include "Definitions.h"
#include "Cards.h"

//Debugging
#include "DebugwithPrint.h"
#define TRICKBASICMEMORY_DEBUG 0

class TrickBasic_Memory
{
    protected:
        Basic_Game_Info _infos;
        Uint _trickNumber; //the current trick number : 0 to 7
        Uint _cardsPlayed; //the number of cards played : 0 to 4
        Player_ID _to_play;
        Player_ID _oldFirstPlayer;
        Position_Trick _winner;
        IntIntPair _scores;

        Card_Color _colorAsked;
        Card_Color _colorMaster;
        Card_Height _heightMaster;

        bool _trickFinished;

        std::array<Cards*,4> _currentTrick;
        WrapperPrint<TRICKBASICMEMORY_DEBUG> _printf;
    public:
        TrickBasic_Memory(){Reset();}
        virtual ~TrickBasic_Memory(){}

        const Uint& TrickNumber() const {return _trickNumber;}
        const Uint& NumberCardsPlayed() const {return _cardsPlayed;}
        const Player_ID& NextPlaying() const {return _to_play;}
        const Position_Trick& CurrentWinner() const {return _winner;}
        const IntIntPair& CurrentScores() const {return _scores;}
        const Card_Color& ColorAsked() const {return _colorAsked;}
        const Card_Color& ColorMaster() const {return _colorMaster;}
        const Card_Height& HeightMaster() const {return _heightMaster;}
        const Cards* operator[](Uint i) const {return _currentTrick[i];}

        bool IsFallen(const Card_Color& color, const Card_Height& height) const; //compute if the card is fallen in the trick or not
        void Reset(); //at the end of the game (8 trick)

        void PutCard(Cards* card);
        void NextToPlay();

    protected:
        virtual void trickFinished(){ _trickFinished = true; } //update _trickFinished
        void playerPlayed(); //to do after each player played
        void updateWinner(); //update the player who currently win the trick
        void finishTrick();
        void updateToPlay(); //at the end of the trick, and store the scores
        void updateScores(Uint pointsInTheTrick);
        void trickOver(); //do whatever you have to when a the trick is over
        virtual void updateBasicGameInfo(){} //this class does not update it

    private:
};

#endif // TRICKBASIC_MEMORY_H
