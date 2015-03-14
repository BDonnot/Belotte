#include "AIPlayRandom.h"
/*
std::list<Cards*>::iterator AIPlayRandom::Play(const std::array<Cards*,4>& trick
                                       ,std::list<std::list<Cards*>::iterator>& playbleCards
                                       ,const std::list<Cards*>& hand
                                       ,Random& rand
                                       ,const TrickStatus& trickStatus)
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
*/
