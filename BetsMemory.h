#ifndef BETSMEMORY_H
#define BETSMEMORY_H

/**
** This class is updated by the bidding, and is used to store all the bets made by the 4 players.
** Only the 3 last bets made are available for the players.
**/

#include <list>
#include <string>

#include "Basic_Game_Info.h"
#include "Player_Bid.h"

class BetsMemory
{
    protected:
        Basic_Game_Info _infos;
        std::list<Player_Bid> _bets;
        const Player_Bid _defaultBid; //to return when there is nothing to return
        std::list<Player_Bid>::const_iterator _itSaved; //the iterator of the element that need to be stored
    public:
        BetsMemory():_itSaved(_bets.begin()){}
        virtual ~BetsMemory(){}
        void TellBet(Uint lastbiddingTurn,const Player_Bid& bid);
        void TellBet(const Player_Bid& bid);
        void ResetBets();
        const Player_Bid& AskBet() const; //return the bet made by the teammate, if any.
        const Uint GetSize() const;
        std::string GetString();
        bool IsBetsOver() const ;
    protected:
    private:
};

#endif // BETSMEMORY_H
