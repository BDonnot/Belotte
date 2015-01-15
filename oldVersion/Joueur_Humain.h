#ifndef JOUEUR_HUMAIN_H
#define JOUEUR_HUMAIN_H

#include "Joueur.h"
class Joueur_Humain : public Joueur
{
    public:
        Joueur_Humain();
        Joueur_Humain(std::string);
        ~Joueur_Humain();
    protected :
        bool I_take_1st_round(int,Carte*,bool&); // Do you take to the colour of the card face up ?
        int I_take_2nd_round(int,Carte*); //Do you take to a colour different of the one of the card face up ? If yes, which one ?
        void I_choose_1st_round(std::list<Images*>&,std::list<Images*>,Images*);
        void I_choose_2nd_round(std::list<Images*>&,std::list<Images*>,Images*,int);
        void I_choose_card(std::list<Images*>&, Images*);
        void clear(); //take back all the cards of his hand
        void Update(std::array<Carte*,4>,Carte*,int,int);
        Carte* choose_card(std::array<Carte*,4>,int,int,int,int);
        void update_nb_cards();
        void sort(int);

        bool _action_done;
        std::list<Carte*> _playable_cards;
        Carte* _card_played_h;
        std::string convertion(int colour);
        bool _first_displayed;
    private:
};
#endif // JOUEUR_HUMAIN_H
