#ifndef WRAPPERMEMORY_H_INCLUDED
#define WRAPPERMEMORY_H_INCLUDED

#include "AIMemPerfectInfo.h"
template<template<class> class Memory>
class WrapperMemory
{
public :
    void InitMemory(Memory<Cards_Basic>& playerMemory,const std::array<PlayerMiniMonteCarlo , 4>& players){}
	void NextPlayer(Memory<Cards_Basic>& playerMemory, const PlayerMiniMonteCarlo & player){}
};

template<>
class WrapperMemory<AIMemPerfectInfo >
{
    public :
		void InitMemory(AIMemPerfectInfo<Cards_Basic>& playerMemory, std::array<PlayerMiniMonteCarlo, 4>& players)
        {
            std::array< std::pair<Player_ID,std::list<Cards_Basic> >, 4> allHands;
            for(Uint i = 0; i < 4; ++i)
            {
                allHands[i] = std::pair<Player_ID,std::list<Cards_Basic> >(players[i].ID(),players[i].GetHand());
            }
            playerMemory.SetGame(allHands);
        }
		void NextPlayer(AIMemPerfectInfo<Cards_Basic>& playerMemory, PlayerMiniMonteCarlo & player)
        {
            playerMemory.SetPlayer(player.ID(),player.GetHand());
        }
};


#endif // WRAPPERMEMORY_H_INCLUDED
