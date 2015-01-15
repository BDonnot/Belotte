#include "Noms_Cartes.h"
#include "Carte.h"
#include <fstream>
using namespace std;

Carte::Carte()
{
    _colour=-1;
    _height=-1;
}
Carte::Carte(int colour,int height,SDL_Event* pevent,int w,int h):
_colour(colour),_height(height),_recto(73,97,path_image(),*pevent),_verso(73,97,"images/back.jpg",*pevent)
{
    //_colour = colour;
    //_height = height;
    _pEvent = pevent;
    /*Images recto(73,97,path_image(),*_pEvent);
    _recto = recto;
    Images verso(73,97,"images/back.jpg",*_pEvent);
    _verso = verso;*/
    _SCREEN_HEIGHT=h;
    _SCREEN_WIDTH=w;
    _has_been_played = false;
    _display_recto = false;
    _down = true;
    _transparent_down = false;
}
Carte::~Carte()
{
}
SDL_Surface* Carte::GetRecto_surface()
{
    return _recto.GetSurface();
}
bool Carte::IsTrump(int colour_trump)
{
    return _colour==colour_trump;
}

int Carte::GetColour()
{
    return _colour;
}

int Carte::GetHeight()
{
    return _height;
}
string Carte::Colour()
{
    string res = Name_Colour[_colour];
    return res;
}
string Carte::Height()
{
    string res = Name_Height[_height];
    return res;
}
string Carte::display()
{
    return Height()+" de "+Colour();
}

bool Carte::is_equal(Carte &c)
{
    Carte *pc=&c;
    Carte *pt=this;
    return (pc==pt);
}

bool Carte::operator== (Carte &c)
{
    return is_equal(c);
}
int Carte::Nb_Point(int colour_trump)
{
    if (IsTrump(colour_trump)) return Value_Trump[_height];
    else return Value_No_Trump[_height];
}
bool Carte::operator!= (Carte &c)
{
    return !(c==*this);
}

array<bool,64> Carte::If_Trump()
{
    array<bool,64> res = {false,false,false,false,false,false,false,false,true,false,false,false,false,false,false,false,true,true,false,true,false,true,true,true,true,true,false,false,false,true,true,false,true,true,true,true,false,true,true,true,true,true,false,false,false,false,false,false,true,true,false,false,false,true,false,false,true,true,false,true,false,true,true,false};
    return res;
}
array<bool,64> Carte::No_Trump() //No_trump[height_1+8*height_2] : return true if height_2 wins over height_1
{
    array<bool,64> res = {false,false,false,false,false,false,false,false,true,false,false,false,false,false,false,false,true,true,false,false,false,false,false,false,true,true,true,false,true,true,true,false,true,true,true,false,false,false,false,false,true,true,true,false,true,false,false,false,true,true,true,false,true,true,false,false,true,true,true,true,true,true,true,false};
    return res;
}
int Carte::convertion(int colour_trump) //To convert the height of a card in winning position in her colour
{
    if (_colour == colour_trump)
    {
        switch (_height)
        {
            case 0 : return 8; //7
            case 1 : return 7; //8
            case 2 : return 2; //9
            case 3 : return 4; //10
            case 4 : return 1; //jack
            case 5 : return 6; //queen
            case 6 : return 5; //king
            case 7 : return 3; //ace
            default : break;
        }
    }
    else
    {
        switch (_height)
        {
            case 0 : return 8; //7
            case 1 : return 7; //8
            case 2 : return 6; //9
            case 3 : return 2; //10
            case 4 : return 5; //jack
            case 5 : return 4; //queen
            case 6 : return 3; //king
            case 7 : return 1; //ace
            default : break;
        }
    }
    fstream infile;
    infile.open("datas/erreurs.txt", fstream::in | fstream::out | fstream::app);
    infile << "Erreur convertion, la couleur rentree etait " << colour_trump <<endl;
    infile.close();
    return -1;
}
//SDL
string Carte::path_image()
{
    string res ="images/";
    switch(_height)
    {
        case 0: res+="7"; break;
        case 1: res+="8"; break;
        case 2: res+="9"; break;
        case 3: res+="t"; break;
        case 4: res+="j"; break;
        case 5: res+="q"; break;
        case 6: res+="k"; break;
        case 7: res+="a"; break;
        default : break;
    }
    switch (_colour)
    {
        case 0: res+="d"; break;
        case 1: res+="h"; break;
        case 2: res+="s"; break;
        case 3: res+="c"; break;
        default : break;
    }
    res+=".GIF";
    return res;
}
void Carte::SetPosition(int w,int h,int option)
{
    _recto.SetPosition(w,h,option);
    _verso.SetPosition(w,h,option);
}
bool Carte::Arrived()
{
    return (!_verso.Moving())||_verso.Arrived();
}
void Carte::Set_animate_donne(int ind_player,int lag,bool transparency,int num_card,int speed)
{
    int posx(0),posy(0),ooption(0);
    int num = num_card-2;
    switch (ind_player)
    {
        case 0:
            posx = _SCREEN_WIDTH/2;
            posy = _SCREEN_HEIGHT-120;
            ooption = 4;
            posx += num*30;
        break;

        case 1:
            posx = _SCREEN_WIDTH - 100;
            posy = _SCREEN_HEIGHT/2.;
            ooption = 2;
            posy += num*15;
        break;

        case 2:
            posx = _SCREEN_WIDTH/2.;
            posy = 120;
            ooption = 3;
            posx += num*30;
        break;

        case 3:
            posx = 100;
            posy = _SCREEN_HEIGHT/2.;
            ooption = 1;
            posy += num*15;
        break;
    }
    _display_recto = transparency;
    if (!transparency) Set_animate(posx,posy,ooption,speed,lag);
    else Set_animate_transparency(posx,posy,ooption,speed,true,lag);
}
void Carte::add_display(list<Images*>& li_display)
{
    li_display.push_back(&_recto);
    li_display.push_back(&_verso);
}
void Carte::reveal(int lag,int speed)
{
    _display_recto = true;
    _recto.Set_transparent(true,speed,lag);
    _verso.Set_transparent(false,speed,lag);
}
int Carte::GetAlpha()
{
    return _recto.GetAlpha();
}
void Carte::give_back(int speed,bool player_human)
{
    int posx,posy;
    posx = _SCREEN_WIDTH/2.;
    posy = _SCREEN_HEIGHT/2.;
    if (!player_human) Set_animate(posx,posy,0,speed,0);
    else Set_animate_transparency(posx,posy,0,speed,false,0);
}
void Carte::reset()
{
    _display_recto = false;
    _recto.reset();
    _verso.reset();
    _down = true;
}
void Carte::Set_animate(double end_x, double end_y, int opt, int speed, int lag)
{
    _recto.Set_animate(end_x,end_y,opt,speed,lag);
    _verso.Set_animate(end_x,end_y,opt,speed,lag);
}
void Carte::Set_animate_transparency(double end_x_th, double end_y_th,int option,int speed,bool growing,int lag)
{
    _recto.Set_animate_transparency(end_x_th,end_y_th,option,speed,growing,lag);
    _verso.Set_animate_transparency(end_x_th,end_y_th,option,speed,!growing,lag);
}
void Carte::restart_click()
{
    _recto.restart_click();
    _verso.restart_click();
}
bool Carte::click_on(bool Short)
{
    bool res = _recto.click_on(Short);
    return res;
}
bool Carte::Mouse_over()
{
    return _recto.Mouse_over();
}
void Carte::Move_down()
{
    if (!_down)
    {
        _recto.Move_down();
        _verso.Move_down();
        _down = true;
    }
}
void Carte::Move_up()
{
    if (_down)
    {
        _recto.Move_up();
        _verso.Move_up();
        _down = false;
    }
}
void Carte::up_transparent()
{
    if (_transparent_down)
    {
        _recto.up_transparent();
        _transparent_down = false;
    }
}
void Carte::down_transparent()
{
    if (!_transparent_down)
    {
        _recto.down_transparent();
        _transparent_down = true;
    }
}
void Carte::update()
{
    _recto.update();
    _verso.update();
}
bool Carte::GetDown()
{
    return _down;
}
