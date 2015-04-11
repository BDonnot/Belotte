#include "Trick_Basic.h"

using namespace::std;

Trick_Basic::Trick_Basic(Cards_Deck* pDeck,const std::array<Player*,4>& players):
_players(players),
//_currentTrick(),
_previousTrick(),
//_oldFirstPlayer(GHOST),
_pDeck(pDeck)
{
    _currentTrick = {nullptr,nullptr,nullptr,nullptr};
    _previousTrick = {nullptr,nullptr,nullptr,nullptr};
    //_posTrick = {FIRST,SECOND,THIRD,FOURTH};
}

Trick_Basic::~Trick_Basic()
{
    for(Uint i = 0;i < 4; ++i)
    {
        delete _currentTrick[i];
        delete _previousTrick[i];
    }
}

string Trick_Basic::GetString() const
{
    string res = "<trick : ";
    res += IntToString(_cardsPlayed);
    res += ">";
    Uint first = _oldFirstPlayer.ToInt();
    Uint counter = 0;
    for(Uint i = first; i < first+4; ++i, ++counter)
    {
        res += _players[i%4]->GetString(_currentTrick[counter]->GetString());
    }
    res += "<\\trick>";
    return res;
}

void Trick_Basic::WinnerTakeCards()
{
    for(Uint i = 0; i < 4; ++i)
    {
        if(_previousTrick[i] == nullptr) break; //TO DO : exception here !
        _pDeck->GetCardBack(_previousTrick[i]);
    }
    if(_trickNumber==8)
    {
        for(Uint i = 0; i < 4; ++i)
        {
            if(_currentTrick[i] == nullptr) break; //TO DO : exception here !
            _pDeck->GetCardBack(_currentTrick[i]);
        }
    }
    _previousTrick = _currentTrick;
    _currentTrick = {nullptr,nullptr,nullptr,nullptr};
    //printf("WinnerTakeCards for trick %d \n",_trickNumber);
}

bool Trick_Basic::Play() // TO DO code the same way as Bidding.cpp
{
    Uint i = _to_play.ToInt();
    trickFinished();
    //TO DO c'est moche !
    if (_cardsPlayed == 4 && _trickFinished) //the trick is over
    {
        trickOver();
        terminatecurrentTrick(i);
        return true;
    }
    Cards * pcard = _players[i]->PlayCard(static_cast<TrickBasic_Memory&>(*this));
    if(pcard == nullptr) { return false; }
    _currentTrick[_cardsPlayed] = pcard;
    playerPlayed();
    //graphics bellow
    animateIfGraphic(pcard,i);
    return false;
}

void Trick_Basic::terminatecurrentTrick(Uint first)
{
    for (Uint i = first,counter = 0; i < first+4; ++i, ++counter)
    {
        _players[i%4]->UpdateEndTrick(static_cast<const TrickBasic_Memory&>(*this),Position_Trick(counter));
    }
}

void Trick_Basic::SetPlayers(const array<Player*,4>& players)
{
    _players = players;
}
