#ifndef BASICMONTECARLOACTION_H
#define BASICMONTECARLOACTION_H

/**
** During the simulating of the games in AIPlayMonteCarlo,
** "actions" (eg giving a specific card to a specific player)
** are done.
** This class define these kind of actions.
**/

#include "Definitions.h"
#include "Cards_Basic.h"
#include "PLAYER_ID.h"

class BasicMonteCarloAction
{
    protected:
        Player_ID _id;
        Cards_Basic _card;
    public:
        BasicMonteCarloAction():_id(UNKNOWN),_card(Card_Height(UNINTIALIZED),Card_Color(NOT_CHOSEN)){}
        BasicMonteCarloAction(const Player_ID & id, const Cards_Basic & card):_id(id),_card(card){}
        virtual ~BasicMonteCarloAction(){}
        const Player_ID & ID() const {return _id;}
        const Cards_Basic & Card() const {return _card;}
    protected:
    private:
};

#endif // BASICMONTECARLOACTION_H
