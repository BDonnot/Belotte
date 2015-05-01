#ifndef CARDS_SDL_H
#define CARDS_SDL_H
/**
** This class implement representation of the cards
** on the screen through SDL.
** It is a pure virtual method.
**/

#include "Basic_Transparency.h"
#include "Basic_Click.h"
#include "Images_Sprite_Move.h"

//Define the variable for debugging purpose
#include "DebugwithPrint.h"
#define CARDS_SDL_DEBUG 1

class Cards_SDL: public  Basic_Transparency, public  Basic_Click, public Images_Sprite_Move<2>
{
    protected :
        Uint32 _half_duration;
        bool _up;
        SDL_Surface * _pScreen;
        bool _first_sprite;
        WrapperPrint<CARDS_SDL_DEBUG> _printf;
    public:
        Cards_SDL(){}
        Cards_SDL(Uint height,Uint color,SDL_Event* event,SDL_Surface * pscreen);
        virtual void Update_on_it();

        void InitMouvement(bool transparency,PositionGraphic pos,Uint duration,Uint time_lag);//to init a mouvement
        void UpdatePositionHand(PositionGraphic& pos_end);

        void Up(bool go_up); //up the card if go_up, otherwise down the card
        void Reveal(Uint duration,Uint time_lag,Uint current_time); //reveal or hide the card, depending on which part of the card is visible
        void Reset();

        const bool GetUp() const {return _up;}
        virtual void Display() = 0;
        virtual ~Cards_SDL(){}
    protected:
        virtual void act();
        void updateMouvement();
        void revealing();
    private:
};

#endif // CARDS_SDL_H
