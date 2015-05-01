#ifndef MONTECARLOACTION_H_INCLUDED
#define MONTECARLOACTION_H_INCLUDED

/**
** This class is particularly helpful in the AIPlayMonteCarlo
** context. It helps this previous class to remember which card
** have been given to whom, and the consequences that followed.
** By consequences, I means cards that need to be absolutely given
** to a specific player. This can append for 2 reasons :
** 1. only this specific player can receive this card
** 2. the player concerned can receive exactly the number
** of cards that can be given to him.
** See "BasicMonteCarloAction", and "AIPlayMonteCarlo"
** for more informations.
** This class only store the information, it does nothing with it.
**/

#include <list>

#include "Definitions.h"
#include "BasicMonteCarloAction.h"
class MonteCarloAction
{
    protected :
        BasicMonteCarloAction _action;
        std::list<BasicMonteCarloAction> _consequences;
    public :
        MonteCarloAction(const BasicMonteCarloAction& action):_action(action){_consequences.clear();}
        MonteCarloAction(){}
        ~MonteCarloAction(){}
        BasicMonteCarloAction & Action() {return _action;}
        std::list<BasicMonteCarloAction>& Consequences() {return _consequences;}
};

#endif // MONTECARLOACTION_H_INCLUDED
