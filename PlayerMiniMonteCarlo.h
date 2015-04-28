#ifndef PLAYERMINIMONTECARLO_H
#define PLAYERMINIMONTECARLO_H

#include <list>
#include<algorithm>
#include<unordered_map>

#include "Definitions.h"

#include "Foncteurs_Players.h"

#include "Cards_Basic.h"
#include "Random.h"
#include "TrickStatus.h"

#include "BasicMonteCarloAction.h"
#include "MonteCarloAction.h"
//TO DO : make a basic class from which this one and Player will inherit...

//play : AIPlayRandom or AIPlayScore
template<class GameMemory>
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

    public:
        PlayerMiniMonteCarlo():_number(GHOST),_status(_number){}
        PlayerMiniMonteCarlo(const Player_ID& number):_number(number),_status(_number){}
        virtual ~PlayerMiniMonteCarlo(){}
        void ReceiveInitInfo(Random& initRandomState);
        void ReceiveCardsInfo(Uint nbCardToReceive,const std::list<Cards_Basic>& cardsToBeGiven,const GameMemory& initMemory);
        void ReceiveHand(const std::list<Cards*>& initHand);
        void ReceiveCard(const Cards_Basic& card); //true if the cards is put, false otherwise
        bool CanReceiveCard(const Cards_Basic& card) const;
        bool CanReceiveAnotherCard() const;
        const Player_ID& ID() const {return _number;}
        const Uint NumberCardToReceive() const {return _nbCardToReceive;}
        const Uint CardInHand() const {return _hand.size();}
        const std::list<Cards_Basic>& GetHand() const {return _hand;}
        std::list< Cards_Basic>& GetPlayableCard(const TrickBasic_Memory& trick)
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

template<class GameMemory>
void PlayerMiniMonteCarlo<GameMemory>::ReceiveInitInfo(Random& initRandomState)
{
    _rand = &initRandomState;
}

template<class GameMemory>
void PlayerMiniMonteCarlo<GameMemory>::ReceiveCardsInfo(Uint nbCardToReceive,
                                                       const std::list<Cards_Basic>& cardsToBeGiven,
                                                       const GameMemory& initMemory)

{
    _hand.clear();
    _nbCardToReceive = nbCardToReceive;
    for(const auto card : cardsToBeGiven)
    {
        _canReceiveCard[card] = !initMemory.Cut(_number,card.GetColour());
    }
}

template<class GameMemory>
void PlayerMiniMonteCarlo<GameMemory>::AddConstraint(const Cards_Basic& card)
{
    _canReceiveCard[card] = false;
    //printf("I have added the constraint in player\n");
}

template<class GameMemory>
void PlayerMiniMonteCarlo<GameMemory>::RemoveConstraint(const Cards_Basic& card)
{
    //TO DO : maybe remember the initial state : we don't want a constraint to be added illegally
    _canReceiveCard[card] = true;
}

template<class GameMemory>
void PlayerMiniMonteCarlo<GameMemory>::ReceiveHand(const std::list<Cards*>& initHand)
{
    _hand.clear();
    for(auto pCard : initHand)
    {
        _hand.push_back(static_cast<Cards_Basic>(*pCard));
    }
    _nbCardToReceive = 0;

}

template<class GameMemory>
void PlayerMiniMonteCarlo<GameMemory>::ReceiveCard(const Cards_Basic& card)
{
    _hand.push_back(card);
    _nbCardToReceive--;
}

template<class GameMemory>
bool PlayerMiniMonteCarlo<GameMemory>::CanReceiveAnotherCard() const
{
    return _nbCardToReceive > 0;
}

template<class GameMemory>
bool PlayerMiniMonteCarlo<GameMemory>::CanReceiveCard(const Cards_Basic& card) const
{
    bool ok = _canReceiveCard.at(card);
    return ok && _nbCardToReceive > 0;
}

template<class GameMemory>
void PlayerMiniMonteCarlo<GameMemory>::RetrieveCard(const Cards_Basic& card)
{
    auto it = _hand.begin();
    const Card_Height height= card.GetHeight();
    const Card_Color color= card.GetColour();
    //printf("I mus retrieve [c:%d,h:%d]\n",color.ToInt(),height.ToInt());
    //printf("I have in Hand :");
    for(auto itC = _hand.begin(); itC != _hand.end(); ++itC)
    {
        //printf("[c:%d,h:%d];",itC->GetColour().ToInt(),itC->GetHeight().ToInt());
        if(itC->GetColour() == color && itC->GetHeight() ==  height)
        {
            it = itC;
        }
    }
    //printf("\n");
    _nbCardToReceive++;
	it = _hand.erase(it);
    //_hand.remove(card);
}
template<class GameMemory>
void PlayerMiniMonteCarlo<GameMemory>::RetrieveCard(std::list<Cards_Basic>::iterator& itCard) //retrieve the card from the hand
{
	itCard = _hand.erase(itCard);
}

template<class GameMemory>
void PlayerMiniMonteCarlo<GameMemory>::PrintHand() const
{
    printf("Mini player %d have :\n",_number.ToInt());
    for(auto card : _hand)
    {
        printf("[c:%d,h:%d] ,",card.GetColour().ToInt(),card.GetHeight().ToInt());
    }
    printf("\n");
}

template<class GameMemory> //TO DO : this is a copy/paste from player, there may be another option...
void PlayerMiniMonteCarlo<GameMemory>::updatePlayableCard(const TrickBasic_Memory& trick)
{
    //auto itEnd = _hand.end();
    _playableCards.clear();
    if (trick.NumberCardsPlayed() == 0) //if we are the first to play, we can play everything
    {
        for(auto pcard : _hand)
        {
			_playableCards.push_back(pcard);
        }
        return;
    }

    _status.Update(trick
                   ,has_colour(trick.ColorAsked())
                   ,has_colour(_basic_info.TrumpColor()));
    _fCanPlayCard.Init(trick,_status,_hand);
	for (auto pcard : _hand)
    {
        if( _fCanPlayCard(pcard) ) _playableCards.push_back(pcard);
    }
}

template<class GameMemory> //TO DO : this is a copy/paste from player, there may be another option...
bool PlayerMiniMonteCarlo<GameMemory>::has_colour(const Card_Color& colour) //do I have the color
{
    for (auto pcard : _hand)
    {
        if (pcard.GetColour() == colour)
            return true;
    }
    return false;
}
#endif // PLAYERMINIMONTECARLO_H
