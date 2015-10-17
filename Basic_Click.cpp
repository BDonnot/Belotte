#include "Basic_Click.h"
using namespace SDL;

Basic_Click::Basic_Click(SDL_Event* event)
{
    _on_it = false;
    _click_on_short = false;
    _click_on_long = false;
    _pEvent = event;
}

void Basic_Click::Update_on_it()
{
    _on_it = false;
}

void Basic_Click::Set_click_on_long()
{
    if (_click_on_long)  _click_on_long=(_on_it||!(_pEvent->type == SDL_MOUSEBUTTONDOWN));
    else _click_on_long = (_on_it&&(_pEvent->type==SDL_MOUSEBUTTONDOWN));
}

void Basic_Click::Set_click_on_short()
{
    _click_on_short=(_on_it&&(_pEvent->type==SDL_MOUSEBUTTONDOWN));
}

bool Basic_Click::Click(bool Short)
{
    bool cShort = _click_on_short;
    bool cLong = _click_on_long;
    if (Short)
    {
        if(cShort) Reset_Click(); //double click not handle by SDL (as far as I know)
        return cShort ;
    }
    return cLong ;
}

void Basic_Click::Reset_Click()
{
    _click_on_long = false;
    _click_on_short = false;
}
