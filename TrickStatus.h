#ifndef TRICKSTATUS_H
#define TRICKSTATUS_H

#include "Definitions.h"
#include "Cards.h"
#include "TrickBasic_Memory.h"
/** This class aims to store all the useful informations
of the current trick for the player.
**/
class TrickStatus
{
    protected:
        Basic_Game_Info _infos;
        Card_Color _trumpColor;
        bool _has_col; //does the player have the color asked
        bool _has_trump; //does the plaeyer have trump
        Position_Trick _myPosition; //Player's position in the trick
    public:
        TrickStatus(){}
        virtual ~TrickStatus(){}
        void Update(const TrickBasic_Memory&  trick
                    ,bool hasCol
                    ,bool hasTrump);
        bool HasCol() const { return _has_col; }
        bool HasTrump() const { return _has_trump; }
        Position_Trick Position() const { return _myPosition; }
        const Card_Color& TrumpColor() const {return _trumpColor; }

    private:
};

#endif // TRICKSTATUS_H
