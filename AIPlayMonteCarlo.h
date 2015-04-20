#ifndef AIPLAYMONTECARLO_H_INCLUDED
#define AIPLAYMONTECARLO_H_INCLUDED

#include <array>
#include <stack>
#include <list>
#include <utility>

#include "Definitions.h"
#include "TrickBasic_Memory.h"
#include "Cards.h"
#include "Random.h"
#include "PlayerMiniMonteCarlo.h"
#include "TrickStatus.h"
#include "ResultMonteCarlo.h"
#include "BasicMonteCarloAction.h"
#include "MonteCarloAction.h"

#define NB_MAX_SIMUL_MONTECARLO 100
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
template<class Memory>
class AIPlayMonteCarlo
{
    protected :
        std::array<PlayerMiniMonteCarlo<Memory> , 4> _players;
        TrickBasic_Memory _trick;
        std::list<Cards_Basic> _unfallenCards;
        Random * _rand;
        const Player_ID _number;
        //const _nbMaxSimul;
    public :
        AIPlayMonteCarlo():_number(GHOST){}
        AIPlayMonteCarlo(const Player_ID & number):_number(number){}
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
        std::list<Cards*>::iterator whichBetter(ResultMonteCarlo& resSimulations,const std::list<std::list<Cards*>::iterator>& playbleCards);
        Uint computeNbPlayerAbleToReceive(const Memory& playerMemory,const Cards_Basic& card,const TrickStatus& trickStatus);

        void updateUnfallenCards(const TrickBasic_Memory& trick,const Memory& playerMemory,const std::list<Cards*>& hand);
        //void removeContraint(const BasicMonteCarloAction& constraint);
        void removeConsequences(const std::list<BasicMonteCarloAction>& consequences);
        void updatePlayerAdd(std::list< PlayerMiniMonteCarlo<Memory> * >& players);
        bool computeConsequences(std::list<BasicMonteCarloAction>& consequences,std::list<PlayerMiniMonteCarlo<Memory> * >& players);

        void addConstraint(const Player_ID & id,const Cards_Basic& card);
        void removeCard(const std::list<BasicMonteCarloAction>& consequences); //update _unfallencards
        void updatePlayer(PlayerMiniMonteCarlo<Memory> * pPlayer,std::list<PlayerMiniMonteCarlo<Memory>  * >& players); //update _unfallencards
        void removeConstraint(const BasicMonteCarloAction& constraint);
        void playerReceiver(std::list<PlayerMiniMonteCarlo<Memory> * >& temp,const std::list<PlayerMiniMonteCarlo<Memory> * >& players,const Cards_Basic& card);
        void computeNumberOfCardReceivable(std::list<Cards_Basic>& res,const std::list<Cards_Basic>& ungivenCards,PlayerMiniMonteCarlo<Memory> * player);

        void resetGiving(const TrickBasic_Memory& trick,const Memory& playerMemory,Random& rand,const std::list<Cards*>& hand);
};

template<class Memory>
std::list<Cards*>::iterator AIPlayMonteCarlo<Memory>::Play(const TrickBasic_Memory& trick
                                       ,std::list<std::list<Cards*>::iterator>& playbleCards
                                       ,const std::list<Cards*>& hand
                                       ,Random& rand
                                       ,const TrickStatus& trickStatus
                                       ,const Memory& playerMemory)
{
    if(playbleCards.size() == 1) //we have no choice... we play this card
    {
        printf("No choice, I play what I can\n");
        return playbleCards.front();
    }
    printf("here it begins for player %d (the %d of the trick)! \n",_number.ToInt(),trick.NumberCardsPlayed());
    initStates(trick,playerMemory,rand,hand); //tell everyone what append in the previous tricks
    printf("I know what happened before\n");
    std::list<Cards*>::iterator res = *playbleCards.begin();

    //initiate the result structure
    std::list<Cards_Basic> playbleBasic;
    for(const auto & itpcard : playbleCards)
    {
        playbleBasic.push_back(static_cast<Cards_Basic>(**itpcard));
    }
    ResultMonteCarlo resMonteCarlo(NB_MAX_SIMUL_MONTECARLO,playbleBasic);


    Uint tempScore;
    //simulate each game accordingly
    for(Uint nbSimul = 0; nbSimul < NB_MAX_SIMUL_MONTECARLO; ++nbSimul)
    {
        printf("simulation %d \n",nbSimul);
        //Monte Caro Simulating each hand of each players
        simulGame(trick,playerMemory,hand,trickStatus);
        printf("I have given the games \n");
        //solve the problem knowing the hand of each player
        for(auto itPCard : playbleCards)
        {
            //printf("Now I evaluate the score of the card [c:%d,h:%d]\n",(*itPCard)->GetColour().ToInt(),(*itPCard)->GetHeight().ToInt());
            tempScore = scoreCards(*itPCard); //
            resMonteCarlo.Put(nbSimul,**itPCard,tempScore);
        }
        printf("\n\n");
        resetGiving(trick,playerMemory,rand,hand);
    }
    printf("\n\n\n");
    res = whichBetter(resMonteCarlo,playbleCards);
    return res;
}

template<class Memory>
Uint AIPlayMonteCarlo<Memory>::scoreCards(const Cards* pcard)
{
    return 0;
}

template<class Memory>
std::list<Cards*>::iterator AIPlayMonteCarlo<Memory>::whichBetter(ResultMonteCarlo& resSimulations,
                                                                         const std::list<std::list<Cards*>::iterator>& playbleCards)
{
    std::list<Cards*>::iterator res = playbleCards.front();
    double maxScore = 0;
    double temp;
    for(auto it = playbleCards.begin(); it != playbleCards.end(); ++it)
    {
        temp = resSimulations.ScoreAggr(static_cast<const Cards_Basic&>(***it));
        if(temp > maxScore)
        {
            maxScore = temp;
            res = *it;
        }
    }
    return res;
}

template<class Memory>
void AIPlayMonteCarlo<Memory>::initStates(const TrickBasic_Memory& trick,const Memory& playerMemory,Random& rand,const std::list<Cards*>& hand)
{
    for(Uint i = 0; i < 4; ++i)
    {
        _players[i] = PlayerMiniMonteCarlo<Memory>(Player_ID(i));
        _players[i].ReceiveInitInfo(rand);
    }
    _rand = &rand;
    resetGiving(trick,playerMemory,rand,hand);
}


template<class Memory>
void AIPlayMonteCarlo<Memory>::resetGiving(const TrickBasic_Memory& trick,const Memory& playerMemory,Random& rand,const std::list<Cards*>& hand)
{
    updateUnfallenCards(trick,playerMemory,hand);
    Uint nbTrick = 8 - trick.TrickNumber() -1;
    Uint nbCardsPlayed = trick.NumberCardsPlayed();
    Player_ID id = _number;

    Uint  i = 0;
    //give the cards to the player that have not played yet
    for( ; i < 4-nbCardsPlayed;++i)
    {
        _players[id.ToInt()].ReceiveCardsInfo(nbTrick+1,_unfallenCards,playerMemory);
        id.Next();
    }
    //init the payer that have already played
    for( ; i < 4; ++i)
    {
        _players[id.ToInt()].ReceiveCardsInfo(nbTrick,_unfallenCards,playerMemory);
        id.Next();
    }
    for(Uint i = 0; i < 4; ++i)
    {
        printf("player %d : still %d card to be given\n",i,_players[i].NumberCardToReceive());
    }
}
template<class Memory>
void AIPlayMonteCarlo<Memory>::updateUnfallenCards(const TrickBasic_Memory& trick,const Memory& playerMemory,const std::list<Cards*>& hand)
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
                _unfallenCards.push_back(Cards_Basic(height,col));
                printf("cards : color %d; height %d",col.ToInt(),height.ToInt());
                printf("=> :");

                for(Uint iPlayer = 0; iPlayer < 4; ++iPlayer)
                {
                    if (iPlayer != _number.ToInt() && !playerMemory.Cut(Player_ID(iPlayer),col)) printf("%d ; ",iPlayer);
                }
                printf("\n");
            }
        }
    }
    printf("I have thus %d cards to give \n",_unfallenCards.size());
    //printf("The constraint are : \n");
}

template<class Memory>
bool AIPlayMonteCarlo<Memory>::cardsInHand(const std::list<Cards*>& hand,const Card_Color& col,const Card_Height& height)
{
    for(auto pcard : hand)
    {
        if(pcard->GetColour() == col && pcard->GetHeight() == height) return true;
    }
    return false;
}

template<class Memory>
void AIPlayMonteCarlo<Memory>::simulGame(const TrickBasic_Memory& trick,const Memory& playerMemory,const std::list<Cards*>& hand,const TrickStatus& trickStatus)
{
    ///compute how many cards each player must receive
    ///compute whom can received which card
    ///give each card randomly to each player, starting by the card only a few player can receive

    _players[trickStatus.ID().ToInt()].ReceiveHand(hand);
    //printf("I have given my hand to myself\n");
    std::list<PlayerMiniMonteCarlo<Memory> * > playerStillCardsToReceive;
    for(Uint iPlayer = 0; iPlayer < 4; ++iPlayer)
    {
        if(_players[iPlayer].ID() != trickStatus.ID()) playerStillCardsToReceive.push_back(&_players[iPlayer]);
    }
    //printf("I know which players have still cards to receive\n");

    std::stack<MonteCarloAction> allActions;
    MonteCarloAction currentActions;
    //printf("the number of card not fallen is %d\n",_unfallenCards.size());
    printf("I will compute the primal consequences\n");
    computeConsequences(currentActions.Consequences(),playerStillCardsToReceive);
    allActions.push(std::move(currentActions));
    //bool primCons = computeConsequences(action.Consequences(),playerStillCardsToReceive);
    //printf("have compute the primal consequences : %d\n",primCons);
    auto itCard = _unfallenCards.begin();

    Cards_Basic tempCard;
    PlayerMiniMonteCarlo<Memory> * tempPlayer;
    BasicMonteCarloAction oldAction;

    while(!_unfallenCards.empty())
    {
        //printf("Now it is time to give one card \n");
        //assign a random card to a random player
        itCard = _unfallenCards.begin();
        std::advance(itCard,_rand->generate_number() % (_unfallenCards.size()-1) );
        tempCard = *itCard;

        auto itPlayer = playerStillCardsToReceive.begin();
        std::advance(itPlayer,_rand->generate_number() % (playerStillCardsToReceive.size()-1) );
        tempPlayer = *(itPlayer);
        printf("I have given a card [c:%d,h:%d] to the player %d \n",tempCard.GetColour().ToInt(),tempCard.GetHeight().ToInt(),tempPlayer->ID().ToInt());

        currentActions = MonteCarloAction(BasicMonteCarloAction(tempPlayer->ID(),tempCard));
        //I give the card to the player
        tempPlayer->ReceiveCard(tempCard);
        //I update the player able to receive other cards
        updatePlayer(tempPlayer,playerStillCardsToReceive);
        //the card has been given : I remove it from the cards to be given
        _unfallenCards.erase(itCard);

        bool res = false;
        Uint nbFailure = 0;
        while(1) //undo what should nt have been done (but we did nt know at the time)
        {
            res = computeConsequences(currentActions.Consequences(),playerStillCardsToReceive);

            if(res) //I was right to do so
            {
                allActions.push( std::move( currentActions ));
                break;
            }
            printf("The stack has no a size of %d\n",allActions.size());

            //Removing the previous constraint (if any) :
            if(nbFailure != 0)
            {
                //TO DO : un truc plus clean bordel !
                printf("I remove the constraint :[c:%d,h:%d] -> %d \n",
                       oldAction.Card().GetColour().ToInt(),
                       oldAction.Card().GetHeight().ToInt(),
                       oldAction.ID().ToInt()
                       );
                removeConstraint(oldAction);
            }
            ++nbFailure;


            //Adding the new one :
            printf("I add the constraint :[c:%d,h:%d] -> %d \n",
                   currentActions.Action().Card().GetColour().ToInt(),
                   currentActions.Action().Card().GetHeight().ToInt(),
                   currentActions.Action().ID().ToInt()
                   );
            addConstraint(currentActions.Action().ID(),currentActions.Action().Card());

            //This cards must be given again
            printf("I update _unfallen \n");
             _players[currentActions.Action().ID().ToInt()].RetrieveCard(currentActions.Action().Card());
            _unfallenCards.push_back(currentActions.Action().Card());

            //get back the cards given as a consequence of the previous one
            printf("I remove the consequences \n");
            removeConsequences(currentActions.Consequences());

            printf("I have still %d card to give\n",_unfallenCards.size());
            printf("I must give : \n");
            for(const auto & card : _unfallenCards)
            {
                printf("[c:%d,h:%d]\n",card.GetColour().ToInt(),card.GetHeight().ToInt());
            }

            //...the players
            printf("I update the players who can still receive some cards\n");
            updatePlayerAdd(playerStillCardsToReceive);

            oldAction = currentActions.Action();
            currentActions = std::move(allActions.top());

            //continue to go down the stack
            allActions.pop();

            printf("action [c:%d,h:%d] -> %d => %d consequences\n",
                   currentActions.Action().Card().GetColour().ToInt(),
                   currentActions.Action().Card().GetHeight().ToInt(),
                   currentActions.Action().ID().ToInt(),
                   currentActions.Consequences().size());

            printf("I go down the stack\n\n");
        }
        printf("I give another card\n\n\n");
    }
}

template<class Memory>
void  AIPlayMonteCarlo<Memory>::removeConsequences(const std::list<BasicMonteCarloAction>& consequences)
{
    for(const BasicMonteCarloAction& action : consequences)
    {
        printf("removeConsequences for player %d who took [c:%d,h:%d]\n",action.ID().ToInt(),action.Card().GetColour().ToInt(),action.Card().GetHeight().ToInt());
        _players[action.ID().ToInt()].RetrieveCard(action.Card());
        _unfallenCards.push_back(action.Card());
    }
}

template<class Memory>
void AIPlayMonteCarlo<Memory>::updatePlayerAdd(std::list<PlayerMiniMonteCarlo<Memory> * >& players)
{
    players.clear();
    for(Uint i = 0; i < 4 ;++i)
    {
        if(_players[i].CanReceiveAnotherCard()) players.push_back(&_players[i]);
    }
}

template<class Memory>
bool AIPlayMonteCarlo<Memory>::computeConsequences(std::list<BasicMonteCarloAction>& consequences,std::list<PlayerMiniMonteCarlo<Memory> * >& players)
{
    Cards_Basic card;
    bool changes = true;
    bool res = true;

    std::list<PlayerMiniMonteCarlo<Memory> * > tempPlayer;
    std::list<Cards_Basic> tempCard;
    while(changes)
    {
        changes = false;
        //removing the cards that can be given to only one player
        for(auto it = _unfallenCards.begin(); it != _unfallenCards.end();++it)
        {

            card = *it;
            playerReceiver(tempPlayer,players,card);
            if(tempPlayer.size() == 0) //no player can receive the card
            {
                printf("No one can received [c:%d,h:%d]\n",card.GetColour().ToInt(),card.GetHeight().ToInt());
                changes = false;
                res = false;
                break;
            }
            else if(tempPlayer.size() == 1)
            {
                PlayerMiniMonteCarlo<Memory> * pPlayer = tempPlayer.front();
                pPlayer->ReceiveCard(card);
                consequences.push_back(BasicMonteCarloAction(pPlayer->ID(),card));
                updatePlayer(pPlayer,players);
                printf("I am force to give [c:%d;h:%d] to %d \n",card.GetColour().ToInt(),card.GetHeight().ToInt(),pPlayer->ID().ToInt());
                res = true;
                changes = true;
            }
        }
        removeCard(consequences);

        //removing the players that can receive exactly the number of card that can be given to them
        for(auto itPlayer = players.begin(); itPlayer !=players.end();++itPlayer)
        {
            computeNumberOfCardReceivable(tempCard,_unfallenCards,(*itPlayer));
            Uint numb = (*itPlayer)->NumberCardToReceive();
            if(tempCard.size() < numb)
            {
                printf("The player %d cannot receive enough cards\n",(*itPlayer)->ID().ToInt() );
                changes = false;
                res = false;
                break;
            }
            else if(tempCard.size() == numb)
            {
                for(auto itPcardToGive = tempCard.begin(); itPcardToGive != tempCard.end(); ++itPcardToGive)
                {
                    card = *itPcardToGive;
                    (*itPlayer)->ReceiveCard(card);
                    _unfallenCards.remove(card);
                    consequences.push_back(BasicMonteCarloAction((*itPlayer)->ID(),card));
                    printf("I am force to give [c:%d;h:%d] to %d \n",card.GetColour().ToInt(),card.GetHeight().ToInt(),(*itPlayer)->ID().ToInt() );
                }
                res = true;
                changes = true;
            }
        }
        updatePlayerAdd(players);
    }
    return res;
}
template<class Memory>
void AIPlayMonteCarlo<Memory>::computeNumberOfCardReceivable(std::list<Cards_Basic>& res,const std::list<Cards_Basic>& ungivenCards,PlayerMiniMonteCarlo<Memory> * player)
{
    res.clear();
    for(const Cards_Basic& card :ungivenCards )
    {
        if(player->CanReceiveCard(card)) res.push_back(card);
    }
}

template<class Memory>
void AIPlayMonteCarlo<Memory>::removeConstraint(const BasicMonteCarloAction& constraint)
{
    _players[constraint.ID().ToInt()].RemoveConstraint(constraint.Card());
}
template<class Memory>
void AIPlayMonteCarlo<Memory>::addConstraint(const Player_ID & id,const Cards_Basic& card)
{
     _players[id.ToInt()].AddConstraint(card);
}

template<class Memory>
void AIPlayMonteCarlo<Memory>::removeCard(const std::list<BasicMonteCarloAction>& consequences) //update _unfallencards
{
    for(const auto& action : consequences)
    {
        _unfallenCards.remove(action.Card());
    }
}

template<class Memory>
void AIPlayMonteCarlo<Memory>::updatePlayer(PlayerMiniMonteCarlo<Memory> * pPlayer,std::list<PlayerMiniMonteCarlo<Memory>  * >& players) //update _unfallencards
{
    if(!pPlayer->CanReceiveAnotherCard())
    {
        for(auto pp = players.begin(); pp != players.end(); ++pp)
        {
            if((*pp)->ID() == pPlayer->ID())
            {
                    players.erase(pp);
                    break;
            }
        }
    }
}


template<class Memory>
void AIPlayMonteCarlo<Memory>::playerReceiver(std::list<PlayerMiniMonteCarlo<Memory> * >& temp,const std::list<PlayerMiniMonteCarlo<Memory> * >& players,const Cards_Basic& card)
{
    temp.clear();
    for(Uint i = 0; i < 4 ;++i)
    {
        if(_players[i].CanReceiveCard(card)) temp.push_back(&_players[i]);
    }
}

template<class Memory>
Uint AIPlayMonteCarlo<Memory>::computeNbPlayerAbleToReceive(const Memory& playerMemory,const Cards_Basic& card,const TrickStatus& trickStatus)
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
