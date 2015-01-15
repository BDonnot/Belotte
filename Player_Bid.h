#ifndef PLAYER_BID_H
#define PLAYER_BID_H

#include "Definitions.h"
#include "Basic_Game_Info.h"

/**
The only aim of this class is to be able to
return a convenient object during the bidding time.
It is also an visual element.
**/
class Player_Bid
{
    protected :
        CARDS_COLOR _color_bid;
        Uint _bid;
        Basic_Game_Info _info;

    public:
        Player_Bid();
        //Player_Bid(const Player_Bid& rhs);
        virtual ~Player_Bid();

        const CARDS_COLOR Color() const;
        const Uint Bid() const;
        const CARDS_COLOR& GetColorBid() const;

        bool Bid(CARDS_COLOR color,Uint bid); //true (and set whatever needs to be) if the bid can be done, false otherwise
        bool Bid(Uint bid);
        bool Bid(CARDS_COLOR color);
        //void BidNum(Uint bid);
        void Reset();

        std::string GetString() const;
    protected:
        virtual void handleGraphicsIfAny();
    private:
};

#endif // PLAYER_BID_H
