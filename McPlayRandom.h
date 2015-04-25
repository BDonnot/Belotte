#ifndef MCPLAYRANDOM_H
#define MCPLAYRANDOM_H

#include<array>
#include<list>

#include "Definitions.h"
#include "Cards_Basic.h"
#include "Random.h"
#include "TrickBasic_Memory.h"

#include "PlayerMiniMonteCarlo.h"

/**
**Force the MonteCarlo Players to play randomly
**/
template<class Memory,Uint nbCardToPlay>
class McPlayRandom
{
    protected :
        Uint _nbGamePlayed; //we play 30 games for each
        Uint _cardsToPlay;
        Player_ID _number;
        Random * _rand;
        std::array<Cards,32> _allCards ;
    public:
        McPlayRandom():_nbGamePlayed(nbCardToPlay){}
        virtual ~McPlayRandom(){}
        void Init(Uint cardsToPlay,
                  const Player_ID& number,
                  Random * rand)
        {
            _cardsToPlay = cardsToPlay;
            _number = number;
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
                    const TrickBasic_Memory& trick);
    protected:
        void putCardInTrick(TrickBasic_Memory& currentTrick, const Cards_Basic& card);
        void chooseCard(std::list<Cards_Basic> & playableCard,Cards_Basic& res);
    private:
};

template<class Memory,Uint nbCardToPlay>
Uint McPlayRandom<Memory,nbCardToPlay>::Play(const std::array<PlayerMiniMonteCarlo<Memory> , 4>& players,
                                            const Cards* pcard,
                                            const TrickBasic_Memory& trick)
{
    Uint res = 0;
    for(Uint nbSimul = 0; nbSimul < _nbGamePlayed; ++nbSimul)
    {
        TrickBasic_Memory currentTrick = trick;
        std::array<PlayerMiniMonteCarlo<Memory> , 4> currentPlayers(players);
        Cards_Basic tempCard = static_cast<const Cards_Basic&>(*pcard);
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
            std::list<Cards_Basic> &playableCards = tempPlayer->GetPlayableCard(currentTrick);

            chooseCard(playableCards,tempCard);

            tempPlayer->RetrieveCard(tempCard);
            putCardInTrick(currentTrick,tempCard);
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

template<class Memory,Uint nbCardToPlay>
void McPlayRandom<Memory,nbCardToPlay>::putCardInTrick(TrickBasic_Memory& currentTrick, const Cards_Basic& card)
{
    currentTrick.PutCard( &(_allCards[card.GetColour().ToInt()*8 + card.GetHeight().ToInt()]) );
}

template<class Memory,Uint nbCardToPlay>
void McPlayRandom<Memory,nbCardToPlay>::chooseCard(std::list<Cards_Basic> & playableCards,Cards_Basic& res)
{
    //printf("%d\n",playableCards.size());
    if(playableCards.size() == 1)
    {
        res = playableCards.front();
    }
    else
    {
        std::list<Cards_Basic>::iterator itTempCard = playableCards.begin();
        std::advance(itTempCard,_rand->generate_number() %(playableCards.size()-1) );
        res = *itTempCard;
    }
}

#endif // MCPLAYRANDOM_H
