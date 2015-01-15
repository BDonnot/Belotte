#include "Text_Typing.h"
using namespace::std;

Text_Typing::Text_Typing()
{
    //ctor
}
Text_Typing::Text_Typing(string path_font,int size,SDL_Color color,SDL_Event* event):Images_Click("",event),Basic_Text(path_font,size,color," ")
{
    _surface = move(SMARTSurface(TTF_RenderText_Solid( _font, _current_text.c_str(), _color)));
    _height = _surface->h;
    _width = _surface->w;
    _text_old = " ";
}
Text_Typing::Text_Typing(string path_font,int size,SDL_Color color,SDL_Event* event,string text):Images_Click("",event),Basic_Text(path_font,size,color,text)
{
    _surface = move(SMARTSurface(TTF_RenderText_Solid( _font, _current_text.c_str(), _color)));
    _height = _surface->h;
    _width = _surface->w;
    _text_old = text;
}
Text_Typing::~Text_Typing()
{
    //dtor
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
            //else _text = DeleteChar(_text);
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
//        SDL_FreeSurface( _surface );
        _surface.reset();
        _surface = move(SMARTSurface(TTF_RenderText_Solid( _font, _current_text.c_str(), _color)));
        _text_old = _current_text;
        //return to orignal position :
        //array<Uint16,2> pos_aux = position_image_aux();
        //_pos_x -= pos_aux[0];
        //_pos_y -= pos_aux[1];
        // set to the right position
        Uint width = _width;
        Uint height = _height;
        PositionGraphic pos(_pos);
        _width = _surface->w;
        _height = _surface->h;
        pos.Set(pos.Getx()+width/2,pos.Gety()+height/2,CENTER);
        SetPosition(pos);
        //pos_aux = position_image_aux();
        //_pos_x += pos_aux[0];
        //_pos_y += pos_aux[1];
   }
   else
   {
        if (_current_text != _text_old)
       {
//            SDL_FreeSurface( _surface );
            _surface.reset();
            _surface = move(SMARTSurface(TTF_RenderText_Solid( _font, " ", _color)));
            _text_old = " ";

            //array<Uint16,2> pos_aux = position_image_aux();
            //_pos_x -= pos_aux[0];
            //_pos_y -= pos_aux[1];
            Uint width = _width;
            Uint height = _height;
            PositionGraphic pos(_pos);
            _width = _surface->w;
            _height = _surface->h;
            pos.Set(pos.Getx()+width/2,pos.Gety()+height/2,CENTER);
            SetPosition(pos);
            //_width = _surface->w;
            //_height = _surface->h;
            //pos_aux = position_image_aux();
            //_pos_x += pos_aux[0];
            //_pos_y += pos_aux[1];
       }
   }
}
void Text_Typing::Change_Text(string text)
{
    _current_text = text;
}
