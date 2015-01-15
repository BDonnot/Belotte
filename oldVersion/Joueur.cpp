#include "Joueur.h"
#include "fonctions_annexes.h"
#include "Foncteurs_Cartes.h"
#include <iostream>
using namespace std;

Joueur::Joueur():_score_take(1)
{
    _name="Anonyme";
    _a_belote=false;
    list<Carte*> hand;
    _hand= hand;
    list<Carte*> tricks_won;
    _tricks_won=tricks_won;
    _res= 0;
    _card_played = NULL;
    _erased = false;
    _has_taken = false;
}
Joueur::Joueur(string name):_score_take(1)
{
    _name = name;
    _erased = false;
    _a_belote=false;
    list<Carte*> hand;
    _hand= hand;
    list<Carte*> tricks_won;
    _tricks_won=tricks_won;
    _res= 0;
    _card_played = NULL;
    _has_taken = false;
}
Joueur::~Joueur()
{
}
string Joueur::Getname()
{
    return _name;
}
void Joueur::GetCard(Carte* pcard,int player_give)
{
    if (pcard != NULL) _hand.push_front(pcard);
    int n = _hand.size();
    int lag = choose_lag(n,player_give);
    pcard->Set_animate_donne(_number,lag,_number==0,n,1);
}
int Joueur::choose_lag(int number_cards,int player_give)
{
    int number = (_number+player_give)%4;
    if (number_cards <= 3) return number*6+number_cards*9;
    if (number_cards <= 5) return 42+number*6+number_cards*9;
    return number*6+number_cards*9;
}
list<Carte*> Joueur::GetMain()
{
    return _hand;
}
void Joueur::PutNumber(int n,SDL_Event* pevent,int w, int h)
{
    _number = n;
    _pEvent = pevent;
    _SCREEN_HEIGHT=h;
    _SCREEN_WIDTH=w;
    SDL_Color red = {255,0,0};
    SDL_Color blue = {0,0,255};
    Images image0(120,90,"images/you.jpg",*_pEvent);
    Text name0("fonts/Starjedi.ttf",15,blue,*_pEvent);
    Images image1(90,100,"images/maul.jpg",*_pEvent);
    Text name1("fonts/Starjedi.ttf",15,red,*_pEvent);
    Images image2(119,88,"images/yoda.jpg",*_pEvent);
    Text name2("fonts/Starjedi.ttf",15,blue,*_pEvent);
    Images image3(90,100,"images/vador.jpg",*_pEvent);
    Text name3("fonts/Starjedi.ttf",15,red,*_pEvent);
    switch (_number)
    {
        case 0:
            image0.SetPosition(_SCREEN_WIDTH/2.,_SCREEN_HEIGHT,4);
            name0.type(_name);
            name0.SetPosition(_SCREEN_WIDTH/2.,_SCREEN_HEIGHT-45,4);
            _image = image0;
            _name_surface = name0;
        break;

        case 1:
            image1.SetPosition(_SCREEN_WIDTH,_SCREEN_HEIGHT/2.,2);
            name1.type(_name);
            name1.SetPosition(_SCREEN_WIDTH-45,_SCREEN_HEIGHT/2.+50,3);
            _image = image1;
            _name_surface = name1;
        break;

        case 2:
            image2.SetPosition(_SCREEN_WIDTH/2.,0,3);
            name2.type(_name);
            name2.SetPosition(_SCREEN_WIDTH/2.,88,3);
            _name_surface = name2;
            _image = image2;
        break;

        case 3:
            image3.SetPosition(0.,_SCREEN_HEIGHT/2.,1);
            name3.type(_name);
            name3.SetPosition(45,_SCREEN_HEIGHT/2.+50,3);
            _name_surface = name3;
            _image = image3;
        break;

        default :
        break;
    }
}
void Joueur::setName(string newName)
{
    _name = newName;
    _name_surface.type(newName);
}
void Joueur::displayMain()
{
    for (list<Carte*>::iterator it = _hand.begin(); it != _hand.end(); ++it)
    {
        cout << (*it)->display() <<endl;
    }
}
bool Joueur::is_equal(Joueur &J)
{
    Joueur *pJ = &J;
    return pJ==this;
}
bool Joueur::operator==(Joueur &J)
{
    return is_equal(J);
}
void Joueur::update_stat(int score_0,int taker,int color_trump)
{
    int team = _number%2;
    int score_bis = 0;
    int taker_team = taker%2;
    if (score_0== 252) score_bis = 0;
    else
    {
        if (score_0== 0) score_bis = 252;
        else score_bis = 162-score_0;
    }
    int diff_score = score_0-score_bis;
    Update_Stats foncteur(diff_score,color_trump);
    if(taker==_number) _score_take.Update(color_trump,foncteur); //if I took
    /*{
        for_each(_to_update.begin(),_to_update.end(),foncteur);
    }*/
    else //I didn't took
    {
        if (team!=taker_team) _score_take.Update(color_trump,foncteur); //for_each(_to_update.begin(),_to_update.end(),foncteur);
    }
}
void Joueur::win_trick(array<Carte*,4> trick)
{
    for (int i=0;i<4;i++)
    {
        _tricks_won.push_front(trick[i]);
    }
}
void Joueur::clear()
{
    list<Carte*> li;
    _hand=li;
    _tricks_won=li;
    _res = 0;
    _has_taken = false;
}
bool Joueur::GetABelote()
{
    return _a_belote;
}
list<Carte*> Joueur::GetTrickWon()
{
    return _tricks_won;
}
list<Carte*> Joueur::playable_cards(array<Carte*,4> trick_in_progress, int trump_colour)
{
    list<Carte*> res;
    for (list<Carte*>::iterator it = _hand.begin(); it != _hand.end(); ++it)
    {
        if (can_play_card(*it,trick_in_progress,trump_colour)) res.push_front(*it);
    }
    return res;
}
bool Joueur::can_play_card(Carte* PmyCard,array<Carte*,4> trick_in_progress, int trump_colour)
{
    Carte* pcard = NULL;
    pcard = trick_in_progress[0];
    int colour_asked,my_colour;
    list <Carte*> my_hand;
    if (PmyCard==NULL) return false;
    if (pcard==NULL) return true; //if we are the starter we can play what we want
    colour_asked = pcard->GetColour();
    my_colour = PmyCard->GetColour();
    if ((has_colour(colour_asked))&&(colour_asked != trump_colour)) return (colour_asked == my_colour); //If we have the colour asked we have to played it
    else
    {
        if (colour_asked==trump_colour)
        {
            if (my_colour==trump_colour) return can_play_trump(PmyCard,trick_in_progress);
            else return (!has_colour(colour_asked));
        }
        else
        {
            if (my_colour==trump_colour) return can_play_trump(PmyCard,trick_in_progress); //If we can play a trump higher we have to do it
            else return can_discarding(trick_in_progress,trump_colour); //otherelse : the card we want to play is neither the colour asked nor a trump, so we discard
        }
    }
}
bool Joueur::has_colour(int colour)
{
    for (list<Carte*>::iterator it = _hand.begin(); it != _hand.end(); ++it)
    {
        if ((*it)->GetColour() == colour) return true;
    }
    return false;
}
bool Joueur::has_card(int colour,int height)
{
    for (list<Carte*>::iterator it = _hand.begin(); it != _hand.end(); ++it)
    {
        if (((*it)->GetColour() == colour)&&((*it)->GetHeight()==height)) return true;
    }
    return false;
}
bool Joueur::can_discarding(array<Carte*,4> trick_in_progress, int trump_colour) //we suppose the player doesn't have the colour asked and that the card is not a trump
{
    int pos=getPosition(trick_in_progress);
    if (pos==1)
    {
        return (!(has_colour(trump_colour))); //if we play in second position we can discard only if we don't have trump
    }
    else
    {
        if (pos==2) return (!(has_colour(trump_colour))||(Strongest_card(2,trick_in_progress,trump_colour)==0)); //if we are in third position, we can discard if our partner is the strongest or if we don't have trump
        else //so we play as the last player
        {
            return (!(has_colour(trump_colour))||(Strongest_card(3,trick_in_progress,trump_colour)==1)); //same rule as if we are in third position
        }
    }
}
int Joueur::Strongest_card(int Stop,array<Carte*,4> trick,int _trump) //return the index, in the trick given as parameter, of the card supposed to win the trick if the trick had only 'Stop'-1 cards
{
    if (Stop > 0)
    {
        if (trick[0]==NULL) return 0;
        int colour = trick[0]->GetColour();
        int i_winning=0;
        Carte* pcard = NULL;
        bool trump = false;
        for (int i = 1;i<Stop;i++)
        {
            pcard = trick[i];
            if (pcard == NULL) return i_winning;
            if (pcard->GetColour()==_trump)
            {
                trump = true;
                if (colour==_trump)
                {
                    if (Carte::If_Trump()[ (trick[i_winning]->GetHeight())+8*(pcard->GetHeight()) ]) i_winning=i;
                }
                else //so the colour played is not trump
                {
                    if (trick[i_winning]->GetColour()==_trump)
                    {
                        if (Carte::If_Trump()[ (trick[i_winning]->GetHeight())+8*(pcard->GetHeight()) ]) i_winning=i;
                    }
                    else i_winning=i;
                }
            }
            else //so the card in progress is not trump
            {
                if ((pcard->GetColour()==colour)&&!trump)
                {
                    if (Carte::No_Trump()[ (trick[i_winning]->GetHeight())+8*(pcard->GetHeight()) ]) i_winning=i;
                }
            }
        }
        return i_winning;
    }
    return 0;
}
int Joueur::getPosition(array<Carte*,4> trick_in_progress) //0 if I play first, 1 if I play 2nd, 2 if I play 3rd and 3 if I play 4th
{
    for (int i = 0; i<4 ;i++) {if (trick_in_progress[i] == NULL) return i;}
    return 4;
}
bool Joueur::trump_played(array<Carte*,4> trick,int trump_colour)
{
    for (int i = 0; i < 4; i++)
    {
        if (trick[i] == NULL) return false;
        if (trick[i]->GetColour() == trump_colour) return true;
    }
    return false;
}
bool Joueur::can_play_trump(Carte* PmyCard,array<Carte*,4> trick) //the card is supposed to be trump and the player is supposed not to have the colour asked
{
    if (PmyCard !=NULL)
    {
        int position = getPosition(trick);
        int max_height = 0;
        int trump_colour = PmyCard->GetColour();
        if (!trump_played(trick,trump_colour)) return true; //we have always the right to play a trump if no trump has been played and we don't have the colour asked
        else
        {
            max_height = trick[Strongest_card(position,trick,trump_colour)]->GetHeight();
            return ((Carte::If_Trump()[max_height+8*PmyCard->GetHeight()])||(!has_higher(trump_colour,max_height)));
        }
    }
    return false;
}
bool Joueur::has_higher(int trump_colour,int max_height)
{
    for (list<Carte*>::iterator it = _hand.begin(); it != _hand.end(); ++it)
    {
        if (((*it)->GetColour()==trump_colour)&&(Carte::If_Trump()[max_height+8*((*it)->GetHeight())])) return true;
    }
    return false;
}

int Joueur::PlayCard(list<Images*>& li_displayed, Images* pbutton,Images* pbelote,array<Carte*,4>& trick, int trump_colour,int player_take,int number_trick,int pos,bool& first_displayed)
{
    Carte* res = NULL;
    if (first_displayed)
    {
        first_displayed = false;
        res = choose_card(trick,trump_colour,player_take,number_trick,pos);
        _card_played = res;
        if (_card_played != NULL)
        {
            switch(_number)
            {
                case 0 : _card_played -> Set_animate(_SCREEN_WIDTH/2., _SCREEN_HEIGHT/2.+80,0,0,0); break;
                case 1 : _card_played -> Set_animate_transparency(_SCREEN_WIDTH/2. +55, _SCREEN_HEIGHT/2.,0,0,true,0); break;
                case 2 : _card_played -> Set_animate_transparency(_SCREEN_WIDTH/2., _SCREEN_HEIGHT/2.-80,0,0,true,0); break;
                case 3 : _card_played -> Set_animate_transparency(_SCREEN_WIDTH/2.-55, _SCREEN_HEIGHT/2.,0,0,true,0); break;
                default : break;
            }
        }
        int posx,posy,ooption,i=0;
        for (list<Carte*>::iterator it = _hand.begin(); it != _hand.end(); ++it)
        {
           define_position_card(posx,posy,ooption,i,8-number_trick);
           i++;
           (*it)->SetPosition(posx,posy,ooption);
        }
    }
    else
    {
        I_choose_card(li_displayed, pbutton);
        if (_card_played != NULL)
        {
            trick.at(pos) = _card_played;
            erase(_card_played);
        }
    }
    if (trick[pos] != NULL)
    {
        if (trick[pos]-> Arrived())
        {
            _card_played = NULL;
            first_displayed = true;
            _erased = false;
            return 1;
        }
    }
    return 0;
}

Carte* Joueur::JoueCarte(array<Carte*,4>& trick, int trump_colour,int player_take,int number_trick,int pos)
{
    return choose_card(trick,trump_colour,player_take,number_trick,pos);
}

bool Joueur::Update_belote(int trump_colour)
{
    bool belote = false;
    for (list<Carte*>::iterator it = _hand.begin(); it != _hand.end(); ++it)
    {
        if ( ((*it)->GetColour() == trump_colour) && ( ((*it)->GetHeight() == 6)||((*it)->GetHeight() == 5) ) )
        {
            if (belote)
            {
                _a_belote = true;
                return true;
            }
            else belote = true;
        }
    }
    _a_belote = false;
    return false;
}
void Joueur::I_choose_card(list<Images*>& li_displayed, Images* pbutton)
{
}
int Joueur::bidding_first_round(list<Images*>& li_displayed, list<Images*> li_aux,Images* ptext,int pos, Carte* pcard, bool& first_displayed)
{
    bool res = false;
    if (first_displayed)
    {
        res = I_take_1st_round(pos,pcard,first_displayed);
        position_text_bid(ptext);
        if (res)
        {
            if (ptext != NULL)
            {
                ptext->type("je prends !");
                ptext->Set_transparent(true,0,1);
                li_displayed.push_back(ptext);
            }
            _res = 0;
            _has_taken = true;
            return _res;
        }
        else
        {
            if (ptext != NULL)
            {
                ptext->type("un !");
                ptext->Set_transparent(true,0,1);
                li_displayed.push_back(ptext);
            }
            _res = -1;
            return _res;
        }
    }
    else
    {
        I_choose_1st_round(li_displayed,li_aux,ptext);
        _has_taken = (_res != -1);
        return _res;
    }
}
void Joueur::I_choose_1st_round(list<Images*>& li_displayed, list<Images*> li_aux,Images* ptext)
{
    li_displayed.push_back(ptext);
}
void Joueur::I_choose_2nd_round(list<Images*>& li_displayed, list<Images*> li_aux,Images* ptext,int colour)
{
    li_displayed.push_back(ptext);
}
int Joueur::bidding_second_round(list<Images*>& li_displayed,list<Images*> li_aux,Images* ptext,int pos, Carte* pcard, bool& first_displayed)
{
    int res = -1;
    if (first_displayed)
    {
        first_displayed = false;
        res =I_take_2nd_round(pos,pcard);
        _res = res;
        if (ptext != NULL)
        {
            position_text_bid(ptext);
            switch (_res)
            {
                case 0 : ptext-> type("carreau"); break;
                case 1 : ptext-> type("coeur"); break;
                case 2 : ptext-> type("pique"); break;
                case 3 : ptext-> type("trefle"); break;
                case -1 : ptext->type("deux !"); break;
                default : break;
            }
            ptext->Set_transparent(true,0,0);
            li_displayed.push_back(ptext);
        }
        return _res;
    }
    else
    {
        I_choose_2nd_round(li_displayed,li_aux,ptext,pcard->GetColour());
        return _res;
    }
}
void Joueur::position_text_bid(Images* ptext)
{
    switch (_number)
    {
        case 0: ptext->SetPosition(_SCREEN_WIDTH/2.,_SCREEN_HEIGHT-60,4); break;
        case 1: ptext->SetPosition(_SCREEN_WIDTH-45,_SCREEN_HEIGHT/2.+65,3); break;
        case 2: ptext->SetPosition(_SCREEN_WIDTH/2.+60,45,1); break;
        case 3: ptext->SetPosition(45,_SCREEN_HEIGHT/2.+65,3); break;
        default : break;
    }
}
int Joueur::how_many_colour(int colour) //how many cards I have in the color
{
    int res = 0;
    for (list<Carte*>::iterator it = _hand.begin(); it != _hand.end(); ++it)
    {
        if ((*it)->GetColour() == colour) res++;
    }
    return res;
}
Images* Joueur::GetpImage()
{
    return &_image;
}
Text* Joueur::GetpNameSurface()
{
    return &_name_surface;
}
void Joueur::erase(Carte* pcard)
{
    Carte* pca = NULL;
    int n = _hand.size();
    int comp = 0;
    if ((pcard != NULL)&&(!_erased))
    {
        while (comp < n)
        {
            pca = _hand.front();
            if ((pca->GetColour()!=pcard->GetColour())||(pca->GetHeight()!=pcard->GetHeight()))
            {
                _hand.push_back(pca);
            }
            _hand.pop_front();
            comp++;
        }
        _erased = true;
    }
}
void Joueur::add_display(list<Images*>& li_display)
{
    //int n = _hand.size();
    //int posx,posy,ooption,i=0;
    for (list<Carte*>::iterator it = _hand.begin(); it != _hand.end(); ++it)
    {
       //define_position_card(posx,posy,ooption,i,n);
       //i++;
       //(*it)->SetPosition(posx,posy,ooption);
       (*it)->add_display(li_display);
    }
}
void Joueur::define_position_card(int& posx,int& posy,int& ooption,int num,int nb_carte)
{
    switch (_number)
    {
        case 0:
            posx = _SCREEN_WIDTH/2;
            posy = _SCREEN_HEIGHT-120;
            ooption = 4;
            posx += 30*(num - nb_carte/2);
        break;
        case 1:
            posx = _SCREEN_WIDTH - 100;
            posy = _SCREEN_HEIGHT/2.;
            ooption = 2;
            posy += 15*(num- nb_carte/2);
        break;
        case 2:
            posx = _SCREEN_WIDTH/2.;
            posy = 120;
            ooption = 3;
            posx -= 30*(num - nb_carte/2);
        break;
        case 3:
            posx = 100;
            posy = _SCREEN_HEIGHT/2.;
            ooption = 1;
            posy -= 15*(num- nb_carte/2);
        break;
        default : break;
    }
}
int Joueur::CountPoints(int trump_color,list<Carte*>& cards,int& nb_cards)
{
    int points_in_hand = 0;
    for (list<Carte*>::iterator it = _tricks_won.begin();it != _tricks_won.end();++it)
    {
        points_in_hand += (*it)->Nb_Point(trump_color);
        cards.push_front(*it);
        nb_cards++;
    }
    return points_in_hand;
}
void Joueur::Give_back(list<Carte*>& cards)
{
    for (list<Carte*>::iterator it = _hand.begin();it!=_hand.end();++it)
    {
        (*it)->give_back(1,_number==0);
        cards.push_front((*it));
    }
    clear();
}
void Joueur::Update_score_take(Carte* pcard_proposed,int player_give)
{
    _score_take.Set_hand(_hand,pcard_proposed,modulo(_number-player_give-1,4));
}
