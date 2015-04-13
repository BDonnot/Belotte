#ifndef AIPLAYMONTECARLO_H_INCLUDED
#define AIPLAYMONTECARLO_H_INCLUDED

#include <array>
#include <PlayerMiniMonteCarlo.h>

#include "Definitions.h"
#include "TrickBasic_Memory.h"
#include "Cards.h"
#include "Random.h"
#include "TrickStatus.h"
#include "ResultMonteCarlo.h"

#define nbMaxSimul 1
/**
idee : on simule les cartes possibles des autres joueurs par Monte Carlo
(optimisation possible : on essaie de sur ponderer les situations mauvaises pour les ennemis
puis les situations bonnes)
on resout le jeu dans chaque simulation : on est en information complete donc pourquoi pas...
on renvoie la carte qui nous parait la mieux : a definir ce qu'on entend par mieux...

ca serait bien de faire un thread la dessus... pour que l'ordi (interface graphique) ne freeze pas pendant que l'ordi
reflechi...
**/

//TO DO : AI Play score can be a basic case of this method...
//TO DO : multi-threading :-)
template<class Memory,class PlayAI>
class AIPlayMonteCarlo
{
    protected :
        std::array<PlayerMiniMonteCarlo<Memory,PlayAI> , 4> _players;
        TrickBasic_Memory _trick;
        std::list<Cards_Basic> _unfallenCards;
        //const _nbMaxSimul;
    public :
        AIPlayMonteCarlo(){}
        virtual ~AIPlayScores(){}
        std::list<Cards*>::iterator Play(const TrickBasic_Memory& trick
                                     ,std::list<std::list<Cards*>::iterator>& plyableCards
                                     ,const std::list<Cards*>& hand
                                     ,Random& rand
                                     ,const TrickStatus& trickStatus
                                     ,const Memory& playerMemory);
    protected :
        void initStates(const Memory& playerMemory,Random& rand);
        void updateUnfallenCards(const TrickBasic_Memory& trick,const Memory& playerMemory);
        Uint scoreCards(const Cards* pcard);
        void simulGame(const TrickBasic_Memory& trick,const Memory& playerMemory);
        void playGame();
        std::list<Cards*>::iterator whichBetter(const ResultMonteCarlo& resSimulations);
};

template<class Memory,class PlayAI>
std::list<Cards*>::iterator AIPlayMonteCarlo<Memory,PlayAI>::Play(const TrickBasic_Memory& trick
                                       ,std::list<std::list<Cards*>::iterator>& playbleCards
                                       ,const std::list<Cards*>& hand
                                       ,Random& rand
                                       ,const TrickStatus& trickStatus
                                       ,const Memory& playerMemory)
{
    initStates(playerMemory,rand); //tell everyone what append in the previous tricks
    std::list<Cards*>::iterator res = *playbleCards.begin();
    ResultMonteCarlo resMonteCarlo(nbMaxSimul); //grille de score : chaque carte sur chaque trajectoire ???

    Uint tempScore;

    //simulate each game accordingly
    for(Uint nbSimul = 0; nbSimul < nbMaxSimul; ++nbSimul)
    {
        simulGame(trick,playerMemory); //Monte Caro Simulating each hand of each players
        //solve the problem knowing the hand of each player
        for(auto itPCard : playbleCards)
        {
            tempScore = scoreCards(*itPCard); //
            resMonteCarlo.Put(nbSimul,*itPCard,tempScore);
        }
    }
    res = whichBetter(const ResultMonteCarlo& resSimulations);
    return res;
}

template<class Memory,class PlayAI>
void AIPlayMonteCarlo<Memory,PlayAI>::initStates(const Memory& playerMemory,Random& rand)
{
    updateUnfallenCards();
    for(Uint i = 0; i < 4, ++i)
    {
        _players[i].ReceiveInitInfo(playerMemory,rand);
    }
}

template<class Memory,class PlayAI>
void AIPlayMonteCarlo<Memory,PlayAI>::updateUnfallenCards(const TrickBasic_Memory& trick,const Memory& playerMemory)
{
    CARDS_COLOR colors[4] = {DIAMOND,HEART,SPADE,CLUB};
    CARDS_HEIGHT heights[8] = {SEVEN,EIGHT,NINE,TEN,JACK,QUEEN,KING,ACE};
    Card_Color col;
    Card_Height height;
    _unfallenCards.clear();
    for(Uint iCol = 0; iCol < 4; ++iCol)
    {
        for(Uint iHeight = 0; iHeight < 8; ++iHeight)
        {
            col = Card_Color(colors[iCol]);
            height = Card_Height(heights[iHeight]);
            if(!playerMemory.CardsFallen(col,height) && !trick.CardsFallen(col,height))
            {
                _unfallenCards.push_back(Cards_Basic(col,height));
            }
        }
    }
}

template<class Memory,class PlayAI>
void AIPlayMonteCarlo<Memory,PlayAI>::simulGame(const TrickBasic_Memory& trick,const Memory& playerMemory)
{
    //TO DO, mais c'est complique ...
}
#endif // AIPLAYMONTECARLO_H_INCLUDED
