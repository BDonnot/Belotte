#ifndef QUIT_H
#define QUIT_H

#ifndef _MSC_VER
	#include "SDL/SDL.h"
#endif //ndef _MSC_VER
#ifdef _MSC_VER
	#include <SDL.h>
#endif //def _MSC_VER

class Quit //Just contain the method Quit()
{
    public:
        Quit();
        Quit(SDL_Event* pevent);
        virtual ~Quit();
        bool KeepLooping();
    protected:
        SDL_Event* _event;
        Uint8* _keystates;
        bool _destruct;
    private:
};

#endif // QUIT_H
