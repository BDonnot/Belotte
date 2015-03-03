#ifndef AIGAMEMEMORY_H
#define AIGAMEMEMORY_H
#include <array>
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
        Basic_Game_Info _infos;
        MemorizeCards _fallenCards;
        MemorizeCuts _playerCut;
        CARDS_HEIGHT _heightsMaster[4]; //key : color, stock the height of the cards master in the color
        POSITION_PLAYER _posPlayer;

    public:
        AIGameMemory(POSITION_PLAYER posPlayer):_posPlayer(posPlayer){}
        virtual ~AIGameMemory();
        void UpdateFullTrick(const std::array<Cards*,4>& trick, POSITION_TRICK posTrick) //posTrick : the position of the player
        {
            Uint i;
            CARDS_COLOR colorAsked = trick[0]->GetColour();
            CARDS_COLOR currentColor;
            CARDS_HEIGHT currentHeight;
            POSITION_PLAYER currentPlayer = _infos.FirstToPlay(posTrick,_posPlayer);

            for(i = 0; i < 4; i++)
            {
                currentColor = trick[i]->GetColour();
                currentHeight = trick[i]->GetHeight();
                Uint intCol = _infos.ColorToInt(currentColor);
                //update the fallen Cards
                _fallenCards.SetFallen(currentColor,currentHeight);
                //update the master cards (in each color)
                if(trick[i]->Win(_heightsMaster[intCol])) _heightsMaster[intCol] = currentHeight;
            }
            //update the playerCut
            for(i= 1; i < 4; i++)
            {
                currentPlayer = _infos.Next(currentPlayer);
                currentColor = trick[i]->GetColour();
                if(currentColor != colorAsked) _playerCut.SetCut(currentPlayer,currentColor );
            }
        }
    protected:
    private:
};

#endif // AIGAMEMEMORY_H
