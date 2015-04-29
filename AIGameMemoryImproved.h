#ifndef AIGAMEMEMORYIMPROVED_H
#define AIGAMEMEMORYIMPROVED_H

#include "Definitions.h"
#include "AIGameMemory.h"

template<class TypeOfCard>
class AIGameMemoryImproved : public AIGameMemory<TypeOfCard>
{
    protected :
        MemorizePlayersCards _canPlayersHaveCard;
    public:
		AIGameMemoryImproved() :AIGameMemory<TypeOfCard>(Player_ID(GHOST), nullptr){}
		AIGameMemoryImproved(const Player_ID& posPlayer, std::list<TypeOfCard>* pHand) : AIGameMemory<TypeOfCard>(posPlayer, pHand){}
        virtual ~AIGameMemoryImproved(){}
        virtual bool SetCannotHaveCard(const Player_ID& player,const Card_Color& col, const Card_Height& height)
        {
            _canPlayersHaveCard.SetCannotHaveCard(player,col,height);
            return true;
        }
    protected:
        virtual void updateSmarter(const TrickBasic_Memory& trick, const Position_Trick& posTrick);
        virtual bool canHave(const Player_ID& player,const Card_Color& col, const Card_Height& height) const
        {
            return _canPlayersHaveCard.CanHaveCard(player,col,height);
        }
        void dealWithTrumps(Uint i,const TrickBasic_Memory& trick,const Player_ID& currentPlayer,const Card_Color& trumpColor );

        virtual void initHeritage(){_canPlayersHaveCard.Reset();}
        virtual bool stillHaveCards(const Player_ID& player,const Card_Color& color) const;

    private:
};

template<class TypeOfCard>
void AIGameMemoryImproved<TypeOfCard>::updateSmarter(const TrickBasic_Memory& trick, const Position_Trick& posTrick)
{
	Card_Color colorAsked = trick.ColorAsked();
	Card_Color trumpColor = this->_infos.TrumpColor();
	Player_ID currentPlayer = this->_infos.FirstToPlay(posTrick, this->_posPlayer); //TO DO : move trick basic memory
	Card_Color tempCol;
	Card_Height tempHeig;
	Cards_Basic tempCard;
	for (Uint i = 0; i < 4; ++i)
	{
		tempCol = trick[i]->GetColour();
		tempHeig = trick[i]->GetHeight();
		this->_canPlayersHaveCard.SetFallen(*trick[i]);
		if (tempCol == trumpColor)
		{
			dealWithTrumps(i, trick, currentPlayer, trumpColor);
		}
		if (tempCol != colorAsked)
		{
			this->_canPlayersHaveCard.SetCutColor(currentPlayer, colorAsked);
		}
		currentPlayer.Next();
	}

}

template<class TypeOfCard>
void AIGameMemoryImproved<TypeOfCard>::dealWithTrumps(Uint iPlayer,
                                                      const TrickBasic_Memory& trick,
                                                      const Player_ID& currentPlayer,
                                                      const Card_Color& trumpColor)
{
    if(iPlayer == 0) return;

	Card_Color tempCol;
	Card_Height tempHei;
	Card_Height maxHTrumpPlay(SEVEN); //the height of the greatest trump played before currentPlayer played
	for (Uint j = 0; j+1 < iPlayer; ++j)
	{
		tempCol = trick[j]->GetColour();
		tempHei = trick[j]->GetHeight();
		if (tempCol == trumpColor && trick[j]->Win(maxHTrumpPlay))
		{
		    maxHTrumpPlay = tempHei;
		}
	}
    /**
    **If the player have not go up at trump
    **it is because he has not higher !
    **/
    if (!trick[iPlayer]->Win(maxHTrumpPlay))
    {
        Cards_Basic maxTrump(maxHTrumpPlay,trumpColor);
        for (Uint icolTrump = 0; icolTrump < 8; ++icolTrump)
        {
            tempHei = Card_Height(icolTrump);
            if (!maxTrump.Win(tempHei))
            {
                _canPlayersHaveCard.SetCannotHaveCard(currentPlayer, trumpColor, tempHei);
            }
        }
    }
}

template<class TypeOfCard>
bool AIGameMemoryImproved<TypeOfCard>::stillHaveCards(const Player_ID& player, const Card_Color& color) const
{
	bool res = true;
	Card_Height tempH;
	for (Uint iH = 0; iH < 8; ++iH)
	{
		tempH = Card_Height(iH);
		res = res || _canPlayersHaveCard.CanHaveCard(player, color, tempH);
	}
	return res;
}
#endif // AIGAMEMEMORYIMPROVED_H

