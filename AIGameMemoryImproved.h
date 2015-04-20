#ifndef AIGAMEMEMORYIMPROVED_H
#define AIGAMEMEMORYIMPROVED_H

#include "Definitions.h"
#include "AIGameMemory.h"

class MemorizePlayersCards  : public BoolStorage<128>
{
    /**
    **Beware, the order is reverse from bool storage :
    ** I can receive a card <=> BoolStorage[index] = false !!!
    **/
    public :
        MemorizePlayersCards(){}
        ~MemorizePlayersCards(){}
        bool CanHaveCard(const Player_ID& player,const Card_Color& color,const Card_Height& height) const
        {
            Uint iPlayer = player.ToInt();
            Uint iColor = color.ToInt();
            Uint iHei = height.ToInt();
            return !getInfo(iPlayer*32 + iColor*8+ iHei);
        }
        void SetCanHaveCard(const Player_ID& player,const Card_Color& color,const Card_Height& height)
        {
            Uint iPlayer = player.ToInt();
            Uint iColor = color.ToInt();
            Uint iHei = height.ToInt();
            setInformation(iPlayer*32 + iColor*8+ iHei);
        }
        void SetFallen(const Cards & card)
        {
            /**
            **When a card is fallen no one can have it
            **/
            Uint iColor = card.GetColour().ToInt();
            Uint iHei = card.GetHeight().ToInt();
            for(Uint iPlayer = 0; iPlayer < 4; ++iPlayer)
            {
                setInformation(iPlayer*32 + iColor*8+ iHei);
            }
        }
        void SetCutColor(const Player_ID& player,const Card_Color& color)
        {
            /**
            **When a player cut at one color, it means he does not have this color anymore
            **/
            Uint iPlayer = player.ToInt();
            Uint iColor = color.ToInt();
            for(Uint iHei = 0; iHei < 8; ++iHei)
            {
                setInformation(iPlayer*32 + iColor*8+ iHei);
            }
        }
};

class AIGameMemoryImproved : public AIGameMemory
{
    MemorizePlayersCards _canPlayersHaveCard;
    public:
        AIGameMemoryImproved(const Player_ID& posPlayer,std::list<Cards*>* pHand):AIGameMemory(posPlayer,pHand){}
        virtual ~AIGameMemoryImproved(){}
        virtual bool SetCanHaveCard(const Player_ID& player,const Card_Color& col, const Card_Height& height)
        {
            _canPlayersHaveCard.SetCanHaveCard(player,col,height);
            return true;
        }
    protected:
        virtual void updateSmarter(const TrickBasic_Memory& trick, const Position_Trick& posTrick);
        virtual bool canHave(const Player_ID& player,const Card_Color& col, const Card_Height& height) const
        {
            return _canPlayersHaveCard.CanHaveCard(player,col,height);
        }
        void dealWithTrumps(Uint i,const TrickBasic_Memory& trick,const Player_ID& currentPlayer,const Card_Color& trumpColor );

    private:
};

#endif // AIGAMEMEMORYIMPROVED_H
