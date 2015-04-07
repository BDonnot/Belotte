#ifndef CARDS_H
#define CARDS_H


/**
CardsUI : depends on the graphical interface
**/
#include "Cards_Basic.h"
#include "Cards_SDL.h"

class Cards : public Cards_Basic, public Cards_SDL
{
    public:
        Cards(){}
        Cards(const Card_Height& height,const Card_Color& color,SDL_Event* event,SDL_Surface * pScreen):
            Cards_Basic(height,color),
            Cards_SDL(height.ToInt(),color.ToInt(),event,pScreen){}
        virtual ~Cards(){}
        virtual void Display()
        {
            static_cast< Images_Sprite<2> * >(this)->Display(_pScreen);
        }
    protected:

    private:
        Cards(const Cards& other);
        Cards& operator=(const Cards& other);

};
#endif // CARDS_H
