#ifndef AIPLAYMONTECARLO_H_INCLUDED
#define AIPLAYMONTECARLO_H_INCLUDED

#include <array>
#include <stack>
#include <list>

#include "Definitions.h"
#include "TrickBasic_Memory.h"
#include "Cards.h"
#include "Random.h"
#include "PlayerMiniMonteCarlo.h"
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
        Random * _rand;
        //const _nbMaxSimul;
    public :
        AIPlayMonteCarlo(){}
        virtual ~AIPlayMonteCarlo(){}
        std::list<Cards*>::iterator Play(const TrickBasic_Memory& trick
                                     ,std::list<std::list<Cards*>::iterator>& plyableCards
                                     ,const std::list<Cards*>& hand
                                     ,Random& rand
                                     ,const TrickStatus& trickStatus
                                     ,const Memory& playerMemory);
    protected :
        void initStates(const TrickBasic_Memory& trick,const Memory& playerMemory,Random& rand,const std::list<Cards*>& hand);
        void updateUnfallenCardsupdateUnfallenCards(const TrickBasic_Memory& trick,const Memory& playerMemory,const std::list<Cards*>& hand);
        Uint scoreCards(const Cards* pcard);
        void simulGame(const TrickBasic_Memory& trick,const Memory& playerMemory,const std::list<Cards*>& hand,const TrickStatus& trickStatus);
        void playGame();
        bool cardsInHand(const std::list<Cards*>& hand,const Card_Color& col,const Card_Height& height);
        std::list<Cards*>::iterator whichBetter(const ResultMonteCarlo& resSimulations);
        Uint computeNbPlayerAbleToReceive(const Memory& playerMemory,const Cards_Basic& card,const TrickStatus& trickStatus);

        void updateUnfallenCards(const TrickBasic_Memory& trick,const Memory& playerMemory,const std::list<Cards*>& hand);
        void removeContraint(const BasicMonteCarloAction& constraint);
        void removeConsequences(const std::list<BasicMonteCarloAction>& consequences);
        void updatePlayerAdd(std::list< PlayerMiniMonteCarlo<Memory,PlayAI> * >& players);
        bool computeConsequences(std::list<Cards_Basic>& consequences,std::list<PlayerMiniMonteCarlo<Memory,PlayAI> * >& players);

        void addConstraint(const Player_ID & id,const BasicMonteCarloAction& constraint);
        void removeCard(const std::list<Cards_Basic>& consequences); //update _unfallencards
        void updatePlayer(PlayerMiniMonteCarlo<Memory,PlayAI> * pPlayer,std::list<PlayerMiniMonteCarlo<Memory,PlayAI>  * >& players); //update _unfallencards
        void removeConstraint(const BasicMonteCarloAction& constraint);
        bool playerReceiver(std::list<PlayerMiniMonteCarlo<Memory,PlayAI> * >& players,const Cards_Basic& card);
};

template<class Memory,class PlayAI>
std::list<Cards*>::iterator AIPlayMonteCarlo<Memory,PlayAI>::Play(const TrickBasic_Memory& trick
                                       ,std::list<std::list<Cards*>::iterator>& playbleCards
                                       ,const std::list<Cards*>& hand
                                       ,Random& rand
                                       ,const TrickStatus& trickStatus
                                       ,const Memory& playerMemory)
{
    initStates(trick,playerMemory,rand,hand); //tell everyone what append in the previous tricks
    std::list<Cards*>::iterator res = *playbleCards.begin();
    ResultMonteCarlo resMonteCarlo(nbMaxSimul); //grille de score : chaque carte sur chaque trajectoire ???

    Uint tempScore;

    //simulate each game accordingly
    for(Uint nbSimul = 0; nbSimul < nbMaxSimul; ++nbSimul)
    {
        simulGame(trick,playerMemory,hand,trickStatus); //Monte Caro Simulating each hand of each players
        //solve the problem knowing the hand of each player
        for(auto itPCard : playbleCards)
        {
            tempScore = scoreCards(*itPCard); //
            resMonteCarlo.Put(nbSimul,*itPCard,tempScore);
        }
    }
    res = whichBetter(resMonteCarlo);
    return res;
}

template<class Memory,class PlayAI>
std::list<Cards*>::iterator AIPlayMonteCarlo<Memory,PlayAI>::whichBetter(const ResultMonteCarlo& resSimulations)
{

}

template<class Memory,class PlayAI>
void AIPlayMonteCarlo<Memory,PlayAI>::initStates(const TrickBasic_Memory& trick,const Memory& playerMemory,Random& rand,const std::list<Cards*>& hand)
{
    _rand = &rand;
    updateUnfallenCards(trick,playerMemory,hand);
    for(Uint i = 0; i < 4; ++i)
    {
        _players[i].ReceiveInitInfo(playerMemory,rand,_unfallenCards);
    }
}

template<class Memory,class PlayAI>
void AIPlayMonteCarlo<Memory,PlayAI>::updateUnfallenCards(const TrickBasic_Memory& trick,const Memory& playerMemory,const std::list<Cards*>& hand)
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
            if(!playerMemory.CardsFallen(col,height) && !trick.IsFallen(col,height) && !cardsInHand(hand,col,height))
            {
                _unfallenCards.push_back(Cards_Basic(col,height));
            }
        }
    }
}

template<class Memory,class PlayAI>
bool AIPlayMonteCarlo<Memory,PlayAI>::cardsInHand(const std::list<Cards*>& hand,const Card_Color& col,const Card_Height& height)
{
    for(auto pcard : hand)
    {
        if(pcard->GetColour() == col && pcard->GetHeight() == height) return true;
    }
    return false;
}

template<class Memory,class PlayAI>
void AIPlayMonteCarlo<Memory,PlayAI>::simulGame(const TrickBasic_Memory& trick,const Memory& playerMemory,const std::list<Cards*>& hand,const TrickStatus& trickStatus)
{
    ///compute how many cards each player must receive
    ///compute whom can received which card
    ///give each card randomly to each player, starting by the card only a few player can receive

    _players[trickStatus.ID().ToInt()].ReceiveHand(hand);
    std::list<PlayerMiniMonteCarlo<Memory,PlayAI> * > playerStillCardsToReceive;
    for(Uint iPlayer = 0; iPlayer < 4; ++iPlayer)
    {
        if(_players[iPlayer] != trickStatus.ID()) playerStillCardsToReceive.push_back(&_players[iPlayer]);
    }
    //TO DO : un truc louche avec la declaration en dessous
    MonteCarloAction action;
    computeConsequences(action.Consequences(),playerStillCardsToReceive);
    auto itCard = _unfallenCards.begin();
    //std::list<PlayerMiniMonteCarlo<Memory,PlayAI> *>::iterator itPlayer = nullptr;
    Cards_Basic tempCard;
    PlayerMiniMonteCarlo<Memory,PlayAI> * tempPlayer;
    std::stack<MonteCarloAction> allActions;
    while(!_unfallenCards.empty())
    {
        //assign a random card to a random player
        itCard = _unfallenCards.begin();
        std::advance(itCard,_rand->generate_number() % _unfallenCards.size() );
        tempCard = *itCard;
        std::list<PlayerMiniMonteCarlo<Memory,PlayAI> * > ableToReceive = playerReceiver(playerStillCardsToReceive,tempCard);
        auto itPlayer = ableToReceive.begin();
        std::advance(itPlayer,_rand->generate_number() % ableToReceive.size() );
        tempPlayer = *(itPlayer);

        MonteCarloAction currentAction(BasicMonteCarloAction(tempPlayer->ID(),tempCard));
        tempPlayer->ReceiveCard(tempCard);
        updatePlayer(tempPlayer,playerStillCardsToReceive); //remove the player if all its cards have been given
        _unfallenCards.erase(itCard); //the card has been given

        bool res = computeConsequences(currentAction.Consequences(),playerStillCardsToReceive);
        if(res)
        {
            allActions.emplace(currentAction);
        }
        while(!res) //undo what should nt have been done (but we did nt know at the time)
        {
            BasicMonteCarloAction oldAction = currentAction.Action();
            addConstraint(oldAction.ID(),oldAction.Card());
            _unfallenCards.push_back(oldAction.Card());
            removeConsequences(currentAction.Consequences());
            updatePlayerAdd(playerStillCardsToReceive); //add player if needed

            currentAction = allActions.pop();
            res = computeConsequences(currentAction.Consequences(),playerStillCardsToReceive);
            if(!res)
            {
                removeContraint(oldAction);
            }
        }
    }
}

template<class Memory,class PlayAI>
void  AIPlayMonteCarlo<Memory,PlayAI>::removeConsequences(const std::list<BasicMonteCarloAction>& consequences)
{
    for(const BasicMonteCarloAction& action : consequences)
    {
        _players[action.ID().ToInt()].RetrieveCard(action.Card());
        _unfallenCards.push_back(action.Card());
    }
}

template<class Memory,class PlayAI>
void AIPlayMonteCarlo<Memory,PlayAI>::updatePlayerAdd(std::list<PlayerMiniMonteCarlo<Memory,PlayAI> * >& players)
{
    players.empty();
    for(const PlayerMiniMonteCarlo<Memory,PlayAI>  player : _players)
    {
        if(player.CanReceiveAnotherCard()) _players.push_back(&player);
    }
}

//TO DO : removeConsequences
template<class Memory,class PlayAI>
bool AIPlayMonteCarlo<Memory,PlayAI>::computeConsequences(std::list<Cards_Basic>& consequences,std::list<PlayerMiniMonteCarlo<Memory,PlayAI> * >& players)
{
    Cards_Basic card;
    bool changes = true;
    while(changes)
    {
        changes = false;
        for(auto it = _unfallenCards.begin(); it != _unfallenCards.end();++it)
        {
            card = *it;
            std::list<PlayerMiniMonteCarlo<Memory,PlayAI> * > ableToReceive = playerReceiver(players,card);
            if(ableToReceive.size() == 0) //no player can receive the card
            {
                return false;
            }
            else if(ableToReceive.size() == 1)
            {
                PlayerMiniMonteCarlo<Memory,PlayAI> * pPlayer = ableToReceive.front();
                pPlayer->ReceiveCard(card);
                consequences.push_back(card);
                updatePlayer(pPlayer,players);
                changes = true;
            }
        }
        removeCard(consequences);
    }
    return true;
}

template<class Memory,class PlayAI>
void AIPlayMonteCarlo<Memory,PlayAI>::removeConstraint(const BasicMonteCarloAction& constraint)
{
    _players[constraint.ID().ToInt()].RemoveConstraint(constraint.Card());
    //_players[action.ID().ToInt()].RetrieveCard(action.Card());
    //_unfallenCards.push_back(action.Card());
}
template<class Memory,class PlayAI>
void AIPlayMonteCarlo<Memory,PlayAI>::addConstraint(const Player_ID & id,const BasicMonteCarloAction& constraint)
{
     _players[constraint.ID().ToInt()].RemoveConstraint(constraint.Card());
}

/*
template<class Memory,class PlayAI>
void AIPlayMonteCarlo<Memory,PlayAI>::removeConsequences(const std::list<BasicMonteCarloAction>& consequences)
{
    for(const auto & BasicMonteCarloAction consequence : consequences)
    {
        removeContraint(consequence);
    }
}
*/

template<class Memory,class PlayAI>
void AIPlayMonteCarlo<Memory,PlayAI>::removeCard(const std::list<Cards_Basic>& consequences) //update _unfallencards
{
    for(const auto card : consequences)
    {
        _unfallenCards.remove(card);
    }
}

template<class Memory,class PlayAI>
void AIPlayMonteCarlo<Memory,PlayAI>::updatePlayer(PlayerMiniMonteCarlo<Memory,PlayAI> * pPlayer,std::list<PlayerMiniMonteCarlo<Memory,PlayAI>  * >& players) //update _unfallencards
{
    if(!pPlayer->CanReceiveAnotherCard())
    {
        for(auto pp = players.begin(); pp != players.end(); ++pp)
        {
            if(pp->ID() == pPlayer->ID())
            {
                    players.erase(pp);
                    break;
            }
        }
    }
}


template<class Memory,class PlayAI>
bool AIPlayMonteCarlo<Memory,PlayAI>::playerReceiver(std::list<PlayerMiniMonteCarlo<Memory,PlayAI> * >& players,const Cards_Basic& card)
{
    std::list<PlayerMiniMonteCarlo<Memory,PlayAI> * > res;
    for(const auto pplayer : players)
    {
        if(pplayer -> CanReceiveCard(card)) res.push_back(pplayer);
    }
}

template<class Memory,class PlayAI>
Uint AIPlayMonteCarlo<Memory,PlayAI>::computeNbPlayerAbleToReceive(const Memory& playerMemory,const Cards_Basic& card,const TrickStatus& trickStatus)
{
    Uint res =  0;
    PLAYER_ID allIDs[4] = {PLAYER0,PLAYER1,PLAYER2,PLAYER3};
    for(Uint iIDplayer = 0; iIDplayer < 4; ++iIDplayer)
    {
        if( allIDs[iIDplayer] == trickStatus.ID().ID()) continue;
        if(playerMemory.CanReceiveCard(Player_ID(allIDs[iIDplayer]),card)) res++;
    }
    return res;
}
#endif // AIPLAYMONTECARLO_H_INCLUDED
