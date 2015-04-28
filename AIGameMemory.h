#ifndef AIGAMEMEMORY_H
#define AIGAMEMEMORY_H

#include <array>
#include <list>

#include "Definitions.h"
#include "Basic_Game_Info.h"

#include "Cards_Basic.h"
#include "Cards.h"
#include "TrickBasic_Memory.h"
#include "MemoryHelpers.h"
#include "CardsGetMethod.h"

/**
This class handles the memory of the AI players. It will store (hopefully in an efficient manner)
all the informations a player need to take the right decision.
This class worries only about the playing phase of the game.
**/

template<class TypeOfCard>
class AIGameMemory
{
    //TO DO : template with the type of cards, because it might be good to store std::list<Cards_Basic>*
    protected :
        Basic_Game_Info _infos;
        MemorizeCards _fallenCards; //
        MemorizeCutsCalls _playerCut; //
        MemorizeCutsCalls _playerCalls; //
        Uint _nbColorPlayed[4]; //key : color ; number of times each color have been played first
        Card_Height _heightsMaster[4]; //key : color, stock the height of the cards master in the color
        Uint _nbRemaining[4]; //key : color ; number of cards remaining in my hand per color
        Uint _longe[4]; //key : color ; score for the longe (number of cards in the color)

        //Player dependent
        Card_Height _greatest[4];  //key : color -> stock the height of the greatest cards I have per color
        Card_Height _smallest[4];  //key : color -> stock the height of the smallest cards I have per color
        bool _IAmMaster[4]; //key : color ; am i the master in the color
        bool _HaveTen[4]; //key : color ; have I the TEN/NINE (at trump) in the color
        bool _protectPoints[4]; //key : color; do I have to protect some important point (TEN, NINE for example) at the color
        Player_ID _posPlayer;
		const std::list<TypeOfCard>* _pHand;

		WrapperMethod<TypeOfCard, std::is_pointer<TypeOfCard>::value> _wrapperCallMethod;

    public:
		AIGameMemory(const Player_ID& posPlayer, std::list<TypeOfCard>* pHand) :
            _posPlayer(posPlayer),
            _pHand(pHand)
            {
                InitEverything();
            }
		virtual ~AIGameMemory(){}
        const Player_ID& ID() {return _posPlayer;}
        void UpdateFullTrick(const TrickBasic_Memory& trick,const Position_Trick& posTrick); //posTrick : the position of the player in the trick
        void InitEverything(); //call after the trump have been chosen, to set everything :-)
        Card_Height Master(const Card_Color& color) const;

        //bool CanHaveCard(const Player_ID& player,const Card_Color& color, const Card_Height& height) const;
        bool Cut(const Player_ID& player,const Card_Color& color) const; //see *CallCut for more information
        bool Call(const Player_ID& player,const Card_Color& color) const;

        bool OpponentsCut(const Card_Color& color) const;
        bool TeammateCut(const Card_Color& color) const;
        bool OpponentsCall(const Card_Color& color) const;
        bool TeammateCall(const Card_Color& color) const;


        bool NextCut(const Card_Color& color) const; //see *CallCut for more information
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

        bool CanReceiveCard(const Player_ID& id, const Card_Color& col, const Card_Height& height); //true if it is possible that the player have this card in the hand

        virtual bool SetCannotHaveCard(const Player_ID& player,const Card_Color& col, const Card_Height& height) {return false;} //return false because nothing is done
    protected:
        //Card_Height heightUnder(const Card_Height& height,bool color);
        void computeNewHeightMaster(); //also update _playerCut
        void updateEverythingElse(const Player_ID& firstToPlay); //update everything that need to go through the hand of the player.

        virtual bool callCut(const Player_ID& player,const Card_Color& color,const MemorizeCutsCalls& StoreCallCut) const; //dot the 'player' call/cut at the 'color'
        virtual bool stillHaveCards(const Player_ID& player,const Card_Color& color) const {return true;}
        bool opponentsCallCut(const Card_Color& color,const MemorizeCutsCalls& StoreCallCut) const; //do my opponents call/cut at the 'color'
        bool teammateCallCut(const Card_Color& color,const MemorizeCutsCalls& StoreCallCut) const; //do my  teammate call/cut at the 'color'
        bool nextCallCut(const Card_Color& color,const MemorizeCutsCalls& StoreCallCut) const; //do the player who play after me call/cut at the 'color'
		void computeScoreLongeAndProtectPoint(const std::list<const TypeOfCard>& cardsInTheColor, Uint& scoreLonge, bool& protectPoints, bool iAmMasterColor, const Card_Height& greatest); //always per color

        virtual void updateSmarter(const TrickBasic_Memory& trick, const Position_Trick& posTrick){}
        virtual bool canHave(const Player_ID& player,const Card_Color& col, const Card_Height& height) const {return true;}
        virtual void initHeritage(){}
		void updatePlayerRelativeAttributes();
    private:
};


template<class TypeOfCard>
void AIGameMemory<TypeOfCard>::UpdateFullTrick(const TrickBasic_Memory& trick, const Position_Trick& posTrick)
{
	Uint i;
	Card_Color colorAsked = trick.ColorAsked();
	Uint intCol = colorAsked.ToInt();
	_nbColorPlayed[intCol] += 1;
	Card_Color currentColor;
	Card_Height currentHeight;
	Player_ID currentPlayer = _infos.FirstToPlay(posTrick, _posPlayer);
	bool modify = false;
	for (i = 0; i < 4; i++)
	{
		currentColor = trick[i]->GetColour();
		currentHeight = trick[i]->GetHeight();
		intCol = currentColor.ToInt();
		//update the fallen Cards
		_fallenCards.SetFallen(currentColor, currentHeight);
		//in this case we need to update the height master
		if (currentHeight == _heightsMaster[intCol]) modify = true;
	}
	//update the masters cards if need to be done
	if (modify) computeNewHeightMaster();

	//update who cuts where
	for (i = 1; i < 4; i++)
	{
		currentPlayer.Next();
		currentColor = trick[i]->GetColour();
		if (currentColor != colorAsked)
		{
			_playerCut.SetCut(currentPlayer, colorAsked);
			if (trick[i]->Value() <= 4) _playerCalls.SetCut(currentPlayer, currentColor); //we don't call with TEN, NINE or ACE ...
			//maybe a smarter option here
		}
	}
	//update smarter memory (for derived class)
	updateSmarter(trick, posTrick);
	//update everything else
	updateEverythingElse(_infos.FirstToPlay(posTrick, _posPlayer));

}

template<class TypeOfCard>
void AIGameMemory<TypeOfCard>::InitEverything()
{
	_fallenCards.Reset();
	_playerCalls.Reset();
	_playerCut.Reset();
	for (int i = 0; i < 4; ++i)
	{
		_nbColorPlayed[i] = 0;
		Card_Color col(i);
		_heightsMaster[i] = (col == _infos.TrumpColor() ? JACK : ACE);
	}
	updateEverythingElse(_posPlayer);
	initHeritage();
}

template<class TypeOfCard>
void AIGameMemory<TypeOfCard>::computeNewHeightMaster()
{
	for (Uint i = 0; i < 4; i++)
	{
		Card_Color currentColor(i);
		bool isTrump = (currentColor == _infos.TrumpColor());
		Card_Height currentMaster = _heightsMaster[i];
		Card_Height seven(SEVEN);
		while (_fallenCards.IsFallen(currentColor, currentMaster) && currentMaster != seven)
		{
			currentMaster = currentMaster.HeightUnder(isTrump);
		}
		_heightsMaster[i] = currentMaster;
	}
}

template<class TypeOfCard>
void AIGameMemory<TypeOfCard>::updateEverythingElse(const Player_ID& firstToPlay)
{
	//INITIALIZE THE ARRAYS
	for (Uint i = 0; i < 4; i++)
	{
		_IAmMaster[i] = false;
		_HaveTen[i] = false;

		_protectPoints[i] = false;
		_longe[i] = 0;
	}

	//COMPUTE THE OTHER THINGS COLOR PER COLOR
	CARDS_COLOR colors[4] = { DIAMOND, HEART, SPADE, CLUB };
	Player_ID currentPlayer;
	Card_Color color;
	for (Uint i = 0; i < 4; i++)
	{
		color = Card_Color(colors[i]);
		Uint iCol = color.ToInt();
		Uint nbLeft = 8 - _fallenCards.ComputeFallen(color);
		if (_nbRemaining[iCol] == nbLeft)
		{
			currentPlayer = firstToPlay;
			for (Uint j = 1; j < 4; j++)
			{
				currentPlayer.Next();
				_playerCut.SetCut(currentPlayer, color);
			}
		}
		if (_greatest[iCol] == _heightsMaster[iCol])_IAmMaster[iCol] = true;
		for (Uint iP = 0; iP < 4; ++iP)
		{
			currentPlayer = Player_ID(iP);
			if (!stillHaveCards(currentPlayer, color)) _playerCut.SetCut(currentPlayer, color);
		}
	}

	//COMPUTE THE THINGS RELATIVE TO THE PLAYER (DEPENDENT OF ITS HAND)
	updatePlayerRelativeAttributes();
}

template<class TypeOfCard>
void AIGameMemory<TypeOfCard>::updatePlayerRelativeAttributes()
{
	//init the values
	std::array< std::list<const TypeOfCard>, 4 > cardsPerColor;
	for (Uint iColor = 0; iColor < 4; iColor++)
	{
		_nbRemaining[iColor] = 0;
		_greatest[iColor] = SEVEN;
		_smallest[iColor] = (iColor == _infos.TrumpColor().ToInt() ? JACK : ACE);

		_longe[iColor] = 0;
	}
	Card_Color color(_infos.TrumpColor());
	Card_Height height(UNINTIALIZED);
	Uint iColor = color.ToInt();
	_greatest[iColor] = JACK;

	//compute things directly from the hand
	for (const TypeOfCard pcard : *_pHand)
	{
		color = _wrapperCallMethod.callGetColour(pcard);
		height = _wrapperCallMethod.callGetHeight(pcard);
		iColor = color.ToInt();
		_nbRemaining[iColor] += 1;
		cardsPerColor[iColor].push_back(pcard);
		if (_wrapperCallMethod.callWin(pcard, _greatest[iColor])) _greatest[iColor] = height;
		if (!(_wrapperCallMethod.callWin(pcard, _smallest[iColor]))) _smallest[iColor] = height;
		if (height == (color == _infos.TrumpColor() ? Card_Height(NINE) : Card_Height(TEN))) _HaveTen[iColor] = true;
	}

	//update _longe and protectPoints by going through the hand color per color a second time
	for (Uint iCol = 0; iCol < 4; iCol++)
	{
		computeScoreLongeAndProtectPoint(cardsPerColor[iCol], _longe[iCol], _protectPoints[iCol], _IAmMaster[iCol], _greatest[iCol]);
	}
}
template<class TypeOfCard>
void AIGameMemory<TypeOfCard>::computeScoreLongeAndProtectPoint(const std::list<const TypeOfCard>& cardsInTheColor, Uint& scoreLonge, bool& protectPoints, bool iAmMasterColor, const Card_Height& greatest)
{
	//TO DO : we can perform some optimizations here !
	bool isTrump = false;
	scoreLonge = 0;
	protectPoints = false;
	for (auto pcard : cardsInTheColor)
	{
		if ((_wrapperCallMethod.callValue(pcard) >= 9) && (!iAmMasterColor)) protectPoints = true;
		if (!isTrump && (_wrapperCallMethod.callGetColour(pcard) == _infos.TrumpColor())) isTrump = true;
		continue;
	}
	if (iAmMasterColor)
	{
		Card_Height currentLonge(greatest);
		Card_Height tempHeight;
		Card_Color tempCol;
		Uint counter = 0;
		bool loopAgain = true;
		while (loopAgain && currentLonge.ToInt() > 0)
		{
			loopAgain = false;
			for (auto pcard : cardsInTheColor)
			{
				tempCol = _wrapperCallMethod.callGetColour(pcard);
				tempHeight = _wrapperCallMethod.callGetHeight(pcard);
				if ((tempHeight == currentLonge) || (_fallenCards.IsFallen(tempCol, tempHeight)))
				{
					counter++;
					currentLonge = currentLonge.HeightUnder(isTrump);
					loopAgain = true;
				}
			}
		}
		if (counter > 3) scoreLonge = 10;
		else if (counter > 2) scoreLonge = 5;
		else if (counter > 1) scoreLonge = 2;
		else scoreLonge = 0;
	}
}

template<class TypeOfCard>
bool AIGameMemory<TypeOfCard>::callCut(const Player_ID& player, const Card_Color& color, const MemorizeCutsCalls& StoreCallCut) const
{
	return StoreCallCut.Cut(player, color);
}

template<class TypeOfCard>
bool AIGameMemory<TypeOfCard>::opponentsCallCut(const Card_Color& color, const MemorizeCutsCalls& StoreCallCut) const
{
	Player_ID opp1(_posPlayer.NextPlayer());
	Player_ID opp2 = opp1.Teammate();
	return callCut(opp1.ToInt(), color, StoreCallCut) || callCut(opp2.ToInt(), color, StoreCallCut);
}

template<class TypeOfCard>
bool AIGameMemory<TypeOfCard>::teammateCallCut(const Card_Color& color, const MemorizeCutsCalls& StoreCallCut) const
{
	Player_ID teammate = _posPlayer.Teammate();
	return callCut(teammate, color, StoreCallCut);
}

template<class TypeOfCard>
bool AIGameMemory<TypeOfCard>::nextCallCut(const Card_Color& color, const MemorizeCutsCalls& StoreCallCut) const
{
	Player_ID nextPlayer(_posPlayer.NextPlayer());
	return callCut(nextPlayer, color, StoreCallCut);
}

template<class TypeOfCard>
bool AIGameMemory<TypeOfCard>::Cut(const Player_ID& player, const Card_Color& color) const //Do 'player' cut at 'color'
{
	return callCut(player, color, _playerCut);
}

template<class TypeOfCard>
bool AIGameMemory<TypeOfCard>::OpponentsCut(const Card_Color& color) const
{
	return opponentsCallCut(color, _playerCut);
}

template<class TypeOfCard>
bool AIGameMemory<TypeOfCard>::TeammateCut(const Card_Color& color) const
{
	return teammateCallCut(color, _playerCut);
}

template<class TypeOfCard>
bool AIGameMemory<TypeOfCard>::NextCut(const Card_Color& color) const
{
	return nextCallCut(color, _playerCut);
}

template<class TypeOfCard>
bool AIGameMemory<TypeOfCard>::Call(const Player_ID& player, const Card_Color& color) const //Do 'player' call at 'color'
{
	return callCut(player, color, _playerCalls);
}

template<class TypeOfCard>
bool AIGameMemory<TypeOfCard>::OpponentsCall(const Card_Color& color) const
{
	return opponentsCallCut(color, _playerCalls);
}

template<class TypeOfCard>
bool AIGameMemory<TypeOfCard>::TeammateCall(const Card_Color& color) const
{
	return teammateCallCut(color, _playerCalls);
}

template<class TypeOfCard>
bool AIGameMemory<TypeOfCard>::NextCall(const Card_Color& color) const
{
	return nextCallCut(color, _playerCalls);
}

template<class TypeOfCard>
Card_Height AIGameMemory<TypeOfCard>::Master(const Card_Color& color) const
{
	return(_heightsMaster[color.ToInt()]);
}

template<class TypeOfCard>
Uint AIGameMemory<TypeOfCard>::NbFallen(const Card_Color& color) const
{
	Uint nbFallen = _fallenCards.ComputeFallen(color);
	return static_cast<Uint>(8 - nbFallen);
}

template<class TypeOfCard>
Uint AIGameMemory<TypeOfCard>::ScoreLonge(const Card_Color& color) const
{
	Uint iColor = color.ToInt();
	return _longe[iColor];
}

template<class TypeOfCard>
bool AIGameMemory<TypeOfCard>::AllRemainingCards(const Card_Color& color) const
{
	Uint iColor = color.ToInt();
	Uint nbFallen = _fallenCards.ComputeFallen(color);
	return nbFallen + _nbRemaining[iColor] == 8;
}

template<class TypeOfCard>
bool AIGameMemory<TypeOfCard>::AmIMaster(const Card_Color& color) const //Have I the strongest card in the color
{
	Uint iColor = color.ToInt();
	return _IAmMaster[iColor];
}

template<class TypeOfCard>
bool AIGameMemory<TypeOfCard>::DoMyTeamTook() const
{
	Player_ID teammate = _posPlayer.Teammate();
	return (_posPlayer == _infos.Taker()) || (teammate == _infos.Taker());
}

template<class TypeOfCard>
bool AIGameMemory<TypeOfCard>::CardsFallen(const Card_Color& color, const Card_Height& height) const
{
	return _fallenCards.IsFallen(color, height);
}

template<class TypeOfCard>
bool AIGameMemory<TypeOfCard>::HaveTen(const Card_Color& color) const
{
	Uint iColor = color.ToInt();
	return _HaveTen[iColor];
}

template<class TypeOfCard>
bool AIGameMemory<TypeOfCard>::ProtectPoints(const Card_Color& color) const
{
	Uint iColor = color.ToInt();
	return _protectPoints[iColor];
}

template<class TypeOfCard>
Uint AIGameMemory<TypeOfCard>::NbColorPlayed(const Card_Color& color) const //number of time each color have been played (first)
{
	Uint iColor = color.ToInt();
	return _nbColorPlayed[iColor];
}

template<class TypeOfCard>
Card_Height AIGameMemory<TypeOfCard>::Greatest(const Card_Color& color) const //return the greatest cards I have in the color
{
	Uint iColor = color.ToInt();
	return _greatest[iColor];
}

template<class TypeOfCard>
Card_Height AIGameMemory<TypeOfCard>::Smallest(const Card_Color& color) const //return the smallest cards I have in the color
{
	Uint iColor = color.ToInt();
	return _smallest[iColor];
}

template<class TypeOfCard>
bool AIGameMemory<TypeOfCard>::CanReceiveCard(const Player_ID& id, const Card_Color& col, const Card_Height& height)
{
	return !callCut(id, col, _playerCut) && canHave(id, col, height);
}
#endif // AIGAMEMEMORY_H
