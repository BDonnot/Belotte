#ifndef SCORE_TEAM_H
#define SCORE_TEAM_H

/**
** This class exist to display the score of each team during a game (8 tricks).
** 2 texts for 2 scores ...
**/

#include<string>

#include "Definitions.h"
#include "Basic_Game_Info.h"
#include "Images_Text.h"

class Score_Team
{
    protected :
        Basic_Game_Info _info;
#if COMPLETE_GAME > 0
        SDL_Surface* _pScreen;
#endif //#if COMPLETE_GAME > 0
		Images_Text _scoreFirstTeam;
		Images_Text _scoreSecondTeam;
    public:
        Score_Team(){}
#if COMPLETE_GAME > 0
        Score_Team(SDL_Surface* pScreen);
#endif //#if COMPLETE_GAME > 0
        virtual ~Score_Team(){}
        void Display(GAME_PHASES currentPhase);
        void Update();
    protected:
    private:
};

#endif // SCORE_TEAM_H
