#include "End_Of_Game.h"
using namespace std;
End_Of_Game::End_Of_Game(SDL_Event* pEvent,SDL_Surface* pScreen,Uint windowsWidth, Uint windowsHeight):
_info()
,_pScreen(pScreen)
,_again(pEvent,"Continuer",move(SDL::PositionGraphic()))
,_quit(pEvent,"Quitter",move(SDL::PositionGraphic()))
,_firstLine("fonts/MLSGY.ttf",25,255,255,255,"L'équipe 1 (attaquante) a gagné avec 123 points.")
, _secondLine("fonts/MLSGY.ttf", 25, 255, 255, 255, "Le contrat est donc rempli, et les scores sont donc :")
, _firstScore("fonts/MLSGY.ttf", 25, 255, 255, 255, "Equipe 0 : 445 points")
, _secondScore("fonts/MLSGY.ttf", 25, 255, 255, 255, "Equipe 1 : 445 points")
{
    SDL::PositionGraphic pos(windowsWidth/2,windowsHeight/9-20,CENTER);
    _again.SetPosition(pos);
    pos.Set(windowsWidth/2,2*windowsHeight/9-20,CENTER);
    _quit.SetPosition(pos);
    pos.Set(windowsWidth/2,3*windowsHeight/9-20,CENTER);
    _firstLine.SetPosition(pos);
    pos.Set(windowsWidth/2,4*windowsHeight/9-20,CENTER);
    _secondLine.SetPosition(pos);
    pos.Set(windowsWidth/2,5*windowsHeight/9-20,CENTER);
    _firstScore.SetPosition(pos);
    pos.Set(windowsWidth/2,6*windowsHeight/9-20,CENTER);
    _secondScore.SetPosition(pos);
    pos.Set(windowsWidth/2+100,7*windowsHeight/9-20,CENTER);
    _again.SetPosition(pos);
    pos.Set(windowsWidth/2-100,7*windowsHeight/9-20,CENTER);
    _quit.SetPosition(pos);
}

void End_Of_Game::Display(GAME_PHASES currentPhase)
{
    if(currentPhase != SCORES) return;
    _firstLine.Display(_pScreen);
    _secondLine.Display(_pScreen);
    _firstScore.Display(_pScreen);
    _secondScore.Display(_pScreen);
    _again.Display(_pScreen);
    _quit.Display(_pScreen);
}

void End_Of_Game::UpdateEvent(GAME_PHASES currentPhase)
{
    if(currentPhase != SCORES) return;
    _again.Update_on_it();
    _quit.Update_on_it();
}

void End_Of_Game::Update()
{
    Uint taker = _info.Taker().ToInt();
    Uint bid = _info.MaxBid();
    IntIntPair scoreWinner = _info.CalculateFinalScores();
    bool winnerWon = scoreWinner.second == taker;
    string text1 = "";
    string text2 = "";
    if(winnerWon)
    {
        text1 = "L'attaque a remporte son contrat de ";
        text1 += IntToString(bid);
        text1 += " en ayant marque ";
        text1 += IntToString(scoreWinner.first);
        text1 += " points.";
        text2 = "Le contrat est donc rempli, et les scores sont donc de :";
    }
    else
    {
        text1 = "L'attaque a perdu son contrat de ";
        text1 += IntToString(bid);
        text1 += " en ayant marque ";
        text1 += IntToString(scoreWinner.first);
        text1 += " points.";
        text2 = "Le contrat est donc chuté, et les scores sont donc de :";
    }
    _firstLine.ChangeText(text1);
    _secondLine.ChangeText(text2);
    const array<Uint,2>& scores(_info.FinalScores());
    text1 = "Equipe 1 : ";
    text1 += IntToString(scores[0]);
    text2 = "Equipe 2 : ";
    text2 += IntToString(scores[1]);
    _firstScore.ChangeText(text1);
    _secondScore.ChangeText(text2);
}

GAME_PHASES End_Of_Game::Next()
{
#if PLAY_HUMAN > 0
    if(_again.Click(true)) return GIVING;
    if(_quit.Click(true)) return EXIT;
    return SCORES;
#else
    return GIVING;
#endif
}
