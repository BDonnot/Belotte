#ifndef AIPLAYRANDOM_H
#define AIPLAYRANDOM_H

#include<list>
#include<array>

#include "Definitions.h"
#include "Cards.h"
#include "Random.h"
#include "TrickStatus.h"

/** A wrapper : implement the way of playing randomly **/
class AIPlayRandom
{
    public:
        AIPlayRandom(){}
        virtual ~AIPlayRandom(){}
        std::list<Cards*>::iterator Play(const std::array<Cards*,4>& trick
                                         ,std::list<std::list<Cards*>::iterator>& plyableCards
                                         ,const std::list<Cards*>& hand
                                         ,Random& rand
                                         ,const TrickStatus& trickStatus);
        //int test();

    protected:
    private:
};

#endif // AIPLAYRANDOM_H
