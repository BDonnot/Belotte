//#include "Images_Sprite.h"
//using namespace::std;
//
//Images_Sprite::Images_Sprite(string path, int number_sprite):
//    Basic_Images(path)
//{
//    _number_sprite = number_sprite;
//    _clip = new SDL_Rect[_number_sprite];
//    _sprite_number = 0;
//}
//Images_Sprite::Images_Sprite()
//{
//    _clip = NULL;
//}
//Images_Sprite::~Images_Sprite()
//{
//    //dtor
//    if(_clip)
//        delete _clip;
//}
//void Images_Sprite::Display(SDL_Surface* destination)
//{
//    act();
//    if (_surface !=NULL) apply_surface( _pos.Getx(), _pos.Gety(), _surface, destination, &(_clip[_sprite_number]) );
//}
//void Images_Sprite::Set_dimension_sprite(const vector<int>& dimension)
//{
//    for (int i = 0; i < _number_sprite; i++)
//    {
//        _clip[i].x = dimension[4*i];
//        _clip[i].y = dimension[4*i+1];
//        _clip[i].w = dimension[4*i+2];
//        _clip[i].h = dimension[4*i+3];
//    }
//}
//void Images_Sprite::Set_dimension_sprite(const vector<int>& coord, const vector<int>& dimension)
//{
//    for (int i = 0; i < _number_sprite; i++)
//    {
//        _clip[i].x = coord[2*i];
//        _clip[i].y = coord[2*i+1];
//        _clip[i].w = dimension[2*i];
//        _clip[i].h = dimension[2*i+1];
//    }
//}
//void Images_Sprite::Set_dimension_sprite(const vector<int>& coord_x,const vector<int>& coord_y,const vector<int>& dimension_w,const vector<int>& dimension_h)
//{
//     for (int i = 0; i < _number_sprite; i++)
//    {
//        _clip[i].x = coord_x[i];
//        _clip[i].y = coord_y[i];
//        _clip[i].w = dimension_w[i];
//        _clip[i].h = dimension_h[i];
//    }
//}
//void Images_Sprite::apply_surface( Uint x, Uint y, const SMARTSurface& source, SDL_Surface* destination, SDL_Rect* clip )
//{
//    SDL_Rect offset;
//    offset.x = x;
//    offset.y = y;
//    SDL_BlitSurface( source.get(), clip, destination, &offset );
//}
///*
//void Images_Sprite::Change_sprite(int sprite_number,Uint16 pos_x,Uint16 pos_y,POSITION_IMAGE option)
//{
//    _sprite_number = sprite_number;
//    _height = _clip[_sprite_number].h;
//    _width = _clip[_sprite_number].w;
//    _pos_option = option;
//    array<Uint16,2> res = position_image_aux();
//    _pos_x = pos_x + res[0];
//    _pos_y = pos_y + res[1];
//}
//*/
//void Images_Sprite::Change_sprite(int sprite_number,PositionGraphic& pos)
//{
//    _sprite_number = sprite_number;
//    _height = _clip[_sprite_number].h;
//    _width = _clip[_sprite_number].w;
//    pos.SetProperPosition(_width,_height);
//    _pos = pos;
//}
//void Images_Sprite::Change_sprite(int sprite_number)
//{
//    _sprite_number = sprite_number;
//    _height = _clip[_sprite_number].h;
//    _width = _clip[_sprite_number].w;
//}
///*
//void Images_Sprite::SetPosition(Uint16 x,Uint16 y,POSITION_IMAGE option)
//{
//    _height = _clip[_sprite_number].h;
//    _width = _clip[_sprite_number].w;
//    _pos_option = option;
//    array<Uint16,2> res = position_image_aux();
//    _pos_x = x+res[0];
//    _pos_y = y+res[1];
//}
//*/
//void Images_Sprite::SetPosition(PositionGraphic& pos)
//{
//    _height = _clip[_sprite_number].h;
//    _width = _clip[_sprite_number].w;
//    pos.SetProperPosition(_width,_height);
//    _pos = pos;
//}
