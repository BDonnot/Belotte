#include "Screen_end.h"

#if COMPLETE_GAME > 0
Screen_end::Screen_end(Basic_Images* fond,SDL_Event* pevent,double screen_width,double screen_height,std::string message):Quit(pevent),
_quit(pevent,"Quitter",PositionGraphic())
,_thanks("fonts/MLSGY.ttf",60,255,255,255,message)
{
    _fond = fond;
    PositionGraphic pos(screen_width/2,3*screen_height/7,CENTER);
    _thanks.SetPosition(pos);
    pos.Set(screen_width/2,3*screen_height/5,CENTER);
    _quit.SetPosition(pos);
}

void Screen_end::Display(SDL_Surface* destination)
{
    bool continuer = true;
    while (continuer)
    {
        while(SDL_PollEvent(_event))
        {
            continuer = continuer && KeepLooping();
            _quit.Update_on_it();
        }
        if (_quit.Click(true)) continuer = false;
        _fond->Display(destination);
        _thanks.Display(destination);
        _quit.Display(destination);
        SDL_Flip(destination);
    }
}
#else
Screen_end::Screen_end(Basic_Images* fond, double screen_width, double screen_height, std::string message) :Quit(),
_quit("Quitter", PositionGraphic())
, _thanks("fonts/MLSGY.ttf", 60, 255, 255, 255, message)
{
	_fond = fond;
	PositionGraphic pos(screen_width / 2, 3 * screen_height / 7, CENTER);
	_thanks.SetPosition(pos);
	pos.Set(screen_width / 2, 3 * screen_height / 5, CENTER);
	_quit.SetPosition(pos);
}
#endif //#if COMPLETE_GAME > 0
