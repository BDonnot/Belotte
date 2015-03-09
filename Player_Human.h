#ifndef PLAYER_HUMAN_H
#define PLAYER_HUMAN_H

#include "Player.h"
#include "Take_Human.h"
#include "TakeHumanCoinche.h"
#include "Random.h"

class Player_Human : public Player
{
    public:
        Player_Human(){}
        Player_Human(POSITION_PLAYER number,Uint16 windows_width, Uint16 windows_height,SDL_Event* pevent,Basic_Images* fond,SDL_Surface* screen):
            Player(number,windows_width,windows_height,pevent,fond,screen)
            ,_TakeCoinche(pevent,screen,windows_width,windows_height)
            ,_rand(rand()){}
        virtual ~Player_Human(){}
        virtual void Update_Mouse(GAME_PHASES currentPhase);
        virtual void Display(GAME_PHASES currentPhase);
        //virtual Cards* Choose_Card(const std::array<Cards*,4>& trick,int i_master); //choose what card the player play
        //virtual int Take(bool first_round,int color_proposed,int height_proposed); //choose if the player take or not. 127 : no, 255 : not choosen, 0-3 : color at which the player wants to take
    protected:
        virtual void updateBid(const BetsMemory& bets);
        virtual void ResetBid(bool betFinished);

        virtual int do_i_take(bool first_round,int color_proposed,int height_proposed);
        virtual std::list<Cards*>::iterator what_card_do_i_play(const std::array<Cards*,4>& trick);

    private:
        Take_Human _Take;
        TakeHumanCoinche _TakeCoinche;
        UpdateCardMouse _UpdateCardMouse; //to update if the mouse is over a card or not

        Player_Human(const Player_Human& other){}
        Player_Human& operator=(const Player_Human& rhs)
        {
            if (this == &rhs) return *this; // handle self assignment
            //assignment operator
            return *this;
        }
        Random _rand;
};

#endif // PLAYER_HUMAN_H
