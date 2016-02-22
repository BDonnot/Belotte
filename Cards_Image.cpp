#include "Cards_Image.h"

using namespace std;


#if COMPLETE_GAME > 0
Cards_Image::Cards_Image(Uint height,Uint color,SDL_Event* event,SDL_Surface * pscreen):
    Basic_Transparency(SDL_ALPHA_OPAQUE),
    Basic_Click(event),
    Images_Sprite_Move<2>("images/sprite_carte (70-90).jpg"),
    _pScreen(pscreen)
{
    array<Uint,2> x = {0,80*(height+1)};
    array<Uint,2> y = {100*(color),100*(color)};
    array<Uint,2> width = {70,70};
    array<Uint,2> hei = {90,90};
    SetDimensionSprite(x,y,width,hei);
    ChangeSprite(0);
    _up = false;
    _half_duration = 0;
    _first_sprite = false;
}
void Cards_Image::Update_on_it()
{
	if (_pEvent->type == SDL_MOUSEMOTION)
	{
		Uint pos_mouse_x = _pEvent->motion.x;
		Uint pos_mouse_y = _pEvent->motion.y;
		const Uint& _pos_x = _pos.Getx();
		const Uint& _pos_y = _pos.Gety();
		if (_on_it&&!((pos_mouse_x>_pos_x) && (pos_mouse_x < _pos_x + _width) && (pos_mouse_y > _pos_y) && (pos_mouse_y < _pos_y + _height))) _on_it = false;
		else
		{
			if (!_on_it && ((pos_mouse_x>_pos_x) && (pos_mouse_x<_pos_x + _width) && (pos_mouse_y >_pos_y) && (pos_mouse_y<_pos_y + _height))) _on_it = true;
		}
	}
	Set_click_on_long();
	Set_click_on_short();
}

void Cards_Image::act()
{
	updateMouvement();
	transparency(); //computation of alpha
	SDL_SetAlpha(_surface.get(), SDL_SRCALPHA | SDL_RLEACCEL, static_cast<Uint8>(_alpha)); //apply the new level of transparency
	Move(); //computation of the new position
}
void Cards_Image::RevealCard(const PositionGraphic& pos, Uint32 duration, Uint32 time_lag)
{
	initMouvement(_sprite_number == 0, pos, duration, time_lag);
}

void Cards_Image::HideCard(const PositionGraphic& pos, Uint32 duration, Uint32 time_lag)
{
	initMouvement(_sprite_number == 1, pos, duration, time_lag);
}

void Cards_Image::initMouvement(bool transparency, PositionGraphic pos, Uint32 duration, Uint32 time_lag)
{
	Uint32 current_time = static_cast<Uint32>(_info.Time());
	if (transparency) Reveal(duration, time_lag, current_time);
	pos.SetProperPosition(_width, _height);
	Set_animate(pos, duration, time_lag);
}

void Cards_Image::UpdatePositionHand(PositionGraphic& pos_end)
{
	pos_end.SetProperPosition(_width, _height);
	if ((pos_end.Getx() != _pos.Getx()) || (pos_end.Gety() != _pos.Gety())) Set_animate(pos_end);
}

void Cards_Image::updateMouvement()
{
	_timer_alpha_current = static_cast<Uint32>(_info.Time());
	revealing();
}

void Cards_Image::revealing()
{
	if (_timer_alpha_current <= _timer_alpha_end) return;
	if (_first_sprite)
	{
		_first_sprite = false;
		ChangeSprite(1 - _sprite_number); //change the sprite 0 if the current sprite is one, 1 otherwise
		Set_Transparent((Uint8)SDL_ALPHA_OPAQUE, _half_duration, 0, _timer_alpha_current);
	}
	else
	{
		_alpha = SDL_ALPHA_OPAQUE;
	}
}
#else
void Cards_Image::RevealCard(const PositionGraphic& pos, Uint duration, Uint time_lag) {}
void Cards_Image::HideCard(const PositionGraphic& pos, Uint duration, Uint time_lag) {}
void Cards_Image::Update_on_it() {}
void Cards_Image::UpdatePositionHand(PositionGraphic& pos_end) {}
void Cards_Image::updateMouvement() {}
void Cards_Image::revealing() {}
void Cards_Image::act() {}
#endif //#if COMPLETE_GAME > 0

Cards_Image::Cards_Image(Uint height, Uint color) :
	Basic_Transparency( static_cast<Uint8>(ALPHA_OPAQUE) )
	,Basic_Click()
	,Images_Sprite_Move<2>("images/sprite_carte (70-90).jpg")
{
	array<Uint, 2> x = { 0,80 * (height + 1) };
	array<Uint, 2> y = { 100 * (color),100 * (color) };
	array<Uint, 2> width = { 70,70 };
	array<Uint, 2> hei = { 90,90 };
	SetDimensionSprite(x, y, width, hei);
	ChangeSprite(0);
	_up = false;
	_half_duration = 0;
	_first_sprite = false;
}

/*
void Cards_InitMouvement(const PositionGraphic& pos,Uint32 duration,Uint32 time_lag)
{
    initMouvement(false,pos,duration,time_lag);
}
*/


void Cards_Image::Up(bool go_up)
{
    if((go_up)&&(!_up))
    {
        _up = true;
        return;
    }
    if ((!go_up)&&(_up))
    {
        _up = false;
    }
}

void Cards_Image::Reveal(Uint duration, Uint time_lag, Uint current_time) //init the revealing of a card : first the back disapear slowly and then the front appear
{
    _first_sprite = true;
    _half_duration = duration /2;
    Set_Transparent((Uint8) 30,_half_duration,time_lag,_info.Time());
}

void Cards_Image::Reset()
{
    Reset_Click();
    _up = false;
}
