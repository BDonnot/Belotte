#ifndef CARDS_H
#define CARDS_H

/**
** Inherit from Cards_Basic and Cards_SDL.
** A card is 2 things :
** the information it contains (height, color etc.)
** and the image on the screen.
**/

#include "Cards_Basic.h"
#include "Cards_SDL.h"

class Cards : public Cards_Basic, public Cards_SDL
{
    public:
        Cards(){}
        Cards(const Card_Height& height,const Card_Color& color):
             Cards_Basic(height,color){}
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
        Cards(const Cards& other){}
        Cards& operator=(const Cards& other){return *this;}

};
#endif // CARDS_H
