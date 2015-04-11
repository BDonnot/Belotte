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
        const bool & IsFallen(Card_Color color, Card_Height height) const
        {
            Uint iColor = color.ToInt();
            Uint iHeight = height.ToInt();
            return getInfo(iColor*8 + iHeight);
        }
        void SetFallen(Card_Color color, Card_Height height)
        {
            Uint iColor = color.ToInt();
            Uint iHeight = height.ToInt();
            setInformation(iColor*8 + iHeight);
        }
        Uint ComputeFallen(Card_Color color) const //compute the number of cards fallen in a specific color
        {
            Uint iColor = color.ToInt();
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
        const bool & Cut(const Player_ID& player, Card_Color color) const
        {
            Uint iPlayer = player.ToInt();
            Uint iColor = color.ToInt();
            return getInfo(iPlayer*4 + iColor);
        }
        void SetCut(const Player_ID& player, Card_Color color)
        {
            Uint iPlayer = player.ToInt();
            Uint iColor = color.ToInt();
            setInformation(iPlayer*4 + iColor);
        }
};

class AIGameMemory
{
    protected :
        Basic_Game_Info _infos;
        Player_ID _posPlayer;
        std::list<Cards* >* _pHand;
        MemorizeCards _fallenCards; //

        MemorizeCutsCalls _playerCut; //
        MemorizeCutsCalls _playerCalls; //
        Uint _nbColorPlayed[4]; //key : color ; number of times each color have been played first

        Card_Height _heightsMaster[4]; //key : color, stock the height of the cards master in the color

        Card_Height _greatest[4];  //key : color -> stock the height of the greatest cards I have per color
        Card_Height _smallest[4];  //key : color -> stock the height of the smallest cards I have per color
        Uint _nbRemaining[4]; //key : color ; number of cards remaining in my hand per color
        Uint _longe[4]; //key : color ; score for the longe (number of cards in the color)
        bool _IAmMaster[4]; //key : color ; am i the master in the color
        bool _HaveTen[4]; //key : color ; have I the TEN/NINE (at trump) in the color
        bool _protectPoints[4]; //key : color; do I have to protect some important point (TEN, NINE for example) at the color


    public:
        AIGameMemory(const Player_ID& posPlayer,std::list<Cards*>* pHand):
            _posPlayer(posPlayer),
            _pHand(pHand)
            {
                InitEverything();
            }
        virtual ~AIGameMemory();
        void UpdateFullTrick(const TrickBasic_Memory& trick,const Position_Trick& posTrick); //posTrick : the position of the player in the trick
        void InitEverything(); //call after the trump have been chosen, to set everything :-)
        Card_Height Master(const Card_Color& color) const;

        bool Cut(const Player_ID& player,const Card_Color& color) const; //see *CallCut for more information
        bool OpponentsCut(const Card_Color& color) const;
        bool TeammateCut(const Card_Color& color) const;
        bool NextCut(const Card_Color& color) const; //see *CallCut for more information
        bool Call(const Player_ID& player,const Card_Color& color) const;
        bool OpponentsCall(const Card_Color& color) const;
        bool TeammateCall(const Card_Color& color) const;
        bool NextCall(const Card_Color& color) const;//see *CallCut for more information

        Uint NbFallen(const Card_Color& color) const; //number of cards fallen in a specific color
        Uint NbColorPlayed(const Card_Color& color) const; //number of time each color have been played (first)
        Uint ScoreLonge(const Card_Color& color) const;

        bool AllRemainingCards(const Card_Color& color) const; //do I have all the remaining cards in the color
        bool AmIMaster(const Card_Color& color) const; //Have I the strongest card in the color
        bool DoMyTeamTook() const;
        bool CardsFallen(const Card_Color& color,const Card_Height& height) const;
        bool HaveTen(const Card_Color& color) const;
        bool ProtectPoints(const Card_Color& color) const;

        Card_Height Greatest(const Card_Color& color) const; //return the greatest cards I have in the color
        Card_Height Smallest(const Card_Color& color) const; //return the smallest cards I have in the color

    protected:
        //Card_Height heightUnder(const Card_Height& height,bool color);
        void computeNewHeightMaster(); //also update _playerCut
        void updateEverythingElse(const Player_ID& firstToPlay); //update everything that need to go through the hand of the player.

        bool callCut(const Player_ID& player,const Card_Color& color,const MemorizeCutsCalls& StoreCallCut) const; //dot the 'player' call/cut at the 'color'
        bool opponentsCallCut(const Card_Color& color,const MemorizeCutsCalls& StoreCallCut) const; //do my opponents call/cut at the 'color'
        bool teammateCallCut(const Card_Color& color,const MemorizeCutsCalls& StoreCallCut) const; //do my  teammate call/cut at the 'color'
        bool nextCallCut(const Card_Color& color,const MemorizeCutsCalls& StoreCallCut) const; //do the player who play after me call/cut at the 'color'
        void computeScoreLongeAndProtectPoint(std::list<const Cards*>& cardsInTheColor,Uint& scoreLonge, bool& protectPoints,bool iAmMasterColor,const Card_Height& greatest); //always per color
    private:
};

#endif // AIGAMEMEMORY_H
