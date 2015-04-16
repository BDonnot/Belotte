#ifndef MONTECARLOACTION_H_INCLUDED
#define MONTECARLOACTION_H_INCLUDED

#include <list>

#include "Definitions.h"
#include "BasicMonteCarloAction.h"
class MonteCarloAction
{
    protected :
        BasicMonteCarloAction _action;
        std::list<BasicMonteCarloAction> _consequences;
    public :
        MonteCarloAction(const BasicMonteCarloAction& action):_action(action){}
        MonteCarloAction(){}
        BasicMonteCarloAction & Action() {return _action;}
        std::list<BasicMonteCarloAction>& Consequences() {return _consequences;}
};

#endif // MONTECARLOACTION_H_INCLUDED
