#include "Cards.h"

using namespace::std;
/*
//array<std::array<bool,8>,8 > Cards::_matrix_win_trump = {false,false,false,false,false,false,false,false
//                                                        ,true,false,false,false,false,false,false,false
//                                                        ,true,true,false,true,false,true,true,true
//                                                        ,true,true,false,false,false,true,true,false
//                                                        ,true,true,true,true,false,true,true,true
//                                                        ,true,true,false,false,false,false,false,false
//                                                        ,true,true,false,false,false,true,false,false
//                                                        ,true,true,false,true,false,true,true,false};
//array<std::array<bool,8>,8 > Cards::_matrix_win_no_trump = {false,false,false,false,false,false,false,false,true,false,false,false,false,false,false,false,true,true,false,false,false,false,false,false,true,true,true,false,true,true,true,false,true,true,true,false,false,false,false,false,true,true,true,false,true,false,false,false,true,true,true,false,true,true,false,false,true,true,true,true,true,true,true,false};
std::array<unsigned int,8> Cards::_value_trump = {0,0,14,10,20,3,4,11};
std::array<unsigned int,8> Cards::_value_no_trump = {0,0,0,10,2,3,4,11};

bool Cards::Win(CARDS_HEIGHT heightOther) const //
{
    Uint height_card = HeightToInt(_height_card);
    Uint height_other = HeightToInt(heightOther);
    if (_info.TrumpColor()==_color_card)
        return _matrix_win_trump[height_card][height_other];
    return _matrix_win_no_trump[height_card][height_other];
}
Uint Cards::HeightToInt(CARDS_HEIGHT height_card) const
{
    switch(height_card)
    {
        case SEVEN :
            return 0;
        case EIGHT :
            return 1;
        case NINE :
            return 2;
        case TEN :
            return 3;
        case JACK :
            return 4;
        case QUEEN :
            return 5;
        case KING :
            return 6;
        case ACE :
            return 7;
        default :
            return 0;
    }
}
*/

Cards::Cards(CARDS_HEIGHT height, CARDS_COLOR color,SDL_Event* event):
Cards_Basic(height,color),
Images_Sprite_Move<2>("images/sprite_carte (70-90).jpg"),
Basic_Click(event),
Basic_Transparency(SDL_ALPHA_OPAQUE)
{
    array<Uint,2> x = {0,80*(static_cast<Uint>(height)+1)};
    array<Uint,2> y = {100*static_cast<Uint>(color),100*static_cast<Uint>(color)};
    array<Uint,2> width = {70,70};
    array<Uint,2> hei = {90,90};
    SetDimensionSprite(x,y,width,hei);
    ChangeSprite(0);
    _Up = false;
    _half_duration = 0;
    _first_sprite = false;
}

Cards::Cards(const Cards& other)
{
    //copy ctor
}

Cards& Cards::operator=(const Cards& rhs)
{
    if (this == &rhs) return *this; // handle self assignment
    //assignment operator
    return *this;
}
void Cards::Update_on_it()
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
void Cards::act()
{
    updateMouvement();
    Transparency(); //calcul of alpha
    SDL_SetAlpha( _surface.get(), SDL_SRCALPHA | SDL_RLEACCEL, _alpha ); //apply the new level of transparency
//    if(_Up)  _pos_end.Gety() -= 50;
    Move(); //calcul of the new position
}
void Cards::InitMouvement(bool transparency,PositionGraphic pos,Uint32 duration,Uint32 time_lag)
{
    Uint32 current_time = _info.Time();
    if (transparency) Reveal(duration,time_lag,current_time);
    pos.SetProperPosition(_width,_height);
    Set_animate(pos,duration,time_lag);
}
void Cards::UpdatePositionHand(PositionGraphic& pos_end)
{
    pos_end.SetProperPosition(_width,_height);
    if ((pos_end.Getx() != _pos.Getx())||(pos_end.Gety() != _pos.Gety())) Set_animate(pos_end);
}
void Cards::updateMouvement()
{
    _timer_alpha_current = _info.Time();
    Revealing();
}
void Cards::Revealing()
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
void Cards::Up(bool go_up)
{
    if((go_up)&&(!_Up))
    {
        _Up = true;
//        _pos_end.Gety() -= 50;
        return;
    }
    if ((!go_up)&&(_Up))
    {
        _Up = false;
//        _pos_end.Gety() -= 50;
    }
}
void Cards::Reveal(Uint32 duration,Uint32 time_lag,Uint32 current_time) //init the revealing of a card : first the back disapear slowly and then the front appear
{
    _first_sprite = true;
//    Uint32 half_duration = duration /2;
    _half_duration = duration /2;
    Set_Transparent((Uint8) 30,_half_duration,time_lag,_info.Time());
}
const bool& Cards::GetUp() const
{
    return _Up;
}
void Cards::Reset()
{
    Reset_Click();
    _Up = false;
}
