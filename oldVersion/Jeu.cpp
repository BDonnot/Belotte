#include "Jeu.h"
#include "fonctions_annexes.h"
#include "Joueur_humain.h"
#include "Joueur_IA.h"
#include "Text.h"
#include "Button.h"
#include "Foncteurs_Cartes.h"
#include "Random.h"


#include <algorithm>
#include <sstream>
using namespace std;

Jeu::Jeu()
{
    //array<Carte*,32> c = {NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL};
    list<Carte*> c;
    _Cards = c;
    _trump=-1;
    _firstGame=true;
    _taker=-1;
    _belote_announced=false;
    array<int,2> team_scores={0,0};
    _team_scores=team_scores;
    _ind_player=0;
    _player_give=0;
    _last_ten=-1;
    _pEvent = NULL;
    _pScreen = NULL;
    array<int,4> array_taking = {-1,-1,-1,-1};
    _array_taking = array_taking;
    array<Carte*,4> trick = {NULL,NULL,NULL,NULL};
    _trick = trick;
    _previous_trick = trick;
}
Jeu::Jeu(int screenWidth,int screenHeight,SDL_Event* pevent,SDL_Surface* screen)
{
    //array<Carte*,32> c = {NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL};
    list<Carte*> c;
    _Cards = c;
    _trump=-1;
    _firstGame=true;
    _taker=-1;
    _belote_announced=false;
    array<int,2> team_scores={0,0};
    _team_scores=team_scores;
    _ind_player=0;
    _player_give=3;
    _last_ten=-1;
    _pEvent = pevent;
    _display_trump.Set_event(pevent);
    _pScreen = screen;
    _SCREEN_WIDTH=screenWidth;
    _SCREEN_HEIGHT=screenHeight;
    _first_displayed=true;
    _first_displayed1 = true;
    _step1 =0;
    _step2 =0;
    _step3 =0;
    array<int,4> array_taking = {-1,-1,-1,-1};
    _array_taking = array_taking;
    array<Carte*,4> trick = {NULL,NULL,NULL,NULL};
    _trick = trick;
    _previous_trick = trick;
}

Jeu::~Jeu()
{
}
int Jeu::GetTrump()
{
    return _trump;
}
list<Carte*> Jeu::GetCards()
{
    return _Cards;
}
array<Carte*,4> Jeu::Gettrick()
{
    return _trick;
}
void Jeu::new_players(int i) //the parameter is the number of human players (1 ou 0)
{
    Joueur_Humain* pjh;
    Joueur_IA* pia1;
    Joueur_IA* pia2;
    Joueur_IA* pia3;
    Joueur_IA* pia0;
    array<Joueur*,4> players = {NULL,NULL,NULL,NULL};
    _Players = players;
    if (i ==1)
    {
        pjh = new Joueur_Humain("sud");
        _Players.at(0) = pjh;
    }
    else
    {
        if (i==0)
        {
            pia0 = new Joueur_IA("sud",1);
            _Players.at(0) = pia0;
        }
    }
    pia1 = new Joueur_IA("est",1);
    pia2 = new Joueur_IA("nord",1);
    pia3 = new Joueur_IA("ouest",1);
    _Players.at(1) = pia1;
    _Players.at(2) = pia2;
    _Players.at(3) = pia3;
    for (int i = 0; i < 4; i++)
    {
        _Players[i]->PutNumber(i,_pEvent,_SCREEN_WIDTH,_SCREEN_HEIGHT);
    }
}
void Jeu::delete_players_cards() //to erase the players and the cards
{
    for (int i = 0; i<4; i++)
    {
        delete _Players[i];
    }
    for (list<Carte*>::iterator it = _Cards.begin(); it!=_Cards.end();++it)
    {
        delete (*it);
    }
}
void Jeu::Cards_generation() //generate the cards
{
    if (_firstGame)
    {
        Carte* pcard = NULL;
        for (int i=0;i<4;i++)
        {
            for(int j=0;j<8;j++)
            {
                pcard = new Carte (i,j,_pEvent,_SCREEN_WIDTH,_SCREEN_HEIGHT);
                _Cards.push_back(pcard);
            };
        };
    }
}
void Jeu::Shuffle()
{
    shuffle(_Cards);
    _last = _Cards.back();
    Reset_Cards reset;
    for_each(_Cards.begin(),_Cards.end(),reset);
}

void Jeu::Cut() //cut the pack
{
    Random random(3,28);
    int nb_rand = random.generate_number();
    list<Carte*>::iterator it = _Cards.begin();
    advance(it,nb_rand);
    list<Carte*> aux;
    aux.splice(aux.begin(),aux,_Cards.begin(),it);
    _Cards.splice(_Cards.end(),aux);
    _last = _Cards.back();
    Reset_Cards reset;
    for_each(_Cards.begin(),_Cards.end(),reset);
    _ind_player = modulo(_player_give+1,4);
}
void Jeu::Give_n_cards(int first,int nb_cards_given,int ind_player,bool give_1) //first : the number of the first card given
{
    Carte* pcard = NULL;
    int comp=first;
    Joueur* pplayer=_Players[ind_player];
    /*
    int n;
    if (give_1)
    {
        if (nb_cards_given==3) n = 0;
        else n = 3;
    }
    else
    {
        if (nb_cards_given==2) n = 6;
        else n = 5;
    }
    */
    for (int i = 0; i < nb_cards_given;i++)
    {
        pcard = _Cards.front();
        _Cards.pop_front();
        pplayer -> GetCard(pcard,_player_give);
        comp++;
    }
}
int Jeu::Give_1(list<Images*>& li_displayed) //The first phase of deal : we give 3 cards, then 2 cards
{
    int nb_cards_given=3;
    int k=0;
    int ind_player;
    int number_player=0;
    int comp=0;
    _Cards.front()->add_display(li_displayed);
    if (_first_displayed1)
    {
        for (k=0;k<2;k++)
        {
            for(ind_player=_player_give+1;ind_player<_player_give+5;ind_player++)
            {
                number_player = modulo(ind_player,4);
                Give_n_cards(comp,nb_cards_given,number_player,true);
                comp+=nb_cards_given;
            };
            nb_cards_given--;
        };
        _Cards.front()->reveal(7*20,0);
        _first_displayed1 = false;
    }
    if (_Cards.front()->GetAlpha()==255)
    {
        _first_displayed1 = true;
        return 1;
    }
    return 0;
}
int Jeu::Give_2(list<Images*> li_displayed,int i_taker)
{
    int nb_cards_given=3;
    int ind_player;
    int number_player=0;
    int comp=21;
    if (_first_displayed1)
    {
        Give_n_cards(20,1,i_taker,false);
        for(ind_player=_player_give+1;ind_player<_player_give+5;ind_player++)
        {
            number_player = modulo(ind_player,4);
            if (number_player!=i_taker) nb_cards_given=3;
            else nb_cards_given=2;
            Give_n_cards(comp,nb_cards_given,number_player,false);
            comp+=nb_cards_given;
        }
        _first_displayed1 = false;
    }
    if (_last->Arrived())
    {
        _first_displayed1 = true;
        return 1;
    }
    return 0;
}
void Jeu::Beginning()
{
    if (_firstGame) Shuffle();
    else Cut();
    _firstGame=false;
}
int Jeu::Give(list<Images*>& li_displayed,list<Images*>& li_aux,list<Images*>& li_text)
{
    int i_taker = _array_taking[0];
    switch (_step2)
    {
        case -1 :
            _step2=0;
            _first_displayed = true;
            return 0;
        case 0 : _step2 += Give_1(li_displayed); break;
        case 1 : _step2 += Taking(li_displayed,li_aux,li_text); break;
        case 2 : _step2 += Give_2(li_displayed,i_taker); break;
        case 3 :
            for (int i =0; i < 4; i++)
            {
                if (_Players[i]->Update_belote(_trump)) _belote_announced = true;
            }
            _step2 += 1;
        break;
        default : break;
    }
    if (_step2 == 4)
    {
        _step2 =0;
        reset_images(li_aux);
        reset_images(li_text);
        return 1;
    }
    return 0;
}
int Jeu::Taking(list<Images*>& li_displayed,list<Images*> li_aux,list<Images*> li_text)
{
    int res =0;
    int j=0;
    Images* ptext = NULL;
    if (li_text.size() <= 3) return 0;
    _Cards.front()->add_display(li_displayed);
    switch (_step3)
    {
        case 0 :
            for (int i = 0; i < 4; i++)
            {
                _Players[i]->Update_score_take(_Cards.front(),_player_give);
            }
            _step3+=1;
            break;
        case 1:
            j = modulo(_player_give+1,4);
            _step3 += player_take(li_displayed,li_aux,li_text,j,true);
            if ((_array_taking[j] != -1)&&(_step3==2)) set_taker(j,true);
        break;
        case 2 :
            display_text_choice(li_displayed,li_text,1);
            j = modulo(_player_give+2,4);
            _step3 += player_take(li_displayed,li_aux,li_text,j,true);
            if ((_array_taking[j] != -1)&&(_step3==3)) set_taker(j,true);
        break;
        case 3:
            display_text_choice(li_displayed,li_text,2);
            j = modulo(_player_give+3,4);
            _step3 += player_take(li_displayed,li_aux,li_text,j,true);
            if ((_array_taking[j] != -1)&&(_step3==4)) set_taker(j,true);
        break;
        case 4:
            display_text_choice(li_displayed,li_text,3);
            j = modulo(_player_give,4);
            _step3 += player_take(li_displayed,li_aux,li_text,j,true);
            if ((_array_taking[j] != -1)&&(_step3==5)) set_taker(j,true);
        break;
        case 5 :
            for (int i = 0; i < 4; i++)
            {
                _array_taking.at(i) = -1;
            }
            reset_images(li_aux);
            reset_images(li_text);
            _step3+=1;
        break;
        case 6 :
            j = modulo(_player_give+1,4);
            _step3 += player_take(li_displayed,li_aux,li_text,j,false);
            if ((_array_taking[j] != -1)&&(_step3==7)) set_taker(j,false);
        break;
        case 7:
            display_text_choice(li_displayed,li_text,1);
            j = modulo(_player_give+2,4);
            _step3 += player_take(li_displayed,li_aux,li_text,j,false);
            if ((_array_taking[j] != -1)&&(_step3==8)) set_taker(j,false);
        break;
        case 8:
            display_text_choice(li_displayed,li_text,2);
            j = modulo(_player_give+3,4);
            _step3 += player_take(li_displayed,li_aux,li_text,j,false);
            if ((_array_taking[j] != -1)&&(_step3==9)) set_taker(j,false);
        break;
        case 9:
            display_text_choice(li_displayed,li_text,3);
            j = modulo(_player_give,4);
            _step3 += player_take(li_displayed,li_aux,li_text,j,false);
            if ((_array_taking[j] != -1)&&(_step3==10)) set_taker(j,false);
        break;
//If nobody has taken
        case 10 :
            ptext = li_text.front();
            _step3+=Put_cards_back(li_displayed,ptext);
        break;
        case 11 :
            _player_give = modulo(_player_give+1,4);
            res = -2;
        break;
//If someone has taken
        case 12 :
            li_aux.pop_front();
            li_aux.front()->reset();
            li_aux.front()->type("ok");
            _step3+=1;
        break;
        case 13 : _step3+= end_giving(li_displayed,li_aux);break;
        case 14 : res = 1; break;
        default : break;
    }
    if (res != 0)
    {
        _step3 = 0;
        reset_images(li_aux);
        reset_images(li_text);
    }
    return res;
}
int Jeu::end_giving(list<Images*>& li_displayed,list<Images*> li_aux)
{
    Images* ptext = NULL;
    Images* pbutton = NULL;
    ptext = li_aux.front();
    li_aux.pop_front();
    pbutton = li_aux.front();
    string str;
    if (_first_displayed1)
    {
        ptext ->SetPosition(_SCREEN_WIDTH/2.,_SCREEN_HEIGHT/3.,0);
        str = _Players[_array_taking[0]]->Getname();
        str += " a pris a ";
       switch (_trump)
       {
           case 0 : str += "carreau."; break;
           case 1 : str += "coeur."; break;
           case 2 : str += "pique."; break;
           case 3 : str += "trefle."; break;
           default : break;
       }
        ptext->type(str);
        pbutton->SetPosition(_SCREEN_WIDTH/2.,2*_SCREEN_HEIGHT/3.,0);
        pbutton->reset();
        li_displayed.push_back(ptext);
        li_displayed.push_back(pbutton);
        _first_displayed1 = false;
    }
    else
    {
        li_displayed.push_back(ptext);
        li_displayed.push_back(pbutton);
    }
    if (pbutton->click_on(true))
    {
        _first_displayed1=true;
        _Cards.front()-> reset();
        reset_images(li_aux);
        return 1;
    }
    return 0;
}
void Jeu::set_taker(int num_player,bool first_bet)
{
    _taker = num_player;
    if (first_bet) _trump = _Cards.front()->GetColour();
    else _trump = _array_taking[num_player];
    _display_trump.Set_color(_trump);
    _display_trump.Set_taker(_taker);
    _array_taking[0] = num_player;
    _step3=12;
    _player_give = modulo(_player_give+1,4);
}
int Jeu::player_take(list<Images*>& li_displayed,list<Images*> li_aux,list<Images*> li_text,int i,bool first_bet)
{
    if ((li_text.size()==0)||(i < 0)||(i>3)) return 0;
    Joueur* pplayer = _Players[i];
    Images* ptext = li_text.front();
    if (ptext == NULL) return 0;
    Carte* pcard=_Cards.front();
    if (first_bet) _array_taking.at(i) = pplayer -> bidding_first_round(li_displayed,li_aux,ptext,i,pcard,_first_displayed1);
    else _array_taking.at(i) = pplayer -> bidding_second_round(li_displayed,li_aux,ptext,i,pcard,_first_displayed1);
    if (ptext ->GetAlpha()==255)
    {
        _first_displayed1 = true;
        reset_images(li_aux);
        reset_images(li_text);
        return 1;
    }
    return 0;
}
void Jeu::display_text_choice(list<Images*>& li_displayed, list<Images*>& li_text, int n)
{
    Images* ptext;
    for (int i = 0; i < n; i++)
    {
        ptext = li_text.front();
        li_text.pop_front();
        li_displayed.push_back(ptext);
    }
}
int Jeu::Put_cards_back(list<Images*>& li_displayed, Images* ptext)
{
    list<Carte*> hand;
    li_displayed.push_back(ptext);
    if (_first_displayed1)
    {
        _Cards.front()->give_back(1,true);
        _first_displayed1=false;
        for (int i=0;i<4;i++)
        {
            _Players[modulo(i+_ind_player,4)] ->Give_back(_Cards);
        }
        ptext->type("Personne n'a pris, on redonne");
        ptext->SetPosition(_SCREEN_WIDTH/2.,_SCREEN_HEIGHT/3.,0);
    }
    bool arrived = true;
    for (list<Carte*>::iterator it = _Cards.begin(); it != _Cards.end();++it)
    {
        (*it)->add_display(li_displayed);
        arrived = (arrived)&&((*it)->Arrived());
    }
    if (arrived)
    {
        Center_and_Restart center_and_restart(_SCREEN_WIDTH,_SCREEN_HEIGHT);
        for_each(_Cards.begin(),_Cards.end(), center_and_restart);
        _first_displayed1=true;
        ptext -> reset();
        return 1;
    }
    return 0;
}
int Jeu::Play_trick(list<Images*>& li_displayed,list<Images*> li_aux,list<Images*> li_text,int number_trick)
{
    Joueur* pplayer=NULL;
    li_aux.pop_front();
    Images* pbutton = li_aux.front();
    Images* pbelote = NULL;
    switch (_step2)
    {
        case 0 :
            pbutton->SetPosition(_SCREEN_WIDTH/2,2*_SCREEN_HEIGHT/3,0);
            pbutton->reset();
            pbutton->type("ok !");
            _step2 += 1;
        break;
        case 1:
            pplayer = _Players[_ind_player];
            _step2 += pplayer -> PlayCard(li_displayed,pbutton,pbelote,_trick,_trump,_taker,number_trick,0,_first_displayed1);
            if (_step2== 2) _ind_player = modulo(_ind_player+1,4);
            display_pli(li_displayed);
        break;
        case 2:
            pplayer = _Players[_ind_player];
            _step2 += pplayer -> PlayCard(li_displayed,pbutton,pbelote,_trick,_trump,_taker,number_trick,1,_first_displayed1);
            if (_step2== 3) _ind_player = modulo(_ind_player+1,4);
            display_pli(li_displayed);
        break;
        case 3:
            pplayer = _Players[_ind_player];
            _step2 += pplayer -> PlayCard(li_displayed,pbutton,pbelote,_trick,_trump,_taker,number_trick,2,_first_displayed1);
            if (_step2== 4) _ind_player = modulo(_ind_player+1,4);
            display_pli(li_displayed);
        break;
        case 4:
            pplayer = _Players[_ind_player];
            _step2 += pplayer -> PlayCard(li_displayed,pbutton,pbelote,_trick,_trump,_taker,number_trick,3,_first_displayed1);
            if (_step2== 5) _ind_player = modulo(_ind_player+1,4);
            display_pli(li_displayed);
        break;
        case 5 :
            pbutton -> reset();
            pbutton->type("ok");
            pbutton->SetPosition(_SCREEN_WIDTH/2.,2*_SCREEN_HEIGHT/3. +20 ,0);
            _step2+=1;
            display_pli(li_displayed);
        break;
        case 6 : _step2 += confirmation_trick(li_displayed,pbutton); break;
        case 7 : _step2 += animation_gather_trick(li_displayed); break;
        case 8 : _step2 += animation_win_trick(li_displayed,number_trick); break;
        default : break;
    }
    if (_step2== 9)
    {
        _step2 = 0;
        reset_images(li_aux);
        reset_images(li_text);
        return 1;
    }
    return 0;
}
void Jeu::display_hands(list<Images*>& li_displayed)
{
    for (int i =0; i < 4; i++)
    {
        _Players[i]->add_display(li_displayed);
    }
}
void Jeu::display_pli(list<Images*>& li_displayed)
{
    for (int i = 0; i < 4; i++)
    {
        if (_trick[i] !=NULL) _trick[i]->add_display(li_displayed);
    }
}
int Jeu::confirmation_trick(list<Images*>&li_displayed, Images* pbutton)
{
    li_displayed.push_back(pbutton);
    display_pli(li_displayed);
    if (pbutton->click_on(true))
    {
        pbutton -> reset();
        return 1;
    }
    return 0;
}
int Jeu::animation_gather_trick(list<Images*>& li_displayed)
{
    display_pli(li_displayed);
    if (_first_displayed1)
    {
        go_trick(_SCREEN_WIDTH/2,_SCREEN_HEIGHT/2.);
        _first_displayed1 = false;
    }
    if (_trick[0]-> Arrived())
    {
        _first_displayed1 = true;
        return 1;
    }
    return 0;
}
int Jeu::animation_win_trick(list<Images*>& li_displayed, int number_trick)
{
    display_pli(li_displayed);
    if (_first_displayed1)
    {
        WhoWinTrick(number_trick);
        switch (_ind_player)
        {
            case 0 : go_trick(_SCREEN_WIDTH/2,_SCREEN_HEIGHT-120); break;
            case 1 : go_trick(_SCREEN_WIDTH -100,_SCREEN_HEIGHT/2.); break;
            case 2 : go_trick(_SCREEN_WIDTH/2,120); break;
            case 3 : go_trick(100,_SCREEN_HEIGHT/2.); break;
            default : break;
        }
        _first_displayed1 = false;
    }
    if (_trick[0]-> Arrived())
    {
        _first_displayed1 = true;
        set_previous_trick();
        return 1;
    }
    return 0;
}
void Jeu::set_previous_trick()
{
    array<Carte*,4> empty_trick = {NULL,NULL,NULL,NULL};
    _previous_trick = _trick;
    _previous_trick[0] -> SetPosition(_SCREEN_WIDTH/2., _SCREEN_HEIGHT/2.+80,0);
    _previous_trick[1] -> SetPosition(_SCREEN_WIDTH/2. +55, _SCREEN_HEIGHT/2.,0);
    _previous_trick[2] -> SetPosition(_SCREEN_WIDTH/2., _SCREEN_HEIGHT/2.-80,0);
    _previous_trick[3] -> SetPosition(_SCREEN_WIDTH/2.-55, _SCREEN_HEIGHT/2.,0);
    _trick = empty_trick;
}
void Jeu::go_trick(int w, int h)
{
    Carte* pcard = NULL;
    for (int i = 0; i < 4; i++)
    {
        pcard = _trick[i];
        pcard->Set_animate(w,h,0,0,0);
    }
}
void Jeu::WhoWinTrick(int number_trick)
{
    Joueur* pgagnant;
    pgagnant = _Players[0];
    array<Carte*,4> orderly_trick ={NULL,NULL,NULL,NULL};
    int gagnant = 0;
    for (int i =0;i<4;i++) {orderly_trick.at(i) = _trick[modulo(i+_ind_player,4)];}
    for (int i = 0; i < 4; i++)
    {
        pgagnant = _Players[i];
        pgagnant -> Update(orderly_trick,_trick[0],_trump,_ind_player);
    }
    gagnant = pgagnant ->Strongest_card(4,_trick,_trump);
    _ind_player = modulo(_ind_player+gagnant,4);
    pgagnant = _Players[_ind_player];
    pgagnant->win_trick(_trick);
    _trick = orderly_trick;
    if (number_trick==7) _last_ten = _ind_player %2;
}

void Jeu::declare_belote()
{
    //_belote_announced = true;
}
int Jeu::screen_end(list<Images*>& li_displayed,list<Images*> li_text)
{
    Images* ptext= NULL;
    if (_first_displayed1)
    {
//we define what we will display
        _first_displayed1=false;
        array<int,3> points = {0,0,0};
        points = CountPoints();
        //update the loaded value from player IA : problem with this fonction
        for (int i = 0; i < 4; i++)
        {
            _Players[i]->update_stat(points[2],_taker,_trump);
        }
    //score of the opposing team
        int score= points[0];
        string score1 = "sur la partie, le score de l'equipe adverse est ";
        stringstream score_1;
        score_1 << _team_scores[1];
        score1 += score_1.str().c_str();
        score1 += " points.";
    //score of my team
        string score0 = "sur la partie, le score de votre equipe est ";
        stringstream score_0;
        score_0 << _team_scores[0];
        score0 += score_0.str().c_str();
        score0 += " points.";
    //display of the result of the round
        string result = "";
        switch (score)
        {
            case 252 : result  = "vous avez gagne la manche par capot !"; break;
            case 162 : result  = "vous avez gagne la manche, ils sont dedans !"; break;
            case 0:
                if (_taker%2==0) result  = "vous etes dedans...";
                else result  = "vous etes capots...";
                break;
            default :
                if (points[0] >= points[1] ) result  = "vous avez gagne la manche avec ";
                else result  = "vous avez perdu la manche avec ";
                stringstream score_manche;
                score_manche << score;
                result  += score_manche.str().c_str();
                result  += " points.";
                break;
        }
        ptext = li_text.front();
        li_text.pop_front();
        ptext->type(result );
        ptext->SetPosition(_SCREEN_WIDTH/2.,_SCREEN_HEIGHT/3.,0);
        li_displayed.push_back(ptext);
        ptext = li_text.front();
        li_text.pop_front();
        ptext->type(score0);
        ptext->SetPosition(_SCREEN_WIDTH/2.,_SCREEN_HEIGHT/2.,0);
        li_displayed.push_back(ptext);
        ptext = li_text.front();
        li_text.pop_front();
        ptext->type(score1);
        ptext->SetPosition(_SCREEN_WIDTH/2.,2.*_SCREEN_HEIGHT/3.,0);
        li_displayed.push_back(ptext);
    }
    else
    {
        for (int i = 0; i < 3; i++) //we display the three texts on the window
        {
            ptext = li_text.front();
            li_text.pop_front();
            li_displayed.push_back(ptext);
        }
        if (li_displayed.front()->click_on(true))
        {
            reset_images(li_displayed);
            reset_images(li_text);
            return 1;
        }
    }
    return 0;
}
array<int,3> Jeu::CountPoints()
{
    for (int i = 0; i < 4; i++) //reset of the tricks
    {
        _trick.at(i) = NULL;
        _previous_trick.at(i) = NULL;
    }
    array<int,3> res = {0,0,0};
    int nb_cards_taker = 0;
    int nb_cards_opponants = 0;
    int score=0; //score of the team that has taken
    int score_2 = -1;
    int score_total = 162; //total number of points in the round, can increase if someone has the belote
    if (_taker%2==_last_ten) score += 10;
    for (int i = 0; i < 4; ++i)
    {
        if ((i%2)==(_taker%2)) score += _Players[i]->CountPoints(_trump,_Cards,nb_cards_taker);
        else score_2 += _Players[i]->CountPoints(_trump,_Cards,nb_cards_opponants);
        if (_belote_announced) if (_Players[i]->GetABelote()) _team_scores[i%2]+=20;
    }
    res.at(2) = score;
    if (nb_cards_taker == 32) //particular case of "capot"
    {
        _team_scores[_taker%2] += 252;
        res.at(_taker%2) = 252;
        res.at(1-_taker%2) = 0;
    }
    else
    {
        if (score >= score_total/2) //the team that has taken win
        {
            _team_scores[_taker%2] += score;
            _team_scores[1-_taker%2] += (score_total - score);
            res.at(_taker%2) = score;
            res.at(1-_taker%2) = score_total - score;
        }
        else
        {
            if (nb_cards_opponants == 32) //the team that has taken loose and didn't win any trick
            {
                _team_scores[1-_taker%2] += 252;
                res.at(1-_taker%2) = 252;
                res.at(_taker%2) = 0;
            }
            else //the team that has taken looses
            {
                _team_scores[1-_taker%2] += 162;
                res.at(_taker%2) = 0;
                res.at(1-_taker%2) = 162;
            }
        }
    }
    return res;
}
int Jeu::Play(list<Images*>& li_display,list<Images*>& li_aux,list<Images*>& li_text)
{
    switch(_step1)
    {
        case 0 : _step1 += clear(li_display,li_aux,li_text); break;
        //case 0 : _step1 +=1; break;
        case 1 :
            if (_first_displayed)
            {
                Beginning();
                _first_displayed = false;
            }
            else _step1+=Give(li_display,li_aux,li_text);
        break;
        case 2 : _step1+= Play_trick(li_display,li_aux,li_text,0); break;
        case 3 : _step1+= Play_trick(li_display,li_aux,li_text,1); break;
        case 4 : _step1+= Play_trick(li_display,li_aux,li_text,2); break;
        case 5 : _step1+= Play_trick(li_display,li_aux,li_text,3); break;
        case 6 : _step1+= Play_trick(li_display,li_aux,li_text,4); break;
        case 7 : _step1+= Play_trick(li_display,li_aux,li_text,5); break;
        case 8 : _step1+= Play_trick(li_display,li_aux,li_text,7); break;
        case 9 : _step1+= Play_trick(li_display,li_aux,li_text,7); break;
        case 10 :
            reset_images(li_display);
            reset_images(li_aux);
            reset_images(li_text);
            _step1 += 1;
            break;
        case 11 : _step1+= screen_end(li_display,li_text); break;
        case 12 : _step1 += clear(li_display,li_aux,li_text); break;
        default : break;
    }
    if (_step1==13)
    {
        _first_displayed = true;
        _step1 = 0;
        return 1;
    }
    return 0;
}
int Jeu::update_nb_cards()
{
    for (int i = 0; i < 4; i++)
    {
        _Players[i]->update_nb_cards();
    }
    return 1;
}
int Jeu::clear(list<Images*>li_display,list<Images*> li_aux,list<Images*> li_text)
{
//reset of the data specific to the round only
    _trump=-1;
    _display_trump.Set_color(_trump);
    _taker=-1;
    _display_trump.Set_taker(_taker);
    _belote_announced=false;
    _last_ten= -1;
//reset of the data specific to players only
    Joueur* pplayer;
    for (int i = 0;i<4;i++)
    {
        pplayer = _Players[i];
        pplayer -> clear();
    }
//reset of the data specific to the game
    _first_displayed = true;
    _first_displayed1 = true;
    _step1 = 0;
    _step2 = 0;
    _step3 = 0;
//reset of the data specific to cards
    Center_and_Restart center_and_restart(_SCREEN_WIDTH,_SCREEN_HEIGHT);
    for_each(_Cards.begin(),_Cards.end(),center_and_restart);
//reset of the data specific to images
    reset_images(li_display);
    reset_images(li_aux);
    reset_images(li_text);
    return 1;
}
void Jeu::reset_images(list<Images*> li_images)
{
    for (list<Images*>::iterator it = li_images.begin(); it != li_images.end(); ++it)
    {
        if (*it != NULL) (*it)->reset();
    }
}
void Jeu::PlayGame(int nb_human_players)
{
    init(nb_human_players);

    SDL_Color red = {255,0,0};
    SDL_Color white = {255,255,255};
    SDL_Color yellow = {255,255,0};
    Images FOND (_SCREEN_WIDTH,_SCREEN_HEIGHT,(string) "images/tapis_moyen_2.jpg", *_pEvent);

    Button jouer("jouer !",*_pEvent);
    jouer.SetPosition(_SCREEN_WIDTH/2.,3*_SCREEN_HEIGHT/4.,0);
    Button ok("ok!",*_pEvent);
    ok.SetPosition(_SCREEN_WIDTH/2.,3*_SCREEN_HEIGHT/4.,0);
    Text what_name((string) "fonts/Starjedi.ttf",24,red,*_pEvent);
    what_name.SetPosition(_SCREEN_WIDTH/2.,_SCREEN_HEIGHT/3.,0);
    Text set_name((string) "fonts/Starjedi.ttf",24,white,*_pEvent);
    set_name.SetPosition(_SCREEN_WIDTH/2.,_SCREEN_HEIGHT/2.,0);

    Text quit((string) "fonts/Starjedi.ttf",24,red,*_pEvent);
    quit.SetPosition(_SCREEN_WIDTH/2.,_SCREEN_HEIGHT/3.,0);
    quit.type((string) "Etes vous certain(e) de vouloir quitter ?");
    Button oui("oui",*_pEvent);
    oui.SetPosition(_SCREEN_WIDTH/2.,_SCREEN_HEIGHT/2.,0);
    Button non ("non",*_pEvent);
    non.SetPosition(_SCREEN_WIDTH/2.,2*_SCREEN_HEIGHT/3.,0);
    Text text_fin_manche("fonts/Starjedi.ttf",24,red,*_pEvent);
    text_fin_manche.SetPosition(_SCREEN_WIDTH/2.,_SCREEN_HEIGHT/3.,0);
    text_fin_manche.type("voulez-vous refaire une partie ?");

    list<Images*> li_aux;
    list<Images*> li_text;
    Text un0 ("fonts/Starjedi.ttf",18,yellow,*_pEvent);
    li_text.push_back(&un0);
    Text un1 ("fonts/Starjedi.ttf",18,yellow,*_pEvent);
    li_text.push_back(&un1);
    Text un2 ("fonts/Starjedi.ttf",18,yellow,*_pEvent);
    li_text.push_back(&un2);
    Text un3 ("fonts/Starjedi.ttf",18,yellow,*_pEvent);
    li_text.push_back(&un3);
    Text nobody("fonts/Starjedi.ttf",18,red,*_pEvent);
    nobody.SetPosition(_SCREEN_WIDTH/2.,_SCREEN_HEIGHT/3.,0);
    li_aux.push_back(&nobody);
    Button b1 ("ok", *_pEvent);
    Button b2 ("ok", *_pEvent);
    Button b3 ("ok", *_pEvent);
    Button b4 ("ok", *_pEvent);
    li_aux.push_back(&b1);
    li_aux.push_back(&b2);
    li_aux.push_back(&b3);
    li_aux.push_back(&b4);

    Text merci((string) "fonts/Starjedi.ttf",40,red,*_pEvent);
    merci.SetPosition(_SCREEN_WIDTH/2.,_SCREEN_HEIGHT/2.,0);
    merci.type((string) "merci d'avoir joue !");

    Text previsou_trick("fonts/Starjedi.ttf",16,yellow,*_pEvent);
    previsou_trick.SetPosition(_SCREEN_WIDTH/2.,_SCREEN_HEIGHT/3.-20,0);
    previsou_trick.type((string) "voici le dernier pli :");

    list<Images*> li_images;
    list<Images*> li_basic;
    list<Images*> li_quit;
    list<Images*> li_thanks;
    li_basic.push_back(&FOND);
    li_quit.push_back(&FOND);
    li_quit.push_back(&quit);
    li_quit.push_back(&oui);
    li_quit.push_back(&non);
    li_thanks.push_back(&FOND);
    li_thanks.push_back(&merci);
//variables used in the statement
    unsigned int i = 0;
    int where = 0;
    Images* pimage=NULL;
    Images* pimage2=NULL;
    string a="";
    string question = "quel nom pour le joueur ";
    bool continuer = true;
    bool continuer_quit = true;
//reset of lists of images
    reset_images(li_images);
    reset_images(li_aux); //un texte, 4 boutons
    reset_images(li_text);//4 textes
    reset_images(li_thanks);
    reset_images(li_quit);
    reset_images(li_basic);
    //Display_trunk trunkk(1);
//main statement
    while (continuer)
    {
        continuer_quit = true;
        li_images = li_basic; //to display only the green background
//determination of what must be displayed
        switch (i)
        {
            case 0 : i+=set_names(li_images,jouer,ok,where,what_name,set_name); break;
            case 1 :
                for (int j = 0; j < 4; j++)
                {
                    pimage = _Players[j]->GetpImage();
                    pimage2 = _Players[j]->GetpNameSurface();
                    li_basic.push_back(pimage);
                    li_basic.push_back(pimage2);
                }
                i+=1;
            break;
            case 2 :
                i+=Play(li_images,li_aux,li_text);
                _display_trump.Set_names( _Players[0]->Getname(),_Players[1]->Getname(),_Players[2]->Getname(),_Players[3]->Getname());
                display_hands(li_images);
                break;
            case 3 :
                reset_images(li_images);
                reset_images(li_aux);
                reset_images(li_text);
                reset_images(li_thanks);
                reset_images(li_quit);
                //reset_images(li_basic);
                i+= 1;
                break;
            case 4 :
                li_images.push_back(&oui);
                li_images.push_back(&non);
                li_images.push_back(&text_fin_manche);
                if (oui.click_on(true)) i = 2;
                if (non.click_on(true))
                {
                    continuer = false;
                    continuer_quit = false;
                    i = 5;
                }
            break;
            default :
                continuer = false;
                continuer_quit = false;
            break;
        }
//events management
        while (SDL_PollEvent(_pEvent))
        {
            if(_pEvent->type == SDL_QUIT) continuer = false;
            update(li_images);
            _display_trump.Update();
        }
//Displaying
        if (continuer)
        {
            if (_display_trump.Mouse_over()) display_old_trick(li_images,&previsou_trick);
            apply_surfaces (li_images,_pScreen);
            _display_trump.Apply_sprite(_pScreen);
            SDL_Flip( _pScreen );
        }
//If we quit the game
        else
        {
            oui.reset();
            non.reset();
            while (continuer_quit)
            {
                while(SDL_PollEvent(_pEvent))
                {
                    if( _pEvent->type == SDL_QUIT ) continuer_quit = false;
                    update(li_quit);
                }
                if (oui.click_on(true)) continuer_quit = false;
                if (non.click_on(true))
                {
                    continuer_quit = false;
                    continuer = true;
                }
                apply_surfaces(li_quit,_pScreen);
                SDL_Flip( _pScreen );
            }
        }
    }
    continuer = true;
    while (continuer)
    {
        while( SDL_PollEvent( _pEvent ) )
        {
            if (_pEvent->type == SDL_QUIT ) continuer = false;
            if (_pEvent->type == SDL_MOUSEBUTTONDOWN) continuer = false;
            if (_pEvent->type == SDL_KEYDOWN) continuer = false;
        }
        apply_surfaces(li_thanks,_pScreen);
        SDL_Flip( _pScreen );
    }
    delete_players_cards();
}
void Jeu::apply_surfaces(list<Images*> li,SDL_Surface* destination)
{
    for (list<Images*>::iterator it = li.begin(); it != li.end(); ++it)
    {
        (*it)->Apply_surface(destination);
    }
}
int Jeu::set_names(list<Images*>& li_displayed,Button& jouer,Button& ok, int& where, Text& what_name, Text& set_name)
{
    Images* pimage=NULL;
    Images* pimage2=NULL;
    string a;
    string question = "quel nom pour le joueur ";
    int where_begin = where;
    if (where == 5) where=0;
    for (int i = 0; i < 4; i++)
    {
        pimage = _Players[i]->GetpImage();
        pimage2 = _Players[i]->GetpNameSurface();
        li_displayed.push_back(pimage);
        li_displayed.push_back(pimage2);
        if ((pimage->click_on(false))||(pimage2->click_on(false))) where = i+1;
        if (where == i+1)
        {
            li_displayed.push_back(&ok);
            if (where != where_begin) set_name.remove_text();
            switch (i)
            {
                case 0 : a="0 ?"; break;
                case 1 : a="1 ?"; break;
                case 2 : a="2 ?"; break;
                case 3 : a="3 ?"; break;
                default : break;
            }
            what_name.type(question+a);
            li_displayed.push_back(&what_name);
            li_displayed.push_back(&set_name);
            if (ok.click_on(true))
            {
                if (set_name.GetText() !="") _Players[i]->setName(set_name.GetText());
                where = 5;
            }
        }
        pimage=NULL;
        pimage2=NULL;
    }
    if (where==0) li_displayed.push_back(&jouer);
    if (jouer.click_on(true)&&(where==0)) return 1;
    return 0;
}
void Jeu::update(list<Images*>& li_images)
{
    for (list<Images*>::iterator it = li_images.begin(); it!=li_images.end(); ++it)
    {
        if (*it != NULL) (*it)->update();
    }
}
void Jeu::init(int nb_joueur_humain)
{
    new_players(nb_joueur_humain);
    Cards_generation();
}
int Jeu::ask_number_human_player()
{
    return 1;
}
void Jeu::display_old_trick(std::list<Images*>& li_displayed,Images* ptext)
{
    Carte* pcard = NULL;
    for (int i = 0; i < 4; i++)
    {
        pcard = _previous_trick[i];
        if (pcard != NULL) pcard->add_display(li_displayed);
    }
    li_displayed.push_back(ptext);
}
