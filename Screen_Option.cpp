#include "Screen_Option.h"

Screen_Option::Screen_Option()
{
    //ctor
}
Screen_Option::Screen_Option(Basic_Images* fond,SDL_Event* pevent,Uint screen_width,Uint screen_height):Quit(pevent),
_nb_player(pevent,"Joueurs",PositionGraphic())
,_screen_size(pevent,"Ecran",PositionGraphic())
,_retour(pevent,"Retour",PositionGraphic())
{
    _fond = fond;
    //_origin_indice = 0;
    PositionGraphic pos(screen_width/2,3*screen_height/8,CENTER);
    _nb_player.SetPosition(pos);
    pos.Set(screen_width/2,4*screen_height/8,CENTER);
    _screen_size.SetPosition(pos);
    pos.Set(screen_width/2,5*screen_height/8,CENTER);
    _retour.SetPosition(pos);
}
Screen_Option::~Screen_Option()
{
    //dtor
}
void Screen_Option::Update()
{
    //if (!_onScreen) return;
    _retour.Update_on_it();
    _nb_player.Update_on_it();
    _screen_size.Update_on_it();
}
OPTION_OUTPUT Screen_Option::Display(SDL_Surface* destination)
{
    //if (!_onScreen) return OPTION_RETURN;
    bool keep_displaying = true;
    Uint32 start = 0,end = 0;
    double FRAMES_PER_SECOND = 32.;
    Uint32 min_time_loop = static_cast<Uint32>(1000./FRAMES_PER_SECOND);
    while (keep_displaying)
    {
        start = SDL_GetTicks();
        while(SDL_PollEvent(_event))
        {
            keep_displaying = keep_displaying && KeepLooping();
            Update();
        }
        if (!keep_displaying) return OPTION_QUIT;
        if (_retour.Click(true))
        {
            SDL_Delay(150);
            _retour.Reset_Click();
            return OPTION_RETURN;
        }
        _fond->Display(destination);
        _nb_player.Display(destination);
        _screen_size.Display(destination);
        _retour.Display(destination);
        end = SDL_GetTicks();
        if (end < min_time_loop + start) SDL_Delay( min_time_loop - end + start );
        SDL_Flip(destination);
    }
    return OPTION_CONTINUE;
}
/*
void Screen_Option::Set_Origin(int i)
{
    _origin_indice = i;
}
*/
