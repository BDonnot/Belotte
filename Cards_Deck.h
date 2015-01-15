#ifndef Cards_DECK_H
#define Cards_DECK_H

#include <list>
#include <array>
#include <algorithm>

#include "Cards.h"
#include "Player.h"
//#include <boost/interprocess/smart_ptr/shared_ptr.hpp>

#include "Foncteurs_Cards.h"
#include "Random.h"

//typedef boost::shared_ptr<Cards> sPtr;

class Cards_Deck
{
    protected:
        std::list<Cards*> _pile;
        bool _first;
        DisplayCardPile _DisplayCardPile;
        Basic_Game_Info _info;
        PositionGraphic _screenCenter;
        Random _rand;
        Random_Decreasing _randDecreasing;

    public:
        Cards_Deck();
        Cards_Deck(SDL_Event* event,SDL_Surface* screen,Uint16 screenWidth, Uint16 screenHeight);
        virtual ~Cards_Deck();
        void BeginGame();
        void GiveCards(const std::list<Player*>& li_players,bool first_round,unsigned int i_taker);
        void GiveCards(const std::list<Player*>& li_players);
        void GiveCards(const std::array<Player*,4>& li_players);
        void Display(GAME_PHASES currentPhase);
        //void Update(Uint32 current_time);
        const bool FirstGame();
        const bool Click(GAME_PHASES currentPhase);
        void UpdateEvent(GAME_PHASES currentPhase);
        void Reset();

        const std::list<Cards*>& GetPile() const;
        std::list<Cards*>& GetPile();

        void GetCardBack(Cards*& pCard);

    private:
        void Cut();
        void Shuffle();
};

#endif // CARDS_DECK_H
