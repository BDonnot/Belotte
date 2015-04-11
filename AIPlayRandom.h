#ifndef AIPLAYRANDOM_H
#define AIPLAYRANDOM_H

#include<list>
#include<array>

#include "Definitions.h"
#include "Cards.h"
#include "Random.h"
#include "TrickStatus.h"

/** A wrapper : implement the way of playing randomly **/
template<class Memory>
class AIPlayRandom
{
    protected :
        const Player_ID _player;
    public:
        AIPlayRandom(const Player_ID& player):_player(player){}
        virtual ~AIPlayRandom(){}
        std::list<Cards*>::iterator Play(const std::array<Cards*,4>& trick
                                         ,std::list<std::list<Cards*>::iterator>& plyableCards
                                         ,const std::list<Cards*>& hand
                                         ,Random& rand
                                         ,const TrickStatus& trickStatus
                                         ,const Memory& playerMemory);

    protected:
    private:
};

template<class Memory>
std::list<Cards*>::iterator AIPlayRandom<Memory>::Play(const std::array<Cards*,4>& trick
                                       ,std::list<std::list<Cards*>::iterator>& playbleCards
                                       ,const std::list<Cards*>& hand
                                       ,Random& rand
                                       ,const TrickStatus& trickStatus
                                       ,const Memory& playerMemory)
{
    Uint n = playbleCards.size();
    switch(n)
    {
    case 0 :
        return playbleCards.front(); //throw an exception here !
    case 1:
        return playbleCards.front();
    default :
        auto it = playbleCards.begin();
        std::advance(it,rand.generate_number()%(n-1));
        return (*it);
    }
}
#endif // AIPLAYRANDOM_H
