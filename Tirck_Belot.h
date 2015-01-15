#ifndef TIRCK_BELOT_H
#define TIRCK_BELOT_H

/*
count the cards (points of each team)
knows which player has to play
remember the last trick played
deal with the trick currently played
*/

#include "Cards.h"
#include "Player.h"
#include "Images_Text.h"
#include "Trump_Image.h"
class Tirck_Belot
{
    public:
        Tirck_Belot();
        virtual ~Tirck_Belot();
    protected:
    private:
        Trump_Image _Trump_Image;
        std::array<int,2> _number_point;
        std::array<Cards*,4> _trick;
        std::array<Cards*,4> _trick_old;

};

#endif // TIRCK_BELOT_H
