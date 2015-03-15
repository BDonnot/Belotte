#include "Score_take.h"
#include "Foncteurs_Cartes.h"
#include "Foncteurs_Datas.h"
#include <algorithm>
using namespace std;
Score_take::Score_take()
{
    //ctor
}

Score_take::Score_take(int type) : //ctor
_type(type)
,_eval_qlt_card("datas/_a_eval_qlt_card.csv",_type)
,_score_1("datas/_a_score_1.csv",_type)
,_bonus_pos_1("datas/_a_bonus_pos_1.csv",_type)
,_bonus_pos_2("datas/_a_bonus_pos_2.csv",_type)
,_bonus_nb_trumps("datas/_a_bonus_nb_trumps.csv",_type)
,_eval_qlt_trump("datas/_a_eval_qlt_trump.csv",_type)
,_eval_qlt_trump_2("datas/_a_eval_qlt_trump_2.csv",_type)
,_score_pcard_given("datas/_a_eval_score_pcard_given.csv",_type)
,_bonus_longe("datas/_a_bonus_longe.csv",_type)
{
    _to_update.push_back(&_score_1);
    _to_update.push_back(&_eval_qlt_card);
    _to_update.push_back(&_score_pcard_given);
    _to_update.push_back(&_bonus_pos_1);
    _to_update.push_back(&_bonus_pos_2);
    _to_update.push_back(&_bonus_nb_trumps);
    _to_update.push_back(&_eval_qlt_trump);
    _to_update.push_back(&_eval_qlt_trump_2);
    _to_update.push_back(&_bonus_longe);
}
void Score_take::Set_hand(list<Carte*> hand,Carte* proposed_card,int position)
{
    _hand = hand;
    sort(); //sort the 'hand'
    _proposed_card = proposed_card;
}
Score_take::~Score_take()
{
    //dtor
}
int Score_take::score_1(Carte* pcard_given,int color) //score of the card returned
{
    if (pcard_given == NULL) return -10000;
    switch(pcard_given->GetHeight())
    {
        case 0 : return _score_1.value(0,color);
        case 1 : return _score_1.value(1,color);
        case 2 : return _score_1.value(2,color);
        case 3 : return _score_1.value(3,color);
        case 4 : return _score_1.value(4,color);
        case 5 : return _score_1.value(5,color);
        case 6 : return _score_1.value(6,color);
        case 7 : return _score_1.value(7,color);
        default : break;
    }
	return -100;
}
double Score_take::score_1(Carte* pcard_given,Obtain_value& value) //score of the card returned
{
    if (pcard_given == NULL) return -10000;
    switch(pcard_given->GetHeight())
    {
        case 0 : return value(_score_1,0);
        case 1 : return value(_score_1,1);
        case 2 : return value(_score_1,2);
        case 3 : return value(_score_1,3);
        case 4 : return value(_score_1,4);
        case 5 : return value(_score_1,5);
        case 6 : return value(_score_1,6);
        case 7 : return value(_score_1,7);
    }
	return -100;
}
int Score_take::bonus_pos_1(int i,int color)
{
    switch(i)
    {
        case 0 : return _bonus_pos_1.value(0,color);
        case 1 : return _bonus_pos_1.value(1,color);
        case 2 : return _bonus_pos_1.value(2,color);
        case 3 : return _bonus_pos_1.value(3,color);
        default : break;
    }
	return -100;
}
double Score_take::bonus_pos_1(int i,Obtain_value& value)
{
    switch(i)
    {
        case 0 : return value(_bonus_pos_1,0);
        case 1 : return value(_bonus_pos_1,1);
        case 2 : return value(_bonus_pos_1,2);
        case 3 : return value(_bonus_pos_1,3);
        default : break;
    }
	return -100;
}
int Score_take::bonus_pos_2(int i,int color)
{
    switch(i)
    {
        case 0 : return _bonus_pos_2.value(0,color);
        case 1 : return _bonus_pos_2.value(1,color);
        case 2 : return _bonus_pos_2.value(2,color);
        case 3 : return _bonus_pos_2.value(3,color);
        default : break;
    }
	return -100;
}
double Score_take::bonus_pos_2(int i,Obtain_value& value)
{
    switch(i)
    {
        case 0 : return value(_bonus_pos_2,0);
        case 1 : return value(_bonus_pos_2,1);
        case 2 : return value(_bonus_pos_2,2);
        case 3 : return value(_bonus_pos_2,3);
        default : break;
    }
	return -100;
}
int Score_take::bonus_nb_trumps(int nb,int color)
{
    switch(nb)
    {
        case 0 : return _bonus_nb_trumps.value(0,color);
        case 1 : return _bonus_nb_trumps.value(1,color);
        case 2 : return _bonus_nb_trumps.value(2,color);
        case 3 : return _bonus_nb_trumps.value(3,color);
        case 4 : return _bonus_nb_trumps.value(4,color);
        case 5 : return _bonus_nb_trumps.value(5,color);
        case 6 : return _bonus_nb_trumps.value(6,color);
        case 7 : return _bonus_nb_trumps.value(7,color);
        case 8 : return _bonus_nb_trumps.value(8,color);
        default : break;
    }
	return -100;
}
double Score_take::bonus_nb_trumps(int nb,Obtain_value& value)
{
    switch(nb)
    {
        case 0 : return value(_bonus_pos_2,0);
        case 1 : return value(_bonus_pos_2,1);
        case 2 : return value(_bonus_pos_2,2);
        case 3 : return value(_bonus_pos_2,3);
        case 4 : return value(_bonus_pos_2,4);
        case 5 : return value(_bonus_pos_2,5);
        case 6 : return value(_bonus_pos_2,6);
        case 7 : return value(_bonus_pos_2,7);
        case 8 : return value(_bonus_pos_2,8);
        default : break;
    }
	return -100;
}
int Score_take::bonus_qlt_game(int color_tested,bool first_round,bool memorize)
{
    int res =0;
    list<Carte*> all_cards_color;
    int color = _hand.front()->GetColour();
    for (list<Carte*>::iterator it = _hand.begin(); it != _hand.end();++it)
    {
        if ((*it)->GetColour()==color) all_cards_color.push_back((*it));
        else
        {
            if (color == color_tested) res += eval_qlt_color_as_trump(all_cards_color,first_round,memorize);
            else res += eval_qlt_color(all_cards_color,memorize);
            color = (*it)->GetColour();
            all_cards_color.clear();
            all_cards_color.push_back(*it);
        }
    }
    return res;
}
int Score_take::eval_qlt_color(list<Carte*>& li_color,bool memorize)
{
    int res = 0,pos=0;
    int color = -1;
    if ((memorize)&&(li_color.size()>0)) color = li_color.front()->GetColour();
    res += bonus_longe(li_color.size(),color);
    for (list<Carte*>::iterator it = li_color.begin(); it != li_color.end(); ++it)
    {
        pos++;
        res += eval_qlt_card(*it,pos,li_color.size(),color);
    }
    return res;
}
int Score_take::eval_qlt_card(Carte* pcard,int pos,int nb_cards, int color)
{
    int res = 0;
    int height = pcard->GetHeight();

    switch(height)
    {
        case 0: //7
            return _eval_qlt_card.value(0,color);
        case 1: //8
            return _eval_qlt_card.value(1,color);
        case 2: //9
            return _eval_qlt_card.value(2,color);
        case 3: //10
            if (pos==2) res += _eval_qlt_card.value(3,color);
            else
            {
                if (nb_cards==1) res += _eval_qlt_card.value(4,color);
                else  res += _eval_qlt_card.value(5,color);
            }
            break;
        case 4: //jack
            return _eval_qlt_card.value(6,color);
        case 5: //queen
            return _eval_qlt_card.value(7,color);
        case 6: //king
            res += _eval_qlt_card.value(8,color);
            if (pos == 3) res += _eval_qlt_card.value(9,color);
            if (pos == 2) res += _eval_qlt_card.value(10,color);
            break;
        case 7: //ace
            return _eval_qlt_card.value(11,color);
        default : break;
    }
    return res;
}
int Score_take::eval_qlt_color_as_trump(list<Carte*> li_trumps,bool first_round,bool memorize)
{
    int res = 0,pos=0,size = li_trumps.size();
    int color = -1,color_bis=-1;
    /*if (size > 0)
    {
        Sort_Cards foncteur(li_trumps.front()->GetColour());
        if (memorize) color = li_trumps.front()->GetColour();
    }
    else Sort_Cards foncteur(-1);*/
    if (size > 0)
    {
        color = li_trumps.front()->GetColour();
        if (memorize) color_bis = color;
    }
    Sort_CardsOld foncteur(color);
    li_trumps.sort(foncteur);
    res += bonus_nb_trumps(size+1*first_round,color_bis);
    int (Score_take::*f) (Carte*,int,int); //Select the right fonction
    if (first_round) f = &Score_take::eval_qlt_trump;
    else f = &Score_take::eval_qlt_trump_2;
    for (list<Carte*>::iterator it = li_trumps.begin(); it != li_trumps.end(); ++it)
    {
        pos++;
        res += (this->*f)(*it,pos,color_bis);
    }
    return res;
}
int Score_take::eval_qlt_trump(Carte* pcard,int pos,int color)
{
    int res = 0;
    int height = pcard->GetHeight();
    int height_given = _proposed_card->GetHeight();
    switch(height)
    {
        case 0: //7
            return  _eval_qlt_trump.value(0,color);
        case 1: //8
            return  _eval_qlt_trump.value(1,color);
        case 2: //9
            if (pos==2) res += _eval_qlt_trump.value(2,color);
            else res += _eval_qlt_trump.value(3,color);
            break;
        case 3: //10
            return  _eval_qlt_trump.value(4,color);
        case 4: //jack
            res += _eval_qlt_trump.value(5,color);
            if (height_given==2) res += _eval_qlt_trump.value(6,color);
            if (height_given==7) res += _eval_qlt_trump.value(7,color);
            break;
        case 5: //queen
            return  _eval_qlt_trump.value(8,color);
        case 6: //king
            return  _eval_qlt_trump.value(9,color);
        case 7: //ace
            if (pos==3) res += _eval_qlt_trump.value(10,color);
            else
            {
                if ((pos ==2)&&((height_given==4)||(height_given==2))) res += _eval_qlt_trump.value(11,color);
                else res += _eval_qlt_trump.value(12,color);
            }
            break;
        default : break;
    }
    return res;
}
int Score_take::eval_qlt_trump_2(Carte* pcard,int pos,int color)
{
    int res = 0;
    int height= pcard->GetHeight();
    switch(height)
    {
        case 0: //7
            return _eval_qlt_trump_2.value(0,color);
        case 1: //8
            return _eval_qlt_trump_2.value(1,color);
        case 2: //9
            if (pos==2) res += _eval_qlt_trump_2.value(2,color);
            else res += _eval_qlt_trump_2.value(2,color);
            break;
        case 3: //10
            return _eval_qlt_trump_2.value(4,color);
        case 4: //jack
            return _eval_qlt_trump_2.value(5,color);
        case 5: //queen
            return _eval_qlt_trump_2.value(6,color);
        case 6: //king
            return _eval_qlt_trump_2.value(7,color);
        case 7: //ace
            if (pos==3) res += _eval_qlt_trump_2.value(8,color);
            else
            {
                if (pos ==2)  res += _eval_qlt_trump_2.value(9,color);
                else res += _eval_qlt_trump_2.value(10,color);
            }
            break;
        default : break;
    }
    return res;
}
int Score_take::score_pcard_given(Carte* pcard,int color)
{
    int res = 0;
    switch(pcard->GetHeight())
    {
        case 0: //7
            return _score_pcard_given.value(0,color);
        case 1: //8
            return _score_pcard_given.value(1,color);
        case 2: //9
            return _score_pcard_given.value(2,color);
        case 3: //10
            res += _score_pcard_given.value(3,color);
            if (!has_colour(pcard->GetColour())) res += _score_pcard_given.value(4,color);
            if (has_card(pcard->GetColour(),7)) res += _score_pcard_given.value(5,color);
            break;
        case 4: //jack
            return _score_pcard_given.value(6,color);
        case 5: //queen
            return _score_pcard_given.value(7,color);
        case 6: //king
            return _score_pcard_given.value(8,color);
        case 7: //ace
            res += _score_pcard_given.value(9,color);
            if (has_card(pcard->GetColour(),3)) res += _score_pcard_given.value(10,color);
            break;
        default : return 0;
    }
    return res;
}
bool Score_take::has_colour(int colour)
{
    for (list<Carte*>::iterator it = _hand.begin(); it != _hand.end(); ++it)
    {
        if ((*it)->GetColour() == colour) return true;
    }
    return false;
}
bool Score_take::has_card(int colour,int height)
{
    for (list<Carte*>::iterator it = _hand.begin(); it != _hand.end(); ++it)
    {
        if (((*it)->GetColour() == colour)&&((*it)->GetHeight()==height)) return true;
    }
    return false;
}
int Score_take::Eval_score(int color, bool first_round)
{
    int res = 0;
    res += eval_score_beginning(color,first_round,false);
    res += bonus_qlt_game(color,first_round,false); //bonus (or malus) if the game is good (or bad)
    return res;
}
int Score_take::eval_score_beginning(int color,bool first_round, bool memorize)
{
    int res = 0,color_aux = -1;
    if (memorize) color_aux = color;
    if (first_round)
    {
        res += score_1(_proposed_card,color_aux); //bonus if the card face up has a hight height
        res += bonus_pos_1(_position,color_aux); //bonus if I play first or my teammate
    }
    else
    {
        res += score_pcard_given(_proposed_card,color_aux); //bonus if the card face up has a hight height
        res += bonus_pos_2(_position,color_aux); //bonus if I play first or my teammate
    }
    return res;
}
void Score_take::sort()
{
    Sort_CardsOld foncteur(-1);
    _hand.sort(foncteur);
}
int Score_take::bonus_longe(int nb,int color)
{
    switch(nb)
    {
        case 0 : return _bonus_longe.value(0,color);
        case 1 : return _bonus_longe.value(1,color);
        case 2 : return _bonus_longe.value(2,color);
        case 3 : return _bonus_longe.value(3,color);
        case 4 : return _bonus_longe.value(4,color);
        case 5 : return _bonus_longe.value(5,color);
        case 6 : return _bonus_longe.value(6,color);
        case 7 : return _bonus_longe.value(7,color);
        case 8 : return _bonus_longe.value(8,color);
        default : break;
    }
    return -1000;
}
void Score_take::Update(int color,Update_Stats& update_stat)
{
    bool first_round = false;
    if (color==_proposed_card->GetColour()) first_round = true;
    eval_score_beginning(color,first_round,true);
    bonus_qlt_game(color,first_round,true);
    for_each(_to_update.begin(),_to_update.end(),update_stat);
}
void Score_take::Human_choice_taking(int color,bool first_round) //higher the score when the human choose to take.
{
    int color_modif = color;
    if (first_round) color_modif = _proposed_card->GetColour();
    Update_Stats foncteur(20,color_modif);
    eval_score_beginning(color_modif,first_round,true);
    bonus_qlt_game(color_modif,first_round,true);
    for_each(_to_update.begin(),_to_update.end(),foncteur);
}
