#ifndef FONCTEURS_CARTES_H_INCLUDED
#define FONCTEURS_CARTES_H_INCLUDED

#include "Carte.h"

class Sort_CardsOld
{
    public :
        Sort_CardsOld(int trump_color):_trump_color(trump_color){}
        bool operator()(Carte* card1,Carte* card2) // says if the card1 must be put at the left of the card2
        {
            int height1 = card1->GetHeight();
            int height2 = card2->GetHeight();
            int color1 = card1->GetColour();
            int color2 = card2->GetColour();
            bool win_couleur[16] = {false,true,true,true,false,false,false,true,false,true,false,true,false,false,false,false}; //sort the colours : carreau, pique,coeur,trefle
            if (color1 != color2) return win_couleur[4*color1+color2];
            if (color1==_trump_color) return Carte::If_Trump()[8*height1+height2];
            return Carte::No_Trump()[8*height1+height2];
        }
    private :
        int _trump_color;

};
class Count_Cards
{
    public :
        Count_Cards(int trump_color,int &init):_trump_color(trump_color),_points_in_hand(init){}
        void operator()(Carte* card) //add the value of card to the _points_in_hand
        {
            _points_in_hand += card->Nb_Point(_trump_color);
        }
    private :
        int _trump_color;
        int& _points_in_hand;

};
class Center_and_Restart
{
    public :
        Center_and_Restart(int w,int h):_SCREEN_WIDTH(w),_SCREEN_HEIGHT(h){}
        void operator()(Carte* pcard) //update the data properly
        {
            pcard->restart_click();
            pcard->SetPosition(_SCREEN_WIDTH/2.,_SCREEN_HEIGHT/2.,0);
        }
    private :
        int _SCREEN_WIDTH;
        int _SCREEN_HEIGHT;
};
class Reset_Cards
{
    public :
        Reset_Cards(){}
        void operator()(Carte* pcard) //update the data properly
        {
            pcard->reset();
        }
};
#endif // FONCTEURS_CARTES_H_INCLUDED
