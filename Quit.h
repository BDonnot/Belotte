#ifndef QUIT_H
#define QUIT_H
#include "SDL/SDL.h"

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
