#ifndef SCREEN_OPTION_H
#define SCREEN_OPTION_H

/**
** This class handles the display of the screen "option"
**/

#include "Quit.h"
#include "Basic_Images.h"
#include "Images_Button.h"

class Screen_Option : public Quit
{
    protected:
        Basic_Images* _fond;
        Images_Button _nb_player;
        Images_Button _screen_size;
        Images_Button _retour;
    public:
        Screen_Option(){}
        Screen_Option(Basic_Images* fond,SDL_Event* pevent,Uint screen_width,Uint screen_height);
        virtual ~Screen_Option(){}
        OPTION_OUTPUT Display(SDL_Surface* destination);
        void Update();
    private:
};

#endif // SCREEN_OPTION_H
