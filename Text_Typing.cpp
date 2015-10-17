#include "Text_Typing.h"
using namespace::std;
using namespace SDL;

Text_Typing::Text_Typing(string path_font, int size, char colR, char colG, char colB, SDL_Event* event) :
Images_Click("", event),
Basic_Text(path_font, size, colR, colG, colB, " ")
{
    _surface = move(SMARTSurface(TTF_RenderText_Solid( _font, _current_text.c_str(), _color)));
    _height = _surface->h;
    _width = _surface->w;
    _text_old = " ";
}

Text_Typing::Text_Typing(string path_font, int size, char colR, char colG, char colB, SDL_Event* event, string text) :
Images_Click("", event),
Basic_Text(path_font, size, colR, colG, colB, text)
{
    _surface = move(SMARTSurface(TTF_RenderText_Solid( _font, _current_text.c_str(), _color)));
    _height = _surface->h;
    _width = _surface->w;
    _text_old = text;
}

void Text_Typing::Update_Text_OnIt()
{
    Update_on_it();
    if(_pEvent->type == SDL_KEYDOWN )
    {
        SDLKey SDK = Convert( _pEvent->key.keysym.sym);
        string chr = Character(SDK);
        if(!(_keystates[SDLK_LCTRL]||_keystates[SDLK_RCTRL]||_keystates[SDLK_LALT]||_keystates[SDLK_RALT]))
        {
            if (chr != "@") _current_text += chr;
        }

    }
}

void Text_Typing::act()
{
    if (_keystates[SDLK_BACKSPACE])
    {
        _current_text = DeleteChar(_current_text);
        SDL_Delay(100);
    }
   if ((_current_text != _text_old)&&(_current_text.length() !=0))
   {
        _surface.reset(); //delete the old surface
        _surface = move(SMARTSurface(TTF_RenderText_Solid( _font, _current_text.c_str(), _color)));
        _text_old = _current_text;
        Uint width = _width;
        Uint height = _height;
        PositionGraphic pos(_pos);
        _width = _surface->w;
        _height = _surface->h;
        pos.Set(pos.Getx()+width/2,pos.Gety()+height/2,CENTER);
        SetPosition(pos);
   }
   else
   {
        if (_current_text != _text_old)
       {
            _surface.reset();
            _surface = move(SMARTSurface(TTF_RenderText_Solid( _font, " ", _color)));
            _text_old = " ";

            Uint width = _width;
            Uint height = _height;
            PositionGraphic pos(_pos);
            _width = _surface->w;
            _height = _surface->h;
            pos.Set(pos.Getx()+width/2,pos.Gety()+height/2,CENTER);
            SetPosition(pos);
       }
   }
}

void Text_Typing::Change_Text(string text)
{
    _current_text = text;
}
