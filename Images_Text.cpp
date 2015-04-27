#include "Images_Text.h"
using namespace::std;
Images_Text::Images_Text()
{
    SDL_EnableUNICODE( SDL_ENABLE );
    _keystates = SDL_GetKeyState( NULL );
}
Images_Text::Images_Text(string path_font, int size, char colR, char colG, char colB, string text) :
Basic_Images("")
, Basic_Text(path_font, size, colR, colG, colB, text)
{
    _surface = move(SMARTSurface(TTF_RenderText_Solid( _font, _current_text.c_str(), _color)));
    _height = _surface->h;
    _width = _surface->w;
}
Images_Text::~Images_Text()
{
    //dtor
}
void Images_Text::Display(SDL_Surface* destination)
{
    act();
    if (_surface==NULL) return;
    if (_change)
    {
        _change = false;
//        SDL_FreeSurface(_surface);
        _surface.reset();
        _surface = move(SMARTSurface(TTF_RenderText_Solid( _font, _current_text.c_str(), _color)));
        //_pos_x += _width/2;
        //_pos_y += _height/2;
        PositionGraphic pos(_pos.Getx()+_width/2,_pos.Gety()+_height/2,CENTER);
        _width = _surface->w;
        _height = _surface->h;
        pos.SetProperPosition(_width,_height);
        SetPosition(pos);
    }
    apply_surface(_pos.Getx(),_pos.Gety(),_surface,destination);
}
