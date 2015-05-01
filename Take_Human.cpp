#include "Take_Human.h"
using namespace::std;

Take_Human::Take_Human(SDL_Event* event,SDL_Surface* screen,Uint16 windows_width, Uint16 windows_height):
    _windows_width(windows_width)
    ,_windows_height(windows_height)
    ,_carreau(event,"Carreau",PositionGraphic())
    ,_coeur(event,"Coeur",PositionGraphic())
    ,_pique(event,"Pique",PositionGraphic())
    ,_trefle(event,"Trefle",PositionGraphic())
    ,_pass(event,"Aucune",PositionGraphic())
    ,_yes(event,"Oui",PositionGraphic())
    ,_no(event,"Non",PositionGraphic())
    ,_question("fonts/MLSGY.ttf",40,255,255,255,"A quelle couleur voulez-vous prendre ?")
    ,_question_which_color("A quelle couleur voulez-vous prendre ?")
    ,_screen(screen)
{
    PositionGraphic pos(windows_width/2,windows_height/3,CENTER);
    _question.SetPosition(pos);
    _first_round = false;
    _displayed = true;
    Change(true);
    pos.Set(windows_width/3,7*windows_height/16,CENTER);
    _carreau.SetPosition(pos);
    pos.Set(2*windows_width/3,7*windows_height/16,CENTER);
    _coeur.SetPosition(pos);
    pos.Set(windows_width/3,9*windows_height/16,CENTER);
    _pique.SetPosition(pos);
    pos.Set(windows_width/3,9*windows_height/16,CENTER);
    _trefle.SetPosition(pos);
    pos.Set(2*windows_width/3,9*windows_height/16,CENTER);
    _pass.SetPosition(pos);
    pos.Set(windows_width/3,windows_height/2,CENTER);
    _yes.SetPosition(pos);
    pos.Set(2*windows_width/3,windows_height/2,CENTER);
    _no.SetPosition(pos);
}

void Take_Human::Change(bool first_round)
{
    _displayed= true;
    _first_round = first_round;
    PositionGraphic pos0(_windows_width/3,7*_windows_height/16,CENTER);
    PositionGraphic pos1(2*_windows_width/3,7*_windows_height/16,CENTER);
    PositionGraphic pos2(_windows_width/3,9*_windows_height/16,CENTER);

    if(_first_round)
    {
        _question.ChangeText(DefineQuestion());
        return ;
    }
    _question.ChangeText(_question_which_color);
    switch (_info.ColorProposed().Color())
    {
    case DIAMOND :
        _coeur.SetPosition(pos0);
        _pique.SetPosition(pos1);
        _trefle.SetPosition(pos2);
        break;
    case HEART :
        _carreau.SetPosition(pos0);
        _pique.SetPosition(pos1);
        _trefle.SetPosition(pos2);
        break;
    case SPADE :
        _carreau.SetPosition(pos0);
        _coeur.SetPosition(pos1);
        _trefle.SetPosition(pos2);
        break;
    case CLUB :
        _carreau.SetPosition(pos0);
        _coeur.SetPosition(pos1);
        _pique.SetPosition(pos2);
        break;
    default :
        _carreau.SetPosition(pos0);
        _coeur.SetPosition(pos1);
        _pique.SetPosition(pos2);
        break;
    }
}

Card_Color Take_Human::Has_Taken() //255 if the human did'nt choose yet, 127 if he passes, otherwie the number of the color choosen
{
    if( _first_round)
    {
        if (_yes.Click(true)) return _info.ColorProposed();
        if (_no.Click(true)) return Card_Color(NO);
        return NOT_CHOSEN;
    }
    //it is the second round
    switch (_info.ColorProposed().Color())
    {
    case DIAMOND :
        if (_coeur.Click(true)) return HEART;
        if (_pique.Click(true)) return SPADE;
        if (_trefle.Click(true)) return CLUB;
        break;
    case HEART :
        if (_carreau.Click(true)) return DIAMOND;
        if (_pique.Click(true)) return SPADE;
        if (_trefle.Click(true)) return CLUB;
        break;
    case SPADE :
        if (_carreau.Click(true)) return DIAMOND;
        if (_coeur.Click(true)) return HEART;
        if (_trefle.Click(true)) return CLUB;
        break;
    case CLUB :
        if (_carreau.Click(true)) return DIAMOND;
        if (_coeur.Click(true)) return HEART;
        if (_pique.Click(true)) return SPADE;
        break;
    default : //TO DO exception here
        break;
    }
    if (_pass.Click(true)) return NO;
    return NOT_CHOSEN;
}

void Take_Human::Display()
{
    if (!_displayed) return; //we do nothing if we are not in a phase we can take
    _question.Display(_screen);
    if (_first_round)
    {
        _yes.Display(_screen);
        _no.Display(_screen);
        return ;
    }
    switch(_info.ColorProposed().Color())
    {
    case DIAMOND :
        _coeur.Display(_screen);
        _pique.Display(_screen);
        _trefle.Display(_screen);
        break;
    case HEART :
        _carreau.Display(_screen);
        _pique.Display(_screen);
        _trefle.Display(_screen);
        break;
    case SPADE :
        _carreau.Display(_screen);
        _coeur.Display(_screen);
        _trefle.Display(_screen);
        break;
    case CLUB :
        _carreau.Display(_screen);
        _coeur.Display(_screen);
        _pique.Display(_screen);
        break;
    default :
        _carreau.Display(_screen);
        _coeur.Display(_screen);
        _pique.Display(_screen);
        break;
    }
    _pass.Display(_screen);
}

void Take_Human::Reset()
{
    _displayed = true;
}

void Take_Human::Update()
{
    if (!_displayed) return; //we don't update if we don't display
    if (_first_round)
    {
        _yes.Update_on_it();
        _no.Update_on_it();
    }
    else
    {
        _carreau.Update_on_it();
        _coeur.Update_on_it();
        _pique.Update_on_it();
        _trefle.Update_on_it();
        _pass.Update_on_it();
    }
}

string Take_Human::DefineQuestion()
{
    string res = "Voulez-vous prendre à";
    switch(_info.ColorProposed().Color())
    {
    case DIAMOND :
        res += " carreau ?";
        return res;
    case HEART :
        res += " coeur ?";
        return res;
    case SPADE :
        res += " pique ?";
        return res;
    case CLUB :
        res += " trèfle ?";
        return res;
    default :
        res += " erreur ?";
        return res;
    }
}
