#include "Images_Button.h"
using namespace::std;
using namespace SDL;

#if COMPLETE_GAME > 0
Images_Button::Images_Button(SDL_Event* event
                             ,std::string text_button
                             ,PositionGraphic pos):
Images_Sprite<2>("images/Sprite_Button.jpg")
,Basic_Click(event)
,_text_on("fonts/MLSGY.ttf",24,255,255,255, text_button)
,_text_off("fonts/MLSGY.ttf",24,255,255,255, text_button)
,_persistant(false)
{
    array<Uint,2> x = {110,0};
    array<Uint,2> y = {0,0};
    array<Uint,2> width = {100,100};
    array<Uint,2> height = {40,40};
    SetDimensionSprite(x,y,width,height);
    ChangeSprite(0);
    SetPosition(pos);
}

Images_Button::Images_Button(SDL_Event* event
                             ,PositionGraphic pos
                             ,std::string path
                             ,int color):
Images_Sprite<2>(path)
,Basic_Click(event)
,_persistant(true)
{
    Uint coord_x[4][2] = {{1,1},{64,64},{129,129},{197,197}};
    array<Uint,2> x = {coord_x[color][0],coord_x[color][1]};
    array<Uint,2> y = {72,1};
    array<Uint,2> width = {51,51};
    array<Uint,2> height = {51,51};
    SetDimensionSprite(x,y,width,height);
    ChangeSprite(0);
    SetPosition(pos);
}

void Images_Button::Display(SDL_Surface* destination)
{
	act();
	if (_surface != NULL) apply_surface(_pos.Getx(), _pos.Gety(), _surface, destination, &(_clip[_sprite_number]));
	if (!_persistant)
	{
		if (_sprite_number == 0) _text_on.Display(destination);
		else _text_off.Display(destination);
	}
}
void Images_Button::Update_on_it()
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
#else
void Images_Button::Update_on_it()
{
	_on_it = true;
}
#endif //#if COMPLETE_GAME > 0

void Images_Button::PositionText()
{
     PositionGraphic pos_text(_pos.Getx()+_width/2,_pos.Gety()+_height/2,CENTER);
    _text_on.SetPosition(pos_text);
    _text_off.SetPosition(pos_text);
}

void Images_Button::act()
{
    bool test = false;
    if(!_persistant) test = _on_it;
    else test = _click_on_long;

    if (test) _sprite_number = 0;
    else _sprite_number = 1;
}

void Images_Button::SetPosition(PositionGraphic& pos)
{
    _height = _clip[_sprite_number].h;
    _width = _clip[_sprite_number].w;

    pos.SetProperPosition(_width,_height);
    _pos = pos;
    if(!_persistant)
        PositionText();
}
