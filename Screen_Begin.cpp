#include "Screen_Begin.h"
using namespace SDL;

Screen_Begin::Screen_Begin(Basic_Images* fond,SDL_Event* pevent,double screen_width,double screen_height):
Quit(pevent)
,_fond(fond)
,_play(pevent,"Jouer",PositionGraphic())
,_option(pevent,"Options",PositionGraphic())
,_quit(pevent,"Quitter",PositionGraphic())
,_onScreen(true)
,_options(_fond,pevent,screen_width,screen_height)
{
    PositionGraphic pos(screen_width/2,3*screen_height/8,CENTER);
    _play.SetPosition(pos);
    pos.Set(screen_width/2,4*screen_height/8,CENTER);
    _option.SetPosition(pos);
    pos.Set(screen_width/2.,5*screen_height/8,CENTER);
    _quit.SetPosition(pos);
}

void Screen_Begin::Update()
{
    if (!_onScreen) return;
    _quit.Update_on_it();
    _play.Update_on_it();
    _option.Update_on_it();
}

OUTPUT_BEGIN Screen_Begin::Display(SDL_Surface* destination)
{
    if (!_onScreen) return PLAY;
    bool keep_displaying = true;
    Uint32 start = 0,end = 0;
    Uint32 min_time_loop = static_cast<Uint32>(1000./FRAMES_PER_SECOND_FOR_ME);
    while (keep_displaying)
    {
        start = SDL_GetTicks();
        while(SDL_PollEvent(_event))
        {
            if (!KeepLooping())
            {
                _onScreen = false;
                return QUIT;
            }
            Update();
        }
        if (_quit.Click(true))
        {
            SDL_Delay(100);
            _quit.Reset_Click();
            _onScreen = false;
            return QUIT;
        }
        if (_play.Click(true))
        {
            _onScreen = false;
            return PLAY;
        }
        if (_option.Click(true))
        {
            SDL_Delay(100);
            _option.Reset_Click();
            switch (_options.Display(destination))
            {
                case OPTION_QUIT :
                    return QUIT;
                case OPTION_RETURN :
                    break;
                default :
                    break;
            }
        }
        _fond->Display(destination);
        _quit.Display(destination);
        _play.Display(destination);
        _option.Display(destination);
        end = SDL_GetTicks();
        if (end < min_time_loop + start) SDL_Delay( min_time_loop - end + start );
        SDL_Flip(destination);
    }
    _onScreen = false;
    return PLAY;
}
