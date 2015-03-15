#ifndef SCORE_TAKE_H
#define SCORE_TAKE_H

#include "Carte.h"
#include "Datas.h"
#include "Foncteurs_Datas.h"
class Score_take
{
    protected:
        std::list<Carte*> _hand;
        Carte* _proposed_card;
        int _type;
        int _position;
        Datas _eval_qlt_card;
        Datas _score_1;
        Datas _bonus_pos_1;
        Datas _bonus_pos_2;
        Datas _bonus_nb_trumps;
        Datas _eval_qlt_trump;
        Datas _eval_qlt_trump_2;
        Datas _score_pcard_given;
        Datas _bonus_longe;
        std::list<Datas*> _to_update;

    public:
        Score_take();
        Score_take(int type);
        virtual ~Score_take();
        void Set_hand(std::list<Carte*> hand,Carte* proposed_card,int position);
        int Eval_score(int color, bool first_round);
        void Update(int color,Update_Stats& updata_stat);
        void Human_choice_taking(int color,bool first_round);

    protected :
        int bonus_longe(int nb,int color);
        int eval_qlt_color(std::list<Carte*>& li_color,bool memorize);
        int eval_qlt_color_as_trump(std::list<Carte*> li_trumps,bool first_round,bool memorize);
        int eval_qlt_trump(Carte* pcard,int pos,int color);
        int eval_qlt_trump_2(Carte* pcard,int pos,int color);
        int eval_qlt_card(Carte* pcard,int pos,int nb_cards, int color);
        int score_1(Carte*,int);
        int bonus_pos_1(int,int);
        int bonus_qlt_game(int color_tested,bool first_round,bool memorize);
        int bonus_pos_2(int,int);
        int score_pcard_given(Carte*,int);
        int bonus_nb_trumps(int,int);
        int eval_score_beginning(int color,bool first_round, bool memorize);
        bool has_colour(int);
        bool has_card(int,int);
        void sort();

        double score_1(Carte*,Obtain_value&);
        double bonus_pos_1(int,Obtain_value&);
        double bonus_pos_2(int,Obtain_value&);
        double bonus_nb_trumps(int nb,Obtain_value& value);
    private:
};
#endif // SCORE_TAKE_H
