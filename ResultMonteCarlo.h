#ifndef RESULTMONTECARLO_H
#define RESULTMONTECARLO_H

/**
** Store the result of the simulation of each
** Monte Carlo trajectory.
** This class is also charged to do the aggregation at
** the end of the game.
**/
#include<unordered_map>
#include<list>
#include<algorithm>

#include "Definitions.h"
#include "Cards_Basic.h"

//Debugging
#include "DebugwithPrint.h"
#define RESULTMONTECARLO_DEBUG 0

class ResultMonteCarlo
{
    protected :
        const Uint _nbMaxSimul;
        const Uint _nbCardUnfallen;
        std::vector< std::vector<Uint> > _score; //[card][traj]
        std::unordered_map<Cards_Basic,Uint> _refCardsInt;

        WrapperPrint<RESULTMONTECARLO_DEBUG> _printf;

    public:
        ResultMonteCarlo():_nbMaxSimul(100),_nbCardUnfallen(0),_refCardsInt(){}
        ResultMonteCarlo(Uint nbMaxSimul,const std::list<Cards_Basic>& playableCards);
        virtual ~ResultMonteCarlo(){}
        void Put(Uint idTraj,const Cards_Basic& cardsPlayed,Uint scoreTraj);
        Uint ScoreAggr(const Cards_Basic& cardsPlayed); //default behavior : average the result
    protected:
    private:
};
#endif // RESULTMONTECARLO_H
