#ifndef AIPLAYRANDOM_H
#define AIPLAYRANDOM_H

/**
** This is the first wrapper for the method playing for AI players.
** It allow them to play randomly a card among
** the cards playable.
**/

#include<list>
#include<array>

#include "Definitions.h"
#include "Cards.h"
#include "Random.h"
#include "TrickStatus.h"

template<class TypeOfCard, template<class> class Memory >
class AIPlayRandom
{
    protected :
        Player_ID _player;
    public:
        AIPlayRandom(const Player_ID& player):_player(player){}
        virtual ~AIPlayRandom(){}
        TypeOfCard Play(const TrickBasic_Memory& trick
                                ,typename std::list<TypeOfCard >& plyableCards
								, const std::list<TypeOfCard>& hand
                                ,Random& rand
                                ,const TrickStatus& trickStatus
								,const Memory<TypeOfCard> & playerMemory);
        void SetNumber(const Player_ID& number)
        {
            _player = number;
        }

    protected:
    private:
};

template<class TypeOfCard, template<class> class Memory >
TypeOfCard AIPlayRandom<TypeOfCard, Memory>::Play(const TrickBasic_Memory& trick
													,typename std::list<TypeOfCard >& playbleCards
													, const std::list<TypeOfCard>& hand
													,Random& rand
													,const TrickStatus& trickStatus
													,const Memory<TypeOfCard>& playerMemory)
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
