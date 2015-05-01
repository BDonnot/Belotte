#include "ResultMonteCarlo.h"
using namespace std;
ResultMonteCarlo::ResultMonteCarlo(Uint nbMaxSimul,const std::list<Cards_Basic>&  playableCards):
_nbMaxSimul(nbMaxSimul),
_nbCardUnfallen(playableCards.size()),
_score(_nbCardUnfallen,vector<Uint>(nbMaxSimul,0))
{
    Uint refCard = 0;
    for(const Cards_Basic& card : playableCards)
    {
        _refCardsInt.emplace(card,refCard);
        _printf("Cards [c:%d,h:%d] -> %d\n",card.GetColour().ToInt(),card.GetHeight().ToInt(),refCard);
        refCard++;
    }
}

void ResultMonteCarlo::Put(Uint idTraj,const Cards_Basic& cardPlayed,Uint scoreTraj)
{
    _printf("Storing the result for card [c:%d,h:%d]\n",cardPlayed.GetColour().ToInt(),cardPlayed.GetHeight().ToInt());
    Uint refCard = _refCardsInt[cardPlayed];
    _printf("The references is %d \n",refCard);
    _score[ refCard ][idTraj] = scoreTraj;
}

Uint ResultMonteCarlo::ScoreAggr(const Cards_Basic& cardPlayed)
{
    //Default behavior : do the average ...
    double res = 0;
    Uint refCard = _refCardsInt[cardPlayed];
    vector<Uint>& scoresCard = _score[refCard];
    sort(scoresCard.begin(),scoresCard.end());
    auto itLast = scoresCard.end();
    for(auto it = scoresCard.begin(); it != itLast; ++it )
    {
        res += static_cast<double>(*it);
    }
    return res/static_cast<double>(_nbMaxSimul - 10);
}
