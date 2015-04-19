#ifndef RESULTMONTECARLO_H
#define RESULTMONTECARLO_H

#include<unordered_map>
#include<list>
#include<algorithm>

#include "Definitions.h"
#include "Cards_Basic.h"

//template<Uint nbMaxSimul, Uint nbCardUnfallen>
class ResultMonteCarlo
{
    protected :
        const Uint _nbMaxSimul;
        const Uint _nbCardUnfallen;
        std::vector< std::vector<Uint> > _score; //[card][traj]
        std::unordered_map<Cards_Basic,Uint> _refCardsInt;

    public:
        ResultMonteCarlo():_nbMaxSimul(100),_nbCardUnfallen(0),_refCardsInt(){}
        ResultMonteCarlo(Uint nbMaxSimul,const std::list<Cards_Basic>& playableCards);
        virtual ~ResultMonteCarlo(){}
        void Put(Uint idTraj,const Cards_Basic& cardsPlayed,Uint scoreTraj);
        Uint ScoreAggr(const Cards_Basic& cardsPlayed);
    protected:
    private:
};
/*
template<Uint nbMaxSimul, Uint nbCardUnfallen>
ResultMonteCarlo<nbMaxSimul,nbCardUnfallen>::ResultMonteCarlo(const std::list<Cards_Basic>&  playableCards):
_nbMaxSimul(nbMaxSimul),
_nbCardUnfallen(nbCardUnfallen)
{
    Uint refCard = 0;
    for(const Cards_Basic& card : playableCards)
    {
        _refCardsInt[card] = refCard;
        refCard++;
    }
}

template<Uint nbMaxSimul, Uint nbCardUnfallen>
void ResultMonteCarlo<nbMaxSimul,nbCardUnfallen>::Put(Uint idTraj,const Cards_Basic& cardsPlayed,Uint scoreTraj)
{
    _score[_refCardsInt[cardsPlayed]][idTraj] = scoreTraj;
}

template<Uint nbMaxSimul, Uint nbCardUnfallen>
Uint ResultMonteCarlo<nbMaxSimul,nbCardUnfallen>::ScoreAggr(const Cards_Basic& cardsPlayed)
{
    std::vector<Uint> scoresCard(_score[_refCardsInt[cardsPlayed]],_score[_refCardsInt[cardsPlayed]]+nbMaxSimul);
    std::sort(scoresCard.begin(),scoresCard.end());
    double res = 0;
    auto itLast = scoresCard.end() - 5;
    for(auto it = scoresCard.begin()+5; it != itLast; ++it )
    {
        res += *it;
    }
    return res/static_cast<double>(nbMaxSimul - 10);
}
*/
#endif // RESULTMONTECARLO_H
