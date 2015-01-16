#ifndef PLAYER_AI_H
#define PLAYER_AI_H

//#include "Definitions.h"
//#include "Basic_Game_Info.h"
#include "Player.h"
#include "Random.h"

#include "AIBetsMemory.h"
#include "AIGameMemory.h"
//#include "AIPlayRandom.h"
/**
Just do the interface between a player and all the type of possible AI
**/

//TO DO remove this class, and create a unique player class, templated with TakeAI and PlayAI for the AI, and
//create the same two classes for human.
//TO DO : add the possibility for the players to have a memory...
template<class TakeAI,class PlayAI>
class Player_AI : public Player
{
    protected :
        Random _rand;
        TakeAI _take;
        PlayAI _play;
        AIBetsMemory _betsMemory;
        AIGameMemory _gameMemory;
    public:
        Player_AI(){}
        Player_AI(POSITION_PLAYER number,Uint16 windows_width, Uint16 windows_height,SDL_Event* pevent,Basic_Images* fond,SDL_Surface* screen):
            Player(number,windows_width,windows_height,pevent,fond,screen)
            ,_rand(rand())
            ,_take()
            ,_play(){}
        virtual ~Player_AI(){}
        //Cards* Choose_Card(const std::array<Cards*,4>& trick,int i_master); //choose what card the player play
        //int Take(bool first_round,int color_proposed,int height_proposed); //choose if the player take or not. 127 : no, 255 : not choosen, 0-3 : color at which the player wants to take
    protected:
        virtual void UpdateBid(const BetsMemory& bets);

        virtual CARDS_COLOR do_i_take(bool first_round,CARDS_COLOR color_proposed,CARDS_HEIGHT height_proposed);
        virtual std::list<Cards*>::iterator what_card_do_i_play(const std::array<Cards*,4>& trick);

        virtual void resetTake(){ _take.Reset(); } //merge with ResetBid() when the template of Player_AI will be upped to Player, thus removing Player_Human and Player_AI

    private:
        Player_AI(const Player_AI& other){}
        Player_AI& operator=(const Player_AI& other)
        {
            if (this == &other) return *this; // handle self assignment
            //assignment operator
            return *this;
        }
};


template<class TakeAI,class PlayAI>
CARDS_COLOR Player_AI<TakeAI,PlayAI>::do_i_take(bool first_round,CARDS_COLOR color_proposed,CARDS_HEIGHT height_proposed) //by default, take if 3 (or more) cards in the color proposed
{ //classical belote (to do : remove -- just change the PlayAI class one for classical belot, the other one for coinche)
    int number_cards = 0;
    if (first_round)
    {
        number_cards = how_many_colour(color_proposed);
        return ((number_cards>3) ? color_proposed: NO);
    }
    for(int i = 0; i < 4; i++)
    {
        CARDS_COLOR res = do_i_take(true,_basic_info.ConvertIntToColor(i),height_proposed);
        if((res!=NO)&&(res!=color_proposed))
            return res;
    }
    return NO;
}

template<class TakeAI,class PlayAI>
std::list<Cards*>::iterator Player_AI<TakeAI,PlayAI>::what_card_do_i_play(const std::array<Cards*,4>& trick) //by default, play a random card
{
    return _play.Play(trick,_playable_cards,_hand,_rand,_currentTrickStatus);
}


template<class TakeAI,class PlayAI>
void Player_AI<TakeAI,PlayAI>::UpdateBid(const BetsMemory& bets)
{
    //if (_number == 1) _currentBid.Bid(_basic_info.ConvertIntToColor(_rand.generate_number()%4),_basic_info.MaxBid()+10);
    //else _currentBid.Bid(NO,_basic_info.MaxBid()+10);
    _take.Bid(_currentBid,_hand,_rand,bets);
}


#endif // PLAYER_AI_H
