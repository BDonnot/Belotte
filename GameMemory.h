#ifndef GAMEMEMORY_H
#define GAMEMEMORY_H

#include <array>
#include <string>

#include "Definitions.h"
#include "Basic_Game_Info.h"
#include "Cards.h"

/** This class is used to remember the previous trick, and the informations
revealed by the player when they play.
**/
// TO DO : unordered map instead of array, maybe... Even map, but needs changes in Cards maybe (see _fallenCards)
//This implementation does not work, implement the previous one
class GameMemory
{
    protected :
        const Basic_Game_Info _infos;
        std::array<std::array<bool,8>,4> _fallenCards; //_fallenCards[color][height] -- the second array is not sorted (no common order between trump/no trump)
        std::array<std::array<bool,4>,4> _playerStillHaveColor; //_playerStillHaveColor[player][color]
        std::array<CARDS_HEIGHT,4> _heightMaster; //_heightMaster[color] return the height master...
        std::array<CARDS_HEIGHT,8> _orderTrump;
        std::array<CARDS_HEIGHT,8> _orderNoTrump;
    public:
        GameMemory();
        virtual ~GameMemory(){}
        //void TellTrump();
        void TellTrick(const std::array<Cards*,4>& trick,POSITION_PLAYER firstToPlay); //tell the memory the state of the current trick
        void TellFullTrick(const std::array<Cards*,4>& trick,POSITION_PLAYER firstToPlay); //tell the memory the state of the current trick, at the end of the trick
        void Reset(); //need to be reseted at the ends of the bidding, otherwise _heightMaster will be wrong
    protected:
        void updateMemory(Cards* currentCard,CARDS_COLOR colorFirst,POSITION_PLAYER playerPlayed);
    private:
};

#endif // GAMEMEMORY_H
