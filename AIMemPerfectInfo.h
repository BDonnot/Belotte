#ifndef AIMEMPERFECTINFO_H
#define AIMEMPERFECTINFO_H

/**
** This class inherit from AIGameMemoryImproved.
** It is some king a specification of this previous methods,
** in the case of the cards of everyone is known by everyone.
** This is currently use in MCPlayGames.
**/

#include <array>

#include "Definitions.h"
#include "AIGameMemoryImproved.h"

//Debugging :
#include "DebugwithPrint.h"
#define AIMEMPERFECTINFO 0


template<class TypeOfCard>
class AIMemPerfectInfo : public AIGameMemoryImproved<TypeOfCard>
{
    protected :
        WrapperPrint<AIMEMPERFECTINFO> _printf;
    public:
		AIMemPerfectInfo() :AIGameMemoryImproved<TypeOfCard>(Player_ID(GHOST), nullptr){}
		virtual ~AIMemPerfectInfo(){}
		void SetPlayer(const Player_ID& player, std::list<TypeOfCard>& hand);
		void SetGame(const std::array< std::pair<Player_ID, std::list<TypeOfCard> >, 4 >& allHands);

    protected:
    private:
};

template<class TypeOfCard>
void AIMemPerfectInfo<TypeOfCard>::SetGame(const std::array< std::pair<Player_ID, std::list<TypeOfCard> >, 4 >& allHands)
{
	Player_ID tempP;
	Card_Color tempC;
	Card_Height tempH;
	_printf("I set the game of all players\n");
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
		_printf("The hand for player %d is :",tempP.ToInt());
		for (const Cards_Basic& pcard : tempPair.second)
		{
            _printf("[c:%d;h:%d] ; ",pcard.GetHeight().ToInt(),pcard.GetColour().ToInt());
			this->_canPlayersHaveCard.SetCanHaveCard(tempP, pcard.GetColour(), pcard.GetHeight());
		}
		_printf("\n");
	}
}

template<class TypeOfCard>
void AIMemPerfectInfo<TypeOfCard>::SetPlayer(const Player_ID& player, std::list<TypeOfCard>& hand)
{
	this->_posPlayer = player;
	this->_pHand = &hand;
	_printf("I set the memory for player %d\n",player.ToInt());
	//COMPUTE THE THINGS RELATIVE TO THE PLAYER (DEPENDENT OF ITS HAND)
	this->updatePlayerRelativeAttributes();
}
#endif // AIMEMPERFECTINFO_H
