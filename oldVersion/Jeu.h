#ifndef JEU_H_INCLUDED
#define JEU_H_INCLUDED

#include <string>
#include <list>
#include <array>

#include "Carte.h"
#include "Joueur.h"
#include "Images.h"
#include "Button.h"
#include "Display_Trump.h"

class Jeu
{
    public :
        Jeu();
        Jeu(int,int,SDL_Event*,SDL_Surface*);
        ~Jeu();
        int GetTrump();
        std::list<Carte*> GetCards();
        std::array<Carte*,4> Gettrick();
        void PlayGame(int nb_human_players);
        void Confront_AI(int nb_round,int& AI_1,int& AI_2);
    private :
        std::array<Joueur*,4> _Players; //all game's players
        std::list<Carte*> _Cards; //all game's cards
        int _trump; //trump's color
        bool _firstGame; //if it is the first game
        int _ind_player; //indice of the player who do the action (play a card, (play a card, bet etc.)
        int _taker; //the team which take
        std::array<Carte*,4> _trick; //trick being played
        bool _belote_announced; //if the belote was announced
        std::array<int,2> _team_scores; //score of each team
        int _player_give; //the player who give the cards
        SDL_Event* _pEvent;
        SDL_Surface* _pScreen;
        int _SCREEN_HEIGHT;
        int _SCREEN_WIDTH;
        bool _first_displayed;
        bool _first_displayed1;
        unsigned int _step1;
        unsigned int _step2;
        unsigned int _step3;
        int _last_ten;
        bool _human;
        Carte* _last;
        std::array<int,4> _array_taking; //represent the choice of the player during the taking phase.
        Display_trump _display_trump;
        std::array<Carte*,4> _previous_trick;

        //to initialize the game (cards and players)
        void new_players(int); //the parameter is the number of human players (1 ou 0)
        void Cards_generation();
        void init(int);
        //to erase the players
        void delete_players_cards();
        //to play a round
        int Play(std::list<Images*>&,std::list<Images*>&,std::list<Images*>&);
        //If nobody takes, players give back their cards :
        int Put_cards_back(std::list<Images*>&,Images*);
        //Phase to shuffle the cards
        void Shuffle();
        //Phase to cut the pack of cards
        void Cut();
        //Phase to know if we cut or shuffle the pack
        void Beginning();
        //Phase when we deal the cards
        int Give_1(std::list<Images*>&); //We deal the 20 first cards
        int Give_2(std::list<Images*>,int); //We deal the 11 last cards and the one that has been taken
        int Taking(std::list<Images*>&,std::list<Images*>,std::list<Images*>); //We select who is taking and to which colour
        void set_taker(int i,bool);
        int player_take(std::list<Images*>&li_displayed,std::list<Images*> li_text,std::list<Images*>li_aux,int i,bool premier_tour);
        int end_giving(std::list<Images*>&,std::list<Images*>);
        void display_text_choice(std::list<Images*>& li_displayed,std::list<Images*>& li_text,int n);
        void Give_n_cards(int,int,int,bool);
        int Give(std::list<Images*>&,std::list<Images*>&,std::list<Images*>&); //We deal the cards
        //Phase of game (tricks)
        int Play_trick(std::list<Images*>&,std::list<Images*>,std::list<Images*>,int);
        void display_hands(std::list<Images*>&);
        void display_pli(std::list<Images*>&);
        int confirmation_trick(std::list<Images*>&, Images*);
        void WhoWinTrick(int);
        int animation_gather_trick(std::list<Images*>&);
        int animation_win_trick(std::list<Images*>&,int);
        void go_trick(int,int);
        //Phase to count the score
        std::array<int,3> CountPoints();
        int screen_end(std::list<Images*>&,std::list<Images*>);
        int clear(std::list<Images*>,std::list<Images*>,std::list<Images*>);//to empty the tricks and everything that needs to be emptied between each round

        void apply_surfaces(std::list<Images*>,SDL_Surface*); //to display what is occuring on the window
        int set_names(std::list<Images*>&,Button&,Button&,int&,Text&,Text&);
        void update(std::list<Images*>&);
        int give_cards();
        int update_nb_cards();
        int ask_number_human_player();
        void reset_images(std::list<Images*>);
        void declare_belote();
        void display_trump(std::list<Images*>&,std::list<Images*>);
        void display_old_trick(std::list<Images*>&,Images* ptext);
        void set_previous_trick();

        void QuiRemportePli(int numero_pli);
        void Joue_pli(int numero_pli);
        void Donne_2(int i_preneur);
        void Donne_n_cartes(int premiere,int nb_cartes_donnees,int ind_joueur);
        void Donne_1();
        void Donne();
        void Joue();
        void clear();
        int Prise();
        void Remet_carte();
        void affiche_score(std::list<Images*>&,std::list<Images*>);

};
#endif // JEU_H_INCLUDED
