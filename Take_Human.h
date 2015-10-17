#ifndef TAKE_HUMAN_H
#define TAKE_HUMAN_H

/**
** Old class, used when I coded the Belote games.
** It is now deprecated.
**/

#include "Basic_Game_Info.h"
#include "Images_Button.h"
#include "Images_Text.h"

class Take_Human
{
    protected:
        Uint16 _windows_width;
        Uint16 _windows_height;
        const Basic_Game_Info _info;
		SDL::Images_Button _carreau;
		SDL::Images_Button _coeur;
		SDL::Images_Button _pique;
		SDL::Images_Button _trefle;
		SDL::Images_Button _pass;
		SDL::Images_Button _yes;
		SDL::Images_Button _no;
		SDL::Images_Text _question;
        std::string _question_which_color;
        SDL_Surface* _screen;

        bool _first_round;
        bool _displayed;

    public:
        Take_Human(){}
        Take_Human(SDL_Event* event,SDL_Surface* screen,Uint16 windows_width, Uint16 widows_height);
        virtual ~Take_Human(){}
        void Display();
        void UpdateMouse();
        Card_Color Has_Taken(); //255 if the human did'nt took, otherwie the number of the color choosen
        void Change(bool first_round);
        void Reset();
        void Update();

    private:
        std::string DefineQuestion();
};

#endif // TAKE_HUMAN_H
