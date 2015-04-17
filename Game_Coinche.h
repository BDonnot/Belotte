#ifndef GAME_COINCHE_H
#define GAME_COINCHE_H

#include<array>
#include<list>
#include <fstream>

#include "Basic_Game_Info.h"
#include "Quit.h"
#include "Basic_Images.h"
#include "Cards_Deck.h"
#include "Player.h"
#include "Player_Human.h"
#include "Player_AI.h"
//#include "GameMemory.h"
#include "AIPlayRandom.h"
#include "AIPlayScores.h"
#include "AIPlayMonteCarlo.h"
#include "AIGameMemory.h"
//#include "AIBetsMemory.h"
//#include "AIGameMemory.h"
//#include "AIPlayRandom.h"

#include "AITakeBasic.h"
#include "Screen_Begin.h"
#include "Screen_Option.h"
#include "Screen_end.h"
#include "Bidding.h"
#include "Trick.h"
#include "End_Of_Game.h"
#include "Save_Game.h"

class Game_Coinche : public Quit
{
    protected :
        Basic_Game_Info _infos;
        SDL_Surface* _pScreen;
        Basic_Images _backSide;
        Screen_Begin _begin;
        Screen_end _end;
        Screen_end _error;
        Cards_Deck _deck;
        std::array<Player*,4> _players;
        GAME_PHASES _currentPhase;
        Bidding _bid;
        Uint32 _timeNextAction;
        Trick _trick;
        End_Of_Game _endGame;
        Save_Game _saveGame; //to save the game


    public:
        Game_Coinche();
        void Play();
        Game_Coinche(SDL_Surface* screen,Uint16 screenWidth, Uint16 screenHeight);
        virtual ~Game_Coinche();
    protected:
        void updateEvent(bool& keep_playing);
        void updateTime(Uint32 diffTime);
        void playGame(bool& keep_playing);
        void Display();
    private:
        bool waitForClick();
};

#endif // GAME_COINCHE_H
