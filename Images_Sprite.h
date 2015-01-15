#ifndef IMAGES_SPRITE_H
#define IMAGES_SPRITE_H

#include <array>
//#include <vector>

#include "Basic_Images.h"

//TO DO : a short description of this class.
//TO DO : inlined the function that needs to be.
template<Uint numberOfSprite>
class Images_Sprite: public Basic_Images
{
     protected :
        SDL_Rect _clip[numberOfSprite];
        Uint _sprite_number; //number of the sprite in the sheet currently used

    public:
        Images_Sprite();
        Images_Sprite(std::string path);
        virtual ~Images_Sprite(){}
        virtual void Display(SDL_Surface* destination);
        virtual void SetPosition(PositionGraphic& pos);
        void ChangeSprite(Uint sprite_number,PositionGraphic& pos); //to change the sprite displayed, and ajust its position on the screen
        void ChangeSprite(Uint sprite_number); //change the sprite currently used
        void SetDimensionSprite(const std::array<Uint,numberOfSprite>& coord_x
                                  ,const std::array<Uint,numberOfSprite>& coord_y
                                  ,const std::array<Uint,numberOfSprite>& dimension_w
                                  ,const std::array<Uint,numberOfSprite>& dimension_h); //set the dimension of the sprite sheet

        void apply_surface( Uint x, Uint y, const SMARTSurface& source, SDL_Surface* destination, SDL_Rect* clip); //overriding of the method for applying the right surface on the screen
        Uint GetSpriteNumber(){return _sprite_number;}
    private:
};

template<Uint numberOfSprite>
Images_Sprite<numberOfSprite>::Images_Sprite(std::string path):
Basic_Images(path)
,_sprite_number(0)
{

}

template<Uint numberOfSprite>
Images_Sprite<numberOfSprite>::Images_Sprite():
_sprite_number(0)
{

}

template<Uint numberOfSprite>
void Images_Sprite<numberOfSprite>::Display(SDL_Surface* destination)
{
    act();
    if (_surface !=nullptr) apply_surface( _pos.Getx(), _pos.Gety(), _surface, destination, &(_clip[_sprite_number]) );
}

template<Uint numberOfSprite>
void Images_Sprite<numberOfSprite>::SetDimensionSprite(const std::array<Uint,numberOfSprite>& coord_x
                                                          ,const std::array<Uint,numberOfSprite>& coord_y
                                                          ,const std::array<Uint,numberOfSprite>& dimension_w
                                                          ,const std::array<Uint,numberOfSprite>& dimension_h)
{
     for (int i = 0; i < numberOfSprite; i++)
    {
        _clip[i].x = coord_x[i];
        _clip[i].y = coord_y[i];
        _clip[i].w = dimension_w[i];
        _clip[i].h = dimension_h[i];
    }
}

template<Uint numberOfSprite>
void Images_Sprite<numberOfSprite>::apply_surface( Uint x, Uint y, const SMARTSurface& source, SDL_Surface* destination, SDL_Rect* clip )
{
    SDL_Rect offset;
    offset.x = x;
    offset.y = y;
    SDL_BlitSurface( source.get(), clip, destination, &offset );
}

template<Uint numberOfSprite>
void Images_Sprite<numberOfSprite>::ChangeSprite(Uint sprite_number,PositionGraphic& pos)
{
    _sprite_number = sprite_number;
    SetPosition(pos);
}

template<Uint numberOfSprite>
void Images_Sprite<numberOfSprite>::ChangeSprite(Uint sprite_number)
{
    _sprite_number = sprite_number;
    _height = _clip[_sprite_number].h;
    _width = _clip[_sprite_number].w;
}

template<Uint numberOfSprite>
void Images_Sprite<numberOfSprite>::SetPosition(PositionGraphic& pos)
{
    _height = _clip[_sprite_number].h;
    _width = _clip[_sprite_number].w;
    pos.SetProperPosition(_width,_height);
    _pos = pos;
}

#endif // IMAGES_SPRITE_H
