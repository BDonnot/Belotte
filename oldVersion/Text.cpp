#include "Text.h"
using namespace std;


Text::Text()
{
    SDL_EnableUNICODE( SDL_ENABLE );
    _surface = NULL;
    _path = "";
    _pEvent = NULL;
    _simple_image = false;
    _on_it = false;
    _click_on_long = false;
    _click_on_short = false;
    _typing = true;
    _nb_moving = 0;
    _mov_x = 0;
    _mov_y=0;
    _nmb_movt = 0;
}
Text::Text(string path, int size,SDL_Color color,SDL_Event& event)
{
    SDL_EnableUNICODE( SDL_ENABLE );
    _height = 0;
    _width = 0;
    _font = TTF_OpenFont( path.c_str(), size );
    _color = color;
    _pos_x = 0;
    _pos_y = 0;
    _surface = NULL;
    _text ="";
    _text_old="";
    _pos_th_x=0;
    _pos_th_y=0;
    _pos_option = 5;
    _path = path;
    _pEvent = &event;
    _simple_image = false;
    _on_it = false;
    _click_on_long = false;
    _click_on_short = false;
    _size = size;
    _typing = true;
    _nb_moving = 0;
    _mov_x = 0;
    _mov_y=0;
    _nmb_movt = 0;
}
Text::~Text()
{
    SDL_EnableUNICODE( SDL_DISABLE );
    if (!_simple_image) SDL_FreeSurface(_surface);
    //TTF_CloseFont(_font); //I don't kown why, but it's not necessary to free the "font" of 'Text' ...
}
Text::Text(const Text& other)
{
    _height=other._height;
    _width=other._width;
    _pos_x=other._pos_x;
    _pos_y=other._pos_y;
    _pos_th_x=other._pos_th_x;
    _pos_th_y=other._pos_th_y;
    _pos_option=other._pos_option;
    _nmb_movt=other._nmb_movt;
    _mov_x=other._mov_x;
    _mov_y=other._mov_y;
    _alpha=other._alpha;
    _on_it = other._on_it;
    _end_x=other._end_x;
    _end_y=other._end_y;
    _click_on_long = other._click_on_long;
    _click_on_short = other._click_on_short;
    _time_lag = other._time_lag;
    _nb_moving=other._nb_moving;
    _transparency=other._transparency;
    _color = other._color;
    _text =other._text;
    _text_old=other._text_old;
    _path=other._path;
    _size = other._size;
    _simple_image = other._simple_image;
    _typing = other._typing;
    _pEvent = other._pEvent;
    _font = TTF_OpenFont( _path.c_str(), _size );
    _surface = TTF_RenderText_Solid( _font, _text.c_str(), _color);
}
Text& Text::operator=(const Text& rhs)
{
    if (this == &rhs) return *this; // handle self assignment
    _height=rhs._height;
    _width=rhs._width;
    _pos_x=rhs._pos_x;
    _pos_y=rhs._pos_y;
    _pos_th_x=rhs._pos_th_x;
    _pos_th_y=rhs._pos_th_y;
    _pos_option=rhs._pos_option;
    _nmb_movt=rhs._nmb_movt;
    _mov_x=rhs._mov_x;
    _mov_y=rhs._mov_y;
    _alpha=rhs._alpha;
    _path=rhs._path;
    _on_it = rhs._on_it;
    _time_lag = rhs._time_lag;
    _nb_moving=rhs._nb_moving;
    _transparency=rhs._transparency;
    _end_x=rhs._end_x;
    _end_y=rhs._end_y;
    _click_on_long = rhs._click_on_long;
    _click_on_short = rhs._click_on_short;
    _color = rhs._color;
    _text =rhs._text;
    _text_old=rhs._text_old;
    _size = rhs._size;
    _simple_image = rhs._simple_image;
    _typing = rhs._typing;
    _pEvent = rhs._pEvent;
    _font = TTF_OpenFont( _path.c_str(), _size );
    _surface = TTF_RenderText_Solid( _font, _text.c_str(), _color);
    return *this;
}
TTF_Font* Text::GetFont()
{
    return _font;
}
string Text::GetText()
{
    return _text;
}
void Text::add_letter(char a)
{
    //maximum lenght authorized for names : 13
   if( _text.length() <= 12 ) _text += a;
   SDL_Delay(30);
}
void Text::remove_letter()
{
    if (_text.length() !=0) _text.erase(_text.length()-1);
    SDL_Delay(30);
}
void Text::write()
{
   if ((_text != _text_old)&&(_text.length() !=0))
   {
        SDL_FreeSurface( _surface );
        _surface = TTF_RenderText_Solid( _font, _text.c_str(), _color);
        _text_old = _text;
        _width = _surface->w;
        _height = _surface->h;
   }
   else
   {
        if (_text != _text_old)
       {
            SDL_FreeSurface( _surface );
            _surface = TTF_RenderText_Solid( _font, " ", _color);
            _text_old = " ";
            _width = _surface->w;
            _height = _surface->h;
       }
   }
}
void Text::remove_text()
{
    _text = "";
    write();
}
void Text::type(string text)
{
   _typing = false;
   _text = text;
   write();
}
void Text::typing()
{
    if(_pEvent->type == SDL_KEYDOWN )
    {
        if (_pEvent->key.keysym.unicode == (Uint16)' ') add_letter((char) _pEvent->key.keysym.unicode);
        else {if (_pEvent->key.keysym.sym == SDLK_BACKSPACE ) remove_letter();
        else {if ((_pEvent->key.keysym.unicode >= (Uint16)'A')&&(_pEvent->key.keysym.unicode <= (Uint16)'Z')) add_letter((char) _pEvent->key.keysym.unicode);
        else {if ((_pEvent->key.keysym.unicode >= (Uint16)'a')&&(_pEvent->key.keysym.unicode <= (Uint16)'z'))add_letter((char) _pEvent->key.keysym.unicode);
        else {if ((_pEvent->key.keysym.unicode >= (Uint16)'0')&&(_pEvent->key.keysym.unicode <= (Uint16)'9')) add_letter((char) _pEvent->key.keysym.unicode);}}}}
        SDL_Delay(15);
    }
}
void Text::update()
{
    set_on_it();
    set_click_on_short();
    set_click_on_long();
    if (_typing) typing();
    write();
}
