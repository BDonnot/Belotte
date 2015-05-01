#ifndef MEMORYHELPERS_H_INCLUDED
#define MEMORYHELPERS_H_INCLUDED

/**
** These class are here to store in an efficient manner
** booleans informations, like for example
** which card have been played, or if a specific has still
** a color.
**/

template<Uint n>
class BoolStorage
{
    ///n : number of boolean stored
    ///by default it is false
protected:
	Basic_Game_Info _infos;
	bool _information[n];
public:
	BoolStorage(){ Reset(); }
	virtual ~BoolStorage(){}
	void Reset()
	{
		for (Uint i = 0; i < n; ++i)
		{
			_information[i] = false;
		}
	}
protected:
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
public:
	MemorizeCards(){}
	~MemorizeCards(){}
	const bool & IsFallen(Card_Color color, Card_Height height) const
	{
		Uint iColor = color.ToInt();
		Uint iHeight = height.ToInt();
		return getInfo(iColor * 8 + iHeight);
	}
	void SetFallen(Card_Color color, Card_Height height)
	{
		Uint iColor = color.ToInt();
		Uint iHeight = height.ToInt();
		setInformation(iColor * 8 + iHeight);
	}
	Uint ComputeFallen(Card_Color color) const //compute the number of cards fallen in a specific color
	{
		Uint iColor = color.ToInt();
		Uint res = 0;
		for (Uint j = 0; j < 8; j++)
		{
			if (_information[iColor * 8 + j]) ++res;
		}
		return res;
	}
};

class MemorizeCutsCalls : public BoolStorage<16> //efficient way to memorize which player cut/called where
{
public:
	MemorizeCutsCalls(){}
	~MemorizeCutsCalls(){}
	const bool & Cut(const Player_ID& player, const Card_Color& color) const
	{
		Uint iPlayer = player.ToInt();
		Uint iColor = color.ToInt();
		return getInfo(iPlayer * 4 + iColor);
	}
	void SetCut(const Player_ID& player, const Card_Color& color)
	{
		Uint iPlayer = player.ToInt();
		Uint iColor = color.ToInt();
		setInformation(iPlayer * 4 + iColor);
	}
};

class MemorizePlayersCards : public BoolStorage<128>
{
	/**
	**Beware, the order is reverse from bool storage :
	** I can receive a card <=> BoolStorage[index] = false !!!
	**/
public:
	MemorizePlayersCards(){}
	~MemorizePlayersCards(){}
	bool CanHaveCard(const Player_ID& player, const Card_Color& color, const Card_Height& height) const
	{
		Uint iPlayer = player.ToInt();
		Uint iColor = color.ToInt();
		Uint iHei = height.ToInt();
		return !getInfo(iPlayer * 32 + iColor * 8 + iHei);
	}
	void SetCannotHaveCard(const Player_ID& player, const Card_Color& color, const Card_Height& height)
	{
		Uint iPlayer = player.ToInt();
		Uint iColor = color.ToInt();
		Uint iHei = height.ToInt();
		setInformation(iPlayer * 32 + iColor * 8 + iHei);
	}
	void SetCanHaveCard(const Player_ID& player, const Card_Color& color, const Card_Height& height)
	{
		Uint iPlayer = player.ToInt();
		Uint iColor = color.ToInt();
		Uint iHei = height.ToInt();
		_information[iPlayer * 32 + iColor * 8 + iHei] = false;
		//setInformation(iPlayer * 32 + iColor * 8 + iHei);
	}
	void SetFallen(const Cards & card)
	{
		/**
		**When a card is fallen no one can have it
		**/
		Uint iColor = card.GetColour().ToInt();
		Uint iHei = card.GetHeight().ToInt();
		for (Uint iPlayer = 0; iPlayer < 4; ++iPlayer)
		{
			setInformation(iPlayer * 32 + iColor * 8 + iHei);
		}
	}
	void SetCutColor(const Player_ID& player, const Card_Color& color)
	{
		/**
		**When a player cut at one color, it means he does not have this color anymore
		**/
		Uint iPlayer = player.ToInt();
		Uint iColor = color.ToInt();
		for (Uint iHei = 0; iHei < 8; ++iHei)
		{
			setInformation(iPlayer * 32 + iColor * 8 + iHei);
		}
	}
};
#endif // MEMORYHELPERS_H_INCLUDED
