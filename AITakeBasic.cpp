#include "AITakeBasic.h"
/*
AITakeBasic::AITakeBasic()
{
    //ctor
}

AITakeBasic::~AITakeBasic()
{
    //dtor
}
*/
void AITakeBasic::Bid(Player_Bid& bid,const std::list<Cards*>& hand,Random& rand,const BetsMemory& bets)
{
    if(_alreadyBid)
    {
        bid.Bid(NO,_basic_info.MaxBid()+10);
        return;
    }
    _alreadyBid = true;
    //if I am the first player of my team to choose :
    const Player_Bid& bidTeammate = bets.AskBet();
    if(bidTeammate.Color() == NOT_CHOSEN)
        return iChoose(bid,hand);
    return iFollow(bid,hand,bidTeammate);
    /*
    if(_basic_info.MaxBid() <= 80)
        bid.Bid(_basic_info.ConvertIntToColor(rand.generate_number()%4),_basic_info.MaxBid()+10);
    else
        bid.Bid(NO,_basic_info.MaxBid()+10);
    */

}

void AITakeBasic::iChoose(Player_Bid& bid,const std::list<Cards*>& hand)
{
    Uint maxBet = _basic_info.MaxBid();

    CARDS_COLOR color = NOT_CHOSEN;
    Uint high = 0;
    Uint counter = 0;

    CARDS_COLOR colorBet = NOT_CHOSEN;
    Uint highBet = maxBet;

    for(auto it = hand.begin(); it != hand.end(); ++it)
    {
        if((*it)->GetHeight() == JACK)
        {
            color = (*it)->GetColour();
            counter = 0;
            high = 80;
            for(auto it2 = hand.begin(); it2 != hand.end(); ++it2)
            {
                if((*it2)->GetColour() == color )
                {
                    counter++;
                    if(((*it2)->GetHeight() == NINE)) high = 90;
                }
            }
            if((counter >= 3 )&&(high > highBet))
            {
                colorBet = color;
                highBet = high;
            }
            else
            {
                color = NOT_CHOSEN;
                high = 0;
            }
        }
    }
    if(colorBet !=NOT_CHOSEN) bid.Bid(colorBet,highBet);
    else bid.Bid(NO,maxBet+10);

/*
    if(maxBet <= 80)
        bid.Bid(_basic_info.ConvertIntToColor(0),maxBet+10);
    else
        bid.Bid(NO,maxBet+10);
*/
}

void AITakeBasic::iFollow(Player_Bid& bid,const std::list<Cards*>& hand,const Player_Bid& bidTeammate)
{
    Uint myBet = bidTeammate.Bid();
    CARDS_COLOR teammateColor = bidTeammate.GetColorBid();
    Uint maxBet = _basic_info.MaxBid();
    for(auto it = hand.begin(); it != hand.end(); ++it)
    {
        if(((*it)->GetHeight() == ACE)&&((*it)->GetColour()!=teammateColor)) myBet+=10;
    }
    if(myBet > maxBet) bid.Bid(teammateColor,myBet);
    else bid.Bid(NO,maxBet+10);
}
