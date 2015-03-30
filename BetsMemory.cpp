#include "BetsMemory.h"
void BetsMemory::TellBet(const Player_Bid& bid)
{
    _bets.push_back(bid); //here
    if(_bets.size() == 1)
    {
        ++_itSaved; //for this iterator to point to the first element.
    }
}

void BetsMemory::ResetBets()
{
    _bets.clear();
    _itSaved = _bets.begin();
}

const Player_Bid& BetsMemory::AskBet() const
{
    if(_bets.size() < 2 ) return _defaultBid;
    auto itRes = _bets.rbegin();
    ++itRes;
    return *itRes;
}

std::string BetsMemory::GetString()
{
    if(_bets.size() == 0) return ""; //TO DO Exception here !
    if(_itSaved == _bets.end()) return ""; //TO DO probably exception here too !
    std::string res = (*_itSaved).GetString();
    ++_itSaved;
    return res;
}

const Uint BetsMemory::GetSize() const
{
    return _bets.size();
}

bool BetsMemory::IsBetsOver(PLAYER_ID posPlayer) const
{
    if (_bets.size() <= 3) return false;
    //if ((_infos.BiddingRound()==0)&&(posPlayer != _infos.Giver())) return false;
    //PLAYER_ID next = _infos.Next(posPlayer);
    bool res = true;
    auto itBet = _bets.rbegin();
    for(Uint i = 0; i < 3; i++)
    {
        res &= (*itBet).GetColorBid()==NO;
        //next = _infos.Next(next);
        ++itBet;
    }
    return res;
}
