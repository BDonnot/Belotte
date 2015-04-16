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

namespace std
{
    template<>
    struct hash<Cards_Basic>
    {
        size_t operator() (const Cards_Basic& card) const
        {
            return( (hash<Uint>()(card.GetColour())) ^ (hash<Uint>()(card.GetHeight()) << 1) >> 1)
        }
    };
}
//play : AIPlayRandom or AIPlayScore
template<class GameMemory,class PlayAI>
class PlayerMiniMonteCarlo
{
    protected:
        const Player_ID _number;
        std::list<Cards_Basic> _hand;
        Uint _nbCardToReceive;
        std::unordered_map<Cards_Basic,bool> _canReceiveCard;
        PlayAI _play;
        Random *_rand;
        TrickStatus _status;

    public:
        PlayerMiniMonteCarlo();
        virtual ~PlayerMiniMonteCarlo();
        void ReceiveInitInfo(const GameMemory& initMemory,Random& initRandomState,const std::list<Cards_Basic>& cardsToGive,Uint nbCardToReceive);
        void ReceiveHand(const std::list<Cards*>& initHand);
        void ReceiveCard(const Cards_Basic& card); //true if the cards is put, false otherwise
        bool CanReceiveCard(const Cards_Basic& card);
        bool CanReceiveAnotherCard();
        const Player_ID& ID() const {return _number;}
        Cards_Basic RetrieveCard(const Cards_Basic& card); //retrieve the card from the hand
        Cards_Basic Play();


    protected:

    private:
};

template<class GameMemory,class PlayAI>
void PlayerMiniMonteCarlo<GameMemory,PlayAI>::ReceiveInitInfo(const GameMemory& initMemory,
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

template<class GameMemory,class PlayAI>
void PlayerMiniMonteCarlo<GameMemory,PlayAI>::ReceiveHand(const std::list<Cards*>& initHand)
{
    _hand.clear();
    for(auto pCard : initHand)
    {
        _hand.push_back(static_cast<Cards_Basic>(*pCard));
    }
    _nbCardToReceive = 0;

}

template<class GameMemory,class PlayAI>
void PlayerMiniMonteCarlo<GameMemory,PlayAI>::ReceiveCard(const Cards_Basic& card) //true if the cards is put, false otherwise
{
    _hand.push_back(card);
    _nbCardToReceive--;
}

template<class GameMemory,class PlayAI>
bool PlayerMiniMonteCarlo<GameMemory,PlayAI>::CanReceiveAnotherCard() //true if the cards is put, false otherwise
{
    return _nbCardToReceive > 0;
}

template<class GameMemory,class PlayAI>
bool PlayerMiniMonteCarlo<GameMemory,PlayAI>::CanReceiveCard(const Cards_Basic& card) //true if the cards is put, false otherwise
{
    return _canReceiveCard[card];
}

template<class GameMemory,class PlayAI>
Cards_Basic PlayerMiniMonteCarlo<GameMemory,PlayAI>::RetrieveCard(const Cards_Basic& card) //retrieve a card from the hand, randomly
{
    auto it = _hand.begin();
    for(it; it != _hand.end(); ++it)
    {
        if(it->GetColour() == card.GetColour() && it->GetHeight() == card.GetHeight() )
            break;
    }
    _nbCardToReceive++;
    _hand.erase(it);
    return card;
}
#endif // PLAYERMINIMONTECARLO_H
