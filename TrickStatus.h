#ifndef TRICKSTATUS_H
#define TRICKSTATUS_H

#include "Definitions.h"
#include "Cards.h"
/** This class aims to store all the useful informations
of the current trick for the player.
**/
class TrickStatus
{
    protected:
        Basic_Game_Info _infos;
        CARDS_COLOR _trumpColor;
        CARDS_COLOR _colour_asked; //the color asked
        bool _has_col; //does the player have the color asked
        bool _has_trump; //does the plaeyer have trump
        POSITION_TRICK _myPosition; //Player's position in the trick
        POSITION_TRICK _winner; //winner's position (in the trick)
        CARDS_HEIGHT _maxHeight; //height of the winning card
        CARDS_COLOR _maxColor; //color of the winning cards
    public:
        TrickStatus(){}
        virtual ~TrickStatus(){}
        void Update(const std::array<Cards*,4>& trick
                    ,bool hasCol
                    ,bool hasTrump);


        CARDS_COLOR ColourAsked() const
        {
            return _colour_asked;
        }
        bool HasCol() const
        {
            return _has_col;
        }
        bool HasTrump() const
        {
            return _has_trump;
        }
        POSITION_TRICK Position() const
        {
            return _myPosition;
        }
        POSITION_TRICK Winner() const
        {
            return _winner;
        }
        CARDS_HEIGHT MaxHeight() const
        {
            return _maxHeight;
        }
        CARDS_COLOR MaxColor() const
        {
            return _maxColor;
        }

    private:
};

#endif // TRICKSTATUS_H
