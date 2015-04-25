#ifndef CARDSGETMETHOD_H_INCLUDED
#define CARDSGETMETHOD_H_INCLUDED
template<class TypeOfCard,bool isPointer>
class WrapperMethod
{
public :
    const Card_Color& callGetColour(const TypeOfCard& card);
    const Card_Height& callGetHeight(const TypeOfCard& card);
    bool callWin(const TypeOfCard& card,const Card_Height& heightOther);
    Uint callValue(const TypeOfCard& card);
};

template<class TypeOfCard>
class WrapperMethod<TypeOfCard,true>
{
public :
    const Card_Color& callGetColour(const TypeOfCard& card);
    const Card_Height& callGetHeight(const TypeOfCard& card);
    bool callWin(const TypeOfCard& card,const Card_Height& heightOther);
    Uint callValue(const TypeOfCard& card);
};
template<class TypeOfCard>
const Card_Color& WrapperMethod<TypeOfCard,true>::callGetColour(const TypeOfCard& card)
{
    return card->GetColour();
}
template<class TypeOfCard>
const Card_Height& WrapperMethod<TypeOfCard,true>::callGetHeight(const TypeOfCard& card)
{
    return card->GetHeight();
}
template<class TypeOfCard>
bool WrapperMethod<TypeOfCard,true>::callWin(const TypeOfCard& card,const Card_Height& heightOther)
{
    return card->Win(heightOther);
}
template<class TypeOfCard>
Uint WrapperMethod<TypeOfCard,true>::callValue(const TypeOfCard& card)
{
    return card->Value();
}

template<class TypeOfCard>
class WrapperMethod<TypeOfCard,false>
{
public :
    const Card_Color& callGetColour(const TypeOfCard& card);
    const Card_Height& callGetHeight(const TypeOfCard& card);
    bool callWin(const TypeOfCard& card,const Card_Height& heightOther);
    Uint callValue(const TypeOfCard& card);
};
template<class TypeOfCard>
const Card_Color& WrapperMethod<TypeOfCard,false>::callGetColour(const TypeOfCard& card)
{
    return card.GetColour();
}
template<class TypeOfCard>
const Card_Height& WrapperMethod<TypeOfCard,false>::callGetHeight(const TypeOfCard& card)
{
    return card.GetHeight();
}
template<class TypeOfCard>
bool WrapperMethod<TypeOfCard,false>::callWin(const TypeOfCard& card,const Card_Height& heightOther)
{
    return card.Win(heightOther);
}
template<class TypeOfCard>
Uint WrapperMethod<TypeOfCard,false>::callValue(const TypeOfCard& card)
{
    return card.Value();
}


#endif // CARDSGETMETHOD_H_INCLUDED
