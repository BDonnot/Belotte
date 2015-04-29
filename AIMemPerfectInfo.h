#ifndef AIMEMPERFECTINFO_H
#define AIMEMPERFECTINFO_H

#include <array>

#include "Definitions.h"
#include "AIGameMemoryImproved.h"

template<class TypeOfCard>
class AIMemPerfectInfo : public AIGameMemoryImproved<TypeOfCard>
{
    public:
		AIMemPerfectInfo() :AIGameMemoryImproved<TypeOfCard>(Player_ID(GHOST), nullptr){}
		virtual ~AIMemPerfectInfo(){}
		void SetPlayer(const Player_ID& player, std::list<TypeOfCard>& hand);
		void SetGame(const std::array< std::pair<Player_ID, std::list<TypeOfCard> >, 4 >& allHands);

        //TO DO : deals with the player dependant methods
        //eg relative to :

    protected:
    private:
};

template<class TypeOfCard>
void AIMemPerfectInfo<TypeOfCard>::SetGame(const std::array< std::pair<Player_ID, std::list<TypeOfCard> >, 4 >& allHands)
{
	Player_ID tempP;
	Card_Color tempC;
	Card_Height tempH;
	//tell everyone that they cannot have this cards, for each cards
	for (Uint iPlayer = 0; iPlayer < 4; ++iPlayer)
	{
		tempP = Player_ID(iPlayer);
		for (Uint icol = 0; icol < 4; ++icol)
		{
			tempC = Card_Color(icol);
			for (Uint iH = 0; iH < 8; ++iH)
			{
				tempH = Card_Height(iH);
				this->_canPlayersHaveCard.SetCannotHaveCard(tempP, tempC, tempH);
			}
		}
	}
	//tell them they can have the cards they have been given
	for (Uint iPair = 0; iPair < 4; ++iPair)
	{
		const std::pair<Player_ID, std::list<TypeOfCard> > & tempPair = allHands[iPair];
		tempP = tempPair.first;
		for (const Cards_Basic& pcard : tempPair.second)
		{
			this->_canPlayersHaveCard.SetCanHaveCard(tempP, pcard.GetColour(), pcard.GetHeight());
		}
	}
}

template<class TypeOfCard>
void AIMemPerfectInfo<TypeOfCard>::SetPlayer(const Player_ID& player, std::list<TypeOfCard>& hand)
{
	this->_posPlayer = player;
	this->_pHand = &hand;
	//COMPUTE THE THINGS RELATIVE TO THE PLAYER (DEPENDENT OF ITS HAND)
	this->updatePlayerRelativeAttributes();
}
#endif // AIMEMPERFECTINFO_H
