#ifndef PLAYER_AI_H
#define PLAYER_AI_H

#include <thread>
#include <future>
#include <chrono>

#include "Definitions.h"
#include "Basic_Game_Info.h"
#include "Player.h"
#include "Random.h"

#include "AIBetsMemory.h"
//#include "AIGameMemory.h"
//#include "AIPlayRandom.h"
/**
Just do the interface between a player and all the type of possible AI
**/

//TO DO remove this class, and create a unique player class, templated with TakeAI and PlayAI for the AI, and
//create the same two classes for human.
//TO DO : add the possibility for the players to have a memory...
template<class GameMemory,class TakeAI, class PlayAI>
class Player_AI : public Player
{
    protected :
        Random _rand;
        AIBetsMemory _betsMemory;
        GameMemory _gameMemory;
        TakeAI _take;
        PlayAI _play;

        std::future< std::list<Cards*>::iterator > _threadChooseCard;
        bool _threadLaunched;
        //std::future_status::timeout _threadChooseCardStatus;
    public:
        Player_AI(){}
        Player_AI(const Player_ID& number,Uint16 windows_width, Uint16 windows_height,SDL_Event* pevent,Basic_Images* fond,SDL_Surface* screen):
            Player(number,windows_width,windows_height,pevent,fond,screen)
            ,_rand(rand())
            ,_betsMemory()
            ,_gameMemory(number,&_hand)
            ,_take()
            ,_play(_number)
            ,_threadLaunched(false){}
        virtual ~Player_AI(){}
        //Cards* Choose_Card(const std::array<Cards*,4>& trick,int i_master); //choose what card the player play
        //int Take(bool first_round,int color_proposed,int height_proposed); //choose if the player take or not. 127 : no, 255 : not choosen, 0-3 : color at which the player wants to take
    protected:
        virtual void updateMemoryTrick(const TrickBasic_Memory& trick,const Position_Trick& myPos);
        virtual void initMemoryTrick();
        virtual void updateBid(const BetsMemory& bets);

        virtual Card_Color do_i_take(bool first_round,const Card_Color& color_proposed,const Card_Height& height_proposed);
        virtual std::list<Cards*>::iterator what_card_do_i_play(const TrickBasic_Memory& trick);

        virtual void resetTake(){ _take.Reset(); } //merge with ResetBid() when the template of Player_AI will be upped to Player, thus removing Player_Human and Player_AI
        std::list<Cards*>::iterator callPlay(PlayAI* play,
                                              const TrickBasic_Memory* trick,
                                              std::list<std::list<Cards*>::iterator >* playableCard,
                                              std::list<Cards*>* hand,
                                              Random* rand,
                                              TrickStatus* currentTrickStatus,
                                              GameMemory* memory);
    private:
        Player_AI(const Player_AI& other){}
        Player_AI& operator=(const Player_AI& other)
        {
            if (this == &other) return *this; // handle self assignment
            //assignment operator
            return *this;
        }
};


template<class GameMemory,class TakeAI, class PlayAI>
Card_Color Player_AI<GameMemory,TakeAI,PlayAI>::do_i_take(bool first_round,const Card_Color& color_proposed,const Card_Height& height_proposed) //by default, take if 3 (or more) cards in the color proposed
{ //classical belote (to do : remove -- just change the PlayAI class one for classical belot, the other one for coinche)
    int number_cards = 0;
    if (first_round)
    {
        number_cards = how_many_colour(color_proposed);
        return ((number_cards>3) ? color_proposed: Card_Color(NO));
    }
    for(int i = 0; i < 4; i++)
    {
        Card_Color res = do_i_take(true,Card_Color(i),height_proposed);
        if((res!=NO)&&(res!=color_proposed))
            return res;
    }
    return NO;
}

template<class GameMemory,class TakeAI, class PlayAI>
std::list<Cards*>::iterator Player_AI<GameMemory,TakeAI,PlayAI>::what_card_do_i_play(const TrickBasic_Memory& trick) //by default, play a random card
{
    std::list<Cards*>::iterator res = _hand.end();
    //printf("I enter the function for player %d\n",_number.ToInt());
    //res = callPlay(&_play,&trick,&_playable_cards,&_hand,&_rand,&_currentTrickStatus,&_gameMemory);
    if(_threadLaunched) //the thread has been launch
    {
        //printf("Thread launch but not finished\n");

        auto status = _threadChooseCard.wait_for(std::chrono::milliseconds(0));

        if(status == std::future_status::ready) //the card has been choosen
        {
            //printf("Thread over\n");
            res = _threadChooseCard.get();
            _threadLaunched = false;
        }
    }
    else //launching the thread
    {
        //printf("Init thread\n");
        _threadLaunched = true;
        _threadChooseCard = std::async(std::launch::async,
                                       &Player_AI<GameMemory,TakeAI,PlayAI>::callPlay,
                                       this,
                                       &_play,
                                       &trick,
                                       &_playable_cards,
                                       &_hand,
                                       &_rand,
                                       &_currentTrickStatus,
                                       &_gameMemory);

    }
    return res;
}

template<class GameMemory,class TakeAI, class PlayAI>
std::list<Cards*>::iterator Player_AI<GameMemory,TakeAI,PlayAI>::callPlay(PlayAI* play,
                                                                          const TrickBasic_Memory* trick,
                                                                          std::list<std::list<Cards*>::iterator >* playableCard,
                                                                          std::list<Cards*>* hand,
                                                                          Random* rand,
                                                                          TrickStatus* currentTrickStatus,
                                                                          GameMemory* memory)
{
    return play->Play(*trick,*playableCard,*hand,*rand,*currentTrickStatus,*memory);
}
//no matching function for call to ‘async(std::launch, <unresolved overloaded function type>,


template<class GameMemory,class TakeAI, class PlayAI>
void Player_AI<GameMemory,TakeAI,PlayAI>::updateBid(const BetsMemory& bets)
{
    //if (_number == 1) _currentBid.Bid(_basic_info.ConvertIntToColor(_rand.generate_number()%4),_basic_info.MaxBid()+10);
    //else _currentBid.Bid(NO,_basic_info.MaxBid()+10);
    _take.Bid(_currentBid,_hand,_rand,bets);
}

template<class GameMemory,class TakeAI, class PlayAI>
void Player_AI<GameMemory,TakeAI,PlayAI>::updateMemoryTrick(const TrickBasic_Memory& trick,const Position_Trick& myPos)
{
    _gameMemory.UpdateFullTrick(trick,myPos);
}

template<class GameMemory,class TakeAI, class PlayAI>
void  Player_AI<GameMemory,TakeAI,PlayAI>::initMemoryTrick()
{
    _gameMemory.InitEverything();
}

#endif // PLAYER_AI_H
