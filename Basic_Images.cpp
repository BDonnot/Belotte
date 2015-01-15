#include "Basic_Images.h"
#include "SDL/SDL_image.h"
using namespace::std;
Basic_Images::Basic_Images():
_info()
,_height(0)
,_width(0)
,_pos(0,0,TOP_LEFT)
,_surface(nullptr)
{
    //_pos_x = 0;
    //_pos_y = 0;
    //_pos_option = CENTER;
    _height = 0;
    _width = 0;
}

Basic_Images::~Basic_Images()
{
//    if (_surface != NULL) SDL_FreeSurface(_surface);
}
Basic_Images::Basic_Images(std::string path):
_info()
,_height(0)
,_width(0)
,_pos(0,0,TOP_LEFT)
,_surface(nullptr)
{
    //_pos_x = 0;
    //_pos_y = 0;
    //_pos_option = CENTER;
    if (path != "")
    {
        _surface = move(SMARTSurface(load_image(path)));
        _height = _surface->h;
        _width = _surface->w;
    }
    else
    {
        _surface = nullptr;
        _height = 0;
        _width = 0;
    }

}
Basic_Images::Basic_Images(const Basic_Images& other)
{
    //copy ctor
}

Basic_Images& Basic_Images::operator=(const Basic_Images& rhs)
{
    if (this == &rhs) return *this; // handle self assignment
    //assignment operator
    return *this;
}
SDL_Surface* Basic_Images::load_image( std::string filename )
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

const PositionGraphic& Basic_Images::GetPosition() const
{
    return _pos;
}
array<Uint,2> Basic_Images::GetSize()
{
    array<Uint,2> res;
    res.at(0) = _width;
    res.at(1) = _height;
    return res;
}
void Basic_Images::SetPosition(PositionGraphic& pos)
{
    pos.SetProperPosition(_width,_height);
    _pos = pos;
}
void Basic_Images::Display(SDL_Surface* destination)
{
    act();
    if (_surface) apply_surface(_pos.Getx(),_pos.Gety(),_surface,destination);
}
void Basic_Images::apply_surface(const Uint& x,const Uint& y,const SMARTSurface& source, SDL_Surface* destination )
{
    SDL_Rect offset;
    offset.x = x;
    offset.y = y;
    SDL_BlitSurface( source.get(), nullptr, destination, &offset );
}
void Basic_Images::act(void)
{
    //do nothing here
}

const SDL_Surface* Basic_Images::Get_Surface() const
{
    return _surface.get();
}
