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
        bool _has_trump; //does the player have trump
        Position_Trick _myPosition; //Player's position in the trick
        Player_ID _myID;
    public:
        TrickStatus(const Player_ID& myID):_myID(myID){}
        virtual ~TrickStatus(){}
        void Update(const TrickBasic_Memory&  trick
                    ,bool hasCol
                    ,bool hasTrump);
        bool HasCol() const { return _has_col; }
        bool HasTrump() const { return _has_trump; }
        const Position_Trick& Position() const { return _myPosition; }
        const Player_ID& ID() const { return _myID; }
        const Card_Color& TrumpColor() const {return _trumpColor; }

    private:
};

#endif // TRICKSTATUS_H
