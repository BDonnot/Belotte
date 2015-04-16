#ifndef RESULTMONTECARLO_H
#define RESULTMONTECARLO_H

#include "Definitions.h"
#include "Cards_Basic.h"

class ResultMonteCarlo
{
    protected :
        const Uint _nbMaxSimul;

    public:
        ResultMonteCarlo():_nbMaxSimul(1){}
        ResultMonteCarlo(Uint nbMaxSimulations):_nbMaxSimul(nbMaxSimulations){}
        virtual ~ResultMonteCarlo(){}
        void Put(Uint idTraj,const Cards_Basic* cardsPlayed,Uint scoreTraj);
        Uint ScoreAggr(const Cards_Basic* cardsPlayed); //Aggregate the score of each card (TO DO : template here ? )
    protected:
    private:
};

#endif // RESULTMONTECARLO_H
