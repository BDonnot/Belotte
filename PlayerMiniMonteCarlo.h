#ifndef PLAYERMINIMONTECARLO_H
#define PLAYERMINIMONTECARLO_H

#include <list>

#include "Definitions.h"
//TO DO : make a basic class from which this one and Player will inherit...

#play : AIPlayRandom or AIPlayScore
template<class GameMemory,class PlayAI>
class PlayerMiniMonteCarlo
{
    protected:
        const Player_ID _number;
        std::list<Cards_Basic> _hand;
        GameMemory _gameMemory;
        PlayAI _play;
        Random *_rand;
        TrickStatus _status;

    public:
        PlayerMiniMonteCarlo();
        virtual ~PlayerMiniMonteCarlo();
        void ReceiveInitInfo(const GameMemory& initMemory,const Random& initRandomState);
        void ReceiveHand(const std::list<Cards*>& initHand);
        Cards_Basic Play();


    protected:

    private:
};

template<class GameMemory,class PlayAI>
void PlayerMiniMonteCarlo<GameMemory,PlayAI>::ReceiveInitInfo(const GameMemory& initMemory,
                                                              const Random& initRandomState)
{
    _rand = *initRandomState;
    _gameMemory = initMemory;

}
template<class GameMemory,class PlayAI>
void PlayerMiniMonteCarlo<GameMemory,PlayAI>::ReceiveHand(const std::list<Cards*>& initHand))
{
    _hand.clear();
    for(auto pCard : initHand)
    {
        _hand.push_back(static_cast<Cards_Basic>(*pCard));
    }

}
#endif // PLAYERMINIMONTECARLO_H
