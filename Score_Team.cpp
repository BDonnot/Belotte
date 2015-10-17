#include "Score_Team.h"
using namespace::std;
using namespace SDL;

Score_Team::Score_Team(SDL_Surface* pScreen):
_info()
,_pScreen(pScreen)
,_scoreFirstTeam("fonts/MLSGY.ttf",30,255,255,255 ,"Equipe 1 : 0")
,_scoreSecondTeam("fonts/MLSGY.ttf",30,255,255,255,"Equipe 2 : 0")
{
    PositionGraphic pos;
    pos.Set(_info.WindowsWidth()-30,15,TOP_RIGHT);
    _scoreFirstTeam.SetPosition(pos);
    pos.Set(_info.WindowsWidth()-30,60,TOP_RIGHT);
    _scoreSecondTeam.SetPosition(pos);
}

void Score_Team::Display(GAME_PHASES currentPhase)
{
    if ((currentPhase != PLAYING)&&(currentPhase != AFTER_TRICK0)&&(currentPhase != AFTER_TRICK1)) return;
    _scoreFirstTeam.Display(_pScreen);
    _scoreSecondTeam.Display(_pScreen);
}
void Score_Team::Update()
{
    const array<Uint,2>& scores = _info.RunningScores();
    //first team
    string newStr = "Equipe ";
    newStr += " 1 : ";
    newStr += IntToString(scores[0]);
    _scoreFirstTeam.ChangeText(newStr);

    //Second team
    newStr = "Equipe ";
    newStr += " 2 : ";
    newStr += IntToString(scores[1]);
    _scoreSecondTeam.ChangeText(newStr);
}
