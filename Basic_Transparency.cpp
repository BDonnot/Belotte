#include "Basic_Transparency.h"
using namespace SDL;

Basic_Transparency::Basic_Transparency()
{
    _alpha = SDL_ALPHA_OPAQUE;
    _alpha_end = SDL_ALPHA_OPAQUE;
    _fps = 1000/32;
}

Basic_Transparency::Basic_Transparency(Uint8 transparency)
{
    _alpha = transparency;
    _alpha_end = transparency;
    _fps = 1000/32;
}

void Basic_Transparency::transparency()
{
    if ((_timer_alpha_current >= _timer_alpha_start)&&(_timer_alpha_current < _timer_alpha_end))
    {
        _alpha += (_alpha_end-_alpha)*_fps/(_timer_alpha_end-_timer_alpha_current);
    }
    else
    {
        if (_timer_alpha_current > _timer_alpha_end) _alpha = _alpha_end;
    }
}

void Basic_Transparency::Update_Transparent(Uint32 current_time)
{
    _timer_alpha_current = current_time;
}

void Basic_Transparency::Set_Transparent(bool increasing,Uint32 duration,Uint32 time_lag,Uint32 current_time)
{
    _timer_alpha_current = current_time;
    _timer_alpha_end = current_time+time_lag+duration;
    _timer_alpha_start = current_time+time_lag;
    if (increasing) _alpha_end = SDL_ALPHA_OPAQUE;
    else _alpha_end = SDL_ALPHA_TRANSPARENT;
}

void Basic_Transparency::Set_Transparent(Uint8 alpha_end,Uint32 duration,Uint32 time_lag,Uint32 current_time)
{
    _timer_alpha_current = current_time;
    _timer_alpha_end = current_time+time_lag+duration;
    _timer_alpha_start = current_time+time_lag;
    _alpha_end = alpha_end;
}
