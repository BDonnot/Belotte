#ifndef SCORE_TEAM_H
#define SCORE_TEAM_H

#include<string>

#include "Definitions.h"
#include "Basic_Game_Info.h"
#include "Images_Text.h"
/**
This class exist to display the score of each team during a game (8 tricks).
**/
class Score_Team
{
    protected :
        Basic_Game_Info _info;
        SDL_Surface* _pScreen;
        Images_Text _scoreFirstTeam;
        Images_Text _scoreSecondTeam;
    public:
        Score_Team();
        Score_Team(SDL_Surface* pScreen);
        virtual ~Score_Team();
        void Display(GAME_PHASES currentPhase);
        void Update();
    protected:
    private:
};

#endif // SCORE_TEAM_H
