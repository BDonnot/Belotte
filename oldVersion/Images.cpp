#include "SDL/SDL_image.h"
#include "Images.h"
using namespace::std;

SDL_Surface* Images::load_image( std::string filename )
{
    SDL_Surface* loadedImage = NULL;
    SDL_Surface* optimizedImage = NULL;
    loadedImage = IMG_Load( filename.c_str() );
    if( loadedImage != NULL )
    {
        optimizedImage = SDL_DisplayFormat( loadedImage );
        SDL_FreeSurface( loadedImage );
        if( optimizedImage != NULL )
        {
            SDL_SetColorKey( optimizedImage, SDL_SRCCOLORKEY, SDL_MapRGB( optimizedImage->format, 0,0xff,0xff ) );
        }
    }
    return optimizedImage;
}
Images::Images()
{
    _height = 0;
    _width = 0;
    _pos_x = 0;
    _pos_y = 0;
    _surface = NULL;
    _pos_th_x=0;
    _pos_th_y=0;
    _pos_option = 5;
    _path = "";
    _on_it = false;
    _click_on_short = false;
    _click_on_long = false;
    _pEvent = NULL;
    _simple_image = true;
    reset();
}
Images::Images(string path,SDL_Event& event)
{
    _surface = load_image(path);
    _height = _surface->w;
    _width = _surface->h;
    _pos_x = 0;
    _pos_y = 0;
    _pos_th_x=0;
    _pos_th_y=0;
    _pos_option = 5;
    _nmb_movt=0;
    _mov_x=0;
    _mov_y=0;
    _alpha = SDL_ALPHA_OPAQUE;
    _path = path;
    _on_it=false;
    _click_on_short = false;
    _click_on_long = false;
    _time_lag = 0;
    _transparency = 0;
    _pEvent = &event;
    _simple_image = true;
    _nmb_movt_alpha = 0;
}
Images::Images(int w, int h,string path,SDL_Event& event)
{
    _surface = load_image(path);
    _height = h;
    _width = w;
    _pos_x = 0;
    _pos_y = 0;
    _pos_th_x=0;
    _pos_th_y=0;
    _pos_option = 5;
    _nmb_movt=0;
    _alpha = SDL_ALPHA_OPAQUE;
    _path = path;
    _on_it = false;
    _click_on_short = false;
    _click_on_long = false;
    _time_lag = 0;
    _transparency = 0;
    _pEvent = &event;
    _end_x=0;
    _end_y=0;
    _simple_image = true;
}
Images::~Images()
{
    if (_simple_image) SDL_FreeSurface(_surface);
}
Images::Images(const Images& other)
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
    _path = other._path;
    _time_lag = other._time_lag;
    _transparency=other._transparency;
    _simple_image = other._simple_image;
    _click_on_long = other._click_on_long;
    _click_on_short = other._click_on_short;
    _nb_moving=other._nb_moving;
    _end_x=other._end_x;
    _end_y=other._end_y;

    _pEvent = other._pEvent;
    _surface = load_image(_path);
}
Images& Images::operator= (const Images& rhs)
{
    if (this == &rhs) return *this;
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
    _on_it = rhs._on_it;
    _path = rhs._path;
    _time_lag = rhs._time_lag;
    _transparency=rhs._transparency;
    _simple_image = rhs._simple_image;
    _click_on_long = rhs._click_on_long;
    _click_on_short = rhs._click_on_short;
    _nb_moving=rhs._nb_moving;
    _end_x=rhs._end_x;
    _end_y=rhs._end_y;

    _pEvent = rhs._pEvent;
    _surface = load_image(_path);
    return *this;
}
SDL_Surface* Images::GetSurface()
{
    return _surface;
}
void Images::SetPosition(double x,double y,int option)
{
    _pos_th_x =x;
    _pos_th_y = y;
    _pos_option = option;
}
array<double,2> Images::position_image_aux(double pos_x,double pos_y,int pos_option)
{
    array<double,2> res;
    double res_x,res_y;
    double width = double(_width);
    double height = double(_height);
    switch (pos_option)
    {
        case 0 :
        res_x = pos_x-width*.5;
        res_y = pos_y-height*.5;
        break;

        case 1 :
        res_x = pos_x;
        res_y = pos_y-height*.5;
        break;

        case 2 :
        res_x = pos_x-width;
        res_y = pos_y-height*.5;
        break;

        case 3 :
        res_x = pos_x-width*.5;
        res_y = pos_y;
        break;

        case 4 :
        res_x = pos_x-width*.5;
        res_y = pos_y-height;
        break;

        case 5 :
        res_x = pos_x;
        res_y = pos_y;
        break;

        default : break;
    }
    res.at(0) = res_x;
    res.at(1) = res_y;
    return res;
}
void Images::position_image()
{
    const array<double,2>& res = position_image_aux(static_cast<double>(_pos_th_x),static_cast<double>(_pos_th_y),_pos_option);
    _pos_x = (Uint16) res[0];
    _pos_y = (Uint16) res[1];
}
void Images::apply_surface( int x, int y, SDL_Surface* source, SDL_Surface* destination )
{
    SDL_Rect offset;
    offset.x = x;
    offset.y = y;
    SDL_BlitSurface( source, NULL, destination, &offset );
}
void Images::Apply_surface(SDL_Surface* destination)
{
    if (_nmb_movt==0) position_image();
    else animate();
    transparent();
    if (_surface !=NULL) apply_surface(_pos_x,_pos_y,_surface,destination);
}
void Images::Set_animate_aux(double end_x_th, double end_y_th,int option,int speed,int time_lag)
{
    position_image();
    double nb_moving = choose_nb_movt(speed);
    const array<double,2>& coord = position_image_aux(end_x_th,end_y_th,option);
    _end_x = coord[0];
    _end_y = coord[1];
    _time_lag = time_lag;
    _nb_moving = nb_moving;
    _mov_x = (_end_x - _pos_x)/nb_moving;
    _mov_y = (_end_y - _pos_y)/nb_moving;
    _nmb_movt=1;
}
void Images::Set_animate(double end_x_th, double end_y_th,int option,int speed,int time_lag)
{
    _transparency = 0;
    Set_animate_aux(end_x_th,end_y_th,option,speed,time_lag);
}
void Images::animate()
{
    if (_nmb_movt == _time_lag) position_image();
    if (_nmb_movt <= _time_lag) _nmb_movt++;
    else
    {
         if (_nmb_movt < _nb_moving+_time_lag)
            {
                _nmb_movt++;
                _pos_x += _mov_x;
                _pos_y += _mov_y;
            }
        else {
            if (_nmb_movt==_nb_moving+_time_lag)
            {
                _nmb_movt++;
                _pos_x = _end_x;
                _pos_y = _end_y;
            }}
    }
    if (_nmb_movt > _time_lag+_nb_moving) //we update the position when the movement is finished
    {
        _pos_th_x = _end_x;
        _pos_th_y = _end_y;
        _pos_option = 5;
        _nmb_movt=0;
    }
}

 bool Images::Arrived()
{
    return (_nmb_movt > _nb_moving+_time_lag);
}
void Images::Set_animate_transparency(double end_x_th, double end_y_th,int option,int speed,bool rising,int time_lag)
{
    Set_transparent(rising,speed,time_lag);
    Set_animate_aux(end_x_th,end_y_th,option,speed,time_lag);
}
int Images::choose_nb_movt(int speed)
{
    int nb_moving = 0;
    switch (speed)
    {
        case 0 :
            nb_moving = 50;
            break;
        case 1 :
            nb_moving = 80;
            break;
        case 2 :
            nb_moving = 100;
            break;
        case 3 :
            nb_moving = 200;
            break;
        case 4 :
            nb_moving = 1000;
            break;
        default :
        nb_moving = 100;
        break;
    }
    return nb_moving;
}
void Images::set_on_it()
{
    int pos_mouse_x = -1;
    int pos_mouse_y = -1;
    if( _pEvent->type == SDL_MOUSEMOTION )
    {
        pos_mouse_x = _pEvent->motion.x;
        pos_mouse_y = _pEvent->motion.y;
        if (_on_it&&!((pos_mouse_x>_pos_x)&&( pos_mouse_x < _pos_x+_width)&&( pos_mouse_y > _pos_y)&&(pos_mouse_y < _pos_y+_height))) _on_it = false;
        else {
        if (!_on_it&&((pos_mouse_x>_pos_x)&&( pos_mouse_x<_pos_x+_width)&&(pos_mouse_y >_pos_y)&&(pos_mouse_y<_pos_y+_height))) _on_it = true;
        }
    }
}
bool Images::Mouse_over()
{
    return _on_it;
}
void Images::set_click_on_long()
{
    if (_click_on_long)  _click_on_long=(_on_it||!(_pEvent->type == SDL_MOUSEBUTTONDOWN));
    else _click_on_long=(_on_it&&(_pEvent->type==SDL_MOUSEBUTTONDOWN));
}
void Images::set_click_on_short()
{
    _click_on_short=(_on_it&&(_pEvent->type==SDL_MOUSEBUTTONDOWN));
}
bool Images::click_on(bool Short)
{
    if (Short) return _click_on_short;
    return _click_on_long;
}
void Images::update()
{
    set_on_it();
    set_click_on_short();
    set_click_on_long();
}
bool Images::Moving()
{
    return !_nmb_movt==0;
}
void Images::Set_transparent(bool rising,int speed,int time_lag)
{
    _nb_moving = choose_nb_movt(speed);
    _nmb_movt_alpha =1;
    if (rising)
    {
        _alpha = SDL_ALPHA_TRANSPARENT;
        _transparency = 1;
        SDL_SetAlpha( _surface, SDL_SRCALPHA | SDL_RLEACCEL, _alpha );
    }
    else
    {
        _alpha = SDL_ALPHA_OPAQUE;
        _transparency = -1;
    }
    _time_lag = time_lag;
}
void Images::transparent()
{
    if (_transparency !=0)
    {
        _nmb_movt_alpha++;
        if (_nmb_movt_alpha >= _time_lag) calcule_alpha(_transparency*255./_nb_moving);
    }
}
void Images::calcule_alpha(double increase)
{
    _alpha += increase;
    if ( _alpha < (double) SDL_ALPHA_TRANSPARENT)
    {
        _alpha = 0;
        _transparency = 0;
    }
    else { if ( _alpha > (double) SDL_ALPHA_OPAQUE)
    {
        _alpha = 255;
        _transparency = 0;
    }}
    SDL_SetAlpha( _surface, SDL_SRCALPHA | SDL_RLEACCEL, _alpha );
}
void Images::type(string a)
{
    //do nothing
}
double Images::GetAlpha()
{
    return _alpha;
}
void Images::reset()
{
    _nmb_movt=0;
    _alpha = SDL_ALPHA_OPAQUE;
    if (_surface !=NULL) SDL_SetAlpha( _surface, SDL_SRCALPHA | SDL_RLEACCEL, _alpha );
    _time_lag = 0;
    _transparency = 0;
    _nmb_movt_alpha = 0;
    _nb_moving = 0;
    _click_on_short=false;
    _click_on_long=false;
    _on_it = false;
}
void Images::Move_down()
{
    _pos_th_y += 45;
    position_image();
}
void Images::Move_up()
{
    _pos_th_y -= 45;
    position_image();
}
void Images::down_transparent()
{
    if (_alpha >= 100) _alpha -= 100;
    if (_surface !=NULL) SDL_SetAlpha( _surface, SDL_SRCALPHA | SDL_RLEACCEL, _alpha );
}
void Images::up_transparent()
{
    if (_alpha <=155) _alpha = SDL_ALPHA_OPAQUE;
    if (_surface !=NULL) SDL_SetAlpha( _surface, SDL_SRCALPHA | SDL_RLEACCEL, _alpha );
}
void Images::restart_click()
{
    _click_on_short=false;
    _click_on_long=false;
}
