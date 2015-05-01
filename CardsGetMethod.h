#ifndef CARDSGETMETHOD_H_INCLUDED
#define CARDSGETMETHOD_H_INCLUDED

/**
** These wrapper define properly the way to access some methods of
** class Cards/Card_Basic, depending on the fact
** it is pointer or not
** access through '.' or '->'
**/

template<class TypeOfCard,bool isPointer>
class WrapperMethod {};

template<class TypeOfCard>
class WrapperMethod<TypeOfCard,true>
{
public :
	const Card_Color& callGetColour(const TypeOfCard& card){ return card->GetColour(); }
	const Card_Height& callGetHeight(const TypeOfCard& card){ return card->GetHeight(); }
	bool callWin(const TypeOfCard& card, const Card_Height& heightOther) { return card->Win(heightOther); }
	Uint callValue(const TypeOfCard& card){ return card->Value(); }
};

template<class TypeOfCard>
class WrapperMethod<TypeOfCard,false>
{
public :
	const Card_Color& callGetColour(const TypeOfCard& card){ return card.GetColour(); }
	const Card_Height& callGetHeight(const TypeOfCard& card){ return card.GetHeight(); }
	bool callWin(const TypeOfCard& card, const Card_Height& heightOther) { return card.Win(heightOther); }
	Uint callValue(const TypeOfCard& card){ return card.Value(); }
};


#endif // CARDSGETMETHOD_H_INCLUDED
