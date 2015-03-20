#ifndef FONCTEURS_CARDSBASIC_H_INCLUDED
#define FONCTEURS_CARDSBASIC_H_INCLUDED
#include "Cards_Basic.h"

class Sort_Cards
{
    public :
        Sort_Cards(int trump_color):
        _trump_color(trump_color)
        {
            _winColor = {false,true,true,true,false,false,false,true,false,true,false,true,false,false,false,false}; //sort the colours : carreau, pique,coeur,trefle
        }
        bool operator()(Cards_Basic* card1,Cards_Basic* card2) // says if the card1 must be put at the left of the card2
        {
//            int height1 = card1->GetHeight();
            CARDS_HEIGHT height2 = card2->GetHeight();
            CARDS_COLOR color1 = card1->GetColour();
            CARDS_COLOR color2 = card2->GetColour();
            if (color1 != color2) return _winColor[4*color1+color2];
            return card1->Win(height2);
        }
    private :
        int _trump_color;
        std::array<bool,16> _winColor;

};


#endif // FONCTEURS_CARDSBASIC_H_INCLUDED
