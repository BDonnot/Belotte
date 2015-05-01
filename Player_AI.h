#ifndef PLAYER_AI_H
#define PLAYER_AI_H

/**
** This class derived from Player, and does
** the interface between Player and all kinds of AI players.
** This creates a new thread when the computer
** choose the right card. The interface does not freeze if
** the computer take a while to choose a card.
**/

#include <thread>
#include <future>
#include <chrono>

#include "Definitions.h"
#include "Basic_Game_Info.h"
#include "Player.h"
#include "Random.h"

#include "AIBetsMemory.h"

//Define the variable for debugging purpose
#include "DebugwithPrint.h"
#define PLAYER_AI_DEBUG 0

///TO DO remove this class, and create a unique player class, templated with TakeAI and PlayAI for the AI, and
///create the same two classes for human.

template<template<class> class GameMemory,class TakeAI, class PlayAI>
class Player_AI : public Player
{
    protected :
        Random _rand;
        AIBetsMemory _betsMemory;
        GameMemory<Cards*> _gameMemory;
        TakeAI _take;
        PlayAI _play;

        std::future< Cards* > _threadChooseCard;
        bool _threadLaunched;

        WrapperPrint<PLAYER_AI_DEBUG> _printf;
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
    protected:
        virtual void updateMemoryTrick(const TrickBasic_Memory& trick,const Position_Trick& myPos);
        virtual void initMemoryTrick();
        virtual void updateBid(const BetsMemory& bets);

        virtual Card_Color do_i_take(bool first_round,const Card_Color& color_proposed,const Card_Height& height_proposed);
        virtual Cards* what_card_do_i_play(const TrickBasic_Memory& trick);

        virtual void resetTake(){ _take.Reset(); } //merge with ResetBid() when the template of Player_AI will be upped to Player, thus removing Player_Human and Player_AI
        Cards* callPlay(PlayAI* play,
                        const TrickBasic_Memory* trick,
                        std::list<Cards* >* playableCard,
                        std::list<Cards*>* hand,
                        Random* rand,
                        TrickStatus* currentTrickStatus,
						GameMemory<Cards*>* memory);
    private:
        Player_AI(const Player_AI& other){}
        Player_AI& operator=(const Player_AI& other){return *this;}
};


template<template<class> class GameMemory, class TakeAI, class PlayAI>
Card_Color Player_AI<GameMemory, TakeAI, PlayAI>::do_i_take(bool first_round, const Card_Color& color_proposed, const Card_Height& height_proposed) //by default, take if 3 (or more) cards in the color proposed
{
    //DEPRECATED
    //classical belote (to do : remove -- just change the PlayAI class one for classical belot, the other one for coinche)
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

template<template<class> class GameMemory, class TakeAI, class PlayAI>
Cards* Player_AI<GameMemory, TakeAI, PlayAI>::what_card_do_i_play(const TrickBasic_Memory& trick) //by default, play a random card
{
    _printf("I choose my card\n");
#if MULTITHREAD_GRAPHIC > 0
    Cards* res = nullptr;
    _printf("I enter the function for player %d\n",_number.ToInt());
    if(_threadLaunched) //the thread has been launch
    {
        _printf("Thread launch but not finished\n");
        auto status = _threadChooseCard.wait_for(std::chrono::milliseconds(0));
        if(status == std::future_status::ready) //the card has been choosen
        {
            _printf("Thread over\n");
            res = _threadChooseCard.get();
            _threadLaunched = false;
        }
    }
    else //launching the thread
    {
        _printf("Init thread\n");
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
#else
	return _play.Play(trick,_playable_cards,_hand,_rand,_currentTrickStatus,_gameMemory);
#endif
}

template<template<class> class GameMemory, class TakeAI, class PlayAI>
Cards* Player_AI<GameMemory,TakeAI,PlayAI>::callPlay(PlayAI* play,
														const TrickBasic_Memory* trick,
														std::list<Cards* >* playableCard,
														std::list<Cards* >* hand,
														Random* rand,
														TrickStatus* currentTrickStatus,
														GameMemory<Cards*>* memory)
{
    return play->Play(*trick,*playableCard,*hand,*rand,*currentTrickStatus,*memory);
}


template<template<class> class GameMemory, class TakeAI, class PlayAI>
void Player_AI<GameMemory,TakeAI,PlayAI>::updateBid(const BetsMemory& bets)
{
    _take.Bid(_currentBid,_hand,_rand,bets);
}

template<template<class> class GameMemory, class TakeAI, class PlayAI>
void Player_AI<GameMemory,TakeAI,PlayAI>::updateMemoryTrick(const TrickBasic_Memory& trick,const Position_Trick& myPos)
{
    _gameMemory.UpdateFullTrick(trick,myPos);
}

template<template<class> class GameMemory, class TakeAI, class PlayAI>
void  Player_AI<GameMemory,TakeAI,PlayAI>::initMemoryTrick()
{
    _gameMemory.InitEverything();
}

#endif // PLAYER_AI_H
