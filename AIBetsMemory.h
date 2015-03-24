#ifndef AIBETSMEMORY_H
#define AIBETSMEMORY_H

#include "Definitions.h"
#include "Basic_Game_Info.h"

/**
This class handles the memory of the AI players. It will store (hopefully in an efficient manner)
all the informations a player need to take the right decision.
This class worries only about the memory of the bidding phase.
**/
class AIBetsMemory
{
    protected :
        bool _toBeRemoved;
    public:
        AIBetsMemory();
        virtual ~AIBetsMemory();
    protected:
    private:
};

#endif // AIBETSMEMORY_H
