#include "Player_Name.h"
using namespace::std;


#if COMPLETE_GAME > 0

Player_Name::Player_Name(Basic_Images* fond,string name,string path_image,SDL_Event* pevent,Uint width,Uint height,PositionGraphic& pos):
Quit(pevent)
,_name("fonts/MLSGY.ttf"
      ,40
      ,255,255,0
      ,pevent
      ,name)
,_name_display("fonts/MLSGY.ttf",30,255,255,255,pevent,name)
,_what_name("fonts/MLSGY.ttf",40,255,255,255,"Quel nom pour ce joueur ?")
,_fond(fond)
,_image(path_image,pevent)
,_width(width)
,_height(height)
,_ok(pevent,"Ok",PositionGraphic())
,_return(pevent,"Retour",PositionGraphic())
{
    _name_origin = name;
    PositionGraphic pos2(_width/2,_height/2,CENTER);
    _name.SetPosition(pos2);
    pos2.Set(_width/2,_height/3,CENTER);
    _what_name.SetPosition(pos2);
    pos2.Set(2*_width/3,2*_height/3,CENTER);
    _return.SetPosition(pos2);
    pos2.Set(_width/3,2*_height/3,CENTER);
    _ok.SetPosition(pos2);
    _image.SetPosition(pos);
    //array<Uint16,2> pos2 = _image.GetPosition();
    array<Uint,2> size_ = _image.GetSize();
    pos.Set(pos.Getx()+size_[0]/2,pos.Gety()+size_[1]/2,CENTER);
    _name_display.SetPosition(pos);
}
bool Player_Name::WhatName(SDL_Surface* screen)
{
	bool continuer = true;
	bool changes = false;
	SDL_Event* pevent = _image.GetEvent();
	if (_image.Click(false))
	{
		while (continuer)
		{
			while (SDL_PollEvent(pevent))
			{
				continuer = continuer && (KeepLooping());
				_name.Update_Text_OnIt();
				_ok.Update_on_it();
				_return.Update_on_it();
			}
			if (_ok.Click(true) && continuer)
			{
				continuer = false;
				changes = true;
			}
			if (_return.Click(true) && continuer) continuer = false;
			_fond->Display(screen);
			_name.Display(screen);
			_image.Display(screen);
			_what_name.Display(screen);
			_ok.Display(screen);
			_return.Display(screen);
			SDL_Flip(screen);
		}
		ResetClick();
		if (changes)
		{
			string res = _name.GetText();
			if ((res != "") && (res != " "))
			{
				_name_display.Change_Text(res);
			}
		}
	}
	return continuer;
}

void Player_Name::Display(SDL_Surface* destination, bool choose_name)
{
	_image.Display(destination);
	_name_display.Display(destination);
	if (choose_name)
		WhatName(destination);
}
#endif //#if COMPLETE_GAME > 0
Player_Name::Player_Name(Basic_Images* fond
	, string name
	, string path_image
	, Uint width
	, Uint height
	, PositionGraphic& pos) :
	Quit()
	, _name("fonts/MLSGY.ttf"
		, 40
		, 255, 255, 0
		, name)
	, _name_display("fonts/MLSGY.ttf", 30, 255, 255, 255, name)
	, _what_name("fonts/MLSGY.ttf", 40, 255, 255, 255, "Quel nom pour ce joueur ?")
	, _fond(fond)
	, _image(path_image)
	, _width(width)
	, _height(height)
	, _ok("Ok", PositionGraphic())
	, _return("Retour", PositionGraphic())
{
	_name_origin = name;
	PositionGraphic pos2(_width / 2, _height / 2, CENTER);
	_name.SetPosition(pos2);
	pos2.Set(_width / 2, _height / 3, CENTER);
	_what_name.SetPosition(pos2);
	pos2.Set(2 * _width / 3, 2 * _height / 3, CENTER);
	_return.SetPosition(pos2);
	pos2.Set(_width / 3, 2 * _height / 3, CENTER);
	_ok.SetPosition(pos2);
	_image.SetPosition(pos);
	//array<Uint16,2> pos2 = _image.GetPosition();
	array<Uint, 2> size_ = _image.GetSize();
	pos.Set(pos.Getx() + size_[0] / 2, pos.Gety() + size_[1] / 2, CENTER);
	_name_display.SetPosition(pos);
}
Player_Name::Player_Name(string name
	, string path_image
	, Uint width
	, Uint height
	, PositionGraphic& pos) :
	Quit()
	, _name("fonts/MLSGY.ttf"
		, 40
		, 255, 255, 0
		, name)
	, _name_display("fonts/MLSGY.ttf", 30, 255, 255, 255, name)
	, _what_name("fonts/MLSGY.ttf", 40, 255, 255, 255, "Quel nom pour ce joueur ?")
	, _fond(nullptr)
	, _image(path_image)
	, _width(width)
	, _height(height)
	, _ok("Ok", PositionGraphic())
	, _return("Retour", PositionGraphic())
{
	_name_origin = name;
	PositionGraphic pos2(_width / 2, _height / 2, CENTER);
	_name.SetPosition(pos2);
	pos2.Set(_width / 2, _height / 3, CENTER);
	_what_name.SetPosition(pos2);
	pos2.Set(2 * _width / 3, 2 * _height / 3, CENTER);
	_return.SetPosition(pos2);
	pos2.Set(_width / 3, 2 * _height / 3, CENTER);
	_ok.SetPosition(pos2);
	_image.SetPosition(pos);
	//array<Uint16,2> pos2 = _image.GetPosition();
	array<Uint, 2> size_ = _image.GetSize();
	pos.Set(pos.Getx() + size_[0] / 2, pos.Gety() + size_[1] / 2, CENTER);
	_name_display.SetPosition(pos);
}

void Player_Name::Update(GAME_PHASES currentPhase)
{
    if (currentPhase != SELECT_NAMES) return;
    _name.Update_Text_OnIt();
    _image.Update_on_it();
}

void Player_Name::ResetClick()
{
    _name.Reset_Click();
    _image.Reset_Click();
    _return.Reset_Click();
    _ok.Reset_Click();
}

const std::string& Player_Name::Name() const
{
    return _name_display.GetText();
}
