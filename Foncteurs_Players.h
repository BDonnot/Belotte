#ifndef FONCTEURS_PLAYERS_H_INCLUDED
#define FONCTEURS_PLAYERS_H_INCLUDED

#include<list>

#include "Cards.h"
#include "Cards_Basic.h"

#include "TrickBasic_Memory.h"
#include "TrickStatus.h"

#include "CardsGetMethod.h"

template<class TypeOfCard>
class CanPlayCards
{
protected :
    const TrickBasic_Memory* _pTrick;
    const TrickStatus* _pStatus;
    typename std::list<TypeOfCard> * _hand;
    WrapperMethod<TypeOfCard,std::is_pointer<TypeOfCard>::value> _wrapperCallMethod;

public :
    CanPlayCards(){}
    ~CanPlayCards(){}
    void Init(const TrickBasic_Memory& trick,const TrickStatus& status,std::list<TypeOfCard> & hand)
    {
        _pTrick = &trick;
        _pStatus = &status;
        _hand = &hand;
    }
    bool operator()(const TypeOfCard& PmyCard)
    {
        /*if (PmyCard==nullptr)
            return false;*/
        Card_Color my_colour = _wrapperCallMethod.callGetColour(PmyCard);
        Card_Height maxHeight = _pTrick->HeightMaster();
        Card_Color maxColor = _pTrick->ColorMaster();
        if (_pStatus->HasCol()) //If we have the color asked we have to played it
        {
            if(_pTrick->ColorAsked() != _pStatus->TrumpColor()) //we must play in the color if we can
                return (my_colour == _pTrick->ColorAsked());
            return (my_colour == _pStatus->TrumpColor())&&((_wrapperCallMethod.callWin(PmyCard,maxHeight))
                                                             ||(!has_higher(_pStatus->TrumpColor(),maxHeight)));
        }
        //So I do not have the color
        if ((!_pStatus->HasTrump())||(static_cast<Uint>(_pTrick->CurrentWinner().ToInt()%2) == static_cast<Uint>(_pTrick->NumberCardsPlayed()%2))) return true; //I can play what I want if I dont have the color asked and : I dont have trump or my partner is the master
        //So I do have trump, my partner is no master, and I do not have the color asked.
        bool trumpPlayed = maxColor == _pStatus->TrumpColor();
        if(trumpPlayed)
            return (my_colour == _pStatus->TrumpColor())&&((_wrapperCallMethod.callWin(PmyCard,maxHeight))
                                                             ||(!has_higher(_pStatus->TrumpColor(),maxHeight)));
        return my_colour == _pStatus->TrumpColor();
    }
protected :
        bool has_higher(const Card_Color&  color_asked,const Card_Height& max_height)
        {
            for (auto pcard : *_hand)
            {
                if ((_wrapperCallMethod.callGetColour(pcard)==color_asked)&&( _wrapperCallMethod.callWin(pcard,max_height)))
                    return true;
            }
            return false;
        }
        bool has_colour(const Card_Color& color)
        {
            for (auto pcard : *_hand)
            {
                if (_wrapperCallMethod.callGetColour(pcard) == color)
                    return true;
            }
            return false;
        }
};

#endif // FONCTEURS_PLAYERS_H_INCLUDED
