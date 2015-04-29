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

#include "DebugwithPrint.h"
#define PRINT_AIPLAYMC 0
//#define NB_MAX_SIMUL_MONTECARLO 100
/**
idee : on simule les cartes possibles des autres joueurs par Monte Carlo
(optimisation possible : on essaie de sur ponderer les situations mauvaises pour les ennemis
puis les situations bonnes)
on resout le jeu dans chaque simulation : on est en information complete donc pourquoi pas...
on renvoie la carte qui nous parait la mieux : a definir ce qu'on entend par mieux...

ca serait bien de faire un thread la dessus... pour que l'ordi (interface graphique) ne freeze pas pendant que l'ordi
reflechi...
**/

template<Uint nbMaxSimul, template<class> class MemoryPlayer, class PlayMC>
class AIPlayMonteCarlo
{
    protected :
		std::array< PlayerMiniMonteCarlo, 4> _players;
        TrickBasic_Memory _trick;
        std::list<Cards_Basic> _unfallenCards;
        Random * _rand;
        const Player_ID _number;
        PlayMC _play;
        Uint _cardsToPlay;
        WrapperPrint<PRINT_AIPLAYMC> _printf;
        //const _nbMaxSimul;
    public :
        //AIPlayMonteCarlo():_number(GHOST){}
        AIPlayMonteCarlo(const Player_ID & number):_number(number){}
        virtual ~AIPlayMonteCarlo(){}
        Cards* Play(const TrickBasic_Memory& trick
                    ,std::list<Cards* >& plyableCards
                    ,std::list<Cards* >& hand
                    ,Random& rand
                    ,const TrickStatus& trickStatus
					, const MemoryPlayer<Cards*>& playerMemory);
    protected :
		void initStates(const TrickBasic_Memory& trick, const MemoryPlayer<Cards*>& playerMemory, Random& rand, const std::list<Cards*>& hand);
		void updateUnfallenCards(const TrickBasic_Memory& trick, const MemoryPlayer<Cards*> & playerMemory, const std::list<Cards*>& hand);
		Uint scoreCards(const std::array<PlayerMiniMonteCarlo, 4>& currentPlayers,
                          const Cards* pcard,
                          TrickBasic_Memory& currentTrick,
                          const std::list<Cards*> hand,
                          const TrickStatus& trickStatus);
        void simulGame(const TrickBasic_Memory& trick,
			const MemoryPlayer<Cards*>& playerMemory, //here
                         const std::list<Cards*>& hand,
                         const TrickStatus& trickStatus,
                         std::stack<MonteCarloAction>& allActions,
						 std::array<PlayerMiniMonteCarlo, 4 >& currentPlayers,
                         std::list<Cards_Basic>&  currentCards,
						 std::list<PlayerMiniMonteCarlo * >& playerStillCardsToReceive);

        bool cardsInHand(const std::list<Cards*>& hand,const Card_Color& col,const Card_Height& height);
        Cards* whichBetter(ResultMonteCarlo& resSimulations,const std::list<Cards* >& playbleCards);
		Uint computeNbPlayerAbleToReceive(const MemoryPlayer<Cards*>& playerMemory, const Cards_Basic& card, const TrickStatus& trickStatus);

		//void updateUnfallenCards(const TrickBasic_Memory& trick, const MemoryPlayer<Cards*>& playerMemory, const std::list<Cards*>& hand);
        //void removeContraint(const BasicMonteCarloAction& constraint);
		void removeConsequences(std::array<PlayerMiniMonteCarlo, 4 > & currentPlayers,
                                const std::list<BasicMonteCarloAction>& consequences,std::list<Cards_Basic>& currentCard);
		void updatePlayerAdd(std::array<PlayerMiniMonteCarlo, 4 > & currentPlayers,
			std::list< PlayerMiniMonteCarlo * >& players);
		bool computeConsequences(std::array<PlayerMiniMonteCarlo, 4 > & currentPlayers,
                                 std::list<BasicMonteCarloAction>& consequences,
                                 std::list<Cards_Basic>& currentCards,
								 std::list<PlayerMiniMonteCarlo * >& players);

		void addConstraint(std::array<PlayerMiniMonteCarlo, 4 > & currentPlayers,
                           const Player_ID & id,
                           const Cards_Basic& card);
        void removeCard(const std::list<BasicMonteCarloAction>& consequences,std::list<Cards_Basic>& currentCard);
		void updatePlayer(PlayerMiniMonteCarlo * pPlayer, std::list<PlayerMiniMonteCarlo * >& players);
		void removeConstraint(std::array<PlayerMiniMonteCarlo, 4 > & currentPlayers,
                              const BasicMonteCarloAction& constraint);
		void playerReceiver(std::array<PlayerMiniMonteCarlo, 4 > & currentPlayers,
			std::list<PlayerMiniMonteCarlo* >& temp,
			const std::list<PlayerMiniMonteCarlo * >& players,
                            const Cards_Basic& card);
        void computeNumberOfCardReceivable(std::list<Cards_Basic>& res,
                                           const std::list<Cards_Basic>& ungivenCards,
										   PlayerMiniMonteCarlo * player);

        void resetGiving(const TrickBasic_Memory& trick,
			const MemoryPlayer<Cards*>& playerMemory,
							Random& rand,
							const std::list<Cards*>& hand);

		void removeForceCard(std::array<PlayerMiniMonteCarlo, 4 > & currentPlayers,
                             bool& changes,
                             bool& res,
                             std::list<BasicMonteCarloAction>& consequences,
                             std::list<Cards_Basic>& currentCards,
							 std::list<PlayerMiniMonteCarlo * >& players);

		void removeForcePlayer(std::array<PlayerMiniMonteCarlo, 4 > & currentPlayers,
                                bool& changes,
                                bool& res,
                                std::list<BasicMonteCarloAction>& consequences,
                                std::list<Cards_Basic>& currentCards,
								std::list<PlayerMiniMonteCarlo * >& players);
};

template<Uint nbMaxSimul, template<class> class MemoryPlayer, class PlayMC>
Cards* AIPlayMonteCarlo<nbMaxSimul, MemoryPlayer, PlayMC>::Play(const TrickBasic_Memory& trick
												,std::list<Cards*> & playbleCards
												,std::list<Cards*>& hand
												,Random& rand
												,const TrickStatus& trickStatus
												, const MemoryPlayer<Cards*>& playerMemory)
{
    if(playbleCards.size() == 1) //we have no choice... we play this card
    {
        _printf("No choice, I play what I can\n");
        return playbleCards.front();
    }

	_printf("The cards played are :");
	for (Uint i = 0; i < trick.NumberCardsPlayed(); ++i)
	{
        _printf("[%d,%d];", trick[i]->GetColour().ToInt(), trick[i]->GetHeight().ToInt());
	}
	_printf("\n");
    _printf("I know what happened before\n");
    Cards* res = playbleCards.front();

    //initiate the result structure
    std::list<Cards_Basic> playbleBasic;
    for(const auto & pcard : playbleCards)
    {
		playbleBasic.push_back(static_cast<const Cards_Basic&>(*pcard));
    }
	ResultMonteCarlo resMonteCarlo(nbMaxSimul, playbleBasic);

    //init the players and the cards to be given
    //give my hand to my mini-me
    initStates(trick,playerMemory,rand,hand); //tell everyone what append in the previous tricks
    _printf("I give the hand (%d cards) to player %d \n",hand.size(),_number.ToInt());
    _players[_number.ToInt()].ReceiveHand(hand);

	std::list<PlayerMiniMonteCarlo * > playerStillCardsToReceive;
    for(Uint iPlayer = 0; iPlayer < 4; ++iPlayer)
    {
        if(_players[iPlayer].ID() != _number) playerStillCardsToReceive.push_back(&_players[iPlayer]);
    }

    std::stack<MonteCarloAction> allActions;
    MonteCarloAction currentActions;

    _printf("I will compute the primal consequences for player %d\n",_number.ToInt());
    computeConsequences(_players,currentActions.Consequences(),_unfallenCards,playerStillCardsToReceive);
    allActions.push(std::move(currentActions));
    std::list<Uint> indexPlayer;
    for(const auto & player : playerStillCardsToReceive)
    {
        indexPlayer.push_back(player->ID().ToInt());
    }


    //simulate each game
    Uint tempScore;
	_printf("I am done -> starting the games\n");
	for (Uint nbSimul = 0; nbSimul < nbMaxSimul ; ++nbSimul)
    {
        _printf("Simulation %d\n",nbSimul);

        std::stack<MonteCarloAction> currentStack(allActions);
        std::list<Cards_Basic>  currentCards = _unfallenCards;
		std::array<PlayerMiniMonteCarlo, 4> currentPlayers = _players;

		std::list<PlayerMiniMonteCarlo * > currentPlayerStillCardsToReceive;
        for(Uint iPlayer : indexPlayer)
        {
            currentPlayerStillCardsToReceive.push_back(&currentPlayers[iPlayer]);
        }

        //Monte Caro Simulating each hand of each players
        simulGame(trick,playerMemory,hand,trickStatus,currentStack,currentPlayers,currentCards,currentPlayerStillCardsToReceive);
        _printf("I look at the %d cards\n",playbleCards.size());
        TrickBasic_Memory currentTrick;
        //solve the problem knowing the hand of each player for each card
        for(auto pCard : playbleCards)
        {
            _printf("I look at [c:%d,h:%d]\n",pCard->GetColour().ToInt(),pCard->GetHeight().ToInt());
            currentTrick = trick ;
            //play the game knowing everything and compute the score of the player (in a minimax / alpha-beta situation)
            _printf("I compute the score for it\n");
			tempScore = scoreCards(currentPlayers, pCard, currentTrick, hand, trickStatus);

            //store the result in the proper structure
            _printf("I put in the result\n");
			resMonteCarlo.Put(nbSimul, *pCard, tempScore);
            currentTrick.Reset();
        }
        _printf("I am done looking at the cards.\n");
        //resetGiving(trick,playerMemory,rand,hand);
    }
    res = whichBetter(resMonteCarlo,playbleCards);
    return res;
}

template<Uint nbMaxSimul, template<class> class MemoryPlayer, class PlayMC>
Uint AIPlayMonteCarlo<nbMaxSimul, MemoryPlayer, PlayMC>::scoreCards(const std::array<PlayerMiniMonteCarlo, 4>& currentPlayers,
                                                  const Cards* pcard,
                                                  TrickBasic_Memory& currentTrick,
                                                  const std::list<Cards*> hand,
                                                  const TrickStatus& trickStatus)
{

    _play.Init(_cardsToPlay,_number,_rand);
    return _play.Play(currentPlayers,pcard,currentTrick,hand,trickStatus);
}

template<Uint nbMaxSimul, template<class> class MemoryPlayer, class PlayMC>
Cards* AIPlayMonteCarlo<nbMaxSimul, MemoryPlayer, PlayMC>::whichBetter(ResultMonteCarlo& resSimulations,
														const std::list<Cards*>& playableCards)
{
	Cards* res = playableCards.front();
    double maxScore = 0.0;
    double temp;
	for (Cards* pcard : playableCards)
    {
		temp = resSimulations.ScoreAggr(static_cast<const Cards_Basic&>(*pcard));
        if(temp > maxScore)
        {
            maxScore = temp;
            res = pcard;
        }
    }
    return res;
}

template<Uint nbMaxSimul, template<class> class MemoryPlayer, class PlayMC>
void AIPlayMonteCarlo<nbMaxSimul, MemoryPlayer, PlayMC>::initStates(const TrickBasic_Memory& trick, const MemoryPlayer<Cards*>& playerMemory, Random& rand, const std::list<Cards*>& hand)
{
    for(Uint i = 0; i < 4; ++i)
    {
		_players[i] = PlayerMiniMonteCarlo(Player_ID(i));
        _players[i].ReceiveInitInfo(rand);
    }
    _rand = &rand;
    updateUnfallenCards(trick,playerMemory,hand);
    resetGiving(trick,playerMemory,rand,hand);
}


template<Uint nbMaxSimul, template<class> class MemoryPlayer, class PlayMC>
void AIPlayMonteCarlo<nbMaxSimul, MemoryPlayer, PlayMC>::resetGiving(
	const TrickBasic_Memory& trick,
	const MemoryPlayer<Cards*>& playerMemory,
	Random& rand,
	const std::list<Cards*>& hand)
{
    Uint nbTrick = 8 - trick.TrickNumber() -1;
    Uint nbCardsPlayed = trick.NumberCardsPlayed();
    Player_ID id = _number;
    Uint  i = 0;
    //give the cards to the player that have not played yet
    for( ; i < 4-nbCardsPlayed;++i)
    {
		_players[id.ToInt()].ReceiveCardsInfo(nbTrick + 1, _unfallenCards, playerMemory);
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
        _printf("player %d : still %d card to be given\n",i,_players[i].NumberCardToReceive());
    }

}

template<Uint nbMaxSimul, template<class> class MemoryPlayer, class PlayMC>
void AIPlayMonteCarlo<nbMaxSimul, MemoryPlayer, PlayMC>::updateUnfallenCards(const TrickBasic_Memory& trick, const MemoryPlayer<Cards*>& playerMemory, const std::list<Cards*>& hand)
{
    CARDS_COLOR colors[4] = {DIAMOND,HEART,SPADE,CLUB};
    CARDS_HEIGHT heights[8] = {SEVEN,EIGHT,NINE,TEN,JACK,QUEEN,KING,ACE};
    Card_Color col;
    Card_Height height;
    _unfallenCards.clear();
    _cardsToPlay = 0;
    for(Uint iCol = 0; iCol < 4; ++iCol)
    {
        for(Uint iHeight = 0; iHeight < 8; ++iHeight)
        {
            col = Card_Color(colors[iCol]);
            height = Card_Height(heights[iHeight]);
            if(!playerMemory.CardsFallen(col,height) && !trick.IsFallen(col,height) && !cardsInHand(hand,col,height))
            {
                _unfallenCards.push_back(Cards_Basic(height,col));
                ++_cardsToPlay;
                _printf("cards : color %d; height %d",col.ToInt(),height.ToInt());
                _printf("=> :");
                for(Uint iPlayer = 0; iPlayer < 4; ++iPlayer)
                {
                    if (iPlayer != _number.ToInt() && !playerMemory.CanReceiveCard(Player_ID(iPlayer),col,height)) _printf("%d ; ",iPlayer);
                }
                _printf("\n");

            }
        }
    }
	_printf("\n");
    _cardsToPlay += hand.size();
}

template<Uint nbMaxSimul, template<class> class MemoryPlayer, class PlayMC>
bool AIPlayMonteCarlo<nbMaxSimul, MemoryPlayer, PlayMC>::cardsInHand(const std::list<Cards*>& hand, const Card_Color& col, const Card_Height& height)
{
    for(auto pcard : hand)
    {
        if(pcard->GetColour() == col && pcard->GetHeight() == height) return true;
    }
    return false;
}

template<Uint nbMaxSimul, template<class> class MemoryPlayer, class PlayMC>
void AIPlayMonteCarlo<nbMaxSimul, MemoryPlayer, PlayMC>::simulGame(const TrickBasic_Memory& trick,
	const MemoryPlayer<Cards*>& playerMemory,
											const std::list<Cards*>& hand,
											const TrickStatus& trickStatus,
											std::stack<MonteCarloAction>& allActions,
											std::array<PlayerMiniMonteCarlo, 4 >& currentPlayers,
											std::list<Cards_Basic>&  currentCards,
											std::list<PlayerMiniMonteCarlo * >& playerStillCardsToReceive)
{
    ///compute how many cards each player must receive
    ///compute whom can received which card
    ///give each card randomly to each player, starting by the card only a few player can receive

    auto itCard = currentCards.begin();

    Cards_Basic tempCard;
	PlayerMiniMonteCarlo * tempPlayer;
    BasicMonteCarloAction oldAction;
    MonteCarloAction currentActions;

    while(!currentCards.empty())
    {
        //assign a random card to a random player
        itCard = currentCards.begin();
        std::advance(itCard,_rand->generate_number() % (currentCards.size()-1) );
        tempCard = *itCard;

        auto itPlayer = playerStillCardsToReceive.begin();
        std::advance(itPlayer,_rand->generate_number() % (playerStillCardsToReceive.size()-1) );
        tempPlayer = *(itPlayer);
        _printf("I have given a card [c:%d,h:%d] to the player %d \n",tempCard.GetColour().ToInt(),tempCard.GetHeight().ToInt(),tempPlayer->ID().ToInt());

        currentActions = MonteCarloAction(BasicMonteCarloAction(tempPlayer->ID(),tempCard));
        //I give the card to the player
        tempPlayer->ReceiveCard(tempCard);
        //I update the player able to receive other cards
        updatePlayer(tempPlayer,playerStillCardsToReceive);
        //the card has been given : I remove it from the cards to be given
        currentCards.erase(itCard);

        bool res = false;
        Uint nbFailure = 0;
        while(1) //undo what should nt have been done (but we did nt know at the time)
        {
            res = computeConsequences(currentPlayers,currentActions.Consequences(),currentCards,playerStillCardsToReceive);

            if(res) //I was right to do so
            {
                allActions.push( std::move( currentActions ));
                break;
            }
            _printf("The stack has no a size of %d\n",allActions.size());

            //Removing the previous constraint (if any) :
            if(nbFailure != 0)
            {
                //TO DO : un truc plus clean bordel !
                _printf("I remove the constraint :[c:%d,h:%d] -> %d \n",
                       oldAction.Card().GetColour().ToInt(),
                       oldAction.Card().GetHeight().ToInt(),
                       oldAction.ID().ToInt()
                       );
                removeConstraint(currentPlayers,oldAction);
            }
            ++nbFailure;


            //Adding the new one :
            _printf("I add the constraint :[c:%d,h:%d] -> %d \n",
                   currentActions.Action().Card().GetColour().ToInt(),
                   currentActions.Action().Card().GetHeight().ToInt(),
                   currentActions.Action().ID().ToInt()
                   );
            addConstraint(currentPlayers,currentActions.Action().ID(),currentActions.Action().Card());

            //This cards must be given again
            _printf("I update _unfallen \n");
            currentPlayers[currentActions.Action().ID().ToInt()].RetrieveCard(currentActions.Action().Card());
            currentCards.push_back(currentActions.Action().Card());

            //get back the cards given as a consequence of the previous one
            _printf("I remove the consequences \n");
            removeConsequences(currentPlayers,currentActions.Consequences(),currentCards);


            _printf("I have still %d card to give\n",currentCards.size());
            _printf("I must give : \n");
			for (const auto & card : currentCards)
			{
				_printf("[c:%d,h:%d]\n", card.GetColour().ToInt(), card.GetHeight().ToInt());
			}

            //...the players
            _printf("I update the players who can still receive some cards\n");
            updatePlayerAdd(currentPlayers,playerStillCardsToReceive);

            oldAction = currentActions.Action();
            currentActions = std::move(allActions.top());

            //continue to go down the stack
            allActions.pop();

            _printf("action [c:%d,h:%d] -> %d => %d consequences\n",
                   currentActions.Action().Card().GetColour().ToInt(),
                   currentActions.Action().Card().GetHeight().ToInt(),
                   currentActions.Action().ID().ToInt(),
                   currentActions.Consequences().size());

            _printf("I go down the stack\n\n");
        }
        _printf("I can give another card\n\n\n");
    }
}

template<Uint nbMaxSimul, template<class> class MemoryPlayer, class PlayMC>
void  AIPlayMonteCarlo<nbMaxSimul, MemoryPlayer, PlayMC>::removeConsequences(std::array<PlayerMiniMonteCarlo, 4 > &currentPlayers,
                                                   const std::list<BasicMonteCarloAction>& consequences,
                                                   std::list<Cards_Basic>& currentCard)
{
    for(const BasicMonteCarloAction& action : consequences)
    {
        _printf("removeConsequences for player %d who took [c:%d,h:%d]\n",action.ID().ToInt(),action.Card().GetColour().ToInt(),action.Card().GetHeight().ToInt());
        currentPlayers[action.ID().ToInt()].RetrieveCard(action.Card());
        currentCard.push_back(action.Card());
    }
}

template<Uint nbMaxSimul, template<class> class MemoryPlayer, class PlayMC>
void AIPlayMonteCarlo<nbMaxSimul, MemoryPlayer, PlayMC>::updatePlayerAdd(std::array<PlayerMiniMonteCarlo, 4 > & currentPlayers,
																							std::list<PlayerMiniMonteCarlo * >& players)
{
    players.clear();
    for(Uint i = 0; i < 4 ;++i)
    {
        if(currentPlayers[i].CanReceiveAnotherCard()) players.push_back(&currentPlayers[i]);
    }
}

template<Uint nbMaxSimul, template<class> class MemoryPlayer, class PlayMC>
bool AIPlayMonteCarlo<nbMaxSimul, MemoryPlayer, PlayMC>::computeConsequences(std::array<PlayerMiniMonteCarlo, 4 > & currentPlayers,
                                                   std::list<BasicMonteCarloAction>& consequences,
                                                   std::list<Cards_Basic>& currentCards,
												   std::list<PlayerMiniMonteCarlo * >& players)
{
    bool changes = true;
    bool res = true;

    Cards_Basic card;
	std::list<PlayerMiniMonteCarlo * > tempPlayer;
    std::list<Cards_Basic> tempCard;
    while(changes)
    {
        changes = false;
        //removing the cards that can be given to only one player
        removeForceCard(currentPlayers,changes,res,consequences,currentCards,players);

        //removing the players that can receive exactly the number of card that can be given to them
        removeForcePlayer(currentPlayers,changes,res,consequences,currentCards,players);
    }
    _printf("The result was %d \n",res);
    return res;
}

template<Uint nbMaxSimul, template<class> class MemoryPlayer, class PlayMC>
void AIPlayMonteCarlo<nbMaxSimul, MemoryPlayer, PlayMC>::removeForceCard(std::array<PlayerMiniMonteCarlo, 4 > & currentPlayers,
                                                bool& changes,
                                                bool& res,
                                                std::list<BasicMonteCarloAction>& consequences,
                                                std::list<Cards_Basic>& currentCards,
												std::list<PlayerMiniMonteCarlo * >& players)
{
    Cards_Basic card;
	std::list<PlayerMiniMonteCarlo * > tempPlayer;

    for(auto it = currentCards.begin(); it != currentCards.end();++it)
    {
        card = *it;
        playerReceiver(currentPlayers,tempPlayer,players,card);
        if(tempPlayer.size() == 0) //no player can receive the card
        {
            _printf("No one can received [c:%d,h:%d]\n",card.GetColour().ToInt(),card.GetHeight().ToInt());
            changes = false;
            res = false;
            break;
        }
        else if(tempPlayer.size() == 1)
        {
			PlayerMiniMonteCarlo * pPlayer = tempPlayer.front();
            consequences.push_back(BasicMonteCarloAction(pPlayer->ID(),card));
            pPlayer->ReceiveCard(card);
            updatePlayer(pPlayer,players);
            _printf("I am force to give [c:%d;h:%d] to %d \n",card.GetColour().ToInt(),card.GetHeight().ToInt(),pPlayer->ID().ToInt());
			res = res && true;
            changes = true;
        }
    }
    removeCard(consequences,currentCards);
}

template<Uint nbMaxSimul, template<class> class MemoryPlayer, class PlayMC>
void AIPlayMonteCarlo<nbMaxSimul, MemoryPlayer, PlayMC>::removeForcePlayer(std::array<PlayerMiniMonteCarlo, 4 > & currentPlayers,
                                                    bool& changes,
                                                    bool& res,
                                                    std::list<BasicMonteCarloAction>& consequences,
                                                    std::list<Cards_Basic>& currentCards,
													std::list<PlayerMiniMonteCarlo * >& players)
{
    Cards_Basic card;
    std::list<Cards_Basic> tempCard;

    for(auto itPlayer = players.begin(); itPlayer !=players.end();++itPlayer)
    {
        computeNumberOfCardReceivable(tempCard,currentCards,(*itPlayer));
        Uint numb = (*itPlayer)->NumberCardToReceive();
        if(tempCard.size() < numb)
        {
			_printf("The player %d cannot receive enough cards : nbTh = %d, nbReal = %d\n", (*itPlayer)->ID().ToInt(), tempCard.size(), numb);
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
                currentCards.remove(card);
                consequences.push_back(BasicMonteCarloAction((*itPlayer)->ID(),card));
                _printf("I am force to give [c:%d;h:%d] to %d \n",card.GetColour().ToInt(),card.GetHeight().ToInt(),(*itPlayer)->ID().ToInt() );
            }
            res = res && true;
            changes = true;
        }
    }
    updatePlayerAdd(currentPlayers,players);
}

template<Uint nbMaxSimul, template<class> class MemoryPlayer, class PlayMC>
void AIPlayMonteCarlo<nbMaxSimul, MemoryPlayer, PlayMC>::computeNumberOfCardReceivable(std::list<Cards_Basic>& res,
                                                             const std::list<Cards_Basic>& ungivenCards,
															 PlayerMiniMonteCarlo * player)
{
    res.clear();
	_printf("Player %d can receive", player->ID().ToInt());
    for(const Cards_Basic& card : ungivenCards )
    {
		if (player->CanReceiveCard(card))
		{
			_printf("[c:%d;h:%d] ", card.GetColour().ToInt(), card.GetHeight().ToInt());
			res.push_back(card);
		}
    }
	_printf("\n");
}

template<Uint nbMaxSimul, template<class> class MemoryPlayer, class PlayMC>
void AIPlayMonteCarlo<nbMaxSimul, MemoryPlayer, PlayMC>::removeConstraint(std::array<PlayerMiniMonteCarlo, 4 > & currentPlayers,
                                                const BasicMonteCarloAction& constraint)
{
    currentPlayers[constraint.ID().ToInt()].RemoveConstraint(constraint.Card());
}

template<Uint nbMaxSimul, template<class> class MemoryPlayer, class PlayMC>
void AIPlayMonteCarlo<nbMaxSimul, MemoryPlayer, PlayMC>::addConstraint(std::array<PlayerMiniMonteCarlo, 4 > & currentPlayers,
                                             const Player_ID & id,
                                             const Cards_Basic& card)
{
     currentPlayers[id.ToInt()].AddConstraint(card);
}

template<Uint nbMaxSimul, template<class> class MemoryPlayer, class PlayMC>
void AIPlayMonteCarlo<nbMaxSimul, MemoryPlayer, PlayMC>::removeCard(const std::list<BasicMonteCarloAction>& consequences,
                                          std::list<Cards_Basic>& currentCard) //update _unfallencards
{
    for(const auto& action : consequences)
    {
        currentCard.remove(action.Card());
    }
}

template<Uint nbMaxSimul, template<class> class MemoryPlayer, class PlayMC>
void AIPlayMonteCarlo<nbMaxSimul, MemoryPlayer, PlayMC>::updatePlayer(PlayerMiniMonteCarlo * pPlayer,
	std::list<PlayerMiniMonteCarlo  * >& players) //update _unfallencards
{
    if(!pPlayer->CanReceiveAnotherCard())
    {
        for(auto pp = players.begin(); pp != players.end(); ++pp)
        {
            if((*pp)->ID() == pPlayer->ID())
            {
                    pp = players.erase(pp);
                    break;
            }
        }
    }
}


template<Uint nbMaxSimul, template<class> class MemoryPlayer, class PlayMC>
void AIPlayMonteCarlo<nbMaxSimul, MemoryPlayer, PlayMC>::playerReceiver(std::array<PlayerMiniMonteCarlo, 4 > & currentPlayers,
	std::list<PlayerMiniMonteCarlo * >& temp,
	const std::list<PlayerMiniMonteCarlo * >& players,
                                              const Cards_Basic& card)
{
    temp.clear();
    for(Uint i = 0; i < 4 ;++i)
    {
        if(currentPlayers[i].CanReceiveCard(card)) temp.push_back(&currentPlayers[i]);
    }
}

template<Uint nbMaxSimul, template<class> class MemoryPlayer, class PlayMC>
Uint AIPlayMonteCarlo<nbMaxSimul, MemoryPlayer, PlayMC>::computeNbPlayerAbleToReceive(const MemoryPlayer<Cards*>& playerMemory, const Cards_Basic& card, const TrickStatus& trickStatus)
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
