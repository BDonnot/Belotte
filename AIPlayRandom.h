#ifndef AIPLAYRANDOM_H
#define AIPLAYRANDOM_H

#include<list>
#include<array>

#include "Definitions.h"
#include "Cards.h"
#include "Random.h"
#include "TrickStatus.h"

/** A wrapper : implement the way of playing randomly **/
template<class Memory,class TypeOfCard>
class AIPlayRandom
{
    protected :
        Player_ID _player;
    public:
        AIPlayRandom(const Player_ID& player):_player(player){}
        virtual ~AIPlayRandom(){}
        typename std::list<TypeOfCard>::iterator Play(const TrickBasic_Memory& trick
                                         ,typename std::list<typename std::list<TypeOfCard>::iterator >& plyableCards
                                         ,const std::list<Cards*>& hand
                                         ,Random& rand
                                         ,const TrickStatus& trickStatus
                                         ,const Memory& playerMemory);
        void SetNumber(const Player_ID& number)
        {
            _player = number;
        }

    protected:
    private:
};

template<class Memory,class TypeOfCard>
typename std::list<TypeOfCard>::iterator AIPlayRandom<Memory,TypeOfCard>::Play(const TrickBasic_Memory& trick
                                                                               ,typename std::list<typename std::list<TypeOfCard>::iterator>& playbleCards
                                                                               ,const std::list<Cards*>& hand
                                                                               ,Random& rand
                                                                               ,const TrickStatus& trickStatus
                                                                               ,const Memory& playerMemory)
{
    Uint n = playbleCards.size();
    switch(n)
    {
    case 0 :
        return playbleCards.front(); //TO DO exception here !
    case 1:
        return playbleCards.front();
    default :
        auto it = playbleCards.begin();
        std::advance(it,rand.generate_number()%(n-1));
        return (*it);
    }
}
#endif // AIPLAYRANDOM_H
