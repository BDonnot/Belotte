#include "Button.h"
using namespace::std;

Button::Button()
{
    _surface = NULL;
    _surface2 = NULL;
    _text_surface = NULL;
    _font = NULL;
    _pEvent = NULL;
    _simple_image = false;
    _on_it = false;
    _click_on_short = false;
    _click_on_long = false;
    _alpha = SDL_ALPHA_OPAQUE;
}
Button::Button(string option,SDL_Event& event)
{
    _height = 40;
    _width = 100;
    _pos_x = 0;
    _pos_y = 0;
    _pos_th_x=0;
    _pos_th_y=0;
    _pos_option = 5;
    _nmb_movt=0;
    _alpha = SDL_ALPHA_OPAQUE;
    _on_it = false;
    _click_on_short = false;
    _click_on_long = false;
    _simple_image = false;

    _pos_text_x=0;
    _pos_text_y=0;
    _color2={255,0,0};
    _color={255,150,0};
    _path = "images/button_off.jpg";
    _path2 = "images/button_on.jpg";
    _text = option;
    _size = 18;
    _path_font = "fonts/Starjedi.ttf";

    _pEvent = &event;
    _font = TTF_OpenFont( _path_font.c_str(), _size );;
    _surface = load_image(_path);
    _surface2 = load_image(_path2);
    _text_surface=TTF_RenderText_Solid( _font, _text.c_str(), _color);
    _text_surface2=TTF_RenderText_Solid( _font, _text.c_str(), _color2);
    _width_text = _text_surface->w;
    _height_text = _text_surface->h;
}
Button::~Button()
{
    //TTF_CloseFont(_font);
    SDL_FreeSurface(_surface);
    SDL_FreeSurface(_surface2);
    SDL_FreeSurface(_text_surface);
    SDL_FreeSurface(_text_surface2);
}

Button::Button(const Button& other)
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
    _path = other._path;
    _simple_image = other._simple_image;
    _time_lag = other._time_lag;
    _transparency=other._transparency;
    _nb_moving=other._nb_moving;
    _end_x=other._end_x;
    _end_y=other._end_y;

    _pos_text_x=other._pos_text_x;
    _pos_text_y=other._pos_text_y;
    _color=other._color;
    _color2=other._color2;
    _pEvent=other._pEvent;
    _path2 = other._path2;
    _on_it = other._on_it;
    _click_on_long = other._click_on_long;
    _click_on_short = other._click_on_short;
    _text = other._text;
    _size = other._size;
    _path_font = other._path_font;

    _surface = load_image(_path);
    _surface2 = load_image(_path2);
    _font = NULL;
    _text_surface = TTF_RenderText_Solid( _font, _text.c_str(), _color);
    _text_surface2 = TTF_RenderText_Solid( _font, _text.c_str(), _color2);
}

Button& Button::operator=(const Button& rhs)
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
    _path = rhs._path;
    _simple_image = rhs._simple_image;
    _time_lag = rhs._time_lag;
    _transparency=rhs._transparency;
    _nb_moving=rhs._nb_moving;
    _end_x=rhs._end_x;
    _end_y=rhs._end_y;

    _pos_text_x=rhs._pos_text_x;
    _pos_text_y=rhs._pos_text_y;
    _color=rhs._color;
    _color2=rhs._color2;
    _pEvent=rhs._pEvent;
    _path2 = rhs._path2;
    _on_it = rhs._on_it;
    _click_on_long = rhs._click_on_long;
    _click_on_short = rhs._click_on_short;
    _text = rhs._text;
    _size = rhs._size;
    _path_font = rhs._path_font;

    _surface = load_image(_path);
    _surface2 = load_image(_path2);
    _font = NULL;
    _text_surface = TTF_RenderText_Solid( _font, _text.c_str(), _color);
    _text_surface2 = TTF_RenderText_Solid( _font, _text.c_str(), _color2);
    return *this;
}
void Button::Apply_surface(SDL_Surface* destination)
{
    position_image();
    if (_on_it)
    {
        apply_surface(_pos_x,_pos_y,_surface2,destination);
        apply_surface(_pos_text_x,_pos_text_y,_text_surface2,destination);
    }
    else
    {
        apply_surface(_pos_x,_pos_y,_surface,destination);
        apply_surface(_pos_text_x,_pos_text_y,_text_surface,destination);
    }
}
void Button::position_image()
{
    array<double,2> res = position_image_aux(_pos_th_x,_pos_th_y,_pos_option);
    _pos_x = res[0];
    _pos_y = res[1];

    _pos_text_x = _pos_x+_width/2.-_width_text/2.;
    _pos_text_y = _pos_y+_height/2.-_height_text/2.;
}
void Button::type(string text)
{
    if (text !=_text)
    {
        SDL_FreeSurface(_text_surface);
        SDL_FreeSurface(_text_surface2);
        _text = text;
        _text_surface=TTF_RenderText_Solid( _font, _text.c_str(), _color);
        _text_surface2=TTF_RenderText_Solid( _font, _text.c_str(), _color2);
        _width_text = _text_surface->w;
        _height_text = _text_surface->h;
        _pos_text_x = _pos_x+_width/2.-_width_text/2.;
        _pos_text_y = _pos_y+_height/2.-_height_text/2.;
    }
}
void Button::update()
{
    set_on_it();
    set_click_on_short();
    set_click_on_long();
}
