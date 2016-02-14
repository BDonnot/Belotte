#ifndef END_OF_GAME_H
#define END_OF_GAME_H

//#include<memory>

#include "Definitions.h"
#include "Basic_Game_Info.h"
#include "Images_Text.h"
#include "Images_Button.h"

/**
** This class aims to easy the display of the last part of the game : who win, the current scores and
** other interesting informations.
**/
class End_Of_Game
{
    protected :
       Basic_Game_Info _info;
#if COMPLETE_GAME > 0
       SDL_Surface* _pScreen;
#endif //COMPLETE_GAME > 0
	   SDL::Images_Button _again;
	   SDL::Images_Button _quit;
	   SDL::Images_Text _firstLine;
	   SDL::Images_Text _secondLine;
	   SDL::Images_Text _firstScore;
	   SDL::Images_Text _secondScore;
    public:
        End_Of_Game(){}
#if COMPLETE_GAME > 0
        End_Of_Game(SDL_Event* pEvent,SDL_Surface* pScreen,Uint windowsWidth, Uint windowsHeight);
#else
		End_Of_Game(Uint windowsWidth, Uint windowsHeight);
#endif //COMPLETE_GAME > 0
        virtual ~End_Of_Game(){}
        void Display(GAME_PHASES currentPhase);
        void Update(); //update the scores and the texts.
        void UpdateEvent(GAME_PHASES currentPhase);
        GAME_PHASES Next();
    protected:
    private:
};

#endif // END_OF_GAME_H
