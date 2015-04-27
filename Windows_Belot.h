#ifndef WINDOWS_BELOT_H
#define WINDOWS_BELOT_H

#if defined (__GNUC__)
	#include "SDL/SDL.h"
#elif defined(_MSC_VER)
	#include <SDL.h>
#endif  //SDL include

#include "Quit.h"
#include "Basic_Images.h"
#include "Screen_end.h"
#include "Screen_Begin.h"
#include "Screen_Option.h"

class Windows_Belot : public Quit
{
    public:
        Windows_Belot();
        void Display();
        virtual ~Windows_Belot();
    protected:
        Basic_Images _back;
        SDL_Surface* _screen;
        Uint16 _SCREEN_WIDTH;
        Uint16 _SCREEN_HEIGHT;
        Uint16 _SCREEN_BPP;
        Uint16 _fps;
        Screen_end _screen_end;
        Screen_Begin _screen_begin;
        Screen_Option _screen_option;
        int _nb_human_player;
        Uint32 _start_loop;
        Uint32 _end_loop;

    private:
};

#endif // WINDOWS_BELOT_H
