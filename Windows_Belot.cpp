#include "Windows_Belot.h"

Windows_Belot::Windows_Belot():Quit()
,_back("images/tapis_moyen_2.jpg")
,_SCREEN_WIDTH(1024),_SCREEN_HEIGHT(768)
,_screen_end(&_back,_event,_SCREEN_WIDTH,_SCREEN_HEIGHT,"Merci d'avoir joué !")
,_screen_begin(&_back,_event,_SCREEN_WIDTH,_SCREEN_HEIGHT)
,_screen_option(&_back,_event,_SCREEN_WIDTH,_SCREEN_HEIGHT)
{
    _SCREEN_BPP=32;
    _fps = 1000/_SCREEN_BPP;
    _screen = SDL_SetVideoMode( _SCREEN_WIDTH, _SCREEN_HEIGHT, _SCREEN_BPP, SDL_ASYNCBLIT);
    _nb_human_player = 1;
}

Windows_Belot::~Windows_Belot()
{
    //dtor
}
void Windows_Belot::Display()
{
    bool continuer = true;
    Uint8 i = 0;
    while (continuer)
    {
        _start_loop = SDL_GetTicks();
        _back.Display(_screen);
        if (i >= 0)
        {
            switch(i)
            {
                case 0 : //TO DO why integer, use "define"'s instead
                    i+=_screen_begin.Display(_screen);
                    //if (i==64) _screen_option.Set_Origin(0);
                    break;
                case 64 : //TO DO why integer, use "define"'s instead
                    i+= _screen_option.Display(_screen);
                    break;
                default: //gere les evenements si aucun ecran particulier n'est difuse
                    while(SDL_PollEvent(_event))
                    {
                        continuer = continuer && KeepLooping();
                    }
                    break;
            }
            if (continuer)
            {
                _end_loop = SDL_GetTicks();
                SDL_Flip(_screen);
                if (_end_loop-_start_loop <_fps) SDL_Delay( _fps - _end_loop+_start_loop );
            }
            else _screen_end.Display(_screen);
        }
        else continuer = false;
    }
}
