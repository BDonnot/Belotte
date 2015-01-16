#ifndef AIGAMEMEMORY_H
#define AIGAMEMEMORY_H

#include "Definitions.h"
#include "Basic_Game_Info.h"

#include "Cards.h"
/**
This class handles the memory of the AI players. It will store (hopefully in an efficient manner)
all the informations a player need to take the right decision.
This class worries only about the playing phase of the game.
**/

class MemorizeCards //just an efficient way to store all the fallen cards.
{
    protected :
        bool _allCards[32];
    public :
        MemorizeCards(){}
        void Reset()
        {
            for (Uint i = 0; i < 32; i++)
            {
                _allCards[i] = false;
            }
        }
        const bool & IsFallen(CARDS_COLOR color, CARDS_HEIGHT height) const
        {
            return _allCards[color*4 + height];
        }
        void SetFallen(CARDS_COLOR color, CARDS_HEIGHT height)
        {
            _allCards[color*4 + height] = true;
        }
};

class AIGameMemory
{
    protected :
        MemorizeCards _fallenCards;
    public:
        AIGameMemory();
        virtual ~AIGameMemory();
    protected:
    private:
};

#endif // AIGAMEMEMORY_H
