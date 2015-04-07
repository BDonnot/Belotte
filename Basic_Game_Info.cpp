#include "Basic_Game_Info.h"
using namespace std;
unsigned long long Basic_Game_Info::_clock = 0;
PLAYER_ID Basic_Game_Info::_giver = PLAYER3;

PLAYER_ID Basic_Game_Info::_taker = GHOST;
Card_Color Basic_Game_Info::_trump_color = Card_Color(NOT_CHOSEN);
unsigned int Basic_Game_Info::_max_bid = MINBET;
Card_Color Basic_Game_Info::_color_proposed = Card_Color(NOT_CHOSEN);

PLAYER_ID Basic_Game_Info::_position_first_player = PLAYER0; //the first person how play the trick
unsigned int Basic_Game_Info::_number_card_played = 0; //cards played in the current trick

array<unsigned int,2> Basic_Game_Info::_final_scores = {0,0};
array<unsigned int,2> Basic_Game_Info::_running_scores = {0,0};
array<unsigned int,2> Basic_Game_Info::_tricks_win = {0,0};

Uint Basic_Game_Info::_windowsWidth = SCREEN_WIDTH;
Uint Basic_Game_Info::_windowsHeight = SCREEN_HEIGHT;
Basic_Game_Info::Basic_Game_Info()
{
    //ctor
}

Basic_Game_Info::~Basic_Game_Info()
{
    //dtor
}
void Basic_Game_Info::SetTaker(PLAYER_ID taker)
{
    _taker = taker;
}
PLAYER_ID Basic_Game_Info::Next(PLAYER_ID posPlayer) const
{
    switch(posPlayer)
    {
    case PLAYER0 :
        return PLAYER1;
    case PLAYER1 :
        return PLAYER2;
    case PLAYER2 :
        return PLAYER3;
    case PLAYER3 :
        return PLAYER0;
    default :
        return GHOST;
    }
}

void Basic_Game_Info::SetGiver() //the giver is always the person at the right
{
    _giver = Next(_giver);
    _position_first_player = Next(_giver);
}
void Basic_Game_Info::SetTrumpColor(const Card_Color& trump_color)
{
    _trump_color = trump_color;
}
PLAYER_ID Basic_Game_Info::posTrickToPlayer(PLAYER_ID firstToPlay,const Position_Trick& posTrick)
{
    if(posTrick == UNKNOWN) return GHOST;
    Uint trick = posTrick.ToInt();
    PLAYER_ID res = firstToPlay;
    for(Uint i = 0; i < trick; i++)
    {
        res = Next(res);
    }
    return res;
}
IntIntPair Basic_Game_Info::CalculateFinalScores() //compute who wins (returns the equip of the winner), determined each score, reset the other parameters
{
    //compute the scores
    IntIntPair res;
    unsigned int takerTeam = _taker % 2; //team of the taker
    unsigned int winner = takerTeam;
    res.first = _running_scores[takerTeam];
    if(_running_scores[takerTeam] > _running_scores[1-takerTeam] && _running_scores[takerTeam] > _max_bid) //the taker win
    {
        if (_tricks_win[takerTeam] == 8)
            _final_scores[takerTeam] += 250;
        else
        {
            _final_scores[takerTeam] += _max_bid;
        }
    }
    else
    {
        _final_scores[1-takerTeam] += 160;
        winner = 1-takerTeam;
    }
    res.second = winner;
    //reset the values
    _trump_color = NOT_CHOSEN;
    _taker = GHOST;
    _running_scores = {0,0};
    _tricks_win = {0,0};
    _max_bid = MINBET;
    return res;
}
void Basic_Game_Info::SetColorProposed(const Card_Color& color)
{
    _color_proposed = color;
}

void Basic_Game_Info::AddClock(unsigned long long diffTime)
{
    _clock += diffTime;
}
void Basic_Game_Info::SetMaxBid(unsigned int max_bid)
{
    _max_bid = max_bid;
}
void Basic_Game_Info::SetScores(PLAYER_ID trick_winner,const IntIntPair& scores)
{
    switch(trick_winner)
    {
    case PLAYER0 : case PLAYER2 :
        _running_scores[0] = scores.first;
        break;
    case PLAYER1: case PLAYER3 :
        _running_scores[1] = scores.second;
        break;
    default : //TO DO exception here
        break;
    }
}
