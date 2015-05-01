#ifndef MCPLAYRANDOM_H
#define MCPLAYRANDOM_H

/**
** This class emulate some games, for the
** Monte Carlo way of playing.
** It has a specific memory (for now AIMemPerfectInfo).
** It also emulates the tricks, and the "Mini Players"
** plays according to PlayAI.
** Per each call at this class,
** nbSimulGames are simulated.
**/


#include<array>
#include<list>

#include "Definitions.h"
#include "Cards_Basic.h"
#include "Random.h"
#include "TrickBasic_Memory.h"
#include "PlayerMiniMonteCarlo.h"
#include "WrapperMemory.h"

//For debugging
#include "DebugwithPrint.h"
#define MCPLAYRANDOM_DEBUG 0

///Improve rapidity :
///TO DO : maybe specialize some version of trick, to be faster
///TO DO : maybe improve some version of PlayAI...

template<Uint nbSimulGames,template<class> class Memory,class PlayAI>
class MCPlayGames
{
    protected :
        Uint _nbGamePlayed; //we play 30 games for each
        Uint _cardsToPlay;
        Player_ID _number;
        Random * _rand;
        std::array<Cards*,32> _allCards ;
        PlayAI _play;
        WrapperMemory<Memory> _updateMemory;
        WrapperPrint<MCPLAYRANDOM_DEBUG> _printf;
    public:
        MCPlayGames():_nbGamePlayed(nbSimulGames),_play(_number)
        {
            for(Uint iCol = 0; iCol < 4; ++iCol)
            {
                for(Uint iHei = 0; iHei < 8; ++iHei)
                {
                    _allCards[iCol*8+iHei] = nullptr;
                }
            }
        }
        virtual ~MCPlayGames()
        {
            _printf("destructor of MCPlayGames called\n");
            if(_allCards[0] != nullptr)
            {
                for(Uint iCol = 0; iCol < 4; ++iCol)
                {
                    for(Uint iHei = 0; iHei < 8; ++iHei)
                    {
                        delete _allCards[iCol*8+iHei];
                    }
                }
            }
        }
        void Init(Uint cardsToPlay,
                  const Player_ID& number,
                  Random * rand)
        {
            _printf("I init the game\n");
            _cardsToPlay = cardsToPlay;
            _number = number;
            _play.SetNumber(_number);
            _rand = rand;
            if(_allCards[0] == nullptr)
            {
                for(Uint iCol = 0; iCol < 4; ++iCol)
                {
                    for(Uint iHei = 0; iHei < 8; ++iHei)
                    {
                        _allCards[iCol*8+iHei] = new Cards(Card_Height(iHei),Card_Color(iCol));
                    }
                }
            }
            _printf("I have finished initiating the game\n");
        }
        Uint Play(const std::array<PlayerMiniMonteCarlo , 4>& players,
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

template<Uint nbSimulGames,template<class> class Memory,class PlayAI>
Uint MCPlayGames<nbSimulGames,Memory,PlayAI>::Play(const std::array<PlayerMiniMonteCarlo, 4>& players,
													const Cards* pcard,
													const TrickBasic_Memory& trick,
													std::list<Cards*> hand,
													const TrickStatus& trickStatus)
{
    Uint res = 0;
    for(Uint nbSimul = 0; nbSimul < _nbGamePlayed; ++nbSimul)
    {
        TrickBasic_Memory currentTrick = trick;
        Memory<Cards_Basic> gameMemory;
        std::array<PlayerMiniMonteCarlo , 4> currentPlayers(players);
		_updateMemory.InitMemory(gameMemory,currentPlayers);

        Cards_Basic tempCard = static_cast<const Cards_Basic&>(*pcard);
        PlayerMiniMonteCarlo* tempPlayer;

        currentPlayers[_number.ToInt()].RetrieveCard(tempCard);
        putCardInTrick(currentTrick,tempCard);
        _printf("There is %d cards to be played\n",_cardsToPlay);
        _printf("I start playing\n");
        while(true)
        {
            currentTrick.NextToPlay();
            _printf("I play trick %d\n",currentTrick.TrickNumber());
            if(currentTrick.TrickNumber() ==8 ) break;
            tempPlayer = & currentPlayers[currentTrick.NextPlaying().ToInt()];
            _updateMemory.NextPlayer(gameMemory,*tempPlayer);
            std::list<Cards_Basic > & playableCards = tempPlayer->GetPlayableCard(currentTrick);

			tempCard = _play.Play(currentTrick, playableCards, tempPlayer->GetHand(), *_rand, trickStatus, gameMemory);

			putCardInTrick(currentTrick, tempCard);
			tempPlayer->RetrieveCard(tempCard);
            _printf("%d has played [c:%d,h:%d], he has still %d cards \n",
                   tempPlayer->ID().ToInt(),
                   tempCard.GetColour().ToInt(),
                   tempCard.GetHeight().ToInt(),
                   tempPlayer->CardInHand());
        }
        IntIntPair bothScores = currentTrick.CurrentScores();
        if(_number.ToInt() == 0 || _number.ToInt() == 2) res += bothScores.first;
        else res += bothScores.second;
        currentTrick.Reset();
    }
    return res;
}

template<Uint nbSimulGames,template<class> class Memory,class PlayAI>
void MCPlayGames<nbSimulGames,Memory,PlayAI>::putCardInTrick(TrickBasic_Memory& currentTrick, const Cards_Basic& card)
{
    currentTrick.PutCard( _allCards[card.GetColour().ToInt()*8 + card.GetHeight().ToInt()]);
}

#endif // MCPLAYRANDOM_H
