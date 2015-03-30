#include "TrickBasic_Memory.h"
using namespace std;

TrickBasic_Memory::TrickBasic_Memory()
{
    Reset();
}

TrickBasic_Memory::~TrickBasic_Memory()
{
    //dtor
}

void TrickBasic_Memory::Reset()
{
    _trickNumber = 0;
    _cardsPlayed = 0;
    _to_play = _infos.Next(_infos.Giver());
    _oldFirstPlayer = GHOST;
    _winner = UNKNOWN;
    _scores.first = 0;
    _scores.second = 0;
    _colorAsked = NOT_CHOSEN;
    _colorMaster = NOT_CHOSEN;
    _heightMaster = UNINTIALIZED;
    _trickFinished = false;
}

void TrickBasic_Memory::updateWinner()
{
    if(_cardsPlayed == 0)
    {
        _colorAsked = _currentTrick[0]->GetColour();
        _colorMaster = _colorAsked;
        _heightMaster = _currentTrick[0]->GetHeight();
        _winner = FIRST;
        return ;
    }
    CARDS_COLOR colorTrump = _infos.TrumpColor();
    CARDS_COLOR color = _currentTrick[_cardsPlayed]->GetColour();
    //CARDS_HEIGHT height = _currentTrick[_cardsPlayed]->GetHeight();

    bool needUpdate = false; //do the last cards played is master
    if(color == colorTrump)
    {
        if(_colorMaster == colorTrump) needUpdate = _currentTrick[_cardsPlayed]->Win(_heightMaster);
        else needUpdate = true;
    }
    else
    {
        if(_colorMaster == colorTrump) needUpdate = false;
        else
        {
            if(color != _colorAsked) needUpdate = false;
            else needUpdate = _currentTrick[_cardsPlayed]->Win(_heightMaster);
        }
    }
    if(needUpdate)
    {
        _winner = _infos.IntToPosTrick(_cardsPlayed-1);
        _colorMaster = _currentTrick[_cardsPlayed]->GetColour();
        _heightMaster = _currentTrick[_cardsPlayed]->GetHeight();
        _winner = _infos.IntToPosTrick(_cardsPlayed);
    }
}

void TrickBasic_Memory::finishTrick()
{
    //printf("le pli %d est fini\n",_trickNumber);
    Uint pointsInTheTrick = 0;
    for(auto pcards : _currentTrick) { pointsInTheTrick += pcards->Value(); }
    if(_trickNumber == 8) pointsInTheTrick += 10;
    updateToPlay();
    updateScores(pointsInTheTrick);
}
void TrickBasic_Memory::updateToPlay()
{
    _oldFirstPlayer = _to_play;
    switch(_winner)
    {
    case FIRST :
        break;
    case SECOND :
        _to_play = _infos.Next(_to_play);
        break;
    case THIRD :
        _to_play = _infos.Next(_infos.Next(_to_play));
        break;
    case FOURTH :
        _to_play = _infos.Next(_infos.Next(_infos.Next(_to_play)));
        break;
    default : //TO DO exception here
        _to_play = GHOST;
        break;
    }
}

void TrickBasic_Memory::updateScores(Uint pointsInTheTrick)
{
    switch(_to_play)
    {
    case PLAYER0 : case PLAYER2 :
        _scores.first += pointsInTheTrick;
        break;
    case PLAYER1 : case PLAYER3 :
        _scores.second += pointsInTheTrick;
        break;
    default : //TO DO exception here
        break;
    }
    _infos.SetScores(_to_play,_scores);
}

void TrickBasic_Memory::trickOver()
{
    //if(_cardsPlayed == 4 && _trickFinished) //The trick is over
    //{
        //printf("pli %d fini\n",_trickNumber);
        _trickFinished = false;
        _cardsPlayed = 0;
        _trickNumber++;
        finishTrick();
        _winner = UNKNOWN;
        _oldFirstPlayer = GHOST;
    //}
}
void TrickBasic_Memory::playerPlayed()
{
    updateWinner();
    _to_play = _infos.Next(_to_play);
    _cardsPlayed++;
}
