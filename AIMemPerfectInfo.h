#ifndef AIMEMPERFECTINFO_H
#define AIMEMPERFECTINFO_H

#include <array>

#include "Definitions.h"
#include "AIGameMemoryImproved.h"

class AIMemPerfectInfo : public AIGameMemoryImproved
{
    public:
        AIMemPerfectInfo();
        virtual ~AIMemPerfectInfo();
        void SetPlayer(const Player_ID& player){_posPlayer = player;}
        void SetGame(const std::array< std::pair<Player_ID,std::list<Cards_Basic> > ,4 >& allHands);

        //TO DO : deals with the player dependant methods
        //eg relative to :

    protected:
    private:
};

#endif // AIMEMPERFECTINFO_H
