#ifndef HUMANEVENTHANDLER_H
#define HUMANEVENTHANDLER_H

#include "Foncteurs_Cards.h"
/**
Handle the keyboard and mouse input for human player
currently unused
**/

class HumanEventHandler
{
    public:
        HumanEventHandler(){}
        //HumanEventHandler(SDL_Event* pevent, SDL_Surface* screen,Uint windows_width,Uint windows_height):
        //    _TakeCoinche(pevent,screen,windows_width,windows_height){}
        virtual ~HumanEventHandler(){}
        void Update_Mouse(GAME_PHASES currentPhase);
    protected:
        //TakeHumanCoinche _TakeCoinche;
        //UpdateCardMouse _UpdateCardMouse; //to update if the mouse is over a card or not
    private:
};

#endif // HUMANEVENTHANDLER_H
