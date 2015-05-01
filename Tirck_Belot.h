#ifndef TIRCK_BELOT_H
#define TIRCK_BELOT_H


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
