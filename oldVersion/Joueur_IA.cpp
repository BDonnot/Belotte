#include "Joueur_IA.h"
#include "fonctions_annexes.h"
#include "Foncteurs_Datas.h"
#include <algorithm>
#include <fstream>
using namespace std;

Joueur_IA::Joueur_IA():Joueur(),_scores_min("datas/_a_scores_min.csv",_type)
{
    _type = 0;
    clear();
    //load();
}
Joueur_IA::Joueur_IA(string name,int type): Joueur(name),_type(type),_scores_min("datas/_a_scores_min.csv",_type),_play_1st("datas/_a_eval_score_play_1st.csv",_type),
_play_2nd("datas/_a_eval_score_play_2nd.csv",_type),_play_3rd("datas/_a_eval_score_play_3rd.csv",_type),_play_4th("datas/_a_eval_score_play_4th.csv",_type)
{
    clear();
    //load();
}
void Joueur_IA::clear()
{
    _hand.clear();
    _tricks_won.clear();
    _cards_fallen.clear();
    _playable_cards.clear();
    array<bool,16> info_who_cut_what = {false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false};
    _info_who_cut_what = info_who_cut_what;
    _resting_trump = 8;
    array<int,4> one = {1,1,1,1};
    _color_master = one;
    array<int,4> eight = {8,8,8,8};
    _nb_resting_cards = eight;
    array<int,16> none = {-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1};
    _calls = none;
    array<int,4> zero = {0,0,0,0};
    _nb_color_played = zero;
    _nb_resting_card_color = zero;
    _res = 0;
    _has_taken = false;
    _taking_round = -1;
     //machine learning
     //reset_datas();
}
void Joueur_IA::load()
{
//machine learning
    /*Datas score_1_aux("datas/_a_score_1.csv",_type);
    _score_1 = score_1_aux;
    Datas eval_qlt("datas/_a_eval_qlt_card.csv",_type);
    _eval_qlt_card = eval_qlt;
    Datas pcard_given("datas/_a_eval_score_pcard_given.csv",_type);
    _score_pcard_given = pcard_given;
    Datas bonu_pos_1("datas/_a_bonus_pos_1.csv",_type);
    _bonus_pos_1 = bonu_pos_1;
    Datas bonu_pos_2("datas/_a_bonus_pos_2.csv",_type);
    _bonus_pos_2 = bonu_pos_2;
    Datas bonus_trumps("datas/_a_bonus_nb_trumps.csv",_type);
    _bonus_nb_trumps = bonus_trumps;*/
   /* Datas play_1st("datas/_a_eval_score_play_1st.csv",_type);
    _play_1st = play_1st;
    Datas play_2nd("datas/_a_eval_score_play_2nd.csv",_type);
    _play_2nd = play_2nd;
    Datas play_3rd("datas/_a_eval_score_play_3rd.csv",_type);
    _play_3rd = play_3rd;
    Datas play_4th("datas/_a_eval_score_play_4th.csv",_type);
    _play_4th = play_4th;*//*
    Datas qlt_trumps("datas/_a_eval_qlt_trump.csv",_type);
    _eval_qlt_trump = qlt_trumps;
    Datas qlt_trumps_2("datas/_a_eval_qlt_trump_2.csv",_type);
    _eval_qlt_trump_2 = qlt_trumps_2;
    Datas score_min("datas/_a_scores_min.csv",_type);
    _scores_min = score_min;
    _to_update.push_back(&_score_1);
    _to_update.push_back(&_eval_qlt_card);
    _to_update.push_back(&_score_pcard_given);
    _to_update.push_back(&_bonus_pos_1);
    _to_update.push_back(&_bonus_pos_2);
    _to_update.push_back(&_bonus_nb_trumps);
    _to_update.push_back(&_eval_qlt_trump);
    _to_update.push_back(&_eval_qlt_trump_2);*/
}
void Joueur_IA::reset_datas()
{
    Reset_Stats reset;
    //for_each(_to_update.begin(),_to_update.end(),reset);
}
Joueur_IA::~Joueur_IA()
{
    //dtor
}
bool Joueur_IA::I_take_1st_round(int position, Carte* pcard_given, bool& first_displayed1)
{
    first_displayed1= false;
    //_score_take.Set_hand(_hand,pcard_given,position);
    int color_given = pcard_given->GetColour();
    int score = _score_take.Eval_score(color_given,true);
    score += 10*Update_belote(color_given); //bonus if I have the belote.
    return (score > _scores_min.value(0,color_given));
}
int Joueur_IA::I_take_2nd_round(int position, Carte* pcard_given)
{
    int color_given = pcard_given->GetColour();
    int i1 = modulo(color_given+1,4);
    int i2 = modulo(color_given+2,4);
    int i3 = modulo(color_given+3,4);
    int score=0;
    int color = i1;
    int score_max = _score_take.Eval_score(i1,false);
    score_max += 10*Update_belote(i1);
    score = _score_take.Eval_score(i2,false);
    score += 10*Update_belote(i2);
    if (score>score_max)
    {
        score_max = score;
        color = i2;
    }
    score = _score_take.Eval_score(i3,false);
    score += 10*Update_belote(i3);
    if (score>score_max)
    {
        score_max = score;
        color = i3;
    }
    if (score_max > _scores_min.value(1,color)) return color;
    else return -1;
}
Carte* Joueur_IA::choose_card(array<Carte*,4> trick,int color_trump,int player_take,int number_trick,int position)
{
    Carte* res = NULL;
    int score_max = 0;
    Carte* pcard= NULL;
    int score = 0,i_master,color_asked = 0,height_master = 0,pseudo_height_master;
    bool pisse=false;
    bool play_trump = false;
    int team_taken = player_take%2;
    if (position ==0)
    {
        res = enter(color_trump,player_take,team_taken,number_trick);
    }
    else
    {
        _playable_cards = playable_cards(trick,color_trump);
        i_master = Strongest_card(position,trick,color_trump);
        height_master = trick[i_master]->GetHeight();
        pseudo_height_master = trick[i_master]->convertion(color_trump);
        color_asked=trick[0]->GetColour();
        for (list<Carte*>::iterator it = _playable_cards.begin(); it != _playable_cards.end(); ++it)
        {
            pcard = *it;
            if (( pcard->GetColour() != color_asked)&&( pcard->GetColour() != color_trump)) pisse = true;
            if ( pcard->GetColour() == color_trump) play_trump = true;
        }
        res = _playable_cards.front();
        //_playable_cards.pop_front();
        score_max=score_play(res,trick,color_trump,player_take,team_taken,position,i_master,pisse,play_trump,number_trick,color_asked,height_master,pseudo_height_master);
        //for (int i = 1; i <n ; i++)
        for (list<Carte*>::iterator it = (++_playable_cards.begin()); it != _playable_cards.end();++it)
        {
            //pcard = _playable_cards.front();
            //_playable_cards.pop_front();
            pcard = *it;
            score = score_play(pcard,trick,color_trump,player_take,team_taken,position,i_master,pisse,play_trump,number_trick,color_asked,height_master,pseudo_height_master);
            if (score>score_max)
            {
                res = pcard;
                score_max=score;
            }
        }
    }
    if (res == NULL) res = playable_cards(trick,color_trump).front();
    _nb_resting_card_color[res->GetColour()] -= 1;
    _playable_cards.clear();
    return res;
}
void Joueur_IA::Update(array<Carte*,4> trick_ordered,Carte* pfirst_card, int color_trump,int ind_first_player)
{
    Carte* pcard = NULL;
    int color = pfirst_card->GetColour();
    _nb_color_played[trick_ordered[ind_first_player]->GetColour()]+= 1;
    for (int i = 0; i <4; i++)
    {
        pcard =trick_ordered[i];
        _cards_fallen.push_front(pcard);
        if (pcard->GetColour()!=color)  _info_who_cut_what[pcard->GetColour()+4*i] = true;
        if ((pcard->GetColour()!=color)&&(pcard->GetColour()!=color_trump))  _calls[pcard->GetColour()+4*i] = true;
        if (pcard->GetColour()==color_trump) _resting_trump--;
        _nb_resting_cards[pcard->GetColour()] -= 1;
    }
    bool update = true;
    while (update)
    {
        update = false;
        for (list<Carte*>::iterator it = _cards_fallen.begin(); it !=_cards_fallen.end(); ++it)
        {
            if ((*it)->convertion(color_trump) == _color_master[(*it)->GetColour()])
            {
                _color_master[(*it)->GetColour()] +=1;
                update = true;
            }
        }
    }
}
Carte* Joueur_IA::enter(int color_trump,int player_take, int team_taken,int number_trick)
{
    list<Carte*>::iterator it = _hand.begin();
    Carte* res= *it;
    ++it;
    bool play_trump = must_play_trump(color_trump,team_taken);
    int pseudo_height_trump_max = _color_master[color_trump];
    Carte* pcard = NULL;
    int score_max = score_play_1st(res,color_trump,player_take,team_taken,play_trump,pseudo_height_trump_max,number_trick);
    int score;
    for (list<Carte*>::iterator itt=(++_hand.begin()); itt!=_hand.end(); ++itt)
    {
        pcard = *itt;
        score = score_play_1st(pcard,color_trump,player_take,team_taken,play_trump,pseudo_height_trump_max,number_trick);
        if (score > score_max)
        {
            res = pcard;
            score_max =score;
        }
    }
    return res;
}
bool Joueur_IA::must_play_trump(int color_trump,int team_taken)
{
    if ((_number%2) == team_taken) //If I am in the team that has taken
    {
        if (!(_info_who_cut_what[modulo(_number-1,4)*4+color_trump]&&_info_who_cut_what[modulo(_number+1,4)*4+color_trump])) //if opponents can still have trumps
        {
            if (_resting_trump != how_many_colour(color_trump)) return true; //If I don't have all the remaining trumps
        }
    }
    return false;
}
int Joueur_IA::score_play_1st(Carte* pcard, int color_trump,int player_take, int team_taken,bool play_trump,int pseudo_height_trump_max, int number_trick)
{
    int res=0;
    int color = pcard->GetColour();
    int height = pcard->GetHeight();
    int pseudo_height = pcard ->convertion(color_trump);
    if (pcard == NULL) return -10000;
//trumps case
    if (play_trump&&(color==color_trump))
    {
        res += _play_1st.value(0,0); //res = 20
        if (pseudo_height==pseudo_height_trump_max)
        {
            if (number_trick == 6) res += _play_1st.value(1,0); //res = -17
            else res += _play_1st.value(2,0); //res=100
        }
        if (height==3)  res += _play_1st.value(3,0); //ten;res=7
        if (height==2)  res += _play_1st.value(4,0); //nine;res=-20
        if (height==7)  res += _play_1st.value(5,0);; //Ace;res=-15
        if (height==4)  res += _play_1st.value(6,0); //Jack;res=100
    }
    if (!play_trump&&(color==color_trump)) res += _play_1st.value(7,0); //res=-15
//Case of friend call
    if (_calls[modulo(_number+2,4)*4+color]) //add an additional condition : if the teammate does a call and we are not the strongest in this colour
    {
        res += _play_1st.value(8,0); //res=5
        if (height==3) //10
        {
            res += _play_1st.value(9,0); //res=-10
            if (opponents_cut(color)) res += _play_1st.value(10,0); //res=-10
        }
        if (height==6) res += _play_1st.value(11,0); //res=8
        if (height==5) //queen
        {
            if (opponents_cut(color)) res += _play_1st.value(12,0); //res=-10
            else res += _play_1st.value(13,0); //res=7
        }
        if (height==4) res += _play_1st.value(14,0); //res=4
        if (_nb_color_played[color] == 0) res += _play_1st.value(15,0); //res=7
        if (opponents_cut(color))
        {
            if (_number%2 == team_taken) res += _play_1st.value(16,0); //res=-5
            else res += _play_1st.value(17,0); //res=7
        }
    }
//Case of the major card
    if (pcard->convertion(color_trump) == _color_master[color])
    {
        res += _play_1st.value(18,0); //res=12
        if (height==3) //10
        {
            res += _play_1st.value(19,0); //17
            if (opponents_cut(color)) res += _play_1st.value(20,0); //res=-25
        }
        if (height==7) //ace
        {
            res += _play_1st.value(21,0); //res=35
            if (opponents_cut(color)) res += _play_1st.value(22,0); //res=-30
        }
        if (height==6) //king
        {
            res += _play_1st.value(23,0); //res=5
            if (opponents_cut(color)) res += _play_1st.value(24,0); //res=-12
        }
        if (_nb_color_played[color] ==0)
        {
            if (pseudo_height == 2) res += _play_1st.value(25,0); // res=-15
            if (pseudo_height == 3) res += _play_1st.value(26,0); //res=-9
        }
        if ((number_trick == 6)&&(color == color_trump)) res += _play_1st.value(27,0); //res=-10
    }
//We try to not play in the "longe" of our opponents
    if (_calls[modulo(_number+1,4)*4+color]||_calls[modulo(_number+1,4)*4+color]) res += _play_1st.value(28,0); //res=-9
    return res;
}
int Joueur_IA::score_play(Carte* pcard,array<Carte*,4> trick,int color_trump,int player_take, int team_take,int position, int i_master,bool pisse,bool play_trump,int number_trick,int color_asked,int height_master,int pseudo_height_master)
{
    if (pcard == NULL) return -10000;
    switch (position)
    {
        case 1 : return score_play_2nd(pcard,trick,color_trump,player_take,team_take,i_master,pisse,play_trump,number_trick,color_asked,height_master,pseudo_height_master);
        case 2 : return score_play_3rd(pcard,trick,color_trump,player_take,team_take,i_master,pisse,play_trump,number_trick,color_asked,height_master,pseudo_height_master);
        case 3 : return score_play_4th(pcard,trick,color_trump,player_take,team_take,i_master,pisse,play_trump,number_trick,color_asked,height_master,pseudo_height_master);
		default : break;
    }
	return -100;
}
int Joueur_IA::score_play_2nd(Carte* pcard, array<Carte*,4> trick,int color_trump,int player_take, int team_take,int i_master,bool pisse,bool play_trump,int number_trick,int color_asked,int height_master,int pseudo_height_master)
{
    int res = 0;
    if (pcard==NULL) return -1000;
    int color = pcard->GetColour();
    int height= pcard->GetHeight();
    int pseudo_height = pcard->convertion(color_trump);
//If I have to play a trump
    if ((play_trump)&&(color==color_trump))
    {
        if (color==color_asked) //if the colour asked is the trump color
        {
            if (is_master(color_trump,color)) res += _play_2nd.value(0,0); //res=20
            else //you are not master: you play the smallest trump you can
            {
                if (pseudo_height == 2) res += _play_2nd.value(1,0); //res=-55
                if (pseudo_height == 3) res += _play_2nd.value(2,0); //res=-30
                if (pseudo_height == 4) res += _play_2nd.value(3,0); //res=-20
                if (height==smallest(color_trump,color_trump)) res += _play_2nd.value(4,0); //res=30
            }
        }
        else //we play a trump but the color asked is not the trump
        {
            if (height==smallest(color_trump,color)) res += _play_2nd.value(5,0); //res=10
            if (height==greatest(color_trump,color)) res += _play_2nd.value(6,0); //res=-30
            if (pseudo_height == 1) res += _play_2nd.value(32,0); //res=-20
            if (pseudo_height == 2) res += _play_2nd.value(7,0); //res=-20
            if (pseudo_height == 3) res += _play_2nd.value(8,0); //res=-10
        }
    }
//If I am the strongest in the color
    if ((pseudo_height == _color_master[color])&&(color==color_asked))
    {
        res += _play_2nd.value(9,0); //res=15
        if (height==3) //10
        {
            res += _play_2nd.value(10,0); //res=10
            if (opponents_cut(color)) res += _play_2nd.value(11,0); //res=-17
        }
        if (height==7) res += _play_2nd.value(12,0); //res=15
        if (height==6) res += _play_2nd.value(13,0); //res=5
        if (_nb_color_played[color] ==0)
        {
            if (height == 3) res += _play_2nd.value(14,0); //res=-20
            if (height == 6) res += _play_2nd.value(14,0); //res=-7
        }
        if (opponents_cut(color)) res += _play_2nd.value(16,0); //res=-20
        if ((number_trick == 6)&&(color == color_trump)) res += _play_2nd.value(17,0); //res=-10
    }
    //I have the colour and I am not the strongest
    if ((color==color_asked)&&(pseudo_height != _color_master[color]))
    {
        if (opponents_cut(color)) res += _play_2nd.value(18,0); //res=-10
        if (pseudo_height == 2) res += _play_2nd.value(19,0); //res=-30
        if (pseudo_height == 3) res += _play_2nd.value(20,0); //res=-10
        if (pseudo_height == 4) res += _play_2nd.value(21,0); //res=-5
    }
//the case of the 10 is taken into account in the case of "pisse"
//If we "pisse"(calls)
    if ((pisse)&&(color!=color_asked)&&(color!=color_trump))
    {
        if (is_master(color_trump,color)) res += _play_2nd.value(22,0); //res=-15
        if (height==greatest(color_trump,color)) res += _play_2nd.value(23,0); //res=-15
        if (height==smallest(color_trump,color)) res += _play_2nd.value(24,0); //res=15
        //we are risk-adverse (more than in the position 3) and the aversion is inversely proportional to the pseudo_height of the card
        if (pseudo_height == 1) res += _play_2nd.value(25,0); //res=-100
        if (pseudo_height == 2) res += _play_2nd.value(26,0); //res=-30
        if (pseudo_height == 3) res += _play_2nd.value(27,0); //res=-10
        if (pseudo_height == 4) res += _play_2nd.value(28,0); //res=-5
        if (pseudo_height == 5) res += _play_2nd.value(29,0); //res=-2
        if (score_longe(color_trump,color) > 5) res += _play_2nd.value(30,0); //res=7
        if (score_longe(color_trump,color) > 8) res += _play_2nd.value(31,0); //res=10
    }
//if the cards value something and the that it is not the color playd, we don't play the card
    if (color!=color_asked)
    {
        if (pseudo_height==1) res += _play_2nd.value(33,0); //res = -25
    }
    return res;
}
int Joueur_IA::score_play_3rd(Carte* pcard, array<Carte*,4> trick,int color_trump,int player_take, int team_take,int i_master,bool pisse,bool play_trump,int number_trick,int color_asked,int height_master,int pseudo_height_master)
{
    int res = 0;
    int color = pcard->GetColour();
    int height= pcard->GetHeight();
    int pseudo_height = pcard->convertion(color_trump);
//If I can play trump
    if (play_trump)
    {
        if ((color_asked!=color_trump)&&(i_master==0)&&(color==color_trump)) res += _play_3rd.value(0,0);//res=-10, we don't play  trump if the partner is mayor
        if ((color==color_trump)&&(color==color_asked))
        {
            if (is_master(color_trump,color)) res += _play_3rd.value(1,0); //res=20
            else //you are not master: you play the smallest trump you can
            {
                if (pseudo_height == 2) res += _play_3rd.value(2,0); //res=-55
                if (pseudo_height == 3) res += _play_3rd.value(3,0); //res=-20
                if (pseudo_height == 4) res += _play_3rd.value(4,0); //res=-10
                if (height==smallest(color_trump,color_trump)) res += _play_3rd.value(5,0); //res=30
            }
        }
        if ((color_asked!=color_trump)&&(i_master==1)&&(color==color_trump))
        {
            res += _play_3rd.value(6,0);//res=15
            if (height==smallest(color_trump,color)) res += _play_3rd.value(7,0); //res=10
            if (height==greatest(color_trump,color)) res += _play_3rd.value(8,0); //res=-30
            if (pseudo_height == 1) res += _play_3rd.value(9,0); //res=-20, we keep the jack for a trick where the color of the trump is played
            if (pseudo_height == 2) res += _play_3rd.value(10,0); //res=-20
            if (pseudo_height == 3) res += _play_3rd.value(11,0); //res=-10
        }
    }
//If my teammate is major
    if (i_master == 0)
    {
        if (color==color_trump)
        {
            if (protect_point(color_trump,color)&&(color_asked!=color_trump)&&(height = greatest(color_trump,color))) res += _play_3rd.value(12,0); //res=9
        }
        else
        {
            if (_nb_color_played[color_asked] == 0)
            {
                if (pseudo_height == 2) res += _play_3rd.value(13,0);//res=-8
                if (pseudo_height == 3) res += _play_3rd.value(14,0); //res=5
                else res += _play_3rd.value(15,0); //res=-3
            }
            else
            {
                if (pseudo_height <= 3) res += _play_3rd.value(16,0); //res=-5
                else res += _play_3rd.value(17,0); //res=3
            }
        }
        //add the case where the opponent don't cut (or has no more trumps) and my partner hase the master card in the colour
    }
//I have the 10 in the colour
    if ((color=color_asked)&&(got_ten(color_trump,color)))
    {
        if (pseudo_height == 2) //if it's the 10 (or the 9 of trump)
        {
            if ((pseudo_height ==_color_master[color])&&(!opponents_cut(color))) res += _play_3rd.value(18,0); //res=15
            else res += _play_3rd.value(19,0); //res=-25
            if (_nb_color_played[color]==1) res += _play_3rd.value(20,0); //res=3
            if (_nb_color_played[color]==0) res += _play_3rd.value(21,0);//res=-7
            if (_calls[modulo(_number+1,4)*4+color]) res += _play_3rd.value(22,0); //res=-8
            else res += _play_3rd.value(23,0); //res=6
        }
        else
        {
            if (!_calls[modulo(_number+1,4)*4+color]) res += _play_3rd.value(24,0); //res=-5
            else res += _play_3rd.value(25,0); //res=6
        }
    }
//If I am major in the color
    if (is_master(color_trump,color))
    {
        if (pcard->Nb_Point(color_trump) < 4) res += _play_3rd.value(26,0); //res=13
        else
        {
            if (opponents_cut(color))
            {
                switch (height)
                {
                    case 3 : res += _play_3rd.value(27,0); //res=-15
                        break;
                    case 7 : res += _play_3rd.value(28,0); //res=-25
                        break;
                    case 2 : res += _play_3rd.value(29,0); //res=-20
                        break;
                    default : break;
                }
            }
            else
            {
                if (_nb_color_played[color] == 0) res += _play_3rd.value(30,0); //res=7
                else res += _play_3rd.value(31,0); //res=-6
            }
        }
    }
//If we "pisse"
    if ((pisse)&&(color!=color_asked)&&(color!=color_trump))
    {
        if (is_master(color_trump,color))
        {
            res += _play_3rd.value(32,0); //res=10
            if (height==greatest(color_trump,color)) res += _play_3rd.value(33,0); //res=-25
            if (height==smallest(color_trump,color)) res += _play_3rd.value(34,0); //res=25
        }
        if (score_longe(color_trump,color) > 5) res += _play_3rd.value(35,0); //res=7
        if (score_longe(color_trump,color) > 8) res += _play_3rd.value(36,0); //res=10
    }
    return res;
}
int Joueur_IA::score_play_4th(Carte* pcard, array<Carte*,4> trick,int color_trump,int player_take, int team_take,int i_master,bool pisse,bool play_trump,int number_trick,int color_asked,int height_master,int pseudo_height_master)
{
    int res = 0;
    int color = pcard->GetColour();
    int height= pcard->GetHeight();
    //If we play trump
    if ((play_trump)&&(color == color_trump))
    {
        if (is_master(color_trump,color_trump)) //will we play a big or a small trump ?
        {
            if (protect_point(color_trump,color_trump))
            {
                if (greatest(color_trump,color_trump)) res += _play_4th.value(0,0);//res=-7
            }
            else
            {
                if (smallest(color_trump,color_trump)) res += _play_4th.value(1,0);//res=7
            }
        }
    }
// If my teammate is major
    if (i_master == 1)
    {
        if (color == color_trump) res += _play_4th.value(2,0); //res=-30
        else
        {
            if (height == 3) res += _play_4th.value(3,0); //res=8
            if (height == 7)
            {
                if (color == color_asked) res += _play_4th.value(4,0); //res=15
                else res += _play_4th.value(5,0);//res=-10
            }
            if ((_nb_resting_cards[color]+_nb_resting_card_color[color] == 8)&&(height == greatest(color_trump,color))) res += _play_4th.value(6,0); //res=35
        }
        if (protect_point(color_trump,color))
        {
            if (greatest(color_trump,color)) res += _play_4th.value(7,0); //res=9
        }
        else
        {
            if (smallest(color_trump,color)) res += _play_4th.value(8,0); //res=7
        }
    }
    else
    {
        switch (pcard->Nb_Point(color_trump))
        {
            case 0 : res += _play_4th.value(9,0); //res=13
                break;
            case 2 : res += _play_4th.value(10,0); //res=9
                break;
            case 3 : res += _play_4th.value(11,0); //res=7
                break;
            case 4 : res += _play_4th.value(12,0); //res=5
                break;
            case 10 : res += _play_4th.value(13,0); //res=-15
                break;
            case 11 : res += _play_4th.value(14,0); //res=-20
                break;
            case 14 : res += _play_4th.value(15,0); //res=-25
                break;
            case 20 : res += _play_4th.value(16,0); //res=100
                break;
            default : break;
        }
        if (greatest(color_trump,color)) res += _play_4th.value(17,0); //res=-7
    }
 //If we "pisse"
    if ((pisse)&&(color!=color_asked)&&(color!=color_trump))
    {
        if (is_master(color_trump,color))
        {
            res += _play_4th.value(18,0); //res=-10
            if (height==greatest(color_trump,color)) res += _play_4th.value(19,0); //res=-25
            if (height==smallest(color_trump,color)) res += _play_4th.value(20,0); //res=25
        }
        if (!is_master(color_trump,color))
        {
            if (i_master==1)
            {
               res += _play_4th.value(21,0); //res=5
                if (height==greatest(color_trump,color)) res += _play_4th.value(22,0); //res=15
                if (height==smallest(color_trump,color)) res += _play_4th.value(23,0); //res=-15
            }
            else
            {
                res += _play_4th.value(24,0); //res=-5
                if (height==greatest(color_trump,color)) res += _play_4th.value(25,0); //res=-15
                if (height==smallest(color_trump,color)) res += _play_4th.value(26,0); //res=15
            }
        }
        if (score_longe(color_trump,color) > 5) res += _play_4th.value(27,0); //res=7
        if (score_longe(color_trump,color) > 8) res += _play_4th.value(28,0); //res=10
    }
//If it is the penultimate trick, we try to keep our trump for the last trick
    if ((number_trick == 6)&&(color == color_trump)) res += _play_4th.value(29,0); //res=-8
//"Passe" of the ace, not always (risky)
    if ((color!=color_trump)&&(!ten_played(color_trump,color,trick,3))&&(height == 7)&&(_nb_color_played[color_asked]==0)&&(height_master!=3))
    {
        if (random(0,9)>=6) res += _play_4th.value(30,0); //res=-9
    }
    return res;
}
int Joueur_IA::greatest(int color_trump,int color)
{
    int height_max=-1;
    bool first = true;
    Carte* pcard = NULL;
    array<bool,64> arr;
    if (color == color_trump) arr= Carte::If_Trump();
    else arr = Carte::No_Trump();
    for (list<Carte*>::iterator it = _playable_cards.begin(); it != _playable_cards.end(); ++it)
    {
        pcard = *it;
        if (pcard->GetColour()==color)
        {
            if (first)
            {
                height_max = pcard->GetHeight();
                first = false;
            }
            else {if (arr[pcard->GetHeight()+height_max*8]) height_max = pcard->GetHeight();}
        }
    }
    return height_max;
}
int Joueur_IA::smallest(int color_trump,int color)
{
    int height_min=8;
    bool first = true;
    Carte* pcard = NULL;
    array<bool,64> arr;
    if (color == color_trump) arr = Carte::If_Trump();
    else arr = Carte::No_Trump();
    for (list<Carte*>::iterator it = _playable_cards.begin(); it !=_playable_cards.end(); ++it)
    {
        pcard = *it;
        if (pcard->GetColour()==color)
        {
            if (first)
            {
                height_min = pcard->GetHeight();
                first = false;
            }
            else {if (arr[pcard->GetHeight()*8+height_min]) height_min = pcard->GetHeight();}
        }
    }
    return height_min;
}
void Joueur_IA::update_nb_cards()
{
    for (list<Carte*>::iterator it = _hand.begin(); it != _hand.end(); ++it)
    {
        _nb_resting_card_color[(*it)->GetColour()] += 1;
    }
}
bool Joueur_IA::is_master(int color_trump,int color)
{
    for (list<Carte*>::iterator it = _hand.begin(); it != _hand.end(); ++it)
    {
        if ((*it)->convertion(color_trump) == _color_master[(*it)->GetColour()]) return true;
    }
    return false;
}
int Joueur_IA::score_longe(int color_trump, int color)
{
    Carte* pcard = NULL;
    int comp = 0;
    bool master = false;
    bool master_2 = false;
    bool master_3 = false;
    int res= 0;
    for (list<Carte*>::iterator it = _hand.begin(); it != _hand.end(); ++it)
    {
        pcard = *it;
        if (pcard->GetColour()==color)
        {
            comp++;
            if (pcard->convertion(color_trump)==_color_master[pcard->GetColour()]) master = true;
            if (pcard->convertion(color_trump)==_color_master[pcard->GetColour()]+1) master_2 = true;
            if (pcard->convertion(color_trump)==_color_master[pcard->GetColour()]+2) master_3 = true;
        }
    }
    switch (comp)
    {
        case 2 : res+=1; break;
        case 3 : res+=3; break;
        case 4 : res+=7; break;
        case 5 : res+=6; break;
        case 6 : res+=5; break;
        default : break;
    }
    if (res > 0)
    {
        if (master) res += 4;
        if (master_2)
        {
            res += 5;
            if (master_3) res += 3;
        }
        else
        {
            if (master_3&&(comp > 3)) res += 3;
        }
    }
    return res;
}
bool Joueur_IA::protect_point(int color_trump,int color)
{
    Carte* pcard = NULL;
    int color_carte;
    bool maitre = is_master(color_trump,color);
    for (list<Carte*>::iterator it = _hand.begin(); it != _hand.end(); ++it)
    {
        pcard = *it;
        color_carte = pcard->GetColour();
        if (color_carte==color)
        {
            if (!maitre&&(pcard->convertion(color_trump)==_color_master[color]+1))
            {
                if (opponents_cut(color)) return true;
            }
            else
            {
                if (!maitre&&(!opponents_cut(color_trump))&&(pcard->convertion(color_trump) <= 2)) return true;
            }
        }
    }
    return false;
}
bool Joueur_IA::got_ten(int color_trump,int color) //works too if it is the 9 of trump
{
    Carte* pcard = NULL;
    for (list<Carte*>::iterator it = _hand.begin(); it != _hand.end(); ++it)
    {
        pcard = *it;
        if ((pcard->GetColour()==color)&&(pcard->convertion(color_trump)==2)) return true;
    }
    return false;
}
bool Joueur_IA::ten_played(int color_trump,int color,array<Carte*,4> trick,int position)
{
    list<Carte*> li_cartes = _cards_fallen;
    Carte* pcard = NULL;
    for (int i = 0; i < position; i++)
    {
        li_cartes.push_front(trick[i]);
    }
    for (list<Carte*>::iterator it = li_cartes.begin(); it != li_cartes.end(); ++it)
    {
        pcard = *it;
        if ((pcard->GetColour()==color)&&(pcard->convertion(color_trump)==2)) return true;
    }
    return false;
}
bool Joueur_IA::opponents_cut(int color)
{
    return (_info_who_cut_what[4*modulo(_number+1,4)+color]||_info_who_cut_what[4*modulo(_number-1,4)+color]);
}
/*void Joueur_IA::update_stat(int score,int taker,int color_trump)
{
    int team = _number%2;
    int score_bis = 0;
    int taker_team = taker%2;
    if (score== 252) score_bis = 0;
    else
    {
        if (score== 0) score_bis = 252;
        else score_bis = 162-score;
    }
    int diff_score = score-score_bis;
    Update_Stats foncteur(diff_score,color_trump);
    if(taker==_number) _score_take.Update(color_trump,foncteur); //if I took
    else //I didn't took
    {
        if (team!=taker_team) _score_take.Update(color_trump,foncteur); //for_each(_to_update.begin(),_to_update.end(),foncteur);
    }
}*/
