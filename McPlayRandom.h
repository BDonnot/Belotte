#ifndef MCPLAYRANDOM_H
#define MCPLAYRANDOM_H

#include<array>
#include<list>

#include "Definitions.h"
#include "Cards_Basic.h"
#include "Random.h"
#include "TrickBasic_Memory.h"

#include "PlayerMiniMonteCarlo.h"

#include "AIMemPerfectInfo.h"

//TO DO clean bellow, improve the coupling AIGameMemory
template<class Memory>
class WrapperMemory
{
public :
    void UpdateMemory(Memory& playerMemory,const std::array<PlayerMiniMonteCarlo<Memory> , 4>& players){}
    void NextPlayer(Memory& playerMemory,const Player_ID& player){}
};


template<>
class WrapperMemory<AIMemPerfectInfo>
{
    public :
        void UpdateMemory(AIMemPerfectInfo& playerMemory,const std::array<PlayerMiniMonteCarlo<AIMemPerfectInfo> , 4>& players)
        {
            std::array< std::pair<Player_ID,std::list<Cards_Basic> >, 4> allHands;
            for(Uint i = 0; i < 4; ++i)
            {
                allHands[i] = std::pair<Player_ID,std::list<Cards_Basic> >(players[i].ID(),players[i].GetHand());
            }
            playerMemory.SetGame(allHands);
        }
        void NextPlayer(AIMemPerfectInfo& playerMemory,const Player_ID& player)
        {
            playerMemory.SetPlayer(player);
        }
};
/**
**Force the MonteCarlo Players to play randomly
**/
template<class Memory,Uint nbCardToPlay,class PlayAI>
class McPlayRandom
{
    protected :
        Uint _nbGamePlayed; //we play 30 games for each
        Uint _cardsToPlay;
        Player_ID _number;
        Random * _rand;
        std::array<Cards,32> _allCards ;
        PlayAI _play;
        WrapperMemory<Memory> _updateMemory;
    public:
        McPlayRandom():_nbGamePlayed(nbCardToPlay),_play(_number){}
        virtual ~McPlayRandom(){}
        void Init(Uint cardsToPlay,
                  const Player_ID& number,
                  Random * rand)
        {
            _cardsToPlay = cardsToPlay;
            _number = number;
            _play.SetNumber(_number); //TO DO : code cette foontion
            _rand = rand;
            for(Uint iCol = 0; iCol < 4; ++iCol)
            {
                for(Uint iHei = 0; iHei < 8; ++iHei)
                {
                    _allCards[iCol*8+iHei] = std::move(Cards(Card_Height(iHei),Card_Color(iCol)));
                }
            }
        }
        Uint Play(const std::array<PlayerMiniMonteCarlo<Memory> , 4>& players,
                    const Cards* pcard,
                    const TrickBasic_Memory& trick,
                    std::list<Cards*> hand,
                    const TrickStatus& trickStatus);
    protected:
        void putCardInTrick(TrickBasic_Memory& currentTrick, const Cards_Basic& card);
        void chooseCard(std::list<std::list<Cards_Basic>::iterator > & playableCard,
                        std::list<Cards_Basic>::iterator& res);
    private:
};

template<class Memory,Uint nbCardToPlay,class PlayAI>
Uint McPlayRandom<Memory,nbCardToPlay,PlayAI>::Play(const std::array<PlayerMiniMonteCarlo<Memory> , 4>& players,
                                            const Cards* pcard,
                                            const TrickBasic_Memory& trick,
                                            std::list<Cards*> hand,
                                            const TrickStatus& trickStatus)
{
    Uint res = 0;
    for(Uint nbSimul = 0; nbSimul < _nbGamePlayed; ++nbSimul)
    {
        TrickBasic_Memory currentTrick = trick;
        Memory gameMemory(_number,&hand); //TO DO : update the memory in a proper way :-)
        //TrickStatus trickStatus;
        std::array<PlayerMiniMonteCarlo<Memory> , 4> currentPlayers(players);
        _updateMemory(currentPlayers);

        Cards_Basic tempCard = static_cast<const Cards_Basic&>(*pcard);
        //Cards_Basic tempItCard;
        PlayerMiniMonteCarlo<Memory> * tempPlayer;

        currentPlayers[_number.ToInt()].RetrieveCard(tempCard);
        putCardInTrick(currentTrick,tempCard);
        //printf("There is %d cards to be played\n",_cardsToPlay);
        //Uint nbZeros = 0; //number of player with 0 card
        //printf("I start playing\n");
        while(true)
        {
            currentTrick.NextToPlay();
            //printf("I play trick %d\n",currentTrick.TrickNumber());
            if(currentTrick.TrickNumber() ==8 ) break;
            tempPlayer = & currentPlayers[currentTrick.NextPlaying().ToInt()];
            _updateMemory.NextPlayer(gameMemory,tempPlayer.ID());
            std::list<Cards_Basic > & playableCards = tempPlayer->GetPlayableCard(currentTrick);

            //chooseCard(playableCards,tempItCard);
			tempCard = _play.Play(currentTrick, playableCards, hand, *_rand, trickStatus, gameMemory);

			putCardInTrick(currentTrick, tempCard);
			tempPlayer->RetrieveCard(tempCard);
            /*
            printf("%d has played [c:%d,h:%d], he has still %d cards \n",
                   tempPlayer->ID().ToInt(),
                   tempCard.GetColour().ToInt(),
                   tempCard.GetHeight().ToInt(),
                   tempPlayer->CardInHand()); */
        }
        IntIntPair bothScores = currentTrick.CurrentScores();
        if(_number.ToInt() == 0 || _number.ToInt() == 2) res += bothScores.first;
        else res += bothScores.second;
        currentTrick.Reset();
    }
    return res;
}

template<class Memory,Uint nbCardToPlay,class PlayAI>
void McPlayRandom<Memory,nbCardToPlay,PlayAI>::putCardInTrick(TrickBasic_Memory& currentTrick, const Cards_Basic& card)
{
    currentTrick.PutCard( &(_allCards[card.GetColour().ToInt()*8 + card.GetHeight().ToInt()]) );
}

/*
template<class Memory,Uint nbCardToPlay>
void McPlayRandom<Memory,nbCardToPlay>::chooseCard(std::list<std::list<Cards_Basic>::iterator > & playableCards,
                                                   std::list<Cards_Basic>::iterator& res)
{
    //printf("%d\n",playableCards.size());
    if(playableCards.size() == 1)
    {
        res = playableCards.front();
    }
    else
    {
        std::list<std::list<Cards_Basic>::iterator >::iterator itTempCard = playableCards.begin();
        std::advance(itTempCard,_rand->generate_number() %(playableCards.size()-1) );
        res = *itTempCard;
    }
}
*/
#endif // MCPLAYRANDOM_H
