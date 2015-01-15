#ifndef TRUMP_IMAGE_H
#define TRUMP_IMAGE_H
/**
the image of the trump (of the set) in the right
corner of the windows with the name of the taker.
if the mouse is over, it displays the last trick played.
**/

#include "Basic_Click.h"
#include "Images_Text.h"
#include "Images_Sprite.h"
#include "Player_Name.h"


class Trump_Image : public Images_Sprite<4>,Basic_Click
{
    public:
        Trump_Image();
        Trump_Image(SDL_Event* event);
        void Set_Taker(int taker);
        virtual ~Trump_Image();
    protected:
        int _trunk;
        int _taker;


        void Update_on_it();
    private:
        Trump_Image(const Trump_Image& other);
        Trump_Image& operator=(const Trump_Image& other);
};

#endif // TRUMP_IMAGE_H
