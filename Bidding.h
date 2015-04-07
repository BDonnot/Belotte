#ifndef BIDDING_H
#define BIDDING_H

#include<array>
#include<list>

#include "Basic_Game_Info.h"
#include "Player.h"
#include "Cards_Deck.h"
#include "BetsMemory.h"
class Bidding
{
    protected :
        Basic_Game_Info _infos;
        std::array<Player*,4> _players;
        BetsMemory _bets;
        SDL_Event* _pEvent;
        SDL_Surface* _pScreen;
        Uint _previousPlayerPassed;
        Images_Text _textAfterBet0;
        Images_Text _textAfterBet1;
        Images_Button _ok;
        Cards_Deck* _pDeck;
        PLAYER_ID _bidder;

    public:
        Bidding(){}
        Bidding(const std::array<Player*,4>& players,SDL_Event* pevent,SDL_Surface* screen,Cards_Deck* pDeck);
        virtual ~Bidding(){}
        bool Bid(GAME_PHASES currentPhase);
        void SetPlayers(const std::array<Player*,4>& players);
        void UpdateEvent(GAME_PHASES currentPhase);
        void Display(GAME_PHASES currentPhase);
        bool Click(bool Short);
        void SummarizeBet();
        GAME_PHASES NextPhase();

        void Reset(); //reset the previous bets

        std::string GetString(); //not const because it erase the bets made in _bets.
    protected:
        void handleBet(const Player_Bid& bid, PLAYER_ID ibet);
        void handleText();
    private:
};

#endif // BIDDING_H
