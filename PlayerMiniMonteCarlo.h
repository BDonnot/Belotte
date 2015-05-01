#ifndef PLAYERMINIMONTECARLO_H
#define PLAYERMINIMONTECARLO_H

/**
** This class represent "Mini Players".
** A "Mini Player" is a player of the game
** which only enters in action in AIPlayMonteCarlo and MCPlayGames
** and has only the basis of a player.
** It is a structure to store the hand,
** and to store efficiently which player can receive which cards
**/

#include <list>
#include<algorithm>
#include<unordered_map>

#include "Definitions.h"

#include "Foncteurs_Players.h"

#include "Cards_Basic.h"
#include "Cards.h"
#include "Random.h"
#include "TrickStatus.h"

#include "BasicMonteCarloAction.h"
#include "MonteCarloAction.h"

//Define the variable for debugging purpose
#include "DebugwithPrint.h"
#define PLAYERMINIMONTECARLO_DEBUG 0

///Code Consistency :
///TO DO : some method are copy/paste from player, this should not be

class PlayerMiniMonteCarlo
{
    protected:
        Player_ID _number;
        Basic_Game_Info _basic_info;
        std::list<Cards_Basic> _hand;
        Uint _nbCardToReceive;
        std::unordered_map<Cards_Basic,bool> _canReceiveCard;
        Random *_rand;
        TrickStatus _status;
        std::list< Cards_Basic > _playableCards;
        CanPlayCards<Cards_Basic> _fCanPlayCard;
        WrapperPrint<PLAYERMINIMONTECARLO_DEBUG> _printf;

    public:
        PlayerMiniMonteCarlo():_number(GHOST),_status(_number){}
        PlayerMiniMonteCarlo(const Player_ID& number):_number(number),_status(_number){}
        virtual ~PlayerMiniMonteCarlo(){}
        void ReceiveInitInfo(Random& initRandomState);

		template<template<class> class GameMemory>
		void ReceiveCardsInfo(Uint nbCardToReceive, const std::list<Cards_Basic>& cardsToBeGiven, const GameMemory<Cards*>& initMemory);

        void ReceiveHand(const std::list<Cards*>& initHand);
        void ReceiveCard(const Cards_Basic& card); //true if the cards is put, false otherwise
        bool CanReceiveCard(const Cards_Basic& card) const;
        bool CanReceiveAnotherCard() const;
        const Player_ID& ID() const {return _number;}
        const Uint NumberCardToReceive() const {return _nbCardToReceive;}
        const Uint CardInHand() {return _hand.size();}
        std::list<Cards_Basic> & GetHand() {return _hand;}
        std::list< Cards_Basic> & GetPlayableCard(const TrickBasic_Memory& trick)
        {
			updatePlayableCard(trick);
            return _playableCards;
        }

        void RetrieveCard(const Cards_Basic& card); //retrieve the card from the hand
        void RetrieveCard(std::list<Cards_Basic>::iterator& card); //retrieve the card from the hand
        Cards_Basic Play();

        void AddConstraint(const Cards_Basic& card);
        void RemoveConstraint(const Cards_Basic& card);

        void PrintHand() const;

    protected:
		void updatePlayableCard(const TrickBasic_Memory& trick); //TO DO : find another way, this is a copy/paste from Player...
        bool has_colour(const Card_Color& colour); //TO DO : find another way, this is a copy/paste from Player...

    private:
};

template<template<class> class GameMemory>
void PlayerMiniMonteCarlo::ReceiveCardsInfo(Uint nbCardToReceive,
	const std::list<Cards_Basic>& cardsToBeGiven,
	const GameMemory<Cards*>& initMemory)

{
	_hand.clear();
	_nbCardToReceive = nbCardToReceive;
	for (const auto card : cardsToBeGiven)
	{
		_canReceiveCard[card] = initMemory.CanReceiveCard(_number, card.GetColour(), card.GetHeight());
	}
}
#endif // PLAYERMINIMONTECARLO_H
