#ifndef JOUEUR_H_INCLUDED
#define JOUEUR_H_INCLUDED

#include "Carte.h"
#include "Text.h"
#include "Score_take.h"

class Joueur
{
public:
    Joueur();
    Joueur(std::string);
    virtual ~Joueur();
    std::string Getname();
    std::list<Carte*> GetMain();
    Text* GetpNameSurface();
    Images* GetpImage();
    void setName(std::string);
    void displayMain();
    void PutNumber(int,SDL_Event*,int,int);
    void GetCard(Carte*,int); //is given a card
    bool operator== (Joueur&);
    int PlayCard(std::list<Images*>&,Images*,Images*,std::array<Carte*,4>&,int,int,int,int,bool&);
    int bidding_first_round(std::list<Images*>&,std::list<Images*>,Images*,int,Carte*,bool&); // Do you accept the card face up as the trump ?
    int bidding_second_round(std::list<Images*>&,std::list<Images*>,Images*,int,Carte*,bool&); //Do you take with another colour as trump, if yes, which one ?
    virtual void clear(); //take all the cards out of his hand
    virtual void Update(std::array<Carte*,4>,Carte*,int,int) =0;
    virtual void update_nb_cards() =0;
    void win_trick(std::array<Carte*,4>);
    bool GetABelote();
    std::list<Carte*> GetTrickWon();
    int how_many_colour(int);
    int Strongest_card(int,std::array<Carte*,4>,int);
    void add_display(std::list<Images*>& li_display);
    bool Update_belote(int trump_colour);
    void update_stat(int score_0,int taker,int color_trump);
    int CountPoints(int trump_color,std::list<Carte*>&,int&);
    void Give_back(std::list<Carte*>& cards);
    void Update_score_take(Carte* pcard_proposed,int player_give);

    Carte* JoueCarte(std::array<Carte*,4>& trick, int trump_colour,int player_take,int number_trick,int pos);
protected:
    std::string _name;//his name
    std::list<Carte*> _hand;
    bool _a_belote;
    bool _belote_played;
    std::list<Carte*> _tricks_won;
    int _number;
    Images _image;
    Text _name_surface;
    SDL_Event* _pEvent;
    int _SCREEN_HEIGHT;
    int _SCREEN_WIDTH;
    int _res;
    bool _erased;
    Carte* _card_played;
    std::list<Carte*> _cards_fallen; //store all the fallen cards
    bool _has_taken;
    Score_take _score_take;

    bool is_equal(Joueur&);
    bool has_colour(int);
    bool has_card(int,int);
    std::list<Carte*> playable_cards(std::array<Carte*,4>, int);
    bool can_discarding(std::array<Carte*,4>, int);
    bool can_play_trump(Carte*,std::array<Carte*,4>);
    int getPosition(std::array<Carte*,4>);
    bool trump_played(std::array<Carte*,4>,int);
    bool has_higher(int,int);
    virtual Carte* choose_card(std::array<Carte*,4>,int,int,int,int)=0;
    virtual bool I_take_1st_round(int,Carte*,bool&)=0; // Do you take to the specified colour ?
    virtual int I_take_2nd_round(int,Carte*)=0;
    virtual void I_choose_1st_round(std::list<Images*>&,std::list<Images*>,Images*);
    virtual void I_choose_2nd_round(std::list<Images*>&,std::list<Images*>,Images*,int);
    virtual void I_choose_card(std::list<Images*>&, Images*);
    void erase(Carte*);
    void position_text_bid(Images*);
    void define_position_card(int&, int& ,int& ,int ,int);
    bool can_play_card(Carte*,std::array<Carte*,4>,int);
    int choose_lag(int number_cards,int player_give);
};
#endif // JOUEUR_H_INCLUDED
