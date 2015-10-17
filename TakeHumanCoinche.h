#ifndef TAKEHUMANCOINCHE_H
#define TAKEHUMANCOINCHE_H

/**
** This class allows the human player to
** interact with game, and to choose the color
** and the value of its bet.
**/

#include<string>
#include<ostream>
#include <sstream>

#include "Definitions.h"
#include "Basic_Game_Info.h"
#include "Player_Bid.h"
#include "Images_Button.h"
#include "Images_Text.h"
#include "Player_Bid.h"
#include "Random.h"

class TakeHumanCoinche
{
    protected :
        Uint16 _windows_width;
        Uint16 _windows_height;
        SDL_Surface* _screen;
        const Basic_Game_Info _info;
		SDL::Images_Button _carreau;
		SDL::Images_Button _coeur;
		SDL::Images_Button _pique;
		SDL::Images_Button _trefle;
		SDL::Images_Button _tt_atout;
		SDL::Images_Button _ss_atout;
		SDL::Images_Button _pass;
		SDL::Images_Button _ok;
		SDL::Images_Button _plus;
		SDL::Images_Button _minus;
		SDL::Images_Text _score;

        Player_Bid _unconfirmedBid;
        Player_Bid _confirmedBid;

        bool _displayed;
        Uint32 _clock;
        Uint32 _clock_min_before_new_change;
    public:
        TakeHumanCoinche(){};
        TakeHumanCoinche(SDL_Event* event,SDL_Surface* screen,Uint16 windows_width, Uint16 windows_height);
        virtual ~TakeHumanCoinche(){}

        void Display(GAME_PHASES currentPhase);
        void UpdateMouse(GAME_PHASES currentPhase);
        void UpdateClock(Uint32 time);

        const Player_Bid& Has_Taken();
        void Reset(bool reset_unconfirmedBid);
    protected:
        void Update();
        void ChangeText(Uint number);
    private:
};

#endif // TAKEHUMANCOINCHE_H
