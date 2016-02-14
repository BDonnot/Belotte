#ifndef Cards_DECK_H
#define Cards_DECK_H

/**
** This class represent the deck of the cards.
** This is particularly useful at the beginning
** while giving, cutting / shuffling all the cards etc.
**/

#include <list>
#include <array>
#include <algorithm>

#include "Cards.h"
#include "Player.h"
#include "Foncteurs_Cards.h"
#include "Random.h"

class Cards_Deck
{
    protected:
        std::list<Cards*> _pile;
        bool _first;
        Basic_Game_Info _info;
        Random _rand;
        Random_Decreasing _randDecreasing;
		SDL::DisplayCardPile _DisplayCardPile;
		SDL::PositionGraphic _screenCenter;

    public:
        Cards_Deck():_rand(3,28),_randDecreasing(31){}
#if COMPLETE_GAME > 0
        Cards_Deck(SDL_Event* event,SDL_Surface* screen,Uint16 screenWidth, Uint16 screenHeight);
#else
		Cards_Deck(Uint16 screenWidth, Uint16 screenHeight);
#endif //COMPLETE_GAME > 0
        virtual ~Cards_Deck();
        void BeginGame();
        void GiveCards(const std::list<Player*>& li_players,bool first_round,unsigned int i_taker);
        void GiveCards(const std::list<Player*>& li_players);
        void GiveCards(const std::array<Player*,4>& li_players);
        void Display(GAME_PHASES currentPhase);

        const bool FirstGame();
        const bool Click(GAME_PHASES currentPhase);
        void UpdateEvent(GAME_PHASES currentPhase);
        void Reset();
        void ReceiveCardsBack(); //call once the game is over and no one took

        const std::list<Cards*>& GetPile() const {return _pile;}
        std::list<Cards*>& GetPile() {return _pile;}

        void GetCardBack(Cards*& pCard);

    private:
        void Cut();
        void Shuffle();
};

#endif // CARDS_DECK_H
