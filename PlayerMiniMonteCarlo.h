#ifndef PLAYERMINIMONTECARLO_H
#define PLAYERMINIMONTECARLO_H

#include <list>
#include<algorithm>
#include<unordered_map>

#include "Definitions.h"
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
        std::list<Cards_Basic> _hand;
        Uint _nbCardToReceive;
        std::unordered_map<Cards_Basic,bool> _canReceiveCard;
        Random *_rand;
        TrickStatus _status;

    public:
        PlayerMiniMonteCarlo():_number(GHOST),_status(_number){}
        PlayerMiniMonteCarlo(const Player_ID& number):_number(number),_status(_number){}
        virtual ~PlayerMiniMonteCarlo(){}
        void ReceiveInitInfo(const GameMemory& initMemory,Random& initRandomState,const std::list<Cards_Basic>& cardsToGive,Uint nbCardToReceive);
        void ReceiveHand(const std::list<Cards*>& initHand);
        void ReceiveCard(const Cards_Basic& card); //true if the cards is put, false otherwise
        bool CanReceiveCard(const Cards_Basic& card) const;
        bool CanReceiveAnotherCard() const;
        const Player_ID& ID() const {return _number;}
        const Uint NumberCardToReceive() const {return _nbCardToReceive;}
        void RetrieveCard(const Cards_Basic& card); //retrieve the card from the hand
        Cards_Basic Play();

        void AddConstraint(const Cards_Basic& card);
        void RemoveConstraint(const Cards_Basic& card);

        void PrintHand() const;
    protected:

    private:
};

template<class GameMemory>
void PlayerMiniMonteCarlo<GameMemory>::ReceiveInitInfo(const GameMemory& initMemory,
                                                              Random& initRandomState,
                                                              const std::list<Cards_Basic>& cardsToGive,
                                                              Uint nbCardToReceive)
{
    _nbCardToReceive = nbCardToReceive;
    _rand = &initRandomState;
    for(const auto card : cardsToGive)
    {
        _canReceiveCard.emplace(card,!initMemory.Cut(_number,card.GetColour()));
    }
}

template<class GameMemory>
void PlayerMiniMonteCarlo<GameMemory>::AddConstraint(const Cards_Basic& card)
{
    _canReceiveCard[card] = false;
    printf("I have added the constraint in player\n");
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
    /*
    auto it = _hand.begin();
    for(it; it != _hand.end(); ++it)
    {
        if(it->GetColour() == card.GetColour() && it->GetHeight() == card.GetHeight() )
            break;
    }
    */
    _nbCardToReceive++;
    _hand.remove(card);
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
#endif // PLAYERMINIMONTECARLO_H
