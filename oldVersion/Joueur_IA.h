#ifndef JOUEUR_IA_H
#define JOUEUR_IA_H

#include "Joueur.h"
#include "Datas.h"
#include "Score_take.h"
#include <vector>
class Joueur_IA : public Joueur
{
    public:
        Joueur_IA();
        Joueur_IA(std::string,int);
        virtual ~Joueur_IA();
    protected:
        bool I_take_1st_round(int,Carte*,bool&);// Do you take to the specified colour ?
        int I_take_2nd_round(int,Carte*);//Do you take to a colour different from teh one specified ? If yes, which one ?
        void clear();
        Carte* choose_card(std::array<Carte*,4>,int,int,int,int);
        void Update(std::array<Carte*,4>,Carte*,int,int);
        void update_nb_cards();
        //void update_stat(int score_0,int taker,int color_trump);
    private:
        std::list<Carte*> _playable_cards; //store the playable cards
        std::array<bool,16> _info_who_cut_what; //store who cuts which colour
        int _resting_trump ; //store the number of remaining trumps
        std::array<int,4> _color_master; //stocke la "pseudo_hauteur" des cartes maitresses dans chaque couleur
        std::array<int,4> _nb_resting_cards; //Store the number of remaining cards in each colour
        std::array<int,16> _calls; //Store the colour of the different calls
        std::array<int,4> _nb_color_played; //number of time each colour has been played
        std::array<int,4> _nb_resting_card_color; // number of cards I have in one colour
        int _taking_round;
        int _type;
//machine learning
        /*Datas _eval_qlt_card;
        Datas _score_1;
        Datas _bonus_pos_1;
        Datas _bonus_pos_2;
        Datas _bonus_nb_trumps;
        Datas _eval_qlt_trump;
        Datas _eval_qlt_trump_2;
        Datas _score_pcard_given;
        std::list<Datas*> _to_update;*/
        Datas _scores_min;
        Datas _play_1st;
        Datas _play_2nd;
        Datas _play_3rd;
        Datas _play_4th;
        void reset_datas();

        int score_1(Carte*,int);
        int bonus_pos_1(int,int);
        int bonus_pos_2(int,int);
        int bonus_nb_trumps(int,int);
        int bonus_qlt_game(const std::array<Carte*,5>& ,int,const std::array<Carte*,5>&,int ,const std::array<Carte*,5>&,int,int);
        int eval_qlt_color(const std::array<Carte*,5>& ,int,int);
        int eval_qlt_card(int,int,int,int);
        int bonus_qlt_trumps(const std::array<Carte*,5>& ,int,int,int);
        int eval_qlt_trump(int,int,int,int,int);
        int score_color(int,Carte*,const std::array<Carte*,5>& ,int,const std::array<Carte*,5>&,int ,const std::array<Carte*,5>&,int ,const std::array<Carte*,5>&,int,int);
        int bonus_qlt_trumps_2(const std::array<Carte*,5>& ,int,int);
        int eval_qlt_trump_2(int,int,int,int);
        int score_pcard_given(Carte*,int);

        int score_play(Carte*, std::array<Carte*,4>,int,int,int,int,int,bool,bool,int,int,int,int);
        int bonus_if_belote(std::array<Carte*,5>,int);
        Carte* enter(int,int,int,int);
        bool must_play_trump(int,int);
        int score_play_1st(Carte*,int,int,int,bool,int,int);
        int score_play_2nd(Carte*, std::array<Carte*,4>,int,int,int,int,bool,bool,int,int,int,int);
        int score_play_3rd(Carte*, std::array<Carte*,4>,int,int,int,int,bool,bool,int,int,int,int);
        int score_play_4th(Carte*, std::array<Carte*,4>,int,int,int,int,bool,bool,int,int,int,int);
        int greatest(int,int); //return the height of the strongest card in a colour
        int smallest(int,int); //return the height of the smallest card in a colour
        bool is_master(int,int); //return true if the player is the master in the colour enter as the parameter
        int score_longe(int,int); //return the quality of the potential longe (corresponding to the colour enter as the parameter)
        bool protect_point(int,int); //return true if we have to protect points in the colour (if we have just a 10 in the colour for example)
        bool got_ten(int,int); //return true if we can play the 10 of the colour
        bool ten_played(int,int,std::array<Carte*,4>,int); //return true if the 10 of the colour has already been played
        bool opponents_cut(int);
        void load();
};
#endif // JOUEUR_IA_H
