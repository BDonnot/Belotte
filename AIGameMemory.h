#ifndef AIGAMEMEMORY_H
#define AIGAMEMEMORY_H
#include <array>
#include <list>
#include "Definitions.h"
#include "Basic_Game_Info.h"

#include "Cards.h"
/**
This class handles the memory of the AI players. It will store (hopefully in an efficient manner)
all the informations a player need to take the right decision.
This class worries only about the playing phase of the game.
**/
template<Uint n>
class BoolStorage
{
    protected :
        bool _information[n];
    public :
        BoolStorage(){Reset();}
        void Reset()
        {
            for(Uint i = 0; i < n ; i++)
            {
                _information[i] = false;
            }
        }
    protected :
        const bool& getInfo(Uint i) const
        {
            return _information[i];
        }
        void setInformation(Uint index)
        {
            _information[index] = true;
        }
};

class MemorizeCards : public BoolStorage<32> //just an efficient way to store all the fallen cards.
{
    public :
        MemorizeCards(){}
        const bool & IsFallen(CARDS_COLOR color, CARDS_HEIGHT height) const
        {
            return getInfo(color*8 + height);
        }
        void SetFallen(CARDS_COLOR color, CARDS_HEIGHT height)
        {
            setInformation(color*8 + height);
        }
        Uint ComputeFallen(CARDS_COLOR color) //compute the number of cards fallen in a specific color
        {
            Uint res = 0;
            for(Uint j = 0; j < 8; j++)
            {
                if(_information[color*8+j]) ++res;
            }
            return res;
        }
};

class MemorizeCuts  : public BoolStorage<16> //efficient way to memorize which player cut where
{
    public :
        MemorizeCuts(){}
        const bool & Cut(POSITION_PLAYER player, CARDS_COLOR color) const
        {
            return getInfo(player*4 + color);
        }
        void SetCut(POSITION_PLAYER player, CARDS_COLOR color)
        {
           setInformation(player*4 + color);
        }
};

class AIGameMemory
{
    protected :
        std::list<Cards*>* _pHand;
        Basic_Game_Info _infos;
        MemorizeCards _fallenCards;
        MemorizeCuts _playerCut;
        CARDS_HEIGHT _heightsMaster[4]; //key : color, stock the height of the cards master in the color
        POSITION_PLAYER _posPlayer;

    public:
        AIGameMemory(POSITION_PLAYER posPlayer,std::list<Cards*>* pHand):
        _pHand(pHand),
        _posPlayer(posPlayer)
        {
            InitEverything();
        }
        virtual ~AIGameMemory();
        void UpdateFullTrick(const std::array<Cards*,4>& trick, POSITION_TRICK posTrick); //posTrick : the position of the player in the trick
        void InitEverything(); //call after the trump have been chosen, to set everything :-)
    protected:
        CARDS_HEIGHT heightUnder(CARDS_HEIGHT height,bool color);
        void computeNewHeightMaster(); //also update _playerCut
        void updatePlayerCutSmarter(POSITION_PLAYER firstToPlay); //implemented : if I am the only one to have a color, the other cut ...

    private:
};

#endif // AIGAMEMEMORY_H
