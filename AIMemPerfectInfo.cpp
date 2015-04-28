#include "AIMemPerfectInfo.h"

/*
void AIMemPerfectInfo::SetGame(const std::array< std::pair<Player_ID,std::list<Cards_Basic> > ,4 >& allHands )
{
    Player_ID tempP;
    Card_Color tempC;
    Card_Height tempH;
    //tell everyone that they cannot have this cards, for each cards
    for(Uint iPlayer = 0; iPlayer < 4; ++iPlayer)
    {
        tempP = Player_ID(iPlayer);
        for(Uint icol = 0; icol < 4; ++icol)
        {
            tempC = Card_Color(icol);
            for(Uint iH = 0; iH < 8; ++iH)
            {
                tempH = Card_Height(iH);
                _canPlayersHaveCard.SetCannotHaveCard(tempP,tempC,tempH);
            }
        }
    }
    //tell them they can have the cards they have been given
    for(Uint iPair = 0; iPair < 4; ++iPair)
    {
        const std::pair<Player_ID,std::list<Cards_Basic> > & tempPair = allHands[iPair];
        tempP = tempPair.first;
        for(const Cards_Basic& pcard : tempPair.second)
        {
             _canPlayersHaveCard.SetCanHaveCard(tempP,pcard.GetColour(),pcard.GetHeight());
        }
    }
}

void AIMemPerfectInfo::SetPlayer(const Player_ID& player,const std::list<Cards_Basic> * hand)
{
	_posPlayer = player;
	_pHand = hand;
	//COMPUTE THE THINGS RELATIVE TO THE PLAYER (DEPENDENT OF ITS HAND)
	updatePlayerRelativeAttributes();
}
*/