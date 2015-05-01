#ifndef GAME_COINCHE_H
#define GAME_COINCHE_H

/**
** This is the class of the game of Coinche.
** It handles the proper behavior of the main classes,
** and ensure the good progress of the game.
** It defines the class of player used.
** It is a "conductor", in a sens that it does nothing,
** but tells everyone when it is time to work.
**/
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

#include "AIPlayRandom.h"
#include "AIPlayScores.h"
#include "AIPlayMonteCarlo.h"
#include "MCPlayGames.h"

#include "AIGameMemory.h"
#include "AIGameMemoryImproved.h"

#include "AITakeBasic.h"
#include "Screen_Begin.h"
#include "Screen_Option.h"
#include "Screen_end.h"
#include "Bidding.h"
#include "Trick.h"
#include "End_Of_Game.h"
#include "Save_Game.h"

//Define the variable for debuging purpose
#include "DebugwithPrint.h"
#define GAME_COINCHE_DEBUG 0

///Add the exception :
///TO DO : future feature to be added here, one day... like handling an exception and trying to recover the game from the files saved...
class Game_Coinche : public Quit
{
    protected :
        typedef Player_AI<AIGameMemoryImproved,AITakeBasic,AIPlayMonteCarlo<3000,1,AIGameMemoryImproved,AIMemPerfectInfo,AIPlayScores<Cards_Basic, AIGameMemoryImproved> > > AIMCScores;
        typedef Player_AI<AIGameMemoryImproved,AITakeBasic,AIPlayMonteCarlo<100,30,AIGameMemoryImproved,AIMemPerfectInfo,AIPlayRandom<Cards_Basic, AIGameMemoryImproved> > > AIMCRandom;
        typedef Player_AI<AIGameMemoryImproved,AITakeBasic, AIPlayScores<Cards*, AIGameMemoryImproved> > AIScores;
        typedef Player_AI<AIGameMemoryImproved,AITakeBasic, AIPlayRandom<Cards*, AIGameMemoryImproved> > AIRandom;

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
        Uint _nbGame; //number of game played

        WrapperPrint<GAME_COINCHE_DEBUG> _printf;

    public:
        Game_Coinche(){}
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
