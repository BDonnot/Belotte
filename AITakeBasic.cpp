#include "AITakeBasic.h"

void AITakeBasic::Bid(Player_Bid& bid,const std::list<Cards*>& hand,Random& rand,const BetsMemory& bets)
{
    if(_alreadyBid)
    {
        bid.Bid(NO,_basic_info.MaxBid()+10);
        _printf("I do not want to bet\n");
        return;
    }
    _alreadyBid = true;
    //if I am the first player of my team to choose :
    const Player_Bid& bidTeammate = bets.AskBet();
    _printf("I will probably bet\n");
    if(bidTeammate.Color() == NOT_CHOSEN)
        return iChoose(bid,hand);
    return iFollow(bid,hand,bidTeammate);
}

void AITakeBasic::iChoose(Player_Bid& bid,const std::list<Cards*>& hand)
{
    Uint maxBet = _basic_info.MaxBid();

    Card_Color color(NOT_CHOSEN);
    Uint high = 0;
    Uint counter = 0;

    Card_Color colorBet(NOT_CHOSEN);
    Uint highBet = maxBet;
    const Card_Color notChosen(NOT_CHOSEN);
    for(auto pcard : hand)
    {
        if(pcard->GetHeight() == JACK)
        {
            color = pcard->GetColour();
            counter = 0;
            high = 80;
            for(auto pcard2 : hand)
            {
                if(pcard2->GetColour() == color )
                {
                    counter++;
                    if((pcard2->GetHeight() == NINE)) high = 90;
                }
            }
            if((counter >= 3 )&&(high > highBet))
            {
                colorBet = Card_Color(color);
                highBet = high;
            }
            else
            {
                color = notChosen;
                high = 0;
            }
        }
    }
    if(colorBet != notChosen) bid.Bid(colorBet,highBet);
    else bid.Bid(Card_Color(NO),maxBet+10);
    _printf("My bet is : %d, c: %d\n",bid.Bid(),bid.Color().ToInt());
}

void AITakeBasic::iFollow(Player_Bid& bid,const std::list<Cards*>& hand,const Player_Bid& bidTeammate)
{
    Uint myBet = bidTeammate.Bid();
    Card_Color teammateColor = bidTeammate.Color();
    Uint maxBet = _basic_info.MaxBid();
    for(auto pcard : hand)
    {
        if((pcard->GetHeight() == ACE)&&(pcard->GetColour()!=teammateColor)) myBet+=7;
        else if((pcard->GetHeight() == NINE)&&(pcard->GetColour() == teammateColor)) myBet+=10;
    }
    if(myBet > maxBet) bid.Bid(teammateColor,(myBet-myBet%10) );
    else bid.Bid(NO,maxBet+10);
}
