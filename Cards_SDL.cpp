#include "Cards_SDL.h"

using namespace std;
Cards_SDL::Cards_SDL(Uint height,Uint color,SDL_Event* event,SDL_Surface * pscreen):
    Basic_Transparency(SDL_ALPHA_OPAQUE),
    Basic_Click(event),
    Images_Sprite_Move<2>("images/sprite_carte (70-90).jpg"),
    _pScreen(pscreen)
{
    array<Uint,2> x = {0,80*(static_cast<Uint>(height)+1)};
    array<Uint,2> y = {100*static_cast<Uint>(color),100*static_cast<Uint>(color)};
    array<Uint,2> width = {70,70};
    array<Uint,2> hei = {90,90};
    SetDimensionSprite(x,y,width,hei);
    ChangeSprite(0);
    _up = false;
    _half_duration = 0;
    _first_sprite = false;
}

Cards_SDL::~Cards_SDL()
{
    //dtor
}

void Cards_SDL::Update_on_it()
{
    if( _pEvent->type == SDL_MOUSEMOTION )
    {
        Uint pos_mouse_x = _pEvent->motion.x;
        Uint pos_mouse_y = _pEvent->motion.y;
        const Uint& _pos_x = _pos.Getx();
        const Uint& _pos_y = _pos.Gety();
        if (_on_it&&!((pos_mouse_x>_pos_x)&&( pos_mouse_x < _pos_x+_width)&&( pos_mouse_y > _pos_y)&&(pos_mouse_y < _pos_y+_height))) _on_it = false;
        else
        {
            if (!_on_it&&((pos_mouse_x>_pos_x)&&( pos_mouse_x<_pos_x+_width)&&(pos_mouse_y >_pos_y)&&(pos_mouse_y<_pos_y+_height))) _on_it = true;
        }
    }
    Set_click_on_long();
    Set_click_on_short();
}
void Cards_SDL::act()
{
    updateMouvement();
    transparency(); //calcul of alpha
    SDL_SetAlpha( _surface.get(), SDL_SRCALPHA | SDL_RLEACCEL, _alpha ); //apply the new level of transparency
//    if(_Up)  _pos_end.Gety() -= 50;
    Move(); //calcul of the new position
}
void Cards_SDL::InitMouvement(bool transparency,PositionGraphic pos,Uint32 duration,Uint32 time_lag)
{
    Uint32 current_time = _info.Time();
    if(transparency && (GetSpriteNumber() ==  0)) Reveal(duration,time_lag,current_time);
    pos.SetProperPosition(_width,_height);
    Set_animate(pos,duration,time_lag);
}
void Cards_SDL::UpdatePositionHand(PositionGraphic& pos_end)
{
    pos_end.SetProperPosition(_width,_height);
    if ((pos_end.Getx() != _pos.Getx())||(pos_end.Gety() != _pos.Gety())) Set_animate(pos_end);
}
void Cards_SDL::updateMouvement()
{
    _timer_alpha_current = _info.Time();
    revealing();
}
void Cards_SDL::revealing()
{
    if(_timer_alpha_current <= _timer_alpha_end) return;
    if (_first_sprite)
    {
        _first_sprite = false;
        ChangeSprite(1-_sprite_number); //change the sprite 0 if the current sprite is one, 1 otherwise
        Set_Transparent((Uint8) SDL_ALPHA_OPAQUE,_half_duration,0,_timer_alpha_current);
    }
    else
    {
        _alpha = SDL_ALPHA_OPAQUE;
    }
}
void Cards_SDL::Up(bool go_up)
{
    if((go_up)&&(!_up))
    {
        _up = true;
//        _pos_end.Gety() -= 50;
        return;
    }
    if ((!go_up)&&(_up))
    {
        _up = false;
//        _pos_end.Gety() -= 50;
    }
}
void Cards_SDL::Reveal(Uint32 duration,Uint32 time_lag,Uint32 current_time) //init the revealing of a card : first the back disapear slowly and then the front appear
{
    _first_sprite = true;
//    Uint32 half_duration = duration /2;
    _half_duration = duration /2;
    Set_Transparent((Uint8) 30,_half_duration,time_lag,_info.Time());
}
const bool Cards_SDL::GetUp() const
{
    return _up;
}
void Cards_SDL::Reset()
{
    Reset_Click();
    _up = false;
}
/*
void Cards_SDL::Display()
{
    this->Display(_pScreen);
}
*/

