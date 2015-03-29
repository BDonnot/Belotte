#ifndef AIGAMEMEMORY_H
#define AIGAMEMEMORY_H
#include <array>
#include <list>
#include "Definitions.h"
#include "Basic_Game_Info.h"

#include "Cards.h"
#include "TrickBasic_Memory.h"
/**
This class handles the memory of the AI players. It will store (hopefully in an efficient manner)
all the informations a player need to take the right decision.
This class worries only about the playing phase of the game.
**/
template<Uint n>
class BoolStorage
{
    protected :
        Basic_Game_Info _infos;
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
            Uint iColor = _infos.ColorToInt(color);
            Uint iHeight = _infos.HeightToInt(height);
            return getInfo(iColor*8 + iHeight);
        }
        void SetFallen(CARDS_COLOR color, CARDS_HEIGHT height)
        {
            Uint iColor = _infos.ColorToInt(color);
            Uint iHeight = _infos.HeightToInt(height);
            setInformation(iColor*8 + iHeight);
        }
        Uint ComputeFallen(CARDS_COLOR color) const //compute the number of cards fallen in a specific color
        {
            Uint iColor = _infos.ColorToInt(color);
            Uint res = 0;
            for(Uint j = 0; j < 8; j++)
            {
                if(_information[iColor*8+j]) ++res;
            }
            return res;
        }
};

class MemorizeCutsCalls  : public BoolStorage<16> //efficient way to memorize which player cut/called where
{
    public :
        MemorizeCutsCalls(){}
        const bool & Cut(PLAYER_ID player, CARDS_COLOR color) const
        {
            Uint iPlayer = _infos.PosPlayerToInt(player);
            Uint iColor = _infos.ColorToInt(color);
            return getInfo(iPlayer*4 + iColor);
        }
        void SetCut(PLAYER_ID player, CARDS_COLOR color)
        {
            Uint iPlayer = _infos.PosPlayerToInt(player);
            Uint iColor = _infos.ColorToInt(color);
            setInformation(iPlayer*4 + iColor);
        }
};
//greatest(color) : the gratest card I have
//smallest(color)
//score longe(color) (init at the beginning)
//protect_point(color) (do i have points that i need to protect : TEN not master without ACE for example)
//_nb_resting_cards
//is_master
//_nb_resting_cards
//got_ten
class AIGameMemory
{
    protected :
        Basic_Game_Info _infos;
        PLAYER_ID _posPlayer;
        std::list<Cards* >* _pHand;
        MemorizeCards _fallenCards; //

        MemorizeCutsCalls _playerCut; //
        MemorizeCutsCalls _playerCalls; //
        Uint _nbColorPlayed[4]; //key : color ; number of times each color have been played first

        CARDS_HEIGHT _heightsMaster[4]; //key : color, stock the height of the cards master in the color

        CARDS_HEIGHT _greatest[4];  //key : color -> stock the height of the greatest cards I have per color
        CARDS_HEIGHT _smallest[4];  //key : color -> stock the height of the smallest cards I have per color
        Uint _nbRemaining[4]; //key : color ; number of cards remaining in my hand per color
        Uint _longe[4]; //key : color ; score for the longe (number of cards in the color)
        bool _IAmMaster[4]; //key : color ; am i the master in the color
        bool _HaveTen[4]; //key : color ; have I the TEN/NINE (at trump) in the color
        bool _protectPoints[4]; //key : color; do I have to protect some important point (TEN, NINE for example) at the color


    public:
        AIGameMemory(PLAYER_ID posPlayer,std::list<Cards*>* pHand):
            _posPlayer(posPlayer),
            _pHand(pHand)
            {
                InitEverything();
            }
        virtual ~AIGameMemory();
        void UpdateFullTrick(const TrickBasic_Memory& trick, POSITION_TRICK posTrick); //posTrick : the position of the player in the trick
        void InitEverything(); //call after the trump have been chosen, to set everything :-)
        CARDS_HEIGHT Master(CARDS_COLOR color) const;

        bool Cut(PLAYER_ID player,CARDS_COLOR color) const; //see *CallCut for more information
        bool OpponentsCut(CARDS_COLOR color) const;
        bool TeammateCut(CARDS_COLOR color) const;
        bool NextCut(CARDS_COLOR color) const; //see *CallCut for more information
        bool Call(PLAYER_ID player,CARDS_COLOR color) const;
        bool OpponentsCall(CARDS_COLOR color) const;
        bool TeammateCall(CARDS_COLOR color) const;
        bool NextCall(CARDS_COLOR color) const;//see *CallCut for more information

        Uint NbFallen(CARDS_COLOR color) const; //number of cards fallen in a specific color
        Uint NbColorPlayed(CARDS_COLOR color) const; //number of time each color have been played (first)
        Uint ScoreLonge(CARDS_COLOR color) const;

        bool AllRemainingCards(CARDS_COLOR color) const; //do I have all the remaining cards in the color
        bool AmIMaster(CARDS_COLOR color) const; //Have I the strongest card in the color
        bool DoMyTeamTook() const;
        bool CardsFallen(CARDS_COLOR color,CARDS_HEIGHT height) const;
        bool HaveTen(CARDS_COLOR color) const;
        bool ProtectPoints(CARDS_COLOR color) const;

        CARDS_HEIGHT Greatest(CARDS_COLOR color) const; //return the greatest cards I have in the color
        CARDS_HEIGHT Smallest(CARDS_COLOR color) const; //return the smallest cards I have in the color

    protected:
        CARDS_HEIGHT heightUnder(CARDS_HEIGHT height,bool color);
        void computeNewHeightMaster(); //also update _playerCut
        void updateEverythingElse(PLAYER_ID firstToPlay); //update everything that need to go through the hand of the player.

        bool callCut(PLAYER_ID player,CARDS_COLOR color,const MemorizeCutsCalls& StoreCallCut) const; //dot the 'player' call/cut at the 'color'
        bool opponentsCallCut(CARDS_COLOR color,const MemorizeCutsCalls& StoreCallCut) const; //do my opponents call/cut at the 'color'
        bool teammateCallCut(CARDS_COLOR color,const MemorizeCutsCalls& StoreCallCut) const; //do my  teammate call/cut at the 'color'
        bool nextCallCut(CARDS_COLOR color,const MemorizeCutsCalls& StoreCallCut) const; //do the player who play after me call/cut at the 'color'
        void computeScoreLongeAndProtectPoint(std::list<const Cards*>& cardsInTheColor,Uint& scoreLonge, bool& protectPoints); //always per color
    private:
};

#endif // AIGAMEMEMORY_H
