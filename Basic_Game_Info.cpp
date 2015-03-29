#include "Basic_Game_Info.h"
using namespace std;
unsigned long long Basic_Game_Info::_clock = 0;
PLAYER_ID Basic_Game_Info::_giver = PLAYER3;

PLAYER_ID Basic_Game_Info::_taker = GHOST;
CARDS_COLOR Basic_Game_Info::_trump_color = NOT_CHOSEN;
unsigned int Basic_Game_Info::_max_bid = MINBET;
CARDS_COLOR Basic_Game_Info::_color_proposed = NOT_CHOSEN;

PLAYER_ID Basic_Game_Info::_position_first_player = PLAYER0; //the first person how play the trick
unsigned int Basic_Game_Info::_number_card_played = 0; //cards played in the current trick

array<unsigned int,2> Basic_Game_Info::_final_scores = {0,0};
array<unsigned int,2> Basic_Game_Info::_running_scores = {0,0};
array<unsigned int,2> Basic_Game_Info::_tricks_win = {0,0};
array<CARDS_COLOR,4> Basic_Game_Info::_int_to_color = {DIAMOND,HEART,SPADE,CLUB};
array<CARDS_HEIGHT,8> Basic_Game_Info::_int_to_height = {SEVEN,EIGHT,NINE,TEN,JACK,QUEEN,KING,ACE};

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
void Basic_Game_Info::SetTrumpColor(const CARDS_COLOR trump_color)
{
    _trump_color = trump_color;
}
Uint Basic_Game_Info::posTrickToInt(POSITION_TRICK posTrick)
{
    switch(posTrick)
    {
    case FIRST :
        return 0;
    case SECOND :
        return 1;
    case THIRD :
        return 2;
    case FOURTH :
        return 3;
    default :
        return 4;
    }
}
PLAYER_ID Basic_Game_Info::posTrickToPlayer(PLAYER_ID firstToPlay,POSITION_TRICK posTrick)
{
    if(posTrick == UNKNOWN) return GHOST;
    Uint trick = posTrickToInt(posTrick);
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
    if(_running_scores[takerTeam] > _running_scores[1-takerTeam]) //the taker win
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
void Basic_Game_Info::SetColorProposed(CARDS_COLOR color)
{
    _color_proposed = color;
}
/*
void Basic_Game_Info::SetNumberCardsPlayed()
{
//    _number_card_played = 0;
//    if(update)
//        _index_strongest_card = _number_card_played;
    _number_card_played++;
//    _number_card_played %= 4;
//    _position_first_player++;
//    _position_next_player%=4;
}
*/

/*
void Basic_Game_Info::SetClock(unsigned long long clock)
{
    _clock = clock;
}
*/
void Basic_Game_Info::AddClock(unsigned long long diffTime)
{
    _clock += diffTime;
}
/*
void Basic_Game_Info::NextBiddingTurn()
{
    _biddingCount++;
}
*/
/*
void Basic_Game_Info::ResetBiddingCount()
{
    ///if(_biddingCount != 0) setGiver(); //TO DO improve the code to remove this ugly security... (this function is called twice instead of one at some point)
    ///_biddingCount = 0;
    setGiver();
}
void Basic_Game_Info::SetHigherCard(POSITION_TRICK winner)
{
    _index_strongest_card = winner;
}
*/

void Basic_Game_Info::SetMaxBid(unsigned int max_bid)
{
    _max_bid = max_bid;
}
const CARDS_COLOR Basic_Game_Info::ConvertIntToColor(int i)
{
    if( (i>=0)&&(i<4))
        return _int_to_color[i];
    return NO;
}
const CARDS_HEIGHT Basic_Game_Info::ConvertIntToHeight(int i)
{
    if( (i>=0)&&(i<8))
        return _int_to_height[i];
    return UNINTIALIZED;
}
//string Basic_Game_Info::IntToString(Uint number)
//{
//    ostringstream convert;
//    convert << number;
//    return convert.str();
//}
