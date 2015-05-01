#ifndef FONCTEURS_CARDSBASIC_H_INCLUDED
#define FONCTEURS_CARDSBASIC_H_INCLUDED

/**
** This class (maybe classes in the next future) define some
** some functors on Cards_Basic.
**/

#include "Cards_Basic.h"

class Sort_Cards
{
    public :
        Sort_Cards()
        {
            _winColor = {false,true,true,true,false,false,false,true,false,true,false,true,false,false,false,false}; //sort the colours : carreau, pique,coeur,trefle
        }
        bool operator()(Cards_Basic* card1,Cards_Basic* card2) // says if the card1 must be put at the left of the card2
        {
            const Card_Height& height2 = card2->GetHeight();
            const Card_Color& color1 = card1->GetColour();
            const Card_Color& color2 = card2->GetColour();
            if (color1 != color2) return _winColor[4*color1.ToInt()+color2.ToInt()];
            return card1->Win(height2);
        }
    private :
        std::array<bool,16> _winColor;

};


#endif // FONCTEURS_CARDSBASIC_H_INCLUDED
