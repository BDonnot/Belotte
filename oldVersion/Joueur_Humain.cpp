#include "Joueur_Humain.h"
#include "Foncteurs_Cartes.h"
using namespace std;

Joueur_Humain::Joueur_Humain()
{
    _res = 0;
}
Joueur_Humain::Joueur_Humain(string name):Joueur(name)
{
    _res = 0;
}

Joueur_Humain::~Joueur_Humain()
{
    //dtor
}

bool Joueur_Humain::I_take_1st_round(int i, Carte* pcard,bool& first_displayed)
{
    first_displayed = false;
    _action_done = false;
    return false;
}
int Joueur_Humain::I_take_2nd_round(int i, Carte* pcard)
{
    _action_done= false;
    _first_displayed = false;
    return -1;
}

void Joueur_Humain::clear()
{
    list<Carte*> li;
    _hand=li;
    _tricks_won=li;
    _action_done = false;
    _has_taken = false;
}
void Joueur_Humain::Update(array<Carte*,4> trick ,Carte* pcard_beginning,int,int)
{
    Carte* pcard = NULL;
    for (int i = 0; i <4; i++)
    {
        pcard =trick[i];
        _cards_fallen.push_front(pcard);
    }
}
Carte* Joueur_Humain::choose_card(array<Carte*,4> trick,int trump_colour,int team_take,int number_trick,int pos)
{
    _action_done= false;
    if (number_trick==0){ sort(trump_colour);}
    list<Carte*> empty;
    _playable_cards = empty;
    for (list<Carte*>::iterator it = _hand.begin(); it != _hand.end(); ++it)
    {
        (*it)->restart_click();
        (*it)->down_transparent();
        if (can_play_card((*it),trick,trump_colour)) _playable_cards.push_back((*it));
    }
    return NULL;
}
void Joueur_Humain::I_choose_card(list<Images*>& li_displayed, Images* pbutton)
{
    Carte* pcard = NULL;
    Carte* pcard2 = NULL;
    if (!_action_done)
    {
        for (list<Carte*>::iterator it = _playable_cards.begin();it!=_playable_cards.end();++it)
        {
            pcard = *it;
            pcard->Move_down();
            pcard -> up_transparent();
            if (pcard->Mouse_over())
            {
                _card_played_h = pcard;
                _card_played_h->Move_up();
                if (pcard2 != NULL) pcard2->Move_down();
            }
            pcard2 =*it;
        }
        if (_card_played_h != NULL)
        {
            if (_card_played_h->click_on(true))
            {
                erase(_card_played_h);
                _card_played = _card_played_h;
                _card_played_h = NULL;
                _card_played -> Set_animate(_SCREEN_WIDTH/2., _SCREEN_HEIGHT/2.+80,0,0,0);
                _action_done = true;
                int posx,posy,ooption,i=0;
                for (list<Carte*>::iterator it = _hand.begin(); it != _hand.end(); ++it)
                {
                   define_position_card(posx,posy,ooption,i,_hand.size());
                   i++;
                   (*it)->SetPosition(posx,posy,ooption);
                }
            }
        }
    }
}
void Joueur_Humain::update_nb_cards()
{
}
void Joueur_Humain::I_choose_1st_round(list<Images*>& li_displayed, list<Images*> li_aux,Images* ptext)
{
    if (!_action_done)
    {
        Images* pimage = NULL;
        pimage = li_aux.front();
        li_aux.pop_front();
        li_displayed.push_back(pimage);
        pimage->type("voulez-vous prendre ?");
        pimage = li_aux.front();
        li_aux.pop_front();
        li_displayed.push_back(pimage);
        pimage->type("oui");
        pimage->SetPosition(_SCREEN_WIDTH/3,_SCREEN_HEIGHT/2,0);
        if (pimage->click_on(true))
        {
            _res = 0;
            _action_done = true;
            ptext->type("je prends");
            _score_take.Human_choice_taking(-1,true);
        }
        pimage = li_aux.front();
        li_displayed.push_back(pimage);
        li_aux.pop_front();
        pimage->type("non");
        pimage->SetPosition(2*_SCREEN_WIDTH/3,_SCREEN_HEIGHT/2,0);
        if (pimage->click_on(true))
        {
            _res = -1;
            _action_done = true;
            ptext->type("un !");
        }
    }
    else li_displayed.push_back(ptext);
}
void Joueur_Humain::I_choose_2nd_round(list<Images*>& li_displayed, list<Images*> li_aux,Images* ptext,int colour)
{
    int j = 0;
    int i = 0;
    if (li_aux.size() > 4)
    {
        if (!_action_done)
        {
            Images* pimage = li_aux.front();
            li_aux.pop_front();
            pimage->type("a quoi voulez-vous prendre ?");
            li_displayed.push_back(pimage);
            pimage = li_aux.front();
            li_aux.pop_front();
            li_displayed.push_back(pimage);
            pimage->type("rien");
            pimage->SetPosition(2*_SCREEN_WIDTH/3,_SCREEN_HEIGHT/2+25,0);
            if (pimage->click_on(true))
            {
                _res = -1;
                _action_done = true;
                ptext->type("deux !");
            }
            if (!_action_done)
            {
                for (list<Images*>::iterator it = li_aux.begin(); it != li_aux.end(); ++it)
                {
                    if (j!= colour)
                    {
                        pimage = *it;
                        li_displayed.push_back(pimage);
                        pimage->type(convertion(j));
                        pimage->SetPosition( (i%2+1)*_SCREEN_WIDTH/3,_SCREEN_HEIGHT/2 + (-1+2*((int) (i>=2)))*25,0);
                        if (pimage->click_on(true))
                        {
                            _res = j;
                            _score_take.Human_choice_taking(_res,false);
                            _action_done = true;
                            ptext->type(convertion(j));
                        }
                        i++;
                    }
                    else --it;
                    j++;
                }
            }
        }
        else li_displayed.push_back(ptext);
    }
}
string Joueur_Humain::convertion(int colour)
{
    switch (colour)
    {
        case 0 : return "carreau";
        case 1 : return "coeur";
        case 2 : return "pique";
        case 3 : return "trefle";
        default : return "";
    }
}
void Joueur_Humain::sort(int trump_colour)
{
    Sort_CardsOld foncteur(trump_colour);
    _hand.sort(foncteur);
    int posx,posy,ooption,i=0;
    for (list<Carte*>::iterator it = _hand.begin(); it != _hand.end(); ++it)
    {
       define_position_card(posx,posy,ooption,i,8);
       i++;
       (*it)->SetPosition(posx,posy,ooption);
    }
}
