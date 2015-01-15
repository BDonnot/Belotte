#ifndef CARTE_H
#define CARTE_H

#include "Images.h"
#include <list>

class Carte
{
    public:
        Carte();
        Carte(int,int,SDL_Event*,int,int);
        ~Carte();
        int GetColour();
        int GetHeight();
        static std::array<bool,64> If_Trump();
        static std::array<bool,64> No_Trump();
        SDL_Surface* GetRecto_surface();
        bool GetDown();
        std::string display();
        bool operator== (Carte&);
        bool operator!= (Carte&);
        int Nb_Point(int);
        //bool win(Carte*,int);
        int convertion(int); //convert height into en pseudo_height
    //SDL
        void SetPosition(int,int,int);
        bool Arrived();
        void Set_animate_donne(int,int,bool,int,int);
        void Set_animate(double,double,int,int,int);
        void Set_animate_transparency(double,double,int,int,bool,int);
        void add_display(std::list<Images*>& li_display);
        void Set_has_been_played(bool);
        void reveal(int,int);
        int GetAlpha();
        void give_back(int,bool);
        void reset();
        void restart_click();
        bool click_on(bool);
        bool Mouse_over();
        void Move_down();
        void Move_up();
        void up_transparent();
        void down_transparent();
        void update();
     private:
        int _colour;
        int _height;
        SDL_Event* _pEvent;
        Images _recto;
        Images _verso; //when the card is face down
        int _SCREEN_HEIGHT;
        int _SCREEN_WIDTH;
        bool _has_been_played;
        bool _display_recto;
        bool _down;
        bool _transparent_down;

        std::string Colour();
        std::string Height();
        bool is_equal(Carte&);
        bool IsTrump(int);
        std::string path_image();
};

#endif // CARTE_H
